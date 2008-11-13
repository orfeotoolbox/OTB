/******************************************************************************
 * $Id: lasheader.cpp 875 2008-09-20 20:28:41Z hobu $
 *
 * Project:  libLAS - http://liblas.org - A BSD library for LAS format data.
 * Purpose:  LAS header class 
 * Author:   Mateusz Loskot, mateusz@loskot.net
 *
 ******************************************************************************
 * Copyright (c) 2008, Mateusz Loskot
 * Copyright (c) 2008, Phil Vachon
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

#include <liblas/lasheader.hpp>
#include <liblas/cstdint.hpp>
#include <liblas/guid.hpp>
#include <liblas/detail/utility.hpp>
// GeoTIFF
#ifdef HAVE_LIBGEOTIFF
#include <geotiff.h>
#include <geo_simpletags.h>
#include "geo_normalize.h"
#include "geo_simpletags.h"
#include "geovalues.h"
#include "geotiffio.h"
#endif // HAVE_LIBGEOTIFF
//std
#include <algorithm>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <cstring> // std::memset, std::memcpy, std::strncpy
#include <cassert>
#include <ctime>


namespace liblas
{

char const* const LASHeader::FileSignature = "LASF";
char const* const LASHeader::SystemIdentifier = "libLAS";
char const* const LASHeader::SoftwareIdentifier = "libLAS 1.0";

LASHeader::LASHeader()
{
    Init();
}

LASHeader::LASHeader(LASHeader const& other) :
    m_sourceId(other.m_sourceId),
    m_reserved(other.m_reserved),
    m_projectId1(other.m_projectId1),
    m_projectId2(other.m_projectId2),
    m_projectId3(other.m_projectId3),
    m_versionMajor(other.m_versionMajor),
    m_versionMinor(other.m_versionMinor),
    m_createDOY(other.m_createDOY),
    m_createYear(other.m_createYear),
    m_headerSize(other.m_headerSize),
    m_dataOffset(other.m_dataOffset),
    m_recordsCount(other.m_recordsCount),
    m_dataFormatId(other.m_dataFormatId),
    m_dataRecordLen(other.m_dataRecordLen),
    m_pointRecordsCount(other.m_pointRecordsCount),
    m_scales(other.m_scales),
    m_offsets(other.m_offsets),
    m_extents(other.m_extents),
    m_proj4(other.m_proj4)
{
    void* p = 0;

    p = std::memcpy(m_signature, other.m_signature, eFileSignatureSize);
    assert(p == m_signature);
    p = std::memcpy(m_projectId4, other.m_projectId4, eProjectId4Size); 
    assert(p == m_projectId4);
    p = std::memcpy(m_systemId, other.m_systemId, eSystemIdSize);
    assert(p == m_systemId);
    p = std::memcpy(m_softwareId, other.m_softwareId, eSoftwareIdSize);
    assert(p == m_softwareId);
    std::vector<uint32_t>(other.m_pointRecordsByReturn).swap(m_pointRecordsByReturn);
    assert(ePointsByReturnSize >= m_pointRecordsByReturn.size());
    
    std::vector<LASVLR>(other.m_vlrs).swap(m_vlrs);

}

LASHeader& LASHeader::operator=(LASHeader const& rhs)
{
    if (&rhs != this)
    {
        void* p = 0;
        p = std::memcpy(m_signature, rhs.m_signature, eFileSignatureSize);
        assert(p == m_signature);
        m_sourceId = rhs.m_sourceId;
        m_reserved = rhs.m_reserved;
        m_projectId1 = rhs.m_projectId1;
        m_projectId2 = rhs.m_projectId2;
        m_projectId3 = rhs.m_projectId3;
        p = std::memcpy(m_projectId4, rhs.m_projectId4, eProjectId4Size); 
        assert(p == m_projectId4);
        m_versionMajor = rhs.m_versionMajor;
        m_versionMinor = rhs.m_versionMinor;
        p = std::memcpy(m_systemId, rhs.m_systemId, eSystemIdSize);
        assert(p == m_systemId);
        p = std::memcpy(m_softwareId, rhs.m_softwareId, eSoftwareIdSize);
        assert(p == m_softwareId);
        m_createDOY = rhs.m_createDOY;
        m_createYear = rhs.m_createYear;
        m_headerSize = rhs.m_headerSize;
        m_dataOffset = rhs.m_dataOffset;
        m_recordsCount = rhs.m_recordsCount;
//        m_recordsCount = 0;
        m_dataFormatId = rhs.m_dataFormatId;
        m_dataRecordLen = rhs.m_dataRecordLen;
        m_pointRecordsCount = rhs.m_pointRecordsCount;
        
        std::vector<uint32_t>(rhs.m_pointRecordsByReturn).swap(m_pointRecordsByReturn);
        assert(ePointsByReturnSize >= m_pointRecordsByReturn.size());

        std::vector<LASVLR>(rhs.m_vlrs).swap(m_vlrs);
        m_scales = rhs.m_scales;
        m_offsets = rhs.m_offsets;
        m_extents = rhs.m_extents;
        m_proj4 = rhs.m_proj4;
    }
    return *this;
}

bool LASHeader::operator==(LASHeader const& other) const
{
    if (&other == this) return true;
    
    if (m_signature != other.m_signature) return false;
    if (m_sourceId != other.m_sourceId) return false;
    if (m_reserved != other.m_reserved) return false;
    if (m_projectId1 != other.m_projectId1) return false;
    if (m_projectId2 != other.m_projectId2) return false;
    if (m_projectId3 != other.m_projectId3) return false;
    if (m_projectId4 != other.m_projectId4) return false;
    if (m_versionMajor != other.m_versionMajor) return false;
    if (m_versionMinor != other.m_versionMinor) return false;
    if (m_systemId != other.m_systemId) return false;
    if (m_softwareId != other.m_softwareId) return false;
    if (m_createDOY != other.m_createDOY) return false;
    if (m_createYear != other.m_createYear) return false;
    if (m_headerSize != other.m_headerSize) return false;
    if (m_dataOffset != other.m_dataOffset) return false;
    if (m_recordsCount != other.m_recordsCount) return false;
    if (m_dataFormatId != other.m_dataFormatId) return false;
    if (m_dataRecordLen != other.m_dataRecordLen) return false;
    if (m_pointRecordsCount != other.m_pointRecordsCount) return false;
    if (m_pointRecordsByReturn != other.m_pointRecordsByReturn) return false;
    if (m_scales != other.m_scales) return false;
    if (m_offsets != other.m_offsets) return false;
    if (m_extents != other.m_extents) return false;
    if (m_proj4 != other.m_proj4) return false;
    
    return true;
}


std::string LASHeader::GetFileSignature() const
{
    return std::string(m_signature, eFileSignatureSize);
}

void LASHeader::SetFileSignature(std::string const& v)
{
    if (0 != v.compare(0, eFileSignatureSize, FileSignature))
        throw std::invalid_argument("invalid file signature");

    std::strncpy(m_signature, v.c_str(), eFileSignatureSize);
}

uint16_t LASHeader::GetFileSourceId() const
{
    return m_sourceId;
}

void LASHeader::SetFileSourceId(uint16_t v)
{
    // TODO: Should we warn or throw about type overflow occuring when
    //       user passes 65535 + 1 = 0
    m_sourceId = v;
}

uint16_t LASHeader::GetReserved() const
{
    return m_reserved;
}

void LASHeader::SetReserved(uint16_t v)
{
    // TODO: Should we warn or throw about type overflow occuring when
    //       user passes 65535 + 1 = 0
    m_reserved = v;
}

liblas::guid LASHeader::GetProjectId() const
{
    return liblas::guid(m_projectId1, m_projectId2, m_projectId3, m_projectId4);
}

void LASHeader::SetProjectId(guid const& v)
{
    v.output_data(m_projectId1, m_projectId2, m_projectId3, m_projectId4);
}

uint8_t LASHeader::GetVersionMajor() const
{
    return m_versionMajor;
}

void LASHeader::SetVersionMajor(uint8_t v)
{
    if (eVersionMajorMin > v || v > eVersionMajorMax)
        throw std::out_of_range("version major out of range");

    m_versionMajor = v;
}

uint8_t LASHeader::GetVersionMinor() const
{
    return m_versionMinor;
}

void LASHeader::SetVersionMinor(uint8_t v)
{
    if (v > eVersionMinorMax)
        throw std::out_of_range("version minor out of range");

    m_versionMinor = v;
}

std::string LASHeader::GetSystemId(bool pad /*= false*/) const
{
    // copy array of chars and trim zeros if smaller than 32 bytes
    std::string tmp(std::string(m_systemId, eSystemIdSize).c_str());

    // pad right side with spaces
    if (pad && tmp.size() < eSystemIdSize)
    {
        tmp.resize(eSystemIdSize, 0);
        assert(tmp.size() == eSystemIdSize);
    }

    assert(tmp.size() <= eSystemIdSize);
    return tmp;
}

