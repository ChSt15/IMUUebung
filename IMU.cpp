#include "IMU.h"






void IMUThread::processData() {



}


void IMUThread::initSensor() {

	uint8_t whoAmI;

	readByte(0x0F, &whoAmI);

	//Check if whoAmI reg is correct.
	if (whoAmI != 0x68) return;

	uint8_t data = 0x04;

	writeByte(CTRL_REG1_G, data);

}


void IMUThread::readSensorData() {



}


void IMUThread::publishData() {



}


void IMUThread::getTelecommandData() {



}


//Functions for I2C communication
void IMUThread::writeBytes(const uint8_t &reg, uint8_t* data, const uint8_t &numberBytes) {

	bus.write(0x6B, &reg, 1);
	bus.write(0x6B, data, numberBytes);

}



void IMUThread::readBytes(const uint8_t &reg, uint8_t* data, const uint8_t numberBytes) {

	bus.write(0x6B, &reg, 1);
	bus.read(0x6B, data, numberBytes);

}


void IMUThread::writeBytesMag(const uint8_t &reg, uint8_t* data, const uint8_t &numberBytes) {

	bus.write(0x1E, &reg, 1);
	bus.write(0x1E, data, numberBytes);

}



void IMUThread::readBytesMag(const uint8_t &reg, uint8_t* data, const uint8_t numberBytes) {

	bus.write(0x1E, &reg, 1);
	bus.read(0x1E, data, numberBytes);

}
