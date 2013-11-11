# Der Projektname
project(MPU60X0)

# Minimal benötigte CMake-Version wenn z.B. bestimmte CMake-Kommandos benutzt werden
cmake_minimum_required(VERSION 2.6)

include_directories(
    ${CMAKE_SOURCE_DIR}/src/public
)

if (CMAKE_COMPILER_IS_GNUCC)
	add_definitions(-Wall -Wextra)
endif()

set(driver_SRC
	src/mpu60X0_i2c.c
)

set(logger_SRC
	${driver_SRC}
	src/mpu60X0_mockwriter.cpp
	src/mpu60X0_logger.cpp
)


add_library(mpu60X0_i2c ${driver_SRC})
add_executable(mpu60X0_i2c_logger ${logger_SRC})
