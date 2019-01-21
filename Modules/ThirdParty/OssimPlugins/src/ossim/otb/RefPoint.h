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


#ifndef RefPoint_h
#define RefPoint_h

#include <ossimPluginConstants.h>

class ossimKeywordlist;

namespace ossimplugins
{


class Ephemeris;


/**
 * @brief This class handles the referential point
 */
class OSSIM_PLUGINS_DLL RefPoint
{
public:
   /**
    * @brief Constructor
    */
   RefPoint();
   /**
    * @brief Destructor
    */
   ~RefPoint();
   /**
    * @brief Copy constructor
    */
   RefPoint(const RefPoint& rhs);
   /**
    * @brief Affectation operator
    */
   RefPoint& operator=(const RefPoint& rhs);

   void set_ephemeris(Ephemeris* ephemeris);
   void set_distance(double distance);
   void set_pix_line(double pix_line);
   void set_pix_col(double pix_col);

   Ephemeris* get_ephemeris();
   double get_distance() const;
   double get_pix_line() const;
   double get_pix_col() const;

   RefPoint* Clone()
   {
      return new RefPoint(*this);
   };

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
   Ephemeris* _ephemeris;
   double _distance;
   double _pix_line;
   double _pix_col;
private:
};
}

#endif
