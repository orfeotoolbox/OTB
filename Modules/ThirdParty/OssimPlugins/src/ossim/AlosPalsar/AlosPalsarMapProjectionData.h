/*
 * Copyright (C) 2005-2019 by Centre National d'Etudes Spatiales (CNES)
 * Copyright (C) 2008-2010 by Centre for Remote Imaging, Sensing and Processing (CRISP)
 *
 * This file is licensed under MIT license:
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


#ifndef AlosPalsarMapProjectionData_h
#define AlosPalsarMapProjectionData_h

#include <cstdlib>
#include <AlosPalsar/AlosPalsarRecordHeader.h>
#include <AlosPalsar/AlosPalsarRecord.h>
#include <string>

namespace ossimplugins
{

/**
 * @ingroup AlosPalsarMapProjectionDataRecord
 * @brief This class is able to read the SAR leader data set summary record of the leader file
 */
class AlosPalsarMapProjectionData : public AlosPalsarRecord
{
public:
  /**
   * @brief Constructor
   */
  AlosPalsarMapProjectionData();

  /**
   * @brief Destructor
   */
  virtual ~AlosPalsarMapProjectionData();

  /**
   * @brief This function write the AlosPalsarMapProjectionData in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const AlosPalsarMapProjectionData& data);

  /**
   * @brief This function read a AlosPalsarMapProjectionData from a stream
   */
  friend std::istream& operator>>(std::istream& is, AlosPalsarMapProjectionData& data);

  /**
   * @brief Copy constructor
   */
  AlosPalsarMapProjectionData(const AlosPalsarMapProjectionData& rhs);

  /**
   * @brief Copy operator
   */
  AlosPalsarMapProjectionData& operator=(const AlosPalsarMapProjectionData& rhs);

  /**
   * @brief This function is able to create a new instance of the class
   */
  AlosPalsarRecord* Instantiate()
  {
    return new AlosPalsarMapProjectionData();
  };

  /**
   * @brief This function is able to create a new instance of the class initialised with the data of the calling instance
   */
  AlosPalsarRecord* Clone()
  {
    return new AlosPalsarMapProjectionData(*this);
  };

  /**
   * @brief Read the class data from a stream
   */
  void Read(std::istream& is)
  {
    is >> *this;
  };

  /**
   * @brief Write the class to a stream
   */
  void Write(std::ostream& os)
  {
    os << *this;
  };

  /**
  * @brief map_proj_des
  */
  std::string   get_map_proj_des() const
  {
    return _map_proj_des;
  };
  /**
  * @brief num_pix_in_line
  */
  int   get_num_pix_in_line() const
  {
    return _num_pix_in_line;
  };
  /**
  * @brief num_lines
  */
  int   get_num_lines() const
  {
    return _num_lines;
  };
  /**
  * @brief nom_interpixel_dist
  */
  double   get_nom_interpixel_dist() const
  {
    return _nom_interpixel_dist;
  };
  /**
  * @brief nom_interline_dist
  */
  double   get_nom_interline_dist() const
  {
    return _nom_interline_dist;
  };
  /**
  * @brief orientation_at_center
  */
  double   get_orientation_at_center() const
  {
    return _orientation_at_center;
  };
  /**
  * @brief orbit_incl
  */
  double   get_orbit_incl() const
  {
    return _orbit_incl;
  };
  /**
  * @brief asc_node_long
  */
  double   get_asc_node_long() const
  {
    return _asc_node_long;
  };
  /**
  * @brief platform_heading
  */
  double   get_platform_heading() const
  {
    return _platform_heading;
  };
  /**
  * @brief name_of_ref_ellipsoid
  */
  std::string   get_name_of_ref_ellipsoid() const
  {
    return _name_of_ref_ellipsoid;
  };
  /**
  * @brief semi_maj_axis
  */
  double   get_semi_maj_axis() const
  {
    return _semi_maj_axis;
  };
  /**
  * @brief semi_min_axis
  */
  double   get_semi_min_axis() const
  {
    return _semi_min_axis;
  };
  /**
  * @brief first_line_first_pixel  _lat
  */
  double   get_first_line_first_pixel_lat() const
  {
    return _first_line_first_pixel_lat;
  };
  /**
  * @brief first_line_first_pixel_lon
  */
  double   get_first_line_first_pixel_lon() const
  {
    return _first_line_first_pixel_lon;
  };
  /**
  * @brief first_line_last_pixel  _lat
  */
  double   get_first_line_last_pixel_lat() const
  {
    return _first_line_last_pixel_lat;
  };
  /**
  * @brief first_line_last_pixel_lon
  */
  double   get_first_line_last_pixel_lon() const
  {
    return _first_line_last_pixel_lon;
  };
  /**
  * @brief last_line_last_pixel_lat
  */
  double   get_last_line_last_pixel_lat() const
  {
    return _last_line_last_pixel_lat;
  };
  /**
  * @brief last_line_last_pixel_lon
  */
  double   get_last_line_last_pixel_lon() const
  {
    return _last_line_last_pixel_lon;
  };
  /**
  * @brief last_line_first_pixel_lat
  */
  double   get_last_line_first_pixel_lat() const
  {
    return _last_line_first_pixel_lat;
  };
  /**
  * @brief last_line_first_pixel_lon
  */
  double   get_last_line_first_pixel_lon() const
  {
    return _last_line_first_pixel_lon;
  };

protected:
  /**
  * @brief map_proj_des
  */
  std::string   _map_proj_des;
  /**
  * @brief num_pix_in_line
  */
  int   _num_pix_in_line;
  /**
  * @brief num_lines
  */
  int   _num_lines;
  /**
  * @brief nom_interpixel_dist
  */
  double   _nom_interpixel_dist;
  /**
  * @brief nom_interline_dist
  */
  double   _nom_interline_dist;
  /**
  * @brief orientation_at_center
  */
  double   _orientation_at_center;
  /**
  * @brief orbit_incl
  */
  double   _orbit_incl;
  /**
  * @brief asc_node_long
  */
  double   _asc_node_long;
  /**
  * @brief platform_heading
  */
  double   _platform_heading;
  /**
  * @brief name_of_ref_ellipsoid
  */
  std::string   _name_of_ref_ellipsoid;
  /**
  * @brief semi_maj_axis
  */
  double   _semi_maj_axis;
  /**
  * @brief semi_min_axis
  */
  double   _semi_min_axis;
  /**
  * @brief first_line_first_pixel_lat
  */
  double   _first_line_first_pixel_lat;
  /**
  * @brief first_line_first_pixel_lon
  */
  double   _first_line_first_pixel_lon;
  /**
  * @brief first_line_last_pixel_lat
  */
  double   _first_line_last_pixel_lat;
  /**
  * @brief first_line_last_pixel_lon
  */
  double   _first_line_last_pixel_lon;
  /**
  * @brief last_line_last_pixel_lat
  */
  double   _last_line_last_pixel_lat;
  /**
  * @brief last_line_last_pixel_lon
  */
  double   _last_line_last_pixel_lon;
  /**
  * @brief last_line_first_pixel_lat
  */
  double   _last_line_first_pixel_lat;
  /**
  * @brief last_line_first_pixel_lon
  */
  double   _last_line_first_pixel_lon;


private:

};
}
#endif
