#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include <inttypes.h>
#include <unistd.h>
#include <string.h>

#include "mpu60X0_logger_config.h"
#include "mpu60X0_mockwriter.hpp"
#include "mpu60X0_i2c.h"


#define PROG_VERSION "1.0"


enum ErrorCode {
	ERR_OK = 0,
	
	ERR_SYNTAX = 1
};


enum LogLevel {
	LEVEL_NONE = 0,
	LEVEL_FATAL = 1,
	LEVEL_ERROR = 2,
	LEVEL_WARNING = 3,
	LEVEL_NOTICE = 4,
	LEVEL_INFO = 5
	
	#if DEBUG_MODE
		,LEVEL_DEBUG = 6
	#endif
};

static uint8_t logLevel;

#if DEBUG_MODE
	#define debug(...) log(LEVEL_DEBUG, __VA_ARGS__)
#else
	#define debug(...)
#endif

static void log(const enum LogLevel level, const char *format, ...) {
	static const char *levelNames[] = 
		#if USE_COLORS
			{
				"",
				BOLDRED"FATAL"RESET,
				RED"ERROR"RESET,
				BOLDYELLOW"WARN"RESET,
				YELLOW"NOTICE"RESET,
				BLUE"INFO"RESET,
				"DEBUG"
			};
		#else
			{
				"",
				"FATAL",
				"ERROR",
				"WARN",
				"NOTICE",
				"INFO",
				"DEBUG"
			};
		#endif
		
	assert(level > LEVEL_NONE);
	assert(level <= LEVEL_DEBUG);
	if (level & logLevel) {
		fprintf(stderr, "%-7s ", levelNames[level]);
		va_list args;
		va_start(args, format);
		vfprintf(stderr, format, args);
		va_end(args);
	}
}

/**
 * Prints the usage of the logger.
 * 
 * @param out The filehandle to write the usage to.
 * @param progName The name of the program
 */
static void printHelp(FILE *out, const char *progName) {
	fprintf(out, 
		"Usage:\n" \
			"\t%s [-a addressBit] [--addr slaveAddr] [-d delay]\n" \
				"\t\t[-s samples] [-o FILENAME] [-v]\n\n" \
			"\t%s --help\n" \
			"\t%s --version\n" \
		"Options:\n" \
			"\t-a addressBit=%u\n" \
					"\t\t\tDefines the state of the ADR0-pin of the MPU60X0 sensor.\n" \
					"\t\t\tIf the option --addr is given, this option will be\n" \
					"\t\t\tignored. The default value of the addressBit is 0.\n" \
			"\t--addr address\n" \
					"\t\t\tIf this option is set, the given 8bit address will be\n" \
					"\t\t\tused as slave address of the I2C device.\n" \
			"\t-d delay=%u\n" \
					"\t\t\tDefines the delay in microseconds between sensor\n" \
					"\t\t\treadings.\n" \
			"\t-s samples=%u\n" \
					"\t\t\tThe amount of samples to read and write into the mock\n" \
					"\t\t\tfile.\n" \
			"\t-o FILENAME\n" \
					"\t\t\tWrites the mock data into the file FILENAME\n" \
					"\t\t\tinstead of writing them to the standard output.\n" \
			"\t-v\n" \
					"\t\t\tTurns on verbose mode which prints the progress\n" \
					"\t\t\twhile reading sensor data and some basic information\n" \
					"\t\t\ton the sensor.\n" \
					"\t\t\tThis option is only available when writing to a file\n" \
					"\t\t\tspecified by the -o option\n" \
			"\t--help\n" \
					"\t\t\tPrints the help for this command.\n" \
			"\t--version\n" \
					"\t\t\tPrints the version information.\n", 
		progName, progName, progName, 
		DEFAULT_ADDRESS_BIT, DEFAULT_DELAY_MS, DEFAULT_SAMPLES);
}


static int sensorInteraction(FILE *output, uint8_t sensorAddr, uint32_t delayMS, uint32_t sampleCount) {
	struct MPU60X0_I2C *sensor = NULL;
	uint8_t sensorId = 0;
	
	// connecting to sensor
	log(LEVEL_INFO, "Connecting to I2C-device 0x%02x\n", sensorAddr);
	sensor = mpu60X0_i2c_new(sensorAddr);
	if (!sensor) {
		log(LEVEL_FATAL, "Failed to create sensor object\n");
		return 1;
	}
	debug("Connected\n");
	
	// check sensor identity
	sensorId = mpu60X0_i2c_whoAmI(sensor);
	if (sensorId != MPU60X0_I2C_SLAVE_ADDR) {
		log(LEVEL_FATAL, "Invalid device at address 0x%02x: Device ID was <0x%02x> but <0x%02x> was expected\n", sensorAddr, sensorId, MPU60X0_I2C_SLAVE_ADDR);
		return 1;
	}
	
	// create mock output writer
	MPU60X0_MockWriter writer(output);
	
	uint32_t currentSample = 0;
	while(currentSample++ < sampleCount) {
		int i;
		
		log(LEVEL_INFO, "%u/%u: read data from sensor\n", currentSample, sampleCount);
		mpu60X0_i2c_readData(sensor);
		
		for(i=0; i<3; ++i) {
			writer.setAccelerationAxis(i, mpu60X0_i2c_getAccelerationAxis(sensor, (enum MPU60X0_AXIS) i));
			writer.setAngularSpeedAxis(i, mpu60X0_i2c_getAngularSpeedAxis(sensor, (enum MPU60X0_AXIS) i));
		}
		writer.setTemperature(mpu60X0_i2c_getTemperature(sensor));
		
		writer.writeValueSet();
		
		usleep(delayMS);
	}
	
	return 0;
}

