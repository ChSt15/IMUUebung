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

	Vector angularVel;
	Vector angularPos;
	Vector linearAccel;

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
		//Starting values for the variables
		float a = 0.0;
		float b = 0.0;
		float s = 0.0;
		float c = 0.0;
		float d = 0.0;

		while (1) {

			suspendCallerUntil(NOW() + 100*MILLISECONDS);
			//for (int var = 0; var < 5; ++var) {
				DataPacket packet;
				//if (dataPacketTransmittBuffer.getOnlyIfNewData(packet)) {
				if (dataPacketTransmittBuffer.get(packet)) {

					//Send new packet
					if (packet.packetID == 'S'){
						s = packet.packetData;
					}
					if (packet.packetID =='A'){
						a = packet.packetData;
					}
					if (packet.packetID =='B'){
						b = packet.packetData;
					}
					if (packet.packetID =='C'){
						c = packet.packetData;
					}
					if (packet.packetID =='D'){
						d = packet.packetData;
					}

					PRINTF("Counter = %.2f, StepSize = %.2f, ActualRunningTime[s] = %.2f, Thread1 t1 = %.0f, Thread2 t2 = %.0f\n", c, s, d, a, b);


					//PRINTF("package ID = %c, data = %f \n", packet.packetID, packet.packetData);
				}
			//}
			//PRINTF("Counter = %.2f, StepSize = %.2f, ActualRunningTime[s] = %.2f, Thread1 t1 = %.2f, Thread2 t2 = %.2f\n", c, s, d, a, b);

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




