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

#include <liblas/lasversion.hpp>
#include <liblas/laswriter.hpp>
#include <liblas/detail/writer/writer.hpp>

// std
#include <stdexcept>
#include <fstream>
#include <string>
#include <cstring> // std::memset
#include <cassert>

namespace liblas
{

Writer::Writer(std::ostream& ofs, Header const& header) :
    m_pimpl(detail::WriterFactory::Create(ofs)), m_header(HeaderPtr(new liblas::Header(header))), 
    m_filters(0),
    m_transforms(0),
    m_reprojection_transform(TransformPtr())
{
    liblas::Header const& h = m_pimpl->WriteHeader(*m_header);
    m_header = HeaderPtr(new liblas::Header(h));

    // Copy our input SRS.  If the user issues SetInputSRS, it will 
    // be overwritten
    m_in_srs = m_header->GetSRS();
}

Writer::~Writer()
{
    assert(0 != m_pimpl.get());

    m_pimpl->UpdateHeader(*m_header);
}

Header const& Writer::GetHeader() const
{
    return *m_header;
}

bool Writer::WritePoint(Point const& point)
{
    if (!point.IsValid())
    {
        return false;
    }

    std::vector<liblas::FilterI*>::const_iterator fi;
    std::vector<liblas::TransformI*>::const_iterator ti;
    bool bHaveTransforms = false;
    bool bHaveFilters = false;
    
    if (m_transforms != 0 ) {
        bHaveTransforms = true;
    }
    
    if (m_filters != 0 ) {
        bHaveFilters = true;
    }
    

    if (bHaveFilters) {
    if (m_filters->size() != 0) {
        // We have filters, filter this point.  All filters must 
        // return true for us to keep it.
        bool keep = false;
        for (fi = m_filters->begin(); fi != m_filters->end(); ++fi) {
            liblas::FilterI* filter = *fi;
            if (filter->filter(point)){
                // if ->filter() is true, we keep the point
                keep = true;
            } else {
                keep = false;
                break;
            }
            
        }
        if (!keep) {
            return false;
        } 
    }
    }
    
    if (bHaveTransforms) {
    if (m_transforms->size() != 0) {
    
        // Apply the transforms to each point
        Point p(point);
        for (ti = m_transforms->begin(); ti != m_transforms->end(); ++ti) {
            liblas::TransformI* transform = *ti;
            transform->transform(p);

        }
        
        // We have to write a copy of our point, because we're applying 
        // transformations that change the point.
        m_pimpl->WritePoint(p, m_header);
        return true;
        
    }
    }

    // if we haven't returned because of the filter and we don't have any 
    // transforms, just write the point
    m_pimpl->WritePoint(point, m_header);
    return true;
}

std::ostream& Writer::GetStream() const
{
    return m_pimpl->GetStream();
}

void Writer::WriteHeader(Header& header)
{
    // The writer may update our header as part of its 
    // writing process (change VLRs for SRS's, for instance).
    liblas::Header const& h = m_pimpl->WriteHeader(header);
    m_header = HeaderPtr(new liblas::Header(h));
}

bool Writer::SetSRS(const SpatialReference& srs)
{
    SetOutputSRS(srs);
    return true;
}

bool Writer::SetInputSRS(const SpatialReference& srs)
{
    m_in_srs = srs;
    return true;
}

bool Writer::SetOutputSRS(const SpatialReference& srs)
{
    m_out_srs = srs;

    // Check the very first transform and see if it is 
    // the reprojection transform.  If it is, we're going to 
    // nuke it and replace it with a new one
    
    // If there was nothing there, we're going to make a new reprojection
    // transform and put in on the transforms list (or make a new transforms
    // list if *that* isn't there).
    TransformI* possible_reprojection_transform = 0;
    
    if (m_transforms != 0) {
        if (m_transforms->size() > 0) {
            possible_reprojection_transform = m_transforms->at(0);
        }
    }
    
    if (m_reprojection_transform.get() == possible_reprojection_transform && m_reprojection_transform.get() != 0) {
        // remove it from the transforms list
        std::vector<TransformI*>::iterator i = m_transforms->begin();
        m_transforms->erase(i);
    }
    
    // overwrite our reprojection transform
    m_reprojection_transform = TransformPtr(new ReprojectionTransform(m_in_srs, m_out_srs));
    
    if (m_transforms != 0) {
        if (m_transforms->size() > 0) {
            // Insert the new reprojection transform to the beginning of the 
            // vector there are already transforms there.
            m_transforms->insert(m_transforms->begin(), m_reprojection_transform.get());
            
        } else {
            // List exists, but its size is 0
            m_transforms->push_back(m_reprojection_transform.get());
        }
    } else {
        // transforms don't exist yet, make a new one and put our 
        // reprojection transform on it.
        m_transforms = new std::vector<liblas::TransformI*>;
        m_transforms->push_back(m_reprojection_transform.get());
    }


    return true;
}
} // namespace liblas

