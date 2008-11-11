/******************************************************************************
 * $Id: lasheader.hpp 875 2008-09-20 20:28:41Z hobu $
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

#ifndef LIBLAS_LASHEADER_HPP_INCLUDED
#define LIBLAS_LASHEADER_HPP_INCLUDED

#include <liblas/cstdint.hpp>
#include <liblas/guid.hpp>
#include <liblas/detail/utility.hpp>
#include <liblas/lasrecordheader.hpp>
//std
#include <string>
#include <vector>

namespace liblas {

/// Definition of public header block.
/// The header contains set of generic data and metadata
/// describing a family of ASPRS LAS files. The header is stored
/// at the beginning of every valid ASPRS LAS file.
class LASHeader
{
public:

    /// Range of allowed ASPRS LAS file format versions.
    enum FormatVersion
    {
        eVersionMajorMin = 1, ///< Minimum of major component
        eVersionMajorMax = 1, ///< Maximum of major component
        eVersionMinorMin = 0, ///< Minimum of minor component
        eVersionMinorMax = 1 ///< Maximum of minor component
    };

    /// Versions of point record format.
    enum PointFormat
    {
        ePointFormat0 = 0, ///< Point Data Format \e 0
        ePointFormat1 = 1  ///< Point Data Format \e 1
    };

    /// Number of bytes of point record storage in particular format.
    enum PointSize
    {
        ePointSize0 = 20, ///< Size of point record in data format \e 0
        ePointSize1 = 28  ///< Size of point record in data format \e 1
    };

    /// Official signature of ASPRS LAS file format, always \b "LASF".
    static char const* const FileSignature;

    /// Default system identifier used by libLAS, always \b "libLAS".
    static char const* const SystemIdentifier;

    /// Default software identifier used by libLAS, always \b "libLAS X.Y".
    static char const* const SoftwareIdentifier;

    /// Default constructor.
    /// The default constructed header is configured according to the ASPRS
    /// LAS 1.1 Specification, point data format set to 0.
    /// Other fields filled with 0.
    LASHeader();

    /// Copy constructor.
    LASHeader(LASHeader const& other);

    /// Assignment operator.
    LASHeader& operator=(LASHeader const& rhs);
    
    /// Comparison operator.
    bool operator==(const LASHeader& other) const;

    /// Get ASPRS LAS file signature.
    /// \return 4-characters long string - \b "LASF".
    std::string GetFileSignature() const;

    /// Set ASPRS LAS file signature.
    /// The only value allowed as file signature is \b "LASF",
    /// defined as FileSignature constant.
    /// \exception std::invalid_argument - if invalid signature given.
    /// \param v - string contains file signature, at least 4-bytes long
    /// with "LASF" as first four bytes.
    void SetFileSignature(std::string const& v);

    /// Get file source identifier.
    /// \exception No throw
    uint16_t GetFileSourceId() const;

    /// Set file source identifier.
    /// \param v - should be set to a value between 1 and 65535.
    /// \exception No throw
    void SetFileSourceId(uint16_t v);

    /// Get value field reserved by the ASPRS LAS Specification.
    /// \note This field is always filled with 0.
    uint16_t GetReserved() const;

    /// Set reserved value for the header identifier.
    /// \param v - should be set to a value between 1 and 65535.
    /// \exception No throw
    void SetReserved(uint16_t v);

    /// Get project identifier.
    /// \return Global Unique Identifier as an instance of liblas::guid class.
    guid GetProjectId() const;

    /// Set project identifier.
    void SetProjectId(guid const& v);

    /// Get major component of version of LAS format.
    /// \return Always 1 is returned as the only valid value.
    uint8_t GetVersionMajor() const;

    /// Set major component of version of LAS format.
    /// \exception std::out_of_range - invalid value given.
    /// \param v - value between eVersionMajorMin and eVersionMajorMax.
    void SetVersionMajor(uint8_t v);

    /// Get minor component of version of LAS format.
    /// \return Valid values are 1 or 0.
    uint8_t GetVersionMinor() const;

    /// Set minor component of version of LAS format.
    /// \exception std::out_of_range - invalid value given.
    /// \param v - value between eVersionMinorMin and eVersionMinorMax.
    void SetVersionMinor(uint8_t v);

    /// Get system identifier.
    /// Default value is \b "libLAS" specified as the SystemIdentifier constant.
    /// \param pad - if true the returned string is padded right with spaces and
    /// its length is 32 bytes, if false (default) no padding occurs and
    /// length of the returned string is <= 32 bytes.
    /// \return value of system identifier field.
    std::string GetSystemId(bool pad = false) const;

    /// Set system identifier.
    /// \exception std::invalid_argument - if identifier longer than 32 bytes.
    /// \param v - system identifiers string.
    void SetSystemId(std::string const& v);

    /// Get software identifier.
    /// Default value is \b "libLAS 1.0", specified as the SoftwareIdentifier constant.
    /// \param pad - if true the returned string is padded right with spaces and its length is 32 bytes,
    /// if false (default) no padding occurs and length of the returned string is <= 32 bytes.
    /// \return value of generating software identifier field.
    std::string GetSoftwareId(bool pad = false) const;

    /// Set software identifier.
    /// \exception std::invalid_argument - if identifier is longer than 32 bytes.
    /// \param v - software identifiers string.
    void SetSoftwareId(std::string const& v);

    /// Get day of year of file creation date.
    /// \todo Use full date structure instead of Julian date number.
    uint16_t GetCreationDOY() const;

    /// Set day of year of file creation date.
    /// \exception std::out_of_range - given value is higher than number 366.
    /// \todo Use full date structure instead of Julian date number.
    void SetCreationDOY(uint16_t v);

    /// Set year of file creation date.
    /// \todo Remove if full date structure is used.
    uint16_t GetCreationYear() const;

    /// Get year of file creation date.
    /// \exception std::out_of_range - given value is higher than number 9999.
    /// \todo Remove if full date structure is used.
    void SetCreationYear(uint16_t v);

    /// Get number of bytes of generic verion of public header block storage.
    /// Standard version of the public header block is 227 bytes long.
    uint16_t GetHeaderSize() const;
    
    /// Get number of bytes from the beginning to the first point record.
    uint32_t GetDataOffset() const;

    /// Set number of bytes from the beginning to the first point record.
    /// \exception std::out_of_range - if given offset is bigger than 227+2 bytes
    /// for the LAS 1.0 format and 227 bytes for the LAS 1.1 format.
    void SetDataOffset(uint32_t v);

    /// Get number of variable-length records.
    uint32_t GetRecordsCount() const;

    /// Set number of variable-length records.
    void SetRecordsCount(uint32_t v);
    
    /// Get identifier of point data (record) format.
    PointFormat GetDataFormatId() const;

    /// Set identifier of point data (record) format.
    void SetDataFormatId(PointFormat v);

    /// \todo To be documented
    uint16_t GetDataRecordLength() const;
    
    /// Get total number of point records stored in the LAS file.
    uint32_t GetPointRecordsCount() const;

    /// Set number of point records that will be stored in a new LAS file.
    void SetPointRecordsCount(uint32_t v);
    
    /// Get array of the total point records per return.
    std::vector<uint32_t> const& GetPointRecordsByReturnCount() const;

    /// Set values of 5-elements array of total point records per return.
    /// \exception std::out_of_range - if index is bigger than 4.
    /// \param index - subscript (0-4) of array element being updated.
    /// \param v - new value to assign to array element identified by index.
    void SetPointRecordsByReturnCount(std::size_t index, uint32_t v);
    
    /// Get scale factor for X coordinate.
    double GetScaleX() const;

    /// Get scale factor for Y coordinate.
    double GetScaleY() const;
    
    /// Get scale factor for Z coordinate.
    double GetScaleZ() const;

    /// Set values of scale factor for X, Y and Z coordinates.
    void SetScale(double x, double y, double z);

    /// Get X coordinate offset.
    double GetOffsetX() const;
    
    /// Get Y coordinate offset.
    double GetOffsetY() const;
    
    /// Get Z coordinate offset.
    double GetOffsetZ() const;

    /// Set values of X, Y and Z coordinates offset.
    void SetOffset(double x, double y, double z);

    /// Get minimum value of extent of X coordinate.
    double GetMaxX() const;

    /// Get maximum value of extent of X coordinate.
    double GetMinX() const;

    /// Get minimum value of extent of Y coordinate.
    double GetMaxY() const;

    /// Get maximum value of extent of Y coordinate.
    double GetMinY() const;

    /// Get minimum value of extent of Z coordinate.
    double GetMaxZ() const;

    /// Get maximum value of extent of Z coordinate.
    double GetMinZ() const;

    /// Set maximum values of extent of X, Y and Z coordinates.
    void SetMax(double x, double y, double z);

    /// Set minimum values of extent of X, Y and Z coordinates.
    void SetMin(double x, double y, double z);

    /// Adds a variable length record to the header
    void AddVLR(LASVLR const& v);
    
    /// Returns a VLR 
    LASVLR const& GetVLR(uint32_t index) const;

    /// Removes a VLR from the the header.
    void DeleteVLR(uint32_t index);

    /// Fetch the georeference as a PROJ.4 definition string.
    std::string GetProj4() const;
    
    /// Set the georeference as a proj.4 string
    void SetProj4(std::string const& v);
    
	/// Rewrite variable-length record with georeference infomation, if available.
    void SetGeoreference();
    
private:
    
    typedef detail::Point<double> PointScales;
    typedef detail::Point<double> PointOffsets;
    typedef detail::Extents<double> PointExtents;

    enum
    {
        eDataSignatureSize = 2,
        eFileSignatureSize = 4,
        ePointsByReturnSize = 5,
        eProjectId4Size = 8,
        eSystemIdSize = 32,
        eSoftwareIdSize = 32,
        eHeaderSize = 227,
        eFileSourceIdMax = 65535
    };

    // TODO (low-priority): replace static-size char arrays
    // with std::string and return const-reference to string object.
    
    //
    // Private function members
    //
    void Init();
    void ClearGeoKeyVLRs();

    //
    // Private data members
    //
    char m_signature[eFileSignatureSize];
    uint16_t m_sourceId;
    uint16_t m_reserved;
    uint32_t m_projectId1;
    uint16_t m_projectId2;
    uint16_t m_projectId3;
    uint8_t m_projectId4[eProjectId4Size];
    uint8_t m_versionMajor;
    uint8_t m_versionMinor;
    char m_systemId[eSystemIdSize];
    char m_softwareId[eSoftwareIdSize];
    uint16_t m_createDOY;
    uint16_t m_createYear;
    uint16_t m_headerSize;
    uint32_t m_dataOffset;
    uint32_t m_recordsCount;
    uint8_t m_dataFormatId;
    uint16_t m_dataRecordLen;
    uint32_t m_pointRecordsCount;
    std::vector<uint32_t> m_pointRecordsByReturn;
    PointScales m_scales;
    PointOffsets m_offsets;
    PointExtents m_extents;
    std::vector<LASVLR> m_vlrs;
    std::string m_proj4;
};

} // namespace liblas

#endif // LIBLAS_LASHEADER_HPP_INCLUDED
