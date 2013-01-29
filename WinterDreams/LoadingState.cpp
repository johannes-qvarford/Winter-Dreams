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

/*
//tile ids of small, normal, and big collision tiles. 
static const int FGID_SMALL_COL = 1;
static const int FGID_MEDIUM_COL = 9;
static const int FGID_BIG_COL = 5;

void createSmallCol(const sf::Vector2f& position, GameState* gameState) {
	gameState->addPhysicalEntity(std::shared_ptr<PhysicalEntity>(new Wall(position.x, position.y, STEP, STEP)));
}
void createMediumCol(const sf::Vector2f& position, GameState* gameState) {
	gameState->addPhysicalEntity(std::shared_ptr<PhysicalEntity>(new Wall(position.x, position.y, STEP * 2, STEP * 2)));
}
void createBigCol(const sf::Vector2f& position, GameState* gameState) {
	gameState->addPhysicalEntity(std::shared_ptr<PhysicalEntity>(new Wall(position.x, position.y, STEP * 4, STEP * 4)));
}

typedef void (*CreateFunc)(const sf::Vector2f& position, GameState* gameState);
*/

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

void LoadingState::update(int milliseconds) {
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
		
//		auto bgTexture_p = resMgr.getTexture(bgFilename);
		auto mlTexture_p = resMgr.getTexture(FS_DIR_MAPS + mlFilename);

		auto yTiles = mLevelData.get<int>("height");

		auto yLength = yTiles * Y_STEP;
		auto mlOffset = sf::Vector2f(cosf(22.5f) * yLength, 0); 

		mLoadedLevel->setBackgroundTexture(mlTexture_p, mlOffset);
//		mLoadedLevel->setBackgroundTexture(bgTexture_p, sf::Vector2f(0, 0));
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
					auto& object = it->second;

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

/*
	//map tile ids to functions creating entities
//	std::map<int, CreateFunc> idToFunc;

//	idToFunc[FGID_SMALL_COL] = createSmallCol;
//	idToFunc[FGID_MEDIUM_COL] = createMediumCol;
//	idToFunc[FGID_BIG_COL] = createBigCol;

	auto layers = mLevelData.get_child("layers");
	int tilesPerRow = 0;

	//iterate over tile layers
	for(auto it = layers.begin(), end = layers.end(); it != end; ++it) {
		auto& layer = it->second;

		//check for the ignore property. if its "false", go to the next layer
		//if its not, or doens't exist, check layer for tiles.
		std::string ignore = layer.get<std::string>("properties.ignore","false");
		if(ignore == "true")
			continue;

		auto& data = layer.get_child("data");
		tilesPerRow = layer.get<int>("width");

		auto currentIndex = 0;

		for(auto tit = data.begin(), tend = data.end(); tit != tend; ++tit, ++currentIndex) {
			auto tileId = tit->second.get_value<int>();

			//find the correct create function based on the tileId
			auto func = idToFunc[tileId];
			
			//ignore unknown tile types
			if(func == nullptr)
				continue;

			//calculate x and y coordinates based on tileId, step length/width and tiles per row
			sf::Vector2f position((currentIndex % tilesPerRow) * STEP, (currentIndex / tilesPerRow) * STEP);

			func(position, mLoadedLevel);

			
		}
	}

	//add map layer...
	auto& resMgr = ResourceManager::get();

	//check properties...
	//calculate the smallest screen x coordinate.
	auto topEdgeToLeftEdgeLength = tilesPerRow * STEP;
	auto smallestScreenX = topEdgeToLeftEdgeLength * cosf(22.5f);


	
	//TODO: use map_layer image in properties
	//auto mapLayerFileName = mLevelData.get<std::string>("properties.mapLayerFile");
	auto mapLayerFileName = MAP_LAYER_FILENAME;

	mLoadedLevel->setMapTexture(

	//add backgrounds...
*/

	//were done!
	//destroy the loadingState, revealing the underlying state(the GameState)
	StateManager::get().popState();
}