static void printSyntaxError(const char *msg, const char *progName) {
	fprintf(stderr, "Invalid syntax: %s\n", msg);
	printHelp(stderr, progName);
}

static int readDecimalParameter(const char *str, int *value) {
	if (!str) return 1;
	
	char checkCode = 0;
	return sscanf(str, "%u%c", value, &checkCode) == 1;
}

static int readHexadecimalParameter(const char *str, int *value) {
	if (!str) return 1;
	
	char checkCode = 0;
	return sscanf(str, "0x%x%c", value, &checkCode) == 1;
}

int main(int argc, char **argv) {
	assert(argc > 0);
	
	
	// handle command line options
	uint8_t sensorAddr = MPU60X0_I2C_SLAVE_ADDR;
	uint8_t isSpecificAddr = 0;
	uint32_t delayMS = DEFAULT_DELAY_MS;
	uint32_t sampleCount = DEFAULT_SAMPLES;
	uint8_t addressMask = DEFAULT_ADDRESS_BIT;
	uint8_t verboseMode = 0;
	char *outFilename = NULL;
	
	/* read command line options */
	int i = 1;
	while(i < argc) {
		char *arg = argv[i];
		
		if (strcmp(arg, "--help") == 0) {
			if (i == 1 && argc == 2) {
				printHelp(stdout, argv[0]);
				return 0;
			} else {
				printSyntaxError("invalid option --help", argv[0]);
				return 1;
			}
		} else if (strcmp(arg, "--version") == 0) {
			if (i == 1 && argc == 2) {
				fprintf(stderr, "Version: "PROG_VERSION"\n");
				return 0;
			} else {
				printSyntaxError("invalid option --help", argv[0]);
				return 1;
			}
		} else if (strcmp(arg, "-a") == 0) {
			int tmpVal = 0;
			if (++i < argc && readDecimalParameter(argv[i], &tmpVal)) {
				addressMask = tmpVal ? 0x02 : 0x00;
			} else {
				printSyntaxError("expected addressBitValue after -a", argv[0]);
				return 1;
			}
		} else if (strcmp(arg, "--addr") == 0) {
			int tmpVal = 0;
			if (++i < argc && (readDecimalParameter(argv[i], &tmpVal) || readHexadecimalParameter(argv[i], &tmpVal)))  {
				isSpecificAddr = 1;
				sensorAddr = tmpVal;
			} else {
				printSyntaxError("expected address after --addr as unsigned decimal or hexadecimal value with 0x-prefix.", argv[0]);
				return 1;
			}
		} else if (strcmp(arg, "-d") == 0) {
			int tmpVal = 0;
			if (++i < argc && readDecimalParameter(argv[i], &tmpVal)) {
				delayMS = tmpVal;
			} else {
				printSyntaxError("expected dealy after -d", argv[0]);
				return 1;
			}
		} else if (strcmp(arg, "-s") == 0) {
			int tmpVal = 0;
			if (++i < argc && readDecimalParameter(argv[i], &tmpVal)) {
				sampleCount = tmpVal;
			} else {
				printSyntaxError("expected sample count after -s", argv[0]);
				return 1;
			}
		} else if (strcmp(arg, "-o") == 0) {
			if (++i < argc) {
				outFilename = argv[i];
			} else {
				printSyntaxError("expected filename after -o", argv[0]);
				return 1;
			}
		} else if (strcmp(arg, "-v") == 0) {
			verboseMode = 1;
		} else {
			printSyntaxError("unknown parameter", argv[0]);
			return 1;
		} 
		++i;
	}
	
	
	
	// settings information managed by parameters
	if (verboseMode) {
		logLevel = 0xFF;
	}
	if (!isSpecificAddr) {
		debug("Device address bit mask:    0x%02x\n", addressMask);
		sensorAddr = MPU60X0_I2C_SLAVE_ADDR | addressMask;
	}
	debug("Device address:             0x%02x\n", sensorAddr);
	debug("Sample count:          %9u\n", sampleCount);
	debug("Delay between samples: %9uus\n", delayMS);
	if (outFilename) {
		debug("Output filename: %15s\n", outFilename);
	}
	
	
	// execution
	FILE *output = stdout;
	if (outFilename) {
		output = fopen(outFilename, "w");
		if (!output) {
			fprintf(stderr, "Failed to open file <%s>!\n", outFilename);
			return 2;
		}
	}
	
	int res = sensorInteraction(output, sensorAddr, delayMS, sampleCount);
	
	
	if (outFilename) {
		fclose(output);
	}
	
	return res;
}