void LASHeader::SetSystemId(std::string const& v)
{
    if (v.size() > eSystemIdSize)
        throw std::invalid_argument("system id too long");

    std::fill(m_systemId, m_systemId + eSystemIdSize, 0);
    std::strncpy(m_systemId, v.c_str(), eSystemIdSize);
}

std::string LASHeader::GetSoftwareId(bool pad /*= false*/) const
{
    std::string tmp(std::string(m_softwareId, eSoftwareIdSize).c_str());

    // pad right side with spaces
    if (pad && tmp.size() < eSoftwareIdSize)
    {
        tmp.resize(eSoftwareIdSize, 0);
        assert(tmp.size() == eSoftwareIdSize);
    }

    assert(tmp.size() <= eSoftwareIdSize);
    return tmp;
}

void LASHeader::SetSoftwareId(std::string const& v)
{
    if (v.size() > eSoftwareIdSize)
        throw std::invalid_argument("generating software id too long");
    
//    m_softwareId = v;
    std::fill(m_softwareId, m_softwareId + eSoftwareIdSize, 0);
    std::strncpy(m_softwareId, v.c_str(), eSoftwareIdSize);
}

uint16_t LASHeader::GetCreationDOY() const
{
    return m_createDOY;
}

void LASHeader::SetCreationDOY(uint16_t v)
{
    if (v > 366)
        throw std::out_of_range("day of year out of range");

    m_createDOY = v;
}

