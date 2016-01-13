################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/AutonomousController.cpp \
../src/Drivetrain.cpp \
../src/Freyja.cpp \
../src/Lifter.cpp \
../src/Robot.cpp \
../src/Subsys.cpp \
../src/TeleopController.cpp \
../src/cmds.cpp \
../src/paths.cpp 

OBJS += \
./src/AutonomousController.o \
./src/Drivetrain.o \
./src/Freyja.o \
./src/Lifter.o \
./src/Robot.o \
./src/Subsys.o \
./src/TeleopController.o \
./src/cmds.o \
./src/paths.o 

CPP_DEPS += \
./src/AutonomousController.d \
./src/Drivetrain.d \
./src/Freyja.d \
./src/Lifter.d \
./src/Robot.d \
./src/Subsys.d \
./src/TeleopController.d \
./src/cmds.d \
./src/paths.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/Users/EricLiu/Documents/workspace/freyja-mod/freyja-mod/src" -I/Users/EricLiu/wpilib/cpp/current/sim/include -I/usr/include -I/usr/include/gazebo-3.2 -I/usr/include/sdformat-2.2 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


