#include <Drivetrain.h>

Drivetrain::Drivetrain() :
leftTalon((uint32_t) 1),
rightTalon((uint32_t) 1),
gyro((uint32_t) 1),

leftDriveEncoder((uint32_t) 1, (uint32_t) 2),
rightDriveEncoder((uint32_t) 1, (uint32_t) 2),

leftGyroController(1, 1, 1, &gyro, &leftTalon),
rightGyroController(1, 1, 1, &gyro, &rightTalon),
leftDriveController(1, 1, 1, &leftDriveEncoder, &leftTalon),
rightDriveController(1, 1, 1, &rightDriveEncoder, &rightTalon),
state(IDLE)

{


}

void Drivetrain::init() {

}

void Drivetrain::update(){

}

void Drivetrain::disable(){

}

void Drivetrain::idle() {

}

bool Drivetrain::isIdle() {

}

void Drivetrain::drive() {
	leftDriveEncoder.SetPIDSourceParameter(PIDSource::kRate);
	rightDriveEncoder.SetPIDSourceParameter(PIDSource::kRate);
}


Drivetrain::~Drivetrain() {

}

