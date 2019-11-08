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


#ifndef Ephemeride_h
#define Ephemeride_h

#include <ossimPluginConstants.h>
#include <otb/JSDDateTime.h>

class ossimKeywordlist;

namespace ossimplugins
{


/**
 * @ingroup Ephemeris
 * @brief This class represents an ephemeris
 */
class OSSIM_PLUGINS_DLL Ephemeris
{
public:
   /**
    * @brief Constructor
    */
   Ephemeris();

   /**
    * @brief Destructor
    */
   virtual ~Ephemeris();

   /**
    * @brief Constructor with initialisations
    */
   Ephemeris(JSDDateTime date, double pos[3], double speed[3]);
   /**
    * @brief Copy constructor
    */
   Ephemeris(const Ephemeris& rhs);

   /**
    * @brief Affectation operator
    */
   Ephemeris& operator=(const Ephemeris& rhs);

   /**
    * @brief This function creatse a copy of the current class
    */
   virtual Ephemeris* Clone()
   {
      return new Ephemeris(*this);
   }

   JSDDateTime get_date() const
   {
      return _date;
   }

   double* get_position()
   {
      return _position;
   }

   const double* get_position() const
   {
      return _position;
   }

   double* get_speed()
   {
      return _speed;
   }

   const double* get_speed() const
   {
      return _speed;
   }

   void set_date(JSDDateTime date)
   {
      _date = date;
   }

   void set_position(double position[3])
   {
      _position[0] = position[0];
      _position[1] = position[1];
      _position[2] = position[2];
   }

   void set_speed(double speed[3])
   {
      _speed[0] = speed[0];
      _speed[1] = speed[1];
      _speed[2] = speed[2];
   }

   /**
    * @brief Method to save object state to a keyword list.
    * @param kwl Keyword list to save to.
    * @param prefix added to keys when saved.
    * @return true on success, false on error.
    */
   bool saveState(ossimKeywordlist& kwl, const char* prefix=0) const;

   /**
    * @brief Method to the load (recreate) the state of the object from a
    * keyword list. Return true if ok or false on error.
    * @return true if load OK, false on error
    */
   bool loadState (const ossimKeywordlist &kwl, const char *prefix=0);

protected:

   JSDDateTime _date;
   double _position[3];
   double _speed[3];
private:
};
}

#endif
