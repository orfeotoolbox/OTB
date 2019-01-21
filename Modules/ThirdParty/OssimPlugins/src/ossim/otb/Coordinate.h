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
