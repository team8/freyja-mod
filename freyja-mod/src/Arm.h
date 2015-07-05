#ifndef SRC_ARM_H_
#define SRC_ARM_H_

#include <WPILib.h>
#include "Subsys.h"
#include "Ports.h"

/**
 * Represents Freyja's pneumatically controlled arms
 */
class Arm : public Subsys {
public:
	Arm();
	virtual ~Arm();

	// Inherited from Subsystem
	void init();
	void update();
	void disable();
	void idle();
	bool isIdle();
	void toggle();

private:
	Compressor compressor;
	DoubleSolenoid solenoid;
	Timer timer;
	const double ARM_EXTEND_TIME = 0.4;


	enum State {
		IDLE_OPEN,
		IDLE_CLOSED,
		OPENING,
		CLOSING,
		DISABLED
	};
	enum CompressorState {
		ON, OFF
	};

	State state;
	CompressorState compressorState;

	void setState(State);
	void open();
	void close();

};

#endif /* SRC_ARM_H_ */
