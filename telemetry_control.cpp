#include "telemetry_control.h"


Topic<DataPacket> dataPacketReceivedTopic(-1, "Data Packet Received");

Topic<DataPacket> dataPacketTransmitTopic(-1, "Data Packet Transmit");
Fifo<DataPacket, 10> dataPacketTransmittBuffer;
Subscriber dataPacketTransmittSubscriber(dataPacketTransmitTopic, dataPacketTransmittBuffer);

Topic<DataPacket> dataPacketReceiveTopic(-1, "Data Packet Transmit");
Fifo<DataPacket, 10> dataPacketReceiveBuffer;
Subscriber dataPacketReceiveSubscriber(dataPacketReceivedTopic, dataPacketReceiveBuffer);

Topic<SensorData> sensorDataTransmitTopic(-1, "SensorData transmit");
Fifo<SensorData, 10> sensorDataTransmittBuffer;
Subscriber sensorTransmittSubscriber(sensorDataTransmitTopic, sensorDataTransmittBuffer);


TelemetryReceiver telemetryReceiver;

TelemetryTransmitter telemetryTransmitter;
