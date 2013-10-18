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

#ifndef ossimCosmoSkymedModel_H
#define ossimCosmoSkymedModel_H

#include <ossimGeometricSarSensorModel.h>
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimDpt.h>

#include <otb/CivilDateTime.h>

#include <iostream>
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
