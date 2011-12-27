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

#ifndef PlatformPosition_h
#define PlatformPosition_h

#include <ossimPluginConstants.h>
#include <otb/JSDDateTime.h>

class ossimKeywordlist;

namespace ossimplugins
{


class Ephemeris;
class HermiteInterpolator;


/**
 * @ingroup SARModel
 * @brief This class handles the platform position
 */
class OSSIM_PLUGINS_DLL PlatformPosition
{
public:
   /**
    * @brief Constructor
    */
   PlatformPosition();
   /**
    * @brief Destructor
    */
   ~PlatformPosition();
   /**
    * @brief Copy constructor
    */
   PlatformPosition(const PlatformPosition& rhs);

   /**
    * @brief Constructor with initialisation
    * @param data Ephemeris
    * @param nbrData Number of Ephemeris in data
    * @remarks All the ephemeris have to be in the same coordinates system
    * @remarks This constructor copy the content of data, so it's the
    * calling fonction that have to manage the memory of data.
    */
   PlatformPosition(Ephemeris** data, int nbrData);

   /**
    * @brief Affectation operator
    */
   PlatformPosition& operator=(const PlatformPosition& rhs);

   /**
    * @brief This function interpolates its ephemeris to create a new ephemeris at the given date and time
    * @param date Date and time at wich the interpolation have to be done
    * @return The ephemeris at the given date, or NULL if an error occurs
    */
   Ephemeris* Interpolate(JSDDateTime date) const;

   PlatformPosition* Clone() const
   {
      return new PlatformPosition(*this);
   };

   void setData(Ephemeris** data, int nbrData);
   Ephemeris* getData(int noData) const;

   int getNbrData() const;
   /**
    * @brief Method to save object state to a keyword list.
    * @param kwl Keyword list to save to.
    * @param prefix added to keys when saved.
    * @return true on success, false on error.
    */
   bool saveState(ossimKeywordlist& kwl,
                  const char* prefix=0) const;

   /**
    * @brief Method to the load (recreate) the state of the object from a
    * keyword list. Return true if ok or false on error.
    * @return true if load OK, false on error
    */
   bool loadState(const ossimKeywordlist &kwl, const char *prefix=0);

protected:

   /**
    * @brief Internal method to initialize data structures
    */
   void InitData(Ephemeris** data, int nbrData);
   void InitAuxiliaryData();

   /**
    * @brief Number of platform positions
    * @see _data
    */
   int _nbrData;

   /**
    * @brief Platform positions
    */
   Ephemeris** _data;

   /**
    * @brief This function deletes all the contents of the class
    */
   void Clear();

private:
   double * _t;
   double ** _p;
   double ** _dp;
   HermiteInterpolator ** _interpolator;
};
}

#endif
