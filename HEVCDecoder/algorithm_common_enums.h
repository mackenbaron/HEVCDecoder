#ifndef _ALGORITHM_COMMON_ENUMS_H_
#define _ALGORITHM_COMMON_ENUMS_H_
#pragma once

//SPEC: Table 7 - 1 – NAL unit type codes and NAL unit type classes
enum NAL_UNIT_TYPE
{
	TRAIL_N = 0,
	TRAIL_R = 1,

	TSA_N = 2,
	TSA_R = 3,

	STSA_N = 4,
	STSA_R = 5,

	UNSPEC63 = 63

};

#endif // _ALGORITHM_COMMON_ENUMS_H_
