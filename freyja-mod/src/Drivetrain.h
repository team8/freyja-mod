// TODO: Add functionality for max v, a and j
// TODO: Add debug method
// TODO: Implement field map system similar to 254


#ifndef SRC_DRIVETRAIN_H_
#define SRC_DRIVETRAIN_H_

#include <WPILib.h>
#include "Subsys.h"
#include <Ports.h>

// MAX_FORWARD_SPEED + MAX_TURN_SPEED should not exceed 1.0
//Constants for regulating speed
#define MAX_FORWARD_SPEED 0.0
#define MAX_TURN_SPEED 0.0
#define SPEED_CONSTANT 0.0
#define TURN_CONSTANT 0.0

//Left PID constants
#define LEFT_DRIVE_PROPORTIONAL 0.0
#define LEFT_DRIVE_INTEGRAL 0.0
#define LEFT_DRIVE_DERIVATIVE 0.0


//Right PID constants
#define RIGHT_DRIVE_PROPORTIONAL 0.0
#define RIGHT_DRIVE_INTEGRAL 0.0
#define RIGHT_DRIVE_DERIVATIVE 0.0

 //Gyro PID constants
#define GYRO_PROPORTIONAL 0.0
#define GYRO_INTEGRAL 0.0
#define GYRO_DERIVATIVE 0.0

//Encoder constants
#define RIGHT_DPP 0.0
#define LEFT_DPP 0.0
#define ENCODER_MAX_PERIOD 0
#define ENCODER_INPUT_RANGE 0
#define ENCODER_DRIVE_OUTPUT_RANGE 0
#define ENCODER_GYRO_OUTPUT_RANGE 0

//Acceptable error constants
#define ACCEPTABLE_DRIVE_ERROR 1
#define ACCEPTABLE_ROTATE_ERROR 1
#define ACCEPTABLE_BRAKE_ERROR 0.01

/**
 * The drivetrain is a subsystem of the robot that moves it around. It has 2 encoders; a left one and a right one.
 * The drivetrain also has a gyro and two PID controllers
 *
 */
class Drivetrain : public Subsys {
public:
	Drivetrain();
	virtual ~Drivetrain();

	// Inherited from Subsystem
	void init();
	void update();
	void disable();
	void idle();
	bool isIdle();

	//Not inherited from Subsystem
	void drive(double turnValue, double forwardValue);
	void driveDist(double distance);
	void rotateAngle(double angle);
	void brake();

private:
	PIDController leftDriveController;
	PIDController rightDriveController;

	PIDController leftGyroController;
	PIDController rightGyroController;

	Talon leftTalon;
	Talon rightTalon;

	Gyro gyro;

	Encoder leftDriveEncoder;
	Encoder rightDriveEncoder;

	enum State{
		TELEOP,
		AUTOMATED_DRIVE,
		AUTOMATED_ROTATE,
		IDLE,
		BRAKING,
		STOPPED
	};
	State state;

	void setState(State state);
	bool encodersStopped();
	int driveControllerError();
	int rotateControllerError();
	void enableGyroControllers();
	void enableDriveControllers();
	void disableControllers();
	void disableGyroControllers();
	void disableDriveControllers();

};

#endif /* SRC_DRIVETRAIN_H_ */
