#ifndef SRC_DRIVETRAIN_H_
#define SRC_DRIVETRAIN_H_

#include <WPILib.h>
#include "Subsys.h"
#include <Ports.h>

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
	void drive();
	void driveDist();
	void rotateAngle();
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

};

#endif /* SRC_DRIVETRAIN_H_ */
