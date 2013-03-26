#include "RegistrationCommonHeaders.h"
#include "Animation.h"
#include "OccludedEntity.h"
#include "ResourceManager.h"
#include "PropertyManager.h"
#include "FileStructure.h"

#include <SFML/Graphics/Texture.hpp>


static void regCallback(SubLevel* subLevel_p, const sf::Vector2f& position, const boost::property_tree::ptree& pt) {
	auto& properties = pt.get_child("properties");
	auto startdisabled = properties.get<bool>("startdisabled", false);
	auto sprite = properties.get<std::string>("sprite");
	auto layer = properties.get<float>("layer", 1);
	auto enabledopacity = properties.get<float>("enabledopacity", 100);
	auto disabledopacity = properties.get<float>("disabledopacity", 0);
	
	auto fadetime = properties.get<int>("fadetime", 500);
	auto name = pt.get<std::string>("name","");

	//based on sprite, get properties used to create animation
	auto& animations = PropertyManager::get().getObjectSettings().get_child("occluder.animations");

	auto& spriteTree = animations.get_child(sprite);
	std::string filename = spriteTree.get<std::string>("filename");
	int xorigin = spriteTree.get<int>("xorigin");
	int yorigin = spriteTree.get<int>("yorigin");
	int spritewidth = spriteTree.get<int>("spritewidth", -1);
	int spriteheight = spriteTree.get<int>("spriteheight", -1);
	int numberofsprites = spriteTree.get<int>("numberofsprites", 1);
	int framespersprite = spriteTree.get<int>("framespersprite", 100);
	int width = spriteTree.get<int>("width", 32);
	int height = spriteTree.get<int>("height", 32);

	//create a temporary texture to get its dimensions.
	auto temp_sp = ResourceManager::get().getTexture(FS_DIR_OBJECTANIMATIONS + "occluder/" + filename);

	auto sWidth = spritewidth != -1 ? spritewidth : temp_sp->getSize().x;
	auto sHeight = spriteheight != -1 ? spriteheight : temp_sp->getSize().y; 

	auto hWidth = width / 32.f * X_STEP;
	auto hHeight = -height / 32.f * Y_STEP;

	Animation anim(FS_DIR_OBJECTANIMATIONS + "occluder/" + filename, sWidth, sHeight, numberofsprites, framespersprite, xorigin, yorigin); 

	auto occluded_sp = std::shared_ptr<OccludedEntity>(new OccludedEntity(
		sf::FloatRect(position, sf::Vector2f(hWidth, hHeight)), anim, enabledopacity / 100, disabledopacity / 100, fadetime, layer * 100, !startdisabled));
	
	//does it have a name? What's it called? Oh, %n? That's cute.
	if(name != "")
		subLevel_p->mapEntityToName( name , occluded_sp);


	subLevel_p->addEntity(occluded_sp);
	subLevel_p->addDrawable(occluded_sp, SubLevel::DRAW_WORLD);
}

static ObjectTypeRegistration reg("occluder", regCallback);
