/******************************************************************************
 * $Id$
 *
 * Project:  libLAS - http://liblas.org - A BSD library for LAS format data.
 * Purpose:  index cell implementation for C++ libLAS 
 * Author:   Gary Huber, gary@garyhuberart.com
 *
 ******************************************************************************
 *
 * (C) Copyright Gary Huber 2010, gary@garyhuberart.com
 *
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following 
 * conditions are met:
 * 
 *     * Redistributions of source code must retain the above copyright 
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright 
 *       notice, this list of conditions and the following disclaimer in 
 *       the documentation and/or other materials provided 
 *       with the distribution.
 *     * Neither the name of the Martin Isenburg or Iowa Department 
 *       of Natural Resources nor the names of its contributors may be 
 *       used to endorse or promote products derived from this software 
 *       without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS 
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED 
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT 
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 ****************************************************************************/

#ifndef LIBLAS_DETAIL_INDEXCELL_HPP_INCLUDED
#define LIBLAS_DETAIL_INDEXCELL_HPP_INCLUDED

// boost
#include <boost/cstdint.hpp>

#include <map>

namespace liblas { namespace detail {

typedef boost::int16_t ElevExtrema;
typedef boost::uint32_t ElevRange;
typedef boost::uint8_t	ConsecPtAccumulator;
typedef std::map<boost::uint32_t, ConsecPtAccumulator> IndexCellData;
typedef std::map<boost::uint32_t, IndexCellData> IndexSubCellData;
typedef boost::uint64_t	TempFileOffsetType;

class IndexCell
{
public:
	IndexCell();
	
private:
	TempFileOffsetType m_FileOffset;
	boost::uint32_t m_NumPoints;
	ElevExtrema m_MinZ, m_MaxZ;
	IndexCellData m_PtRecords;
	IndexSubCellData m_ZCellRecords;
	IndexSubCellData m_SubCellRecords;

public:
	void SetFileOffset(TempFileOffsetType fos);
	void SetNumPoints(boost::uint32_t nmp);
	TempFileOffsetType GetFileOffset(void) const;
	boost::uint32_t GetNumRecords(void) const;
	boost::uint32_t GetNumPoints(void) const;
	boost::uint32_t GetNumSubCellRecords(void) const;
	boost::uint32_t GetNumZCellRecords(void) const;
	ElevExtrema GetMinZ(void) const {return m_MinZ;}
	ElevExtrema GetMaxZ(void) const {return m_MaxZ;}
	bool RoomToAdd(boost::uint32_t a);
	void AddPointRecord(boost::uint32_t a);
	void AddPointRecord(boost::uint32_t a, boost::uint8_t b);
	bool IncrementPointRecord(boost::uint32_t a);
	void RemoveMainRecords(void);
	void RemoveAllRecords(void);
	void UpdateZBounds(double TestZ);
	ElevRange GetZRange(void) const;
	void AddZCell(boost::uint32_t a, boost::uint32_t b);
	bool IncrementZCell(boost::uint32_t a, boost::uint32_t b);
	void AddSubCell(boost::uint32_t a, boost::uint32_t b);
	bool IncrementSubCell(boost::uint32_t a, boost::uint32_t b);
	boost::uint8_t GetPointRecordCount(boost::uint32_t a);
	const IndexCellData::iterator GetFirstRecord(void);
	const IndexCellData::iterator GetEnd(void);
	const IndexSubCellData::iterator GetFirstSubCellRecord(void);
	const IndexSubCellData::iterator GetEndSubCell(void);
	const IndexSubCellData::iterator GetFirstZCellRecord(void);
	const IndexSubCellData::iterator GetEndZCell(void);
};

}} // namespace liblas::detail

#endif // LIBLAS_DETAIL_INDEXCELL_HPP_INCLUDED
