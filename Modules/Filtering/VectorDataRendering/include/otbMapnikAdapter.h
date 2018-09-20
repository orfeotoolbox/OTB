/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef otbMapnikAdapter_h
#define otbMapnikAdapter_h

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include <mapnik/datasource_cache.hpp>
#include <mapnik/font_engine_freetype.hpp>
#include <mapnik/agg_renderer.hpp>
#include <mapnik/filter_factory.hpp>
#include <mapnik/color_factory.hpp>
#include <mapnik/image_util.hpp>
#include <mapnik/config_error.hpp>
#include <mapnik/memory_datasource.hpp>
#include <mapnik/layer.hpp>
#include <mapnik/map.hpp>
#include <mapnik/value.hpp>
#pragma GCC diagnostic pop
#else
#include <mapnik/datasource_cache.hpp>
#include <mapnik/font_engine_freetype.hpp>
#include <mapnik/agg_renderer.hpp>
#include <mapnik/filter_factory.hpp>
#include <mapnik/color_factory.hpp>
#include <mapnik/image_util.hpp>
#include <mapnik/config_error.hpp>
#include <mapnik/memory_datasource.hpp>
#include <mapnik/layer.hpp>
#include <mapnik/map.hpp>
#include <mapnik/value.hpp>
#endif

#ifdef OTB_MAPNIK_COMPATIBILITY_API07
namespace mapnik {
typedef Layer layer;
typedef Image32 image_32;
}
#endif

namespace otb
{
namespace [[deprecated]] mapnik_otb
{
// this should be removed once mapnik support for version < 2.0 is dropped.
// should be around 01/2013.
#ifdef OTB_MAPNIK_COMPATIBILITY_API07
static size_t get_num_layer(const mapnik::Map& map) { return map.layerCount(); }
static unsigned get_height(const mapnik::Map& map) { return map.getHeight(); }
static unsigned get_width(const mapnik::Map& map) { return map.getWidth(); }
typedef mapnik::Envelope<double> box2d;
static void zoom_to_box(mapnik::Map* map, const mapnik::Envelope<double>& envelope)
{
  map->zoomToBox(envelope);
}
typedef mapnik::geometry2d geom;
static geom* create_geom(int geom_type)
{
  geom* g = 0;
  switch (geom_type)
    {
    case mapnik::Point:
      g = new mapnik::point<mapnik::vertex<double, 2> >;
    case mapnik::LineString:
      g = new mapnik::line_string<mapnik::vertex<double, 2> , mapnik::vertex_vector2>;
    case mapnik::Polygon:
      g = new mapnik::polygon<mapnik::vertex<double, 2>, mapnik::vertex_vector2>;
    default:
      std::cerr
        << "Please fix otb::mapnik_otb::create_geom for mapnik 0.7" << std::endl;
    }
  return g;
}
#else
static size_t get_num_layer(const mapnik::Map& map) { return map.layer_count(); }
static unsigned get_height(const mapnik::Map& map) { return map.height(); }
static unsigned get_width(const mapnik::Map& map) { return map.width(); }
typedef mapnik::box2d<double> box2d;
static void zoom_to_box(mapnik::Map* map, const mapnik::box2d<double>& envelope)
{
  map->zoom_to_box(envelope);
}
typedef mapnik::vertex<double, 2>  vertex2d;
typedef mapnik::geometry<vertex2d> geom;
static geom* create_geom(mapnik::eGeomType geom_type) { return new geom(geom_type); }
#endif
}
}

#endif
