/******************************************************************************
 * $Id$
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

#include <liblas/guid.hpp>
#include <liblas/lasheader.hpp>
#include <liblas/lasspatialreference.hpp>
#include <liblas/lasschema.hpp>
#include <liblas/detail/private_utility.hpp>
// boost
#include <boost/cstdint.hpp>
#include <boost/lambda/lambda.hpp>
//std
#include <algorithm>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <cstring> // std::memset, std::memcpy, std::strncpy
#include <cassert>
#include <ctime>

using namespace boost;

namespace liblas {

char const* const Header::FileSignature = "LASF";
char const* const Header::SystemIdentifier = "libLAS";
char const* const Header::SoftwareIdentifier = "libLAS 1.6.0b2";


Header::Header() : m_schema(ePointFormat3)
{
    Init();
}

Header::Header(Header const& other) :
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
    // m_dataFormatId(other.m_dataFormatId),
    // m_dataRecordLen(other.m_dataRecordLen),
    m_pointRecordsCount(other.m_pointRecordsCount),
    m_scales(other.m_scales),
    m_offsets(other.m_offsets),
    m_extent(other.m_extent),
    m_srs(other.m_srs),
    m_schema(other.m_schema)
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
    
    std::vector<VariableRecord>(other.m_vlrs).swap(m_vlrs);

}

Header& Header::operator=(Header const& rhs)
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
        // m_dataFormatId = rhs.m_dataFormatId;
        // m_dataRecordLen = rhs.m_dataRecordLen;
        m_pointRecordsCount = rhs.m_pointRecordsCount;
        
        std::vector<uint32_t>(rhs.m_pointRecordsByReturn).swap(m_pointRecordsByReturn);
        assert(ePointsByReturnSize >= m_pointRecordsByReturn.size());

        std::vector<VariableRecord>(rhs.m_vlrs).swap(m_vlrs);
        m_scales = rhs.m_scales;
        m_offsets = rhs.m_offsets;
        m_extent = rhs.m_extent;
        m_srs = rhs.m_srs;
        m_schema = rhs.m_schema;

    }
    return *this;
}

bool Header::operator==(Header const& other) const
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
    // if (m_dataFormatId != other.m_dataFormatId) return false;
    // if (m_dataRecordLen != other.m_dataRecordLen) return false;
    if (m_pointRecordsCount != other.m_pointRecordsCount) return false;
    if (m_pointRecordsByReturn != other.m_pointRecordsByReturn) return false;
    if (m_scales != other.m_scales) return false;
    if (m_offsets != other.m_offsets) return false;
    if (m_extent != other.m_extent) return false;
    
    return true;
}


std::string Header::GetFileSignature() const
{
    return std::string(m_signature, eFileSignatureSize);
}

void Header::SetFileSignature(std::string const& v)
{
    if (0 != v.compare(0, eFileSignatureSize, FileSignature))
        throw std::invalid_argument("invalid file signature");

    std::strncpy(m_signature, v.c_str(), eFileSignatureSize);
}

uint16_t Header::GetFileSourceId() const
{
    return m_sourceId;
}

void Header::SetFileSourceId(uint16_t v)
{
    // TODO: Should we warn or throw about type overflow occuring when
    //       user passes 65535 + 1 = 0
    m_sourceId = v;
}

uint16_t Header::GetReserved() const
{
    return m_reserved;
}

void Header::SetReserved(uint16_t v)
{
    // TODO: Should we warn or throw about type overflow occuring when
    //       user passes 65535 + 1 = 0
    m_reserved = v;
}

liblas::guid Header::GetProjectId() const
{
    return liblas::guid(m_projectId1, m_projectId2, m_projectId3, m_projectId4);
}

void Header::SetProjectId(guid const& v)
{
    v.output_data(m_projectId1, m_projectId2, m_projectId3, m_projectId4);
}

uint8_t Header::GetVersionMajor() const
{
    return m_versionMajor;
}

void Header::SetVersionMajor(uint8_t v)
{
    if (eVersionMajorMin > v || v > eVersionMajorMax)
        throw std::out_of_range("version major out of range");

    m_versionMajor = v;
}

uint8_t Header::GetVersionMinor() const
{
    return m_versionMinor;
}

void Header::SetVersionMinor(uint8_t v)
{
    if (v > eVersionMinorMax)
        throw std::out_of_range("version minor out of range");
    
    m_versionMinor = v;


}

std::string Header::GetSystemId(bool pad /*= false*/) const
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

