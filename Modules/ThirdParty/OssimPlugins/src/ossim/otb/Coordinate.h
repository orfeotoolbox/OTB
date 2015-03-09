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

#ifndef Coordinate_h
#define Coordinate_h

#include <ossimPluginConstants.h>

namespace ossimplugins
{


/**
 * @brief This class represents a coordinate
 */

class OSSIM_PLUGINS_DLL Coordinate
{
public:
   /**
    * @brief Constructor
    */
   Coordinate();
   /**
    * @brief Destructor
    */
   ~Coordinate();
   /**
    * @brief Constructor with initializations
    */
   Coordinate(double x, double y, double z);
   /**
    * @brief Copy constructor
    */
   Coordinate(const Coordinate& rhs);

   /**
    * @brief Affectation operator
    */
   Coordinate& operator=(const Coordinate& rhs);

   void set_coordinates(double x, double y, double z);

   double get_x()
   {
      return _x;
   };

   double get_y()
   {
      return _y;
   };

   double get_z()
   {
      return _z;
   };
protected:
   double _x;
   double _y;
   double _z;
private:
};
}

#endif
