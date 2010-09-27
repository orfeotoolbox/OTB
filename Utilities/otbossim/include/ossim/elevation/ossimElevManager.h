//*****************************************************************************
// FILE: ossimElevManager.h
//
// License:  See top level LICENSE.txt file.
//
// DESCRIPTION:
//   Contains declaration of class ossimElevManager. This object provides a
//   single interface to an imaging chain for accessing multiple elevation
//   sources. This object owns one or more elevation sources in an ordered
//   list. When queried for an elevation at a particular point, it searches
//   the available sources for the best result, instantiating new sources if
//   necessary.
//
// SOFTWARE HISTORY:
//>
//   13Apr2001  Oscar Kramer
//              Initial coding.
//<
//*****************************************************************************
#ifndef ossimElevManager_HEADER
#define ossimElevManager_HEADER
#include <vector>
#include <ossim/base/ossimConstants.h>
#include <ossim/elevation/ossimElevSource.h>
#include <ossim/elevation/ossimElevationDatabase.h>
#include <OpenThreads/ReadWriteMutex>
class OSSIM_DLL ossimElevManager : public ossimElevSource
{
public: 
   typedef std::vector<ossimRefPtr<ossimElevationDatabase> > ElevationDatabaseListType;
   virtual ~ossimElevManager();
   
   /**
    * METHOD: instance()
    * Implements singelton pattern
    */
   static ossimElevManager* instance();
   
   virtual double getHeightAboveEllipsoid(const ossimGpt& gpt);
   virtual double getHeightAboveMSL(const ossimGpt& gpt);
   virtual double getAccuracyLE90(const ossimGpt& /*gpt*/) const
   {
      std::cout << "ossimElevManager::getAccuracyLE90(): NOT IMPLEMENTED!!!\n";
      return 0.0;
   }
   virtual double getAccuracyCE90(const ossimGpt& /*gpt*/) const
   {
      return 0.0;
      std::cout << "ossimElevManager::getAccuracyLE90(): NOT IMPLEMENTED!!!\n";
   }
   virtual bool pointHasCoverage(const ossimGpt& /*gpt*/) const
   {
      std::cout << "ossimElevManager::pointHasCoverage(): NOT IMPLEMENTED!!!\n";
      return false;
   }
   virtual double getMeanSpacingMeters() const
   {
      std::cout << "ossimElevManager::pointHasCoverage(): NOT IMPLEMENTED AND SHOULD NOT BE USED AT THIS LEVEL!!!\n";
      return 1.0;
   }
   ossim_uint32 getNumberOfElevationDatabases()const
   {
      return (ossim_uint32)m_elevationDatabaseList.size();
   }
   ossimElevationDatabase* getElevationDatabase(ossim_uint32 idx)
   {
      return m_elevationDatabaseList[idx].get();
   }
   const ossimElevationDatabase* getElevationDatabase(ossim_uint32 idx)const
   {
      return m_elevationDatabaseList[idx].get();
   }
   ElevationDatabaseListType& getElevationDatabaseList()
   {
      return m_elevationDatabaseList;
   }
   const ElevationDatabaseListType& getElevationDatabaseList()const
   {
      return m_elevationDatabaseList;
   }
   void addDatabase(ossimElevationDatabase* database);
   bool loadElevationPath(const ossimFilename& path);
   
   void setDefaultHeightAboveEllipsoid(double meters) {m_defaultHeightAboveEllipsoid=meters;}
   void setElevationOffset(double meters) {m_elevationOffset=meters;}
   double getElevationOffset() const { return m_elevationOffset; }
   
   void getOpenCellList(std::vector<ossimFilename>& list) const;

   void setUseGeoidIfNullFlag(bool flag)
   {
      m_useGeoidIfNullFlag = flag;
   }
   
   bool getUseGeoidIfNullFlag()const
   {
      return m_useGeoidIfNullFlag;
   }
   void clear();
   /**
    * Method to save the state of an object to a keyword list.
    * Return true if ok or false on error.
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0) const;
   
   /**
    * Method to the load (recreate) the state of an object from a keyword
    * list.  Return true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);
   
protected:
   ossimElevManager();
   void loadStandardElevationPaths();
   
   static ossimElevManager* m_instance;
   ElevationDatabaseListType m_elevationDatabaseList;
   ossim_float64 m_defaultHeightAboveEllipsoid;
   ossim_float64 m_elevationOffset;
   
   // if an elevation is returned that's null for ellipsoid then use the geoid manager to calculate a shift
   //
   bool          m_useGeoidIfNullFlag; 
   
   
   /**
    * I have tried the readwrite lock interfaces but have found it unstable.  I am using the standard Mutex
    * and it seems to be much more stable across all platforms.  More testing needs to occur for the ReadWriteMutex.
    * For now we will use Mutex.
    */
   OpenThreads::Mutex m_mutex;
};

#endif
