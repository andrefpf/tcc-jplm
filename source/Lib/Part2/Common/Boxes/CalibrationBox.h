#ifndef CALIBRATIONBOX_H__
#define CALIBRATIONBOX_H__

#include "Lib/Common/Boxes/Box.h"

class CalibrationBox
{
public:
  constexpr static uint32_t id = 0x6c666361;
	CalibrationBox();
	~CalibrationBox();
	
};

#endif /* end of include guard: CALIBRATIONBOX_H__ */