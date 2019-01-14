/*
 * Copyright (C) 2005-2019 by Centre National d'Etudes Spatiales (CNES)
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


#ifndef ossimCosmoSkymedModel_H
#define ossimCosmoSkymedModel_H

#include <ossimGeometricSarSensorModel.h>
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimDpt.h>

#include <otb/CivilDateTime.h>

#include <list>

namespace ossimplugins
{

class PlatformPosition;
class SensorParams;
class RefPoint;
/**
 * @brief This class allows for direct localisation and indirect localisation
 * using the CosmoSkymed sensor model
 */
class ossimCosmoSkymedModel : public ossimGeometricSarSensorModel
{
public:
  /**
   * @brief Constructor
   */
  ossimCosmoSkymedModel();

  /**
   * @brief Destructor
   */
  ~ossimCosmoSkymedModel();

  /**
   * @brief This function associates an image column number to a slant range when the image is georeferenced (ground projected)
   * @param col Column coordinate of the image point
   */
  virtual double getSlantRangeFromGeoreferenced(double col) const;

protected:

  /**
   * @brief Number of columns
   */
  double _nbCol;
  /**
  * @brief Slant Range for each Ground Range Projection reference point
  */
  double _SrGr_R0 ;
  /**
  * @brief Slant Range for each Ground Range Projection coefficients
  */
  std::vector<double> _SrGr_coeffs ;
  /**
   * @brief Scene Center range time
   */
  double _sceneCenterRangeTime;
  /**
   * @brief Pixel spacing
   */
  double _pixel_spacing ;

private:
  virtual bool InitPlatformPosition(const ossimKeywordlist &kwl, const char *prefix);
  virtual bool InitSensorParams(const ossimKeywordlist &kwl, const char *prefix);
  virtual bool InitRefPoint(const ossimKeywordlist &kwl, const char *prefix);
  /**
   * @brief Initializes the Slant Range to Ground Range data sets :
   *        _SrToGr_R0,_SrToGr_coeffs_number,_SrToGr_exponent,_SrToGr_coeffs,_nbCol
   */
  virtual bool InitSRGR(const ossimKeywordlist &kwl, const char *prefix);

  bool UtcDateTimeStringToCivilDate(const std::string &utcString, CivilDateTime &outputDate);

  TYPE_DATA
};
}

#endif
