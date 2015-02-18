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

#ifndef RectangularCoordinate_h
#define RectangularCoordinate_h

#include <ossimPluginConstants.h>
#include <otb/Coordinate.h>

namespace ossimplugins
{


class GeodesicCoordinate;
/**
 * @ingroup Date
 * @brief This class represents a coordinate in a rectangular reference
 */
 class OSSIM_PLUGINS_DLL RectangularCoordinate : public Coordinate
 {
 public:
  /**
    * @brief Constructor
    */
  RectangularCoordinate();
  /**
   * @brief Destructor
   */
  ~RectangularCoordinate();
  /**
   * @brief Cosntructor with initializations
   */
  RectangularCoordinate(double x, double y, double z);
  /**
   * @brief Copy constructor
   */
  RectangularCoordinate(const RectangularCoordinate& rhs);

  /**
   * @brief Affectation operator
   */
  RectangularCoordinate& operator=(const RectangularCoordinate& rhs);

  void AsGeodesicCoordinates(double demiGdAxe, double demiPtAxe, GeodesicCoordinate* geod);
 protected:
 private:
 };
}

 #endif
