// TODO: Add debug method
// TODO: Implement field map system similar to 254


#ifndef SRC_DRIVETRAIN_H_
#define SRC_DRIVETRAIN_H_

#include <WPILib.h>
#include "Subsys.h"
#include <Ports.h>

//PID constants
#define DRIVE_PROPORTIONAL 0.1
#define DRIVE_INTEGRAL 0.0
#define DRIVE_DERIVATIVE 0.0

 //Gyro PID constants
#define GYRO_PROPORTIONAL 0.1
#define GYRO_INTEGRAL 0.0
#define GYRO_DERIVATIVE 0.0

//Encoder constants
#define RIGHT_DPP 0.1545595
#define LEFT_DPP 0.1577287
#define ENCODER_INPUT_RANGE 999
#define ENCODER_DRIVE_OUTPUT_RANGE 0.5
#define ENCODER_GYRO_OUTPUT_RANGE 0.5

// The maximum change in the power over 1 cycle provided to the robot that will not tip
#define MAXIMUM_POWER_CHANGE 0

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
	 * This method is called continuously to keep the drivetrain updated and used to passively
	 * set the State when an automated state is done
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
	// MAX_FORWARD_SPEED + MAX_TURN_SPEED should not exceed 1.0
	//Constants for regulating speed
	const double MAX_FORWARD_SPEED = 0.25;
	const double MAX_TURN_SPEED = 0.15;
	const double SPEED_SCALING = 1.0;
	const double TURN_SCALING = 1.0;

	//Acceptable error constants
	const double ACCEPTABLE_DRIVE_ERROR = 1;
	const double ACCEPTABLE_ROTATE_ERROR = 1;
	const double ACCEPTABLE_BRAKE_ERROR = 0.01;

	// Max period for which
	const int ENCODER_MAX_PERIOD = 100;

	//Constants for sampling velocities
	const int CYCLES_PER_SAMPLE = 5;
	const int SAMPLES_PER_CALC = 5;

	/**
	 * The talon that controls the left wheels
	 */
	TalonSRX leftTalon1;

	/**
	 * The other talon that controls the left wheels
	 */
	TalonSRX leftTalon2;

	/**
	 * The talon that controls the right wheels
	 */
	TalonSRX rightTalon1;

	/**
	 * The other talon that controls the right wheels
	 */
	TalonSRX rightTalon2;

	/**
	 * The gyro of the robot that returns the current angle of the robot
	 */
	Gyro gyro;

	/**
	 * The encoder on the center left wheel that keeps track of rotation
	 */
	Encoder leftEncoder;

	/**
	 * The encoder on the center right wheel that keeps track of rotation
	 */
	Encoder rightEncoder;

	/**
	 * A controller for the left wheels when driving a set distance
	 */
	PIDController leftDriveController1;
	PIDController leftDriveController2;

	/**
	 * A controller for the right wheels when driving a set distance
	 */
	PIDController rightDriveController1;
	PIDController rightDriveController2;

	/**
	 * A controller for the left wheels when rotating angle
	 */
	PIDController leftGyroController1;
	PIDController leftGyroController2;

	/**
	 * A controller for the right wheels when rotating angle
	 */
	PIDController rightGyroController1;
	PIDController rightGyroController2;

	/**
	 * A boolean to check if the encoders are returning values
	 * If they are then PID, braking and SmartDrive will run, otherwise just simple drive
	 */

	/**
	 * Keeps track of the State of the Drivetrain passively
	 * for use in the update method
	 *
	 * TELEOP - Used for standard human controlled operation
	 * AUTOMATED_DRIVE - Used when driving a set distance
	 * AUTOMATED_ROTATE - Used when rotating a set angle
	 * IDLE - Used when no motion is desired and no joystick controls have been made
	 * BRAKING - Used to automatically stop the robot
	 * STOPPED - Used for testing, no code running
	 */
	enum State{
		IDLE,
		TELEOP,
		AUTOMATED_DRIVE,
		AUTOMATED_ROTATE,
		BRAKING,
		STOPPED
	};
	State state;

	/**
	 * Previous power values for smart drive
	 */
	int prevLeftPower = 0;
	int prevRightPower = 0;

	/**
	 * Tick counts for sampling velocities and adding them
	 */
	int sampleTick = 0;
	int sumTick = 0;

	/**
	 * Sums of velocity readings of left encoder
	 */
	double nextLeftVelSum = 0;
	double leftVelSum;
	double prevLeftVelSum;

	/**
	 * Sums of velocity readings of right encoder
	 */
	double nextRightVelSum = 0;
	double rightVelSum;
	double prevRightVelSum;

	/**
	 * Sets the State of Drivetrain as long as it is not in STOPPED
	 */
	void setState(State state);

	/**
	 * Limits the drive power to a range based on MAXIMUM_POWER_CHANGE
	 */
	int limitPower(int power, int prevPower);

	/**
	 * Gets the left acceleration
	 */
	double getLeftAcceleration();

	/**
	 * Gets the right acceleration
	 */
	double getRightAcceleration();

	/**
	 * Samples a velocity from both encoders
	 */
	void sampleVelocities();

	/**
	 * Checks to see if the left acceleration is below the maximum acceleration limit
	 */
	bool isLeftAccelBelowLim();

	/**
	 * Checks to see if the right acceleration is below the maximum acceleration limit
	 */
	bool isRightAccelBelowLim();

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

	void simpleDrive(double leftPower, double rightPower);

	void smartDrive(int leftPower, int rightPower);

	/**
	 * Prints general debugging information
	 */
	void debug();


	/**
	 * Returns true if all encoders are returning values
	 * If this is not true then SmartDrive, PID and braking will be disabled
	 */
	bool encodersOnline();


};

#endif /* SRC_DRIVETRAIN_H_ */
