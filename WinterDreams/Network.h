#ifndef INCLUDED_NETWORK
#define INCLUDED_NETWORK

#include <SFML\Graphics\Texture.hpp>
#include <SFML\Network\Http.hpp>
#include <SFML\Network\TcpSocket.hpp>
#include <SFML\Network\IpAddress.hpp>
#include <SFML\Network\Packet.hpp>
#include <SFML\System\Vector2.hpp>
#include <SFML\System\Vector3.hpp>

struct PacketContents{
	int player;				//The uKontroll player identifier
	sf::Vector2f joystick;	//The uKontroll joystick X
	sf::Vector3f gyro;		//The uKontroll device gyro X
	bool a;					//The uKontroll A button
	bool b;					//The uKontroll B button
};

//////////////////////////////////////////////////
// /Sends a request package to ukontroll.com/newsession/
// /containing the API key and responds with a Response class
// /which holds all the data the server sends back
//////////////////////////////////////////////////
sf::Http::Response getResponse();
//////////////////////////////////////////////////
// /Sends a request package to ukontroll.com/qr/
// /containing the session ID from the GET RESPONSE
// /and returns a valid sf::Texture (hopefully!)
//////////////////////////////////////////////////
sf::Texture getQR(const sf::Http::Response& getResponse);
///////////////////////////////////////////////////////////////////
// /Connects to the ukontroll server via the Session IP from the
// /GET RESPONSE on port :2511 (so tell people to keep it open!)
// /Returns a sf::TcpSocket pointer. Now also claims the session
// /as admin!
///////////////////////////////////////////////////////////////////
sf::TcpSocket* openSocket(const sf::Http::Response& getResponse);
//////////////////////////////////////////////////
// /Receives a packet from the server containing
// /the uKontroller data string and returns a
// /struct containing all your data for you, lazy.
//////////////////////////////////////////////////
PacketContents getPacket(sf::TcpSocket* tcpSocket);

#endif