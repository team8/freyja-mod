#ifndef SRC_DRIVETRAIN_H_
#define SRC_DRIVETRAIN_H_

#include <WPILib.h>
#include "Subsys.h"
#include <Ports.h>

// Robot will be able to move faster than MAX_SPEED while turning
#define MAX_SPEED 0.0
#define MAX_TURN_SPEED 0.0

#define LEFT_DRIVE_PROPORTIONAL 0.0
#define LEFT_DRIVE_INTEGRAL 0.0
#define LEFT_DRIVE_DERIVATIVE 0.0

#define RIGHT_DRIVE_PROPORTIONAL 0.0
#define RIGHT_DRIVE_INTEGRAL 0.0
#define RIGHT_DRIVE_DERIVATIVE 0.0

#define GYRO_PROPORTIONAL 0.0
#define GYRO_INTEGRAL 0.0
#define GYRO_DERIVATIVE 0.0

#define RIGHT_DPP 0.0
#define LEFT_DPP 0.0
#define ENCODER_MAX_PERIOD 0
#define ENCODER_INPUT_RANGE 0
#define ENCODER_DRIVE_OUTPUT_RANGE 0
#define ENCODER_GYRO_OUTPUT_RANGE 0

/**
 * Represents Freyja's West-Coast Drivetrain
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
	void drive(double x, double y);
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

	void setState(State state);\
	bool encodersStopped();
	int driveControllerError();
	int rotateControllerError();
};

#endif /* SRC_DRIVETRAIN_H_ */
