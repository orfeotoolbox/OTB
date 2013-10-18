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
