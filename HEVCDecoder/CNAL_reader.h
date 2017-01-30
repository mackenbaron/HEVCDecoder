#ifndef _CNAL_READER_H_
#define _CNAL_READER_H_
#pragma once

#include "cpp_common_includes.h"
#include "Cstream_reader.h"
#include "CNAL_unit.h"

class CNAL_reader
{
	CStream_reader m_stream_reader;
public:
	CNAL_reader(string& bs_name)
		 : m_stream_reader(bs_name)
	{

	}

	CNAL_unit read_next_NAL();
	vector<CNAL_unit>& read_all_NALs();
};

CNAL_unit CNAL_reader::read_next_NAL()
{
	while (m_stream_reader.next_bits(24) != 0x000001 && 
		   m_stream_reader.next_bits(32) != 0x00000001)
	{
		uint8_t leading_zero_8bits = m_stream_reader.read_bits(NUM_BITES_IN_BYTE);
		assert(leading_zero_8bits == 0x0);
	}

	if (m_stream_reader.next_bits(24) != 0x000001)
	{
		uint8_t zero_byte = m_stream_reader.read_bits(NUM_BITES_IN_BYTE);
		assert(zero_byte == 0x0);
	}

	uint32_t start_code_prefix_one_3bytes = m_stream_reader.read_bits(NUM_BITES_IN_BYTE * 3);
	assert(start_code_prefix_one_3bytes == 0x000001);

	//Now bitstream is pointing to the beginning of a NAL Unit
	CNAL_unit nal;

	//Read NAL header, 2 bytes, SPEC: 7.3.1.2  NAL unit header syntax
	nal.m_header.m_forbidden_zero_bit		= m_stream_reader.read_bits(1);
	nal.m_header.m_nal_unit_type			= NAL_UNIT_TYPE(m_stream_reader.read_bits(6));
	nal.m_header.m_nuh_layer_id				= m_stream_reader.read_bits(6);
	nal.m_header.m_nuh_temporal_id_plus1	= m_stream_reader.read_bits(3);

	//Read NAL rbsp
	while (m_stream_reader.next_bits(24) != 0x000001 && 
		   m_stream_reader.next_bits(32) != 0x00000001 /* && not end of file*/)
	{
		if (m_stream_reader.next_bits(24) == EMULATION_PREVENTION_THREE_BYTE)
		{
			nal.m_rbsp_byte.push_back(m_stream_reader.read_bits(NUM_BITES_IN_BYTE));
			nal.m_rbsp_byte.push_back(m_stream_reader.read_bits(NUM_BITES_IN_BYTE));
			m_stream_reader.read_bits(NUM_BITES_IN_BYTE); //emulation_prevention_three_byte
		}
		else
		{
			nal.m_rbsp_byte.push_back(m_stream_reader.read_bits(NUM_BITES_IN_BYTE));
		}
	}

	return nal;
}

#endif //_CNAL_READER_H_