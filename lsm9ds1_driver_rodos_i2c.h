#ifndef LSM9DS1_DRIVER_RODOS_I2C_H
#define LSM9DS1_DRIVER_RODOS_I2C_H


#include "lsm9ds1_driver_base.h"

#include "rodos.h"
#include "hal_gpio.h"
#include "hal_i2c.h"


/**
 * Implementation for arduino
 */
class LSM9DS1_Driver_RodosI2C: public LSM9DS1::LSM9DS1_Driver_Base {
public:

	LSM9DS1_Driver_RodosI2C(HAL_I2C* bus) {
		bus_ = bus;
	}


protected:

    /**
     * Writes bytes into registers of accel/gyro sensor.
     * @param address Address of devise to write to.
     * @param reg Starting register to be written to.
     * @param data Pointer to data to be written
     * @param numberBytes Number of bytes to write from data pointer
     * @returns true if successfull, false if failed
     */
    bool writeBytes(const uint8_t &address, const uint8_t &reg, const uint8_t* data, const uint32_t &numberBytes) override {

        uint8_t bytes[numberBytes + 1];
		bytes[0] = reg;
		for (uint32_t i = 0; i < numberBytes; i++) bytes[i+1] = data[i];
		bus_->write(address, bytes, numberBytes + 1);

        return true;

    };

    /**
     * Reads bytes from registers of accel/gyro sensor.
     * @param address Address of devise to write to.
     * @param reg Starting register to be read from.
     * @param data Pointer to where data should be placed.
     * @param numberBytes Number of bytes to read from sensor.
     * @returns true if successfull, false if failed
     */
    bool readBytes(const uint8_t &address, const uint8_t &reg, uint8_t* data, const uint32_t &numberBytes) override {

    	bus_->writeRead(address, &reg, 1, data, numberBytes);

        return true;

    };


private:

    HAL_I2C* bus_;


};



#endif
