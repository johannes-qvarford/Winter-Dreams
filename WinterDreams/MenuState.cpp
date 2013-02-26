#include "MenuState.h"
#include "StartGameButton.h"
#include "ResourceManager.h"
#include "FileStructure.h"
#include "WindowManager.h"
#include "ListFrame.h"
#include "QRDisplay.h"

#include <boost/foreach.hpp>
#include <memory>

#define foreach BOOST_FOREACH

MenuState* MenuState::makeMainMenuState() {
	auto& res = ResourceManager::get();
	
	auto state_p = new MenuState();

	auto widgets = std::vector<std::shared_ptr<Widget> >();
	
	//create a start button
	auto start_position = sf::Vector2f(300, 550);
	auto start_sp = std::make_shared<StartGameButton>(start_position);

	widgets.push_back(start_sp);
	
	//create qr widget
	auto qr_position = sf::Vector2f(0, 900);
	auto qr_sp = std::make_shared<QRDisplay>(qr_position);

	widgets.push_back(qr_sp);
	
	//create a list frame
	auto listFrame_sp = std::make_shared<ListFrame>(widgets);

	//add a background image.
	auto bg_sp = res.getTexture(FS_DIR_UI + "main_menu.png");

	state_p->setBackground(bg_sp);
	state_p->addWidget(start_sp);
	state_p->addWidget(listFrame_sp);
	state_p->addWidget( qr_sp);

	return state_p;
}

MenuState::MenuState() {
}



MenuState::~MenuState() {
}

void MenuState::update() {
	foreach(auto& widget_sp, mWidgets) {
		widget_sp->update(this);
	}
}

void MenuState::render() {
	auto& window = *WindowManager::get().getRenderWindow();
	auto bgSprite = sf::Sprite();
	bgSprite.setTexture(*mBackground_sp);

	window.draw(bgSprite);
	foreach(auto& widget_sp, mWidgets) {
		window.draw(*widget_sp);
	}
}

void MenuState::setBackground(std::shared_ptr<sf::Texture> texture_sp) {
	mBackground_sp = texture_sp;
}

void MenuState::addWidget(std::shared_ptr<Widget> widget_sp) {
	mWidgets.push_back(widget_sp);
}
