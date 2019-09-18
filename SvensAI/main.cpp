#include <string>
#include <tchar.h>
#include <stdio.h>
#include <memory>

#include "Serial.h"

int main() {

	char* comPort = new char[10] {"\\\\.\\COM3"};
	std::unique_ptr<Serial> serial = std::make_unique<Serial>(std::move(comPort)); //serial port that receive and send data to mBot robot
	std::shared_ptr<char[]> buffor(new char[10]{}); //buffor for serial data. NOTE: C++17 required

	bool mFlag = true; //flag for main AI loop

	while (mFlag && serial->IsConnected()) { //main AI loop
		if (serial->ReadData(buffor, 10)!=-1) { //get data to buffer
			printf("Data from serial: %s", buffor.get()); //print data from buffer
		}
	}
}
