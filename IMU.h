#include "rodos.h"
#include "hal_gpio.h"
#include "hal_i2c.h"

#include "3d_math.h"

#include "telemetry_control.h"


//Accel/Gyro registers
#define CTRL_REG1_G 0x10
#define CTRL_REG6_XL 0x20
//Mag registers
#define CTRL_REG1_M 0x20
#define CTRL_REG2_M 0x21
#define CTRL_REG3_M 0x22


//Telemetry commands from UART
Fifo<DataPacket, 10> dataPacketBuffer;
Subscriber dataPacketSubscriber(dataPacketReceivedTopic, dataPacketBuffer);

class IMUThread:public Thread {
private:


public:

	void init() {
		bus.init(400000);
	}

	void run() {

		while (1) {

			if (!sensorInitialised) {

				initSensor();

			} else {

				getTelecommandData();

				readSensorData();

				processData();


			}

		}

	}


	//Main sensor control functions
	void processData();

	void initSensor();

	void readSensorData();

	void publishData();

	void getTelecommandData();

	//Gyro and accel Sensor communication functions.
	void writeByte(const uint8_t &reg, uint8_t data) {
		writeBytes(reg, &data, 1);
	}

	void writeBytes(const uint8_t &reg, uint8_t* data, const uint8_t &numberBytes);

	void readByte(const uint8_t &reg, uint8_t* data) {
		readBytes(reg, data, 1);
	}

	void readBytes(const uint8_t &reg, uint8_t* data, const uint8_t numberBytes);

	//Magnetometer communication functions.
	void writeByteMag(const uint8_t &reg, uint8_t data) {
		writeBytesMag(reg, &data, 1);
	}

	void writeBytesMag(const uint8_t &reg, uint8_t* data, const uint8_t &numberBytes);

	void readByteMag(const uint8_t &reg, uint8_t* data) {
		readBytesMag(reg, data, 1);
	}

	void readBytesMag(const uint8_t &reg, uint8_t* data, const uint8_t numberBytes);

private:

	//I2C Bus to communicate with sensor
	HAL_I2C bus = HAL_I2C(I2C_IDX2);

	//Raw sensor values
	Vector gyro;
	Vector accel;
	Vector mag;

	float temp;

	//Sensor values after processing
	SensorData sensorData;


	bool sensorInitialised = false;


	int64_t publishInterval = 1*SECONDS;


	int64_t  lastPublishTimestamp = 0;


};


IMUThread imuThread;
