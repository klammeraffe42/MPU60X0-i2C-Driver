/*
 * mpu60X0_i2c.h
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

#include <ctype.h>

/**
 * 
 */
enum MPU60X0_AXIS {
	MPU60X0_AXIS_X = 0,
	MPU60X0_AXIS_Y = 1,
	MPU60X0_AXIS_Z = 2
};

enum MPU60X0_ACCELERATION_WORKING_RANGE {
	MPU60X0_ACCELERATION_WORKING_RANGE_2G = 0,
	MPU60X0_ACCELERATION_WORKING_RANGE_4G = 1,
	MPU60X0_ACCELERATION_WORKING_RANGE_8G = 2,
	MPU60X0_ACCELERATION_WORKING_RANGE_16G = 3
};

enum MPU60X0_ANGULAR_SPEED_WORKING_RANGE {
	MPU60X0_ANGULAR_SPEED_WORKING_RANGE_250 = 0,
	MPU60X0_ANGULAR_SPEED_WORKING_RANGE_500 = 1,
	MPU60X0_ANGULAR_SPEED_WORKING_RANGE_1000 = 2,
	MPU60X0_ANGULAR_SPEED_WORKING_RANGE_2000 = 3
};

extern struct MPU60X0_I2C *mpu60X0_i2c_new(const uint8_t addr);

extern void mpu60X0_i2c_free(const struct MPU60X0_I2C *);

/**
 * Reads the device identifier of a MPU60X0 sensor.
 * 
 * @param The device object.
 * @return The devide identifier.
 */
extern uint8_t mpu60X0_i2c_whoAmI(const struct MPU60X0_I2C *);

extern float mpu60X0_i2c_getAccelerationAxis(const struct MPU60X0_I2C *, const enum MPU60X0_AXIS);

extern void mpu60X0_i2c_getAcceleration(const struct MPU60X0_I2C *, float *);

extern float mpu60X_i2c_getAngularSpeedAxis(const struct MPU60X0_I2C *, const enum MPU60X0_AXIS);

extern void mpu60X0_i2c_getAngularSpeed(const struct MPU60X0_I2C *, float *);

extern float mpu60X0_i2c_getTemperature(const struct MPU60X0_I2C *);

extern void mpu60X0_i2c_setAccelerationWorkingRange(const struct MPU60X0_I2C *, const enum MPU60X0_ACCELERATION_WORKING_RANGE);

extern enum MPU60X0_ACCELERATION_WORKING_RANGE mpu60X0_i2c_getAccelerationWorkingRange(const struct MPU60X0_I2C *);

extern void mpu60X0_i2c_setAngularSpeedWorkingRange(const struct MPU60X0_I2C *, const enum MPU60X0_ANGULAR_SPEED_WORKING_RANGE);

extern enum MPU60X0_ANGULAR_SPEED_WORKING_RANGE mpu60X0_i2c_getAngularSpeedWorkingRange(const struct MPU60X0_I2C *);


extern void mpu60X0_i2c_readConfiguration(const struct MPU60X0_I2C *);

extern void mpu60X0_i2c_writeConfiguration(const struct MPU60X0_I2C *);

extern void mpu60X0_i2c_readAcceleration(const struct MPU60X0_I2C *);

extern void mpu60X0_i2c_readAngularSpeed(const struct MPU60X0_I2C *);

extern void mpu60X0_i2c_readTemperature(const struct MPU60X0_I2C *);

extern void mpu60X0_i2c_readData(const struct MPU60X0_I2C *);

#endif
