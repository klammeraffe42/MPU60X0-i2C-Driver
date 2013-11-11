#ifndef _MPU60X0_MOCKWRITER_H
#define _MPU60X0_MOCKWRITER_H

#include <inttypes.h>
#include <stdio.h>

#include "mpu60X0_i2c.h"

class MPU60X0_MockWriter
{
	private:
		FILE * file;
		
		float d_acceleration[3];
		float d_angularSpeed[3];
		float d_temperature;
	
	public:
		MPU60X0_MockWriter(FILE *);
		~MPU60X0_MockWriter();
		
		float getAccelerationAxis(const uint8_t) const;
		float getAngularSpeedAxis(const uint8_t) const;
		float getTemperature() const;
		
		void setAccelerationAxis(const uint8_t, const float);
		void setAngularSpeedAxis(const uint8_t, const float);
		void setTemperature(const float);
		
		void resetValues();
		
		void writeValueSet() const;
};

#endif
