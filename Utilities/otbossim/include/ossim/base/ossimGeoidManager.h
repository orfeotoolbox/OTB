//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// AUTHOR: Oscar Kramer
//
// DESCRIPTION: Contains declaration of class ossimGeoidManager. Maintains
//   a list of geoids.
//
// LIMITATIONS: None.
//
//*****************************************************************************
//  $Id: ossimGeoidManager.h 10216 2007-01-09 17:33:01Z dburken $

#ifndef ossimGeoidManager_HEADER
#define ossimGeoidManager_HEADER

#include <vector>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimGeoid.h>

/*!****************************************************************************
 *
 * CLASS: ossimGeoidManager 
 *
 *****************************************************************************/
class OSSIMDLLEXPORT ossimGeoidManager : public ossimGeoid
{
public:


   virtual ~ossimGeoidManager();
      
   /*!
    * Implements singelton pattern:
    */
   static ossimGeoidManager* instance();

   
   /*!
    * Permits initialization of geoids from directory name. Should never be
    * called since called on specific geoid types:
    */
   virtual bool open(const ossimFilename& dir, ossimByteOrder byteOrder);

   /*!
    *  Returns the offset from the ellipsoid to the geoid.
    *  Returns OSSIM_DBL_NAN if grid does not contain the point.
    */
   virtual double offsetFromEllipsoid(const ossimGpt& gpt) const;

   /*!
    * Method to save the state of the object to a keyword list.
    * Return true if ok or false on error. DO NOTHING
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0) const;

   /*!
    * Method to the load (recreate) the state of the object from a keyword
    * list.  Return true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);

   /*!
    * Permits adding additional geoids to the list: 
    */
   virtual void addGeoid(ossimRefPtr<ossimGeoid> geoid);

   const ossimRefPtr<ossimGeoid> findGeoidByShortName(const ossimString& shortName,
                                                      bool caseSensitive=true)const;
private:
   /*!
    *  Private constructor.  Use "instance" method.
    */
   ossimGeoidManager();

   static ossimGeoidManager* theInstance;
   mutable std::vector< ossimRefPtr<ossimGeoid> > theGeoidList;

   TYPE_DATA
};

#endif
