/******************************************************************************
 * $Id$
 *
 * Project:  libLAS - http://liblas.org - A BSD library for LAS format data.
 * Purpose:  index output implementation for C++ libLAS 
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

#include <liblas/detail/index/indexoutput.hpp>
#include <limits>

using namespace std;

namespace liblas { namespace detail {

IndexOutput::IndexOutput(liblas::Index *indexsource) :
	m_index(indexsource), 
	m_VLRCommonDataSize(5 * sizeof(boost::uint32_t)),
	m_VLRDataSizeLocation(4 * sizeof(boost::uint32_t)),
	m_FirstCellLocation(0),
	m_LastCellLocation(sizeof(boost::uint32_t) * 2)
{
} // IndexOutput::IndexOutput

bool IndexOutput::InitiateOutput(void)
{

	boost::uint8_t VersionMajor = LIBLAS_INDEX_VERSIONMAJOR, VersionMinor = LIBLAS_INDEX_VERSIONMINOR;
	char DestStr[LIBLAS_INDEX_MAXSTRLEN];
	boost::uint16_t StringLen;
	boost::uint16_t WritePos = 0;
	
	try {
		// write a header in standard VLR format
		m_indexVLRHeaderData.resize(16000);
		m_indexVLRHeaderRecord.SetUserId("liblas");
		m_indexVLRHeaderRecord.SetRecordId(42);
		m_indexVLRHeaderRecord.SetDescription("LibLAS Index Header");
		// set the header data into the header data string
		// Index file version
		WriteVLRData_n(m_indexVLRHeaderData, VersionMajor, WritePos);
		WriteVLRData_n(m_indexVLRHeaderData, VersionMinor, WritePos);
		// creator		
		strncpy(DestStr, m_index->GetIndexAuthorStr(), LIBLAS_INDEX_MAXSTRLEN - 1);
		DestStr[LIBLAS_INDEX_MAXSTRLEN - 1] = 0;
		StringLen = static_cast<boost::uint16_t>(strlen(DestStr) + 1);
		WriteVLRData_n(m_indexVLRHeaderData, StringLen, WritePos);
		WriteVLRData_str(m_indexVLRHeaderData, DestStr, StringLen, WritePos);
		// comment
		strncpy(DestStr, m_index->GetIndexCommentStr(), LIBLAS_INDEX_MAXSTRLEN - 1);
		DestStr[LIBLAS_INDEX_MAXSTRLEN - 1] = 0;
		StringLen = static_cast<boost::uint16_t>(strlen(DestStr) + 1);
		WriteVLRData_n(m_indexVLRHeaderData, StringLen, WritePos);
		WriteVLRData_str(m_indexVLRHeaderData, DestStr, StringLen, WritePos);
		// date	
		strncpy(DestStr, m_index->GetIndexDateStr(), LIBLAS_INDEX_MAXSTRLEN - 1);
		DestStr[LIBLAS_INDEX_MAXSTRLEN - 1] = 0;
		StringLen = static_cast<boost::uint16_t>(strlen(DestStr) + 1);
		WriteVLRData_n(m_indexVLRHeaderData, StringLen, WritePos);
		WriteVLRData_str(m_indexVLRHeaderData, DestStr, StringLen, WritePos);

		// file index extents
		double TempData = m_index->GetMinX();
		WriteVLRData_n(m_indexVLRHeaderData, TempData, WritePos);
		TempData = m_index->GetMaxX();
		WriteVLRData_n(m_indexVLRHeaderData, TempData, WritePos);
		TempData = m_index->GetMinY();
		WriteVLRData_n(m_indexVLRHeaderData, TempData, WritePos);
		TempData = m_index->GetMaxY();
		WriteVLRData_n(m_indexVLRHeaderData, TempData, WritePos);
		TempData = m_index->GetMinZ();
		WriteVLRData_n(m_indexVLRHeaderData, TempData, WritePos);
		TempData = m_index->GetMaxZ();
		WriteVLRData_n(m_indexVLRHeaderData, TempData, WritePos);

		// ID number of associated data VLR's - normally 43 but may use heigher numbers
		// in order to store more than one index in a file
		boost::uint32_t TempLong = m_index->GetDataVLR_ID();
		WriteVLRData_n(m_indexVLRHeaderData, TempLong, WritePos);
		
		// index cell matrix and number of points
		TempLong = m_index->GetPointRecordsCount();
		WriteVLRData_n(m_indexVLRHeaderData, TempLong, WritePos);
		TempLong = m_index->GetCellsX();
		WriteVLRData_n(m_indexVLRHeaderData, TempLong, WritePos);
		TempLong = m_index->GetCellsY();
		WriteVLRData_n(m_indexVLRHeaderData, TempLong, WritePos);
		TempLong = m_index->GetCellsZ();
		WriteVLRData_n(m_indexVLRHeaderData, TempLong, WritePos);
		
		// record length		
		m_indexVLRHeaderRecord.SetRecordLength(WritePos);
		m_indexVLRHeaderData.resize(WritePos);
		m_indexVLRHeaderRecord.SetData(m_indexVLRHeaderData);
		m_index->GetIndexHeader()->AddVLR(m_indexVLRHeaderRecord);

		// initialize VLR data members
		m_FirstCellInVLR = true;
		m_indexVLRCellRecord.SetUserId("liblas");
		m_indexVLRCellRecord.SetRecordId(43);
		m_indexVLRCellRecord.SetDescription("LibLAS Index Data");
		
		return true;
	}
	catch (std::bad_alloc) {
		return false;
	}
	catch (std::out_of_range) {
		return false;
	}

} // IndexOutput::InitiateOutput

bool IndexOutput::OutputCell(liblas::detail::IndexCell *CellBlock, boost::uint32_t x, boost::uint32_t y)
{

	try {
		if (m_FirstCellInVLR)
		{
			if (! InitializeVLRData(x, y))
				return false;
			m_indexVLRTempData.resize(numeric_limits<unsigned short>::max());
			m_TempWritePos = 0;
		} // if

		// data output for one cell may result in a partial VLR or more than one VLR depending on the number
		// of point records for the cell.
		// Partially filled VLR's are only flushed when the next cell would overflow the VLR size limit of USHRT_MAX
		// or on the last cell in the index cell block
		boost::uint32_t SubCellsXY, SubCellsZ, NumPts, PtRecords;

		NumPts = CellBlock->GetNumPoints();
		if (NumPts)
		{
			// current cell, x, y
			WriteVLRData_n(m_indexVLRTempData, x, m_TempWritePos);
			WriteVLRData_n(m_indexVLRTempData, y, m_TempWritePos);
			// min and max Z
			ElevExtrema ExtremaZ = CellBlock->GetMinZ();
			WriteVLRData_n(m_indexVLRTempData, ExtremaZ, m_TempWritePos);
			ExtremaZ = CellBlock->GetMaxZ();
			WriteVLRData_n(m_indexVLRTempData, ExtremaZ, m_TempWritePos);
			
			// number of subcells in this cell in both XY and Z
			PtRecords = CellBlock->GetNumRecords();
			WriteVLRData_n(m_indexVLRTempData, PtRecords, m_TempWritePos);
			SubCellsXY = CellBlock->GetNumSubCellRecords();
			WriteVLRData_n(m_indexVLRTempData, SubCellsXY, m_TempWritePos);
			SubCellsZ = CellBlock->GetNumZCellRecords();
			WriteVLRData_n(m_indexVLRTempData, SubCellsZ, m_TempWritePos);

			// <<<>>> prevent array overruns
			// compile data into one long vector m_indexVLRTempData
			if (SubCellsZ)
			{
				for (liblas::detail::IndexSubCellData::iterator MyCellIt = CellBlock->GetFirstZCellRecord();
					MyCellIt != CellBlock->GetEndZCell(); ++MyCellIt)
				{
					// subcell number
					boost::uint32_t ZCellID = MyCellIt->first;
					WriteVLRData_n(m_indexVLRTempData, ZCellID, m_TempWritePos);
					// number of point records in subcell
					boost::uint32_t ZCellNumPts = static_cast<boost::uint32_t>(MyCellIt->second.size());
					WriteVLRData_n(m_indexVLRTempData, ZCellNumPts, m_TempWritePos);
					for (liblas::detail::IndexCellData::iterator MyPointIt = MyCellIt->second.begin();
						MyPointIt != MyCellIt->second.end(); ++MyPointIt)
					{
						boost::uint32_t PointID = MyPointIt->first;
						assert(PointID < m_index->GetPointRecordsCount());
						WriteVLRData_n(m_indexVLRTempData, PointID, m_TempWritePos);
						boost::uint8_t ConsecutivePts = MyPointIt->second;
						WriteVLRData_n(m_indexVLRTempData, ConsecutivePts, m_TempWritePos);
					} // for
				} // for
			} // if
			if (SubCellsXY)
			{
				for (liblas::detail::IndexSubCellData::iterator MyCellIt = CellBlock->GetFirstSubCellRecord();
					MyCellIt != CellBlock->GetEndSubCell(); ++MyCellIt)
				{
					// subcell number
					boost::uint32_t SubCellID = MyCellIt->first;
					WriteVLRData_n(m_indexVLRTempData, SubCellID, m_TempWritePos);
					// number of point records in subcell
					boost::uint32_t SubCellNumPts = static_cast<boost::uint32_t>(MyCellIt->second.size());
					WriteVLRData_n(m_indexVLRTempData, SubCellNumPts, m_TempWritePos);
					for (liblas::detail::IndexCellData::iterator MyPointIt = MyCellIt->second.begin();
						MyPointIt != MyCellIt->second.end(); ++MyPointIt)
					{
						boost::uint32_t PointID = MyPointIt->first;
						assert(PointID < m_index->GetPointRecordsCount());
						WriteVLRData_n(m_indexVLRTempData, PointID, m_TempWritePos);
						boost::uint8_t ConsecutivePts = MyPointIt->second;
						WriteVLRData_n(m_indexVLRTempData, ConsecutivePts, m_TempWritePos);
					} // for
				} // for
			} // if
			if (! (SubCellsZ || SubCellsXY))
			{
				for (liblas::detail::IndexCellData::iterator MyPointIt = CellBlock->GetFirstRecord();
					MyPointIt != CellBlock->GetEnd(); ++MyPointIt)
				{
					boost::uint32_t PointID = MyPointIt->first;
					assert(PointID < m_index->GetPointRecordsCount());
					WriteVLRData_n(m_indexVLRTempData, PointID, m_TempWritePos);
					boost::uint8_t ConsecutivePts = MyPointIt->second;
					WriteVLRData_n(m_indexVLRTempData, ConsecutivePts, m_TempWritePos);
				} // for
			} // if

			// copy data to VLR
			// if new cell data causes VLR data to exceed limit add VLR to header VLR list and start new VLR
			if (m_SomeDataReadyToWrite && (m_TempWritePos + m_DataRecordSize > numeric_limits<unsigned short>::max()))
			{
				m_indexVLRCellPointData.resize(m_DataRecordSize);
				m_indexVLRCellRecord.SetRecordLength(static_cast<boost::uint16_t>(m_DataRecordSize));
				m_indexVLRCellRecord.SetData(m_indexVLRCellPointData);
				m_index->GetIndexHeader()->AddVLR(m_indexVLRCellRecord);
				// get set up for next VLR
				if (! InitializeVLRData(x, y))
					return false;
			} // if
			// if size allows, add to VLR cell record data
			if (m_TempWritePos + m_DataRecordSize <= numeric_limits<unsigned short>::max())
			{
				boost::uint32_t WritePos = m_DataRecordSize;
				// update last cell in VLR, x, y
				WriteVLRDataNoInc_n(m_indexVLRCellPointData, x, m_LastCellLocation);
				WriteVLRDataNoInc_n(m_indexVLRCellPointData, y, m_LastCellLocation + sizeof(boost::uint32_t));
				// update data record size
				m_DataRecordSize += m_TempWritePos;
				WriteVLRDataNoInc_n(m_indexVLRCellPointData, m_DataRecordSize, m_VLRDataSizeLocation);
				WriteVLRDataNoInc_str(m_indexVLRCellPointData, (char * const)&m_indexVLRTempData[0], m_TempWritePos, WritePos);
				m_SomeDataReadyToWrite = true;
				// rewind counter to start new cell data
				m_TempWritePos = 0;
			} // if
			else
			{
				// unusual situation where one cell's data exceeds the size allowed for a VLR (USHRT_MAX)
				// make as many USHRT_MAX size VLR's until all cell is represented.
				// distinguish the continuation style VLR with the first byte being the total size of the record.
				// When reading a VLR, if the size field is larger than USHRT_MAX you know you have more than one 
				// record to read and concatenate. 
				// WritePos should be at the end of the small VLR common data section
				boost::uint32_t WritePos = m_DataRecordSize;
				// m_TempWritePos is the size of the new data block
				// m_DataRecordSize is amount of common data written already
				boost::uint32_t UnwrittenBytes = m_TempWritePos + m_DataRecordSize;
				// Make DataRecordSize equal to the total size of the data + common data 
				m_DataRecordSize += m_TempWritePos;
				// write the total size in the common data section
				// resizing shouldn't be necessary
				if (m_indexVLRCellPointData.size() != numeric_limits<unsigned short>::max())
					m_indexVLRCellPointData.resize(numeric_limits<unsigned short>::max());
				WriteVLRDataNoInc_n(m_indexVLRCellPointData, m_DataRecordSize, m_VLRDataSizeLocation);
				
				// write out the part that fits in this VLR (excluding the common data)
				boost::uint32_t WrittenBytes = numeric_limits<unsigned short>::max() - WritePos;
				WriteVLRDataNoInc_str(m_indexVLRCellPointData, (char * const)&m_indexVLRTempData[0], WrittenBytes, WritePos);
				// add this VLR
				m_indexVLRCellRecord.SetRecordLength(static_cast<boost::uint16_t>(numeric_limits<unsigned short>::max()));
				m_indexVLRCellRecord.SetData(m_indexVLRCellPointData);
				m_index->GetIndexHeader()->AddVLR(m_indexVLRCellRecord);
				
				// subtract the part written
				UnwrittenBytes -= WrittenBytes;
				
				while (UnwrittenBytes > 0)
				{
					boost::uint32_t NewWrittenBytes = (UnwrittenBytes < numeric_limits<unsigned short>::max() ? UnwrittenBytes: numeric_limits<unsigned short>::max());
					WriteVLRDataNoInc_str(m_indexVLRCellPointData, (char * const)&m_indexVLRTempData[WrittenBytes], NewWrittenBytes, 0);
					WrittenBytes += NewWrittenBytes;
					UnwrittenBytes -= NewWrittenBytes;
					m_indexVLRCellRecord.SetRecordLength(static_cast<boost::uint16_t>(NewWrittenBytes));
					m_indexVLRCellRecord.SetData(m_indexVLRCellPointData);
					m_index->GetIndexHeader()->AddVLR(m_indexVLRCellRecord);
				} // while
				// wipe the slate clean for the next cell
				m_SomeDataReadyToWrite = false;
				m_TempWritePos = 0;
				m_FirstCellInVLR = true;
			} // else
		} // if
		return true;
	} // try
	catch (std::bad_alloc) {
		return false;
	}
	catch (std::out_of_range) {
		return false;
	}
	
} // IndexOutput::OutputCell

bool IndexOutput::InitializeVLRData(boost::uint32_t CurCellX, boost::uint32_t CurCellY)
{

	try {
		m_indexVLRCellPointData.resize(numeric_limits<unsigned short>::max());

		m_DataRecordSize = m_VLRCommonDataSize;
		// 1st cell in VLR, x, y
		WriteVLRDataNoInc_n(m_indexVLRCellPointData, CurCellX, m_FirstCellLocation);
		WriteVLRDataNoInc_n(m_indexVLRCellPointData, CurCellY, m_FirstCellLocation + sizeof(boost::uint32_t));
		// last cell in VLR, x, y
		WriteVLRDataNoInc_n(m_indexVLRCellPointData, CurCellX, m_LastCellLocation);
		WriteVLRDataNoInc_n(m_indexVLRCellPointData, CurCellY, m_LastCellLocation + sizeof(boost::uint32_t));
		// data record size
		WriteVLRDataNoInc_n(m_indexVLRCellPointData, m_DataRecordSize, m_VLRDataSizeLocation);
		m_FirstCellInVLR = false;
		m_SomeDataReadyToWrite = false;

		return true;
	}
	catch (std::bad_alloc) {
		return false;
	}
	catch (std::out_of_range) {
		return false;
	}
} // IndexOutput::InitializeVLRData

bool IndexOutput::FinalizeOutput(void)
{

	try {
		// copy data to VLR
		// if new cell data causes VLR data to exceed limit add VLR to header VLR list and start new VLR
		if (m_SomeDataReadyToWrite)
		{
		#ifdef LIBLAS_INDEX_PADLASTVLR
			boost::uint32_t PadBytes = m_DataRecordSize % 4;
			if (PadBytes && (m_DataRecordSize + PadBytes <= numeric_limits<unsigned short>::max()))
			{
				m_DataRecordSize += PadBytes;
				for (boost::uint32_t i = 0; i < PadBytes; ++i)
				{
					m_indexVLRCellPointData[m_DataRecordSize - i - 1] = 0;
				} // for
			} // if
		#endif // LIBLAS_INDEX_PADLASTVLR
			m_indexVLRCellPointData.resize(m_DataRecordSize);
			m_indexVLRCellRecord.SetRecordLength(static_cast<boost::uint16_t>(m_DataRecordSize));
			m_indexVLRCellRecord.SetData(m_indexVLRCellPointData);
			m_index->GetIndexHeader()->AddVLR(m_indexVLRCellRecord);
		} // if
		return true;
	}
	catch (std::bad_alloc) {
		return false;
	}
	catch (std::out_of_range) {
		return false;
	}

} // IndexOutput::FinalizeOutput

}} // namespace liblas detail