uint16_t LASHeader::GetCreationYear() const
{
    return m_createYear;
}

void LASHeader::SetCreationYear(uint16_t v)
{
    // mloskot: I've taken these values arbitrarily
    if (v > 9999)
        throw std::out_of_range("year out of range");

    m_createYear = v;
}

uint16_t LASHeader::GetHeaderSize() const
{
    return eHeaderSize;
}

uint32_t LASHeader::GetDataOffset() const
{
    return m_dataOffset;
}

void LASHeader::SetDataOffset(uint32_t v)
{
    uint32_t const dataSignatureSize = 2;
    uint16_t const hsize = GetHeaderSize();

    if ((eVersionMinorMin == m_versionMinor && v < hsize + dataSignatureSize)
        || (eVersionMinorMax == m_versionMinor && v < hsize))
    {
        throw std::out_of_range("data offset out of range");
    }

    m_dataOffset = v;
}

uint32_t LASHeader::GetRecordsCount() const
{
    return m_recordsCount;
}

void LASHeader::SetRecordsCount(uint32_t v)
{
    m_recordsCount = v;
}

LASHeader::PointFormat LASHeader::GetDataFormatId() const
{
    if (ePointFormat0 == m_dataFormatId)
        return ePointFormat0;
    else
        return ePointFormat1;
}

void LASHeader::SetDataFormatId(LASHeader::PointFormat v)
{
    m_dataFormatId = (ePointFormat0 == v ? 0 : 1);

    if (ePointFormat0 == m_dataFormatId)
        m_dataRecordLen = ePointSize0;
    else
        m_dataRecordLen = ePointSize1;
}

uint16_t LASHeader::GetDataRecordLength() const
{
    // NOTE: assertions below are used to check if our assumption is correct,
    // for debugging purpose only.

    if (ePointFormat0 == m_dataFormatId)
    {
        assert(ePointSize0 == m_dataRecordLen);
        return ePointSize0;
    }
    else
    {
        assert(ePointSize1 == m_dataRecordLen);
        return ePointSize1;
    }
}

