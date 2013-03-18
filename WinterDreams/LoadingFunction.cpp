#include "LoadingFunction.h"
#include "SubLevel.h"
#include "LevelState.h"
#include "StateManager.h"
#include "GameToScreen.h"
#include "ResourceManager.h"
#include "PropertyManager.h"
#include "ObjectFactory.h"
#include "FileStructure.h"
#include "StateManager.h"
#include <map>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/optional/optional.hpp>

#include <iostream>

#include "Player.h"
#include "InventoryDisplay.h"
#include "Camera.h"

static const std::string NAME_PROPERTIES_IGNORE = "ignore";
static const std::string NAME_LEVELSETTINGS_BACKGROUND = "background";
static const std::string NAME_LEVELSETTINGS_MAPLAYER = "map";

LoadingSpecs::LoadingSpecs(const LoadingSpecs& l):
		mLoadedLevel_p( l.mLoadedLevel_p),
		mLevelName( l.mLevelName ),
		mRunMutex_p( l.mRunMutex_p ),
		mResourceMutex_p( l.mResourceMutex_p ),
		mRunning_p( l.mRunning_p ),
		mMainThreadRunning_p( l.mMainThreadRunning_p )
	{
	}

static void loadSubLevel(const std::string& subLevelName, LevelState* levelState_p, sf::Mutex* mutex);

void loadingFunc::loadLevel(LoadingSpecs& specs) {
	

	
	//look in settings for which sublevels to load.
	specs.mResourceMutex_p->lock();
	auto& settings = PropertyManager::get().getGeneralSettings();
	specs.mResourceMutex_p->unlock();
	auto& levels = settings.get_child("levels");
	auto& level = levels.get_child(specs.mLevelName);
	auto& sublevels = level.get_child("sublevels");

	for(auto it = sublevels.begin(), end = sublevels.end(); it != end; ++it) {
		//load sublevels one at a time.
		auto subLevelName = it->first;
		loadSubLevel(subLevelName, specs.mLoadedLevel_p, specs.mResourceMutex_p);
	}
	specs.mResourceMutex_p->lock();
	specs.mLoadedLevel_p->switchSubLevel(level.get<std::string>("first_sublevel_name"));
	specs.mResourceMutex_p->unlock();
	//now, add player, camera, and inventory display to sub levels.
	std::shared_ptr<Player> player_sp = specs.mLoadedLevel_p->getPlayer();
	std::shared_ptr<Camera> camera_sp = specs.mLoadedLevel_p->getCamera();
	std::shared_ptr<InventoryDisplay> display_sp = specs.mLoadedLevel_p->getInventoryDisplay();

	for(auto it = sublevels.begin(), end = sublevels.end(); it != end; ++it) {
		//iterate over all sublevels.
		auto subLevelName = it->first;
		auto subLevel_sp = specs.mLoadedLevel_p->getSubLevel(subLevelName);

		//add them to the sublevel.
		subLevel_sp->addGraphicalEntity(std::static_pointer_cast<GraphicalEntity>(player_sp));
		subLevel_sp->addScript(std::static_pointer_cast<Script>(camera_sp));
		subLevel_sp->addScript(std::static_pointer_cast<Script>(display_sp));
	}

	//we're done here!
	specs.mRunMutex_p->lock();
	*specs.mRunning_p = false;
	specs.mRunMutex_p->unlock();
	
	bool stop = false;
	while(stop == false) {
		sf::sleep(sf::milliseconds(10));
		specs.mRunMutex_p->lock();
		if(*specs.mMainThreadRunning_p == false)
			stop = true;
		specs.mRunMutex_p->unlock();
	}
}

static void loadSubLevel(const std::string& subLevelName, LevelState* levelState_p, sf::Mutex* mutex) {
	using namespace boost::property_tree;
	
	//get managers
	mutex->lock();
	auto& propMgr = PropertyManager::get();
	auto& resMgr = ResourceManager::get();
	auto& objFact = ObjectFactory::get();
	mutex->unlock();
	auto tilesToObjects = std::map<int, std::string>();
	
	//read sublevel from json

	auto subLevelFilename = propMgr.getGeneralSettings().get<std::string>("levels." + levelState_p->getLevelName() + ".sublevels." + subLevelName);
	auto levelData = ptree();
	json_parser::read_json(FS_DIR_LEVELS + subLevelFilename, levelData);

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
		auto mlFilename = properties.get<std::string>(NAME_LEVELSETTINGS_MAPLAYER);
		auto bkFilename = properties.get<std::string>("background");
		
//		auto bgTexture_sp = resMgr.getTexture(bgFilename);
		mutex->lock();		
		std::cout<< "Done \n";
		auto mlTexture_sp = resMgr.getTexture(FS_DIR_MAPS + mlFilename);
		mutex->unlock();
		mutex->lock();		
		std::cout<< "Done \n";
		auto bkTexture_sp = resMgr.getTexture(FS_DIR_BACKGROUNDS + bkFilename);
		mutex->unlock();

		auto yTiles = levelData.get<int>("height");

		auto yLength = yTiles * Y_STEP;

		//need these offsets, don't know why
		//may need to change when switching level.
		const float X_OFFSET = properties.get<float>("xmapoffset");	
		const float Y_OFFSET = properties.get<float>("ymapoffset");
		auto mlOffset = sf::Vector2f((cosf(22.5f) * yLength) + X_OFFSET, Y_OFFSET);

		subLevel_sp->setMapTexture(mlTexture_sp, mlOffset);
		subLevel_sp->setBackgroundTexture(bkTexture_sp, sf::Vector2f(0,0));
//		mLoadedLevel->setBackgroundTexture(bgTexture_sp, sf::Vector2f(0, 0));
	}

	
	{
		//check tilesets, to map gids to objectnames
		auto tilesets = levelData.get_child("tilesets");
		for(auto it = tilesets.begin(), end = tilesets.end(); it != end; ++it) {
			//look for first gid in tileset
			auto& tileset = it->second;
			auto firstgid = tileset.get<int>("firstgid");
			
			auto tileproperties_optional = tileset.get_child_optional("tileproperties");
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
			auto ignore = layer.get<std::string>("properties." + NAME_PROPERTIES_IGNORE,"");

			if(ignore == "true")
				continue;

			auto type = layer.get<std::string>("type");

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
					auto x = object.get<int>("x", 0);
					auto y = object.get<int>("y", 0);
					auto position = sf::Vector2f(x * X_STEP / 32, y * Y_STEP / 32);
					mutex->lock();
					objFact.callCallback(objectType, subLevel_sp.get(), position, object);
					mutex->unlock();
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
					mutex->lock();
					objFact.callCallback(objectType, subLevel_sp.get(), position, emptyPt);
					mutex->unlock();
				}
			}
		}
	}
}
