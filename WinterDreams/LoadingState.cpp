#include "LoadingState.h"
#include "GameState.h"
#include "StateManager.h"
#include "Wall.h"
#include "ResourceManager.h"
#include "PropertyManager.h"
#include "ObjectFactory.h"
#include "GameToScreen.h"
#include "FileStructure.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/optional/optional.hpp>
#include <SFML/System/Vector2.hpp>
#include <map>
#include <string>

static const std::string NAME_PROPERTIES_IGNORE = "ignore";
static const std::string NAME_LEVELSETTINGS_BACKGROUND = "background";
static const std::string NAME_LEVELSETTINGS_MAPLAYER = "map";

LoadingState::LoadingState(GameState* gameState_p, std::string filepath):
	mLoadedLevel(gameState_p),
	mLevelData()
{
	using namespace boost::property_tree;
	json_parser::read_json(filepath, mLevelData);
}

LoadingState::~LoadingState() {
}

void LoadingState::update() {
	using namespace boost::property_tree;


	/*
		get background filename from propMgr.levelSettings.'levelname'.background.
		get map layer filename from propMgr.levelSettings.'levelname'.maplayer

		get map offset by using the levels y length
		place maptexture at offset in GameState
		place backgroundtexure at (0,0) in GameState

		Check all tilesets...
			save firstgid as property firstgid
			Check tileproperties...
				if has objectname property
					map objectname to firstgid + tile

		Check all layers...
			if not has use property
				continue
			if has type="objectgroup"
				Check all objects...
					call factory with objectname, constructed position and tree
			else
				Check all tiles...
					save objectname as objectname mapped to gid
					call factory with objectname, constructed position and empty tree

	*/
	auto& propMgr = PropertyManager::get();
	auto& resMgr = ResourceManager::get();
	auto& objFact = ObjectFactory::get();
	auto tilesToObjects = std::map<int, std::string>();

	{
		//use properties 'height' from levelData. And 'background' and 'maplayer' from levelProperties
		//to place background and map layer correctly in GameState.
		auto& properties = mLevelData.get_child("properties");

//		auto& bgFilename = level.get<std::string>(NAME_LEVELSETTINGS_BACKGROUND);
		auto& mlFilename = properties.get<std::string>(NAME_LEVELSETTINGS_MAPLAYER);
		
//		auto bgTexture_sp = resMgr.getTexture(bgFilename);
		auto mlTexture_sp = resMgr.getTexture(FS_DIR_MAPS + mlFilename);

		auto yTiles = mLevelData.get<int>("height");

		auto yLength = yTiles * Y_STEP;

		//need these offsets, don't know why
		//may need to change when switching level.
		const float X_OFFSET = -45.f;
		const float Y_OFFSET = -17.f;
		auto mlOffset = sf::Vector2f((cosf(22.5f) * yLength) + X_OFFSET, Y_OFFSET); 


		mLoadedLevel->setMapTexture(mlTexture_sp, mlOffset);
//		mLoadedLevel->setBackgroundTexture(bgTexture_sp, sf::Vector2f(0, 0));
	}

	
	{
		//check tilesets, to map gids to objectnames
		auto tilesets = mLevelData.get_child("tilesets");
		for(auto it = tilesets.begin(), end = tilesets.end(); it != end; ++it) {
			//look for first gid in tileset
			auto& tileset = it->second;
			auto firstgid = tileset.get<int>("firstgid");
			
			auto& tileproperties_optional = tileset.get_child_optional("tileproperties");
			if(!tileproperties_optional)
				continue;

			auto& tileproperties = tileproperties_optional.get();


			for(auto tpit = tileproperties.begin(), tpend = tileproperties.end(); tpit != tpend; ++tpit) {
				auto& name = tpit->first;
				auto& prop = tpit->second;

				//does this tilenumber have a objectname?
				int index = std::stoi(name);
				auto objectname = prop.get<std::string>("objectname","");
				if(objectname == "")
					continue;

				//map index + firstgid to objectname
				tilesToObjects.insert(std::pair<int, std::string>(index + firstgid, objectname));
			}
		}
	}

	{
		auto& layers = mLevelData.get_child("layers");

		for(auto it = layers.begin(), end = layers.end(); it != end; ++it) {
			auto& layer = it->second;
			auto& ignore = layer.get<std::string>("properties." + NAME_PROPERTIES_IGNORE,"");

			if(ignore == "true")
				continue;

			auto& type = layer.get<std::string>("type");

			if( type == "objectgroup") {
				//an object layer
				auto& objects = layer.get_child("objects");

				//iterate over objects
				for(auto ogit = objects.begin(), ogend = objects.end(); ogit != ogend; ++ogit) {
					auto& object = ogit->second;

					auto objectType = object.get<std::string>("type","");
					if(objectType == "")
						continue;

					//an objects position is (x,y) * 32 / STEP in gamecoordinates
					auto x = object.get<int>("x");
					auto y = object.get<int>("y");
					auto position = sf::Vector2f(x * X_STEP / 32, y * Y_STEP / 32);

					objFact.callCallback(objectType, mLoadedLevel, position, object);
				}
			}
			else {
				//a tile layer
				auto& data = layer.get_child("data");
				auto width = mLevelData.get<int>("width");
				auto index = 0;
				//tiles doesn't have any properties
				auto emptyPt = boost::property_tree::ptree();

				//iterate over tiledata
				for(auto dait = data.begin(), daend = data.end(); dait != daend; ++dait, ++index) {
					auto& node = dait->second;
					auto gid = node.get_value<int>();

					//ignore tiles we don't recognize
					auto ttoit = tilesToObjects.find(gid);
					if(ttoit == tilesToObjects.end())
						continue;

					auto& objectType = ttoit->second;
					//a tiles x position is its (index % width) * X_STEP
					//a tiles y position is its (index / width) * Y_STEP
					auto x = index % width;
					auto y = index / width;
					auto position = sf::Vector2f(x * X_STEP, y * Y_STEP);

					objFact.callCallback(objectType, mLoadedLevel, position, emptyPt);
				}
			}
		}
	}

	StateManager::get().popState();
}