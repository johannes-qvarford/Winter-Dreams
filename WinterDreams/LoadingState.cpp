#include "LoadingState.h"
#include "GameState.h"
#include "StateManager.h"
#include "Wall.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <SFML/System/Vector2.hpp>
#include <map>

//tile ids of small, normal, and big collision tiles. 
const int FGID_SMALL_COL = 1;
const int FGID_MEDIUM_COL = 9;
const int FGID_BIG_COL = 5;

//the length and width of each tile, in the game
const float STEP = 35.77708763999664f;

LoadingState::LoadingState(GameState* gameState_p, std::string filepath):
	mLoadedLevel(gameState_p),
	mLevelData()
{
	using namespace boost::property_tree;
	json_parser::read_json(filepath, mLevelData);
}

LoadingState::~LoadingState() {
}

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

void LoadingState::update(int milliseconds) {
	using namespace boost::property_tree;

	//map tile ids to functions creating entities
	std::map<int, CreateFunc> idToFunc;

	idToFunc[FGID_SMALL_COL] = createSmallCol;
	idToFunc[FGID_MEDIUM_COL] = createMediumCol;
	idToFunc[FGID_BIG_COL] = createBigCol;

	auto layers = mLevelData.get_child("layers");

	//iterate over layers
	for(auto it = layers.begin(), end = layers.end(); it != end; ++it) {
		auto& layer = it->second;

		//check for the ignore property. if its "false", go to the next layer
		//if its not, or doens't exist, check layer for tiles.
		std::string ignore = layer.get<std::string>("properties.ignore","false");
		if(ignore == "true")
			continue;

		auto& data = layer.get_child("data");
		auto tilesPerRow = layer.get<int>("width");

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

	//were done!
	//destroy the loadingState, revealing the underlying state(the GameState)
	StateManager::get().popState();
}