uint32_t LASHeader::GetPointRecordsCount() const
{
    return m_pointRecordsCount;
}

void LASHeader::SetPointRecordsCount(uint32_t v)
{
    m_pointRecordsCount = v;
}

std::vector<uint32_t> const& LASHeader::GetPointRecordsByReturnCount() const
{
    return m_pointRecordsByReturn;
}

void LASHeader::SetPointRecordsByReturnCount(std::size_t index, uint32_t v)
{
    assert(m_pointRecordsByReturn.size() == LASHeader::ePointsByReturnSize);

    uint32_t& t = m_pointRecordsByReturn.at(index);
    t = v;
}


double LASHeader::GetScaleX() const
{
    return m_scales.x;
}

double LASHeader::GetScaleY() const
{
    return m_scales.y;
}

double LASHeader::GetScaleZ() const
{
    return m_scales.z;
}

void LASHeader::SetScale(double x, double y, double z)
{
    double const minscale = 0.01;
    m_scales.x = (0 == x) ? minscale : x;
    m_scales.y = (0 == y) ? minscale : y;
    m_scales.z = (0 == z) ? minscale : z;
}

double LASHeader::GetOffsetX() const
{
    return m_offsets.x;
}

double LASHeader::GetOffsetY() const
{
    return m_offsets.y;
}

double LASHeader::GetOffsetZ() const
{
    return m_offsets.z;
}

void LASHeader::SetOffset(double x, double y, double z)
{
    m_offsets = PointOffsets(x, y, z);
}

double LASHeader::GetMaxX() const
{
    return m_extents.max.x;
}

double LASHeader::GetMinX() const
{
    return m_extents.min.x;
}

double LASHeader::GetMaxY() const
{
    return m_extents.max.y;
}

double LASHeader::GetMinY() const
{
    return m_extents.min.y;
}

double LASHeader::GetMaxZ() const
{
    return m_extents.max.z;
}

double LASHeader::GetMinZ() const
{
    return m_extents.min.z;
}

void LASHeader::SetMax(double x, double y, double z)
{
    m_extents.max = detail::Point<double>(x, y, z);
}

void LASHeader::SetMin(double x, double y, double z)
{
    m_extents.min = detail::Point<double>(x, y, z);
}

void LASHeader::AddVLR(LASVLR const& v) 
{
    m_vlrs.push_back(v);

    uint32_t end_size = 0;
    std::vector<LASVLR>::const_iterator i;
        
    // Calculate a new data offset size
    for (i = m_vlrs.begin(); i != m_vlrs.end(); ++i) 
    {
        end_size += (*i).GetTotalSize();
    }

    uint32_t size = 0;
    uint32_t const dataSignatureSize = 2;
    
    // Add the signature if we're a 1.0 file    
    if (eVersionMinorMin == m_versionMinor) {
        size = end_size + dataSignatureSize; 
    } else {
        size = end_size;
    }

    SetDataOffset(GetHeaderSize()+size);

    // We're assuming that the reader or writer has reset 
    // the VLR count to 0 before adding them back with AddVLR  
    // FIXME I think this is still broken - hobu
    m_recordsCount += 1;
}

LASVLR const& LASHeader::GetVLR(uint32_t index) const 
{
    return m_vlrs[index];
}

void LASHeader::DeleteVLR(uint32_t index) 
{    
    if (index >= m_vlrs.size())
        throw std::out_of_range("index is out of range");

    std::vector<LASVLR>::iterator i = m_vlrs.begin() + index;

    // Deal with the dataoffset when deleting
    uint32_t size = (*i).GetTotalSize();

    m_vlrs.erase(i);
    m_recordsCount = static_cast<uint32_t>(m_vlrs.size());
    
    SetDataOffset(GetDataOffset() - size);
    
}

/// Fetch the Georeference as a proj.4 string
std::string LASHeader::GetProj4() const 
{
    return m_proj4;
}

       
void LASHeader::SetProj4(std::string const& v)
{
    m_proj4 = v;
}

