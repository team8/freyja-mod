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

	/**
	 * Sets state to IDLE_OPEN
	 * Starts compressor
	 */
	void init();

	/**
	 * Runs state machine which idles the arm
	 * after enough time has passed
	 */
	void update();

	/**
	 * Turns off the solenoid, timer, and compressor
	 * Sets state to DISABLED preventing further state changes
	 */
	void disable();

	/**
	 * Turns off the compressor and stops timer
	 */
	void idle();

	/**
	 * Returns whether or not the arm is idle
	 */
	bool isIdle();

	/**
	 * If arm is open/opening close arm and vice versa
	 */
	void toggle();

private:
	/**
	 * An enum for the state of the arm
	 */
	enum State {
		IDLE_OPEN,
		IDLE_CLOSED,
		OPENING,
		CLOSING,
		DISABLED
	};

	/**
	 * The time for arm to idle after moving
	 */
	const double ARM_TRANSITION_TIME = 0;
	/**
	 * A solenoid that operates in both directions
	 */
	DoubleSolenoid sol;

	/**
	 * Always compressing after the subsystem is initialized until disabled
	 * Runs through the PCM
	 */
	Compressor compressor;

	/**
	 * Used to idle the arm after enough time
	 * @see const ARM_TRANSITION_TIME
	 */
	Timer timer;

	/**
	 * Current state of the arm
	 * @see enum State
	 */
	State state;
	/**
	 * Changes the state unless currently DISABLED
	 * @see enum State
	 */

	void setState(State);

	/**
	 * If not open/opening, start opening the arm for specified time
	 * @see const ARM_TRANSITION_TIME
	 */
	void open();

	/**
	 * If not closed/closing, start closing the arm for specified time
	 * @see const ARM_TRANSITION_TIME
	 */
	void close();
};

#endif /* SRC_ARM_H_ */
