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

#include <liblas/lasversion.hpp>
#include <liblas/lasreader.hpp>
#include <liblas/detail/reader/reader.hpp>
#include <liblas/detail/reader/cachedreader.hpp>
#include <liblas/utility.hpp>

// boost
#include <boost/cstdint.hpp>

// std
#include <stdexcept>
#include <fstream>
#include <string>
#include <cstring> // std::memset
#include <cassert>
#include <iostream>

using namespace boost;

namespace liblas
{

Reader::Reader(std::istream& ifs) :
    m_pimpl(new detail::CachedReaderImpl(ifs,3)),
    m_header(HeaderPtr()),
    m_point(0),
    m_empty_point(new Point()),
    bCustomHeader(false),
    m_filters(0),
    m_transforms(0),
    m_reprojection_transform(TransformPtr())
{
    Init();
}

Reader::Reader(std::istream& ifs, uint32_t cache_size) :
    m_pimpl(new detail::CachedReaderImpl(ifs, cache_size)),
    m_header(HeaderPtr()),
    m_point(0),
    m_empty_point(new Point()),
    bCustomHeader(false),
    m_filters(0),
    m_transforms(0),
    m_reprojection_transform(TransformPtr())
{
    Init();
}

Reader::Reader(std::istream& ifs, uint32_t cache_size, Header& header) :
    m_pimpl(new detail::CachedReaderImpl(ifs, cache_size)),
    m_header(HeaderPtr( )),    
    m_point(0),
    m_empty_point(new Point()),
    bCustomHeader(true),
    m_filters(0),
    m_transforms(0),
    m_reprojection_transform(TransformPtr())
{
    // if we have a custom header, create a slot for it and then copy 
    // the header we were given
    m_header = HeaderPtr(new Header(header));
    Init();
}

Reader::Reader(ReaderI* reader) :
    m_pimpl(reader),
    m_header(HeaderPtr()),
    m_point(0),
    m_empty_point(new Point()),
    bCustomHeader(false),
    m_filters(0),
    m_transforms(0),
    m_reprojection_transform(TransformPtr())
{
    Init();
}

Reader::Reader(std::istream& ifs, Header& header) :
    m_pimpl(new detail::ReaderImpl(ifs)),
    m_header(HeaderPtr( )),    
    m_point(0),
    m_empty_point(new Point()),
    bCustomHeader(true),
    m_filters(0),
    m_transforms(0),
    m_reprojection_transform(TransformPtr())
{
    // if we have a custom header, create a slot for it and then copy 
    // the header we were given
    m_header = HeaderPtr(new Header(header));
    Init();
}

Reader::~Reader()
{
    // empty, but required so we can implement PIMPL using
    // std::auto_ptr with incomplete type (Reader).
    // delete m_empty_point;
    
    // if (m_header != 0) {
    //     delete m_header;
    // }
}

Header const& Reader::GetHeader() const
{
    return *m_header;
}

Point const& Reader::GetPoint() const
{
    return *m_point;
}

bool Reader::KeepPoint(liblas::Point const& p)
{
    std::vector<liblas::FilterPtr>::const_iterator fi;
    
    // If there's no filters on this reader, we keep 
    // the point no matter what.
    if (m_filters.empty() ) {
        return true;
    }

    for (fi = m_filters.begin(); fi != m_filters.end(); ++fi) {
        liblas::FilterPtr filter = *fi;
        if (!filter->filter(p)){
            return false;
        }
    }
    return true;
}

bool Reader::ReadNextPoint()
{
    std::vector<liblas::TransformPtr>::const_iterator ti;
  
    try {
        // m_point = m_pimpl->ReadNextPoint(m_header).get();
        m_point = const_cast<Point*>(&(m_pimpl->ReadNextPoint(m_header)));
        
        // Filter the points and continue reading until we either find 
        // one to keep or throw an exception.
        if (!KeepPoint(*m_point)) {
            m_point = const_cast<Point*>(&(m_pimpl->ReadNextPoint(m_header)));
            while (!KeepPoint(*m_point)) {
                m_point = const_cast<Point*>(&(m_pimpl->ReadNextPoint(m_header)));
            }
        }
        
        if (!m_transforms.empty()) {
            // Apply the transforms to each point

            for (ti = m_transforms.begin(); ti != m_transforms.end(); ++ti) {
                liblas::TransformPtr transform = *ti;
                transform->transform(*m_point);
            }            
        }

        return true;
    } catch (std::out_of_range) {
        m_point = 0;
        return false;
    }
}

bool Reader::ReadPointAt(std::size_t n)
{
    std::vector<liblas::TransformPtr>::const_iterator ti;

    if (m_header->GetPointRecordsCount() <= n)
    {
        throw std::out_of_range("point subscript out of range");
    }
    
    try {
        m_point = const_cast<Point*>(&(m_pimpl->ReadPointAt(n, m_header)));
        if (!m_transforms.empty()) {
            for (ti = m_transforms.begin(); ti != m_transforms.end(); ++ti) {
                liblas::TransformPtr transform = *ti;
                transform->transform(*m_point);
            }            
            }
        return true;
    } catch (std::out_of_range) {
        m_point = 0;
        return false;
    }
}

bool Reader::seek(std::size_t n)
{
    try {
        m_pimpl->Seek(n, m_header);
        return true;
    } catch (std::out_of_range) {
        m_point = 0;
        return false;
    }

}
Point const& Reader::operator[](std::size_t n)
{
    if (m_header->GetPointRecordsCount() <= n)
    {
        throw std::out_of_range("point subscript out of range");
    }
    
    ReadPointAt(n);

    if (m_point == 0) 
    {
        throw std::out_of_range("no point record at given position");
    }

    return *m_point;
}

void Reader::Init()
{   
    // Copy our existing header in case we have already set a custom 
    // one.  We will use this instead of the one from the stream if 
    // the constructor with the header was used.
    
    Header custom_header;
    if (m_header != 0) 
    {
        custom_header = *m_header;
    }

    m_header = m_pimpl->ReadHeader();

        // throw std::runtime_error("public header block reading failure");

    m_pimpl->Reset(m_header);
    
    if (bCustomHeader) {
        custom_header.SetDataOffset(m_header->GetDataOffset());
        *m_header = custom_header;
    }
    
    // This is yucky, but we need to ensure that we have a reference 
    // to a real point existing as soon as we are constructed.  
    // This is for someone who tries to GetPoint without first reading 
    // a point, and it will ensure they get something valid.  We just 
    // keep it around until the reader closes down and then deletes.  
    // 
    m_point = m_empty_point.get();
    
    // Copy our input SRS.  If the user issues SetInputSRS, it will 
    // be overwritten
    m_in_srs = m_header->GetSRS();
}

std::istream& Reader::GetStream() const
{
    return m_pimpl->GetStream();
}

void Reader::Reset() 
{
    Init();
}

bool Reader::IsEOF() const
{
    return m_pimpl->GetStream().eof();
}

bool Reader::SetSRS(const SpatialReference& srs)
{
    return SetOutputSRS(srs);
}

bool Reader::SetInputSRS(const SpatialReference& srs)
{
    m_in_srs = srs;
    return true;
}

bool Reader::SetOutputSRS(const SpatialReference& srs)
{
    m_out_srs = srs;
    m_pimpl->Reset(m_header);

    // Check the very first transform and see if it is 
    // the reprojection transform.  If it is, we're going to 
    // nuke it and replace it with a new one
    
    // If there was nothing there, we're going to make a new reprojection
    // transform and put in on the transforms list (or make a new transforms
    // list if *that* isn't there).
    liblas::TransformPtr possible_reprojection_transform;
    
   if (!m_transforms.empty()) {
        possible_reprojection_transform = m_transforms.at(0);
    }
    
    if (m_reprojection_transform == possible_reprojection_transform && m_reprojection_transform.get() != 0) {
        // remove it from the transforms list
        std::vector<liblas::TransformPtr>::iterator i = m_transforms.begin();
        m_transforms.erase(i);
    }
    
    // overwrite our reprojection transform
    m_reprojection_transform = TransformPtr(new ReprojectionTransform(m_in_srs, m_out_srs));
    
    if (!m_transforms.empty()) {
        // Insert the new reprojection transform to the beginning of the 
        // vector there are already transforms there.
        m_transforms.insert(m_transforms.begin(), m_reprojection_transform);
        
    } else {
        // List exists, but its size is 0
        m_transforms.push_back(m_reprojection_transform);
    } 
    return true;
}

liblas::property_tree::ptree Reader::Summarize() 
{

    
    liblas::Summary s;

    Reset();
    bool read = ReadNextPoint();
    if (!read)
    {
        throw std::runtime_error("Unable to read any points from file.");
    }
        
    while (read) 
    {
        liblas::Point const& p = GetPoint();
        s.AddPoint(p);
        read = ReadNextPoint();
    }
    
    return s.GetPTree();
    
    // Summarize the schema
    // liblas::Schema schema = m_header->GetSchema();
    
    // // if both min == max *and* min is 0, we're declaring this 
    // // dimension inactive.
    // if (detail::compare_distance(max.GetX(), min.GetX() ) && detail::compare_distance(0, min.GetX()))
    // {
    //     DimensionPtr d = schema.GetDimension("X");
    //     d->IsActive(false);
    // }
    // 
    // // if both min == max *and* min is 0, we're declaring this 
    // // dimension inactive.
    // if (detail::compare_distance(max.GetY(), min.GetY() ) && detail::compare_distance(0, min.GetY()))
    // {
    //     DimensionPtr d = schema.GetDimension("Y");
    //     d->IsActive(false);
    // }
    // 
    // // if both min == max *and* min is 0, we're declaring this 
    // // dimension inactive.
    // if (detail::compare_distance(max.GetZ(), min.GetZ() ) && detail::compare_distance(0, min.GetZ()))
    // {
    //     DimensionPtr d = schema.GetDimension("Z");
    //     d->IsActive(false);
    // }


}
} // namespace liblas