void Header::SetSystemId(std::string const& v)
{
    if (v.size() > eSystemIdSize)
        throw std::invalid_argument("system id too long");

    std::fill(m_systemId, m_systemId + eSystemIdSize, 0);
    std::strncpy(m_systemId, v.c_str(), eSystemIdSize);
}

std::string Header::GetSoftwareId(bool pad /*= false*/) const
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

void Header::SetSoftwareId(std::string const& v)
{
    if (v.size() > eSoftwareIdSize)
        throw std::invalid_argument("generating software id too long");
    
//    m_softwareId = v;
    std::fill(m_softwareId, m_softwareId + eSoftwareIdSize, 0);
    std::strncpy(m_softwareId, v.c_str(), eSoftwareIdSize);
}

uint16_t Header::GetCreationDOY() const
{
    return m_createDOY;
}

void Header::SetCreationDOY(uint16_t v)
{
    if (v > 366)
        throw std::out_of_range("day of year out of range");

    m_createDOY = v;
}

uint16_t Header::GetCreationYear() const
{
    return m_createYear;
}

void Header::SetCreationYear(uint16_t v)
{
    // mloskot: I've taken these values arbitrarily
    if (v > 9999)
        throw std::out_of_range("year out of range");

    m_createYear = v;
}

uint16_t Header::GetHeaderSize() const
{
    return m_headerSize;
}

void Header::SetHeaderSize(uint16_t v)
{

    m_headerSize = v;
}

uint32_t Header::GetDataOffset() const
{
    return m_dataOffset;
}

void Header::SetDataOffset(uint32_t v)
{
    // uint32_t const dataSignatureSize = 2;
    // uint16_t const hsize = GetHeaderSize();
    // 
    // if ( (m_versionMinor == 0 && v < hsize + dataSignatureSize) ||
    //      (m_versionMinor == 1 && v < hsize) ||
    //      (m_versionMinor == 2 && v < hsize) )
    // {
    //     throw std::out_of_range("data offset out of range");
    // }
    
    m_dataOffset = v;
    
}

uint32_t Header::GetRecordsCount() const
{
    return m_recordsCount;
}

void Header::SetRecordsCount(uint32_t v)
{
    m_recordsCount = v;
}

liblas::PointFormatName Header::GetDataFormatId() const
{
    return m_schema.GetDataFormatId();

}

void Header::SetDataFormatId(liblas::PointFormatName v)
{
    m_schema.SetDataFormatId(v);
}

uint16_t Header::GetDataRecordLength() const
{
    // No matter what the schema says, this must be a a short in size.
    return static_cast<boost::uint16_t>(m_schema.GetByteSize());
}

uint32_t Header::GetPointRecordsCount() const
{
    return m_pointRecordsCount;
}

void Header::SetPointRecordsCount(uint32_t v)
{
    m_pointRecordsCount = v;
}

Header::RecordsByReturnArray const& Header::GetPointRecordsByReturnCount() const
{
    return m_pointRecordsByReturn;
}

void Header::SetPointRecordsByReturnCount(std::size_t index, uint32_t v)
{
    assert(m_pointRecordsByReturn.size() == Header::ePointsByReturnSize);

    uint32_t& t = m_pointRecordsByReturn.at(index);
    t = v;
}


double Header::GetScaleX() const
{
    return m_scales.x;
}

double Header::GetScaleY() const
{
    return m_scales.y;
}

double Header::GetScaleZ() const
{
    return m_scales.z;
}

