#include "home_position/home_position.h"
#include <cmath>
#include <iostream>

namespace HomePosition {
	double distanceFromStart(const Position3D& current) {
		double dx = current.x - START_POSITION.x;
		double dy = current.y - START_POSITION.y;
		double dz = current.z - START_POSITION.z;
		return std::sqrt(dx * dx + dy * dy + dz * dz);
	}

	void positionToStart(Position3D& pos) {
		pos = START_POSITION;
	}

	void resetButtonToStart(Position3D& pos) {
		pos = START_POSITION;
	}

	void printPosition(const Position3D& pos) {
		std::cout << "Position: (" << pos.x << ", " << pos.y << ", " << pos.z << ")" << std::endl;
	}
}