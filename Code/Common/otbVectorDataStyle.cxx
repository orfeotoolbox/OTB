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

namespace otb
{
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
::LoadOSMStyle(mapnik::Map& mapnikMap) const
{

  {
    mapnik::feature_type_style style;
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[mapnik:geometry] = 1"));
      style.add_rule(rule);
    }
//     {
//       mapnik::rule_type rule;
//       rule.set_filter(mapnik::create_filter("[mapnik:geometry] = 2"));
//       {
//         mapnik::line_symbolizer geom = mapnik::line_symbolizer();
//         mapnik::stroke stroke = mapnik::stroke(mapnik::color("red"), 6);
//         stroke.set_opacity (0.5);
//         geom.set_stroke(stroke);
//         rule.append(geom);
//       }
//       {
//         mapnik::line_symbolizer geom = mapnik::line_symbolizer();
//         mapnik::stroke stroke = mapnik::stroke(mapnik::color("yellow"), 2);
//         stroke.set_opacity (1.0);
//         geom.set_stroke(stroke);
//         rule.append(geom);
//       }
//       style.add_rule(rule);
//     }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[mapnik:geometry] = 3"));
      style.add_rule(rule);
    }
    mapnikMap.insert_style("mapnik:selection",style);
  }
  {
    mapnik::feature_type_style style;
    {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(250000000000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(6000000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#b5d0d0"), 0.5);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    mapnikMap.insert_style("world-1",style);
  }
  {
    mapnik::feature_type_style style;
    {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(6000000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(600000LLU/m_ScaleFactor));
      style.add_rule(rule);
    }
    mapnikMap.insert_style("world",style);
  }
  {
    mapnik::feature_type_style style;
    {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(600000LLU/m_ScaleFactor));
      style.add_rule(rule);
    }
    mapnikMap.insert_style("coast-poly",style);
  }
  {
    mapnik::feature_type_style style;
    {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(600000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("skyblue"));
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    mapnikMap.insert_style("coast-line",style);
  }
  {
    mapnik::feature_type_style style;
    {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(2500000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(500000LLU/m_ScaleFactor));
      style.add_rule(rule);
    }
    mapnikMap.insert_style("builtup",style);
  }
  {
    mapnik::feature_type_style style;
    {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(50000000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(10000000LLU/m_ScaleFactor));
      mapnik::text_symbolizer textSymb("place_name", "DejaVu Sans Book", 10, mapnik::color("#444"));
      textSymb.set_halo_radius(1);
      textSymb.set_wrap_width (0);
      rule.append(textSymb);
      style.add_rule(rule);
    }
    mapnikMap.insert_style("places",style);
  }
  {
    mapnik::feature_type_style style;
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[place] = 'city'"));
      rule.set_max_scale(static_cast<unsigned long long>(10000000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(2000000LLU/m_ScaleFactor));
      mapnik::text_symbolizer textSymb("name", "DejaVu Sans Book", 10, mapnik::color("#000"));
      textSymb.set_halo_radius(1);
      textSymb.set_wrap_width (0);
      textSymb.set_displacement(0.0,0);
      rule.append(textSymb);
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[place] = 'city'"));
      rule.set_max_scale(static_cast<unsigned long long>(2000000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(20000LLU/m_ScaleFactor));
      mapnik::text_symbolizer textSymb("name", "DejaVu Sans Book", 14, mapnik::color("#000"));
      textSymb.set_halo_radius(2);
      textSymb.set_wrap_width (0);
      textSymb.set_displacement(0.0,0);
      rule.append(textSymb);
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[place] = 'town'"));
      rule.set_max_scale(static_cast<unsigned long long>(2000000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(50000LLU/m_ScaleFactor));
      mapnik::text_symbolizer textSymb("name", "DejaVu Sans Book", 11, mapnik::color("#000"));
      textSymb.set_halo_radius(2);
      textSymb.set_wrap_width (20);
      rule.append(textSymb);
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[place] = 'town'"));
      rule.set_max_scale(static_cast<unsigned long long>(50000LLU/m_ScaleFactor));
      mapnik::text_symbolizer textSymb("name", "DejaVu Sans Book", 14, mapnik::color("#777777"));
      textSymb.set_halo_radius(1);
      textSymb.set_wrap_width (20);
      rule.append(textSymb);
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[place] = 'village'"));
      rule.set_max_scale(static_cast<unsigned long long>(250000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(50000LLU/m_ScaleFactor));
      mapnik::text_symbolizer textSymb("name", "DejaVu Sans Book", 8, mapnik::color("#000"));
      textSymb.set_halo_radius(2);
      textSymb.set_wrap_width (0);
      rule.append(textSymb);
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[place] = 'village'"));
      rule.set_max_scale(static_cast<unsigned long long>(50000LLU/m_ScaleFactor));
      mapnik::text_symbolizer textSymb("name", "DejaVu Sans Book", 11, mapnik::color("#777777"));
      textSymb.set_halo_radius(1);
      textSymb.set_wrap_width (0);
      rule.append(textSymb);
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[place] = 'hamlet'"));
      rule.set_max_scale(static_cast<unsigned long long>(125000LLU/m_ScaleFactor));
      mapnik::text_symbolizer textSymb("name", "DejaVu Sans Book", 8, mapnik::color("#000"));
      textSymb.set_halo_radius(1);
      textSymb.set_wrap_width (0);
      rule.append(textSymb);
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[amenity] = 'pub'"));
      rule.set_max_scale(static_cast<unsigned long long>(5000LLU/m_ScaleFactor));
      mapnik::text_symbolizer textSymb("name", "DejaVu Sans Bold", 10, mapnik::color("brown"));
      textSymb.set_halo_radius(1);
      textSymb.set_wrap_width (0);
      textSymb.set_displacement(0.0,16);
      rule.append(textSymb);
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[amenity] = 'place_of_worship'"));
      rule.set_max_scale(static_cast<unsigned long long>(5000LLU/m_ScaleFactor));
      mapnik::text_symbolizer textSymb("name", "DejaVu Sans Book", 10, mapnik::color("#000033"));
      textSymb.set_halo_radius(1);
      textSymb.set_wrap_width (20);
      textSymb.set_displacement(0.0,12);
      rule.append(textSymb);
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[leisure] <> '' or [landuse] <> '' "));
      rule.set_max_scale(static_cast<unsigned long long>(20000LLU/m_ScaleFactor));
      mapnik::text_symbolizer textSymb("name", "DejaVu Sans Book", 10, mapnik::color("#000"));
      textSymb.set_halo_radius(2);
      textSymb.set_wrap_width (10);
      rule.append(textSymb);
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[natural] = 'wood'"));
      rule.set_max_scale(static_cast<unsigned long long>(20000LLU/m_ScaleFactor));
      mapnik::text_symbolizer textSymb("name", "DejaVu Sans Book", 10, mapnik::color("#000"));
      textSymb.set_halo_radius(2);
      textSymb.set_wrap_width (10);
      rule.append(textSymb);
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[natural] = 'peak'"));
      rule.set_max_scale(static_cast<unsigned long long>(100000LLU/m_ScaleFactor));
      mapnik::text_symbolizer textSymb("name", "DejaVu Sans Book", 10, mapnik::color("brown"));
      textSymb.set_halo_radius(1);
      textSymb.set_wrap_width (20);
      textSymb.set_displacement(0.0,10);
      rule.append(textSymb);
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[natural] = 'water' or [natural] = 'lake' or [landuse] = 'reservoir'"));
      rule.set_max_scale(static_cast<unsigned long long>(20000LLU/m_ScaleFactor));
      mapnik::text_symbolizer textSymb("name", "DejaVu Sans Book", 10, mapnik::color("#6699cc"));
      textSymb.set_halo_radius(1);
      textSymb.set_wrap_width (20);
      rule.append(textSymb);
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[tourism] <> ''"));
      rule.set_max_scale(static_cast<unsigned long long>(10000LLU/m_ScaleFactor));
      mapnik::text_symbolizer textSymb("name", "DejaVu Sans Book", 10, mapnik::color("#660033"));
      textSymb.set_halo_radius(2);
      textSymb.set_wrap_width (10);
      rule.append(textSymb);
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[amenity] = 'school' or [amenity] = 'university'"));
      rule.set_max_scale(static_cast<unsigned long long>(25000LLU/m_ScaleFactor));
      mapnik::text_symbolizer textSymb("name", "DejaVu Sans Book", 8, mapnik::color("#000033"));
      textSymb.set_halo_radius(2);
      textSymb.set_wrap_width (12);
      rule.append(textSymb);
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[amenity] = 'hospital'"));
      rule.set_max_scale(static_cast<unsigned long long>(25000LLU/m_ScaleFactor));
      mapnik::text_symbolizer textSymb("name", "DejaVu Sans Book", 8, mapnik::color("#000033"));
      textSymb.set_halo_radius(2);
      textSymb.set_wrap_width (12);
      textSymb.set_displacement(0.0,12);
      rule.append(textSymb);
      style.add_rule(rule);
    }
    mapnikMap.insert_style("text",style);
  }
  {
    mapnik::feature_type_style style;
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[landuse] = 'reservoir' or [landuse] = 'water'"));
      rule.set_max_scale(static_cast<unsigned long long>(1000000LLU/m_ScaleFactor));
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[natural] = 'lake' or [natural] = 'water'"));
      rule.set_max_scale(static_cast<unsigned long long>(1000000LLU/m_ScaleFactor));
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[natural] = 'land'"));
      rule.set_max_scale(static_cast<unsigned long long>(1000000LLU/m_ScaleFactor));
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[waterway]='river'"));
      rule.set_max_scale(static_cast<unsigned long long>(250000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(25000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#b5d0d0"), 2);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[waterway]='canal'"));
      rule.set_max_scale(static_cast<unsigned long long>(250000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#b5d0d0"), 3);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[waterway]='canal'"));
      rule.set_max_scale(static_cast<unsigned long long>(100000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(50000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#b5d0d0"), 4);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      mapnik::text_symbolizer textSymb("name", "DejaVu Sans Book", 8, mapnik::color("#6699cc"));
      textSymb.set_label_placement(mapnik::LINE_PLACEMENT);
      textSymb.set_halo_radius(1);
      rule.append(textSymb);
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(100000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(25000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#b5d0d0"), 1);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[waterway]='river'"));
      rule.set_max_scale(static_cast<unsigned long long>(25000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#b5d0d0"), 10);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      mapnik::text_symbolizer textSymb("name", "DejaVu Sans Book", 10, mapnik::color("#6699cc"));
      textSymb.set_label_placement(mapnik::LINE_PLACEMENT);
      textSymb.set_halo_radius(1);
      rule.append(textSymb);
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[waterway]='canal'"));
      rule.set_max_scale(static_cast<unsigned long long>(50000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#b5d0d0"), 7);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      mapnik::text_symbolizer textSymb("name", "DejaVu Sans Book", 9, mapnik::color("#6699cc"));
      textSymb.set_label_placement(mapnik::LINE_PLACEMENT);
      textSymb.set_halo_radius(1);
      rule.append(textSymb);
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(25000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#b5d0d0"), 2);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      mapnik::text_symbolizer textSymb("name", "DejaVu Sans Book", 8, mapnik::color("#6699cc"));
      textSymb.set_label_placement(mapnik::LINE_PLACEMENT);
      textSymb.set_halo_radius(1);
      rule.append(textSymb);
      style.add_rule(rule);
    }
    mapnikMap.insert_style("water",style);
  }
  {
    mapnik::feature_type_style style;
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'motorway' or [highway]='motorway_link'"));
      rule.set_max_scale(static_cast<unsigned long long>(150000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(75000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#506077"), 4);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'motorway' or [highway]='motorway_link'"));
      rule.set_max_scale(static_cast<unsigned long long>(75000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(25000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#506077"), 7);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'motorway' or [highway]='motorway_link'"));
      rule.set_max_scale(static_cast<unsigned long long>(25000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#506077"), 14);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'motorway' or [highway]='motorway_link'"));
      rule.set_max_scale(static_cast<unsigned long long>(5000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#506077"), 17);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'trunk' or [highway] = 'trunk_link'"));
      rule.set_max_scale(static_cast<unsigned long long>(150000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(75000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#477147"), 4);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'trunk' or [highway] = 'trunk_link'"));
      rule.set_max_scale(static_cast<unsigned long long>(75000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(25000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#477147"), 11);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'trunk' or [highway] = 'trunk_link'"));
      rule.set_max_scale(static_cast<unsigned long long>(25000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#477147"), 14);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'trunk' or [highway] = 'trunk_link'"));
      rule.set_max_scale(static_cast<unsigned long long>(5000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#477147"), 17);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'primary' or [highway] = 'primary_link'"));
      rule.set_max_scale(static_cast<unsigned long long>(150000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(75000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#8d4346"), 4);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'primary' or [highway] = 'primary_link'"));
      rule.set_max_scale(static_cast<unsigned long long>(75000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(25000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#8d4346"), 10);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'primary' or [highway] = 'primary_link'"));
      rule.set_max_scale(static_cast<unsigned long long>(25000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#8d4346"), 12);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'primary' or [highway] = 'primary_link'"));
      rule.set_max_scale(static_cast<unsigned long long>(5000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#8d4346"), 17);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'secondary' or [highway] = 'secondary_link'"));
      rule.set_max_scale(static_cast<unsigned long long>(150000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(75000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#a37b48"), 4);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'secondary' or [highway] = 'secondary_link'"));
      rule.set_max_scale(static_cast<unsigned long long>(75000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(25000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#a37b48"), 10);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'secondary' or [highway] = 'secondary_link'"));
      rule.set_max_scale(static_cast<unsigned long long>(25000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#a37b48"), 12);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'secondary' or [highway] = 'secondary_link'"));
      rule.set_max_scale(static_cast<unsigned long long>(5000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#a37b48"), 17);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'tertiary'"));
      rule.set_max_scale(static_cast<unsigned long long>(100000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(50000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#999"), 7);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'residential' or [highway] = 'minor' or [highway] = 'unclassified'"));
      rule.set_max_scale(static_cast<unsigned long long>(100000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(50000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#999"), 3);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'tertiary'"));
      rule.set_max_scale(static_cast<unsigned long long>(50000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(25000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#999"), 9);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'residential' or [highway] = 'minor' or [highway] = 'unclassified'"));
      rule.set_max_scale(static_cast<unsigned long long>(50000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(25000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#999"), 4.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'tertiary' or [highway] = 'residential' or [highway] = 'minor' or [highway] = 'unclassified'"));
      rule.set_max_scale(static_cast<unsigned long long>(25000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#999"), 11);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'tertiary' or [highway] = 'residential' or [highway] = 'minor' or [highway] = 'unclassified'"));
      rule.set_max_scale(static_cast<unsigned long long>(5000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#999"), 16);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'service'"));
      rule.set_max_scale(static_cast<unsigned long long>(50000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(25000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#999"), 3);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'service'"));
      rule.set_max_scale(static_cast<unsigned long long>(25000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#999"), 8);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    mapnikMap.insert_style("minor-roads-casing",style);
  }
  {
    mapnik::feature_type_style style;
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'motorway' or [highway] = 'motorway_link'"));
      rule.set_max_scale(static_cast<unsigned long long>(500000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(25000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#809bc0"), 4);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'motorway' or [highway] = 'motorway_link'"));
      rule.set_max_scale(static_cast<unsigned long long>(25000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(10000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#809bc0"), 8);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'motorway' or [highway] = 'motorway_link'"));
      rule.set_max_scale(static_cast<unsigned long long>(10000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#809bc0"), 12);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'motorway' or [highway] = 'motorway_link'"));
      rule.set_max_scale(static_cast<unsigned long long>(5000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#809bc0"), 14);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'trunk' or [highway] = 'trunk_link'"));
      rule.set_max_scale(static_cast<unsigned long long>(100000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(25000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#7fc97f"), 8);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'trunk' or [highway] = 'trunk_link'"));
      rule.set_max_scale(static_cast<unsigned long long>(50000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#7fc97f"), 10);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'trunk' or [highway] = 'trunk_link'"));
      rule.set_max_scale(static_cast<unsigned long long>(5000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#7fc97f"), 14);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'primary' or [highway] = 'primary_link'"));
      rule.set_max_scale(static_cast<unsigned long long>(100000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(25000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#e46d71"), 8);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'primary' or [highway] = 'primary_link'"));
      rule.set_max_scale(static_cast<unsigned long long>(25000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#e46d71"), 10);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'primary' or [highway] = 'primary_link'"));
      rule.set_max_scale(static_cast<unsigned long long>(5000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#e46d71"), 14);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'secondary' or [highway] = 'secondary_link'"));
      rule.set_max_scale(static_cast<unsigned long long>(150000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(75000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#fdbf6f"), 2);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'secondary' or [highway] = 'secondary_link'"));
      rule.set_max_scale(static_cast<unsigned long long>(75000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(25000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#fdbf6f"), 8);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'secondary' or [highway] = 'secondary_link'"));
      rule.set_max_scale(static_cast<unsigned long long>(25000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#fdbf6f"), 10);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'secondary' or [highway] = 'secondary_link'"));
      rule.set_max_scale(static_cast<unsigned long long>(5000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#fdbf6f"), 14);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(100000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000LLU/m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[railway] = 'tram' or [railway] = 'light_rail'"));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#aaa"), 2);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(200000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000LLU/m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[railway]='subway'"));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#777"), 2);
        stroke.add_dash(6,2);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'tertiary' or [highway] = 'residential' or [highway] = 'unclassified'"));
      rule.set_max_scale(static_cast<unsigned long long>(1000000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#ccc"), 1);
        stroke.add_dash(6,2);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'residential' or [highway] = 'minor' or [highway] = 'unclassified'"));
      rule.set_max_scale(static_cast<unsigned long long>(100000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(50000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#fff"), 2);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        stroke.add_dash(6,2);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'residential'  or [highway] = 'minor' or [highway] = 'unclassified'"));
      rule.set_max_scale(static_cast<unsigned long long>(50000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(15000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#fff"), 3);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        stroke.add_dash(6,2);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'residential'  or [highway] = 'minor' or [highway] = 'unclassified'"));
      rule.set_max_scale(static_cast<unsigned long long>(25000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#fff"), 9.4);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        stroke.add_dash(6,2);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'residential'  or [highway] = 'minor' or [highway] = 'unclassified'"));
      rule.set_max_scale(static_cast<unsigned long long>(5000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#fff"), 13);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        stroke.add_dash(6,2);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'tertiary'"));
      rule.set_max_scale(static_cast<unsigned long long>(100000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(50000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#ffffcc"), 5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        stroke.add_dash(6,2);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'tertiary'"));
      rule.set_max_scale(static_cast<unsigned long long>(50000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(25000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#ffffcc"), 7);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        stroke.add_dash(6,2);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'tertiary'"));
      rule.set_max_scale(static_cast<unsigned long long>(25000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#ffffcc"), 9.4);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        stroke.add_dash(6,2);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'tertiary'"));
      rule.set_max_scale(static_cast<unsigned long long>(5000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#ffffcc"), 13);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        stroke.add_dash(6,2);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'service'"));
      rule.set_max_scale(static_cast<unsigned long long>(100000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(50000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#bbbbbb"), 1);
        stroke.add_dash(6,2);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'service'"));
      rule.set_max_scale(static_cast<unsigned long long>(50000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(25000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("white"), 2);
        stroke.add_dash(6,2);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'service'"));
      rule.set_max_scale(static_cast<unsigned long long>(25000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("white"), 6);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        stroke.add_dash(6,2);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'bridleway'"));
      rule.set_max_scale(static_cast<unsigned long long>(100000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#fff"), 4);
        stroke.set_opacity (0.4);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        stroke.add_dash(6,2);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("green"), 1.5);
        stroke.add_dash(4,2);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'footway'"));
      rule.set_max_scale(static_cast<unsigned long long>(100000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#fff"), 6);
        stroke.set_opacity (0.4);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        stroke.add_dash(4,2);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("salmon"), 2.0);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        stroke.add_dash(1,3);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'cycleway'"));
      rule.set_max_scale(static_cast<unsigned long long>(100000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("white"), 4);
        stroke.set_opacity (0.4);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        stroke.add_dash(1,3);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("green"), 1.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        stroke.add_dash(1,3);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'track'"));
      rule.set_max_scale(static_cast<unsigned long long>(100000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("white"), 6);
        stroke.set_opacity (0.4);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        stroke.add_dash(1,3);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("salmon"), 2.0);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        stroke.add_dash(4,2);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'pedestrian'"));
      rule.set_max_scale(static_cast<unsigned long long>(50000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("grey"), 5);
        stroke.add_dash(4,2);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#ededed"), 4);
        stroke.add_dash(4,2);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'unsurfaced' or [highway] = 'byway'"));
      rule.set_max_scale(static_cast<unsigned long long>(100000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#fff"), 6);
        stroke.set_opacity (0.4);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        stroke.add_dash(4,2);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#ffc000"), 2.0);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        stroke.add_dash(4,4);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[route] = 'ferry'"));
      rule.set_max_scale(static_cast<unsigned long long>(500000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("navy"), 0.8);
        stroke.add_dash(6,6);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[aeroway] = 'runway'"));
      rule.set_max_scale(static_cast<unsigned long long>(500000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(200000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#bbc"), 2);
        stroke.add_dash(6,6);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[aeroway] = 'runway'"));
      rule.set_max_scale(static_cast<unsigned long long>(200000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(50000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#bbc"), 7);
        stroke.add_dash(6,6);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[aeroway] = 'runway'"));
      rule.set_max_scale(static_cast<unsigned long long>(50000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#bbc"), 18);
        stroke.add_dash(6,6);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[aeroway] = 'taxiway'"));
      rule.set_max_scale(static_cast<unsigned long long>(500000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(50000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#bbc"), 1);
        stroke.add_dash(6,6);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[aeroway] = 'taxiway'"));
      rule.set_max_scale(static_cast<unsigned long long>(50000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(20000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#bbc"), 4);
        stroke.add_dash(6,6);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[aeroway] = 'taxiway'"));
      rule.set_max_scale(static_cast<unsigned long long>(20000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#bbc"), 6);
        stroke.add_dash(6,6);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    mapnikMap.insert_style("minor-roads",style);
  }
  {
    mapnik::feature_type_style style;
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'motorway' or [highway] = 'motorway_link'"));
      rule.set_max_scale(static_cast<unsigned long long>(25000000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#809bc0"), 1);
        stroke.add_dash(6,6);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'motorway' or [highway] = 'motorway_link'"));
      rule.set_max_scale(static_cast<unsigned long long>(5000000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#809bc0"), 2);
        stroke.add_dash(6,6);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'motorway' or [highway] = 'motorway_link'"));
      rule.set_max_scale(static_cast<unsigned long long>(1000000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(500000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#809bc0"), 3);
        stroke.add_dash(6,6);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'trunk' or [highway] = 'trunk_link'"));
      rule.set_max_scale(static_cast<unsigned long long>(25000000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(2500000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#7fc97f"), 1);
        stroke.add_dash(6,6);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'trunk' or [highway] = 'trunk_link'"));
      rule.set_max_scale(static_cast<unsigned long long>(2500000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(500000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#7fc97f"), 2);
        stroke.add_dash(6,6);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'trunk' or [highway] = 'trunk_link'"));
      rule.set_max_scale(static_cast<unsigned long long>(500000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#7fc97f"), 3);
        stroke.add_dash(6,6);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'primary' or [highway] = 'primary_link'"));
      rule.set_max_scale(static_cast<unsigned long long>(5000000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(2000000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#e46d71"), 0.7);
        stroke.add_dash(6,6);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'primary' or [highway] = 'primary_link'"));
      rule.set_max_scale(static_cast<unsigned long long>(2000000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(500000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#e46d71"), 2.0);
        stroke.add_dash(6,6);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'primary' or [highway] = 'primary_link'"));
      rule.set_max_scale(static_cast<unsigned long long>(500000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#e46d71"), 3);
        stroke.add_dash(6,6);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'secondary' or [highway] = 'secondary_link'"));
      rule.set_max_scale(static_cast<unsigned long long>(2000000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(500000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#fdbf6f"), 1);
        stroke.add_dash(6,6);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'secondary' or [highway] = 'secondary_link'"));
      rule.set_max_scale(static_cast<unsigned long long>(500000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100000LLU/m_ScaleFactor));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#fdbf6f"), 2);
        stroke.add_dash(6,6);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(2500000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000000LLU/m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[railway] = 'rail' or [railway] = 'tram' or [railway] = 'light_rail'"));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#777"), 1);
        stroke.add_dash(6,6);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(1000000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100000LLU/m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[railway] = 'rail' or [railway] = 'tram' or [railway] = 'light_rail'"));
      {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke stroke = mapnik::stroke(mapnik::color("#777"), 2);
        stroke.add_dash(6,6);
        geom.set_stroke(stroke);
        rule.append(geom);
      }
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(100000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000LLU/m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[railway] = 'preserved'"));
      style.add_rule(rule);
    }
    mapnikMap.insert_style("roads",style);
  }
  {
    mapnik::feature_type_style style;
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'trunk' or [highway] = 'primary' or [highway] = 'secondary'"));
      rule.set_max_scale(static_cast<unsigned long long>(75000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(50000LLU/m_ScaleFactor));
      mapnik::text_symbolizer textSymb("name", "DejaVu Sans Book", 8, mapnik::color("black"));
      textSymb.set_label_placement(mapnik::LINE_PLACEMENT);
      textSymb.set_halo_radius(0);
      rule.append(textSymb);
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'trunk' or [highway] = 'primary' or [highway] = 'secondary'"));
      rule.set_max_scale(static_cast<unsigned long long>(50000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000LLU/m_ScaleFactor));
      mapnik::text_symbolizer textSymb("name", "DejaVu Sans Book", 10, mapnik::color("black"));
      textSymb.set_label_placement(mapnik::LINE_PLACEMENT);
      textSymb.set_halo_radius(0);
      rule.append(textSymb);
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(25000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000LLU/m_ScaleFactor));
      mapnik::text_symbolizer textSymb("name", "DejaVu Sans Book", 9, mapnik::color("#000"));
      textSymb.set_label_placement(mapnik::LINE_PLACEMENT);
      textSymb.set_halo_radius(1);
      rule.append(textSymb);
      style.add_rule(rule);
    }
    {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(5000LLU/m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100LLU/m_ScaleFactor));
      mapnik::text_symbolizer textSymb("name", "DejaVu Sans Book", 11, mapnik::color("#000"));
      textSymb.set_label_placement(mapnik::LINE_PLACEMENT);
      textSymb.set_halo_radius(1);
      rule.append(textSymb);
      style.add_rule(rule);
    }
    mapnikMap.insert_style("roads-text",style);
  }


}

}

