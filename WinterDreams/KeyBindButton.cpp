#include "KeyBindButton.h"
#include <SFML\Graphics\Text.hpp>
#include "WindowManager.h"
#include "PropertyManager.h"
#include "InputManager.h"

class KeyBindButtonSpecs {
public:

	static KeyBindButtonSpecs& get(){ static KeyBindButtonSpecs sSpecs; return sSpecs; }

	float mXOffset;

	float mYOffset;

	std::string mFilename;

private:

	KeyBindButtonSpecs() {
		auto& kbnd = PropertyManager::get().getGeneralSettings().get_child("ui.settings.keybind");
		mXOffset = kbnd.get<float>("xoffset");
		mYOffset = kbnd.get<float>("yoffset");

		mFilename = kbnd.get<std::string>("filename");
	}
};



KeyBindButton::KeyBindButton():
	Button(sf::Vector2f(KeyBindButtonSpecs::get().mXOffset, KeyBindButtonSpecs::get().mYOffset), KeyBindButtonSpecs::get().mFilename),
	mUpdated(false){
		mFont = ResourceManager::get().getFont(FS_DIR_FONTS + "arial.ttf");
}

void KeyBindButton::activate(){
	if(mUpdated == false && (InputManager::get().isADown() ||InputManager::get().isStartDown() ) ){
		mUpdated = true;
		mKeyname="Press a key!";
		while (mKeyname=="Press a key!"){
			for(int i=0; i<101; ++i){
				if (sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(i))){
					mKeyname=="";
					bindKey(i);
				}
			}
		}
		mUpdated=false;
	}
}

void KeyBindButton::bindKey(int ei){
	mKeyCode = static_cast<sf::Keyboard::Key>(ei);
}

void KeyBindButton::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	int ei = static_cast<int>(mKeyCode);
	std::string kn=" ";
	if (ei < 26){							// Enum 0-25 is A through Z
		char c = ei+65;						// which translates ASCII 65 through 90
		kn.push_back(c);
	} else if (ei < 36){					// Enum 26-35 is 0-9
		char c = ei+36;						// and ASCII 48-57
		kn.push_back(c);
	} else if (ei < 75){
		switch (ei){
			case 36: {kn="escape"; break;}
			case 37: {kn="left control"; break;}
			case 38: {kn="left shift"; break;}
			case 39: {kn="left alt"; break;}
			case 40: {kn="left system"; break;}
			case 41: {kn="right control"; break;}
			case 42: {kn="right shift"; break;}
			case 43: {kn="right alt"; break;}
			case 44: {kn="right system"; break;}
			case 45: {kn="menu"; break;}
			case 46: {kn="["; break;}
			case 47: {kn="]"; break;}
			case 48: {kn=";"; break;}
			case 49: {kn=","; break;}
			case 50: {kn="."; break;}
			case 51: {kn="'"; break;}
			case 52: {kn="/"; break;}
			case 53: {kn="\\"; break;}
			case 54: {kn="~"; break;}
			case 55: {kn="="; break;}
			case 56: {kn="-"; break;}
			case 57: {kn="space"; break;}
			case 58: {kn="return"; break;}
			case 59: {kn="backspace"; break;}
			case 60: {kn="tab"; break;}
			case 61: {kn="page up"; break;}
			case 62: {kn="page down"; break;}
			case 63: {kn="end"; break;}
			case 64: {kn="home"; break;}
			case 65: {kn="insert"; break;}
			case 66: {kn="delete"; break;}
			case 67: {kn="+"; break;}
			case 68: {kn="-"; break;}
			case 69: {kn="*"; break;}
			case 70: {kn="/"; break;}
			case 71: {kn="left"; break;}
			case 72: {kn="right"; break;}
			case 73: {kn="up"; break;}
			case 74: {kn="down"; break;}
		}
	} else if (ei < 84) {
		kn="numpad ";
		char c = ei - 27;
		kn.push_back(c);
	} else if (ei<100) {
		kn="F";
		char c = ei-37;
		kn.push_back(c);
	} else if (ei == 100) {kn="pause";}

	sf::Text keystring(kn,*mFont);
	keystring.setPosition(KeyBindButtonSpecs::get().mXOffset*target.getSize().x,KeyBindButtonSpecs::get().mYOffset*target.getSize().y);
	target.draw(keystring);
}