void Header::SetScale(double x, double y, double z)
{

    double const minscale = 0.01;
    m_scales.x = (detail::compare_distance(0.0, x)) ? minscale : x;
    m_scales.y = (detail::compare_distance(0.0, y)) ? minscale : y;
    m_scales.z = (detail::compare_distance(0.0, z)) ? minscale : z;
}

double Header::GetOffsetX() const
{
    return m_offsets.x;
}

double Header::GetOffsetY() const
{
    return m_offsets.y;
}

double Header::GetOffsetZ() const
{
    return m_offsets.z;
}

void Header::SetOffset(double x, double y, double z)
{
    m_offsets = PointOffsets(x, y, z);
}

double Header::GetMaxX() const
{
    return m_extent.max(0);
}

double Header::GetMinX() const
{
    return m_extent.min(0);
}

double Header::GetMaxY() const
{
    return m_extent.max(1);
}

double Header::GetMinY() const
{
    return m_extent.min(1);
}

double Header::GetMaxZ() const
{
    return m_extent.max(2);
}

double Header::GetMinZ() const
{
    return m_extent.min(2);
}

void Header::SetMax(double x, double y, double z)
{
    // m_extent = Bounds(m_extent.min(0), m_extent.min(1), m_extent.max(0), m_extent.max(1), m_extent.min(2), m_extent.max(2));
    // Bounds(minx, miny, minz, maxx, maxy, maxz)
    m_extent = Bounds<double>(m_extent.min(0), m_extent.min(1), m_extent.min(2), x, y, z);
}

void Header::SetMin(double x, double y, double z)
{
    m_extent = Bounds<double>(x, y, z, m_extent.max(0), m_extent.max(1), m_extent.max(2));
}

void Header::SetExtent(Bounds<double> const& extent)
{
    m_extent = extent;
}

const Bounds<double>& Header::GetExtent() const
{
    return m_extent;
}

void Header::AddVLR(VariableRecord const& v) 
{
    m_vlrs.push_back(v);
    m_recordsCount += 1;
}

VariableRecord const& Header::GetVLR(uint32_t index) const 
{
    return m_vlrs[index];
}

const std::vector<VariableRecord>& Header::GetVLRs() const
{
    return m_vlrs;
}

void Header::DeleteVLR(uint32_t index) 
{    
    if (index >= m_vlrs.size())
        throw std::out_of_range("index is out of range");

    std::vector<VariableRecord>::iterator i = m_vlrs.begin() + index;

    m_vlrs.erase(i);
    m_recordsCount = static_cast<uint32_t>(m_vlrs.size());

}


void Header::Init()
{
    // Initialize public header block with default
    // values according to LAS 1.2

    m_versionMajor = 1;
    m_versionMinor = 2;
    // m_dataFormatId = ePointFormat0;
    // m_dataRecordLen = ePointSize0;
    
    m_createDOY = m_createYear = 0;
    std::time_t now;
    std::time(&now);
    std::tm* ptm = std::gmtime(&now);
    if (0 != ptm)
    {
        m_createDOY = static_cast<uint16_t>(ptm->tm_yday);
        m_createYear = static_cast<uint16_t>(ptm->tm_year + 1900);
    }

    m_headerSize = eHeaderSize;

    m_sourceId = m_reserved = m_projectId2 = m_projectId3 = uint16_t();
    m_projectId1 = uint32_t();
    std::memset(m_projectId4, 0, sizeof(m_projectId4)); 

    m_dataOffset = eHeaderSize; // excluding 2 bytes of Point Data Start Signature
    m_recordsCount = 0;
    m_pointRecordsCount = 0;

    std::memset(m_signature, 0, eFileSignatureSize);
    std::strncpy(m_signature, FileSignature, eFileSignatureSize);
//    m_signature = Header::FileSignature;

    std::memset(m_systemId, 0, eSystemIdSize);
    std::strncpy(m_systemId, SystemIdentifier, eSystemIdSize);
//    m_systemId = Header::SystemIdentifier;

    std::memset(m_softwareId, 0, eSoftwareIdSize);
    std::strncpy(m_softwareId, SoftwareIdentifier, eSoftwareIdSize);
//    m_softwareId = Header::SoftwareIdentifier;

    m_pointRecordsByReturn.resize(ePointsByReturnSize);

    // Zero scale value is useless, so we need to use a small value.
    SetScale(0.01, 0.01, 0.01);
}

