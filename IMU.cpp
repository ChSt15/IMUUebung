#include "IMU.h"



HAL_I2C bus = HAL_I2C(I2C_IDX2);
LSM9DS1_Driver_RodosI2C imu = LSM9DS1_Driver_RodosI2C(&bus);


IMUThread imuThread;


void IMUThread::getSensorData() {

	float x,y,z;

	if (imu.readGyroscope(x,y,z)) {

		gyro.x = x;
		gyro.y = y;
		gyro.z = z;

		gyro = gyro - gyroOffset;

	}

	if (imu.readAccelerometer(x,y,z)) {

		accel.x = x;
		accel.y = y;
		accel.z = z;

		accel = accel - accelOffset;

	}

	if (imu.readMagnetometer(x,y,z)) {

		mag.x = x;
		mag.y = y;
		mag.z = z;

		mag = (mag - magMin)/(magMax - magMin)*2-1;

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


void IMUThread::calibrateSensor() {

	calibrated = true;

	PRINTF("Calibrating gyroscope...");

	gyroOffset = 0;

	for (uint32_t i = 0; i < 100; i++) {

		while(!imu.gyroscopeAvailable()) suspendCallerUntil(NOW() + 1*MILLISECONDS);

		float x,y,z;

		imu.readGyroscope(x,y,z);

		gyroOffset += Vector(x,y,z);

	}

	gyroOffset *= 1/100.0f;

	suspendCallerUntil(NOW() + 3*SECONDS);


	PRINTF("Calibrating accel...");

	PRINTF("Rotate z down...");

	suspendCallerUntil(NOW() + 3*SECONDS);

	PRINTF("begin...");

	accelOffset = 0;

	Vector bufZ = 0;

	for (uint32_t i = 0; i < 100; i++) {

		while(!imu.gyroscopeAvailable()) suspendCallerUntil(NOW() + 1*MILLISECONDS);

		float x,y,z;

		imu.readAccelerometer(x,y,z);

		bufZ += Vector(x,y,z);

	}

	//bufZ /= 100;


	PRINTF("Rotate x down...");

	suspendCallerUntil(NOW() + 3*SECONDS);

	PRINTF("begin...");

	accelOffset = 0;

	Vector bufX = 0;

	for (uint32_t i = 0; i < 100; i++) {

		while(!imu.gyroscopeAvailable()) suspendCallerUntil(NOW() + 1*MILLISECONDS);

		float x,y,z;

		imu.readAccelerometer(x,y,z);

		bufX += Vector(x,y,z);

	}

	//bufX /= 100;


	PRINTF("Rotate y down...");

	suspendCallerUntil(NOW() + 3*SECONDS);

	PRINTF("begin...");

	accelOffset = 0;

	Vector bufY = 0;

	for (uint32_t i = 0; i < 100; i++) {

		while(!imu.gyroscopeAvailable()) suspendCallerUntil(NOW() + 1*MILLISECONDS);

		float x,y,z;

		imu.readAccelerometer(x,y,z);

		bufY += Vector(x,y,z);

	}

	//bufY /= 100;



	accelOffset.x = (bufZ.x + bufY.x)/200.0f;
	accelOffset.y = (bufX.y + bufZ.y)/200.0f;
	accelOffset.z = (bufX.z + bufY.z)/200.0f;

	PRINTF("Starting mag calibration...");

	Vector magVector;;

	int64_t start = NOW();

	while(NOW() - start < 10*SECONDS) {

		while(!imu.magnetometerAvailable()) suspendCallerUntil(NOW() + 1*MILLISECONDS);

		float x,y,z;

		imu.readMagnetometer(x,y,z);

		magVector = Vector(x,y,z);

		if (magVector.x > magMax.x) {
			magMax.x = magVector.x;
		}
		if (magVector.x < magMin.x) {
			magMin.x = magVector.x;
		}
		if (magVector.y > magMax.y) {
			magMax.y = magVector.y;
		}
		if (magVector.y < magMin.y) {
			magMin.y = magVector.y;
		}
		if (magVector.z > magMax.z) {
			magMax.z = magVector.z;
		}
		if (magVector.z < magMin.z) {
			magMin.z = magVector.z;
		}

	}






}


Vector angleFix(const Vector &euler) {

	Vector buf = euler;

	if (buf.x > 180*DEGREES) buf.x = buf.x - 180.0f*DEGREES;
	else if (buf.x < -180*DEGREES) buf.x = buf.x + 180.0f*DEGREES;

	if (buf.y > 180*DEGREES) buf.y = buf.y - 180.0f*DEGREES;
	else if (buf.y < -180*DEGREES) buf.y = buf.y + 180.0f*DEGREES;

	if (buf.z > 180*DEGREES) buf.z = buf.z - 180.0f*DEGREES;
	else if (buf.z < -180*DEGREES) buf.z = buf.z + 180.0f*DEGREES;

	return buf;

}


void IMUThread::processData() {

	//Calculate time since last process
	float dTime = float(NOW() - lastSensorProcessTimestamp)/SECONDS;
	lastSensorProcessTimestamp = NOW();

	//static Quaternion attitude(1,0,0,0);

	//Integrate gyro values
	sensorData.gyroPosition = sensorData.gyroPosition + sensorData.gyro*dTime;

	//Integrate gyro values correctly
	float cosP = cos(sensorData.euler.y);
	float sinP = sin(sensorData.euler.y);
	float cosR = cos(sensorData.euler.x);
	float sinR = sin(sensorData.euler.x);

	Vector gyroWorld = 0;

	gyroWorld.x = (sensorData.gyro.y*cosR*cosP - sensorData.gyro.z*sinR*cosP)/cosP;
	gyroWorld.y = (sensorData.gyro.x*cosP + sensorData.gyro.y*sinR*sinP + sensorData.gyro.z*cosR*sinP)/cosP;
	gyroWorld.z = (sensorData.gyro.y*sinR + sensorData.gyro.z*cosR)/cosP;

	sensorData.euler = angleFix(sensorData.euler + gyroWorld*dTime);



	/*attitude = attitude*Quaternion(gyro, gyro.magnitude()*dTime);

	sensorData.euler.x = attitude.x;
	sensorData.euler.y = attitude.y;
	sensorData.euler.z = attitude.z;*/



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



