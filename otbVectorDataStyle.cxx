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
    mapnik::rule_type rule;
    rule.set_max_scale(250000000000LLU);
    rule.set_min_scale(600000);
    rule.append(mapnik::line_symbolizer(mapnik::color("#f2efe9")));
//     rule.append(mapnik::polygon_symbolizer(mapnik::Color(242,239,233)));
    style.add_rule(rule);
    mapnikMap.insert_style("world",style);
  }
  {
    mapnik::feature_type_style style;
    mapnik::rule_type rule;
    rule.set_max_scale(600000);
    rule.append(mapnik::line_symbolizer(mapnik::color("#f2efe9")));
//     rule.append(mapnik::polygon_symbolizer(mapnik::Color(242,239,233)));
    style.add_rule(rule);
    mapnikMap.insert_style("coast-poly",style);
  }
  {
    mapnik::feature_type_style style;
    mapnik::rule_type rule;
    rule.set_max_scale(600000);
    rule.append(mapnik::line_symbolizer(mapnik::color("#e46d71")));
//     rule.append(mapnik::polygon_symbolizer(mapnik::Color(242,239,233)));
    style.add_rule(rule);
    mapnikMap.insert_style("coast-poly",style);
  }
  {
    mapnik::feature_type_style style;
    mapnik::rule_type rule;
    rule.set_max_scale(100000000);
//       rule.set_max_scale(1000000);
//     rule.set_min_scale(500000);
//       rule.append(mapnik::line_symbolizer(mapnik::color("#809bc0"),8.0));
    rule.append(mapnik::line_symbolizer(mapnik::color("#80c09b"),8.0));
    mapnik::text_symbolizer textSymb("name", "DejaVu Sans Book", 9, mapnik::color("#000"));
    textSymb.set_label_placement(mapnik::LINE_PLACEMENT);
    rule.append(textSymb);
    style.add_rule(rule);
    mapnikMap.insert_style("roads",style);
  }
  {
    mapnik::feature_type_style style;
    mapnik::rule_type rule;
    rule.set_max_scale(100000000);
//       rule.set_max_scale(1000000);
//     rule.set_min_scale(500000);
    rule.append(mapnik::line_symbolizer(mapnik::color("#b5d0d0"),10));
    mapnik::text_symbolizer textSymb("name", "DejaVu Sans Book", 10, mapnik::color("#6699cc"));
    textSymb.set_label_placement(mapnik::LINE_PLACEMENT);
    rule.append(textSymb);
    style.add_rule(rule);
    mapnikMap.insert_style("river",style);
  }
}

}

