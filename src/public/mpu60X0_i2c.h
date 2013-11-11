/*
 * struct MPU60X0_I2C *.h
 * 
 * Copyright 2013 Klammeraffe42 <klammeraffe42@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#ifndef _MPU60X0_I2C_H
#define _MPU60X0_I2C_H

#include <inttypes.h>

#ifdef __cplusplus
	extern "C" { 
#endif

/**
 * The default MPU60X0 I2C slave address mask.
 * 
 * The address of MPU60X0 depends on the state of the address input pin
 * (ADR0). The pin state defines the state of the second least 
 * significant bit in the I2C slave address.
 * 
 * To retrieve the full I2C address use (0x68 | (ADR0 ? 0x02 : 0x00)) 
 * with ADR0 as value of the address input pin. 
 */
#define MPU60X0_I2C_SLAVE_ADDR 0x68u

/**
 * Defines the axes of the sensor.
 */
enum MPU60X0_AXIS {
	MPU60X0_AXIS_X = 0,
	MPU60X0_AXIS_Y = 1,
	MPU60X0_AXIS_Z = 2
};

/**
 * Defines the accelerometer full-scale range in G=9.81m/s²
 */
enum MPU60X0_ACCELERATION_WORKING_RANGE {
	MPU60X0_ACCELERATION_WORKING_RANGE_2G = 0,
	MPU60X0_ACCELERATION_WORKING_RANGE_4G = 1,
	MPU60X0_ACCELERATION_WORKING_RANGE_8G = 2,
	MPU60X0_ACCELERATION_WORKING_RANGE_16G = 3
};

/**
 * Defines the gyroscope full-scale range in degrees per second
 */
enum MPU60X0_ANGULAR_SPEED_WORKING_RANGE {
	MPU60X0_ANGULAR_SPEED_WORKING_RANGE_250 = 0,
	MPU60X0_ANGULAR_SPEED_WORKING_RANGE_500 = 1,
	MPU60X0_ANGULAR_SPEED_WORKING_RANGE_1000 = 2,
	MPU60X0_ANGULAR_SPEED_WORKING_RANGE_2000 = 3
};

/**
 * Creates a new device object pointer. Use this object to communicate
 * with the device.
 * 
 * @param The device i2c address
 * @return device object pointer
 */
extern struct MPU60X0_I2C * mpu60X0_i2c_new(const uint8_t);

/**
 * Destroys a device object.
 * 
 * @param The object to destroy
 */
extern void mpu60X0_i2c_free(struct MPU60X0_I2C *);

/**
 * Returns the device identifier of a MPU60X0 sensor.
 * 
 * @param The device object.
 * @return The device identifier.
 */
extern uint8_t mpu60X0_i2c_whoAmI(struct MPU60X0_I2C *);

/**
 * Returns a 1-dimensional linear acceleration from the device object.
 * 
 * @param The device object.
 * @param The axis to read.
 * @return The acceleration in direction the direction specified.
 */
extern float mpu60X0_i2c_getAccelerationAxis(struct MPU60X0_I2C *, const enum MPU60X0_AXIS);

/**
 * Returns the linear acceleration in all three dimensions.
 * 
 * @param The device object.
 * @param Pointer to write to.
 */
extern void mpu60X0_i2c_getAcceleration(struct MPU60X0_I2C *, float *);


/**
 * Returns the 1-dimensional angular speed from the device object.
 * 
 * @param The device object.
 * @param The axis to read.
 * @return The .
 */
extern float mpu60X0_i2c_getAngularSpeedAxis(struct MPU60X0_I2C *, const enum MPU60X0_AXIS);

/**
 * Returns the angular speed in all three dimensions.
 * 
 * @param The device object.
 * @param The pointer to write to.
 */
extern void mpu60X0_i2c_getAngularSpeed(struct MPU60X0_I2C *, float *);

/**
 * Returns the temperatur from the device object.
 * 
 * @param The device object.
 * @return The temperatur of the sensor in °C.
 */
extern float mpu60X0_i2c_getTemperature(struct MPU60X0_I2C *);

/**
 * Sets the working range for the acceleration sensor in the device 
 * object.
 * 
 * @param The device object.
 * @param The working range.
 */
extern void mpu60X0_i2c_setAccelerationWorkingRange(struct MPU60X0_I2C *, const enum MPU60X0_ACCELERATION_WORKING_RANGE);

/**
 * Returns the working range for the acceleration sensor in the device 
 * object.
 * 
 * @param The device object.
 * @return The working range.
 */
extern enum MPU60X0_ACCELERATION_WORKING_RANGE mpu60X0_i2c_getAccelerationWorkingRange(struct MPU60X0_I2C *);

/**
 * Sets the working range for the gyroscope sensor in the device object.
 * 
 * @param The device object.
 * @param The working range.
 */
extern void mpu60X0_i2c_setAngularSpeedWorkingRange(struct MPU60X0_I2C *, const enum MPU60X0_ANGULAR_SPEED_WORKING_RANGE);

/**
 * Returns the working range for the gyroscope sensor in the device 
 * object.
 * 
 * @param The device object.
 * @param The axis to read.
 * @return The device identifier.
 */
extern enum MPU60X0_ANGULAR_SPEED_WORKING_RANGE mpu60X0_i2c_getAngularSpeedWorkingRange(struct MPU60X0_I2C *);

/**
 * Reads the device configuration to the device object.
 * 
 * @param The device object.
 */
extern void mpu60X0_i2c_readConfiguration(struct MPU60X0_I2C *);

/**
 * Writes the configuration from the device object to the sensor.
 * 
 * @param The device object.
 */
extern void mpu60X0_i2c_writeConfiguration(struct MPU60X0_I2C *);

/**
 * Reads the linear acceleration from the device to the device object.
 * 
 * @param The device object.
 */
extern void mpu60X0_i2c_readAcceleration(struct MPU60X0_I2C *);

/**
 * Reads angular speed from the device to the device object.
 * 
 * @param The device object.
 */
extern void mpu60X0_i2c_readAngularSpeed(struct MPU60X0_I2C *);

/**
 * Reads the temperatur of the sensor from the device to the device 
 * object.
 * 
 * @param The device object.
 */
extern void mpu60X0_i2c_readTemperature(struct MPU60X0_I2C *);

/**
 * Reads all data from the device to the device object.
 * 
 * @param The device object.
 */
extern void mpu60X0_i2c_readData(struct MPU60X0_I2C *);


#ifdef __cplusplus
	}
#endif
#endif
