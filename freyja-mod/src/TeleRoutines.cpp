#include "WPILib.h"

#include "TeleCtrlr.h"
#include "AutoCtrlr.h"

class Freyja_mod: public IterativeRobot {
private:

	Robot robot;
	TeleCtrlr telectrlr;
	AutoCtrlr autoctrlr;

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
	autoctrlr.init(paths::NOTHING);
}

void Freyja_mod::AutonomousPeriodic() {
	autoctrlr.update(&robot);
	robot.update();
}

void Freyja_mod::TeleopInit() {
	telectrlr.init();
}

void Freyja_mod::TeleopPeriodic() {
	telectrlr.update(&robot);
	robot.update();
}

void Freyja_mod::DisabledInit() {
	robot.disable();
}

void Freyja_mod::DisabledPeriodic() {
	robot.disable();
}

START_ROBOT_CLASS(Freyja_mod)
