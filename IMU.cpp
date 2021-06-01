#include "IMU.h"


namespace {
	//Accel/Gyro registers
	const uint8_t AG_WHOAMI_REG = 0x0F;
	const uint8_t CTRL_REG1_G = 0x10;
	const uint8_t CTRL_REG6_XL = 0x20;
	const uint8_t CTRL_REG8 = 0x22;
	const uint8_t STATUS_REG_AG = 0x17;
	const uint8_t  OUT_GYRO_REG_START = 0x18;
	const uint8_t OUT_ACCEL_REG_START = 0x28;
	const uint8_t OUT_TEMP_REG_START = 0x15;
	//Mag registers
	const uint8_t M_WHOAMI_REG = 0x0F;
	const uint8_t STATUS_REG_M = 0x27;
	const uint8_t CTRL_REG1_M = 0x20;
	const uint8_t CTRL_REG2_M = 0x21;
	const uint8_t CTRL_REG3_M = 0x22;
	const uint8_t CTRL_REG4_M = 0x23;
	const uint8_t CTRL_REG5_M = 0x24;

}



HAL_I2C bus = HAL_I2C(I2C_IDX2);
LSM9DS1_Driver_RodosI2C imu = LSM9DS1_Driver_RodosI2C(&bus);


IMUThread imuThread;


void IMUThread::getSensorData() {

	float x,y,z;

	if (imu.readGyroscope(x,y,z)) {

		gyro.x = x;
		gyro.y = y;
		gyro.z = z;

	}

	if (imu.readAccelerometer(x,y,z)) {

		accel.x = x;
		accel.y = y;
		accel.z = z;

	}

	if (imu.readMagnetometer(x,y,z)) {

		mag.x = x;
		mag.y = y;
		mag.z = z;

	}

	if (imu.readTemperature(x)) {

		temp = x;

	}

	//Update sensor readings
	sensorData.temp = temp;
	sensorData.gyro = gyro;
	sensorData.accel = accel;
	sensorData.mag = mag;

}


void IMUThread::processData() {

	//Calculate time since last process
	float dTime = float(NOW() - lastSensorProcessTimestamp)/SECONDS;
	lastSensorProcessTimestamp = NOW();

	//Integrate gyro values
	sensorData.gyroPosition += sensorData.gyro*dTime;

	//Integrate gyro values correctly
	float cosP = cos(sensorData.euler.y);
	float sinP = sin(sensorData.euler.y);
	float cosR = cos(sensorData.euler.x);
	float sinR = sin(sensorData.euler.x);

	Vector gyroWorld = 0;

	gyroWorld.x = (sensorData.gyro.y*cosR*cosP - sensorData.gyro.z*sinR*cosP)/cosP;
	gyroWorld.y = (cosP + sensorData.gyro.y*sinR*sinP + sensorData.gyro.z*cosR*sinP)/cosP;
	gyroWorld.z = (sensorData.gyro.y*sinR + sensorData.gyro.z*cosR)/cosP;

	sensorData.euler += gyroWorld*dTime;



}


void IMUThread::publishData() {

	if ((NOW()- lastPublishTimestamp) >= publishInterval){ //Damit die Funktion nicht blockiert so wie suspendCallUntil es tun wuerde
		lastPublishTimestamp = NOW();

		//Publish into sensorDataTransmitTopic
		sensorDataTransmitTopic.publish(sensorData);

	}


}


void IMUThread::getTelecommandData() {

		DataPacket datenPacket;
		if (dataPacketTransmittBuffer.get(datenPacket)) {
			if(datenPacket.packetID == 'P'){
				publishInterval = datenPacket.packetData;
			}
		}
}



