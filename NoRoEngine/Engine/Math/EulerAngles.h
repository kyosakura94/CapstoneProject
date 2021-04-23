
#include <stdio.h>
struct EulerAngles {

public:

	float xAxis, yAxis, zAxis; // euler angles

	inline void set(float xAxis_, float yAxis_, float zAxis_) {
		xAxis = xAxis_; yAxis = yAxis_; zAxis = zAxis_;
	}

	inline EulerAngles(float xAxis, float yAxis, float zAxis) {
		set(xAxis, yAxis, zAxis);
	}

	/// Here's a set of constructors
	inline explicit EulerAngles(float s = 0.0f) {
		set(s, s, s);
	}

	inline EulerAngles(const EulerAngles& e) {
		set(e.xAxis, e.yAxis, e.zAxis);
	}

	inline EulerAngles& operator = (const EulerAngles& e) {
		set(e.xAxis, e.yAxis, e.zAxis);
		return *this;
	}

	inline void print() {
		printf("%1.8f %1.8f %1.8f\n\n", xAxis, yAxis, zAxis);
	}
};
