#pragma once

#include "rodos.h"
#include "hal.h"
#include "stdio.h"


#include "telemetry_control.h"



struct ThreadInterval {

	uint8_t threadID = 0;
	uint32_t interval = 1;

};

//Topic
extern Topic<ThreadInterval> threadIntervalCommandTopic;
extern Topic<float> stepSizeCommandTopic;


//Subscriber
extern CommBuffer<DataPacket> dataPacketBuffer;



class PacketHandler:public Thread {
private:
/*	//Sends time to corresponding thread
	void sendThreadIntervalCommand() {
		threadIntervalCommandTopic.publish(threadInterval);
	} */

	ThreadInterval threadInterval;
public:

	void init() {

	}

	void run() {

		while (1) {

			suspendCallerUntil(NOW() + 10*MILLISECONDS);

			//Receive packet if new packet
			DataPacket packet;
			if (dataPacketBuffer.getOnlyIfNewData(packet)) {

				//Update state machine according to new packet.
				if (packet.packetID == 'S') {

					float stepSize = packet.packetData;
					stepSizeCommandTopic.publish(stepSize);

				} else if (packet.packetID == 'A') {

					ThreadInterval threadInterval;
					threadInterval.threadID = 1;
					threadInterval.interval = packet.packetData;
					//sendThreadIntervalCommand();
					threadIntervalCommandTopic.publish(threadInterval);

				} else if (packet.packetID == 'B') {

					ThreadInterval threadInterval;
					threadInterval.threadID = 2;
					threadInterval.interval = packet.packetData;
					//sendThreadIntervalCommand();
					threadIntervalCommandTopic.publish(threadInterval);

				}

			}

		}

	}

};
