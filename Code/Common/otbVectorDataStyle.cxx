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
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(600000ULL / m_ScaleFactor));
        {
        mapnik::polygon_symbolizer geom = mapnik::polygon_symbolizer();
        geom.set_fill(mapnik::color("#f2efe9"));
        rule.append(geom);
        }
      style.add_rule(rule);
      }
    mapnikMap.insert_style("coast-poly", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(2500000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(500000ULL / m_ScaleFactor));
        {
        mapnik::polygon_symbolizer geom = mapnik::polygon_symbolizer();
        geom.set_fill(mapnik::color("#ddd"));
        rule.append(geom);
        }
      style.add_rule(rule);
      }
    mapnikMap.insert_style("builtup", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[highway]='turning_circle'"));
        {
        mapnik::point_symbolizer geom = mapnik::point_symbolizer();
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[highway]='turning_circle'"));
        {
        mapnik::point_symbolizer geom = mapnik::point_symbolizer();
        rule.append(geom);
        }
      style.add_rule(rule);
      }
    mapnikMap.insert_style("turning_circle", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(250000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[railway]='station' and not [disused]='yes'"));
        {
        mapnik::point_symbolizer geom = mapnik::point_symbolizer();
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[railway]='station' and not [disused]='yes'"));
        {
        mapnik::point_symbolizer geom = mapnik::point_symbolizer();
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[railway]='station' and [disused]='yes'"));
        {
        mapnik::point_symbolizer geom = mapnik::point_symbolizer();
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[railway]='station' and not [disused]='yes'"));
        {
        mapnik::point_symbolizer geom = mapnik::point_symbolizer();
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[railway]='halt' or [railway]='tram_stop' or [aerialway]='station'"));
        {
        mapnik::point_symbolizer geom = mapnik::point_symbolizer();
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[railway]='halt' or [railway]='tram_stop' or [aerialway]='station'"));
        {
        mapnik::point_symbolizer geom = mapnik::point_symbolizer();
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[railway]='halt' or [railway]='tram_stop' or [aerialway]='station'"));
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Book", 8, mapnik::color("#66f"));
      textSymb0.set_halo_radius(1);
      textSymb0.set_wrap_width(0);
      textSymb0.set_displacement(0.0, -8);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[railway] ='halt' or [railway]='tram_stop' or [aerialway]='station'"));
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Book", 10, mapnik::color("#66f"));
      textSymb0.set_halo_radius(1);
      textSymb0.set_wrap_width(0);
      textSymb0.set_displacement(0.0, -10);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[railway]='station' and not [disused]='yes'"));
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Bold", 9, mapnik::color("#66f"));
      textSymb0.set_halo_radius(1);
      textSymb0.set_wrap_width(0);
      textSymb0.set_displacement(0.0, -8);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[railway]='station' and not [disused]='yes'"));
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Bold", 11, mapnik::color("#66f"));
      textSymb0.set_halo_radius(1);
      textSymb0.set_wrap_width(0);
      textSymb0.set_displacement(0.0, -14);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[railway]='station' and [disused]='yes'"));
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Bold", 9, mapnik::color("grey"));
      textSymb0.set_halo_radius(1);
      textSymb0.set_wrap_width(0);
      textSymb0.set_displacement(0.0, -9);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(10000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[railway]='level_crossing'"));
        {
        mapnik::point_symbolizer geom = mapnik::point_symbolizer();
        rule.append(geom);
        }
      style.add_rule(rule);
      }
    mapnikMap.insert_style("stations", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(2000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(500000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[aeroway]='airport'"));
        {
        mapnik::point_symbolizer geom = mapnik::point_symbolizer();
        rule.append(geom);
        }
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Bold", 8, mapnik::color("#6692da"));
      textSymb0.set_halo_radius(1);
      textSymb0.set_wrap_width(0);
      textSymb0.set_displacement(0.0, -12);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(500000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[aeroway]='airport'"));
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Bold", 9, mapnik::color("#6692da"));
      textSymb0.set_halo_radius(1);
      textSymb0.set_wrap_width(0);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(500000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[aeroway]='aerodrome'"));
        {
        mapnik::point_symbolizer geom = mapnik::point_symbolizer();
        rule.append(geom);
        }
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Oblique", 8, mapnik::color("#6692da"));
      textSymb0.set_halo_radius(1);
      textSymb0.set_wrap_width(0);
      textSymb0.set_displacement(0.0, -12);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(500000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[aeroway]='aerodrome'"));
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Oblique", 8, mapnik::color("#6692da"));
      textSymb0.set_halo_radius(1);
      textSymb0.set_wrap_width(0);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(10000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[railway]='level_crossing'"));
        {
        mapnik::point_symbolizer geom = mapnik::point_symbolizer();
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(10000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[railway]='level_crossing'"));
        {
        mapnik::point_symbolizer geom = mapnik::point_symbolizer();
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[man_made]='lighthouse'"));
        {
        mapnik::point_symbolizer geom = mapnik::point_symbolizer();
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(500000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[natural]='peak'"));
        {
        mapnik::point_symbolizer geom = mapnik::point_symbolizer();
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(10000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[natural]='tree'"));
        {
        mapnik::point_symbolizer geom = mapnik::point_symbolizer();
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter(
                        "[man_made]='power_wind' or ([power]='generator' and [power_source] = 'wind')"));
        {
        mapnik::point_symbolizer geom = mapnik::point_symbolizer();
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(10000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[man_made]='windmill'"));
        {
        mapnik::point_symbolizer geom = mapnik::point_symbolizer();
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[man_made]='mast'"));
        {
        mapnik::point_symbolizer geom = mapnik::point_symbolizer();
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[highway]='mini_roundabout'"));
        {
        mapnik::point_symbolizer geom = mapnik::point_symbolizer();
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[highway]='gate' or [barrier]='gate'"));
        {
        mapnik::point_symbolizer geom = mapnik::point_symbolizer();
        rule.append(geom);
        }
      style.add_rule(rule);
      }
    mapnikMap.insert_style("symbols", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[amenity]='atm'"));
        {
        mapnik::point_symbolizer geom = mapnik::point_symbolizer();
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[amenity]='bank'"));
        {
        mapnik::point_symbolizer geom = mapnik::point_symbolizer();
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[amenity]='bar'"));
        {
        mapnik::point_symbolizer geom = mapnik::point_symbolizer();
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[amenity]='bus_stop' or [highway]='bus_stop'"));
        {
        mapnik::point_symbolizer geom = mapnik::point_symbolizer();
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[amenity]='cafe'"));
        {
        mapnik::point_symbolizer geom = mapnik::point_symbolizer();
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(10000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[tourism]='camp_site'"));
        {
        mapnik::point_symbolizer geom = mapnik::point_symbolizer();
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[amenity]='cinema'"));
        {
        mapnik::point_symbolizer geom = mapnik::point_symbolizer();
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[amenity]='fuel'"));
        {
        mapnik::point_symbolizer geom = mapnik::point_symbolizer();
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[amenity]='hospital'"));
        {
        mapnik::point_symbolizer geom = mapnik::point_symbolizer();
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[tourism]='hotel'"));
        {
        mapnik::point_symbolizer geom = mapnik::point_symbolizer();
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[amenity]='library'"));
        {
        mapnik::point_symbolizer geom = mapnik::point_symbolizer();
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[man_made]='mast'"));
        {
        mapnik::point_symbolizer geom = mapnik::point_symbolizer();
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[tourism]='museum'"));
        {
        mapnik::point_symbolizer geom = mapnik::point_symbolizer();
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[amenity]='parking'"));
        {
        mapnik::point_symbolizer geom = mapnik::point_symbolizer();
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[amenity]='pharmacy'"));
        {
        mapnik::point_symbolizer geom = mapnik::point_symbolizer();
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(10000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[amenity]='place_of_worship' and [religion] = 'christian'"));
        {
        mapnik::point_symbolizer geom = mapnik::point_symbolizer();
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(10000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[amenity]='place_of_worship' and [religion] = 'muslim'"));
        {
        mapnik::point_symbolizer geom = mapnik::point_symbolizer();
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(10000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[amenity]='place_of_worship' and [religion] = 'sikh'"));
        {
        mapnik::point_symbolizer geom = mapnik::point_symbolizer();
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(10000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[amenity]='place_of_worship' and [religion] = 'jewish'"));
        {
        mapnik::point_symbolizer geom = mapnik::point_symbolizer();
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(10000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[amenity]='place_of_worship'"));
        {
        mapnik::point_symbolizer geom = mapnik::point_symbolizer();
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[amenity]='post_box'"));
        {
        mapnik::point_symbolizer geom = mapnik::point_symbolizer();
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[amenity]='post_office'"));
        {
        mapnik::point_symbolizer geom = mapnik::point_symbolizer();
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(10000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[amenity]='pub'"));
        {
        mapnik::point_symbolizer geom = mapnik::point_symbolizer();
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(10000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[amenity]='recycling'"));
        {
        mapnik::point_symbolizer geom = mapnik::point_symbolizer();
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[amenity]='restaurant' or [amenity]='fast_food'"));
        {
        mapnik::point_symbolizer geom = mapnik::point_symbolizer();
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[amenity]='telephone'"));
        {
        mapnik::point_symbolizer geom = mapnik::point_symbolizer();
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[amenity]='theatre'"));
        {
        mapnik::point_symbolizer geom = mapnik::point_symbolizer();
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[amenity]='toilets'"));
        {
        mapnik::point_symbolizer geom = mapnik::point_symbolizer();
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(10000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[tourism]='viewpoint'"));
        {
        mapnik::point_symbolizer geom = mapnik::point_symbolizer();
        rule.append(geom);
        }
      style.add_rule(rule);
      }
    mapnikMap.insert_style("points", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'residential' or [highway]='unclassified'"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#999"), 1);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'pedestrian' or [highway]='service' or [highway]='footway'"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("grey"), 1);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
    mapnikMap.insert_style("highway-area-casing", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'living_street'"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
        {
        mapnik::polygon_symbolizer geom = mapnik::polygon_symbolizer();
        geom.set_fill(mapnik::color("#bbb"));
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "[highway] = 'residential' or [highway]='unclassified' or [highway]='service'"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
        {
        mapnik::polygon_symbolizer geom = mapnik::polygon_symbolizer();
        geom.set_fill(mapnik::color("#fff"));
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'pedestrian' or [highway]='footway'"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
        {
        mapnik::polygon_symbolizer geom = mapnik::polygon_symbolizer();
        geom.set_fill(mapnik::color("#ededed"));
        rule.append(geom);
        }
      style.add_rule(rule);
      }
    mapnikMap.insert_style("highway-area-fill", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[leisure] = 'marina'"));
      rule.set_max_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
        {
        mapnik::polygon_symbolizer geom = mapnik::polygon_symbolizer();
        geom.set_fill(mapnik::color("#b5d0d0"));
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[man_made] = 'pier'"));
      rule.set_max_scale(static_cast<unsigned long long>(200000ULL / m_ScaleFactor));
        {
        mapnik::polygon_symbolizer geom = mapnik::polygon_symbolizer();
        geom.set_fill(mapnik::color("#f2efe9"));
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[leisure] = 'playground'"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
        {
        mapnik::polygon_symbolizer geom = mapnik::polygon_symbolizer();
        geom.set_fill(mapnik::color("#ccffff"));
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#666"), 0.3);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "[tourism] = 'camp_site' or [tourism]='caravan_site' or [tourism] = 'picnic_site'"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
        {
        mapnik::polygon_symbolizer geom = mapnik::polygon_symbolizer();
        geom.set_fill(mapnik::color("#ccff99"));
        geom.set_opacity(0.5);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#666"), 0.3);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[tourism] = 'attraction'"));
      rule.set_max_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
        {
        mapnik::polygon_symbolizer geom = mapnik::polygon_symbolizer();
        geom.set_fill(mapnik::color("#f2caea"));
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[landuse] = 'quarry'"));
      rule.set_max_scale(static_cast<unsigned long long>(500000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("grey"), 0.5);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[leisure] = 'nature_reserve' or [landuse] = 'vineyard'"));
      rule.set_max_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
        {
        mapnik::polygon_symbolizer geom = mapnik::polygon_symbolizer();
        geom.set_fill(mapnik::color("#abdf96"));
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[landuse] = 'vineyard'"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[landuse] = 'vineyard'"));
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[leisure] = 'nature_reserve'"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[leisure] = 'nature_reserve'"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter(
                        "[landuse] = 'cemetery' or [landuse] = 'grave_yard' or [amenity]='grave_yard'"));
        {
        mapnik::polygon_symbolizer geom = mapnik::polygon_symbolizer();
        geom.set_fill(mapnik::color("#aacbaf"));
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[landuse] = 'residential'"));
        {
        mapnik::polygon_symbolizer geom = mapnik::polygon_symbolizer();
        geom.set_fill(mapnik::color("#ddd"));
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[military] = 'barracks'"));
        {
        mapnik::polygon_symbolizer geom = mapnik::polygon_symbolizer();
        geom.set_fill(mapnik::color("#ff8f8f"));
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[landuse]='military'"));
        {
        mapnik::polygon_symbolizer geom = mapnik::polygon_symbolizer();
        geom.set_fill(mapnik::color("#ffa8a8"));
        geom.set_opacity(0.4);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[landuse]='field' or [natural]='field'"));
        {
        mapnik::polygon_symbolizer geom = mapnik::polygon_symbolizer();
        geom.set_fill(mapnik::color("#666600"));
        geom.set_opacity(0.2);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[landuse]='field' or [natural]='field'"));
        {
        mapnik::polygon_symbolizer geom = mapnik::polygon_symbolizer();
        geom.set_fill(mapnik::color("#666600"));
        geom.set_opacity(0.2);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#660"), 0.3);
        stroke.set_opacity (0.4);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(2000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(500000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[military] = 'danger_area'"));
        {
        mapnik::polygon_symbolizer geom = mapnik::polygon_symbolizer();
        geom.set_fill(mapnik::color("pink"));
        geom.set_opacity(0.3);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(500000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[military] = 'danger_area'"));
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter(
                        "[landuse] = 'cemetery' or [landuse] = 'grave_yard' or [amenity]='grave_yard'"));
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[landuse] = 'meadow' or [landuse] = 'grass'"));
        {
        mapnik::polygon_symbolizer geom = mapnik::polygon_symbolizer();
        geom.set_fill(mapnik::color("#cfeca8"));
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[leisure] = 'park' or [leisure] = 'recreation_ground'"));
      rule.set_max_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
        {
        mapnik::polygon_symbolizer geom = mapnik::polygon_symbolizer();
        geom.set_fill(mapnik::color("#b6fdb6"));
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[tourism] = 'zoo'"));
      rule.set_max_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[leisure] = 'common'"));
      rule.set_max_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
        {
        mapnik::polygon_symbolizer geom = mapnik::polygon_symbolizer();
        geom.set_fill(mapnik::color("#cfeca8"));
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[leisure] = 'garden'"));
      rule.set_max_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
        {
        mapnik::polygon_symbolizer geom = mapnik::polygon_symbolizer();
        geom.set_fill(mapnik::color("#cfeca8"));
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[leisure] = 'golf_course'"));
      rule.set_max_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
        {
        mapnik::polygon_symbolizer geom = mapnik::polygon_symbolizer();
        geom.set_fill(mapnik::color("#b5e3b5"));
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[landuse] = 'allotments'"));
      rule.set_max_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
        {
        mapnik::polygon_symbolizer geom = mapnik::polygon_symbolizer();
        geom.set_fill(mapnik::color("#c8b084"));
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[landuse] = 'forest'"));
      rule.set_max_scale(static_cast<unsigned long long>(2000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
        {
        mapnik::polygon_symbolizer geom = mapnik::polygon_symbolizer();
        geom.set_fill(mapnik::color("#8dc56c"));
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[landuse] = 'forest'"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[landuse]='farmyard'"));
      rule.set_max_scale(static_cast<unsigned long long>(2000000ULL / m_ScaleFactor));
        {
        mapnik::polygon_symbolizer geom = mapnik::polygon_symbolizer();
        geom.set_fill(mapnik::color("#ddbf92"));
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[landuse] = 'farm' or [landuse]='farmland'"));
      rule.set_max_scale(static_cast<unsigned long long>(2000000ULL / m_ScaleFactor));
        {
        mapnik::polygon_symbolizer geom = mapnik::polygon_symbolizer();
        geom.set_fill(mapnik::color("#ead8bd"));
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[landuse] = 'recreation_ground' or [landuse] = 'conservation'"));
      rule.set_max_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
        {
        mapnik::polygon_symbolizer geom = mapnik::polygon_symbolizer();
        geom.set_fill(mapnik::color("#cfeca8"));
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[landuse] = 'village_green'"));
      rule.set_max_scale(static_cast<unsigned long long>(500000ULL / m_ScaleFactor));
        {
        mapnik::polygon_symbolizer geom = mapnik::polygon_symbolizer();
        geom.set_fill(mapnik::color("#cfeca8"));
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[landuse] = 'retail'"));
      rule.set_max_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
        {
        mapnik::polygon_symbolizer geom = mapnik::polygon_symbolizer();
        geom.set_fill(mapnik::color("#f1dada"));
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[landuse] = 'retail'"));
      rule.set_max_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("red"), 0.3);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[landuse] = 'industrial' or [landuse]='railway'"));
      rule.set_max_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
        {
        mapnik::polygon_symbolizer geom = mapnik::polygon_symbolizer();
        geom.set_fill(mapnik::color("#dfd1d6"));
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[power] = 'station'"));
      rule.set_max_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(200000ULL / m_ScaleFactor));
        {
        mapnik::polygon_symbolizer geom = mapnik::polygon_symbolizer();
        geom.set_fill(mapnik::color("#bbb"));
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[power] = 'station'"));
      rule.set_max_scale(static_cast<unsigned long long>(200000ULL / m_ScaleFactor));
        {
        mapnik::polygon_symbolizer geom = mapnik::polygon_symbolizer();
        geom.set_fill(mapnik::color("#bbb"));
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#555"), 0.4);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[power] = 'sub_station'"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
        {
        mapnik::polygon_symbolizer geom = mapnik::polygon_symbolizer();
        geom.set_fill(mapnik::color("#bbb"));
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#555"), 0.4);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[natural] = 'glacier'"));
      rule.set_max_scale(static_cast<unsigned long long>(10000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(2500000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#9cf"), 1.5);
        stroke.add_dash(4, 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[natural] = 'glacier'"));
      rule.set_max_scale(static_cast<unsigned long long>(2500000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#9cf"), 1.5);
        stroke.add_dash(4, 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Book", 9, mapnik::color("#99f"));
      textSymb0.set_halo_radius(1);
      textSymb0.set_wrap_width(20);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[landuse] = 'commercial'"));
      rule.set_max_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
        {
        mapnik::polygon_symbolizer geom = mapnik::polygon_symbolizer();
        geom.set_fill(mapnik::color("#efc8c8"));
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "[landuse] = 'brownfield' or [landuse]='landfill' or [landuse] = 'greenfield' or [landuse]='construction'"));
      rule.set_max_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
        {
        mapnik::polygon_symbolizer geom = mapnik::polygon_symbolizer();
        geom.set_fill(mapnik::color("#9d9d6c"));
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[natural] = 'wood' or [landuse] = 'wood'"));
      rule.set_max_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
        {
        mapnik::polygon_symbolizer geom = mapnik::polygon_symbolizer();
        geom.set_fill(mapnik::color("#aed1a0"));
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[natural] = 'heath'"));
      rule.set_max_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
        {
        mapnik::polygon_symbolizer geom = mapnik::polygon_symbolizer();
        geom.set_fill(mapnik::color("#ffffc0"));
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[landuse] = 'basin'"));
      rule.set_max_scale(static_cast<unsigned long long>(5000000ULL / m_ScaleFactor));
        {
        mapnik::polygon_symbolizer geom = mapnik::polygon_symbolizer();
        geom.set_fill(mapnik::color("#b5d0d0"));
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "[amenity] = 'university' or [amenity] = 'college' or [amenity] = 'school' or [amenity]='hospital'"));
      rule.set_max_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
        {
        mapnik::polygon_symbolizer geom = mapnik::polygon_symbolizer();
        geom.set_fill(mapnik::color("#f0f0d8"));
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "[amenity] = 'university' or [amenity] = 'college' or [amenity] = 'school' or [amenity]='hospital'"));
      rule.set_max_scale(static_cast<unsigned long long>(250000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("brown"), 0.3);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[amenity] = 'parking'"));
      rule.set_max_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
        {
        mapnik::polygon_symbolizer geom = mapnik::polygon_symbolizer();
        geom.set_fill(mapnik::color("#f7efb7"));
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[amenity] = 'parking'"));
      rule.set_max_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#eeeed1"), 0.3);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[aeroway] = 'apron'"));
      rule.set_max_scale(static_cast<unsigned long long>(200000ULL / m_ScaleFactor));
        {
        mapnik::polygon_symbolizer geom = mapnik::polygon_symbolizer();
        geom.set_fill(mapnik::color("#f0e0ff"));
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[natural] = 'beach'"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[natural] = 'mud'"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
    mapnikMap.insert_style("leisure", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[leisure] = 'sports_centre' or [leisure]='stadium'"));
      rule.set_max_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
        {
        mapnik::polygon_symbolizer geom = mapnik::polygon_symbolizer();
        geom.set_fill(mapnik::color("#33cc99"));
        rule.append(geom);
        }
      style.add_rule(rule);
      }
    mapnikMap.insert_style("sports_grounds", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[leisure] = 'pitch'"));
      rule.set_max_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
        {
        mapnik::polygon_symbolizer geom = mapnik::polygon_symbolizer();
        geom.set_fill(mapnik::color("#8ad3af"));
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#888"), 0.5);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[railway] = 'station' or [building] = 'station'"));
      rule.set_max_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
        {
        mapnik::polygon_symbolizer geom = mapnik::polygon_symbolizer();
        geom.set_fill(mapnik::color("#d4aaaa"));
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[building] = 'supermarket'"));
      rule.set_max_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
        {
        mapnik::polygon_symbolizer geom = mapnik::polygon_symbolizer();
        geom.set_fill(mapnik::color("pink"));
        geom.set_opacity(0.5);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[amenity] = 'place_of_worship'"));
      rule.set_max_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
        {
        mapnik::polygon_symbolizer geom = mapnik::polygon_symbolizer();
        geom.set_fill(mapnik::color("#777"));
        geom.set_opacity(0.5);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[amenity] = 'place_of_worship'"));
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
        {
        mapnik::polygon_symbolizer geom = mapnik::polygon_symbolizer();
        geom.set_fill(mapnik::color("#777"));
        geom.set_opacity(0.5);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#111"), 0.3);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[leisure]='track'"));
      rule.set_max_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
        {
        mapnik::polygon_symbolizer geom = mapnik::polygon_symbolizer();
        geom.set_fill(mapnik::color("#74dcba"));
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#888"), 0.5);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "[building] <> 'station' and [building] <> 'supermarket' and [building] <> ''"));
      rule.set_max_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
        {
        mapnik::polygon_symbolizer geom = mapnik::polygon_symbolizer();
        geom.set_fill(mapnik::color("#cc9999"));
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[aeroway] = 'terminal'"));
      rule.set_max_scale(static_cast<unsigned long long>(200000ULL / m_ScaleFactor));
        {
        mapnik::polygon_symbolizer geom = mapnik::polygon_symbolizer();
        geom.set_fill(mapnik::color("#cc99ff"));
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[aeroway] = 'terminal'"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#330066"), 0.2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
    mapnikMap.insert_style("buildings", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[military] = 'danger_area'"));
      rule.set_max_scale(static_cast<unsigned long long>(200000ULL / m_ScaleFactor));
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Bold", 9, mapnik::color("pink"));
      textSymb0.set_halo_radius(1);
      textSymb0.set_wrap_width(10);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[amenity] = 'university'"));
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Bold", 9, mapnik::color("#000033"));
      textSymb0.set_halo_radius(1);
      textSymb0.set_wrap_width(16);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[amenity] = 'college'"));
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Book", 9, mapnik::color("#000033"));
      textSymb0.set_halo_radius(1);
      textSymb0.set_wrap_width(14);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[way_area] >= 150000 and not [waterway]='riverbank'"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Book", 10, mapnik::color("#000033"));
      textSymb0.set_halo_radius(1);
      textSymb0.set_wrap_width(20);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([way_area] >= 80000 and [way_area] < 150000) and not [waterway]='riverbank'"));
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Book", 10, mapnik::color("#000033"));
      textSymb0.set_halo_radius(1);
      textSymb0.set_wrap_width(20);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([way_area] >= 20000 and [way_area] < 80000) and not [waterway]='riverbank'"));
      rule.set_max_scale(static_cast<unsigned long long>(10000ULL / m_ScaleFactor));
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Book", 10, mapnik::color("#000033"));
      textSymb0.set_halo_radius(1);
      textSymb0.set_wrap_width(20);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[way_area] < 20000 and not [waterway]='riverbank'"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Book", 10, mapnik::color("#000033"));
      textSymb0.set_halo_radius(1);
      textSymb0.set_wrap_width(20);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
    mapnikMap.insert_style("area-text", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[place] = 'country'"));
      rule.set_max_scale(static_cast<unsigned long long>(200000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(50000000ULL / m_ScaleFactor));
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Book", 9, mapnik::color("#9d6c9d"));
      textSymb0.set_halo_radius(1);
      textSymb0.set_wrap_width(0);
      textSymb0.set_displacement(0.0, 0);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[place] = 'country'"));
      rule.set_max_scale(static_cast<unsigned long long>(50000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000000ULL / m_ScaleFactor));
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Book", 11, mapnik::color("#9d6c9d"));
      textSymb0.set_halo_radius(1);
      textSymb0.set_wrap_width(0);
      textSymb0.set_displacement(0.0, 0);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
    mapnikMap.insert_style("country", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[place] = 'city' and [capital]='yes'"));
      rule.set_max_scale(static_cast<unsigned long long>(50000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(10000000ULL / m_ScaleFactor));
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Book", 9, mapnik::color("#000"));
      textSymb0.set_halo_radius(1);
      textSymb0.set_wrap_width(0);
      textSymb0.set_displacement(0.0, 0);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[place] = 'city' and [capital]='yes'"));
      rule.set_max_scale(static_cast<unsigned long long>(10000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(2000000ULL / m_ScaleFactor));
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Book", 11, mapnik::color("#000"));
      textSymb0.set_halo_radius(1);
      textSymb0.set_wrap_width(0);
      textSymb0.set_displacement(0.0, 0);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[place] = 'city' and not [capital]='yes'"));
      rule.set_max_scale(static_cast<unsigned long long>(10000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(2000000ULL / m_ScaleFactor));
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Book", 8, mapnik::color("#000"));
      textSymb0.set_halo_radius(1);
      textSymb0.set_wrap_width(0);
      textSymb0.set_displacement(0.0, 0);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[place] = 'city'"));
      rule.set_max_scale(static_cast<unsigned long long>(2000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(500000ULL / m_ScaleFactor));
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Book", 11, mapnik::color("#000"));
      textSymb0.set_halo_radius(1);
      textSymb0.set_wrap_width(0);
      textSymb0.set_displacement(0.0, 0);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[place] = 'city'"));
      rule.set_max_scale(static_cast<unsigned long long>(500000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Book", 14, mapnik::color("#000"));
      textSymb0.set_halo_radius(1);
      textSymb0.set_wrap_width(0);
      textSymb0.set_displacement(0.0, 0);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
    mapnikMap.insert_style("city", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[place] = 'town'"));
      rule.set_max_scale(static_cast<unsigned long long>(2000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(500000ULL / m_ScaleFactor));
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Book", 8, mapnik::color("#000"));
      textSymb0.set_halo_radius(1);
      textSymb0.set_wrap_width(20);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[place] = 'town'"));
      rule.set_max_scale(static_cast<unsigned long long>(500000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Book", 10, mapnik::color("#000"));
      textSymb0.set_halo_radius(1);
      textSymb0.set_wrap_width(20);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[place] = 'town'"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Book", 14, mapnik::color("#777777"));
      textSymb0.set_halo_radius(1);
      textSymb0.set_wrap_width(20);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
    mapnikMap.insert_style("town", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[place] = 'suburb'"));
      rule.set_max_scale(static_cast<unsigned long long>(250000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Book", 10, mapnik::color("#000"));
      textSymb0.set_halo_radius(1);
      textSymb0.set_wrap_width(0);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[place] = 'suburb'"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Book", 13, mapnik::color("#777777"));
      textSymb0.set_halo_radius(1);
      textSymb0.set_wrap_width(0);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
    mapnikMap.insert_style("suburb", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[place] = 'village'"));
      rule.set_max_scale(static_cast<unsigned long long>(250000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Book", 9, mapnik::color("#000"));
      textSymb0.set_halo_radius(1);
      textSymb0.set_wrap_width(0);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[place] = 'village'"));
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Book", 12, mapnik::color("#777777"));
      textSymb0.set_halo_radius(1);
      textSymb0.set_wrap_width(0);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
    mapnikMap.insert_style("village", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[place] = 'hamlet' or [place] = 'locality'"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(10000ULL / m_ScaleFactor));
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Book", 8, mapnik::color("#000"));
      textSymb0.set_halo_radius(1);
      textSymb0.set_wrap_width(0);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[place] = 'hamlet' or [place] = 'locality'"));
      rule.set_max_scale(static_cast<unsigned long long>(10000ULL / m_ScaleFactor));
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Book", 11, mapnik::color("#777777"));
      textSymb0.set_halo_radius(1);
      textSymb0.set_wrap_width(0);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
    mapnikMap.insert_style("hamlet", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[addr:interpolation] <> ''"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#555"), 1);
        stroke.add_dash(2, 4);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
    mapnikMap.insert_style("interpolation", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[addr:housenumber] <> '' and [building] <> ''"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      mapnik::text_symbolizer textSymb0("addr:housenumber", "DejaVu Sans Book", 9, mapnik::color("#555"));
      textSymb0.set_halo_radius(0);
      textSymb0.set_wrap_width(0);
      textSymb0.set_displacement(0.0, 0);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
    mapnikMap.insert_style("way_numbers", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[addr:housenumber] <> ''"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      mapnik::text_symbolizer textSymb0("addr:housenumber", "DejaVu Sans Book", 9, mapnik::color("#555"));
      textSymb0.set_halo_radius(1);
      textSymb0.set_halo_fill(mapnik::color("#eee"));
      textSymb0.set_wrap_width(0);
      textSymb0.set_displacement(0.0, 0);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
    mapnikMap.insert_style("node_numbers", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[place] = 'island'"));
      rule.set_max_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Oblique", 9, mapnik::color("#000"));
      textSymb0.set_halo_radius(1);
      textSymb0.set_wrap_width(0);
      textSymb0.set_displacement(0.0, 0);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "[amenity]='pub' or [amenity]='restaurant' or [amenity]='cafe' or [amenity]='bar' or [amenity]='fast_food'"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Bold", 10, mapnik::color("#734a08"));
      textSymb0.set_halo_radius(1);
      textSymb0.set_wrap_width(0);
      textSymb0.set_displacement(0.0, 14);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[amenity]='cinema' or [amenity]='library' or [amenity]='theatre'"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Bold", 10, mapnik::color("#734a08"));
      textSymb0.set_halo_radius(1);
      textSymb0.set_wrap_width(0);
      textSymb0.set_displacement(0.0, 18);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[amenity] = 'place_of_worship' or [amenity] = 'police'"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Book", 10, mapnik::color("#000033"));
      textSymb0.set_halo_radius(1);
      textSymb0.set_wrap_width(30);
      textSymb0.set_displacement(0.0, 16);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[leisure] <> '' or [landuse] <> '' "));
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Book", 9, mapnik::color("#000"));
      textSymb0.set_halo_radius(2);
      textSymb0.set_wrap_width(10);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[natural] = 'wood'"));
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Book", 10, mapnik::color("#000"));
      textSymb0.set_halo_radius(2);
      textSymb0.set_wrap_width(10);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[natural] = 'peak'"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Book", 10, mapnik::color("brown"));
      textSymb0.set_halo_radius(1);
      textSymb0.set_displacement(0.0, 10);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[natural] = 'peak'"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      mapnik::text_symbolizer textSymb0("ele", "DejaVu Sans Oblique", 9, mapnik::color("brown"));
      textSymb0.set_halo_radius(1);
      textSymb0.set_displacement(0.0, 20);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "[natural] = 'water' or [natural] = 'lake' or [landuse] = 'reservoir' or [landuse] = 'basin'"));
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Book", 10, mapnik::color("#6699cc"));
      textSymb0.set_halo_radius(1);
      textSymb0.set_wrap_width(20);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[tourism] = 'hotel'"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Book", 10, mapnik::color("#0092da"));
      textSymb0.set_halo_radius(1);
      textSymb0.set_wrap_width(0);
      textSymb0.set_displacement(0.0, 16);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[amenity] = 'fuel'"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Book", 9, mapnik::color("#0092da"));
      textSymb0.set_halo_radius(1);
      textSymb0.set_wrap_width(0);
      textSymb0.set_displacement(0.0, 14);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[tourism] = 'camp_site'"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Book", 10, mapnik::color("#0092da"));
      textSymb0.set_halo_radius(1);
      textSymb0.set_wrap_width(70);
      textSymb0.set_displacement(0.0, 15);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[tourism]='museum'"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Book", 10, mapnik::color("#734a08"));
      textSymb0.set_halo_radius(1);
      textSymb0.set_wrap_width(0);
      textSymb0.set_displacement(0.0, 16);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[tourism] = 'attraction'"));
      rule.set_max_scale(static_cast<unsigned long long>(10000ULL / m_ScaleFactor));
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Book", 10, mapnik::color("#660033"));
      textSymb0.set_halo_radius(2);
      textSymb0.set_wrap_width(10);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[amenity] = 'university'"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Book", 10, mapnik::color("#000033"));
      textSymb0.set_halo_radius(2);
      textSymb0.set_wrap_width(12);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[amenity] = 'school' or [amenity] = 'college'"));
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Book", 8, mapnik::color("#000033"));
      textSymb0.set_halo_radius(2);
      textSymb0.set_wrap_width(12);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[man_made] = 'lighthouse'"));
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Book", 9, mapnik::color("#000033"));
      textSymb0.set_halo_radius(2);
      textSymb0.set_wrap_width(12);
      textSymb0.set_displacement(0.0, 16);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[amenity] = 'hospital'"));
      rule.set_max_scale(static_cast<unsigned long long>(10000ULL / m_ScaleFactor));
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Book", 8, mapnik::color("#da0092"));
      textSymb0.set_halo_radius(2);
      textSymb0.set_wrap_width(12);
      textSymb0.set_displacement(0.0, 16);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[amenity] = 'pharmacy'"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Book", 8, mapnik::color("#da0092"));
      textSymb0.set_halo_radius(1);
      textSymb0.set_wrap_width(12);
      textSymb0.set_displacement(0.0, 12);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'motorway_junction'"));
      rule.set_max_scale(static_cast<unsigned long long>(500000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(200000ULL / m_ScaleFactor));
      mapnik::text_symbolizer textSymb0("ref", "DejaVu Sans Book", 9, mapnik::color("#6666ff"));
      textSymb0.set_halo_radius(1);
      textSymb0.set_wrap_width(12);
      textSymb0.set_minimum_distance (20);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'motorway_junction'"));
      rule.set_max_scale(static_cast<unsigned long long>(200000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      mapnik::text_symbolizer textSymb0("ref", "DejaVu Sans Book", 9, mapnik::color("#6666ff"));
      textSymb0.set_halo_radius(1);
      textSymb0.set_wrap_width(12);
      textSymb0.set_minimum_distance (20);
      rule.append(textSymb0);
      mapnik::text_symbolizer textSymb1("name", "DejaVu Sans Book", 8, mapnik::color("#6666ff"));
      textSymb1.set_halo_radius(1);
      textSymb1.set_wrap_width(12);
      textSymb1.set_displacement(0.0, -8);
      textSymb1.set_minimum_distance (20);
      rule.append(textSymb1);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'motorway_junction'"));
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      mapnik::text_symbolizer textSymb0("ref", "DejaVu Sans Book", 13, mapnik::color("#6666ff"));
      textSymb0.set_halo_radius(1);
      textSymb0.set_wrap_width(12);
      textSymb0.set_minimum_distance (20);
      rule.append(textSymb0);
      mapnik::text_symbolizer textSymb1("name", "DejaVu Sans Book", 14, mapnik::color("#6666ff"));
      textSymb1.set_halo_radius(1);
      textSymb1.set_wrap_width(12);
      textSymb1.set_displacement(0.0, -14);
      textSymb1.set_minimum_distance (20);
      rule.append(textSymb1);
      style.add_rule(rule);
      }
    mapnikMap.insert_style("text", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "[waterway] = 'dock' or [landuse] = 'reservoir' or [landuse] = 'water' or [waterway] = 'mill_pond' or [waterway] = 'riverbank'"));
      rule.set_max_scale(static_cast<unsigned long long>(2000000ULL / m_ScaleFactor));
        {
        mapnik::polygon_symbolizer geom = mapnik::polygon_symbolizer();
        geom.set_fill(mapnik::color("#b5d0d0"));
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[natural] = 'lake' or [natural] = 'water'"));
      rule.set_max_scale(static_cast<unsigned long long>(5000000ULL / m_ScaleFactor));
        {
        mapnik::polygon_symbolizer geom = mapnik::polygon_symbolizer();
        geom.set_fill(mapnik::color("#b5d0d0"));
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[natural] = 'marsh'"));
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[natural] = 'scrub'"));
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[natural] = 'land'"));
      rule.set_max_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
        {
        mapnik::polygon_symbolizer geom = mapnik::polygon_symbolizer();
        geom.set_fill(mapnik::color("#f2efe9"));
        rule.append(geom);
        }
      style.add_rule(rule);
      }
    mapnikMap.insert_style("water_areas", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[waterway]='weir'"));
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#aaa"), 2);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[waterway]='river'"));
      rule.set_max_scale(static_cast<unsigned long long>(250000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#b5d0d0"), 2);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[waterway]='canal' and not [disused]='yes'"));
      rule.set_max_scale(static_cast<unsigned long long>(250000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#b5d0d0"), 3);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[waterway]='canal' and not [disused]='yes'"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#b5d0d0"), 4);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Book", 8, mapnik::color("#6699cc"));
      textSymb0.set_label_placement(mapnik::LINE_PLACEMENT);
      textSymb0.set_halo_radius(1);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_else(true);
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#b5d0d0"), 1);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[waterway]='river'"));
      rule.set_max_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#b5d0d0"), 10);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Book", 10, mapnik::color("#6699cc"));
      textSymb0.set_label_placement(mapnik::LINE_PLACEMENT);
      textSymb0.set_halo_radius(1);
      textSymb0.set_minimum_distance (200);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[waterway]='canal' and ([tunnel] = 'yes' or [tunnel] ='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#b5d0d0"), 7);
        stroke.add_dash(4, 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("white"), 3);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Book", 9, mapnik::color("#6699cc"));
      textSymb0.set_label_placement(mapnik::LINE_PLACEMENT);
      textSymb0.set_halo_radius(1);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "[waterway]='canal' and not ([tunnel] = 'yes' or [tunnel] ='true' or [disused]='yes')"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#b5d0d0"), 7);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Book", 9, mapnik::color("#6699cc"));
      textSymb0.set_label_placement(mapnik::LINE_PLACEMENT);
      textSymb0.set_halo_radius(1);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[waterway]='canal' and ([tunnel] = 'yes' or [tunnel] ='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#b5d0d0"), 11);
        stroke.add_dash(4, 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("white"), 7);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Book", 9, mapnik::color("#6699cc"));
      textSymb0.set_label_placement(mapnik::LINE_PLACEMENT);
      textSymb0.set_halo_radius(1);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "[waterway]='canal' and not ([tunnel] = 'yes' or [tunnel] ='true' or [disused]='yes')"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#b5d0d0"), 11);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Book", 9, mapnik::color("#6699cc"));
      textSymb0.set_label_placement(mapnik::LINE_PLACEMENT);
      textSymb0.set_halo_radius(1);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[waterway]='derelict_canal' or ([waterway]='canal' and [disused]='yes')"));
      rule.set_max_scale(static_cast<unsigned long long>(250000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#b5e4d0"), 1.5);
        stroke.set_opacity (0.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        stroke.add_dash(4, 4);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[waterway]='derelict_canal' or ([waterway]='canal' and [disused]='yes')"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#b5e4d0"), 2.5);
        stroke.set_opacity (0.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        stroke.add_dash(4, 6);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Book", 9, mapnik::color("#80d1ae"));
      textSymb0.set_label_placement(mapnik::LINE_PLACEMENT);
      textSymb0.set_halo_radius(1);
      textSymb0.set_label_spacing(600);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[waterway]='derelict_canal' or ([waterway]='canal' and [disused]='yes')"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#b5e4d0"), 4.5);
        stroke.set_opacity (0.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        stroke.add_dash(4, 8);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Book", 10, mapnik::color("#80d1ae"));
      textSymb0.set_label_placement(mapnik::LINE_PLACEMENT);
      textSymb0.set_halo_radius(1);
      textSymb0.set_label_spacing(600);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([waterway]='stream' or [waterway]='drain') and ([tunnel]='yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#b5d0d0"), 2.2);
        stroke.add_dash(4, 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("white"), 1.4);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_else(true);
      rule.set_max_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#b5d0d0"), 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Book", 8, mapnik::color("#6699cc"));
      textSymb0.set_label_placement(mapnik::LINE_PLACEMENT);
      textSymb0.set_halo_radius(1);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
    mapnikMap.insert_style("water_lines", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(10000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#777"), 1);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(10000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#777"), 1.5);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
    mapnikMap.insert_style("power", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("grey"), 4);
        stroke.set_opacity (0.8);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(10000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("grey"), 6);
        stroke.set_opacity (0.8);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(10000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("grey"), 9);
        stroke.set_opacity (0.8);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
    mapnikMap.insert_style("citywalls", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
        {
        mapnik::point_symbolizer geom = mapnik::point_symbolizer();
        rule.append(geom);
        }
      style.add_rule(rule);
      }
    mapnikMap.insert_style("power_towers", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'motorway' or [highway]='motorway_link') and ([tunnel] = 'yes' or [tunnel] ='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(200000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#506077"), 3);
        stroke.add_dash(4, 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'motorway' or [highway]='motorway_link') and ([tunnel] = 'yes' or [tunnel] ='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#506077"), 6.5);
        stroke.add_dash(4, 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'motorway' or [highway]='motorway_link') and ([tunnel] = 'yes' or [tunnel] ='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#506077"), 10);
        stroke.add_dash(4, 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'motorway' or [highway]='motorway_link') and ([tunnel] = 'yes' or [tunnel] ='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#506077"), 13);
        stroke.add_dash(4, 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'trunk' or [highway] = 'trunk_link') and ([tunnel] = 'yes' or [tunnel] ='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(200000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#477147"), 4);
        stroke.add_dash(4, 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'trunk' or [highway] = 'trunk_link') and ([tunnel] = 'yes' or [tunnel] ='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#477147"), 8);
        stroke.add_dash(4, 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'trunk' or [highway] = 'trunk_link') and ([tunnel] = 'yes' or [tunnel] ='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#477147"), 11);
        stroke.add_dash(4, 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'trunk' or [highway] = 'trunk_link') and ([tunnel] = 'yes' or [tunnel] ='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#477147"), 14);
        stroke.add_dash(4, 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'primary' or [highway] = 'primary_link') and ([tunnel] = 'yes' or [tunnel] ='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(200000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#8d4346"), 4);
        stroke.add_dash(4, 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'primary' or [highway] = 'primary_link') and ([tunnel] = 'yes' or [tunnel] ='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#8d4346"), 8);
        stroke.add_dash(4, 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'primary' or [highway] = 'primary_link') and ([tunnel] = 'yes' or [tunnel] ='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#8d4346"), 11);
        stroke.add_dash(4, 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'primary' or [highway] = 'primary_link') and ([tunnel] = 'yes' or [tunnel] ='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#8d4346"), 14);
        stroke.add_dash(4, 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'secondary' or [highway] = 'secondary_link') and ([tunnel]='yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(150000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(75000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#a37b48"), 4);
        stroke.add_dash(4, 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'secondary' or [highway] = 'secondary_link') and ([tunnel]='yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(75000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#a37b48"), 10);
        stroke.add_dash(4, 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'secondary' or [highway] = 'secondary_link') and ([tunnel]='yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#a37b48"), 12);
        stroke.add_dash(4, 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'secondary' or [highway] = 'secondary_link') and ([tunnel]='yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#a37b48"), 17);
        stroke.add_dash(4, 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
    mapnikMap.insert_style("tunnels-casing", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'motorway' or [highway]='motorway_link') and ([tunnel] = 'yes' or [tunnel] ='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(200000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#d6dfea"), 2);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'motorway' or [highway] = 'motorway_link') and ([tunnel] = 'yes' or [tunnel] ='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#d6dfea"), 5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'motorway' or [highway] = 'motorway_link') and ([tunnel] = 'yes' or [tunnel] ='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#d6dfea"), 8.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'motorway' or [highway] = 'motorway_link')  and ([tunnel] = 'yes' or [tunnel] ='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#d6dfea"), 11);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'trunk' or [highway] = 'trunk_link') and ([tunnel] = 'yes' or [tunnel] ='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(200000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#cdeacd"), 2.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'trunk' or [highway] = 'trunk_link') and ([tunnel] = 'yes' or [tunnel] ='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#cdeacd"), 6.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'trunk' or [highway] = 'trunk_link') and ([tunnel] = 'yes' or [tunnel] ='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#cdeacd"), 9);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'trunk' or [highway] = 'trunk_link') and ([tunnel] = 'yes' or [tunnel] ='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#cdeacd"), 12);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'primary' or [highway] = 'primary_link') and ([tunnel] = 'yes' or [tunnel] ='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(500000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#f4c3c4"), 2.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'primary' or [highway] = 'primary_link') and ([tunnel] = 'yes' or [tunnel] ='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#f4c3c4"), 6.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'primary' or [highway] = 'primary_link') and ([tunnel] = 'yes' or [tunnel] ='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#f4c3c4"), 9);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'primary' or [highway] = 'primary_link') and ([tunnel] = 'yes' or [tunnel] ='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#f4c3c4"), 12);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'secondary' or [highway] = 'secondary_link') and ([tunnel]='yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(150000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(75000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#fee0b8"), 2);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'secondary' or [highway] = 'secondary_link') and ([tunnel]='yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(75000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#fee0b8"), 8);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'secondary' or [highway] = 'secondary_link') and ([tunnel]='yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#fee0b8"), 10);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'secondary' or [highway] = 'secondary_link') and ([tunnel]='yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#fee0b8"), 14);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
    mapnikMap.insert_style("tunnels-fill", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway]='motorway_link' and not ([tunnel] = 'yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(200000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#506077"), 1.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway]='motorway_link' and not ([tunnel] = 'yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#506077"), 4.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway]='motorway_link' and not ([tunnel] = 'yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#506077"), 8);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway]='motorway_link' and not ([tunnel] = 'yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#506077"), 11);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "[highway] = 'trunk_link' and not ([tunnel] = 'yes' or [tunnel] = 'true' or [tunnel] = '1')"));
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
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "[highway] = 'trunk_link' and not ([tunnel] = 'yes' or [tunnel] = 'true' or [tunnel] = '1')"));
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
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "[highway] = 'trunk_link' and not ([tunnel] = 'yes' or [tunnel] = 'true' or [tunnel] = '1')"));
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
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "[highway] = 'trunk_link' and not ([tunnel] = 'yes' or [tunnel] = 'true' or [tunnel] = '1')"));
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
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "[highway] = 'primary_link' and not ([tunnel] = 'yes' or [tunnel] = 'true' or [tunnel] = '1')"));
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
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "[highway] = 'primary_link' and not ([tunnel] = 'yes' or [tunnel] = 'true' or [tunnel] = '1')"));
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
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "[highway] = 'primary_link' and not ([tunnel] = 'yes' or [tunnel] = 'true' or [tunnel] = '1')"));
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
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "[highway] = 'primary_link' and not ([tunnel] = 'yes' or [tunnel] = 'true' or [tunnel] = '1')"));
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
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "[highway] = 'secondary_link' and not ([tunnel] = 'yes' or [tunnel] = 'true' or [tunnel] = '1')"));
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
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "[highway] = 'secondary_link' and not ([tunnel] = 'yes' or [tunnel] = 'true' or [tunnel] = '1')"));
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
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "[highway] = 'secondary_link' and not ([tunnel] = 'yes' or [tunnel] = 'true' or [tunnel] = '1')"));
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
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "[highway] = 'secondary_link' and not ([tunnel] = 'yes' or [tunnel] = 'true' or [tunnel] = '1')"));
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
    mapnikMap.insert_style("minor-roads-casing-links", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'motorway' and not ([tunnel] = 'yes' or [tunnel]='true')"));
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
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
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
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
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
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
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
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'motorway' and not ([tunnel] = 'yes' or [tunnel]='true')"));
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
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
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
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
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
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
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
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'motorway' and not ([tunnel] = 'yes' or [tunnel]='true')"));
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
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
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
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
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
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
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
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'motorway' and not ([tunnel] = 'yes' or [tunnel]='true')"));
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
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
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
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
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
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
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
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'tertiary' and not ([tunnel]='yes' or [tunnel]='true')"));
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
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'tertiary' and ([tunnel]='yes' or [tunnel]='true')"));
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
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'residential' or [highway] = 'minor' or [highway] = 'unclassified' or [highway] = 'road') and not ([tunnel]='yes' or [tunnel]='true')"));
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
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'residential' or [highway] = 'minor' or [highway] = 'unclassified' or [highway] = 'road') and ([tunnel]='yes' or [tunnel]='true')"));
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
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'tertiary' and not ([tunnel]='yes' or [tunnel]='true')"));
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
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'tertiary' and ([tunnel]='yes' or [tunnel]='true')"));
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
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'residential' or [highway] = 'minor' or [highway] = 'unclassified' or [highway] = 'road') and not ([tunnel]='yes' or [tunnel]='true')"));
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
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'residential' or [highway] = 'minor' or [highway] = 'unclassified' or [highway] = 'road') and ([tunnel]='yes' or [tunnel]='true')"));
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
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'tertiary' or [highway] = 'residential' or [highway] = 'minor' or [highway] = 'unclassified' or [highway] = 'road') and not ([tunnel]='yes' or [tunnel]='true')"));
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
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'tertiary' or [highway] = 'residential' or [highway] = 'minor' or [highway] = 'unclassified' or [highway] = 'road') and ([tunnel]='yes' or [tunnel]='true')"));
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
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'tertiary' or [highway] = 'residential' or [highway] = 'minor' or [highway] = 'unclassified' or [highway] = 'road') and not ([tunnel]='yes' or [tunnel]='true')"));
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
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'tertiary' or [highway] = 'residential' or [highway] = 'minor' or [highway] = 'unclassified' or [highway] = 'road') and ([tunnel]='yes' or [tunnel]='true')"));
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
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "[highway] = 'service' and not ([service]='parking_aisle' or [tunnel]='yes' or [tunnel]='true')"));
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
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'service' and ([tunnel]='yes' or [tunnel]='true')"));
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
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "[highway] = 'service' and not ([service]='parking_aisle' or[tunnel]='yes' or [tunnel]='true')"));
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
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'service' and ([tunnel]='yes' or [tunnel]='true')"));
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
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'service' and [service]='parking_aisle'"));
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
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'pedestrian' and not ([tunnel]='yes' or [tunnel]='true')"));
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
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'pedestrian' and ([tunnel]='yes' or [tunnel]='true')"));
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
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'pedestrian' and not ([tunnel]='yes' or [tunnel]='true')"));
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
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'pedestrian' and ([tunnel]='yes' or [tunnel]='true')"));
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
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'pedestrian' and not ([tunnel]='yes' or [tunnel]='true')"));
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
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'pedestrian' and ([tunnel]='yes' or [tunnel]='true')"));
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
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'pedestrian' and not ([tunnel]='yes' or [tunnel]='true')"));
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
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'pedestrian' and ([tunnel]='yes' or [tunnel]='true')"));
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
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway]='motorway_link' and not ([tunnel] = 'yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(200000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#809bc0"), 0.5);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'motorway_link' and not ([tunnel] = 'yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#809bc0"), 3);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'motorway_link' and not ([tunnel] = 'yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#809bc0"), 6.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'motorway_link' and not ([tunnel] = 'yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#809bc0"), 9);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'trunk_link' and not ([tunnel] = 'yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(200000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#a9dba9"), 2.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'trunk_link' and not ([tunnel] = 'yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#a9dba9"), 7);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'trunk_link' and not ([tunnel] = 'yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#a9dba9"), 11);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'trunk_link' and not ([tunnel] = 'yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#a9dba9"), 15.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'primary_link' and not ([tunnel] = 'yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(500000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#ec989a"), 2.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'primary_link' and not ([tunnel] = 'yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#ec989a"), 7);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'primary_link' and not ([tunnel] = 'yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#ec989a"), 11);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'primary_link' and not ([tunnel] = 'yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#ec989a"), 15.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'secondary_link' and not ([tunnel]='yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(150000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(75000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#fed7a5"), 2);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'secondary_link' and not ([tunnel]='yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(75000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#fed7a5"), 8);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'secondary_link' and not ([tunnel]='yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#fed7a5"), 11);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'secondary_link' and not ([tunnel]='yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#fed7a5"), 15.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
    mapnikMap.insert_style("minor-roads-fill-links", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(200000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'proposed' or [highway]='construction') and not [construction] <>''"));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#9cc"), 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("white"), 2);
        stroke.add_dash(4, 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(200000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'proposed' or [highway]='construction') and ([construction]='motorway' or [construction]='motorway_link')"));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#809bc0"), 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("white"), 2);
        stroke.add_dash(4, 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(200000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'proposed' or [highway]='construction') and ([construction]='trunk' or [construction]='trunk_link')"));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#a9dba9"), 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("white"), 2);
        stroke.add_dash(4, 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(200000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'proposed' or [highway]='construction') and ([construction]='primary' or [construction]='primary_link')"));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#ec989a"), 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("white"), 2);
        stroke.add_dash(4, 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(200000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'proposed' or [highway]='construction') and ([construction]='secondary' or [construction]='secondary_link')"));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#fed7a5"), 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("white"), 2);
        stroke.add_dash(4, 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(10000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'proposed' or [highway]='construction') and not [construction] <>''"));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#9cc"), 4);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("white"), 3.5);
        stroke.add_dash(6, 4);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(10000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'proposed' or [highway]='construction') and ([construction]='motorway' or [construction]='motorway_link')"));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#809bc0"), 4);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("white"), 3.5);
        stroke.add_dash(6, 4);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(10000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'proposed' or [highway]='construction') and ([construction]='trunk' or [construction]='trunk_link')"));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#a9dba9"), 4);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("white"), 3.5);
        stroke.add_dash(6, 4);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(10000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'proposed' or [highway]='construction') and ([construction]='primary' or [construction]='primary_link')"));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#ec989a"), 4);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("white"), 3.5);
        stroke.add_dash(6, 4);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(10000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'proposed' or [highway]='construction') and ([construction]='secondary' or [construction]='secondary_link')"));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#fed7a5"), 4);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("white"), 3.5);
        stroke.add_dash(6, 4);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(10000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'proposed' or [highway]='construction') and ([construction]='tertiary' or [construction]='tertiary_link')"));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#ffffb3"), 4);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("white"), 3.5);
        stroke.add_dash(6, 4);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(10000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'proposed' or [highway]='construction') and ([construction]='residential' or [construction]='unclassified' or [construction]='living_street' or [construction]='service')"));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#aaa"), 4);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("white"), 3.5);
        stroke.add_dash(6, 4);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(10000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'proposed' or [highway]='construction') and not [construction] <>''"));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#9cc"), 8);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("white"), 7);
        stroke.add_dash(8, 6);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(10000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'proposed' or [highway]='construction') and ([construction]='motorway' or [construction]='motorway_link')"));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#809bc0"), 8);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("white"), 7);
        stroke.add_dash(8, 6);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(10000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'proposed' or [highway]='construction') and ([construction]='trunk' or [construction]='trunk_link')"));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#a9dba9"), 8);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("white"), 7);
        stroke.add_dash(8, 6);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(10000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'proposed' or [highway]='construction') and ([construction]='primary' or [construction]='primary_link')"));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#ec989a"), 8);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("white"), 7);
        stroke.add_dash(8, 6);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(10000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'proposed' or [highway]='construction') and ([construction]='secondary' or [construction]='secondary_link')"));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#fed7a5"), 8);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("white"), 7);
        stroke.add_dash(8, 6);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(10000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'proposed' or [highway]='construction') and ([construction]='tertiary' or [construction]='tertiary_link')"));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#ffffb3"), 8);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("white"), 7);
        stroke.add_dash(8, 6);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(10000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'proposed' or [highway]='construction') and ([construction]='residential' or [construction]='unclassified' or [construction]='living_street' or [construction]='service')"));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#aaa"), 8);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("white"), 7);
        stroke.add_dash(8, 6);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'motorway' and not ([tunnel] = 'yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(200000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#809bc0"), 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'motorway' and not ([tunnel] = 'yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#809bc0"), 5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'motorway' and not ([tunnel] = 'yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#809bc0"), 8.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'motorway' and not ([tunnel] = 'yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#809bc0"), 11);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'trunk' and not ([tunnel] = 'yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(200000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#a9dba9"), 2.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'trunk' and not ([tunnel] = 'yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#a9dba9"), 7);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'trunk' and not ([tunnel] = 'yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#a9dba9"), 11);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'trunk' and not ([tunnel] = 'yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#a9dba9"), 15.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'primary' and not ([tunnel] = 'yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(500000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#ec989a"), 2.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'primary' and not ([tunnel] = 'yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#ec989a"), 7);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'primary' and not ([tunnel] = 'yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#ec989a"), 11);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'primary' and not ([tunnel] = 'yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#ec989a"), 15.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'secondary' and not ([tunnel]='yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(150000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(75000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#fed7a5"), 2);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'secondary' and not ([tunnel]='yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(75000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#fed7a5"), 8);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'secondary' and not ([tunnel]='yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#fed7a5"), 11);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'secondary' and not ([tunnel]='yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#fed7a5"), 15.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[railway] = 'rail' and ([tunnel] = 'yes' or [tunnel] = 'true')"));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#ffffff"), 3);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.add_dash(1, 9);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#fdfdfd"), 3);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.add_dash(0, 1);
        stroke.add_dash(1, 8);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#ececec"), 3);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.add_dash(0, 2);
        stroke.add_dash(1, 7);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#cacaca"), 3);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.add_dash(0, 3);
        stroke.add_dash(1, 6);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#afafaf"), 3);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.add_dash(0, 4);
        stroke.add_dash(1, 5);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#a1a1a1"), 3);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.add_dash(0, 5);
        stroke.add_dash(1, 4);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#9b9b9b"), 3);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.add_dash(0, 6);
        stroke.add_dash(1, 3);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter(
                        "([railway] = 'disused' or [railway] = 'abandoned' or [railway]='construction') and not [highway] <> ''"));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("grey"), 2);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.add_dash(2, 4);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter(
                        "([railway] = 'disused' or [railway] = 'abandoned' or [railway]='construction') and not [highway] <> '' and [bridge]='yes'"));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 6);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#fff"), 4.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("grey"), 2);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.add_dash(2, 4);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[railway] = 'rail' and not ([tunnel] = 'yes' or [tunnel] = 'true')"));
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
        stroke.add_dash(8, 12);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[railway] = 'rail' and ([bridge] = 'yes' or [bridge]='true')"));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 7);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("white"), 5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[railway] = 'rail' and not ([tunnel]='yes' or [tunnel] = 'true')"));
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
        stroke.add_dash(0, 11);
        stroke.add_dash(8, 1);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[railway] = 'rail' and ([tunnel] = 'yes' or [tunnel] = 'true')"));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#ffffff"), 3);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.add_dash(1, 9);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#fdfdfd"), 3);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.add_dash(0, 1);
        stroke.add_dash(1, 8);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#ececec"), 3);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.add_dash(0, 2);
        stroke.add_dash(1, 7);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#cacaca"), 3);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.add_dash(0, 3);
        stroke.add_dash(1, 6);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#afafaf"), 3);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.add_dash(0, 4);
        stroke.add_dash(1, 5);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#a1a1a1"), 3);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.add_dash(0, 5);
        stroke.add_dash(1, 4);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#9b9b9b"), 3);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.add_dash(0, 6);
        stroke.add_dash(1, 3);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[railway]='narrow_gauge' and not ([tunnel]='yes' or [tunnel] = 'true')"));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#666"), 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[railway]='narrow_gauge' and ([tunnel]='yes' or [tunnel] = 'true')"));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#666"), 5);
        stroke.add_dash(5, 3);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#fff"), 4);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#aaa"), 1.5);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[railway]='tram' and ([tunnel]='yes' or [tunnel]='true')"));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#444"), 1);
        stroke.add_dash(5, 3);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[railway]='tram' and ([tunnel]='yes' or [tunnel]='true')"));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#444"), 2);
        stroke.add_dash(5, 3);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[railway]='light_rail' and ([tunnel] = 'yes' or [tunnel]='true')"));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#666"), 2);
        stroke.add_dash(5, 3);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[railway]='light_rail' and not ([tunnel] = 'yes' or [tunnel]='true')"));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#666"), 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(200000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[railway]='subway' and ([tunnel] = 'yes' or [tunnel]='true')"));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#999"), 2);
        stroke.add_dash(5, 3);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(200000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[railway]='subway' and not ([tunnel] = 'yes' or [tunnel]='true')"));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#999"), 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "[highway] = 'tertiary' or [highway] = 'residential' or [highway] = 'unclassified' or [highway] = 'road'"));
      rule.set_max_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#bbb"), 1);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'road'"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#ddd"), 2);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'road'"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#ddd"), 3);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'road'"));
      rule.set_max_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#ddd"), 9.4);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'road'"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#ddd"), 13);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "[highway] = 'residential' or [highway] = 'minor' or [highway] = 'unclassified'"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#fff"), 2);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "[highway] = 'residential'  or [highway] = 'minor' or [highway] = 'unclassified'"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(15000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#fff"), 3);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "[highway] = 'residential'  or [highway] = 'minor' or [highway] = 'unclassified'"));
      rule.set_max_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#fff"), 9.4);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "[highway] = 'residential'  or [highway] = 'minor' or [highway] = 'unclassified'"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#fff"), 13);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'living_street'"));
      rule.set_max_scale(static_cast<unsigned long long>(200000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#bbb"), 1.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'living_street'"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(15000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#bbb"), 3);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'living_street'"));
      rule.set_max_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#bbb"), 9.4);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'living_street'"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#bbb"), 13);
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
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#ffffb3"), 4.5);
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
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#ffffb3"), 6);
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
      rule.set_max_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#ffffb3"), 9.4);
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
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#ffffb3"), 13);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'service' and not [service]='parking_aisle'"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#bbbbbb"), 1);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'service' and not [service]='parking_aisle'"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("white"), 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'service' and not [service]='parking_aisle'"));
      rule.set_max_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("white"), 6);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'service' and [service]='parking_aisle'"));
      rule.set_max_scale(static_cast<unsigned long long>(10000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#fff"), 3);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'pedestrian'"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#ededed"), 1.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'pedestrian'"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#ededed"), 3);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'pedestrian'"));
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(10000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#ededed"), 5.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'pedestrian'"));
      rule.set_max_scale(static_cast<unsigned long long>(10000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#ededed"), 8);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'steps'"));
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("salmon"), 5.0);
        stroke.add_dash(2, 1);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'bridleway' or ([highway] = 'path' and [horse] = 'designated')) and not ([tunnel]='yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
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
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("green"), 1.5);
        stroke.add_dash(4, 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'footway' or ([highway] = 'path' and [foot] = 'designated')) and not ([tunnel]='yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#fff"), 6);
        stroke.set_opacity (0.4);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("salmon"), 2.0);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        stroke.add_dash(1, 3);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "[highway] = 'path' and not ([bicycle] = 'designated' or [foot] = 'designated' or [horse] = 'designated') and not ([tunnel] = 'yes' or [tunnel] = 'true' or [tunnel] = '1')"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("white"), 1.0);
        stroke.set_opacity (0.4);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 0.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        stroke.add_dash(6, 3);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'steps'"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#fff"), 6);
        stroke.set_opacity (0.4);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("salmon"), 2.0);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        stroke.add_dash(1, 3);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'cycleway' or ([highway] = 'path' and [bicycle] = 'designated')) and not ([tunnel]='yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("white"), 4);
        stroke.set_opacity (0.4);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("blue"), 1.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        stroke.add_dash(1, 3);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'byway'"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("white"), 6);
        stroke.set_opacity (0.4);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#ffcc00"), 2.0);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        stroke.add_dash(3, 4);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'track'"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("white"), 3);
        stroke.set_opacity (0.4);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#996600"), 1.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        stroke.add_dash(3, 4);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'unsurfaced'"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
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
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#debd9c"), 3);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        stroke.add_dash(2, 4);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'unsurfaced'"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#fff"), 5);
        stroke.set_opacity (0.4);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#debd9c"), 4);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        stroke.add_dash(4, 6);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[route] = 'ferry'"));
      rule.set_max_scale(static_cast<unsigned long long>(5000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(500000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#66f"), 0.4);
        stroke.add_dash(4, 4);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[route] = 'ferry'"));
      rule.set_max_scale(static_cast<unsigned long long>(500000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#66f"), 0.8);
        stroke.add_dash(6, 6);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[aeroway] = 'runway'"));
      rule.set_max_scale(static_cast<unsigned long long>(500000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(200000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#bbc"), 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[aeroway] = 'runway'"));
      rule.set_max_scale(static_cast<unsigned long long>(200000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#bbc"), 7);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[aeroway] = 'runway' and not ([bridge]='yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#bbc"), 18);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[aeroway] = 'taxiway'"));
      rule.set_max_scale(static_cast<unsigned long long>(500000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#bbc"), 1);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[aeroway] = 'taxiway' and not ([bridge]='yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#bbc"), 4);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[aeroway] = 'taxiway' and not ([bridge]='yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#bbc"), 6);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
    mapnikMap.insert_style("minor-roads-fill", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "[access] = 'permissive' and ([highway]='unclassified' or [highway]='residential' or [highway]='service')"));
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#cf9"), 6);
        stroke.set_opacity (0.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        stroke.add_dash(6, 8);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[access] = 'permissive' and [highway]='footway'"));
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#cf9"), 6);
        stroke.set_opacity (0.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        stroke.add_dash(6, 8);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "[access]='destination' and ([highway]='unclassified' or [highway]='residential' or [highway]='service')"));
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#c2e0ff"), 6);
        stroke.set_opacity (0.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        stroke.add_dash(6, 8);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[access] = 'private' or [access] = 'no'"));
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#efa9a9"), 6);
        stroke.set_opacity (0.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        stroke.add_dash(6, 8);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
    mapnikMap.insert_style("access", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[aerialway] = 'cable_car' or [aerialway]='gondola'"));
      rule.set_max_scale(static_cast<unsigned long long>(200000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[aerialway] = 'chair_lift' or [aerialway]='drag_lift'"));
      rule.set_max_scale(static_cast<unsigned long long>(200000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
    mapnikMap.insert_style("aerialways", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'bridleway' or ([highway] = 'path' and [horse] = 'designated')) and ([tunnel]='yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("grey"), 5);
        stroke.add_dash(4, 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#fff"), 3);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("green"), 2);
        stroke.set_opacity (0.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        stroke.add_dash(4, 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'footway' or ([highway] = 'path' and [foot] = 'designated')) and ([tunnel]='yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("grey"), 5.5);
        stroke.add_dash(4, 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#fff"), 3.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("salmon"), 2.5);
        stroke.set_opacity (0.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        stroke.add_dash(1, 3);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "[highway] = 'path' and not ([bicycle] = 'designated' or [foot] = 'designated' or [horse] = 'designated') and ([tunnel] = 'yes' or [tunnel] = 'true' or [tunnel] = '1')"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("grey"), 5.5);
        stroke.add_dash(4, 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("white"), 1.0);
        stroke.set_opacity (0.4);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 0.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        stroke.add_dash(6, 3);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'cycleway' or ([highway] = 'path' and [bicycle] = 'designated')) and ([tunnel]='yes' or [tunnel]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("grey"), 5);
        stroke.add_dash(4, 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("white"), 3);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("blue"), 2);
        stroke.set_opacity (0.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        stroke.add_dash(1, 3);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
    mapnikMap.insert_style("footbikecycle-tunnels", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[tracktype]='grade1'"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("white"), 3.5);
        stroke.set_opacity (0.4);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#b37700"), 2);
        stroke.set_opacity (0.7);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[tracktype]='grade2'"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("white"), 3);
        stroke.set_opacity (0.4);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#a87000"), 1.5);
        stroke.set_opacity (0.8);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        stroke.add_dash(9, 4);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[tracktype]='grade3'"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("white"), 3);
        stroke.set_opacity (0.4);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#996600"), 1.5);
        stroke.set_opacity (0.8);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        stroke.add_dash(3, 4);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[tracktype]='grade4'"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("white"), 3);
        stroke.set_opacity (0.4);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#996600"), 2);
        stroke.set_opacity (0.8);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        stroke.add_dash(4, 7);
        stroke.add_dash(1, 5);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[tracktype]='grade5'"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("white"), 3);
        stroke.set_opacity (0.4);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#996600"), 2);
        stroke.set_opacity (0.8);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        stroke.add_dash(1, 5);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_else(true);
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("white"), 3);
        stroke.set_opacity (0.4);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#996600"), 1.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        stroke.add_dash(3, 4);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
    mapnikMap.insert_style("tracks-notunnel-nobridge", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[tracktype]='grade1'"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 5);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("white"), 3.5);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#b37700"), 2);
        stroke.set_opacity (0.7);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[tracktype]='grade2'"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 4.5);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("white"), 3);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#a87000"), 1.5);
        stroke.set_opacity (0.8);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        stroke.add_dash(3, 4);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[tracktype]='grade3'"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 4.5);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("white"), 3.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#996600"), 2);
        stroke.set_opacity (0.7);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[tracktype]='grade4'"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 4.5);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("white"), 3);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#996600"), 2);
        stroke.set_opacity (0.8);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        stroke.add_dash(4, 7);
        stroke.add_dash(1, 5);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[tracktype]='grade5'"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 4.5);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("white"), 3);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#996600"), 2);
        stroke.set_opacity (0.8);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        stroke.add_dash(1, 5);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_else(true);
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 4.5);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("white"), 3);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#996600"), 1.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        stroke.add_dash(3, 4);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
    mapnikMap.insert_style("tracks-bridges", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[tracktype]='grade1'"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("grey"), 5);
        stroke.add_dash(4, 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("white"), 3.5);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#b37700"), 2);
        stroke.set_opacity (0.5);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[tracktype]='grade2'"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("grey"), 4.5);
        stroke.add_dash(4, 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("white"), 3);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#a87000"), 1.5);
        stroke.set_opacity (0.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        stroke.add_dash(3, 4);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[tracktype]='grade3'"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("grey"), 4.5);
        stroke.add_dash(4, 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("white"), 3.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#996600"), 2);
        stroke.set_opacity (0.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[tracktype]='grade4'"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("grey"), 4.5);
        stroke.add_dash(4, 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("white"), 3);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#996600"), 2);
        stroke.set_opacity (0.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        stroke.add_dash(4, 7);
        stroke.add_dash(1, 5);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[tracktype]='grade5'"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("grey"), 4.5);
        stroke.add_dash(4, 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("white"), 3);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#996600"), 2);
        stroke.set_opacity (0.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        stroke.add_dash(1, 5);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_else(true);
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("grey"), 4.5);
        stroke.add_dash(4, 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("white"), 3);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#996600"), 1.5);
        stroke.set_opacity (0.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        stroke.add_dash(3, 4);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
    mapnikMap.insert_style("tracks-tunnels", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'motorway' or [highway]='motorway_link') and ([bridge] = 'yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(200000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#506077"), 3);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'trunk' or [highway]='trunk_link') and ([bridge] = 'yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(200000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#477147"), 4);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'trunk' or [highway]='trunk_link') and ([bridge] = 'yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 8);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'trunk' or [highway]='trunk_link') and ([bridge] = 'yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 11);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'trunk' or [highway]='trunk_link') and ([bridge] = 'yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 16);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'primary' or [highway]='primary_link') and ([bridge] = 'yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(200000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#8d4346"), 4);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'secondary' or [highway]='secondary_link') and ([bridge] = 'yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(75000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 10);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'secondary' or [highway]='secondary_link') and ([bridge] = 'yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 12);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'secondary' or [highway]='secondary_link') and ([bridge] = 'yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 16);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'tertiary'  and ([bridge] = 'yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 7.5);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'tertiary'  and ([bridge] = 'yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 11);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'tertiary'  and ([bridge] = 'yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 16);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'residential' or [highway] = 'minor' or [highway] = 'unclassified' or [highway] = 'road') and ([bridge] = 'yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 4.5);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'residential' or [highway] = 'minor' or [highway] = 'unclassified' or [highway] = 'road') and ([bridge] = 'yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 11);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'residential' or [highway] = 'minor' or [highway] = 'unclassified' or [highway] = 'road') and ([bridge] = 'yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 16);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'service'  and ([bridge] = 'yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 3);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'service'  and ([bridge] = 'yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 8);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'pedestrian' and ([bridge] = 'yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 2.2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'pedestrian'  and ([bridge] = 'yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 3.8);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'pedestrian'  and ([bridge] = 'yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(10000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 7);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'pedestrian'  and ([bridge] = 'yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(10000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 9.5);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
    mapnikMap.insert_style("road-bridges-casing", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'motorway' or [highway]='motorway_link') and ([bridge] = 'yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(200000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#809bc0"), 2);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'trunk' or [highway]='trunk_link') and ([bridge] = 'yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(200000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#a9dba9"), 3);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'trunk' or [highway]='trunk_link') and ([bridge] = 'yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#a9dba9"), 7);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'trunk' or [highway]='trunk_link') and ([bridge] = 'yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#a9dba9"), 9.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'trunk' or [highway]='trunk_link') and ([bridge] = 'yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#a9dba9"), 14.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'primary' or [highway]='primary_link') and ([bridge] = 'yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(200000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#ec989a"), 3);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'secondary' or [highway] = 'secondary_link') and ([bridge]='yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(75000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#fed7a5"), 9);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'secondary' or [highway] = 'secondary_link') and ([bridge]='yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#fed7a5"), 10.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'secondary' or [highway] = 'secondary_link') and ([bridge]='yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#fed7a5"), 14.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'tertiary' and ([bridge]='yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#ffffb3"), 6);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'tertiary' and ([bridge]='yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#ffffb3"), 9.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'tertiary' and ([bridge]='yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#ffffb3"), 14);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'road' and ([bridge] = 'yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#ddd"), 3.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'road' and ([bridge] = 'yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#ddd"), 9.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'road' and ([bridge] = 'yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#ddd"), 14);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'residential' or [highway] = 'minor' or [highway] = 'unclassified') and ([bridge] = 'yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("white"), 3.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'residential' or [highway] = 'minor' or [highway] = 'unclassified') and ([bridge] = 'yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("white"), 9.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'residential' or [highway] = 'minor' or [highway] = 'unclassified') and ([bridge] = 'yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("white"), 14);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'service' and ([bridge] = 'yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("white"), 2);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'service' and ([bridge] = 'yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("white"), 6);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'pedestrian' and ([bridge] = 'yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#ededed"), 1.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'pedestrian' and ([bridge] = 'yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#ededed"), 3);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'pedestrian' and ([bridge] = 'yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(10000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#ededed"), 5.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'pedestrian' and ([bridge] = 'yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(10000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#ededed"), 8);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
    mapnikMap.insert_style("road-bridges-fill", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[waterway]='canal' and ([bridge] = 'yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#000"), 7);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#b5d0d0"), 6);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Book", 9, mapnik::color("#6699cc"));
      textSymb0.set_label_placement(mapnik::LINE_PLACEMENT);
      textSymb0.set_halo_radius(1);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[waterway]='canal' and ([bridge] = 'yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#000"), 11);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#b5d0d0"), 10);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Book", 9, mapnik::color("#6699cc"));
      textSymb0.set_label_placement(mapnik::LINE_PLACEMENT);
      textSymb0.set_halo_radius(1);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
    mapnikMap.insert_style("waterway-bridges", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[aeroway] = 'runway' and ([bridge]='yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 19);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#bbc"), 18);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[aeroway] = 'taxiway' and ([bridge]='yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 5);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#bbc"), 4);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[aeroway] = 'taxiway' and ([bridge]='yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 7);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#bbc"), 6);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[railway]='subway' and ([bridge] = 'yes' or [bridge]='true')"));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 5.5);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("white"), 4);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#999"), 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[railway]='light_rail' and ([bridge]='yes' or [bridge]='true')"));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#555"), 5.5);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("white"), 4);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#666"), 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'unsurfaced' and ([bridge]='yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 5);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#fff"), 4);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#debd9c"), 3);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        stroke.add_dash(2, 4);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'unsurfaced' and ([bridge]='yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 6.5);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#fff"), 5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#debd9c"), 4);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        stroke.add_dash(4, 6);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'bridleway' or ([highway] = 'path' and [horse] = 'designated')) and ([bridge]='yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 5.5);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#fff"), 4);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("green"), 1.5);
        stroke.add_dash(4, 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'footway' or ([highway] = 'path' and [foot] = 'designated')) and ([bridge]='yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 6);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#fff"), 4.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("salmon"), 2.0);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        stroke.add_dash(1, 3);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "[highway] = 'path' and not ([bicycle] = 'designated' or [foot] = 'designated' or [horse] = 'designated') and ([bridge] = 'yes' or [bridge] = 'true' or [bridge] = '1')"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 4);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("white"), 2.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 0.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        stroke.add_dash(6, 3);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'cycleway' or ([highway] = 'path' and [bicycle] = 'designated')) and ([bridge]='yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 5.5);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("white"), 4);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("blue"), 1.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        stroke.add_dash(1, 3);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'byway' and ([bridge]='yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 5.5);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("white"), 4);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#ffcc00"), 1.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        stroke.add_dash(3, 4);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
    mapnikMap.insert_style("noncased-ways-bridges", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'motorway' or [highway]='motorway_link') and ([bridge] = 'yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 6.5);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'motorway' or [highway]='motorway_link') and ([bridge] = 'yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 9);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'motorway' or [highway]='motorway_link') and ([bridge] = 'yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 12);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
    mapnikMap.insert_style("mwaybridge_layer0_casing", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'motorway' or [highway]='motorway_link') and ([bridge] = 'yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#809bc0"), 5.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'motorway' or [highway]='motorway_link') and ([bridge] = 'yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#809bc0"), 7.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'motorway' or [highway]='motorway_link') and ([bridge] = 'yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#809bc0"), 10);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
    mapnikMap.insert_style("mwaybridge_layer0_fill", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'motorway' or [highway]='motorway_link') and ([bridge] = 'yes' or [bridge]='true') and [layer]='1'"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 6.5);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'motorway' or [highway]='motorway_link') and ([bridge] = 'yes' or [bridge]='true') and [layer]='1'"));
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 9);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'motorway' or [highway]='motorway_link') and ([bridge] = 'yes' or [bridge]='true') and [layer]='1'"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 12);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
    mapnikMap.insert_style("mwaybridge_layer1_casing", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'motorway' or [highway]='motorway_link') and ([bridge] = 'yes' or [bridge]='true') and [layer]='1'"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#809bc0"), 5.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'motorway' or [highway]='motorway_link') and ([bridge] = 'yes' or [bridge]='true') and [layer]='1'"));
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#809bc0"), 7.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'motorway' or [highway]='motorway_link') and ([bridge] = 'yes' or [bridge]='true') and [layer]='1'"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#809bc0"), 10);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
    mapnikMap.insert_style("mwaybridge_layer1_fill", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'motorway' or [highway]='motorway_link') and ([bridge] = 'yes' or [bridge]='true') and [layer]='2'"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 6.5);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'motorway' or [highway]='motorway_link') and ([bridge] = 'yes' or [bridge]='true') and [layer]='2'"));
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 9);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'motorway' or [highway]='motorway_link') and ([bridge] = 'yes' or [bridge]='true') and [layer]='2'"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 12);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
    mapnikMap.insert_style("mwaybridge_layer2_casing", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'motorway' or [highway]='motorway_link') and ([bridge] = 'yes' or [bridge]='true') and [layer]='2'"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#809bc0"), 5.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'motorway' or [highway]='motorway_link') and ([bridge] = 'yes' or [bridge]='true') and [layer]='2'"));
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#809bc0"), 7.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'motorway' or [highway]='motorway_link') and ([bridge] = 'yes' or [bridge]='true') and [layer]='2'"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#809bc0"), 10);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
    mapnikMap.insert_style("mwaybridge_layer2_fill", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'motorway' or [highway]='motorway_link') and ([bridge] = 'yes' or [bridge]='true') and [layer]='3'"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 6.5);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'motorway' or [highway]='motorway_link') and ([bridge] = 'yes' or [bridge]='true') and [layer]='3'"));
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 9);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'motorway' or [highway]='motorway_link') and ([bridge] = 'yes' or [bridge]='true') and [layer]='3'"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 12);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
    mapnikMap.insert_style("mwaybridge_layer3_casing", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'motorway' or [highway]='motorway_link') and ([bridge] = 'yes' or [bridge]='true') and [layer]='3'"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#809bc0"), 5.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'motorway' or [highway]='motorway_link') and ([bridge] = 'yes' or [bridge]='true') and [layer]='3'"));
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#809bc0"), 7.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'motorway' or [highway]='motorway_link') and ([bridge] = 'yes' or [bridge]='true') and [layer]='3'"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#809bc0"), 10);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
    mapnikMap.insert_style("mwaybridge_layer3_fill", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'motorway' or [highway]='motorway_link') and ([bridge] = 'yes' or [bridge]='true') and [layer]='4'"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 6.5);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'motorway' or [highway]='motorway_link') and ([bridge] = 'yes' or [bridge]='true') and [layer]='4'"));
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 9);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'motorway' or [highway]='motorway_link') and ([bridge] = 'yes' or [bridge]='true') and [layer]='4'"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 12);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
    mapnikMap.insert_style("mwaybridge_layer4_casing", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'motorway' or [highway]='motorway_link') and ([bridge] = 'yes' or [bridge]='true') and [layer]='4'"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#809bc0"), 5.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'motorway' or [highway]='motorway_link') and ([bridge] = 'yes' or [bridge]='true') and [layer]='4'"));
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#809bc0"), 7.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'motorway' or [highway]='motorway_link') and ([bridge] = 'yes' or [bridge]='true') and [layer]='4'"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#809bc0"), 10);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
    mapnikMap.insert_style("mwaybridge_layer4_fill", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'motorway' or [highway]='motorway_link') and ([bridge] = 'yes' or [bridge]='true') and [layer]='5'"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 6.5);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'motorway' or [highway]='motorway_link') and ([bridge] = 'yes' or [bridge]='true') and [layer]='5'"));
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 9);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'motorway' or [highway]='motorway_link') and ([bridge] = 'yes' or [bridge]='true') and [layer]='5'"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 12);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
    mapnikMap.insert_style("mwaybridge_layer5_casing", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'motorway' or [highway]='motorway_link') and ([bridge] = 'yes' or [bridge]='true') and [layer]='5'"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#809bc0"), 5.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'motorway' or [highway]='motorway_link') and ([bridge] = 'yes' or [bridge]='true') and [layer]='5'"));
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#809bc0"), 7.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'motorway' or [highway]='motorway_link') and ([bridge] = 'yes' or [bridge]='true') and [layer]='5'"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#809bc0"), 10);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
    mapnikMap.insert_style("mwaybridge_layer5_fill", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'primary' or [highway]='primary_link') and ([bridge] = 'yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 8);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'primary' or [highway]='primary_link') and ([bridge] = 'yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 11);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'primary' or [highway]='primary_link') and ([bridge] = 'yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 16);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
    mapnikMap.insert_style("primarybridge_layer0_casing", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'primary' or [highway]='primary_link') and ([bridge] = 'yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#ec989a"), 7);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'primary' or [highway]='primary_link') and ([bridge] = 'yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#ec989a"), 9.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'primary' or [highway]='primary_link') and ([bridge] = 'yes' or [bridge]='true')"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#ec989a"), 14.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
    mapnikMap.insert_style("primarybridge_layer0_fill", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'primary' or [highway]='primary_link') and ([bridge] = 'yes' or [bridge]='true') and [layer]='1'"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 8);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'primary' or [highway]='primary_link') and ([bridge] = 'yes' or [bridge]='true') and [layer]='1'"));
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 11);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'primary' or [highway]='primary_link') and ([bridge] = 'yes' or [bridge]='true') and [layer]='1'"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 16);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
    mapnikMap.insert_style("primarybridge_layer1_casing", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'primary' or [highway]='primary_link') and ([bridge] = 'yes' or [bridge]='true') and [layer]='1'"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#ec989a"), 7);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'primary' or [highway]='primary_link') and ([bridge] = 'yes' or [bridge]='true') and [layer]='1'"));
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#ec989a"), 9.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'primary' or [highway]='primary_link') and ([bridge] = 'yes' or [bridge]='true') and [layer]='1'"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#ec989a"), 14.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
    mapnikMap.insert_style("primarybridge_layer1_fill", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'primary' or [highway]='primary_link') and ([bridge] = 'yes' or [bridge]='true') and [layer]='2'"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 8);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'primary' or [highway]='primary_link') and ([bridge] = 'yes' or [bridge]='true') and [layer]='2'"));
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 11);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'primary' or [highway]='primary_link') and ([bridge] = 'yes' or [bridge]='true') and [layer]='2'"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 16);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
    mapnikMap.insert_style("primarybridge_layer2_casing", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'primary' or [highway]='primary_link') and ([bridge] = 'yes' or [bridge]='true') and [layer]='2'"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#ec989a"), 7);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'primary' or [highway]='primary_link') and ([bridge] = 'yes' or [bridge]='true') and [layer]='2'"));
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#ec989a"), 9.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter(
                        "([highway] = 'primary' or [highway]='primary_link') and ([bridge] = 'yes' or [bridge]='true') and [layer]='2'"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#ec989a"), 14.5);
        stroke.set_line_join(mapnik::ROUND_JOIN);
        stroke.set_line_cap(mapnik::ROUND_CAP);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
    mapnikMap.insert_style("primarybridge_layer2_fill", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'motorway' or [highway] = 'motorway_link'"));
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
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'motorway' or [highway] = 'motorway_link'"));
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
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'motorway' or [highway] = 'motorway_link'"));
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
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'motorway' or [highway] = 'motorway_link'"));
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
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'motorway' or [highway] = 'motorway_link'"));
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
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'trunk' or [highway] = 'trunk_link'"));
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
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'trunk' or [highway] = 'trunk_link'"));
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
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'trunk' or [highway] = 'trunk_link'"));
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
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'trunk' or [highway] = 'trunk_link'"));
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
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'primary' or [highway] = 'primary_link'"));
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
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'primary' or [highway] = 'primary_link'"));
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
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'primary' or [highway] = 'primary_link'"));
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
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'primary' or [highway] = 'primary_link'"));
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
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'secondary' or [highway] = 'secondary_link'"));
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
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'secondary' or [highway] = 'secondary_link'"));
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
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(10000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(2000000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[railway] = 'rail'"));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#aaa"), 0.6);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(2000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[railway] = 'rail' and not ([tunnel]='yes' or [tunnel]='true')"));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#aaa"), 1);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(2000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[railway] = 'rail' and ([tunnel]='yes' or [tunnel]='true')"));
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
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(2500000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter(
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
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[railway] = 'rail' and not ([tunnel]='yes' or [tunnel]='true')"));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#aaa"), 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[railway] = 'rail' and ([tunnel]='yes' or [tunnel]='true')"));
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
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter(
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
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(200000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[railway] = 'preserved'"));
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
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[railway] = 'preserved'"));
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
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[railway]='monorail'"));
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
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[railway]='tram' and not ([tunnel]='yes' or [tunnel]='true')"));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#444"), 1);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[railway]='tram' and not ([tunnel]='yes' or [tunnel]='true')"));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#444"), 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
    mapnikMap.insert_style("trams", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'motorway' and [length] = 1"));
      rule.set_max_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'motorway' and [length] = 2"));
      rule.set_max_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'motorway' and [length] = 3"));
      rule.set_max_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'motorway' and [length] = 4"));
      rule.set_max_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'motorway' and [length] = 5"));
      rule.set_max_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'motorway' and [length] = 6"));
      rule.set_max_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'motorway' and [length] = 7"));
      rule.set_max_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'motorway' and [length] = 8"));
      rule.set_max_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'trunk' and [length] = 1"));
      rule.set_max_scale(static_cast<unsigned long long>(500000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'trunk' and [length] = 2"));
      rule.set_max_scale(static_cast<unsigned long long>(500000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'trunk' and [length] = 3"));
      rule.set_max_scale(static_cast<unsigned long long>(500000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'trunk' and [length] = 4"));
      rule.set_max_scale(static_cast<unsigned long long>(500000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'trunk' and [length] = 5"));
      rule.set_max_scale(static_cast<unsigned long long>(500000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'trunk' and [length] = 6"));
      rule.set_max_scale(static_cast<unsigned long long>(500000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'trunk' and [length] = 7"));
      rule.set_max_scale(static_cast<unsigned long long>(500000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'trunk' and [length] = 8"));
      rule.set_max_scale(static_cast<unsigned long long>(500000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'primary' and [length] = 1"));
      rule.set_max_scale(static_cast<unsigned long long>(200000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'primary' and [length] = 2"));
      rule.set_max_scale(static_cast<unsigned long long>(200000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'primary' and [length] = 3"));
      rule.set_max_scale(static_cast<unsigned long long>(200000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'primary' and [length] = 4"));
      rule.set_max_scale(static_cast<unsigned long long>(200000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'primary' and [length] = 5"));
      rule.set_max_scale(static_cast<unsigned long long>(200000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'primary' and [length] = 6"));
      rule.set_max_scale(static_cast<unsigned long long>(200000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'primary' and [length] = 7"));
      rule.set_max_scale(static_cast<unsigned long long>(200000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'primary' and [length] = 8"));
      rule.set_max_scale(static_cast<unsigned long long>(200000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'secondary' and [length] = 1"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'secondary' and [length] = 2"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'secondary' and [length] = 3"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'secondary' and [length] = 4"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'secondary' and [length] = 5"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'secondary' and [length] = 6"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'secondary' and [length] = 7"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'secondary' and [length] = 8"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'tertiary' and [length] = 1"));
      rule.set_max_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'tertiary' and [length] = 2"));
      rule.set_max_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'tertiary' and [length] = 3"));
      rule.set_max_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'tertiary' and [length] = 4"));
      rule.set_max_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'tertiary' and [length] = 5"));
      rule.set_max_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'tertiary' and [length] = 6"));
      rule.set_max_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'tertiary' and [length] = 7"));
      rule.set_max_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'tertiary' and [length] = 8"));
      rule.set_max_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'trunk' or [highway] = 'primary'"));
      rule.set_max_scale(static_cast<unsigned long long>(75000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Book", 8, mapnik::color("black"));
      textSymb0.set_label_placement(mapnik::LINE_PLACEMENT);
      textSymb0.set_halo_radius(0);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'secondary'"));
      rule.set_max_scale(static_cast<unsigned long long>(75000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Book", 8, mapnik::color("black"));
      textSymb0.set_label_placement(mapnik::LINE_PLACEMENT);
      textSymb0.set_halo_radius(1);
      textSymb0.set_halo_fill(mapnik::color("#fed7a5"));
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'trunk' or [highway] = 'primary'"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Book", 9, mapnik::color("black"));
      textSymb0.set_label_placement(mapnik::LINE_PLACEMENT);
      textSymb0.set_halo_radius(0);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'trunk' or [highway] = 'primary'"));
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Book", 10, mapnik::color("black"));
      textSymb0.set_label_placement(mapnik::LINE_PLACEMENT);
      textSymb0.set_halo_radius(0);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'secondary'"));
      rule.set_max_scale(static_cast<unsigned long long>(50000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Book", 9, mapnik::color("black"));
      textSymb0.set_label_placement(mapnik::LINE_PLACEMENT);
      textSymb0.set_halo_radius(1);
      textSymb0.set_halo_fill(mapnik::color("#fed7a5"));
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'secondary'"));
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Book", 10, mapnik::color("black"));
      textSymb0.set_label_placement(mapnik::LINE_PLACEMENT);
      textSymb0.set_halo_radius(1);
      textSymb0.set_halo_fill(mapnik::color("#fed7a5"));
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'tertiary'"));
      rule.set_max_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Book", 9, mapnik::color("#000"));
      textSymb0.set_label_placement(mapnik::LINE_PLACEMENT);
      textSymb0.set_halo_radius(1);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'tertiary'"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Book", 11, mapnik::color("#000"));
      textSymb0.set_label_placement(mapnik::LINE_PLACEMENT);
      textSymb0.set_halo_radius(1);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(10000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[highway] = 'proposed' or [highway]='construction'"));
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Book", 9, mapnik::color("#000"));
      textSymb0.set_label_placement(mapnik::LINE_PLACEMENT);
      textSymb0.set_halo_radius(1);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(10000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[highway] = 'proposed' or [highway]='construction'"));
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Book", 11, mapnik::color("#000"));
      textSymb0.set_label_placement(mapnik::LINE_PLACEMENT);
      textSymb0.set_halo_radius(1);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'unclassified' or [highway] = 'residential'"));
      rule.set_max_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Book", 9, mapnik::color("#000"));
      textSymb0.set_label_placement(mapnik::LINE_PLACEMENT);
      textSymb0.set_halo_radius(1);
      textSymb0.set_label_spacing(300);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[highway] = 'unclassified' or [highway] = 'residential'"));
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Book", 11, mapnik::color("#000"));
      textSymb0.set_label_placement(mapnik::LINE_PLACEMENT);
      textSymb0.set_halo_radius(1);
      textSymb0.set_label_spacing(400);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_else(true);
      rule.set_max_scale(static_cast<unsigned long long>(25000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Book", 9, mapnik::color("#000"));
      textSymb0.set_label_placement(mapnik::LINE_PLACEMENT);
      textSymb0.set_halo_radius(1);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_else(true);
      rule.set_max_scale(static_cast<unsigned long long>(5000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100ULL / m_ScaleFactor));
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Book", 11, mapnik::color("#000"));
      textSymb0.set_label_placement(mapnik::LINE_PLACEMENT);
      textSymb0.set_halo_radius(1);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
    mapnikMap.insert_style("roads-text", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(500000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[man_made] = 'pier'"));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#f2efe9"), 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[man_made] = 'pier'"));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#f2efe9"), 4);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[man_made] = 'pier'"));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#f2efe9"), 7);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
    mapnikMap.insert_style("piers", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[natural] = 'cliff'"));
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[natural] = 'cliff'"));
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(20000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[man_made] = 'embankment'"));
      style.add_rule(rule);
      }
    mapnikMap.insert_style("cliffs", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_max_scale(static_cast<unsigned long long>(10000ULL / m_ScaleFactor));
      rule.set_filter(mapnik::create_filter("[barrier] <> '' or [natural]='hedge'"));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("black"), 0.4);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
    mapnikMap.insert_style("barriers", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[oneway] = 'yes' or [oneway] = 'true' or [oneway] = '1'"));
      rule.set_max_scale(static_cast<unsigned long long>(10000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#6c70d5"), 1);
        stroke.add_dash(0, 12);
        stroke.add_dash(10, 152);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#6c70d5"), 2);
        stroke.add_dash(0, 12);
        stroke.add_dash(9, 153);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#6c70d5"), 3);
        stroke.add_dash(0, 18);
        stroke.add_dash(2, 154);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#6c70d5"), 4);
        stroke.add_dash(0, 18);
        stroke.add_dash(1, 155);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[oneway] = '-1'"));
      rule.set_max_scale(static_cast<unsigned long long>(10000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#6c70d5"), 1);
        stroke.add_dash(0, 12);
        stroke.add_dash(10, 152);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#6c70d5"), 2);
        stroke.add_dash(0, 13);
        stroke.add_dash(9, 152);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#6c70d5"), 3);
        stroke.add_dash(0, 14);
        stroke.add_dash(2, 158);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("#6c70d5"), 4);
        stroke.add_dash(0, 15);
        stroke.add_dash(1, 158);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
    mapnikMap.insert_style("directions", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[boundary]='national_park'"));
      rule.set_max_scale(static_cast<unsigned long long>(5000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
        {
        mapnik::polygon_symbolizer geom = mapnik::polygon_symbolizer();
        geom.set_fill(mapnik::color("green"));
        geom.set_opacity(0.05);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("green"), 1.5);
        stroke.set_opacity (0.15);
        stroke.add_dash(4, 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[boundary]='national_park'"));
      rule.set_max_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
        {
        mapnik::polygon_symbolizer geom = mapnik::polygon_symbolizer();
        geom.set_fill(mapnik::color("green"));
        geom.set_opacity(0.05);
        rule.append(geom);
        }
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("green"), 3);
        stroke.set_opacity (0.15);
        stroke.add_dash(6, 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[boundary]='national_park'"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("green"), 3);
        stroke.set_opacity (0.15);
        stroke.add_dash(6, 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[boundary]='national_park'"));
      rule.set_max_scale(static_cast<unsigned long long>(2500000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Bold", 8, mapnik::color("#9c9"));
      textSymb0.set_halo_radius(1);
      textSymb0.set_wrap_width(14);
      textSymb0.set_displacement(0.0, -8);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[boundary]='national_park'"));
      rule.set_max_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(200000ULL / m_ScaleFactor));
      mapnik::text_symbolizer textSymb0("name", "DejaVu Sans Bold", 11, mapnik::color("#9c9"));
      textSymb0.set_halo_radius(1);
      textSymb0.set_wrap_width(14);
      rule.append(textSymb0);
      style.add_rule(rule);
      }
    mapnikMap.insert_style("boundary", style);
    }
    {
    mapnik::feature_type_style style;
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[admin_level]='2' or [admin_level]='3'"));
      rule.set_max_scale(static_cast<unsigned long long>(200000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(5000000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("purple"), 0.6);
        stroke.set_opacity (0.2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[admin_level]='2' or [admin_level]='3'"));
      rule.set_max_scale(static_cast<unsigned long long>(5000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("purple"), 2);
        stroke.set_opacity (0.2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[admin_level]='2'"));
      rule.set_max_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("purple"), 6);
        stroke.set_opacity (0.1);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[admin_level]='3'"));
      rule.set_max_scale(static_cast<unsigned long long>(1000000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("purple"), 5);
        stroke.set_opacity (0.1);
        stroke.add_dash(4, 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[admin_level]='4'"));
      rule.set_max_scale(static_cast<unsigned long long>(500000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("purple"), 3);
        stroke.set_opacity (0.2);
        stroke.add_dash(4, 3);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[admin_level]='5'"));
      rule.set_max_scale(static_cast<unsigned long long>(500000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("purple"), 2);
        stroke.set_opacity (0.3);
        stroke.add_dash(6, 3);
        stroke.add_dash(2, 3);
        stroke.add_dash(2, 3);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[admin_level]='6'"));
      rule.set_max_scale(static_cast<unsigned long long>(500000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("purple"), 2);
        stroke.set_opacity (0.3);
        stroke.add_dash(6, 3);
        stroke.add_dash(2, 3);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[admin_level]='8' or [admin_level]='7'"));
      rule.set_max_scale(static_cast<unsigned long long>(200000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("purple"), 1.5);
        stroke.set_opacity (0.3);
        stroke.add_dash(5, 2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("[admin_level]='10' or [admin_level]='9'"));
      rule.set_max_scale(static_cast<unsigned long long>(100000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("purple"), 2);
        stroke.set_opacity (0.3);
        stroke.add_dash(2, 3);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
      {
      mapnik::rule_type rule;
      rule.set_filter(mapnik::create_filter("not [admin_level] <>''"));
      rule.set_max_scale(static_cast<unsigned long long>(2000000ULL / m_ScaleFactor));
      rule.set_min_scale(static_cast<unsigned long long>(200000ULL / m_ScaleFactor));
        {
        mapnik::line_symbolizer geom = mapnik::line_symbolizer();
        mapnik::stroke          stroke = mapnik::stroke(mapnik::color("purple"), 1);
        stroke.set_opacity (0.2);
        geom.set_stroke(stroke);
        rule.append(geom);
        }
      style.add_rule(rule);
      }
    mapnikMap.insert_style("admin", style);
    }

}

} //namespace otb
