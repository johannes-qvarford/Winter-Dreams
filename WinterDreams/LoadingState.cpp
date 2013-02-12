#include "LoadingState.h"
#include "SubLevel.h"
#include "LevelState.h"
#include "StateManager.h"
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

static void loadSubLevel(const std::string& subLevelName, LevelState* levelState_p) {
	using namespace boost::property_tree;
	
	//get managers
	auto& propMgr = PropertyManager::get();
	auto& resMgr = ResourceManager::get();
	auto& objFact = ObjectFactory::get();
	auto tilesToObjects = std::map<int, std::string>();
	
	//read sublevel from json
	auto& levelData = ptree();
	json_parser::read_json(FS_DIR_LEVELS + subLevelName, levelData);

	//connect sublevel with level
	auto subLevel_sp = std::shared_ptr<SubLevel>(new SubLevel(levelState_p));
	levelState_p->addSubLevel(subLevelName, subLevel_sp);

	{
		//use properties 'height' from levelData. And 'background' and 'maplayer' from levelProperties
		//to place background and map layer correctly in SubLevel.
		auto& properties = levelData.get_child("properties");

//		auto& bgFilename = level.get<std::string>(NAME_LEVELSETTINGS_BACKGROUND);
		auto map = properties.get_child(NAME_LEVELSETTINGS_MAPLAYER);
		properties.get_value<std::string>();
		auto& mlFilename = properties.get<std::string>(NAME_LEVELSETTINGS_MAPLAYER);
		
//		auto bgTexture_sp = resMgr.getTexture(bgFilename);
		auto mlTexture_sp = resMgr.getTexture(FS_DIR_MAPS + mlFilename);

		auto yTiles = levelData.get<int>("height");

		auto yLength = yTiles * Y_STEP;

		//need these offsets, don't know why
		//may need to change when switching level.
		const float X_OFFSET = -45.f;
		const float Y_OFFSET = -17.f;
		auto mlOffset = sf::Vector2f((cosf(22.5f) * yLength) + X_OFFSET, Y_OFFSET); 

		subLevel_sp->setMapTexture(mlTexture_sp, mlOffset);
//		mLoadedLevel->setBackgroundTexture(bgTexture_sp, sf::Vector2f(0, 0));
	}

	
	{
		//check tilesets, to map gids to objectnames
		auto tilesets = levelData.get_child("tilesets");
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
		auto& layers = levelData.get_child("layers");

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

					objFact.callCallback(objectType, subLevel_sp.get(), position, object);
				}
			}
			else {
				//a tile layer
				auto& data = layer.get_child("data");
				auto width = levelData.get<int>("width");
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

					objFact.callCallback(objectType, subLevel_sp.get(), position, emptyPt);
				}
			}
		}
	}

}

static void loadLevel(const std::string& levelName, LevelState* level_p) {
	//look in settings for which sublevels to load.
	auto& settings = PropertyManager::get().getGeneralSettings();
	auto& levels = settings.get_child("levels");
	auto& level = levels.get_child(levelName);
	auto& sublevels = level.get_child("sublevels");

	for(auto it = sublevels.begin(), end = sublevels.end(); it != end; ++it) {
		//load sublevels one at a time.
		auto& entry = it->second;
		auto& subLevelName = entry.get_value<std::string>();
		loadSubLevel(subLevelName, level_p);
	}
	level_p->switchSubLevel(level.get<std::string>("first_sublevel_name"));

	StateManager::get().popState();
}

LoadingState::LoadingState(std::string levelName, LevelState* level_p)
{
	using namespace boost::property_tree;
	loadLevel(levelName, level_p);
}

LoadingState::~LoadingState() {
}

void LoadingState::update() {
	using namespace boost::property_tree;



}