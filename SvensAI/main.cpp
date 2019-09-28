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
#include "LightSensor.h"
#include "SonicSensor.h"
#include "Map.h"

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
		//																							  {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18}
			{{"crossroad"},		0,				  2,			 3,				 1,			-1,		  {0,1,1,1,0,0,0,0,0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
			{{"crossroad"},		1,				  4,			 0,				 -1,		-1,		  {1,0,0,0,1,0,0,0,0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
			{{"crossroad"},		2,				5,			 6,				 4,			 0,		  {1,0,0,0,1,1,1,0,0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
			{{"crossroad"},		3,				6,			-1,				 0,			-1,		  {1,0,0,0,0,0,1,0,0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
			{{"crossroad"},		4,				7,			 2,				-1,			 1,		  {0,1,1,0,0,0,0,1,0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
			{{"crossroad"},		5,				9,			10,				 8,			 2,		  {0,0,1,0,0,0,0,0,1,1, 1, 0, 0, 0, 0, 0, 0, 0, 0}},
			{{"crossroad"},		6,				11,				-1,				2,			3,		  {0,0,1,1,0,0,0,0,0,0, 0, 1, 0, 0, 0, 0, 0, 0, 0}},
			{{"crossroad"},		7,				-1,				13,				12,			4,		  {0,0,0,0,1,0,0,0,0,0, 0, 0, 1, 1, 0, 0, 0, 0, 0}},
			{{"crossroad"},		8,				14,				5,				-1,			13,		  {0,0,0,0,0,1,0,0,0,0, 0, 0, 0, 1, 1, 0, 0, 0, 0}},
			{{"crossroad"},		9,				-1,				15,				14,			5,		  {0,0,0,0,0,1,0,0,0,0, 0, 0, 0, 0, 1, 1, 0, 0, 0}},
			{{"crossroad"},		10,				15,				-1,				5,			-1,		  {0,0,0,0,0,1,0,0,0,0, 0, 0, 0, 0, 0, 1, 0, 0, 0}},
			{{"crossroad"},		11,				-1,				-1,				16,			6,		  {0,0,0,0,0,0,1,0,0,0, 0, 0, 0, 0, 0, 0, 1, 0, 0}},
			{{"crossroad"},		12,				17,				7,				-1,			-1,		  {0,0,0,0,0,0,0,1,0,0, 0, 0, 0, 0, 0, 0, 0, 1, 0}},
			{{"crossroad"},		13,				8,				-1,				7,			-1,		  {0,0,0,0,0,0,0,1,1,0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
			{{"crossroad"},		14,				-1,				9,				17,			8,		  {0,0,0,0,0,0,0,0,1,1, 0, 0, 0, 0, 0, 0, 0, 1, 0}},
			{{"crossroad"},		15,				-1,				18,				9,			10,		  {0,0,0,0,0,0,0,0,0,1, 1, 0, 0, 0, 0, 0, 0, 0, 1}},
			{{"crossroad"},		16,				18,				11,				-1,			-1,		  {0,0,0,0,0,0,0,0,0,0, 0, 1, 0, 0, 0, 0, 0, 0, 1}},
			{{"crossroad"},		17,				-1,				14,				-1,			12,		  {0,0,0,0,0,0,0,0,0,0, 0, 0, 1, 0, 1, 0, 0, 0, 0}},
			{{"crossroad"},		18,				-1,				-1,				15,			16,		  {0,0,0,0,0,0,0,0,0,0, 0, 0, 0, 0, 0, 1, 1, 0, 0}}
	};
	std::shared_ptr<Map> map_ptr = std::make_shared<Map>(std::move(map), 4, Map::Direction::east);		//map object that will calculate directions to move by robot - (will be) shared with AI

#pragma endregion

	std::unique_ptr<Serial> serial = std::make_unique<Serial>(std::move(comPort));	//serial port that receive and send data to mBot robot

	char* readBuffer  = new char[readBufferSize];									//buffer for receving serial data - readBuffer[0] = light sensor state, readBuffer[1] = sonic sensor state (in HEX)
	char* writebuffer;																//buffer for sending data to Arduino - writeBuffer[0] = Arduino function, writeBuffer[1] = Arduino function argument

	std::vector<std::shared_ptr<Sensor>> sensors(sensorsSize);
	sensors[0] = std::make_shared<LightSensor>();									//seting up light sensor  - (will be) shared with AI
	sensors[1] = std::make_shared<SonicSensor>();									//setting up sonic sensor - (will be) shared with AI
	
	bool mFlag = true;																//flag for main AI loop

	int pathLenght = map_ptr->setDirections(6);										//store data about robot path lenght

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

#pragma region LightSensor
				switch (sensors[0]->getValue())										//Light sensor switch
				{
				case 0:
					
					break;
				case 1:
					                          
					break;
				case 2:

					break;
				case 3:
					if (pathLenght > 0 && (sensors[0]->getValue() != lastLightSensorValue)) {
						Map::MoveDirection moveDir = map_ptr->moveNext();
						writebuffer = new char[writeBufferSize] {static_cast<char>(moveDir)}; //0 - move function, {0 - foward, 1 - turn right, 2 - turn left}
						pathLenght--;
						if(pathLenght!=0)
						serial->WriteData(writebuffer, writeBufferSize);
						std::this_thread::sleep_for(std::chrono::seconds(2));
					}
					else if (pathLenght == 0) {
						int direction;
						printf("Enter another direction: ");
						std::cin >> direction;
						pathLenght = map_ptr->setDirections(direction);
					}
					break;
				}
#pragma endregion
				if (sensors[0]->getValue() != lastLightSensorValue) {
					lastLightSensorValue = sensors[0]->getValue();
				}
		}
	}
}
