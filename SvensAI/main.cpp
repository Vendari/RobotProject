#include <string>
#include <tchar.h>
#include <iostream>
#include <memory>
#include <vector>
#include <stack>
#include <thread>
#include <chrono>

#include "Serial.h"
#include "Sensor.h"
#include "Place.h"
#include "LightSensor.h"
#include "SonicSensor.h"
#include "Map.h"
#include "Sven.h"
#include "Inteligence.h"

int main() {

#pragma region Settings
	char* comPort = new char[10] {"\\\\.\\COM3"};									//need to be set up to mBot COM port
	int readBufferSize = 2;
	int writeBufferSize = 1;
	int sensorsSize = 2;
	using LightSensorState = LightSensor::LightSensorState;

	using Point = Map::Point;
	std::vector<Point> map{
	//	{{<Place name>}, <number of node>, <foward node>, <right node>, <left node>, <back node>, {<connections>}}
	//																							  {0,1,2,3,4,5,6}
		{{Place::Restaurant},0,				  3,			 2,				  1,		-1,		  {0,1,1,1,0,0,0}},
		{{Place::Home},		1,				  5,			 0,				 -1,		-1,		  {1,0,0,1,0,1,0}},
		{{Place::Crossroad},2,				  6,			-1,				  0,		-1,		  {1,0,0,1,0,0,1}},
		{{Place::Crossroad},3,				  4,			-1,				 -1,		 0,		  {1,0,0,0,1,0,0}},
		{{Place::Work},		4,				 -1,			 6,				  5,		 3,		  {0,0,0,1,0,1,1}},
		{{Place::Crossroad},5,				 -1,			 4,				 -1,		 1,		  {0,1,0,0,1,0,0}},
		{{Place::Toilet},	6,				 -1,			-1,				  4,		 2,		  {0,0,1,0,1,0,0}},
	};
	
	std::shared_ptr<Map> map_ptr = std::make_shared<Map>(std::move(map), 0, Map::Direction::north);		//map object that will calculate directions to move by robot - (will be) shared with AI

	std::shared_ptr<Sven> sven_ptr = std::make_shared<Sven>(50, 0, 1); // (hunger, toilet, level)
	std::unique_ptr<Inteligence> inteligence = std::make_unique<Inteligence>();
#pragma endregion

	std::unique_ptr<Serial> serial = std::make_unique<Serial>(std::move(comPort));	//serial port that receive and send data to mBot robot

	char* readBuffer  = new char[readBufferSize];									//buffer for receving serial data - readBuffer[0] = light sensor state, readBuffer[1] = sonic sensor state (in HEX)
	char* writebuffer;																//buffer for sending data to Arduino - writeBuffer[0] = Arduino function, writeBuffer[1] = Arduino function argument

	std::vector<std::unique_ptr<Sensor>> sensors(sensorsSize);
	sensors[0] = std::make_unique<LightSensor>();									//seting up light sensor
	sensors[1] = std::make_unique<SonicSensor>();									//setting up sonic sensor
	
	bool mFlag = true;																//flag for main AI loop

	int *pathLenght = new int;																//store data about robot path lenght
	inteligence->think(sven_ptr, map_ptr, pathLenght);

	int lastLightSensorValue, lastSonicSensorValue;									//store lasts sensor data to not overprint the console and to make synchronous movement
	lastLightSensorValue = lastSonicSensorValue = 0;

	while (mFlag && serial->IsConnected()) {										//main AI loop
		if (serial->ReadData(readBuffer, readBufferSize)!=-1) {						//get data to readBuffer
			for (int i = 0; i < readBufferSize; i++) {
				switch (i)
				{
				case 0:																//Light sensor
					sensors[0]->setValue(atoi(&readBuffer[0]));
					break;
				case 1:																//Sonic sensor
					sensors[1]->setValue(readBuffer[1]);
					break;
				default:
					printf("No case programmed for sensor %i", i);
					break;
				}
			}
			}

#pragma region LightSensor
				/*switch (sensors[0]->getValue())										//Light sensor switch
				{
				case 0:
					
					break;
				case 1:
					                          
					break;
				case 2:

					break;
				case 3:*/
					if (*pathLenght>0){// && (sensors[0]->getValue() != lastLightSensorValue)) {  //Change to connect to robot
						Map::MoveDirection moveDir = map_ptr->moveNext();
						if (*pathLenght > 1) {
							writebuffer = new char[writeBufferSize] {static_cast<char>(moveDir)}; //0 - move function, {0 - foward, 1 - turn right, 2 - turn left}
							serial->WriteData(writebuffer, writeBufferSize);
						}
						*pathLenght = *pathLenght-1;
						std::this_thread::sleep_for(std::chrono::seconds(1));
						sven_ptr->move();
						//inteligence->think(sven_ptr, map_ptr, pathLenght);
					}
					else if (*pathLenght == 0) {
						inteligence->think(sven_ptr, map_ptr, pathLenght);
						sven_ptr->logStats();
					}
					/*break;
				}
				if (sensors[0]->getValue() != lastLightSensorValue) {
					lastLightSensorValue = sensors[0]->getValue();
				}*/
#pragma endregion
	}
	delete pathLenght;
}
