#ifndef SRC_LIFTER_H_
#define SRC_LIFTER_H_

#include <WPILib.h>
#include "Subsys.h"
#include <Ports.h>

/**
 * Represents Freyja's chain lifter
 */
class Lifter : public Subsys {
public:
	Lifter();
	virtual ~Lifter();

	// Inherited from Subsystem
	void init();
	void update();
	void disable();
	bool isIdle();

private:

};

#endif /* SRC_LIFTER_H_ */
