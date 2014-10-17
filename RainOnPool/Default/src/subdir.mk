################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/G308_Geometry.cpp \
../src/G308_ImageLoader.cpp \
../src/ParticleEngine.cpp \
../src/TextureDemo.cpp \
../src/main.cpp \
../src/sphere.cpp 

OBJS += \
./src/G308_Geometry.o \
./src/G308_ImageLoader.o \
./src/ParticleEngine.o \
./src/TextureDemo.o \
./src/main.o \
./src/sphere.o 

CPP_DEPS += \
./src/G308_Geometry.d \
./src/G308_ImageLoader.d \
./src/ParticleEngine.d \
./src/TextureDemo.d \
./src/main.d \
./src/sphere.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"C:\Users\Yen-hua Chuang\308\RainOnPool\include" -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


