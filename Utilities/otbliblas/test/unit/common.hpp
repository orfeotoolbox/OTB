// $Id$
//
// (C) Copyright Mateusz Loskot 2008, mateusz@loskot.net
// Distributed under the BSD License
// (See accompanying file LICENSE.txt or copy at
// http://www.opensource.org/licenses/bsd-license.php)
//
#include <liblas/liblas.hpp>
#include <liblas/laspoint.hpp>
#include <liblas/lasheader.hpp>

#include <iostream>

namespace tut
{

// Predicate testing LASPoint against given XY coordinates
// and tolerance.
struct is_xy
{
    is_xy(double x, double y, double tolerance)
        : x(x), y(y), t(tolerance)
    {}

    bool operator()(liblas::Point const& p)
    {
        double const dx = x - p.GetX();
        double const dy = y - p.GetY();

        return ((dx <= t && dx >= -t) && (dy <= t && dy >= -t));
    }

    double x;
    double y;
    double t;
};

// Functor to calculate bounding box of a set of points
struct bbox_calculator
{
    bbox_calculator(liblas::Bounds<double>& bbox) : bbox(&bbox), empty(true) {}

    void operator()(liblas::Point const& p)
    {
        if (0 == bbox)
            throw std::invalid_argument("bbox is null");

        // Box initialization during first iteration only
        if (empty)
        {
            bbox->min(0, p.GetX());
            bbox->max(0, p.GetX());
            bbox->min(1, p.GetY());
            bbox->max(1, p.GetY());
            bbox->min(2, p.GetZ());
            bbox->max(2, p.GetZ());
            empty = false;
        }

        // Expand bounding box to include given point
        bbox->min(0, std::min(bbox->min(0), p.GetX()));
        bbox->min(1, std::min(bbox->min(1), p.GetY()));
        bbox->min(2, std::min(bbox->min(2), p.GetZ()));
        bbox->max(0, std::max(bbox->max(0), p.GetX()));
        bbox->max(1, std::max(bbox->max(1), p.GetY()));
        bbox->max(2, std::max(bbox->max(2), p.GetZ()));
    }

private:
    liblas::Bounds<double>* bbox;
    bool empty;
};

// Common test procedure for default constructed point data.
void test_default_point(liblas::Point const& p);

// Common test procedure for default constructed header data.
void test_default_header(liblas::Header const& h);

// Test of header data in trunk/test/data/TO_core_last_clip.las file
void test_file10_header(liblas::Header const& h);

// Test of 1st point record in trunk/test/data/TO_core_last_clip.las file
void test_file10_point1(liblas::Point const& p);

// Test of 2nd point record in trunk/test/data/TO_core_last_clip.las file
void test_file10_point2(liblas::Point const& p);

// Test of 4th point record in trunk/test/data/TO_core_last_clip.las file
void test_file10_point4(liblas::Point const& p);

} // namespace tut