bool SameVLRs(std::string const& name, boost::uint16_t id, liblas::VariableRecord const& record)
{
    if (record.GetUserId(false) == name) {
        if (record.GetRecordId() == id) {
            return true;
        }
    }
    return false;
}


void Header::DeleteVLRs(std::string const& name, boost::uint16_t id)
{

    m_vlrs.erase( std::remove_if( m_vlrs.begin(), 
                                  m_vlrs.end(),
                                  boost::bind( &SameVLRs, name, id, _1 ) ),
                  m_vlrs.end());

    m_recordsCount = static_cast<uint32_t>(m_vlrs.size());        

}



void Header::SetGeoreference() 
{    
    std::vector<VariableRecord> vlrs = m_srs.GetVLRs();

    // Wipe the GeoTIFF-related VLR records off of the Header
    DeleteVLRs("LASF_Projection", 34735);
    DeleteVLRs("LASF_Projection", 34736);
    DeleteVLRs("LASF_Projection", 34737);

    std::vector<VariableRecord>::const_iterator i;

    for (i = vlrs.begin(); i != vlrs.end(); ++i) 
    {
        AddVLR(*i);
    }
}

SpatialReference Header::GetSRS() const
{
    return m_srs;
}

void Header::SetSRS(SpatialReference& srs)
{
    m_srs = srs;
}

Schema const& Header::GetSchema() const
{
    
    return m_schema;
}

void Header::SetSchema(const Schema& format)
{

    // // A user can use the set the header's version information and 
    // // format information and sizes by using a PointFormat instance
    // // in addition to setting all the settings individually by hand

    // // The DataRecordLength will be set to the max of either the format's 
    // // byte size or the pointformat's specified size according to whether 
    // // or not it has color or time (FIXME: or waveform packets once we get to 1.3 )
    // // The extra space that is available can be used to store LASPoint::GetExtraData.
    // // We trim the format size to uint16_t because that's what the header stores 

    // if (format.HasColor() && format.HasTime()) {
    //     SetDataFormatId(liblas::ePointFormat3);
    //     SetDataRecordLength(std::max(   static_cast<uint16_t>(ePointSize3),
    //                                     static_cast<uint16_t>(format.GetByteSize())));
    // } else if (format.HasColor()  && !format.HasTime()) {
    //     SetDataFormatId(liblas::ePointFormat2);
    //     SetDataRecordLength(std::max(   static_cast<uint16_t>(ePointSize2),
    //                                     static_cast<uint16_t>(format.GetByteSize())));
    // } else if (!format.HasColor()  && format.HasTime()) {
    //     SetDataFormatId(liblas::ePointFormat1);
    //     SetDataRecordLength(std::max(   static_cast<uint16_t>(ePointSize1),
    //                                     static_cast<uint16_t>(format.GetByteSize())));
    // } else {
    //     SetDataFormatId(liblas::ePointFormat0);
    //     SetDataRecordLength(std::max(   static_cast<uint16_t>(ePointSize0),
    //                                     static_cast<uint16_t>(format.GetByteSize())));
    // }
    
    m_schema = format;
    
    Dimension x = m_schema.GetDimension("X");
    x.SetScale(m_scales.x);
    x.IsFinitePrecision(true);
    x.SetOffset(m_offsets.x);
    m_schema.SetDimension(x);
    
    Dimension y = m_schema.GetDimension("Y");
    y.SetScale(m_scales.y);
    y.IsFinitePrecision(true);
    y.SetOffset(m_offsets.y);
    m_schema.SetDimension(y);
    
    Dimension z = m_schema.GetDimension("Z");
    z.SetScale(m_scales.z);
    z.IsFinitePrecision(true);
    z.SetOffset(m_offsets.z);
    m_schema.SetDimension(z);
    
} 

