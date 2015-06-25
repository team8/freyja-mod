#include "WPILib.h"

#include "TeleController.h"
#include "AutoController.h"

class Freyja_mod: public IterativeRobot {
private:

	Robot robot;
	TeleController telecontroller;
	AutoController autocontroller;

	void RobotInit();

	void AutonomousInit();

	void AutonomousPeriodic();

	void TeleopInit();

	void TeleopPeriodic();

	void DisabledInit();

	void DisabledPeriodic();

};

void Freyja_mod::RobotInit() {
	robot.init();
}

void Freyja_mod::AutonomousInit() {
	autocontroller.init(paths::NOTHING);
}

void Freyja_mod::AutonomousPeriodic() {
	autocontroller.update(&robot);
	robot.update();
}

void Freyja_mod::TeleopInit() {
	telecontroller.init();
}

void Freyja_mod::TeleopPeriodic() {
	telecontroller.update(&robot);
	robot.update();
}

void Freyja_mod::DisabledInit() {
	robot.disable();
}

void Freyja_mod::DisabledPeriodic() {
	robot.disable();
}

START_ROBOT_CLASS(Freyja_mod)
