/******************************************************************************
 * $Id$
 *
 * Project:  libLAS - http://liblas.org - A BSD library for LAS format data.
 * Purpose:  LAS utility classes
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

#include <liblas/utility.hpp>
// boost
#include <boost/cstdint.hpp>
// std
#include <vector>

using namespace boost;

namespace liblas { 

Summary::Summary() :
    synthetic(0),
    withheld(0),
    keypoint(0),
    count(0),
    first(true),
    bHaveHeader(false)
    
{
    classes.assign(0);
    points_by_return.assign(0);
    returns_of_given_pulse.assign(0);    
}

Summary::Summary(Summary const& other)
    : classes(other.classes)
    , synthetic(other.synthetic)
    , withheld(other.withheld)
    , keypoint(other.keypoint)
    , count(other.count)
    , points_by_return(other.points_by_return)
    , returns_of_given_pulse(other.returns_of_given_pulse)
    , first(other.first)
    , min(other.min)
    , max(other.max)
    , m_header(other.m_header)
    , bHaveHeader(other.bHaveHeader)
{
}

Summary& Summary::operator=(Summary const& rhs)
{
    if (&rhs != this)
    {
        classes = rhs.classes;
        synthetic = rhs.synthetic;
        withheld = rhs.withheld;
        keypoint = rhs.keypoint;
        count = rhs.count;
        first = rhs.first;
        points_by_return = rhs.points_by_return;
        returns_of_given_pulse = rhs.returns_of_given_pulse;
        min = rhs.min;
        max = rhs.max;
        m_header = rhs.m_header;
        bHaveHeader = rhs.bHaveHeader;
    }
    return *this;
}

void Summary::AddPoint(liblas::Point const& p)
{
        count++;

        if (first) {
            min = p;
            max = p;
            
            // We only summarize the base dimensions 
            // but we want to be able to read/set them all.  The 
            // point copy here would set the header ptr of min/max
            // to be whatever might have come off of the file, 
            // and this may/may not have space for time/color
            min.SetHeaderPtr(HeaderPtr());
            max.SetHeaderPtr(HeaderPtr());
            first = false;
        }
        
        min.SetX(std::min(p.GetX(), min.GetX()));
        max.SetX(std::max(p.GetX(), max.GetX()));

        min.SetY(std::min(p.GetY(), min.GetY()));
        max.SetY(std::max(p.GetY(), max.GetY()));        

        min.SetZ(std::min(p.GetZ(), min.GetZ()));
        max.SetZ(std::max(p.GetZ(), max.GetZ()));

        min.SetIntensity(std::min(p.GetIntensity(), min.GetIntensity()));
        max.SetIntensity(std::max(p.GetIntensity(), max.GetIntensity()));

        min.SetTime(std::min(p.GetTime(), min.GetTime()));
        max.SetTime(std::max(p.GetTime(), max.GetTime()));

        min.SetReturnNumber(std::min(p.GetReturnNumber(), min.GetReturnNumber()));
        max.SetReturnNumber(std::max(p.GetReturnNumber(), max.GetReturnNumber()));

        min.SetNumberOfReturns(std::min(p.GetNumberOfReturns(), min.GetNumberOfReturns()));
        max.SetNumberOfReturns(std::max(p.GetNumberOfReturns(), max.GetNumberOfReturns()));

        min.SetScanDirection(std::min(p.GetScanDirection(), min.GetScanDirection()));
        max.SetScanDirection(std::max(p.GetScanDirection(), max.GetScanDirection()));

        min.SetFlightLineEdge(std::min(p.GetFlightLineEdge(), min.GetFlightLineEdge()));
        max.SetFlightLineEdge(std::max(p.GetFlightLineEdge(), max.GetFlightLineEdge()));

        min.SetScanAngleRank(std::min(p.GetScanAngleRank(), min.GetScanAngleRank()));
        max.SetScanAngleRank(std::max(p.GetScanAngleRank(), max.GetScanAngleRank()));

        min.SetUserData(std::min(p.GetUserData(), min.GetUserData()));
        max.SetUserData(std::max(p.GetUserData(), max.GetUserData()));

        min.SetPointSourceID(std::min(p.GetPointSourceID(), min.GetPointSourceID()));
        max.SetPointSourceID(std::max(p.GetPointSourceID(), max.GetPointSourceID()));
        
        liblas::Classification const& cls = p.GetClassification();
        
        boost::uint8_t minc = std::min(cls.GetClass(), min.GetClassification().GetClass());
        boost::uint8_t maxc = std::max(cls.GetClass(), max.GetClassification().GetClass());
        
        classes[cls.GetClass()]++;
        
        if (cls.IsWithheld()) withheld++;
        if (cls.IsKeyPoint()) keypoint++;
        if (cls.IsSynthetic()) synthetic++;
        
        min.SetClassification(liblas::Classification(minc));
        max.SetClassification(liblas::Classification(maxc));
        
        liblas::Color const& color = p.GetColor();
        
        liblas::Color::value_type red;
        liblas::Color::value_type green;
        liblas::Color::value_type blue;
        
        red = std::min(color.GetRed(), min.GetColor().GetRed());
        green = std::min(color.GetGreen(), min.GetColor().GetGreen());
        blue = std::min(color.GetBlue(), min.GetColor().GetBlue());
        
        min.SetColor(liblas::Color(red, green, blue));
        
        red = std::max(color.GetRed(), max.GetColor().GetRed());
        green = std::max(color.GetGreen(), max.GetColor().GetGreen());
        blue = std::max(color.GetBlue(), max.GetColor().GetBlue());        

        max.SetColor(liblas::Color(red, green, blue));

        points_by_return[p.GetReturnNumber()]++;
        returns_of_given_pulse[p.GetNumberOfReturns()]++;    
}

void Summary::SetHeader(liblas::Header const& h) 
{
    m_header = h;
    bHaveHeader = true;
}

ptree Summary::GetPTree() const
{
    ptree pt;
    
    ptree pmin = min.GetPTree();
    ptree pmax = max.GetPTree();
    
     
    pt.add_child("minimum", pmin);
    pt.add_child("maximum", pmax);
    
    ptree klasses;
    
    for (classes_type::size_type i=0; i < classes.size(); i++)
    {
        if (classes[i] != 0)
        {
            liblas::Classification c(i, false, false, false);
            std::string const& name = c.GetClassName();

            klasses.put("name", name);
            klasses.put("count", classes[i]);
            klasses.put("id", i);
            pt.add_child("classification.classification",klasses);            
        }
    }
    pt.put("encoding.withheld", withheld);
    pt.put("encoding.keypoint", keypoint);
    pt.put("encoding.synthetic", synthetic);
    
    ptree returns;
    bool have_returns = false;
    for (boost::array<boost::uint32_t,8>::size_type i=0; i < points_by_return.size(); i++) {
        if (i == 0) continue;

        if (points_by_return[i] != 0)
        {
            have_returns = true;
            returns.put("id", i);
            returns.put("count", points_by_return[i]);
            pt.add_child("points_by_return.return", returns);
            
        }
    }
    
    if (! have_returns) {
        // Assume all points are first return
        returns.put("id", 1);
        returns.put("count", count);
        pt.add_child("points_by_return.return", returns);        
    }
    
    ptree pulses;
    for (boost::array<boost::uint32_t,8>::size_type i=0; i < returns_of_given_pulse.size(); i++) {
        if (returns_of_given_pulse[i] != 0) {
            pulses.put("id",i);
            pulses.put("count", returns_of_given_pulse[i]);
            pt.add_child("returns_of_given_pulse.pulse", pulses);
        }
    }
    
    pt.put("count", count);

    liblas::property_tree::ptree top;
    if (bHaveHeader)
        top.add_child("summary.header",m_header.GetPTree());
    top.add_child("summary.points",pt);
    return top;
}

std::ostream& operator<<(std::ostream& os, liblas::Summary const& s)
{
    liblas::property_tree::ptree tree = s.GetPTree();
    
    os << "---------------------------------------------------------" << std::endl;
    os << "  Point Inspection Summary" << std::endl;
    os << "---------------------------------------------------------" << std::endl;

    os << "  Header Point Count: " << tree.get<std::string>("summary.header.count") << std::endl;
    os << "  Actual Point Count: " << tree.get<std::string>("summary.points.count") << std::endl;
    
    os << std::endl;
    os << "  Minimum and Maximum Attributes (min,max)" << std::endl;
    os << "---------------------------------------------------------" << std::endl;
    
    boost::uint32_t x_precision = 6;
    boost::uint32_t y_precision = 6;
    boost::uint32_t z_precision = 6;

    try {
        double frac = 0;
        double integer = 0;
        double x_scale = tree.get<double>("summary.header.scale.x");
        double y_scale = tree.get<double>("summary.header.scale.y");
        double z_scale = tree.get<double>("summary.header.scale.x");
        frac = std::modf(x_scale, &integer);
        x_precision = static_cast<boost::uint32_t>(std::fabs(std::floor(std::log10(frac))));
        frac = std::modf(y_scale, &integer);
        y_precision = static_cast<boost::uint32_t>(std::fabs(std::floor(std::log10(frac))));
        frac = std::modf(z_scale, &integer);
        z_precision = static_cast<boost::uint32_t>(std::fabs(std::floor(std::log10(frac))));
        

    }
    catch (liblas::property_tree::ptree_bad_path const& e) {
        ::boost::ignore_unused_variable_warning(e);
    }
    
    os << "  Min X, Y, Z: \t\t";
    os.setf(std::ios_base::fixed, std::ios_base::floatfield);
    os.precision(x_precision);
    os << tree.get<double>("summary.points.minimum.x") << ", ";
    os.precision(y_precision);
    os << tree.get<double>("summary.points.minimum.y") << ", ";
    os.precision(z_precision);
    os << tree.get<double>("summary.points.minimum.z") << ", ";

    os << std::endl;
    os << "  Max X, Y, Z: \t\t";
    os.setf(std::ios_base::fixed, std::ios_base::floatfield);
    os.precision(x_precision);
    os << tree.get<double>("summary.points.maximum.x") << ", ";
    os.precision(y_precision);
    os << tree.get<double>("summary.points.maximum.y") << ", ";
    os.precision(z_precision);
    os << tree.get<double>("summary.points.maximum.z") << ", ";
    
    os << std::endl;
    os << "  Bounding Box:\t\t";
    os << tree.get<double>("summary.points.minimum.x") << ", "
       << tree.get<double>("summary.points.minimum.y") << ", "
       << tree.get<double>("summary.points.maximum.x") << ", "
       << tree.get<double>("summary.points.maximum.y");
    
    os << std::endl;
    os.precision(6);
    os << "  Time:\t\t\t" << tree.get<double>("summary.points.minimum.time") << ", "
       << tree.get<double>("summary.points.maximum.time");


    os << std::endl;
    os.setf(std::ios::floatfield);
    os << "  Return Number:\t" << tree.get<boost::uint32_t>("summary.points.minimum.returnnumber") << ", "
       << tree.get<boost::uint32_t>("summary.points.maximum.returnnumber");

    os << std::endl;
    os << "  Return Count:\t\t" << tree.get<boost::uint32_t>("summary.points.minimum.numberofreturns") << ", "
       << tree.get<boost::uint32_t>("summary.points.maximum.numberofreturns");

    os << std::endl;
    os << "  Flightline Edge:\t" << tree.get<boost::uint32_t>("summary.points.minimum.flightlineedge") << ", "
       << tree.get<boost::uint32_t>("summary.points.maximum.flightlineedge");

    os << std::endl;
    os << "  Intensity:\t\t" << tree.get<boost::uint32_t>("summary.points.minimum.intensity") << ", "
       << tree.get<boost::uint32_t>("summary.points.maximum.intensity");

    os << std::endl;
    os << "  Scan Direction Flag:\t" << tree.get<boost::int32_t>("summary.points.minimum.scandirection") << ", "
       << tree.get<boost::int32_t>("summary.points.maximum.scandirection");

    os << std::endl;
    os << "  Scan Angle Rank:\t" << tree.get<boost::int32_t>("summary.points.minimum.scanangle") << ", "
       << tree.get<boost::int32_t>("summary.points.maximum.scanangle");

    os << std::endl;
    os << "  Classification:\t" << tree.get<boost::int32_t>("summary.points.minimum.classification.id") << ", "
       << tree.get<boost::int32_t>("summary.points.maximum.classification.id");

    os << std::endl;
    os << "  Point Source Id:\t" << tree.get<boost::uint32_t>("summary.points.minimum.pointsourceid") << ", "
       << tree.get<boost::uint32_t>("summary.points.maximum.pointsourceid");

    os << std::endl;
    os << "  User Data:\t\t" << tree.get<boost::uint32_t>("summary.points.minimum.userdata") << ", "
       << tree.get<boost::uint32_t>("summary.points.maximum.userdata");

    os << std::endl;
    os << "  Minimum Color:\t" 
       << tree.get<boost::uint32_t>("summary.points.minimum.color.red") << " "
       << tree.get<boost::uint32_t>("summary.points.minimum.color.green") << " "
       << tree.get<boost::uint32_t>("summary.points.minimum.color.blue") << " ";

    os << std::endl;
    os << "  Maximum Color:\t" 
       << tree.get<boost::uint32_t>("summary.points.maximum.color.red") << " "
       << tree.get<boost::uint32_t>("summary.points.maximum.color.green") << " "
       << tree.get<boost::uint32_t>("summary.points.maximum.color.blue") << " ";

    os << std::endl;
    os << std::endl;
    os << "  Number of Points by Return" << std::endl;
    os << "---------------------------------------------------------" << std::endl;
    
    BOOST_FOREACH(ptree::value_type &v,
            tree.get_child("summary.points.points_by_return"))
    {
        boost::uint32_t i = v.second.get<boost::uint32_t>("id");
        boost::uint32_t count = v.second.get<boost::uint32_t>("count");
        os << "\t(" << i << ") " << count;
    }
    
    os << std::endl;
    os << std::endl;
    os << "  Number of Returns by Pulse" << std::endl;
    os << "---------------------------------------------------------" << std::endl;
    
    BOOST_FOREACH(ptree::value_type &v,
            tree.get_child("summary.points.returns_of_given_pulse"))
    {
        boost::uint32_t i = v.second.get<boost::uint32_t>("id");
        boost::uint32_t count = v.second.get<boost::uint32_t>("count");
        os << "\t(" << i << ") " << count;
    }     

    os << std::endl;
    os << std::endl;
    os << "  Point Classifications" << std::endl;
    os << "---------------------------------------------------------" << std::endl;
    

    BOOST_FOREACH(ptree::value_type &v,
            tree.get_child("summary.points.classification"))
    {
        boost::uint32_t i = v.second.get<boost::uint32_t>("id");
        boost::uint32_t count = v.second.get<boost::uint32_t>("count");
        std::string name = v.second.get<std::string>("name");
        os << "\t" << count << " " << name << " (" << i << ") " << std::endl;;
    }

    os << "  -------------------------------------------------------" << std::endl;
    os << "  \t" << tree.get<boost::uint32_t>("summary.points.encoding.withheld") << " withheld" << std::endl;
    os << "  \t" << tree.get<boost::uint32_t>("summary.points.encoding.keypoint") << " keypoint" << std::endl;
    os << "  \t" << tree.get<boost::uint32_t>("summary.points.encoding.synthetic") << " synthetic" << std::endl;
    os << "  -------------------------------------------------------" << std::endl;

    return os;
    
}

} // namespace liblas
