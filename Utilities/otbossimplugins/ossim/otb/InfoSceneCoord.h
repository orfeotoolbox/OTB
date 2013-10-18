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

#ifndef InfoSceneCoord_h
#define InfoSceneCoord_h

#include <vector>
#include <ossim/base/ossimString.h>
#include <ossimPluginConstants.h>

class ossimKeywordlist;

namespace ossimplugins
{


/**
 * @ingroup InfoSceneCoord
 * @brief This class represents an InfoSceneCoord
 */
class OSSIM_PLUGINS_DLL InfoSceneCoord
{
public:
   /**
    * @brief Constructor
    */
   InfoSceneCoord();

   /**
    * @brief Destructor
    */
   virtual ~InfoSceneCoord();

   /**
    * @brief Copy constructor
    */
   InfoSceneCoord(const InfoSceneCoord& rhs);

   /**
    * @brief Affectation operator
    */
   InfoSceneCoord& operator=(const InfoSceneCoord& rhs);

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

   void set_refRow(ossim_uint32 value)
   {
      _refRow = value;
   }
   void set_refColumn(ossim_uint32 value)
   {
      _refColumn = value;
   }
   void set_lat(double value)
   {
	   _lat = value;
   }
   void set_lon(double value)
   {
	   _lon = value;
   }
   void set_azimuthTimeUTC(ossimString value)
   {
	   _azimuthTimeUTC = value;
   }
   void set_rangeTime(double value)
   {
	   _rangeTime = value;
   }
   void set_incidenceAngle(double value)
   {
      _incidenceAngle = value;
   }


protected:

      /**
       * @brief Reference Row (refRow node).
       */
      ossim_uint32 _refRow;

      /**
       * @brief Reference Column (refColumn node).
       */
      ossim_uint32 _refColumn;
      /**
       * @brief Latitude (lat node).
       */
      double _lat;
      /**
       * @brief Longitude (lon node).
       */
      double _lon;
      /**
       * @brief azimuth time  (azimuthTimeUTC node).
       */
      ossimString _azimuthTimeUTC;
      /**
       * @brief range time  (rangeTime node).
       */
      double _rangeTime;
      /**
       * @brief IncidenceAngle (incidenceAngle node).
       */
      double _incidenceAngle;

private:
};
}

#endif
