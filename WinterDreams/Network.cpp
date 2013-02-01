#include "Network.h"
#include <string>
#include <SFML\System\Time.hpp>
#include <iostream>

sf::Http::Response getResponse(){
	sf::Http http;
	sf::Http::Request request;

	request.setMethod(sf::Http::Request::Get);
	request.setUri("newsession/API_KEY");

	http.setHost("http://ukontroll.com/");
	return http.sendRequest(request);
}

sf::Texture getQR(const sf::Http::Response& getResponse){
	sf::Http http;
	sf::Http::Request request;

	std::string getResponseBody=getResponse.getBody();
	std::string sessionId = getResponseBody.substr(getResponseBody.find_last_of('=')+1);

	request.setMethod(sf::Http::Request::Get);
	request.setUri("chart?chs=300x300&cht=qr&chld=L|1&chl="+sessionId);
	//request.setUri("qr/"+sessionId);

	//http.setHost("http://ukontroll.com/");
	http.setHost("http://chart.googleapis.com/");
	std::string qrPNG = http.sendRequest(request).getBody();
	const char* qrPointer = qrPNG.c_str();
	sf::Texture qrTexture;
	qrTexture.loadFromMemory(qrPointer, qrPNG.size());
	return qrTexture;
}

sf::TcpSocket* openSocket(const sf::Http::Response& getResponse){
	sf::TcpSocket* socket = new sf::TcpSocket();
	socket->setBlocking(0);

	std::string getResponseBody=getResponse.getBody();

	auto ipStart = getResponseBody.find("&ip=")+4;
	auto ipEnd = getResponseBody.find("&session=");
	auto ipLength = ipEnd-ipStart;
	auto tokenLength = ipStart-10;

	std::string sessionIP = getResponseBody.substr(ipStart,ipLength);
	sf::IpAddress socketIP(sessionIP);
	sf::Socket::Status status;
	status = socket->connect(socketIP,2511);
	if (status == sf::Socket::Done){
		std::cout << "All wired up!" << std::endl;
	} else if (status == sf::Socket::NotReady){
		std::cout << "Ain't baked yet" << std::endl;
		while (status == sf::Socket::NotReady){
			sf::Time sleep = sf::milliseconds(10);
			sf::sleep(sleep);
			status = socket->connect(socketIP,2511);
		}
	} else if (status == sf::Socket::Disconnected){
		std::cout << "This really shouldn't happen" << std::endl;
	} else if (status == sf::Socket::Error){
		std::cout << "u dun goofed nao" << std::endl;
	}

	sf::Packet packet;
	std::string token = getResponseBody.substr(6,tokenLength);
	std::string login = "admin|"+token;
	socket->send(login.c_str(), login.size()+1);
	return socket;
}

PacketContents getPacket(sf::TcpSocket* tcpSocket){
	//Receive the packet
	sf::Packet packet;
	PacketContents packetContents;
	tcpSocket->receive(packet);
	//Begin manipulating the hell out of it
	const char* char_content = reinterpret_cast<const char*>(packet.getData());
	std::string content;
	for (int i=0; i<packet.getDataSize();++i){
		if (char_content[i]!=';'){
			content.push_back(char_content[i]);
		} else {
			content.push_back(';');
			break;
		}
	}

	int pos=0;
	int packetpos=0;
	std::string buffer;
	std::string subcontents;
	int joystickX, joystickY, gyroX, gyroY, gyroZ;
	do{
		subcontents=content.substr(pos,1);
		if (subcontents!="|"){
			buffer+=subcontents;
			++pos;
		} else {
			switch(packetpos){
			case 0: {packetContents.player=stoi(buffer); break;}
			case 1: {joystickX=stoi(buffer); break;} /////////////////////////////////////
			case 2: {joystickY=stoi(buffer); break;} //									//
			case 3: {gyroX=stoi(buffer); break;}	 // These five are written to temps //
			case 4: {gyroY=stoi(buffer); break;}	 //									//
			case 5: {gyroZ=stoi(buffer); break;}	 /////////////////////////////////////
			case 6: {packetContents.a=stoi(buffer); break;}
			case 7: {packetContents.b=stoi(buffer); break;}
			}
			buffer="";
			++packetpos;
			++pos;
		}
	}while(subcontents!=";");
	packetContents.joystick=sf::Vector2f(joystickX,joystickY);
	packetContents.gyro=sf::Vector3f(gyroX,gyroY,gyroZ);
	return packetContents;
}
