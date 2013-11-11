# Der Projektname
project(MPU60X0)

# Minimal benötigte CMake-Version wenn z.B. bestimmte CMake-Kommandos benutzt werden
cmake_minimum_required(VERSION 2.6)

include_directories(
    ${CMAKE_SOURCE_DIR}/src/public
)

set(driver_SRC
	src/mpu60X0_i2c.c
)


add_library(mpu60X0_i2c ${driver_SRC})
