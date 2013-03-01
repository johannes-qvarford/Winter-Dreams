#include "Network.h"
#include <string>
#include <SFML/System/Time.hpp>
#include <iostream>

// GetResponse returns a Reponse class that contains a string of API fluff
sf::Http::Response getResponse(){
	sf::Http http;
	sf::Http::Request request;

	request.setMethod(sf::Http::Request::Get);	// Sets the HTTP Method to GET
	request.setUri("newsession/5adb77354af04740968d647e6c8a77d20ce2c476");		// An URI is the part of an URL beyond the domain

	http.setHost("http://ukontroll.com/");		// The rest of the URL.
	return http.sendRequest(request);
}

// Returns an sf::Texture containing
sf::Texture getQR(const sf::Http::Response& getResponse){
	sf::Http http;
	sf::Http::Request request;

	std::string getResponseBody=getResponse.getBody();	//Saves the content of getResponse to a string 
	std::string sessionId = getResponseBody.substr(getResponseBody.find_last_of('=')+1);	//Looks up the SessionID (beyond the last = character)

	request.setMethod(sf::Http::Request::Get);
	request.setUri("qr/"+sessionId);
	http.setHost("http://ukontroll.com/");	// /qr/SESSION_ID is ukontroll's QR generating subpage.
	std::string qrPNG = http.sendRequest(request).getBody();	
	const char* qrPointer = qrPNG.c_str();	// We get the png as a cstring so we need to actually fluff around a little
	sf::Texture qrTexture;
	qrTexture.loadFromMemory(qrPointer, qrPNG.size());	// Load the png from memory, because memory buffers didn't work in SFML
	return qrTexture;
}

sf::TcpSocket* openSocket(const sf::Http::Response& getResponse){
	sf::TcpSocket* socket = new sf::TcpSocket();
	socket->setBlocking(1);		//Blocking freezes the thread while waiting for the socket to start

	std::string getResponseBody=getResponse.getBody();

	auto ipStart = getResponseBody.find("&ip=")+4;	//find the Ip adresse part of the string
	auto ipEnd = getResponseBody.find("&session=");	//and the end
	auto ipLength = ipEnd-ipStart;	//find the length of the IP adresse
	auto tokenLength = ipStart-10;	//find the length of the Token

	std::string sessionIP = getResponseBody.substr(ipStart,ipLength);	//extract the IP adresse
	sf::IpAddress socketIP(sessionIP);
	sf::Socket::Status status;
	status = socket->connect(socketIP,2511);			//This block returns a few console outputs depending on what the heck happened
	if (status == sf::Socket::Done){
		std::cout << "All wired up!" << std::endl;		// means the connection has been established
	} if (status == sf::Socket::NotReady){
		std::cout << "Ain't baked yet" << std::endl;	// means it's still trying to connect
		while (status == sf::Socket::NotReady){
			sf::Time sleep = sf::milliseconds(10);
			sf::sleep(sleep);
			status = socket->connect(socketIP,2511);
		}
	} if (status == sf::Socket::Disconnected){
		std::cout << "This really shouldn't happen" << std::endl;	// You got disconnected from the server right after connecting.
	} if (status == sf::Socket::Error){						// Something's off. Did the server kick you out?
		std::cout << "u dun goofed nao" << std::endl;				// Something's wrong and the socket returned an error.
	}
	std::cout << "done" << std::endl;
	std::string token = getResponseBody.substr(6,tokenLength);		// shoot a packet of login data to the server. I don't know why you'd do this.
	std::string login = "admin|"+token+";";
	socket->send(login.c_str(), login.size()+1);
	socket->setBlocking(false);
	return socket;
}

PacketContents getPacket(sf::TcpSocket* tcpSocket, bool* connectionStatus ){
	static std::string buf("");
	static std::string lastFrameMovement("");

	//Set up variables for the function
	PacketContents packetContents;
	sf::Socket::Status status;
	char temp[42];
	std::size_t r(0);	
	std::string data, content = "";	
		//Get the data-stream from the uKontroll server
	status = tcpSocket->receive(reinterpret_cast<void*>(temp), sizeof(temp), r);
	
		//Fill a std::string with the data from the uKontroll server
	for( size_t i = 0; i < r; ++i) {
		data.push_back(temp[i]);
	}
		//If the data contained "connected" we either
		//connected or disconnected. Either way,
		//we set out lastMovement to 0;
	if (data.find("connected")!=data.npos){
		lastFrameMovement = "1|0|0|0|-0|0|0|0;";
		buf = lastFrameMovement;
		std::cout <<"Connect/Disconnect" <<std::endl;
		*connectionStatus  = false;
		return packetContents;
	}
		//Push the gotten data onto the buf
	buf = buf + data;
		//content is assigned 
	content = buf.substr( 0, buf.find(";")+1 );
		//If we've gotten more then one instruction
		//via the socket, store the remaining data in buf.
	if( buf.size() > 0 )
		buf = buf.substr( buf.find(";")+1 );
		
		//Handel the data
	if( status == sf::Socket::Done || connectionStatus){
			//Switch the bool that indicates we've connected
		*connectionStatus = true;
			//If no new data's reached us, use last frames data
		if(content.size() < 1)
			content = lastFrameMovement;

		std::cout<< content <<std::endl;
		int pos=0;
		int packetpos=0;
		// Time to manuall parse the std::string
					
		std::string buffer;
		std::string subcontents;
		int joystickX, joystickY, gyroX, gyroY, gyroZ;
		do{
			subcontents=content.substr(pos,1);
			if (subcontents!="|"){				//Looks for pipes to separate every input
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
				case 6: {packetContents.a=stoi(buffer) ? true : false; break;}
				case 7: {packetContents.b=stoi(buffer) ? true : false; break;}
				}
				buffer="";
				++packetpos;
				++pos;
			}
		}while(subcontents!=";");

		packetContents.joystick=sf::Vector2f(float(joystickX), float(joystickY));		//returns joystick as a vec2
		packetContents.gyro=sf::Vector3f(float(gyroX), float(gyroY), float(gyroZ));		//and gyro as a vec3
	}
		//The joystick sends scrap data on first frame after connect. Handle that data.
	if( std::abs(packetContents.joystick.x) > 100 || std::abs(packetContents.joystick.y) > 100 ){
		packetContents.joystick = sf::Vector2f(0,0);
	}
		//If buf conntains to much data, keep the last pieces of data
	if( buf.size() > 29 ){
		int lastposition = buf.find_last_of(';');
		buf = buf.substr(lastposition+1);
	}
		//If no new content's been sent this frame, content will be assigned
		//lastFrameMovement. 
	if( content.size() > 0 ){
		lastFrameMovement = content;
	}

	return packetContents;	//pukes
}
