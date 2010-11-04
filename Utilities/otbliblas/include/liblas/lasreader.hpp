/******************************************************************************
* $Id$
*
* Project:  libLAS - http://liblas.org - A BSD library for LAS format data.
* Purpose:  LAS reader class 
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

#ifndef LIBLAS_LASREADER_HPP_INCLUDED
#define LIBLAS_LASREADER_HPP_INCLUDED

#include <liblas/lasheader.hpp>
#include <liblas/laspoint.hpp>
#include <liblas/lasvariablerecord.hpp>
#include <liblas/lasspatialreference.hpp>
#include <liblas/lastransform.hpp>
#include <liblas/lasfilter.hpp>
#include <liblas/external/property_tree/ptree.hpp>
// boost
#include <boost/cstdint.hpp>
// std
#include <cstddef>
#include <iosfwd>
#include <memory>
#include <string>
#include <vector>

namespace liblas {


/// Defines public interface to LAS reader implementation.
class Reader
{
public:

    /// Consructor initializes reader with input stream as source of LAS records.
    /// @param ifs - stream used as source of LAS records.
    /// @exception std::runtime_error - on failure state of the input stream.
    Reader(std::istream& ifs);
    Reader(std::istream& ifs, boost::uint32_t cache_size);
    Reader(std::istream& ifs, boost::uint32_t cache_size, Header& header);
    Reader(ReaderI* reader);
    
    
    /// User-defined consructor initializes reader with input stream and
    /// a header to override the values in the file
    /// @exception std::runtime_error - on failure state of the input stream.
    Reader(std::istream& ifs, Header& header);
    
    /// Destructor.
    /// @exception nothrow
    ~Reader();
    
    /// Provides read-only access to header of LAS file being read.
    /// @exception nothrow
    Header const& GetHeader() const;

    /// Provides read-only access to current point record.
    /// @exception nothrow
    Point const& GetPoint() const;

    /// Provides read-only access to collection of variable-length records.
    /// @exception nothrow
    std::vector<VariableRecord> const& GetVLRs() const;

    /// Allow fetching of the stream attached to the reader.
    /// @exception nothrow
    std::istream& GetStream() const;

    /// Checks if end-of-file has been reached.
    bool IsEOF() const;

    /// Fetches next point record in file.
    /// @exception may throw std::exception
    bool ReadNextPoint();

    /// Fetches n-th point record from file.
    /// @exception may throw std::exception
    bool ReadPointAt(std::size_t n);

    /// Reinitializes state of the reader.
    /// @exception may throw std::exception
    void Reset();

    /// Move to the specified point to start 
    /// ReadNextPoint operations
    /// @exception may throw std::exception
    bool seek(std::size_t n);
    
    /// Reproject data as they are written if the Reader's reference is
    /// different than the Header's.
    /// @exception may throw std::exception
    bool SetSRS(const SpatialReference& ref);
    
    /// Override the spatial reference of the Reader's Header for 
    /// writing purposes.
    /// @exception may throw std::exception
    bool SetInputSRS(const SpatialReference& ref);

    /// Override the spatial reference of the Reader's Header for 
    /// writing purposes.
    /// @exception may throw std::exception
    bool SetOutputSRS(const SpatialReference& ref);

    /// Provides index-based access to point records.
    /// The operator is implemented in terms of ReadPointAt method
    /// and is not const-qualified because it updates file stream position.
    /// @exception may throw std::exception
    Point const& operator[](std::size_t n);
    
    /// Sets filters that are used to determine whether or not to 
    /// keep a point that was read from the file.  Filters have *no* 
    /// effect for reading data at specific locations in the file.  
    /// They only affect reading ReadNextPoint-style operations
    /// Filters are applied *before* transforms.
    void SetFilters(std::vector<liblas::FilterPtr> const& filters) {m_filters = filters;}

    /// Sets transforms to apply to points.  Points are transformed in 
    /// place *in the order* of the transform list.
    /// Filters are applied *before* transforms.  If an input/output SRS 
    /// is set on the reader, the reprojection transform will happen *first* 
    /// before any other transforms are applied.  This transform is a 
    /// special case.  You can define your own reprojection transforms and add 
    /// it to the list, but be sure to not issue a SetOutputSRS to trigger 
    /// the internal transform creation
    void SetTransforms(std::vector<liblas::TransformPtr> const& transforms) {m_transforms = transforms;}

    /// Summarize the file represented by the reader in the form of a 
    /// property_tree.  
    liblas::property_tree::ptree Summarize();
private:

    // Blocked copying operations, declared but not defined.
    Reader(Reader const& other);
    Reader& operator=(Reader const& rhs);

    void Init(); // throws on error
    bool KeepPoint(liblas::Point const& p);

    const std::auto_ptr<ReaderI> m_pimpl;

    HeaderPtr m_header;
    Point* m_point;
    PointPtr m_empty_point;
    
    
    // Set if the user provides a header to override the header as 
    // read from the istream
    bool bCustomHeader;
    
    std::vector<liblas::FilterPtr> m_filters;
    std::vector<liblas::TransformPtr> m_transforms;

    TransformPtr m_reprojection_transform;

    SpatialReference m_out_srs;
    SpatialReference m_in_srs;
    
};



} // namespace liblas

#endif // ndef LIBLAS_LASREADER_HPP_INCLUDED
