/*
* @Author: iseidel
* @Date:   2019-05-07 14:34:47
* @Last Modified by:   iseidel
* @Last Modified time: 2019-05-07 15:59:23
*/

#include "ColorSpaces.h"

double ColorSpaces::clip01d(double value) {
	if (value > 1.0) {
		return 1.0;
	}
	if (value < 0.0) {
		return 0.0;
	}
	return value;
}