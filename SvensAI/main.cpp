#include <string>
#include <tchar.h>
#include <stdio.h>
#include <memory>
#include <vector>
#include <stack>

#include "Serial.h"
#include "Sensor.h"
#include "LightSensor.h"
#include "SonicSensor.h"
#include "Map.h"

int main() {

#pragma region Settings
	char* comPort = new char[10] {"\\\\.\\COM3"};									//need to be set up to mBot COM port
	int readBufferSize = 2;
	int writeBufferSize = 2;
	int sensorsSize = 2;
	using LightSensorState = LightSensor::LightSensorState;

	using Point = Map::Point;
	std::vector<Point> map{
	//	{{<Place name>}, <number of node>, <foward node>, <right node>, <left node>, <back node>, {<connections>}}
	//																							  {0,1,2,3,4,5}
		{{"crossroad"},		0,				  3,			 1,				 5,			-1,		  {0,1,0,1,0,0}},
		{{"crossroad"},		1,				  2,			-1,				 0,			-1,		  {1,0,1,0,0,0}},
		{{"crossroad"},		2,				 -1,			-1,				 3,			 1,		  {0,1,0,1,0,0}},
		{{"crossroad"},		3,				 -1,			 2,				 4,			 0,		  {1,0,1,0,1,0}},
		{{"crossroad"},		4,				 -1,			 3,				-1,			 5,		  {0,0,0,0,0,0}},
		{{"crossroad"},		5,				  4,			 0,				-1,			-1,		  {1,0,0,0,1,0}}
	};
	
	std::shared_ptr<Map> map_ptr = std::make_shared<Map>(std::move(map), 2, Map::Direction::west);		//map object that will calculate directions to move by robot - (will be) shared with AI

#pragma endregion

	std::unique_ptr<Serial> serial = std::make_unique<Serial>(std::move(comPort));	//serial port that receive and send data to mBot robot

	char* readBuffer  = new char[readBufferSize];									//buffer for receving serial data - readBuffer[0] = light sensor state, readBuffer[1] = sonic sensor state (in HEX)
	char* writebuffer;																//buffer for sending data to Arduino - writeBuffer[0] = Arduino function, writeBuffer[1] = Arduino function argument

	std::vector<std::shared_ptr<Sensor>> sensors(sensorsSize);
	sensors[0] = std::make_shared<LightSensor>();									//seting up light sensor  - (will be) shared with AI
	sensors[1] = std::make_shared<SonicSensor>();									//setting up sonic sensor - (will be) shared with AI
	
	bool mFlag = true;																//flag for main AI loop

	int pathLenght = map_ptr->setDirections(5);										//store data about robot path lenght

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

				switch (sensors[0]->getValue())										//Light sensor switch
				{
				case LightSensorState::foward:
					
					break;
				case LightSensorState::right:

					break;
				case LightSensorState::left:

					break;
				case LightSensorState::stop:
					if (pathLenght > 0) {
						Map::MoveDirection moveDir = map_ptr->moveNext();
						writebuffer = new char[writeBufferSize] {'0', static_cast<char>(moveDir)}; //0 - move function, {0 - foward, 1 - turn right, 2 - turn left}
						serial->WriteData(writebuffer, 2);
					}
					break;
				default:
					break;
				}
		}
	}
}
