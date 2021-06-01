#include "lsm9ds1_driver_base.h"


using namespace LSM9DS1;



bool LSM9DS1_Driver_Base::readGyroscope(float &x, float &y, float &z) {

    bool dataIsNew = gyroscopeAvailable();

    uint8_t dataBuffer[6];

    readBytesAG(LSM9DS1::OUT_GYRO_REG_START, dataBuffer, 6);

    int16_t xBuf = static_cast<int16_t>(dataBuffer[1])<<8 | dataBuffer[0];
    int16_t yBuf = static_cast<int16_t>(dataBuffer[3])<<8 | dataBuffer[2];
    int16_t zBuf = static_cast<int16_t>(dataBuffer[5])<<8 | dataBuffer[4];

    x = float(xBuf)*0.07f*3.14159265f/180.0f;
    y = float(yBuf)*0.07f*3.14159265f/180.0f;
    z = float(zBuf)*0.07f*3.14159265f/180.0f;

    return dataIsNew;

}


bool LSM9DS1_Driver_Base::readAccelerometer(float &x, float &y, float &z) {

    bool dataIsNew = accelerometerAvailable();

    uint8_t dataBuffer[6];

    readBytesAG(LSM9DS1::OUT_ACCEL_REG_START, dataBuffer, 6);

    int16_t xBuf = static_cast<int16_t>(dataBuffer[1])<<8 | dataBuffer[0];
	int16_t yBuf = static_cast<int16_t>(dataBuffer[3])<<8 | dataBuffer[2];
	int16_t zBuf = static_cast<int16_t>(dataBuffer[5])<<8 | dataBuffer[4];

    x = -9.81*static_cast<float>(xBuf)*0.061f*0.001;
    y = -9.81*static_cast<float>(yBuf)*0.061f*0.001;
    z = -9.81*static_cast<float>(zBuf)*0.061f*0.001;

    return dataIsNew;

}


bool LSM9DS1_Driver_Base::readMagnetometer(float &x, float &y, float &z) {

    bool dataIsNew = magnetometerAvailable();

    uint8_t dataBuffer[6];

    readBytesM(LSM9DS1::OUT_MAG_REG_START, dataBuffer, 6);

    int16_t xBuf = static_cast<int16_t>(dataBuffer[1])<<8 | dataBuffer[0];
	int16_t yBuf = static_cast<int16_t>(dataBuffer[3])<<8 | dataBuffer[2];
	int16_t zBuf = static_cast<int16_t>(dataBuffer[5])<<8 | dataBuffer[4];

    x = -static_cast<float>(xBuf)*0.14f*0.001;
    y = static_cast<float>(yBuf)*0.14f*0.001;
    z = -static_cast<float>(zBuf)*0.14f*0.001;

    return dataIsNew;

}


bool LSM9DS1_Driver_Base::readTemperature(float &temp) {

    bool dataIsNew = temperatureAvailable();

    int8_t dataBuffer[2];

    readBytesAG(LSM9DS1::OUT_TEMP_REG_START, (uint8_t*)dataBuffer, 2);

    int16_t tempBuf = dataBuffer[0] + static_cast<int16_t>(dataBuffer[1])<<8;
    temp = -float(tempBuf)/16.0f/100.0f + 25.0f;

    return dataIsNew;

}


bool LSM9DS1_Driver_Base::initialiseAG() {

    //Buffer for reading data
    uint8_t byte;

    //Read from WhoAmI reg to confirm connection working
    if (!readByteAG(WHOAMI_AG, &byte)) {
        return false;
    }
    
    //Check if WhoAmI is correct
    if (byte != 0x68) {
        return false;
    }

    //Setup device
    if (!writeByteAG(CTRL_REG1_G, 0b10011000)) { //Set gyro ODR=238Hz, cutoff= 14Hz, Range=2000dps
        return false;
    } 

    //Signify success
    return true;
}


bool LSM9DS1_Driver_Base::initialiseM() {

    //Buffer for reading data
    uint8_t byte;

    //Read from WhoAmI reg to confirm connection working
    if (!readByteM(WHOAMI_M, &byte)) {
        return false;
    }
    
    //Check if WhoAmI is correct
    if (byte != 0b00111101) {
        return false;
    }

    //Setup device
    if (!writeByteM(CTRL_REG1_M, 0b00011100)) return false; //Set ODR 80Hz
    if (!writeByteM(CTRL_REG2_M, 0b00000000)) return false; //Set scale 4 Gauss
    if (!writeByteM(CTRL_REG3_M, 0b00000000)) return false; //Set continuous conv. mode
    //if (!writeByteM(CTRL_REG4_M, 0b00000000)) return false;
    //if (!writeByteM(CTRL_REG5_M, 0b00000000)) return false;

    //Signify success
    return true;

}


bool LSM9DS1_Driver_Base::gyroscopeAvailable() {

    uint8_t byte;

    if (!readByteAG(LSM9DS1::STATUS_REG_AG, &byte)) return false;

    return (byte>>1)&0x01 == 0x01;

}


bool LSM9DS1_Driver_Base::accelerometerAvailable() {

    uint8_t byte;

    if (!readByteAG(LSM9DS1::STATUS_REG_AG, &byte)) return false;

    return (byte>>0)&0x01 == 0x01;

}


bool LSM9DS1_Driver_Base::temperatureAvailable() {

    uint8_t byte;

    if (!readByteAG(LSM9DS1::STATUS_REG_AG, &byte)) return false;

    return (byte>>2)&0x01 == 0x01;

}


bool LSM9DS1_Driver_Base::magnetometerAvailable() {

    uint8_t byte;

    if (!readByteM(LSM9DS1::STATUS_REG_M, &byte)) return false;

    return (byte>>3)&0x01 == 0x01;

}