liblas::property_tree::ptree Header::GetPTree( ) const
{
    using liblas::property_tree::ptree;
    ptree pt;
    
    pt.put("filesignature", GetFileSignature());
    pt.put("projectdid", GetProjectId());
    pt.put("systemid", GetSystemId());
    pt.put("softwareid", GetSoftwareId());
    
    
    std::ostringstream version;
    version << static_cast<int>(GetVersionMajor());
    version <<".";
    version << static_cast<int>(GetVersionMinor());
    pt.put("version", version.str());
    
    pt.put("filesourceid", GetFileSourceId());
    pt.put("reserved", GetReserved());

// #ifdef HAVE_GDAL
//     pt.put("srs", GetSRS().GetWKT(liblas::SpatialReference::eHorizontalOnly, true));
// #else
// #ifdef HAVE_LIBGEOTIFF
//     pt.put("srs", GetSRS().GetProj4());
// #endif
// #endif
    
    ptree srs = GetSRS().GetPTree();
    pt.add_child("srs", srs);
    
    std::ostringstream date;
    date << GetCreationDOY() << "/" << GetCreationYear();
    pt.put("date", date.str());
    
    pt.put("size", GetHeaderSize());
    pt.put("dataoffset", GetDataOffset());

    
    pt.put("count", GetPointRecordsCount());
    pt.put("dataformatid", GetDataFormatId());
    pt.put("datarecordlength", GetDataRecordLength());
    
    ptree return_count;
    liblas::Header::RecordsByReturnArray returns = GetPointRecordsByReturnCount();
    for (boost::uint32_t i=0; i< 5; i++){
        ptree r;
        r.put("id", i);
        r.put("count", returns[i]);
        return_count.add_child("return", r);
    }
    pt.add_child("returns", return_count);
    
    pt.put("scale.x", GetScaleX());
    pt.put("scale.y", GetScaleY());
    pt.put("scale.z", GetScaleZ());
    
    pt.put("offset.x", GetOffsetX());
    pt.put("offset.y", GetOffsetY());
    pt.put("offset.z", GetOffsetZ());
    
    pt.put("minimum.x", GetMinX());
    pt.put("minimum.y", GetMinY());
    pt.put("minimum.z", GetMinZ());
    
    pt.put("maximum.x", GetMaxX());
    pt.put("maximum.y", GetMaxY());
    pt.put("maximum.z", GetMaxZ());

    
    for (boost::uint32_t i=0; i< GetRecordsCount(); i++) {
        pt.add_child("vlrs.vlr", GetVLR(i).GetPTree());
    }    
    
    return pt;
}

