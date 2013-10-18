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
