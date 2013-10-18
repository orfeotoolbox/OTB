//----------------------------------------------------------------------------
//
// "Copyright Centre National d'Etudes Spatiales"
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------
// $Id$

#include "erssar/ErsSarMapProjectionData.h"

namespace ossimplugins
{

   ErsSarMapProjectionData::ErsSarMapProjectionData() : ErsSarRecord("map_proj_data_rec")
   {
   }

   ErsSarMapProjectionData::~ErsSarMapProjectionData()
   {
   }

   std::ostream& operator<<(std::ostream& os, const ErsSarMapProjectionData& data)
   {
      os << "map_proj_des:" << data._map_proj_des.c_str() << std::endl;

      os << "num_pix_in_line:" << data._num_pix_in_line << std::endl;

      os << "num_lines:" << data._num_lines << std::endl;

      os << "nom_interpixel_dist:" << data._nom_interpixel_dist << std::endl;

      os << "nom_interline_dist:" << data._nom_interline_dist << std::endl;

      os << "orientation_at_center:" << data._orientation_at_center << std::endl;

      os << "orbit_incl:" << data._orbit_incl << std::endl;

      os << "asc_node_long:" << data._asc_node_long << std::endl;

      os << "platform_heading:" << data._platform_heading << std::endl;

      os << "name_of_ref_ellipsoid:" << data._name_of_ref_ellipsoid.c_str() << std::endl;

      os << "semi_maj_axis:" << data._semi_maj_axis << std::endl;

      os << "semi_min_axis:" << data._semi_min_axis << std::endl;

      os << "first_line_first_pixel_lat:" << data._first_line_first_pixel_lat << std::endl;

      os << "first_line_first_pixel_lon:" << data._first_line_first_pixel_lon << std::endl;

      os << "first_line_last_pixel_lat:" << data._first_line_last_pixel_lat << std::endl;

      os << "first_line_last_pixel_lon:" << data._first_line_last_pixel_lon << std::endl;

      os << "last_line_last_pixel_lat:" << data._last_line_last_pixel_lat << std::endl;

      os << "last_line_last_pixel_lon:" << data._last_line_last_pixel_lon << std::endl;

      os << "last_line_first_pixel_lat:" << data._last_line_first_pixel_lat << std::endl;

      os << "last_line_first_pixel_lon:" << data._last_line_first_pixel_lon << std::endl;

      return os;

   }

   std::istream& operator>>(std::istream& is, ErsSarMapProjectionData& data)
   {
      char buf16[17];
      buf16[16] = '\0';
      char buf32[33];
      buf32[32] = '\0';
      // char buf8[9];
      // buf8[8] = '\0';
      // char buf4[5];
      // buf4[4] = '\0';

      is.read(buf16, 16);

      is.read(buf32, 32);
      data._map_proj_des = buf32;

      is.read(buf16, 16);
      data._num_pix_in_line = atoi(buf16);

      is.read(buf16, 16);
      data._num_lines = atoi(buf16);

      is.read(buf16, 16);
      data._nom_interpixel_dist = atof(buf16);

      is.read(buf16, 16);
      data._nom_interline_dist = atof(buf16);

      is.read(buf16, 16);
      data._orientation_at_center = atof(buf16);

      is.read(buf16, 16);
      data._orbit_incl = atof(buf16);

      is.read(buf16, 16);
      data._asc_node_long = atof(buf16);

      is.read(buf16, 16);

      is.read(buf16, 16);

      is.read(buf16, 16);

      is.read(buf16, 16);
      data._platform_heading = atof(buf16);

      is.read(buf32, 32);
      data._name_of_ref_ellipsoid = buf32;

      is.read(buf16, 16);
      data._semi_maj_axis = atof(buf16);

      is.read(buf16, 16);
      data._semi_min_axis = atof(buf16);

      char buf772[772];
      is.read(buf772, 772);

      is.read(buf16, 16);
      data._first_line_first_pixel_lat = atof(buf16);

      is.read(buf16, 16);
      data._first_line_first_pixel_lon = atof(buf16);

      is.read(buf16, 16);
      data._first_line_last_pixel_lat = atof(buf16);

      is.read(buf16, 16);
      data._first_line_last_pixel_lon = atof(buf16);

      is.read(buf16, 16);
      data._last_line_last_pixel_lat = atof(buf16);

      is.read(buf16, 16);
      data._last_line_last_pixel_lon = atof(buf16);

      is.read(buf16, 16);
      data._last_line_first_pixel_lat = atof(buf16);

      is.read(buf16, 16);
      data._last_line_first_pixel_lon = atof(buf16);

      char buf420[420];
      is.read(buf420, 420);

      return is;
   }


