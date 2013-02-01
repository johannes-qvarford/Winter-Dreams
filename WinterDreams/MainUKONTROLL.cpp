#include <SFML/Graphics.hpp>
#include "Network.h"
#include <SFML/System/Clock.hpp>
 
 int main()
 {
     // Create the main window
     sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");
 
	 sf::Http::Response response = getResponse();
	 sf::Texture qr = getQR(response);
	 sf::TcpSocket* socket = openSocket(response);

     // Load a sprite to display
     sf::Sprite sprite(qr);
 
	 sf::Clock clock;
	 sf::Time limit = sf::milliseconds( static_cast<int>(1000/60) );

<<<<<<< HEAD
     // Start the game loop
     while (window.isOpen())
     {
		 clock.restart();
		 auto packet = getPacket(socket);
=======
int main()
{
	try {
		auto gameState_p = new GameState();
		auto firstLevelFilename = PropertyManager::get().getGeneralSettings().get<std::string>("first_level_filename");
		auto loadState_p = new LoadingState(gameState_p, FS_DIR_LEVELS + firstLevelFilename);
		/*
			Push the game state first, and the load state second.
			The load state will be updated until it has initialized the game state,
			at which point it will be popped from the StateManager.
		*/
		StateManager::get().pushState(gameState_p);
		StateManager::get().pushState(loadState_p);
>>>>>>> 495961dd90725e7d7d1d7759a4e627180cf3e33b

		 sprite.move(packet.joystick.x/25,packet.joystick.y/25);

         // Process events
         sf::Event event;
         while (window.pollEvent(event))
         {
             // Close window : exit
             if (event.type == sf::Event::Closed)
                 window.close();
         }
 
         // Clear screen
         window.clear();
 
         // Draw the sprite
         window.draw(sprite);
 
         // Update the window
         window.display();

		 sf::sleep( limit - sf::milliseconds( clock.getElapsedTime().asMilliseconds() ) );
     }
 
     return EXIT_SUCCESS;
 }
