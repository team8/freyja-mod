#include <AutonomousController.h>
#include <TeleopController.h>
#include "WPILib.h"


class Freyja : public IterativeRobot {
public:

	Freyja();

	void RobotInit();

	void AutonomousInit();

	void AutonomousPeriodic();

	void TeleopInit();

	void TeleopPeriodic();

	void DisabledInit();

	void DisabledPeriodic();

private:

	Robot robot;
	TeleopController telecontroller;
	AutonomousController autocontroller;
};

 Freyja::Freyja() :
	telecontroller(&robot),
 	autocontroller(&robot)
{

}

void Freyja::RobotInit() {
	robot.init();
}

void Freyja::AutonomousInit() {
	autocontroller.init(paths::LANDFILL_AUTO);
}

void Freyja::AutonomousPeriodic() {
	autocontroller.update();
	robot.update();
}

void Freyja::TeleopInit() {
	std::cout << "TeleopInit in Freyja.cpp called" << std::endl;
	telecontroller.init();
	std::cout << "RobotInit in Freyja.cpp finished" << std::endl;
}

void Freyja::TeleopPeriodic() {
	telecontroller.update();
	robot.update();
}

void Freyja::DisabledInit() {
	robot.disable();
}

void Freyja::DisabledPeriodic() {
	robot.disable();
}

START_ROBOT_CLASS(Freyja)