   ErsSarMapProjectionData::ErsSarMapProjectionData(const ErsSarMapProjectionData& rhs):
      ErsSarRecord(rhs),
      _map_proj_des(rhs._map_proj_des),
      _num_pix_in_line(rhs._num_pix_in_line),
      _num_lines(rhs._num_lines),
      _nom_interpixel_dist(rhs._nom_interpixel_dist),
      _nom_interline_dist(rhs._nom_interline_dist),
      _orientation_at_center(rhs._orientation_at_center),
      _orbit_incl(rhs._orbit_incl),
      _asc_node_long(rhs._asc_node_long),
      _platform_heading(rhs._platform_heading),
      _name_of_ref_ellipsoid(rhs._name_of_ref_ellipsoid),
      _semi_maj_axis(rhs._semi_maj_axis),
      _semi_min_axis(rhs._semi_min_axis),
      _first_line_first_pixel_lat(rhs._first_line_first_pixel_lat),
      _first_line_first_pixel_lon(rhs._first_line_first_pixel_lon),
      _first_line_last_pixel_lat(rhs._first_line_last_pixel_lat),
      _first_line_last_pixel_lon(rhs._first_line_last_pixel_lon),
      _last_line_last_pixel_lat(rhs._last_line_last_pixel_lat),
      _last_line_last_pixel_lon(rhs._last_line_last_pixel_lon),
      _last_line_first_pixel_lat(rhs._last_line_first_pixel_lat),
      _last_line_first_pixel_lon(rhs._last_line_first_pixel_lon)
   {}

   ErsSarMapProjectionData& ErsSarMapProjectionData::operator=(const ErsSarMapProjectionData& rhs)
   {
      _map_proj_des = rhs._map_proj_des;
      _num_pix_in_line = rhs._num_pix_in_line;
      _num_lines = rhs._num_lines;
      _nom_interpixel_dist = rhs._nom_interpixel_dist;
      _nom_interline_dist = rhs._nom_interline_dist;
      _orientation_at_center = rhs._orientation_at_center;
      _orbit_incl = rhs._orbit_incl;
      _asc_node_long = rhs._asc_node_long;
      _platform_heading = rhs._platform_heading;
      _name_of_ref_ellipsoid = rhs._name_of_ref_ellipsoid;
      _semi_maj_axis = rhs._semi_maj_axis;
      _semi_min_axis = rhs._semi_min_axis;
      _first_line_first_pixel_lat = rhs._first_line_first_pixel_lat;
      _first_line_first_pixel_lon = rhs._first_line_first_pixel_lon;
      _first_line_last_pixel_lat = rhs._first_line_last_pixel_lat;
      _first_line_last_pixel_lon = rhs._first_line_last_pixel_lon;
      _last_line_last_pixel_lat = rhs._last_line_last_pixel_lat;
      _last_line_last_pixel_lon = rhs._last_line_last_pixel_lon;
      _last_line_first_pixel_lat = rhs._last_line_first_pixel_lat;
      _last_line_first_pixel_lon = rhs._last_line_first_pixel_lon;

      return *this;
   }
}
