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


#ifndef IncidenceAngles_h
#define IncidenceAngles_h

#include <ossim/base/ossimConstants.h>
#include <ossimPluginConstants.h>
#include <otb/JSDDateTime.h>
#include <otb/InfoIncidenceAngle.h>

class ossimKeywordlist;

namespace ossimplugins
{

/**
 * @ingroup IncidenceAngles
 * @brief This class represents Incidence Angles
 */
class OSSIM_PLUGINS_DLL IncidenceAngles
{
public:
   /**
    * @brief Constructor
    */
  IncidenceAngles();

   /**
    * @brief Destructor
    */
  virtual ~IncidenceAngles();

   /**
    * @brief Copy constructor
    */
  IncidenceAngles(const IncidenceAngles& rhs);

   /**
    * @brief Affectation operator
    */
  IncidenceAngles& operator=(const IncidenceAngles& rhs);

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
   /*!
    * METHOD: print()
    * Fulfills base-class pure virtual. Dumps contents of object to ostream.
    */
   virtual std::ostream& print(std::ostream& out) const;

  void set_numberOfCornerIncidenceAngles(const ossim_uint32& numberOfCornerIncidenceAngles)
  {
    _numberOfCornerIncidenceAngles = numberOfCornerIncidenceAngles;
  }
  void set_centerInfoIncidenceAngle(const InfoIncidenceAngle& centerInfoIncidenceAngle)
  {
    _centerInfoIncidenceAngle = centerInfoIncidenceAngle;
  }
  void set_cornersInfoIncidenceAngle(const std::vector<InfoIncidenceAngle>& cornersInfoIncidenceAngle)
  {
    _tabCornersInfoIncidenceAngle = cornersInfoIncidenceAngle;
  }

protected:

      /**
       * @brief Number of incidence angles
       */
      ossim_uint32 _numberOfCornerIncidenceAngles;
      /**
       * @brief Center InfoIncidenceAngle.
       */
      InfoIncidenceAngle _centerInfoIncidenceAngle;
      /**
       * @brief Corners InfoIncidenceAngle.
       */
      std::vector<InfoIncidenceAngle> _tabCornersInfoIncidenceAngle;
  
private:
};
}

#endif
