#pragma once

namespace HomePosition {
	struct Position3D {
		double x;
		double y;
		double z;
	};

	inline const Position3D START_POSITION{ 0.0, 0.0, 0.0 };

	double distanceFromStart(const Position3D& current);
	void positionToStart(Position3D& pos);
	void resetButtonToStart(Position3D& pos);
	void printPosition(const Position3D& pos);
}