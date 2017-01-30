#ifndef _CSTREAM_READER_H_
#define _CSTREAM_READER_H_

#pragma once
#include <iostream>
#include <fstream>
#include <bitset>

using namespace std;

class CStream_reader
{
	ifstream m_bs_file;
	uint32_t m_bs_byte_aligned_pos;
	uint32_t m_bs_bit_aligned_pos;
	uint8_t m_pbuffer[NUM_BYTES_IN_WORD];
	uint8_t m_buffer_start_idx;
	uint8_t m_buffer_end_idx;

public:
	CStream_reader(string& bs_name);
	uint32_t get_stream_bitpos();
	uint32_t read_bits(const uint32_t num_bits_to_return);
	uint32_t next_bits(const uint32_t num_bits_to_peek);

};

/*! \brief Class Constructor
*
*
*  Constructs the CStream_reader class object
*/
CStream_reader::CStream_reader(string& bs_name)
{
	m_bs_byte_aligned_pos = 0;
	m_bs_bit_aligned_pos = 0;
	memset(m_pbuffer, 0, NUM_BYTES_IN_WORD * sizeof(uint8_t));
	m_buffer_start_idx = 0;
	m_buffer_end_idx = 0;
	
	m_bs_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	
	try
	{
		m_bs_file.open(bs_name, ios::in | ios::binary);
	}
	catch (std::ifstream::failure e)
	{
		std::cerr << "Exception opening/reading/closing file\n";
	}
	
}

/*! \brief returns the bit position in m_pbuffer
*
*
*  Detailed description starts here.
*/
uint32_t CStream_reader::get_stream_bitpos()
{
	return m_bs_bit_aligned_pos;
}

/*! \brief Reads num_bits_to_return bits from the buffer and returns
*
* --- reads the next n bits from the bitstream and advances the bitstream pointer by n bit positions.
* --- When n is equal to 0, read_bits( n ) is specified to return a value equal to 0 and to not advance the bitstream pointer.
* --- num_bits_to_return must be less than or equal to 32
* --- SPEC: 7.2  Specification of syntax functions and descriptors
*/
uint32_t CStream_reader::read_bits(const uint32_t num_bits_to_return)
{
	if (num_bits_to_return == 0)
	{
		return 0;
	}

	uint32_t num_bits_left_to_read = num_bits_to_return;
	uint32_t res = 0;

	uint32_t res_bit_idx = 0;
	if (m_buffer_start_idx != m_buffer_end_idx)
	{
		while (m_buffer_start_idx <= m_buffer_end_idx && num_bits_left_to_read > 0)
		{
			res = (res << res_bit_idx) | (m_pbuffer[m_buffer_start_idx >> RIGHT_SHIFT_TO_DIVIDE_BY_8] >> (NUM_BITES_IN_BYTE - m_buffer_start_idx % (NUM_BITES_IN_BYTE - 1)));
			
			m_bs_bit_aligned_pos++;
			m_bs_byte_aligned_pos = m_bs_bit_aligned_pos >> RIGHT_SHIFT_TO_DIVIDE_BY_8;
			res_bit_idx++;
			m_buffer_start_idx--;
			num_bits_left_to_read--;
		}

		if (num_bits_left_to_read == 0)
		{
			return res;
		}
	}

	uint32_t num_bytes_to_read = UTILS::round_up(num_bits_left_to_read, NUM_BITES_IN_BYTE);

	m_bs_file.read((char*)m_pbuffer, num_bytes_to_read);
	m_buffer_start_idx = 0;
	m_buffer_end_idx = num_bytes_to_read * NUM_BITES_IN_BYTE;

	res_bit_idx = 0;
	while (m_buffer_start_idx <= m_buffer_end_idx && num_bits_left_to_read > 0)
	{
		res = (res << res_bit_idx) | (m_pbuffer[m_buffer_start_idx >> RIGHT_SHIFT_TO_DIVIDE_BY_8] >> (NUM_BITES_IN_BYTE - m_buffer_start_idx % (NUM_BITES_IN_BYTE - 1)));
		
		m_bs_bit_aligned_pos++;
		m_bs_byte_aligned_pos = m_bs_bit_aligned_pos >> RIGHT_SHIFT_TO_DIVIDE_BY_8;
		res_bit_idx++;
		m_buffer_start_idx--;
		num_bits_left_to_read--;
	}

	assert(num_bits_left_to_read == 0);
	return res;
}

/*! \brief Brief description.
*         Brief description continued.
*
*  Detailed description starts here.
*/
uint32_t CStream_reader::next_bits(const uint32_t num_bits_to_peek)
{
	if (num_bits_to_peek)
	{
		return 0;
	}

	//Check if there are enough bits left infile
	streampos curr_pos = m_bs_file.tellg();
	m_bs_file.seekg(m_bs_file.end);
	if ((uint32_t(m_bs_file.tellg()) - curr_pos) < UTILS::round_up(num_bits_to_peek, NUM_BITES_IN_BYTE))
	{
		return 0;
	}
	m_bs_file.seekg(curr_pos);

	//Make local copies of variables
	curr_pos = m_bs_file.tellg();
	uint32_t temp_bs_bit_aligned_pos = m_bs_bit_aligned_pos;
	uint32_t temp_bs_byte_aligned_pos = m_bs_byte_aligned_pos;
	uint32_t temp_buffer_start_idx = m_buffer_start_idx;
	uint32_t temp_buffer_end_idx = m_buffer_end_idx;
	uint8_t temp_buffer[NUM_BYTES_IN_WORD];
	memcpy(temp_buffer, m_pbuffer, NUM_BYTES_IN_WORD);

	uint32_t res = read_bits(num_bits_to_peek);

	//Restore the variables
	m_bs_file.seekg(curr_pos);
	m_bs_bit_aligned_pos = temp_bs_bit_aligned_pos;
	m_bs_byte_aligned_pos = temp_bs_byte_aligned_pos;
	m_buffer_start_idx = temp_buffer_start_idx;
	m_buffer_end_idx = temp_buffer_end_idx;
	memcpy(m_pbuffer, temp_buffer, NUM_BYTES_IN_WORD);

	return res;
}

#endif //_CSTREAM_READER_H_