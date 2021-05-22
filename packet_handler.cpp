#include "packet_handler.h"


Topic<ThreadInterval> threadIntervalCommandTopic(-1, "Thread interval");
Topic<float> stepSizeCommandTopic(-1, "Thread 1 step size");


//Subscriber
CommBuffer<DataPacket> dataPacketBuffer;
Subscriber dataPacketSubscriber(dataPacketReceivedTopic, dataPacketBuffer);
DataPacket packet;


PacketHandler packetHandler;
