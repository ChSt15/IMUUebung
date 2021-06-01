#ifndef LSM9DS1_DRIVER_BASE_H
#define LSM9DS1_DRIVER_BASE_H

#include "stdint.h"

namespace LSM9DS1 {

    //Accel/gyro address
    const uint8_t AG_ADDRESS = 0x6B;
    //Mag address
    const uint8_t M_ADDRESS = 0x1E;
    //Accel/Gyro registers
	const uint8_t WHOAMI_AG = 0x0F;
	const uint8_t CTRL_REG1_G = 0x10;
	const uint8_t CTRL_REG6_XL = 0x20;
	const uint8_t CTRL_REG8 = 0x22;
	const uint8_t STATUS_REG_AG = 0x17;
	const uint8_t OUT_GYRO_REG_START = 0x18;
	const uint8_t OUT_ACCEL_REG_START = 0x28;
	const uint8_t OUT_TEMP_REG_START = 0x15;
	//Mag registers
	const uint8_t WHOAMI_M = 0x0F;
	const uint8_t STATUS_REG_M = 0x27;
	const uint8_t CTRL_REG1_M = 0x20;
	const uint8_t CTRL_REG2_M = 0x21;
	const uint8_t CTRL_REG3_M = 0x22;
	const uint8_t CTRL_REG4_M = 0x23;
	const uint8_t CTRL_REG5_M = 0x24;
    const uint8_t OUT_MAG_REG_START = 0x28;


    /**
     * This is the driver base class. 
     * It needs to be inhereted from and the virtual functions writeBytes() and readBytes() need to be implemented.
     * This allows for SPI, I2C and plattform independent development.
     */
    class LSM9DS1_Driver_Base {
    public:

        bool gyroscopeAvailable();
        bool readGyroscope(float &x, float &y, float &z);

        bool accelerometerAvailable();
        bool readAccelerometer(float &x, float &y, float &z);

        bool magnetometerAvailable();
        bool readMagnetometer(float &x, float &y, float &z);

        bool temperatureAvailable();
        bool readTemperature(float &temp);

        bool initialiseM();
        bool initialiseAG();


    protected:

        /**
         * Writes bytes into registers of accel/gyro sensor.
         * @param address Address of devise to write to.
         * @param reg Starting register to be written to.
         * @param data Pointer to data to be written
         * @param numberBytes Number of bytes to write from data pointer
         * @returns true if successfull, false if failed
         */
        virtual bool writeBytes(const uint8_t &address, const uint8_t &reg, const uint8_t* data, const uint32_t &numberBytes) = 0;

        /**
         * Reads bytes from registers of accel/gyro sensor.
         * @param address Address of devise to write to.
         * @param reg Starting register to be read from.
         * @param data Pointer to where data should be placed.
         * @param numberBytes Number of bytes to read from sensor.
         * @returns true if successfull, false if failed
         */
        virtual bool readBytes(const uint8_t &address, const uint8_t &reg, uint8_t* data, const uint32_t &numberBytes) = 0;

        /**
         * Writes bytes into registers of accel/gyro sensor.
         * @param reg Starting register to be written to.
         * @param data Pointer to data to be written
         * @param numberBytes Number of bytes to write from data pointer
         * @returns true if successfull, false if failed
         */
        inline bool writeBytesAG(const uint8_t &reg, const uint8_t* data, const uint32_t &numberBytes) {
            return writeBytes(AG_ADDRESS, reg, data, numberBytes);
        }

        /**
         * Reads bytes from registers of accel/gyro sensor.
         * @param reg Starting register to be read from.
         * @param data Pointer to where data should be placed.
         * @param numberBytes Number of bytes to read from sensor.
         * @returns true if successfull, false if failed
         */
        inline bool readBytesAG(const uint8_t &reg, uint8_t* data, const uint32_t &numberBytes) {
            return readBytes(AG_ADDRESS, reg, data, numberBytes);
        }

        /**
         * Writes a byte into register of accel/gyro sensor.
         * @param reg Register to be written to.
         * @param data Data to be written.
         * @returns true if successfull, false if failed
         */
        inline bool writeByteAG(const uint8_t &reg, const uint8_t &data) {
            return writeBytesAG(reg, &data, 1);
        }

        /**
         * Reads a byte from register of accel/gyro sensor.
         * @param reg Register to be read from.
         * @param data Pointer of where byte will be placed. 
         * @returns true if successfull, false if failed
         */
        inline bool readByteAG(const uint8_t &reg, uint8_t* data) {
            return readBytesAG(reg, data, 1);
        }

        /**
         * Writes bytes into registers of mag sensor.
         * @param reg Starting register to be written to.
         * @param data Pointer to data to be written
         * @param numberBytes Number of bytes to write from data pointer
         * @returns true if successfull, false if failed
         */
        inline bool writeBytesM(const uint8_t &reg, const uint8_t* data, const uint32_t &numberBytes) {
            return writeBytes(M_ADDRESS, reg, data, numberBytes);
        }

        /**
         * Reads bytes from registers of mag sensor.
         * @param reg Starting register to be read from.
         * @param data Pointer to where data should be placed.
         * @param numberBytes Number of bytes to read from sensor.
         * @returns true if successfull, false if failed
         */
        inline bool readBytesM(const uint8_t &reg, uint8_t* data, const uint32_t &numberBytes) {
            return readBytes(M_ADDRESS, reg, data, numberBytes);
        }

        /**
         * Writes a byte into register of mag sensor.
         * @param reg Register to be written to.
         * @param data Data to be written.
         * @returns true if successfull, false if failed
         */
        inline bool writeByteM(const uint8_t &reg, const uint8_t &data) {
            return writeBytesM(reg, &data, 1);
        }

        /**
         * Reads a byte from register of mag sensor.
         * @param reg Register to be read from.
         * @param data Pointer of where byte will be placed. 
         * @returns true if successfull, false if failed
         */
        inline bool readByteM(const uint8_t &reg, uint8_t* data) {
            return readBytesM(reg, data, 1);
        }

    };

}




#endif
