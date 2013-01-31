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

     // Start the game loop
     while (window.isOpen())
     {
		 clock.restart();
		 auto packet = getPacket(socket);

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