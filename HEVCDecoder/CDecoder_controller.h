#ifndef _CDECODER_CONTROLLER_H_
#define _CDECODER_CONTROLLER_H_

#pragma once

#include "cpp_common_includes.h"
#include "CNAL_reader.h"


class CDecoder_controller
{
	CNAL_reader m_NAL_reader;
public:
	CDecoder_controller(string& bs_name)
		:m_NAL_reader(bs_name)
	{

	}

};
#endif //_CDECODER_CONTROLLER_H_
