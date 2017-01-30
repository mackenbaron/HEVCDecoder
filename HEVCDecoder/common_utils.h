#ifndef _COMMON_UTILS_H_
#define _COMMON_UTILS_H_

#pragma once

#include "algorithm_common_consts.h"

namespace UTILS
{
	uint32_t round_up(uint32_t num, uint32_t multiple)
	{
		return (((num + multiple - 1) / multiple) * multiple);
	}

	uint32_t round_down(uint32_t num, uint32_t multiple)
	{
		return ((num / multiple) * multiple);
	}
};
#endif // _COMMON_UTILS_H_


