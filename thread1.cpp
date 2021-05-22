#include "rodos.h"
#include "hal_gpio.h"

#include "packet_handler.h"



HAL_GPIO red(GPIO_062);

//The 2 Buffers needed for Interval and StepSize
CommBuffer<ThreadInterval> thread2IntervalBuffer;
Subscriber thread2IntervalSubscriber(threadIntervalCommandTopic, thread2IntervalBuffer);

CommBuffer<float> stepsizeBuffer;
Subscriber stepsizeSubscriber(stepSizeCommandTopic, stepsizeBuffer);


class Thread1:public Thread {
private:

	float counter = 0;
	float stepSize = 1;

	uint32_t interval = 3000;
public:

	void init() {
		red.init(1,1,1);
	}

	void run() {

		while (1) {

			//Only check every
			suspendCallerUntil(NOW() + interval*MILLISECONDS);

			//Receive packet if new packet for Interval of thread 1
			ThreadInterval time;
			if (thread2IntervalBuffer.getOnlyIfNewData(time)) {

				//So that we get the Interval of thread 1
				if (time.threadID == 1) {
					interval = time.interval;

				}

			}

			//Receive packet if new packet for StepSize
			float stepSizeBuf;
			if (stepsizeBuffer.getOnlyIfNewData(stepSizeBuf)) {

				stepSize = stepSizeBuf;

			}

			//Increment counter
			counter += stepSize;

			//Create and send command
			DataPacket packet2;
			packet2.packetID = 'A';
			packet2.packetData = interval;

			//Create and send command
			DataPacket packet;
			packet.packetID = 'S';
			packet.packetData = stepSize;

			DataPacket packet3;
			packet3.packetID = 'C';
			packet3.packetData = counter;

			DataPacket packet4;
			packet4.packetID = 'D';
			packet4.packetData = NOW()/SECONDS;

			dataPacketTransmitTopic.publish(packet);
			dataPacketTransmitTopic.publish(packet2);
			dataPacketTransmitTopic.publish(packet3);
			dataPacketTransmitTopic.publish(packet4);

			//toggle LED
			red.setPins(!red.readPins());

			//So that Thread does not suspend until forever when it is set to 0
			if (interval == 0){
				suspendCallerUntil(NOW() + 10000*MILLISECONDS);
				interval = 3000;
			}

		}

	}

};


Thread1 thread1;
