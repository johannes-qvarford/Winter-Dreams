
#if !defined(SCENARY_NAME)
#error not defined SCENARY_OBJNAME
#endif

#define STRINGIFY(x) #x
#define XSTRINGIFY(x) STRINGIFY(x)
#define SCENARY_CALLBACK SCENARY_NAME ## Callback
#define SCENARY_REG SCENARY_NAME ## Reg
#define SCENARY_STRING XSTRINGIFY(SCENARY_NAME)

#include "RegistrationCommonHeaders.h"
#include "PropertyManager.h"
#include "ResourceManager.h"
#include "OccludedEntity.h"

static void SCENARY_CALLBACK(SubLevel* subLevel_p, const sf::Vector2f& position, const boost::property_tree::ptree& pt) {
	
	//based on scenary, get properties used to create animation
	auto& scenary = PropertyManager::get().getObjectSettings().get_child("objects.scenery");


	auto& spriteTree = scenary.get_child(SCENARY_STRING ".animations");
	std::string filename = spriteTree.get<std::string>("filename");
	int xorigin = spriteTree.get<int>("xorigin");
	int yorigin = spriteTree.get<int>("yorigin");
	int spritewidth = spriteTree.get<int>("spritewidth", -1);
	int spriteheight = spriteTree.get<int>("spriteheight", -1);
	int numberofsprites = spriteTree.get<int>("numberofsprites", 1);
	int framespersprite = spriteTree.get<int>("framespersprite", 100);

	//create a temporary texture to get its dimensions.
	auto temp_sp = ResourceManager::get().getTexture(FS_DIR_OBJECTANIMATIONS + "scenary/" + filename);

	auto sWidth = spritewidth != -1 ? spritewidth : temp_sp->getSize().x;
	auto sHeight = spriteheight != -1 ? spriteheight : temp_sp->getSize().y; 

	Animation anim(FS_DIR_OBJECTANIMATIONS + "scenary/" + filename, sWidth, sHeight, numberofsprites, framespersprite, xorigin, yorigin); 

	auto occluded_sp = std::shared_ptr<OccludedEntity>(new OccludedEntity(
		sf::FloatRect(position, sf::Vector2f(X_STEP, -Y_STEP)), anim, 1.f, 1.f, 100, 1, true));

	subLevel_p->addGraphicalEntity(occluded_sp);
}

static ObjectTypeRegistration SCENARY_REG(SCENARY_STRING, SCENARY_CALLBACK);