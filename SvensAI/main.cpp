#include <string>
#include <tchar.h>
#include <stdio.h>
#include <memory>
#include <vector>

#include "Serial.h"
#include "Sensor.h"
#include "LightSensor.h"
#include "SonicSensor.h"

int main() {

	char* comPort = new char[10] {"\\\\.\\COM3"};									//need to be set up to mBot COM port
	std::unique_ptr<Serial> serial = std::make_unique<Serial>(std::move(comPort));	//serial port that receive and send data to mBot robot

	int bufferSize = 2;
	char* buffer = new char[bufferSize];											//buffor for serial data - buffer[0] = light sensor state, buffer[1] = sonic sensor state (in HEX)

	int sensorsSize = 2;
	std::vector<std::shared_ptr<Sensor>> sensors(sensorsSize);
	sensors[0] = std::make_shared<LightSensor>();
	sensors[1] = std::make_shared<SonicSensor>();
	
	bool mFlag = true;																//flag for main AI loop

	while (mFlag && serial->IsConnected()) {										//main AI loop
		if (serial->ReadData(buffer, bufferSize)!=-1) {								//get data to buffer
			for (int i = 0; i < bufferSize; i++) {
				switch (i)
				{
				case 0:																//Light sensor
					sensors[0]->setValue(atoi(&buffer[0]));
					break;
				case 1:																//Sonic sensor
					sensors[1]->setValue(buffer[1]);
					break;
				default:
					printf("No case programmed for sensor %i", i);
					break;
				}
			}
		}
	}
}
