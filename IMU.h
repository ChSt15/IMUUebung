#include "rodos.h"
#include "hal_gpio.h"
#include "hal_i2c.h"

#include "3d_math.h"

#include "telemetry_control.h"

#include "lsm9ds1_driver_rodos_i2c.h"


//Telemetry commands from UART
Fifo<DataPacket, 10> dataPacketBuffer;
Subscriber dataPacketSubscriber(dataPacketReceivedTopic, dataPacketBuffer);

extern HAL_I2C bus;
extern LSM9DS1_Driver_RodosI2C imu;


class IMUThread:public Thread {
private:


public:

	void init() {

		bus.init(400000);

	}

	void run() {

		while (1) {

			suspendCallerUntil(NOW() + 1*MILLISECONDS);

			if (!accelGyroInitialised && NOW() - lastAccelGyroInit >= 1*SECONDS) {
				lastAccelGyroInit = NOW();
				accelGyroInitialised = imu.initialiseAG();
			}

			if (!magInitialised && NOW() - lastMagInit >= 1*SECONDS) {
				lastMagInit = NOW();
				magInitialised = imu.initialiseM();
			}

			if (magInitialised && accelGyroInitialised) {

				getTelecommandData();

				getSensorData();

				processData();

				publishData();


			}

		}

	}


	//Main sensor control functions
	void processData();

	void getSensorData();

	void publishData();

	void getTelecommandData();

private:

	//Raw sensor values
	Vector gyro;
	Vector accel;
	Vector mag;

	float temp;

	//Sensor values after processing
	SensorData sensorData;


	bool accelGyroInitialised = false;
	bool magInitialised = false;

	int64_t lastAccelGyroInit = 0;
	int64_t lastMagInit = 0;

	int64_t lastSensorProcessTimestamp = 0;


	int64_t publishInterval = 100*MILLISECONDS;


	int64_t  lastPublishTimestamp = 0;


};
