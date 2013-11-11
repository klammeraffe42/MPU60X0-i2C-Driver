#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "mpu60X0_i2c.h"

#ifdef __cplusplus
extern "C" { 
#endif

struct MPU60X0_I2C {
	uint8_t c_addr;
	enum MPU60X0_ACCELERATION_WORKING_RANGE c_accelerationWorkingRange;
	enum MPU60X0_ANGULAR_SPEED_WORKING_RANGE c_angularSpeedWorkingRange;
	
	float d_acceleration[3];
	float d_angularSpeed[3];
	float d_temperature;
	
	uint32_t b_value;
	float b_values[7];
	enum MPU60X0_ACCELERATION_WORKING_RANGE b_accelerationWorkingRange;
	enum MPU60X0_ANGULAR_SPEED_WORKING_RANGE b_angularSpeedWorkingRange;
};


struct MPU60X0_I2C * mpu60X0_i2c_new(const uint8_t addr) {
	struct MPU60X0_I2C * result = malloc(sizeof(*result));
	
	if (result) {
		result->c_addr = addr;
		result->c_accelerationWorkingRange = MPU60X0_ACCELERATION_WORKING_RANGE_2G;
		result->c_angularSpeedWorkingRange = MPU60X0_ANGULAR_SPEED_WORKING_RANGE_250;
		
		result->d_acceleration[0] = 0.0;
		result->d_acceleration[1] = 0.0;
		result->d_acceleration[2] = 0.0;
		result->d_angularSpeed[0] = 0.0;
		result->d_angularSpeed[1] = 0.0;
		result->d_angularSpeed[2] = 0.0;
		result->d_temperature = 0.0;
		
		result->b_value = 0;
	}
	
	return result;
}

/**
 * Destroys a device object.
 * 
 * @param The object to destroy
 */
inline void mpu60X0_i2c_free(struct MPU60X0_I2C * o) {
	if(o) {
		free(o);
	}
}

/**
 * Returns the device identifier of a MPU60X0 sensor.
 * 
 * @param The device object.
 * @return The device identifier.
 */
inline uint8_t mpu60X0_i2c_whoAmI(struct MPU60X0_I2C * o) {
	assert(o);
	
	return MPU60X0_I2C_SLAVE_ADDR;
}

/**
 * Returns a 1-dimensional linear acceleration from the device object.
 * 
 * @param The device object.
 * @param The axis to read.
 * @return The acceleration in direction the direction specified.
 */
inline float mpu60X0_i2c_getAccelerationAxis(struct MPU60X0_I2C * o, const enum MPU60X0_AXIS a) {
	assert(o);
	assert(a < 3);
	
	return o->d_acceleration[a];
}

/**
 * Returns the linear acceleration in all three dimensions.
 * 
 * @param The device object.
 * @param Pointer to write to.
 */
inline void mpu60X0_i2c_getAcceleration(struct MPU60X0_I2C * o, float *d) {
	assert(o);
	assert(d);
	
	memcpy((char *) (void *) d, (char *) (void *) o->d_acceleration,
		3*sizeof(*d));
}


/**
 * Returns the 1-dimensional angular speed from the device object.
 * 
 * @param The device object.
 * @param The axis to read.
 * @return The .
 */
inline float mpu60X0_i2c_getAngularSpeedAxis(struct MPU60X0_I2C * o, const enum MPU60X0_AXIS a) {
	assert(o);
	assert(a < 3);
	
	return o->d_angularSpeed[a];
}

/**
 * Returns the angular speed in all three dimensions.
 * 
 * @param The device object.
 * @param The pointer to write to.
 */
inline void mpu60X0_i2c_getAngularSpeed(struct MPU60X0_I2C * o, float *d) {
	assert(o);
	assert(d);
	
	memcpy((char *) (void *) d, (char *) (void *) o->d_angularSpeed,
		3*sizeof(*d));
}

/**
 * Returns the temperatur from the device object.
 * 
 * @param The device object.
 * @return The temperatur of the sensor in °C.
 */
inline float mpu60X0_i2c_getTemperature(struct MPU60X0_I2C * o) {
	assert(o);
	
	return o->d_temperature;
}

/**
 * Sets the working range for the acceleration sensor in the device 
 * object.
 * 
 * @param The device object.
 * @param The working range.
 */
inline void mpu60X0_i2c_setAccelerationWorkingRange(struct MPU60X0_I2C * o, const enum MPU60X0_ACCELERATION_WORKING_RANGE r) {
	assert(o);
	
	o->c_accelerationWorkingRange = r;
}

/**
 * Returns the working range for the acceleration sensor in the device 
 * object.
 * 
 * @param The device object.
 * @return The working range.
 */
inline enum MPU60X0_ACCELERATION_WORKING_RANGE mpu60X0_i2c_getAccelerationWorkingRange(struct MPU60X0_I2C * o) {
	assert(o);
	
	return o->c_accelerationWorkingRange;
}

/**
 * Sets the working range for the gyroscope sensor in the device object.
 * 
 * @param The device object.
 * @param The working range.
 */
inline void mpu60X0_i2c_setAngularSpeedWorkingRange(struct MPU60X0_I2C * o, const enum MPU60X0_ANGULAR_SPEED_WORKING_RANGE r) {
	assert(o);
	
	o->c_angularSpeedWorkingRange = r;
}

/**
 * Returns the working range for the gyroscope sensor in the device 
 * object.
 * 
 * @param The device object.
 * @param The axis to read.
 * @return The device identifier.
 */
inline enum MPU60X0_ANGULAR_SPEED_WORKING_RANGE mpu60X0_i2c_getAngularSpeedWorkingRange(struct MPU60X0_I2C * o) {
	assert(o);
	
	return o->c_angularSpeedWorkingRange;
}

/**
 * Reads the device configuration to the device object.
 * 
 * @param The device object.
 */
inline void mpu60X0_i2c_readConfiguration(struct MPU60X0_I2C * o) {
	assert(o);
	
	o->c_accelerationWorkingRange = o->b_accelerationWorkingRange;
	o->c_angularSpeedWorkingRange = o->b_accelerationWorkingRange;
}

/**
 * Writes the configuration from the device object to the sensor.
 * 
 * @param The device object.
 */
inline void mpu60X0_i2c_writeConfiguration(struct MPU60X0_I2C * o) {
	assert(o);
	
	o->b_accelerationWorkingRange = o->c_accelerationWorkingRange;
	o->b_angularSpeedWorkingRange = o->c_accelerationWorkingRange;
}

static void mpu60X0_i2c_genNextDummyValues(struct MPU60X0_I2C * o) {
	assert(o);
	
	o->b_values[0] = 1.0 * o->b_value / (o->c_accelerationWorkingRange + 1);
	o->b_values[1] = -1.0 * o->b_value / (o->c_accelerationWorkingRange + 1);
	o->b_values[2] = 4.0 * o->b_value / (o->c_accelerationWorkingRange + 1);

	o->b_values[6] = 20.0 + sin(0.1 * o->b_value);
	
	++o->b_value;
}

/**
 * Reads the linear acceleration from the device to the device object.
 * 
 * @param The device object.
 */
void mpu60X0_i2c_readAcceleration(struct MPU60X0_I2C * o) {
	assert(o);
	
	mpu60X0_i2c_genNextDummyValues(o);
	
	o->d_acceleration[0] = o->b_values[0];
	o->d_acceleration[1] = o->b_values[1];
	o->d_acceleration[2] = o->b_values[2];
}

/**
 * Reads angular speed from the device to the device object.
 * 
 * @param The device object.
 */
void mpu60X0_i2c_readAngularSpeed(struct MPU60X0_I2C * o) {
	assert(o);
	
	mpu60X0_i2c_genNextDummyValues(o);
	
	o->d_angularSpeed[0] = o->b_values[3];
	o->d_angularSpeed[1] = o->b_values[4];
	o->d_angularSpeed[2] = o->b_values[5];
}

/**
 * Reads the temperatur of the sensor from the device to the device 
 * object.
 * 
 * @param The device object.
 */
void mpu60X0_i2c_readTemperature(struct MPU60X0_I2C * o) {
	assert(o);
	
	mpu60X0_i2c_genNextDummyValues(o);
	
	o->d_temperature = o->b_values[6];
}

/**
 * Reads all data from the device to the device object.
 * 
 * @param The device object.
 */
void mpu60X0_i2c_readData(struct MPU60X0_I2C * o) {
	assert(o);
	
	mpu60X0_i2c_genNextDummyValues(o);
	
	o->d_acceleration[0] = o->b_values[0];
	o->d_acceleration[1] = o->b_values[1];
	o->d_acceleration[2] = o->b_values[2];
	o->d_angularSpeed[0] = o->b_values[3];
	o->d_angularSpeed[1] = o->b_values[4];
	o->d_angularSpeed[2] = o->b_values[5];
	o->d_temperature = o->b_values[6];
}

#ifdef __cplusplus
}
#endif
