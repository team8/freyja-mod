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
 * The drivetrain is a subsystem of the robot that moves it around. It has 2 encoders and 2 Talons; a left and a right one.
 * The drivetrain also has two gyro PID controllers linked to one gyro. Each gyro controller controls the Talon on its side.
 * It also has 2 drive PID controllers that control the Talons on their sides.
 *
 */
class Drivetrain : public Subsys {
public:
	/**
	 * Constructor for this drivetrain.
	 */
	Drivetrain();

	/**
	 * Destructor for this drivetrain
	 */
	virtual ~Drivetrain();

	/**
	 * Initializes the drivetrain and its components
	 */
	void init();

	/**
	 * This method is called continuously to keep the drivetrain updated
	 */
	void update();

	/**
	 * Disables the drivetrain
	 */
	void disable();

	/**
	 * Idles the drivetrain
	 */
	void idle();

	/**
	 * Determines if the drivetrain is idle or not
	 * @return true if drivetrain is idling.
	 */
	bool isIdle();

	/**
	 * Drives the robot according to the given values
	 * @param turnValue the X value of the joystick
	 * @param forwardValue the Y value of the joystick
	 */
	void drive(double turnValue, double forwardValue);

	/**
	 * Drive a set distance
	 * @param distance how far you want to drive
	 */
	void driveDist(double distance);

	/**
	 * Rotates the drivetrain by an angle
	 * @param angle how much you want to turn
	 */
	void rotateAngle(double angle);

	/**
	 * Gradually halts the drivetrain. Acts just like a normal brake.
	 */
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

	/**
	 *
	 */
	void setState(State state);

	/**
	 * Returns true if the encoders are stopped
	 * @return if both encoders are stopped
	 */
	bool encodersStopped();

	/**
	 * Gets the maximum error of the drive PIDControllers
	 * @return the error of the drive PIDControllers
	 */
	int driveControllerError();

	/**
	 * Gets the maximum error of the gyro PIDControllers
	 * @return the error of the gyro PIDControllers
	 */
	int rotateControllerError();

	/**
	 * Enables the gyro PIDControllers that rotate the robot
	 */
	void enableGyroControllers();

	/**
	 * Enables the drive PIDControlers that drive and brake the robot
	 */
	void enableDriveControllers();

	/**
	 * Disables all the PIDControllers
	 */
	void disableControllers();

	/**
	 * Disables the gyro PIDControllers that rotate the robot
	 */
	void disableGyroControllers();

	/**
	 * Disables the drive PIDControllers that drive and brake the robot
	 */
	void disableDriveControllers();
};

#endif /* SRC_DRIVETRAIN_H_ */
