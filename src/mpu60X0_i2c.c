

struct MPU60X0_I2C {
	uint8_t c_addr;
	uint8_t c_accelerationWorkingRange;
	uint8_t c_angularSpeedWorkingRange;
	
	float d_acceleration[3];
	float d_angularSpeed[3];
	float d_temperature;
};
