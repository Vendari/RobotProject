#include <string>
#include <tchar.h>
#include <stdio.h>
#include <memory>
#include <iostream>
#include <thread>
#include <chrono>


#include "Serial.h"

int main() {

	char* comPort = new char[10] {"\\\\.\\COM5"};
	std::unique_ptr<Serial> serial = std::make_unique<Serial>(std::move(comPort)); //serial port that receive and send data to mBot robot

	char* buffer = new char[2];

	bool mFlag = true; //flag for main AI loop

	while (mFlag && serial->IsConnected()) { //main AI loop
		if (serial->ReadData(buffer, 2)!=-1) { //get data to buffer
			for(int i =0; i<2; i++)
				std::cout << buffer[i]; //showing data from buffer
			std::cout << std::endl;
			serial->WriteData(buffer, 2); //testing writing to buffer
		}
	}

	buffer = nullptr;
	delete[] buffer;
}
