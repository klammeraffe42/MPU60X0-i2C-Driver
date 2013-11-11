#include <assert.h>

#include "mpu60X0_mockwriter.hpp"


MPU60X0_MockWriter::MPU60X0_MockWriter(FILE *out):
	file(out)
{
}

MPU60X0_MockWriter::~MPU60X0_MockWriter()
{
}

float MPU60X0_MockWriter::getAccelerationAxis(const uint8_t axis) const
{
	assert(axis < 3);
	return this->d_acceleration[axis];
}

float MPU60X0_MockWriter::getAngularSpeedAxis(const uint8_t axis) const
{
	assert(axis < 3);
	return this->d_angularSpeed[axis];
}

float MPU60X0_MockWriter::getTemperature() const
{
	return this->d_temperature;
}


void MPU60X0_MockWriter::setAccelerationAxis(const uint8_t axis, const float val)
{
	assert(axis < 3);
	this->d_acceleration[axis] = val;
}

void MPU60X0_MockWriter::setAngularSpeedAxis(const uint8_t axis, const float val)
{
	assert(axis < 3);
	this->d_angularSpeed[axis] = val;
}

void MPU60X0_MockWriter::setTemperature(const float val) 
{
	this->d_temperature = val;
}

void MPU60X0_MockWriter::resetValues() 
{
	uint8_t i = 0;
	for(i=0; i<3; ++i) {
		this->d_acceleration[i] = 0.0;
		this->d_angularSpeed[i] = 0.0;
	}
	this->d_temperature = 0.0;
}

void MPU60X0_MockWriter::writeValueSet() const
{
	fprintf(this->file, "%.2f;%.2f;%.2f;%.2f;%.2f;%.2f;%.2f\n",
		this->d_acceleration[0], this->d_acceleration[1],
		this->d_acceleration[2], this->d_angularSpeed[0],
		this->d_angularSpeed[1], this->d_angularSpeed[2],
		this->d_temperature);
}