void LASHeader::Init()
{
    // Initialize public header block with default
    // values according to LAS 1.1

    m_versionMajor = 1;
    m_versionMinor = 1;
    m_dataFormatId = ePointFormat0;
    m_dataRecordLen = ePointSize0;


    std::time_t now;
    std::tm *ptm;

    std::time(&now);
    ptm = std::gmtime(&now);
    
    m_createDOY = static_cast<uint16_t>(ptm->tm_yday);
    m_createYear = static_cast<uint16_t>(ptm->tm_year + 1900);
    
    m_headerSize = eHeaderSize;

    m_sourceId = m_reserved = m_projectId2 = m_projectId3 = uint16_t();
    m_projectId1 = uint32_t();
    std::memset(m_projectId4, 0, sizeof(m_projectId4)); 

    m_dataOffset = eHeaderSize; // excluding 2 bytes of Point Data Start Signature
    m_recordsCount = 0;
    m_pointRecordsCount = 0;

    std::memset(m_signature, 0, eFileSignatureSize);
    std::strncpy(m_signature, FileSignature, eFileSignatureSize);
//    m_signature = LASHeader::FileSignature;

    std::memset(m_systemId, 0, eSystemIdSize);
    std::strncpy(m_systemId, SystemIdentifier, eSystemIdSize);
//    m_systemId = LASHeader::SystemIdentifier;

    std::memset(m_softwareId, 0, eSoftwareIdSize);
    std::strncpy(m_softwareId, SoftwareIdentifier, eSoftwareIdSize);
//    m_softwareId = LASHeader::SoftwareIdentifier;

    m_pointRecordsByReturn.resize(ePointsByReturnSize);

    // Zero scale value is useless, so we need to use a small value.
    SetScale(0.01, 0.01, 0.01);
}

