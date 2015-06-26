#ifndef SRC_ARM_H_
#define SRC_ARM_H_

#include <WPILib.h>
#include "Subsys.h"
#include <Ports.h>

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
	bool isIdle();

private:

};

#endif /* SRC_ARM_H_ */
