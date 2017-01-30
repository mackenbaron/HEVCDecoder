#ifndef _CNAL_UNIT_H_
#define _CNAL_UNIT_H_
#pragma once

#include "cpp_common_includes.h"
#include "algorithm_common_enums.h"


class CNAL_unit_header
{
public:
	bitset<1> m_forbidden_zero_bit;
	NAL_UNIT_TYPE m_nal_unit_type;
	bitset<6> m_nuh_layer_id;
	bitset<3> m_nuh_temporal_id_plus1;
};

class CNAL_unit
{
public:
	CNAL_unit_header m_header;
	vector<uint8_t> m_rbsp_byte;
};

#endif //_CNAL_UNIT_H_