void Header::to_rst(std::ostream& os) const
{

    using liblas::property_tree::ptree;
    ptree tree = GetPTree();

    os << "---------------------------------------------------------" << std::endl;
    os << "  Header Summary" << std::endl;
    os << "---------------------------------------------------------" << std::endl;
    os << std::endl;

    os << "  Version:                     " << tree.get<std::string>("version") << std::endl;
    os << "  Source ID:                   " << tree.get<boost::uint32_t>("filesourceid") << std::endl;
    os << "  Reserved:                    " << tree.get<std::string>("reserved") << std::endl;
    os << "  Project ID/GUID:             '" << tree.get<std::string>("projectdid") << "'" << std::endl;
    os << "  System ID:                   '" << tree.get<std::string>("systemid") << "'" << std::endl;
    os << "  Generating Software:         '" << tree.get<std::string>("softwareid") << "'" << std::endl;
    os << "  File Creation Day/Year:      " << tree.get<std::string>("date") << std::endl;
    os << "  Header Byte Size             " << tree.get<boost::uint32_t>("size") << std::endl;
    os << "  Data Offset:                 " << tree.get<std::string>("dataoffset") << std::endl;

    os << "  Number Var. Length Records:  ";
    try {
      os << tree.get_child("vlrs").size();
    }
    catch (liblas::property_tree::ptree_bad_path const& e) {
      ::boost::ignore_unused_variable_warning(e);
      os << "None";
    }
    os << std::endl;

    os << "  Point Data Format:           " << tree.get<boost::uint32_t>("dataformatid") << std::endl;
    os << "  Number of Point Records:     " << tree.get<boost::uint32_t>("count") << std::endl;

    std::ostringstream returns_oss;
    BOOST_FOREACH(ptree::value_type &v,
          tree.get_child("returns"))
    {
          returns_oss << v.second.get<std::string>("count")<< " ";

    }        

    os << "  Number of Points by Return:  " << returns_oss.str() << std::endl;

    os.setf(std::ios_base::fixed, std::ios_base::floatfield);
    double x_scale = tree.get<double>("scale.x");
    double y_scale = tree.get<double>("scale.y");
    double z_scale = tree.get<double>("scale.z");

    boost::uint32_t x_precision = 6;
    boost::uint32_t y_precision = 6;
    boost::uint32_t z_precision = 6;
    double frac = 0;
    double integer = 0;
    
    frac = std::modf(x_scale, &integer);
    x_precision = static_cast<boost::uint32_t>(std::fabs(std::floor(std::log10(frac))));
    frac = std::modf(y_scale, &integer);
    y_precision = static_cast<boost::uint32_t>(std::fabs(std::floor(std::log10(frac))));
    frac = std::modf(z_scale, &integer);
    z_precision = static_cast<boost::uint32_t>(std::fabs(std::floor(std::log10(frac))));

    os << "  Scale Factor X Y Z:          ";
    os.precision(x_precision);
    os << tree.get<double>("scale.x") << " "; 
    os.precision(y_precision);
    os << tree.get<double>("scale.y") << " "; 
    os.precision(z_precision);
    os << tree.get<double>("scale.z") << std::endl;

    os << "  Offset X Y Z:                ";
    os.precision(x_precision);
    os << tree.get<double>("offset.x") << " ";
    os.precision(y_precision);
    os << tree.get<double>("offset.y") << " ";
    os.precision(z_precision);
    os << tree.get<double>("offset.z") << std::endl;

    os << "  Min X Y Z:                   ";
    os.precision(x_precision);
    os << tree.get<double>("minimum.x") << " "; 
    os.precision(y_precision);
    os << tree.get<double>("minimum.y") << " ";
    os.precision(z_precision);
    os << tree.get<double>("minimum.z") << std::endl;

    os << "  Max X Y Z:                   ";
    os.precision(x_precision);
    os << tree.get<double>("maximum.x") << " ";
    os.precision(y_precision);
    os << tree.get<double>("maximum.y") << " ";
    os.precision(z_precision);
    os << tree.get<double>("maximum.z") << std::endl;         

    os << "  Spatial Reference:  " << std::endl;
    os << tree.get<std::string>("srs.prettywkt") << std::endl;
    os << tree.get<std::string>("srs.gtiff") << std::endl;   

    // os << "---------------------------------------------------------" << std::endl;
    // os << "  VLR Summary" << std::endl;
    // os << "---------------------------------------------------------" << std::endl;

    // try {
    //     std::ostringstream vlrs_oss;
    //     BOOST_FOREACH(ptree::value_type &v,
    //             tree.get_child("vlrs"))
    //     {
    //             vlrs_oss << "    User: '" 
    //                      << v.second.get<std::string>("userid")
    //                      << "' - Description: '"
    //                      << v.second.get<std::string>("description") 
    //                      <<"'" 
    //                      << std::endl;
    //             vlrs_oss << "    ID: " << v.second.get<boost::uint32_t>("id")
    //                      << " Length: " <<v.second.get<boost::uint32_t>("length")
    //                      << std::endl;
    //     }
    // 
    //     os << vlrs_oss.str();
    // }
    // catch (liblas::property_tree::ptree_bad_path const& e) {
    //     ::boost::ignore_unused_variable_warning(e);
    // }
}
std::ostream& operator<<(std::ostream& os, liblas::Header const& h)
{

    
    h.to_rst(os);
    return os;
    
}
} // namespace liblas
