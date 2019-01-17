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


#ifndef SceneCoord_h
#define SceneCoord_h

#include <otb/JSDDateTime.h>
#include <otb/InfoSceneCoord.h>
#include <ossim/base/ossimConstants.h>
#include <ossimPluginConstants.h>

class ossimKeywordlist;

namespace ossimplugins
{

/**
 * @ingroup SceneCoord
 * @brief This class represents Scence coordinate
 */
class OSSIM_PLUGINS_DLL SceneCoord
{
public:
   /**
    * @brief Constructor
    */
 SceneCoord();

   /**
    * @brief Destructor
    */
  virtual ~SceneCoord();

   /**
    * @brief Copy constructor
    */
  SceneCoord(const SceneCoord& rhs);

   /**
    * @brief Affectation operator
    */
    SceneCoord& operator=(const SceneCoord& rhs);

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

  void set_numberOfSceneCoord(const ossim_uint32& numberOfSceneCoord)
  {
    _numberOfSceneCoord = numberOfSceneCoord;
  }
  void set_centerSceneCoord(const InfoSceneCoord& centerSceneCoord)
  {
    _centerSceneCoord = centerSceneCoord;
  }
  void set_cornersSceneCoord(const std::vector<InfoSceneCoord>& cornersSceneCoord)
  {
    _tabCornersSceneCoord = cornersSceneCoord;
  }

protected:

      /**
       * @brief Number of scene coordinate
       */
      ossim_uint32 _numberOfSceneCoord;
      /**
       * @brief Center scene coordinate.
       */
      InfoSceneCoord _centerSceneCoord;
      /**
       * @brief Corners InfoIncidenceAngle.
       */
      std::vector<InfoSceneCoord> _tabCornersSceneCoord;
  
private:
};
}

#endif
