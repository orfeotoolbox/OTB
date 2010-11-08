/******************************************************************************
 * $Id$
 *
 * Project:  libLAS - http://liblas.org - A BSD library for LAS format data.
 * Purpose:  LAS writer class 
 * Author:   Mateusz Loskot, mateusz@loskot.net
 *
 ******************************************************************************
 * Copyright (c) 2008, Mateusz Loskot
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

#ifndef LIBLAS_LASWRITER_HPP_INCLUDED
#define LIBLAS_LASWRITER_HPP_INCLUDED

#include <liblas/lasversion.hpp>
#include <liblas/lasheader.hpp>
#include <liblas/laspoint.hpp>
#include <liblas/lastransform.hpp>
#include <liblas/lasfilter.hpp>
#include <liblas/export.hpp>
// boost
#include <boost/shared_ptr.hpp>
// std
#include <iosfwd> // std::ostream
#include <string>
#include <memory>
#include <cstdlib> // std::size_t

namespace liblas {

/// Defines public interface to LAS writer implementation.
/// This class 
class LAS_DLL Writer
{
public:

    /// Consructor initializes reader with specified output stream and header specification.
    /// @param ofs - stream used as destination for LAS records.
    /// @param header - specifies obligatory properties of LAS file.
    /// @exception std::runtime_error - on failure state of the input stream.
    Writer(std::ostream& ofs, Header const& header);

    /// Destructor does not close file attached to the output stream
    /// Header may be updated after writing operation completed, if necessary
    /// in order to maintain data consistency.
    ~Writer();
    
    /// Provides access to header structure.
    Header const& GetHeader() const;

    /// \todo TODO: Move point record composition deep into writer implementation.
    /// \todo TODO: How to handle point_source_id in portable way, for LAS 1.0 and 1.1
    bool WritePoint(Point const& point);

    /// Allow fetching of the stream
    std::ostream& GetStream() const;
    
    /// Allow in-place writing of header
    void WriteHeader(Header& header);

    /// Reproject data as they are written if the Writer's reference is
    /// different than the Header's
    bool SetSRS(const SpatialReference& ref);
    bool SetInputSRS(const SpatialReference& ref);
    bool SetOutputSRS(const SpatialReference& ref);

    /// Sets filters that are used to determine wither or not to 
    /// keep a point that before we write it
    /// Filters are applied *before* transforms.
    void SetFilters(std::vector<liblas::FilterI*>* filters) {m_filters = filters;}

    /// Sets transforms to apply to points.  Points are transformed in 
    /// place *in the order* of the transform list.
    /// Filters are applied *before* transforms.  If an input/output SRS 
    /// is set on the writer, the reprojection transform will happen *first* 
    /// before any other transforms are applied.  This transform is a 
    /// special case.  You can define your own reprojection transforms and add 
    /// it to the list, but be sure to not issue a SetOutputSRS to trigger 
    /// the internal transform creation
    void SetTransforms(std::vector<liblas::TransformI*>* transforms) {m_transforms = transforms;}
    
private:
    
    // Blocked copying operations, declared but not defined.
    Writer(Writer const& other);
    Writer& operator=(Writer const& rhs);

    const std::auto_ptr<WriterI> m_pimpl;

    HeaderPtr m_header;
    detail::PointRecord m_record;

    std::vector<liblas::FilterI*>* m_filters;
    std::vector<liblas::TransformI*>* m_transforms;
    
    TransformPtr m_reprojection_transform;
    
    SpatialReference m_out_srs;
    SpatialReference m_in_srs;
    
};

} // namespace liblas

#endif // ndef LIBLAS_LASWRITER_HPP_INCLUDED
