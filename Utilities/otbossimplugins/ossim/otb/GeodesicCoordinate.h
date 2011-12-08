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

#ifndef GeodesicCoordinate_h
#define GeodesicCoordinate_h

#include <ossimPluginConstants.h>
#include <otb/Coordinate.h>

namespace ossimplugins
{


/**
 * @ingroup Date
 * @brief This class represents a coordinate in a geodesic reference
 */
 class OSSIM_PLUGINS_DLL GeodesicCoordinate : public Coordinate
 {
 public:
  /**
    * @brief Constructor
    */
  GeodesicCoordinate();
  /**
   * @brief Destructor
   */
  ~GeodesicCoordinate();
  /**
   * @brief Cosntructor with initializations
   */
  GeodesicCoordinate(double x, double y, double z);
  /**
   * @brief Copy constructor
   */
  GeodesicCoordinate(const GeodesicCoordinate& rhs);

  /**
   * @brief Affectation operator
   */
  GeodesicCoordinate& operator=(const GeodesicCoordinate& rhs);
 protected:
 private:
 };
}

 #endif
