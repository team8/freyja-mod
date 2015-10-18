#include "Subsys.h"

Subsys::Subsys() {

}

Subsys::~Subsys() {

}

void Subsys::startTrouting() {
	trouting = true;
}

bool Subsys::endTrout() {
	return trouting;
}
