/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "otbVectorDataStyle.h"
#include <mapnik/filter_factory.hpp>
#include <mapnik/rule.hpp>
#include <mapnik/text_symbolizer.hpp>
#ifndef OTB_MAPNIK_COMPATIBILITY_API07
#include <mapnik/feature_type_style.hpp>
#endif

namespace otb
{
namespace mapnik_otb
{
// this should be removed once mapnik support for version < 2.0 is dropped.
// should be around 01/2013.
#ifdef OTB_MAPNIK_COMPATIBILITY_API07
typedef mapnik::rule_type rule;
static mapnik::filter_ptr parse_expression(std::string const& wkt)
{
  return mapnik::create_filter(wkt);
}
static shared_ptr<mapnik::text_symbolizer> create_text_symbolizer(
  mapnik::filter_ptr filter,
  std::string const& face_name,
  unsigned size,
  mapnik::color const& fill)
{
  shared_ptr<mapnik::text_symbolizer> p(
    new mapnik::text_symbolizer(
      "name", face_name, size, fill));
  return p;
}

#else
typedef mapnik::rule rule;
static mapnik::expression_ptr parse_expression(std::string const& wkt)
{
  return mapnik::parse_expression(wkt);
}
static boost::shared_ptr<mapnik::text_symbolizer> create_text_symbolizer(
  mapnik::expression_ptr filter,
  std::string const& face_name,
  unsigned size,
  mapnik::color const& fill)
{
  boost::shared_ptr<mapnik::text_symbolizer> p(
    new mapnik::text_symbolizer(
      filter, face_name, size, fill));
  return p;
}

#endif
}

/**
* PrintSelf Method
 */
void
VectorDataStyle
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

void
VectorDataStyle
::LoadBinaryRasterizationStyle(mapnik::Map& mapnikMap) const
{
  {
  mapnik::feature_type_style style;
  {
    mapnik_otb::rule rule;
    rule.set_filter(mapnik_otb::parse_expression("[geometry] = 'line'"));
    {
     mapnik::line_symbolizer geom = mapnik::line_symbolizer();
     mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#000000"), 1);
     stroke.set_line_join(mapnik::ROUND_JOIN);
     stroke.set_line_cap(mapnik::ROUND_CAP);
     geom.set_stroke(stroke);
     rule.append(geom);
    }
    style.add_rule(rule);
   }
   {
    mapnik_otb::rule rule;
    rule.set_filter(mapnik_otb::parse_expression("[geometry] = 'polygon'"));
     {
      mapnik::polygon_symbolizer geom = mapnik::polygon_symbolizer();
      geom.set_fill(mapnik::color("#000000"));
      geom.set_gamma(0.0);
      rule.append(geom);
     }
     style.add_rule(rule);
   }
   {
    mapnik_otb::rule rule;
    rule.set_filter(mapnik_otb::parse_expression("[geometry] = 'point'"));
    {
     mapnik::point_symbolizer geom = mapnik::point_symbolizer();
     rule.append(geom);
    }
    style.add_rule(rule);
   }
  mapnikMap.insert_style("binary-rasterization", style);
  }
}

void
VectorDataStyle
::LoadOSMStyle(mapnik::Map& mapnikMap) const
{
    {
    mapnik::feature_type_style style;
      {
      mapnik_otb::rule rule;
      rule.set_max_scale(static_cast<unsigned long long>(250000000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(600000ULL / m_ScaleFactor));
        {
        mapnik::polygon_symbolizer geom = mapnik::polygon_symbolizer();
        geom.set_fill(mapnik::color("#f2efe9"));
        rule.append(geom);
        }
      style.add_rule(rule);
      }
    mapnikMap.insert_style("world", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[place] = 'city' and [capital]='yes'"));
      rule.set_max_scale(static_cast<unsigned long long>(50000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(10000000ULL / m_ScaleFactor));
      boost::shared_ptr<mapnik::text_symbolizer> textSymb0 = mapnik_otb::create_text_symbolizer(mapnik_otb::parse_expression("[name]"),
                                        "DejaVu Sans Book", 9, mapnik::color("#000"));
      textSymb0->set_halo_radius(1);
      textSymb0->set_wrap_width(0);
      textSymb0->set_displacement(0.0, 0);
      rule.append(*textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[place] = 'city' and [capital]='yes'"));
      rule.set_max_scale(static_cast<unsigned long long>(10000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(2000000ULL / m_ScaleFactor));
      boost::shared_ptr<mapnik::text_symbolizer> textSymb0 = mapnik_otb::create_text_symbolizer(mapnik_otb::parse_expression("[name]"),
                                        "DejaVu Sans Book", 11, mapnik::color("#000"));
      textSymb0->set_halo_radius(1);
      textSymb0->set_wrap_width(0);
      textSymb0->set_displacement(0.0, 0);
      rule.append(*textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[place] = 'city' and not [capital]='yes'"));
      rule.set_max_scale(static_cast<unsigned long long>(10000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(2000000ULL / m_ScaleFactor));
      boost::shared_ptr<mapnik::text_symbolizer> textSymb0 = mapnik_otb::create_text_symbolizer(mapnik_otb::parse_expression("[name]"),
                                        "DejaVu Sans Book", 8, mapnik::color("#000"));
      textSymb0->set_halo_radius(1);
      textSymb0->set_wrap_width(0);
      textSymb0->set_displacement(0.0, 0);
      rule.append(*textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[place] = 'city'"));
      rule.set_max_scale(static_cast<unsigned long long>(2000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(500000ULL / m_ScaleFactor));
      boost::shared_ptr<mapnik::text_symbolizer> textSymb0 = mapnik_otb::create_text_symbolizer(mapnik_otb::parse_expression("[name]"),
                                        "DejaVu Sans Book", 11, mapnik::color("#000"));
      textSymb0->set_halo_radius(1);
      textSymb0->set_wrap_width(0);
      textSymb0->set_displacement(0.0, 0);
      rule.append(*textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[place] = 'city'"));
      rule.set_max_scale(static_cast<unsigned long long>(500000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      boost::shared_ptr<mapnik::text_symbolizer> textSymb0 = mapnik_otb::create_text_symbolizer(mapnik_otb::parse_expression("[name]"),
                                        "DejaVu Sans Book", 14, mapnik::color("#000"));
      textSymb0->set_halo_radius(1);
      textSymb0->set_wrap_width(0);
      textSymb0->set_displacement(0.0, 0);
      rule.append(*textSymb0);
      style.add_rule(rule);
      }
    mapnikMap.insert_style("city", style);
    }
    {
    mapnik::feature_type_style style;
    {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'motorway' and [length] = 1"));
      rule.set_max_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'motorway' and [length] = 2"));
      rule.set_max_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'motorway' and [length] = 3"));
      rule.set_max_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'motorway' and [length] = 4"));
      rule.set_max_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'motorway' and [length] = 5"));
      rule.set_max_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'motorway' and [length] = 6"));
      rule.set_max_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'motorway' and [length] = 7"));
      rule.set_max_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'motorway' and [length] = 8"));
      rule.set_max_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'trunk' and [length] = 1"));
      rule.set_max_scale(static_cast<unsigned long long>(500000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'trunk' and [length] = 2"));
      rule.set_max_scale(static_cast<unsigned long long>(500000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'trunk' and [length] = 3"));
      rule.set_max_scale(static_cast<unsigned long long>(500000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'trunk' and [length] = 4"));
      rule.set_max_scale(static_cast<unsigned long long>(500000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'trunk' and [length] = 5"));
      rule.set_max_scale(static_cast<unsigned long long>(500000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'trunk' and [length] = 6"));
      rule.set_max_scale(static_cast<unsigned long long>(500000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'trunk' and [length] = 7"));
      rule.set_max_scale(static_cast<unsigned long long>(500000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'trunk' and [length] = 8"));
      rule.set_max_scale(static_cast<unsigned long long>(500000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'primary' and [length] = 1"));
      rule.set_max_scale(static_cast<unsigned long long>(200000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'primary' and [length] = 2"));
      rule.set_max_scale(static_cast<unsigned long long>(200000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'primary' and [length] = 3"));
      rule.set_max_scale(static_cast<unsigned long long>(200000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'primary' and [length] = 4"));
      rule.set_max_scale(static_cast<unsigned long long>(200000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'primary' and [length] = 5"));
      rule.set_max_scale(static_cast<unsigned long long>(200000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'primary' and [length] = 6"));
      rule.set_max_scale(static_cast<unsigned long long>(200000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'primary' and [length] = 7"));
      rule.set_max_scale(static_cast<unsigned long long>(200000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'primary' and [length] = 8"));
      rule.set_max_scale(static_cast<unsigned long long>(200000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'secondary' and [length] = 1"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'secondary' and [length] = 2"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'secondary' and [length] = 3"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'secondary' and [length] = 4"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'secondary' and [length] = 5"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'secondary' and [length] = 6"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'secondary' and [length] = 7"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'secondary' and [length] = 8"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'tertiary' and [length] = 1"));
      rule.set_max_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'tertiary' and [length] = 2"));
      rule.set_max_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'tertiary' and [length] = 3"));
      rule.set_max_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'tertiary' and [length] = 4"));
      rule.set_max_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'tertiary' and [length] = 5"));
      rule.set_max_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'tertiary' and [length] = 6"));
      rule.set_max_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'tertiary' and [length] = 7"));
      rule.set_max_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'tertiary' and [length] = 8"));
      rule.set_max_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'trunk' or [highway] = 'primary'"));
      rule.set_max_scale(static_cast<unsigned long long>(75000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      boost::shared_ptr<mapnik::text_symbolizer> textSymb0 = mapnik_otb::create_text_symbolizer(mapnik_otb::parse_expression("[name]"),
                                        "DejaVu Sans Book", 8, mapnik::color("black"));
      textSymb0->set_label_placement(mapnik::LINE_PLACEMENT);
      textSymb0->set_halo_radius(0);
      rule.append(*textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'secondary'"));
      rule.set_max_scale(static_cast<unsigned long long>(75000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      boost::shared_ptr<mapnik::text_symbolizer> textSymb0 = mapnik_otb::create_text_symbolizer(mapnik_otb::parse_expression("[name]"),
                                        "DejaVu Sans Book", 8, mapnik::color("black"));
      textSymb0->set_label_placement(mapnik::LINE_PLACEMENT);
      textSymb0->set_halo_radius(1);
      textSymb0->set_halo_fill(mapnik::color("#fed7a5"));
      rule.append(*textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'trunk' or [highway] = 'primary'"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      boost::shared_ptr<mapnik::text_symbolizer> textSymb0 = mapnik_otb::create_text_symbolizer(mapnik_otb::parse_expression("[name]"),
                                        "DejaVu Sans Book", 9, mapnik::color("black"));
      textSymb0->set_label_placement(mapnik::LINE_PLACEMENT);
      textSymb0->set_halo_radius(0);
      rule.append(*textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'trunk' or [highway] = 'primary'"));
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
      boost::shared_ptr<mapnik::text_symbolizer> textSymb0 = mapnik_otb::create_text_symbolizer(mapnik_otb::parse_expression("[name]"),
                                        "DejaVu Sans Book", 10, mapnik::color("black"));
      textSymb0->set_label_placement(mapnik::LINE_PLACEMENT);
      textSymb0->set_halo_radius(0);
      rule.append(*textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'secondary'"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      boost::shared_ptr<mapnik::text_symbolizer> textSymb0 = mapnik_otb::create_text_symbolizer(mapnik_otb::parse_expression("[name]"),
                                        "DejaVu Sans Book", 9, mapnik::color("black"));
      textSymb0->set_label_placement(mapnik::LINE_PLACEMENT);
      textSymb0->set_halo_radius(1);
      textSymb0->set_halo_fill(mapnik::color("#fed7a5"));
      rule.append(*textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'secondary'"));
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
      boost::shared_ptr<mapnik::text_symbolizer> textSymb0 = mapnik_otb::create_text_symbolizer(mapnik_otb::parse_expression("[name]"),
                                        "DejaVu Sans Book", 10, mapnik::color("black"));
      textSymb0->set_label_placement(mapnik::LINE_PLACEMENT);
      textSymb0->set_halo_radius(1);
      textSymb0->set_halo_fill(mapnik::color("#fed7a5"));
      rule.append(*textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'tertiary'"));
      rule.set_max_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      boost::shared_ptr<mapnik::text_symbolizer> textSymb0 = mapnik_otb::create_text_symbolizer(mapnik_otb::parse_expression("[name]"),
                                        "DejaVu Sans Book", 9, mapnik::color("#000"));
      textSymb0->set_label_placement(mapnik::LINE_PLACEMENT);
      textSymb0->set_halo_radius(1);
      rule.append(*textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'tertiary'"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      boost::shared_ptr<mapnik::text_symbolizer> textSymb0 = mapnik_otb::create_text_symbolizer(mapnik_otb::parse_expression("[name]"),
                                        "DejaVu Sans Book", 11, mapnik::color("#000"));
      textSymb0->set_label_placement(mapnik::LINE_PLACEMENT);
      textSymb0->set_halo_radius(1);
      rule.append(*textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(10000ULL / m_ScaleFactor));
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'proposed' or [highway]='construction'"));
      boost::shared_ptr<mapnik::text_symbolizer> textSymb0 = mapnik_otb::create_text_symbolizer(mapnik_otb::parse_expression("[name]"),
                                        "DejaVu Sans Book", 9, mapnik::color("#000"));
      textSymb0->set_label_placement(mapnik::LINE_PLACEMENT);
      textSymb0->set_halo_radius(1);
      rule.append(*textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_max_scale(static_cast<unsigned long long>(10000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'proposed' or [highway]='construction'"));
      boost::shared_ptr<mapnik::text_symbolizer> textSymb0 = mapnik_otb::create_text_symbolizer(mapnik_otb::parse_expression("[name]"),
                                        "DejaVu Sans Book", 11, mapnik::color("#000"));
      textSymb0->set_label_placement(mapnik::LINE_PLACEMENT);
      textSymb0->set_halo_radius(1);
      rule.append(*textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'unclassified' or [highway] = 'residential'"));
      rule.set_max_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      boost::shared_ptr<mapnik::text_symbolizer> textSymb0 = mapnik_otb::create_text_symbolizer(mapnik_otb::parse_expression("[name]"),
                                        "DejaVu Sans Book", 9, mapnik::color("#000"));
      textSymb0->set_label_placement(mapnik::LINE_PLACEMENT);
      textSymb0->set_halo_radius(1);
      textSymb0->set_label_spacing(300);
      rule.append(*textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'unclassified' or [highway] = 'residential'"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      boost::shared_ptr<mapnik::text_symbolizer> textSymb0 = mapnik_otb::create_text_symbolizer(mapnik_otb::parse_expression("[name]"),
                                        "DejaVu Sans Book", 11, mapnik::color("#000"));
      textSymb0->set_label_placement(mapnik::LINE_PLACEMENT);
      textSymb0->set_halo_radius(1);
      textSymb0->set_label_spacing(400);
      rule.append(*textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_else(true);
      rule.set_max_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      boost::shared_ptr<mapnik::text_symbolizer> textSymb0 = mapnik_otb::create_text_symbolizer(mapnik_otb::parse_expression("[name]"),
                                        "DejaVu Sans Book", 9, mapnik::color("#000"));
      textSymb0->set_label_placement(mapnik::LINE_PLACEMENT);
      textSymb0->set_halo_radius(1);
      rule.append(*textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_else(true);
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      boost::shared_ptr<mapnik::text_symbolizer> textSymb0 = mapnik_otb::create_text_symbolizer(mapnik_otb::parse_expression("[name]"),
                                        "DejaVu Sans Book", 11, mapnik::color("#000"));
      textSymb0->set_label_placement(mapnik::LINE_PLACEMENT);
      textSymb0->set_halo_radius(1);
      rule.append(*textSymb0);
      style.add_rule(rule);
      }
    mapnikMap.insert_style("roads-text", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'motorway' or [highway] = 'motorway_link'"));
      rule.set_max_scale(static_cast<unsigned long long>(25000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#809bc0"), 0.5);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'motorway' or [highway] = 'motorway_link'"));
      rule.set_max_scale(static_cast<unsigned long long>(5000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(2000000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#809bc0"), 1);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'motorway' or [highway] = 'motorway_link'"));
      rule.set_max_scale(static_cast<unsigned long long>(2000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#809bc0"), 1.4);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'motorway' or [highway] = 'motorway_link'"));
      rule.set_max_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(500000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#809bc0"), 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'motorway' or [highway] = 'motorway_link'"));
      rule.set_max_scale(static_cast<unsigned long long>(500000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(200000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#809bc0"), 2.5);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'trunk' or [highway] = 'trunk_link'"));
      rule.set_max_scale(static_cast<unsigned long long>(25000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#a9dba9"), 0.4);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'trunk' or [highway] = 'trunk_link'"));
      rule.set_max_scale(static_cast<unsigned long long>(5000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(2000000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#97d397"), 1);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'trunk' or [highway] = 'trunk_link'"));
      rule.set_max_scale(static_cast<unsigned long long>(2000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(500000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#97d397"), 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'trunk' or [highway] = 'trunk_link'"));
      rule.set_max_scale(static_cast<unsigned long long>(500000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(200000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#97d397"), 2.5);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'primary' or [highway] = 'primary_link'"));
      rule.set_max_scale(static_cast<unsigned long long>(5000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(2000000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#ec989a"), 0.5);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'primary' or [highway] = 'primary_link'"));
      rule.set_max_scale(static_cast<unsigned long long>(2000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#ec989a"), 1.2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'primary' or [highway] = 'primary_link'"));
      rule.set_max_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(500000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#ec989a"), 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'primary' or [highway] = 'primary_link'"));
      rule.set_max_scale(static_cast<unsigned long long>(500000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(200000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#ec989a"), 2.5);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'secondary' or [highway] = 'secondary_link'"));
      rule.set_max_scale(static_cast<unsigned long long>(2000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(500000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#fecc8b"), 1);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'secondary' or [highway] = 'secondary_link'"));
      rule.set_max_scale(static_cast<unsigned long long>(500000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#fecc8b"), 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_max_scale(static_cast<unsigned long long>(10000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(2000000ULL / m_ScaleFactor));
      rule.set_filter(mapnik_otb::parse_expression("[railway] = 'rail'"));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#aaa"), 0.6);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_max_scale(static_cast<unsigned long long>(2000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
      rule.set_filter(mapnik_otb::parse_expression("[railway] = 'rail' and not ([tunnel]='yes' or [tunnel]='true')"));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#aaa"), 1);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_max_scale(static_cast<unsigned long long>(2000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
      rule.set_filter(mapnik_otb::parse_expression("[railway] = 'rail' and ([tunnel]='yes' or [tunnel]='true')"));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#aaa"), 1);
        stroke.add_dash(5, 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_max_scale(static_cast<unsigned long long>(2500000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
      rule.set_filter(mapnik_otb::parse_expression(
                        "[railway] = 'tram' or [railway] = 'light_rail' or [railway] = 'narrow_gauge'"));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#ccc"), 1);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_max_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_filter(mapnik_otb::parse_expression("[railway] = 'rail' and not ([tunnel]='yes' or [tunnel]='true')"));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#aaa"), 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_max_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_filter(mapnik_otb::parse_expression("[railway] = 'rail' and ([tunnel]='yes' or [tunnel]='true')"));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#aaa"), 2);
        stroke.add_dash(5, 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_max_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_filter(mapnik_otb::parse_expression(
                        "[railway] = 'tram' or [railway] = 'light_rail' or [railway] = 'narrow_gauge'"));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#aaa"), 1);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_max_scale(static_cast<unsigned long long>(200000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_filter(mapnik_otb::parse_expression("[railway] = 'preserved'"));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#aaa"), 1.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
      rule.set_filter(mapnik_otb::parse_expression("[railway] = 'preserved'"));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#999999"), 3);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("white"), 1);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.add_dash(0, 1);
        stroke.add_dash(8, 1);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
      rule.set_filter(mapnik_otb::parse_expression("[railway]='monorail'"));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#fff"), 4);
        stroke.set_opacity (0.4);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#777"), 3.0);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        stroke.add_dash(2, 3);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
    mapnikMap.insert_style("roads", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'motorway' and not ([tunnel] = 'yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(200000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#506077"), 3);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression(
                        "[highway] = 'trunk' and not ([tunnel] = 'yes' or [tunnel] = 'true' or [tunnel] = '1')"));
      rule.set_max_scale(static_cast<unsigned long long>(200000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#477147"), 3);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression(
                        "[highway] = 'primary' and not ([tunnel] = 'yes' or [tunnel] = 'true' or [tunnel] = '1')"));
      rule.set_max_scale(static_cast<unsigned long long>(200000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#8d4346"), 3);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression(
                        "[highway] = 'secondary' and not ([tunnel] = 'yes' or [tunnel] = 'true' or [tunnel] = '1')"));
      rule.set_max_scale(static_cast<unsigned long long>(150000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(75000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#a37b48"), 2.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'motorway' and not ([tunnel] = 'yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#506077"), 6.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression(
                        "[highway] = 'trunk' and not ([tunnel] = 'yes' or [tunnel] = 'true' or [tunnel] = '1')"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#477147"), 7.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression(
                        "[highway] = 'primary' and not ([tunnel] = 'yes' or [tunnel] = 'true' or [tunnel] = '1')"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#8d4346"), 7.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression(
                        "[highway] = 'secondary' and not ([tunnel] = 'yes' or [tunnel] = 'true' or [tunnel] = '1')"));
      rule.set_max_scale(static_cast<unsigned long long>(75000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#a37b48"), 8.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'motorway' and not ([tunnel] = 'yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#506077"), 10);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression(
                        "[highway] = 'trunk' and not ([tunnel] = 'yes' or [tunnel] = 'true' or [tunnel] = '1')"));
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#477147"), 11.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression(
                        "[highway] = 'primary' and not ([tunnel] = 'yes' or [tunnel] = 'true' or [tunnel] = '1')"));
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#8d4346"), 11.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression(
                        "[highway] = 'secondary' and not ([tunnel] = 'yes' or [tunnel] = 'true' or [tunnel] = '1')"));
      rule.set_max_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#a37b48"), 11.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression(
                        "[highway] = 'motorway' and not ([tunnel] = 'yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#506077"), 13);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression(
                        "[highway] = 'trunk' and not ([tunnel] = 'yes' or [tunnel] = 'true' or [tunnel] = '1')"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#477147"), 16);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression(
                        "[highway] = 'primary' and not ([tunnel] = 'yes' or [tunnel] = 'true' or [tunnel] = '1')"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#8d4346"), 16);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression(
                        "[highway] = 'secondary' and not ([tunnel] = 'yes' or [tunnel] = 'true' or [tunnel] = '1')"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#a37b48"), 16);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression(
                        "[highway] = 'tertiary' and not ([tunnel]='yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#bbb"), 6);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'tertiary' and ([tunnel]='yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#999"), 6);
        stroke.add_dash(4, 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression(
                        "([highway] = 'residential' or [highway] = 'minor' or [highway] = 'unclassified' or "
                        "[highway] = 'road') and not ([tunnel]='yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#999"), 3);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression(
                        "([highway] = 'residential' or [highway] = 'minor' or [highway] = 'unclassified' or "
                        "[highway] = 'road') and ([tunnel]='yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#999"), 3);
        stroke.add_dash(4, 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'tertiary' and not ([tunnel]='yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#bbb"), 7.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'tertiary' and ([tunnel]='yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#999"), 7.5);
        stroke.add_dash(4, 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression(
                        "([highway] = 'residential' or [highway] = 'minor' or [highway] = 'unclassified' or "
                        "[highway] = 'road') and not ([tunnel]='yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#999"), 4.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression(
                        "([highway] = 'residential' or [highway] = 'minor' or [highway] = 'unclassified' or "
                        "[highway] = 'road') and ([tunnel]='yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#999"), 4.5);
        stroke.add_dash(4, 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression(
                        "([highway] = 'tertiary' or [highway] = 'residential' or [highway] = 'minor' or "
                        "[highway] = 'unclassified' or [highway] = 'road') and not ([tunnel]='yes' or "
                        "[tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#bbb"), 11);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression(
                        "([highway] = 'tertiary' or [highway] = 'residential' or [highway] = 'minor' or "
                        "[highway] = 'unclassified' or [highway] = 'road') and ([tunnel]='yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#999"), 11);
        stroke.add_dash(4, 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression(
                        "([highway] = 'tertiary' or [highway] = 'residential' or [highway] = 'minor' or "
                        "[highway] = 'unclassified' or [highway] = 'road') and not ([tunnel]='yes' or "
                        "[tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#bbb"), 16);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression(
                        "([highway] = 'tertiary' or [highway] = 'residential' or [highway] = 'minor' or "
                        "[highway] = 'unclassified' or [highway] = 'road') and ([tunnel]='yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#999"), 16);
        stroke.add_dash(4, 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression(
                        "[highway] = 'service' and not ([service]='parking_aisle' or [tunnel]='yes' or "
                        "[tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#999"), 3);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'service' and ([tunnel]='yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#999"), 3);
        stroke.add_dash(4, 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression(
                        "[highway] = 'service' and not ([service]='parking_aisle' or[tunnel]='yes' or "
                        "[tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#999"), 8);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'service' and ([tunnel]='yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#999"), 8);
        stroke.add_dash(4, 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'service' and [service]='parking_aisle'"));
      rule.set_max_scale(static_cast<unsigned long long>(10000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#999"), 4);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'pedestrian' and not ([tunnel]='yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("grey"), 2);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'pedestrian' and ([tunnel]='yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("grey"), 2);
        stroke.add_dash(4, 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'pedestrian' and not ([tunnel]='yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("grey"), 3.6);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'pedestrian' and ([tunnel]='yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("grey"), 3.6);
        stroke.add_dash(4, 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'pedestrian' and not ([tunnel]='yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(10000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("grey"), 6.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'pedestrian' and ([tunnel]='yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(10000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("grey"), 6.5);
        stroke.add_dash(4, 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'pedestrian' and not ([tunnel]='yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(10000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("grey"), 9);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik_otb::rule rule;
      rule.set_filter(mapnik_otb::parse_expression("[highway] = 'pedestrian' and ([tunnel]='yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(10000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("grey"), 9);
        stroke.add_dash(4, 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
    mapnikMap.insert_style("minor-roads-casing", style);
    }
}
} //namespace otb
