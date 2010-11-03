/******************************************************************************
 * $Id$
 *
 * Project:  libLAS - http://liblas.org - A BSD library for LAS format data.
 * Purpose:  LAS transform class 
 * Author:   Howard Butler, hobu.inc@gmail.com
 *
 ******************************************************************************
 * Copyright (c) 2010, Howard Butler
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

#ifndef LIBLAS_LASTRANSFORM_HPP_INCLUDED
#define LIBLAS_LASTRANSFORM_HPP_INCLUDED

#include <liblas/lasversion.hpp>
#include <liblas/laspoint.hpp>
#include <liblas/lasspatialreference.hpp>
// boost
#include <boost/shared_ptr.hpp>
// std
#include <vector>

namespace liblas {

#ifndef HAVE_GDAL
    typedef struct OGRCoordinateTransformationHS *OGRCoordinateTransformationH;
    typedef struct OGRSpatialReferenceHS *OGRSpatialReferenceH;
#endif


/// Defines public interface to LAS transform implementation.
class TransformI
{
public:
    
    virtual bool transform(Point& point) = 0;
    virtual ~TransformI() {};
};

typedef boost::shared_ptr<liblas::TransformI> TransformPtr;

class ReprojectionTransform: public TransformI
{
public:
    
    ReprojectionTransform(const SpatialReference& inSRS, const SpatialReference& outSRS);    
    ~ReprojectionTransform();

    bool transform(Point& point);

private:

    // FIXME: use shared_ptr with custom deleter and get rid of bloat of OGR manual calls --mloskot
    OGRCoordinateTransformationH m_transform;
    OGRSpatialReferenceH m_in_ref;
    OGRSpatialReferenceH m_out_ref;

    ReprojectionTransform(ReprojectionTransform const& other);
    ReprojectionTransform& operator=(ReprojectionTransform const& rhs);
};

} // namespace liblas

#endif // ndef LIBLAS_LASTRANSFORM_HPP_INCLUDED
