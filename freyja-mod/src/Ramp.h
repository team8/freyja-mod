#ifndef SRC_RAMP_H_
#define SRC_RAMP_H_

#include <WPILib.h>
#include "Subsys.h"
#include <Ports.h>

/**
 * Represents Freyja's ramp
 */
class Ramp : public Subsys {
public:
	Ramp();
	virtual ~Ramp();

	// Inherited from Subsystem
	void init();
	void update();
	void disable();
	void idle();
	bool isIdle();

private:

};

#endif /* SRC_RAMP_H_ */
