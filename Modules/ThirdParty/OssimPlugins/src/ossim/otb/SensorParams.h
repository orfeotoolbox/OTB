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

#ifndef SensorParams_h
#define SensorParams_h

#include <ossimPluginConstants.h>
#include <string>

class ossimKeywordlist;

namespace ossimplugins
{

/**
 * @ingroup SARModel
 * @brief This class handles the sensor parameters
 * @todo There are too many setters in this class. Public attributes would be
 * simplier.
 */
class OSSIM_PLUGINS_DLL SensorParams
{
public:

   enum SightDirection
   {
      Left  = 0,
      Right = 1
   };

   /// Constructor
   SensorParams();

   /**
    * Saves state to a keyword list.
    * @param[in,out] kwl Keyword list to save to.
    * @param[in] prefix added to keys when saved.
    * @return true
    * @throw std::runtime_error if a field cannot be encoded
    */
   bool saveState(ossimKeywordlist& kwl, std::string const& prefix = "") const;

   /**
    * Loads (recreate) the state of the object from a keyword list.
    * @return true
    * @throw std::runtime_error if a field cannot be decoded
    */
   bool loadState (const ossimKeywordlist &kwl, std::string const& prefix = "");

   double get_prf() const
   {
      return m_prf;
   }

   double get_sf() const
   {
      return m_sf;
   }

   double get_rwl() const
   {
      return m_rwl;
   }

   void set_prf(double prf)
   {
      m_prf = prf;
   }

   void set_sf(double sf)
   {
      m_sf = sf;
   }

   void set_rwl(double rwl)
   {
      m_rwl = rwl;
   }

   SightDirection get_sightDirection() const
   {
      return m_sightDirection;
   }

   void set_sightDirection(SightDirection sight)
   {
      m_sightDirection = sight;
   }

   int get_col_direction() const
   {
      return m_col_direction;
   }

   int get_lin_direction() const
   {
      return m_lin_direction;
   }

   double get_nAzimuthLook() const
   {
      return m_nAzimuthLook ;
   }

   double get_nRangeLook() const
   {
      return m_nRangeLook ;
   }

   void set_col_direction(int dir)
   {
      m_col_direction = dir;
   }

   void set_lin_direction(int dir)
   {
      m_lin_direction = dir;
   }

   void set_nAzimuthLook(double look)
   {
      m_nAzimuthLook = look;
   }

   void set_nRangeLook(double look)
   {
      m_nRangeLook = look;
   }

   double get_semiMajorAxis() const
   {
      return m_semiMajorAxis ;
   }

   double get_semiMinorAxis() const
   {
      return m_semiMinorAxis ;
   }

   void set_semiMajorAxis(double value)
   {
      m_semiMajorAxis = value;
   }

   void set_semiMinorAxis(double value)
   {
      m_semiMinorAxis = value;
   }

   double get_dopcen() const
   {
      return m_dopcen;
   }

   double get_dopcenLinear() const
   {
      return m_dopcenLinear;
   }

   void set_dopcen(double value)
   {
      m_dopcen = value;
   }

   void set_dopcenLinear(double value)
   {
      m_dopcenLinear = value;
   }

private:
   /// Nominal PRF, Hz
   double m_prf;

   /// Sampling frequency
   double m_sf;

   /// Radar wave length
   double m_rwl;

   /// Columns direction (1=increasing, -1=decreasing)
   int m_col_direction;

   /// Lines direction (1=increasing, -1=decreasing)
   int m_lin_direction;

   /// Antenna pointing direction
   SightDirection m_sightDirection;

   /// Ellipsoid semi_major axis, m. Default : WGS84
   double m_semiMajorAxis;

   /// Ellipsoid semi_minor axis, m. Default : WGS84
   double m_semiMinorAxis;

   /// Number of azimuth looks
   double m_nAzimuthLook ;

   /// Number of range looks
   double m_nRangeLook ;

   /// Doppler centroid (at range 0)
   double m_dopcen;

   /// Doppler centroid linear term (wrt range in km)
   double m_dopcenLinear;

};
}
#endif
