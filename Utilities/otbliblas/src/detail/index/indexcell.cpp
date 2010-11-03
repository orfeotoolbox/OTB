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

#include <liblas/detail/index/indexcell.hpp>
#include <cmath>
#include <limits>

using namespace std;

namespace liblas { namespace detail {

IndexCell::IndexCell() :
	m_FileOffset(0), 
	m_NumPoints(0), 
	m_MinZ(numeric_limits<ElevExtrema>::max()), 
	m_MaxZ(numeric_limits<ElevExtrema>::min())
{
} // IndexCell::IndexCell


void IndexCell::SetFileOffset(boost::uint32_t fos)
{
	m_FileOffset = fos;
} // IndexCell::SetFileOffset

void IndexCell::SetNumPoints(boost::uint32_t nmp)
{
	m_NumPoints = nmp;
} // IndexCell::SetNumPoints

boost::uint32_t IndexCell::GetFileOffset(void) const
{
	return(m_FileOffset);
} // IndexCell::GetFileOffset

boost::uint32_t IndexCell::GetNumRecords(void) const
{
	return((boost::uint32_t)m_PtRecords.size());
} // IndexCell::GetNumRecords

boost::uint32_t IndexCell::GetNumPoints(void) const
{
	return(m_NumPoints);
} // IndexCell::GetNumPoints

boost::uint32_t IndexCell::GetNumSubCellRecords(void) const
{
	return((boost::uint32_t)m_SubCellRecords.size());
} // IndexCell::GetNumSubCellRecords

boost::uint32_t IndexCell::GetNumZCellRecords(void) const
{
	return((boost::uint32_t)m_ZCellRecords.size());
} // IndexCell::GetNumZCellRecords

bool IndexCell::RoomToAdd(boost::uint32_t a)
{
	return (m_PtRecords[a] < numeric_limits<ConsecPtAccumulator>::max());
} // IndexCell::RoomToAdd

void IndexCell::AddPointRecord(boost::uint32_t a)
{
	m_PtRecords[a] = 1;
	++m_NumPoints;
} // IndexCell::AddPointRecord

void IndexCell::AddPointRecord(boost::uint32_t a, boost::uint8_t b)
{
	m_PtRecords[a] = b;
	m_NumPoints += b;
} // IndexCell::AddPointRecord

bool IndexCell::IncrementPointRecord(boost::uint32_t a)
{
	IndexCellData::iterator MyIT;

	if ((MyIT = m_PtRecords.find(a)) != m_PtRecords.end())
	{
		if (MyIT->second < numeric_limits<ConsecPtAccumulator>::max())
		{
			++MyIT->second;
			++m_NumPoints;
			return true;
		} // if
	} // if
	return false;
} // IndexCell::IncrementPointRecord

void IndexCell::RemoveMainRecords(void)
{
	m_PtRecords.clear();
} // IndexCell::RemoveRecords

void IndexCell::RemoveAllRecords(void)
{
	IndexSubCellData::iterator MyIT;
	m_PtRecords.clear();
	for (MyIT = m_ZCellRecords.begin(); MyIT != m_ZCellRecords.end(); ++MyIT)
	{
		MyIT->second.clear();
	} // if
	m_ZCellRecords.clear();
	for (MyIT = m_SubCellRecords.begin(); MyIT != m_SubCellRecords.end(); ++MyIT)
	{
		MyIT->second.clear();
	} // if
	m_SubCellRecords.clear();
} // IndexCell::RemoveRecords

void IndexCell::UpdateZBounds(double TestZ)
{
	if (TestZ > numeric_limits<ElevExtrema>::max())
		m_MaxZ = numeric_limits<ElevExtrema>::max();
	else if (TestZ < numeric_limits<ElevExtrema>::min())
		m_MinZ = numeric_limits<ElevExtrema>::min();
	else
	{
		if (TestZ > m_MaxZ)
			m_MaxZ = static_cast<ElevExtrema>(ceil(TestZ));
		if (TestZ < m_MinZ)
			m_MinZ = static_cast<ElevExtrema>(floor(TestZ));
	} // else
} // IndexCell::UpdateZBounds

ElevRange IndexCell::GetZRange(void) const
{
return (m_MaxZ > m_MinZ ? static_cast<ElevRange>(m_MaxZ - m_MinZ): 0);
} // IndexCell::GetZRange

void IndexCell::AddZCell(boost::uint32_t a, boost::uint32_t b)
{
	IndexSubCellData::iterator MyIT;

	if ((MyIT = m_ZCellRecords.find(a)) != m_ZCellRecords.end())
	{
		MyIT->second[b] = 1;
	} // if
	else
	{
		IndexCellData p;
		p[b] = 1;
		m_ZCellRecords[a] = p;
	} // else
} // IndexCell::AddZCell

bool IndexCell::IncrementZCell(boost::uint32_t a, boost::uint32_t b)
{
	IndexSubCellData::iterator MyIT;
	IndexCellData::iterator YourIT;

	if ((MyIT = m_ZCellRecords.find(a)) != m_ZCellRecords.end())
	{
		if ((YourIT = MyIT->second.find(b)) != MyIT->second.end())
		{
			if (YourIT->second < numeric_limits<ConsecPtAccumulator>::max())
			{
				++YourIT->second;
				return true;
			} // if
		} // if
	} // if
	return false;
} // IndexCell::IncrementZCell

void IndexCell::AddSubCell(boost::uint32_t a, boost::uint32_t b)
{
	IndexSubCellData::iterator MyIT;

	if ((MyIT = m_SubCellRecords.find(a)) != m_SubCellRecords.end())
	{
		MyIT->second[b] = 1;
		//MyIT->second.insert(b, 1);
	} // if
	else
	{
		IndexCellData p;
		p[b] = 1;
		m_SubCellRecords[a] = p;
	} // else
} // IndexCell::AddSubCell

bool IndexCell::IncrementSubCell(boost::uint32_t a, boost::uint32_t b)
{
	IndexSubCellData::iterator MyIT;
	IndexCellData::iterator YourIT;

	if ((MyIT = m_SubCellRecords.find(a)) != m_SubCellRecords.end())
	{
		if ((YourIT = MyIT->second.find(b)) != MyIT->second.end())
		{
			if (YourIT->second < numeric_limits<ConsecPtAccumulator>::max())
			{
				++YourIT->second;
				return true;
			} // if
		} // if
	} // if
	return false;
} // IndexCell::IncrementSubCell

boost::uint8_t IndexCell::GetPointRecordCount(boost::uint32_t a)
{
	return m_PtRecords[a];
} // IndexCell::GetPointRecordCount

const IndexCellData::iterator IndexCell::GetFirstRecord(void)
{
	return (m_PtRecords.begin());
} // IndexCell::GetFirstRecord

const IndexCellData::iterator IndexCell::GetEnd(void)
{
	return (m_PtRecords.end());
} // IndexCell::GetEnd

const IndexSubCellData::iterator IndexCell::GetFirstSubCellRecord(void)
{
	return (m_SubCellRecords.begin());
} // IndexCell::GetFirstRecord

const IndexSubCellData::iterator IndexCell::GetEndSubCell(void)
{
	return (m_SubCellRecords.end());
} // IndexCell::GetEnd

const IndexSubCellData::iterator IndexCell::GetFirstZCellRecord(void)
{
	return (m_ZCellRecords.begin());
} // IndexCell::GetFirstRecord

const IndexSubCellData::iterator IndexCell::GetEndZCell(void)
{
	return (m_ZCellRecords.end());
} // IndexCell::GetEnd


}} // namespace liblas detail

