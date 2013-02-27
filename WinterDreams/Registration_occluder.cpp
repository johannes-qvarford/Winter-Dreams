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

	auto spriteName = properties.get<std::string>("sprite");
	auto layer = properties.get<int>("layer");
	auto enabledopacity = properties.get<float>("enabledopacity");
	auto disabledopacity = properties.get<float>("disabledopacity");
	auto fadetime = properties.get<int>("fadetime");
	auto name = pt.get<std::string>("name","");

	auto width = pt.get<int>("width") / 32.f * X_STEP;
	auto height = -pt.get<int>("height") / 32.f * Y_STEP;//need to negate height.


	auto correctedPosition = position;
//	correctedPosition.y = correctedPosition.y - height - Y_STEP;//need to adjust position for ingame representation

	//based on spriteName, get properties used to create animation
	auto& animations = PropertyManager::get().getObjectSettings().get_child("occluder.animations");

	auto& spriteTree = animations.get_child(spriteName);
	std::string filename = spriteTree.get<std::string>("filename");
	int xorigin = spriteTree.get<int>("xorigin");
	int yorigin = spriteTree.get<int>("yorigin");
	


	//create a temporary texture to get its dimensions.
	auto temp_sp = ResourceManager::get().getTexture(FS_DIR_OBJECTANIMATIONS + "occluder/" + filename);

	Animation anim(FS_DIR_OBJECTANIMATIONS + "occluder/" + filename, temp_sp->getSize().x, temp_sp->getSize().y, 1, 100, xorigin, yorigin); 

	auto occluded_sp = std::shared_ptr<OccludedEntity>(new OccludedEntity(
		sf::FloatRect(correctedPosition, sf::Vector2f(X_STEP, -Y_STEP)), anim, enabledopacity, disabledopacity, fadetime, layer, !startdisabled));
	
	//does it have a name? What's it called? Oh, %n? That's cute.
	if(name != "")
		subLevel_p->mapEntityToName( name , occluded_sp);

	subLevel_p->addGraphicalEntity(occluded_sp);
}

static ObjectTypeRegistration reg("occluder", regCallback);
