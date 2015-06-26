#include <AutonomousController.h>
#include <TeleopController.h>
#include "WPILib.h"


class Freyja : public IterativeRobot {
private:

	Robot robot;
	TeleopController telecontroller;
	AutonomousController autocontroller;

	void RobotInit();

	void AutonomousInit();

	void AutonomousPeriodic();

	void TeleopInit();

	void TeleopPeriodic();

	void DisabledInit();

	void DisabledPeriodic();

};

void Freyja::RobotInit() {
	robot.init();
}

void Freyja::AutonomousInit() {
	autocontroller.init(paths::NOTHING);
}

void Freyja::AutonomousPeriodic() {
	autocontroller.update(&robot);
	robot.update();
}

void Freyja::TeleopInit() {
	telecontroller.init();
}

void Freyja::TeleopPeriodic() {
	telecontroller.update(&robot);
	robot.update();
}

void Freyja::DisabledInit() {
	robot.disable();
}

void Freyja::DisabledPeriodic() {
	robot.disable();
}

START_ROBOT_CLASS(Freyja)
