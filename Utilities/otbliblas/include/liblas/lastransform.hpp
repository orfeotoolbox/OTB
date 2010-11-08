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
#include <liblas/export.hpp>
// boost
#include <boost/shared_ptr.hpp>
// std
#include <vector>
#include <string>

namespace liblas {

#ifndef HAVE_GDAL
    typedef struct OGRCoordinateTransformationHS *OGRCoordinateTransformationH;
    typedef struct OGRSpatialReferenceHS *OGRSpatialReferenceH;
#endif


/// Defines public interface to LAS transform implementation.
class LAS_DLL TransformI
{
public:
    
    virtual bool transform(Point& point) = 0;
    virtual ~TransformI() {}
};

typedef boost::shared_ptr<liblas::TransformI> TransformPtr;

class LAS_DLL ReprojectionTransform: public TransformI
{
public:
    
    ReprojectionTransform(const SpatialReference& inSRS, const SpatialReference& outSRS);    
    ReprojectionTransform(const SpatialReference& inSRS, const SpatialReference& outSRS, liblas::HeaderPtr new_header);    
    ~ReprojectionTransform();

    bool transform(Point& point);

private:

#ifdef HAVE_GDAL
    struct OGRSpatialReferenceDeleter
    {
       template <typename T>
       void operator()(T* ptr)
       {
           ::OSRDestroySpatialReference(ptr);
       }
    };

    struct OSRTransformDeleter
    {
       template <typename T>
       void operator()(T* ptr)
       {
           ::OCTDestroyCoordinateTransformation(ptr);
       }
    };

#endif

    liblas::HeaderPtr m_new_header;
    
    typedef boost::shared_ptr<void> ReferencePtr;
    typedef boost::shared_ptr<void> TransformPtr;
    ReferencePtr m_in_ref_ptr;
    ReferencePtr m_out_ref_ptr;
    TransformPtr m_transform_ptr;
    


    ReprojectionTransform(ReprojectionTransform const& other);
    ReprojectionTransform& operator=(ReprojectionTransform const& rhs);
    
    void Initialize(SpatialReference const& inSRS, SpatialReference const& outSRS);
};

class LAS_DLL TranslationTransform: public TransformI
{
public:
    
    TranslationTransform(std::string const& expression);
    ~TranslationTransform();

    bool transform(Point& point);
    
    enum OPER_TYPE
    {
        eOPER_MULTIPLY = 0, 
        eOPER_DIVIDE = 1, 
        eOPER_SUBTRACT = 2,  
        eOPER_ADD = 3,
        eOPER_NONE = -99
    };

    // Yes, Mateusz, I'm embarassed by this :)
    struct operation{
        OPER_TYPE oper;
        std::string dimension;
        double value;
        std::string expression;
        
        operation(std::string name) : oper(eOPER_NONE), dimension(name), value(0.0)
        {
        }
    };

private:

    TranslationTransform(TranslationTransform const& other);
    TranslationTransform& operator=(TranslationTransform const& rhs);
    
    operation GetOperation(std::string const& expression);
    
    std::vector<operation> operations;
    
    std::string m_expression;
};
} // namespace liblas

#endif // ndef LIBLAS_LASTRANSFORM_HPP_INCLUDED
