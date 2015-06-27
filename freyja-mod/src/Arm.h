#ifndef SRC_ARM_H_
#define SRC_ARM_H_

#include <WPILib.h>
#include "Subsys.h"
#include <Ports.h>

#define ARM_EXTEND_TIME 0.4

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
	Compressor comp;
	DoubleSolenoid sol;
	Timer timer;

	enum State {
		IDLE_OPEN,
		IDLE_CLOSED,
		OPENING,
		CLOSING,
		STOPPED
	};
	enum CompressorState {
		ON, OFF
	};

	State state;
	CompressorState compState;

	void setState(State);
	void open();
	void close();

};

#endif /* SRC_ARM_H_ */
