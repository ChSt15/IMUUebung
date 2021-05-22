#include "telemetry_control.h"


Topic<DataPacket> dataPacketReceivedTopic(-1, "Data Packet Received");

Topic<DataPacket> dataPacketTransmitTopic(-1, "Data Packet Transmit");
//CommBuffer<DataPacket> dataPacketTransmittBuffer;
Fifo<DataPacket, 10> dataPacketTransmittBuffer;
Subscriber dataPacketTransmittSubscriber(dataPacketTransmitTopic, dataPacketTransmittBuffer);


TelemetryReceiver telemetryReceiver;

TelemetryTransmitter telemetryTransmitter;
