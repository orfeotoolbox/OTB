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

#include <ossimPluginConstants.h>
#include <ossim/base/ossimConstants.h>
#include <string>

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

   void set_refRow(ossim_uint32 value)
   {
      m_refRow = value;
   }
   void set_refColumn(ossim_uint32 value)
   {
      m_refColumn = value;
   }
   void set_lat(double value)
   {
      m_lat = value;
   }
   void set_lon(double value)
   {
      m_lon = value;
   }
   void set_azimuthTimeUTC(std::string const& value)
   {
      m_azimuthTimeUTC = value;
   }
   void set_rangeTime(double value)
   {
      m_rangeTime = value;
   }
   void set_incidenceAngle(double value)
   {
      m_incidenceAngle = value;
   }

   double get_lat() const { return m_lat;}
   double get_lon() const { return m_lon;}
private:

   /**
    * @brief Reference Row (refRow node).
    */
   ossim_uint32 m_refRow;

   /**
    * @brief Reference Column (refColumn node).
    */
   ossim_uint32 m_refColumn;
   /**
    * @brief Latitude (lat node).
    */
   double m_lat;
   /**
    * @brief Longitude (lon node).
    */
   double m_lon;
   /**
    * @brief azimuth time  (azimuthTimeUTC node).
    */
   std::string m_azimuthTimeUTC;
   /**
    * @brief range time  (rangeTime node).
    */
   double m_rangeTime;
   /**
    * @brief IncidenceAngle (incidenceAngle node).
    */
   double m_incidenceAngle;
};
}

#endif
