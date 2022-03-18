#ifndef UTILS_H
#define UTILS_H

float fclamp(float min, float value, float max) {
	return (value < min) ? min : ((value > max) ? max : (value));
}

#endif //UTILS_H
