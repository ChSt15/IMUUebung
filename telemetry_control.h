#pragma once



#include "rodos.h"
#include "hal.h"
#include "stdio.h"

#include "3d_math.h"



namespace RODOS {extern HAL_UART uart_stdout;}
#define TeleURAT uart_stdout





struct DataPacket{
	char packetID = 0;
	float packetData = 0;
};


struct SensorData{

	//Raw sensor values
	Vector gyro; // radian/sec
	Vector accel; // m/s^2
	Vector mag; // Gauss

	//Integration of gyro values. Relative to start attitude.
	Vector gyroPosition;

	//Acceleration angles in radian.
	float roll_acc, pitch_acc;

	//Heading from magnetometer in radian from north-east-positive 0 to 2*PI.
	float heading_mag;

	//Euler. X-Roll, Y-Pitch, Z-Yaw/heading
	Vector euler;

	//Temperature in °C
	float temp;

};


extern Topic<DataPacket> dataPacketReceivedTopic;

extern Topic<DataPacket> dataPacketTransmitTopic;
extern Fifo<DataPacket, 10> dataPacketTransmittBuffer;

extern Topic<SensorData> sensorDataTransmitTopic;
extern Fifo<SensorData, 10> sensorDataTransmittBuffer;



class TelemetryTransmitter:public Thread {
public:

	void init() {

		uart_stdout.init();
		uart_stdout.config(UART_PARAMETER_ENABLE_DMA, 13);

	}

	void run() {
		Vector vec;
		int interval = 100; //ms
		char wasDrucken = 'A'; //Was soll gedruckt werden

		while (1) {

			//Update Interval (Eigentlich Thread 3)
			DataPacket datenPacket;
			if (dataPacketTransmittBuffer.get(datenPacket)) {
				if(datenPacket.packetID == 'I'){
					interval = datenPacket.packetData;
				} else if(datenPacket.packetID == 'A'){
					wasDrucken = datenPacket.packetData;
				} else if(datenPacket.packetID == 'M'){
					wasDrucken = datenPacket.packetData;
				} else if(datenPacket.packetID == 'G'){
					wasDrucken = datenPacket.packetData;
				}
			}

			suspendCallerUntil(NOW() + interval*MILLISECONDS);

			SensorData packet;
			if (sensorDataTransmittBuffer.get(packet)) {

				if (wasDrucken == 'G'){
				//Gyroscope:
				float gx = packet.gyro.x * (180/PI);
				float gy = packet.gyro.y * (180/PI);
				float gz = packet.gyro.z * (180/PI);
				float pitch = packet.gyroPosition.x * (180/PI);
				float roll = packet.gyroPosition.y * (180/PI);
				float yaw = packet.gyroPosition.z * (180/PI);

				PRINTF("Gyroscope is successfully read\ngx[deg/s] = %.4f , gy[deg/s] = %.4f , gz[deg/s] = %.4f\nThe Pitch = %.4fdeg\nThe Roll = %.4fdeg\n, The Yaw = %.4fdeg\nThe Temperature is = %.4f°C\n----------------------------------\n", gx, gy, gz, pitch, roll, yaw, packet.temp);
				}

				if (wasDrucken == 'M'){
				//Magnetometer
				float pitch_acc = packet.pitch_acc * (180/PI);
				float roll_acc = packet.roll_acc * (180/PI);
				float mx = packet.mag.x;
				float my = packet.mag.y;
				float mz = packet.mag.z;
				float angleMag = packet.heading_mag * (180/PI);

				PRINTF("Orientation Angle of Accelerometer is pitch_acc[deg] = %.4f and roll_acc[deg] = %.4f\nmx[Gauss] = %.4f, my[Gauss] = %.4f, mz[Gauss] = %.4f\nOrientation angle of Magnetometer is %.4fdeg\n Accelerometer is successfully read\n----------------------------------------------\n", pitch_acc, roll_acc, mx, my, mz, angleMag);
				}

				if (wasDrucken == 'A'){
				//Accelerometer
				float pitch_acc = packet.pitch_acc * (180/PI);
				float roll_acc = packet.roll_acc * (180/PI);
				float ax = packet.accel.x;
				float ay = packet.accel.y;
				float az = packet.accel.z;
				PRINTF("Orientation Angle of Accelerometer is pitch_acc[deg] = %.4f and roll_acc[deg] = %.4f\nAccelerometer is successfully read\nax[m/s^2] = %.4f, ay[m/s^2] = %.4f, az[m/s^2] = %.4f\n-------------------------------------\n", pitch_acc, roll_acc, ax, ay, az);
				}

			}

		}

	}

};



class TelemetryReceiver:public Thread {
private:

	char packet[20];
	int packetSize = 0;
	int numberBytes = 0;

	long long startTimestamp = 0;
public:

	void init() {
		startTimestamp=NOW();
		//uart_stdout.init();
		//uart_stdout.config(UART_PARAMETER_ENABLE_DMA, 1);

	}

	void run() {


		//while (1) {
		TIME_LOOP(NOW(),5000*MILLISECONDS){

			startTimestamp=NOW();
			//Reactivation time at 5000ms to check if timeout has occured.
			uart_stdout.suspendUntilDataReady(NOW()+5000*MILLISECONDS);
			packetSize=(int)(NOW()/MILLISECONDS);
			//Check if data received.
			if (uart_stdout.isDataReady()) {

				char byte[13];
				char begin,end;
				float number;
				char messageType;

				//Read 13bytes data in buffer
				uart_stdout.read(byte, 13);

				sscanf(byte, "%c%c%f%c", &begin, &messageType, &number, &end);

				if (begin == '$' && end == '#') {
					PRINTF("The recieved Message T=%c and Data =%f\n", messageType, number);
					DataPacket packet;

					packet.packetID = messageType;
					packet.packetData = number;

					dataPacketReceivedTopic.publish(packet);
				} else {
					PRINTF("Message Corrupted\n");
				}


			//Check if timed out.
			} else if (NOW() - startTimestamp >= 5000*MILLISECONDS) {

				numberBytes = (int)(startTimestamp/MILLISECONDS);
				startTimestamp = NOW();

				PRINTF("Message Timeout\n");// vor %d n%d now%d\n",numberBytes,packetSize,(int)(startTimestamp/MILLISECONDS));

			}

		}

	}

};

