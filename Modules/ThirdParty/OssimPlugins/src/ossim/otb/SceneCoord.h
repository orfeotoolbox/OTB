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

#ifndef SceneCoord_h
#define SceneCoord_h

#include "otb/InfoSceneCoord.h"
#include "ossimPluginConstants.h"
#include <ossim/base/ossimConstants.h>
#include <string>
#include <vector>
#include <iosfwd>

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
    * @brief Method to save object state to a keyword list.
    * @param kwl Keyword list to save to.
    * @param prefix added to keys when saved.
    * @return true
    * @throw std::runtime_error if a field cannot be encoded
    */
   bool saveState(ossimKeywordlist& kwl, std::string const& prefix="") const;

   /**
    * @brief Method to the load (recreate) the state of the object from a
    * keyword list. Return true if ok or false on error.
    * @return true
    * @throw std::runtime_error if a field cannot be decoded
    */
   bool loadState (const ossimKeywordlist &kwl, std::string const& prefix="");
   /*!
    * METHOD: print()
    * Fulfills base-class pure virtual. Dumps contents of object to ostream.
    */
   std::ostream& print(std::ostream& out) const;

   void set_numberOfSceneCoord(ossim_uint32 numberOfSceneCoord)
   {
      m_numberOfSceneCoord = numberOfSceneCoord;
   }
   void set_centerSceneCoord(const InfoSceneCoord& centerSceneCoord)
   {
      m_centerSceneCoord = centerSceneCoord;
   }
   void set_cornersSceneCoord(const std::vector<InfoSceneCoord>& cornersSceneCoord)
   {
      m_tabCornersSceneCoord = cornersSceneCoord;
   }

private:

   /**
    * @brief Number of scene coordinate
    */
   ossim_uint32 m_numberOfSceneCoord;
   /**
    * @brief Center scene coordinate.
    */
   InfoSceneCoord m_centerSceneCoord;
   /**
    * @brief Corners InfoIncidenceAngle.
    */
   std::vector<InfoSceneCoord> m_tabCornersSceneCoord;
};
}

#endif
