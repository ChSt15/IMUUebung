#include "rodos.h"
#include "hal_gpio.h"

#include "packet_handler.h"


HAL_GPIO blue(GPIO_063);

//Buffer for Interval
CommBuffer<ThreadInterval> threadIntervalBuffer;
Subscriber threadIntervalSubscriber(threadIntervalCommandTopic, threadIntervalBuffer);


class Thread2:public Thread {
private:
	uint32_t interval = 2000;

public:

	void init() {
		blue.init(1,1,1);
	}

	void run() {

		while (1) {

			//Only check every
			suspendCallerUntil(NOW() + interval*MILLISECONDS);

			//Receive packet if new packet of Interval thread 2
			ThreadInterval time;
			if (threadIntervalBuffer.getOnlyIfNewData(time)) {

				//So that we get the Interval of thread 2
				if (time.threadID == 2) {
					interval = time.interval;
				}

			}



			//Create and send command
			DataPacket packet;
			packet.packetID = 'B';
			packet.packetData = interval;

			dataPacketTransmitTopic.publish(packet);


			//toggle LED
			blue.setPins(!blue.readPins());

			//So that Thread does not suspend until forever when it is set to 0
			if (interval == 0){
				suspendCallerUntil(NOW() + 10000*MILLISECONDS);
				interval = 2000;
			}


		}

	}

};


Thread2 thread2;
