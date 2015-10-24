#include "cmds.h"
#include "Robot.h"

void cmds::STOP(Robot *const robot) {
	robot->disable();
}

void cmds::DRIVE_DIST(Robot *const robot) {
	robot->drivetrain.driveDist(50);
}

void cmds::DRIVE_SHORT_DIST(Robot *const robot) {
	robot->drivetrain.driveDist(15);
}

void cmds::DRIVE_LANDFILL(Robot *const robot) {
	robot->drivetrain.driveDist(66);
}

void cmds::DRIVE_AUTO_ZONE(Robot *const robot) {
	robot->drivetrain.driveDist(86);
}

void cmds::RAMP_DEPLOY(Robot *const robot) {
	robot->ramp.start();
	if(robot->ramp.hasDeployed()) {
		robot->ramp.stop();
	}
}

void cmds::LIFT_UP(Robot *const robot) {
//
//	while(true) {
//		if(robot->isIdle()) {
			robot->lifter.liftLevel(2.5);
//			if(robot->lifter.isIdle()) {
//				break;
//			}
//		}
//	}

}


void cmds::TURN_LEFT(Robot *const robot) {
	robot->drivetrain.rotateAngle(-90);
}

void cmds::TURN_RIGHT(Robot *const robot) {
	robot->drivetrain.rotateAngle(90);
}