void LASHeader::ClearGeoKeyVLRs()
{
    std::string const uid("LASF_Projection");

    uint32_t beg_size = 0;
    uint32_t end_size = 0;

    std::vector<LASVLR> vlrs = m_vlrs;
    std::vector<LASVLR>::const_iterator i;
    std::vector<LASVLR>::iterator j;

    for (i = m_vlrs.begin(); i != m_vlrs.end(); ++i)
    {
        LASVLR record = *i;
        beg_size += (*i).GetTotalSize();

        std::string user = record.GetUserId(true);
        if (uid == user.c_str())
        {
            uint16_t id = record.GetRecordId();

            if (34735 == id)
            {
                // Geotiff SHORT key
                for(j = vlrs.begin(); j != vlrs.end(); ++j)
                {
                    if (*j == *i)
                    {
                        vlrs.erase(j);
                        break;
                    }
                }
            }
            else if (34736 == id)
            {
                // Geotiff DOUBLE key
                for(j = vlrs.begin(); j != vlrs.end(); ++j)
                {
                    if (*j == *i)
                    {
                        vlrs.erase(j);
                        break;
                    }
                }
            }        
            else if (34737 == id)
            {
                // Geotiff ASCII key
                for (j = vlrs.begin(); j != vlrs.end(); ++j)
                {
                    if (*j == *i)
                    {
                        vlrs.erase(j);
                        break;
                    }
                }
            }
        } // uid == user
    }
    
    // Copy our list of surviving VLRs back to our member variable
    // and update header information
    m_vlrs = vlrs;
    m_recordsCount = static_cast<uint32_t>(m_vlrs.size());
    
    // Calculate a new data offset size
    for (i = m_vlrs.begin(); i != m_vlrs.end(); ++i) 
    {
        end_size += (*i).GetTotalSize();
    }

    uint32_t size = 0;
    uint32_t const dataSignatureSize = 2;
    
    // Add the signature if we're a 1.0 file    
    if (eVersionMinorMin == m_versionMinor)
    {
        size = end_size + dataSignatureSize; 
    }
    else
    {
        size = end_size;
    }

    SetDataOffset(GetHeaderSize()+size);

}
void LASHeader::SetGeoreference() 
{
#ifndef HAVE_LIBGEOTIFF

    ;

#else

    int ret = 0;
    
    detail::raii_wrapper<ST_TIFF> st(ST_Create(), ST_Destroy);
    detail::raii_wrapper<GTIF> gt(GTIFNewSimpleTags(st.get()), GTIFFree);    
	
    // Wipe out any existing VLRs that represent geotiff keys on the 
    // header.  We're going to rewrite them to be totally derived from the 
    // proj4 string.
    ClearGeoKeyVLRs();
    
    if (GetProj4().empty())
		return;

    ret = GTIFSetFromProj4(gt.get(), GetProj4().c_str());
    if (!ret)
	{
        throw std::invalid_argument("PROJ.4 string is invalid or unsupported");
    }

    ret = GTIFWriteKeys(gt.get());
    if (!ret)
	{
        throw std::runtime_error("The geotiff keys could not be written");
    }
    
    short* kdata = NULL;
    short kvalue = 0;
    double* ddata = NULL;
    double dvalue = 0;
    int dtype = 0;
    int dcount = 0;
    int ktype = 0;
    int kcount = 0;

    //GTIFF_GEOKEYDIRECTORY == 34735
    ret = ST_GetKey(st.get(), 34735, &kcount, &ktype, (void**)&kdata);
    if (ret)
	{    
        LASVLR record;
        int i = 0;
        record.SetRecordId(34735);
        record.SetUserId("LASF_Projection");
        std::vector<uint8_t> data;

        // Shorts are 2 bytes in length
        uint16_t length = 2*kcount;

        record.SetRecordLength(length);
        
        // Copy the data into the data vector
        for (i = 0; i < kcount; i++)
		{
            kvalue = kdata[i];
            
            uint8_t* v = reinterpret_cast<uint8_t*>(&kvalue); 
            
            data.push_back(v[0]);
            data.push_back(v[1]);
        }

        record.SetData(data);

        AddVLR(record);
    }

    // FIXME We don't handle ASCII keys yet
    // GTIFF_ASCIIPARAMS == 34737
    // ret = ST_GetKey(st.get(), 34737, &acount, &atype, (void**)&adata);
    // if (ret) {
    //     
    //     LASVLR record;
    //     int i = 0;
    //     record.SetRecordId(34737);
    //     record.SetUserId("LASF_Projection");
    //     std::vector<uint8_t> data;
    // 
    //     uint32_t length = acount;
    //     record.SetRecordLength(length);
    //     
    //     // Copy the data into the data vector
    // 
    //     for (i=0; i<acount;i++) {
    //         avalue = adata[i];
    //         
    //         uint8_t* v =  reinterpret_cast<uint8_t*>(&avalue);
    //         
    //         data.push_back(v[0]);
    //         data.push_back(v[1]);
    //         data.push_back(v[2]);
    //         data.push_back(v[3]);
    //         data.push_back(v[4]);
    //         data.push_back(v[5]);
    //         data.push_back(v[6]);
    //         data.push_back(v[7]);
    //         
    //     }
    //     record.SetData(data);
    //     AddVLR(record);
    // }


    // GTIFF_DOUBLEPARAMS == 34736
    ret = ST_GetKey(st.get(), 34736, &dcount, &dtype, (void**)&ddata);

    if (ret)
	{       
        LASVLR record;
        int i = 0;
        record.SetRecordId(34736);
        record.SetUserId("LASF_Projection");
        std::vector<uint8_t> data;

        // Doubles are 8 bytes in length
        uint16_t length = 8*dcount;
        record.SetRecordLength(length);
        
        // Copy the data into the data vector
        for (i=0; i<dcount;i++)
		{
            dvalue = ddata[i];
            
            uint8_t* v =  reinterpret_cast<uint8_t*>(&dvalue);
            
            data.push_back(v[0]);
            data.push_back(v[1]);
            data.push_back(v[2]);
            data.push_back(v[3]);
            data.push_back(v[4]);
            data.push_back(v[5]);
            data.push_back(v[6]);
            data.push_back(v[7]);   
        }
        
		record.SetData(data);
        AddVLR(record);
    }
#endif // HAVE_LIBGEOTIFF
}

} // namespace liblas

