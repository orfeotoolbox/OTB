//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------
// $Id$

#ifndef ossimSrtmElevationDatabase_HEADER
#define ossimSrtmElevationDatabase_HEADER 1
#include "ossimElevationDatabase.h"
#include <ossim/base/ossimFilename.h>
#include <ossim/elevation/ossimSrtmHandler.h>
#include <OpenThreads/Mutex>

class OSSIM_DLL ossimSrtmElevationDatabase : public ossimElevationCellDatabase
{
public:
   typedef std::vector<ossimRefPtr<CellInfo> > DirectMap; // 360x180 cell grid
   ossimSrtmElevationDatabase()
   :ossimElevationCellDatabase()
   {
   }
   ossimSrtmElevationDatabase(const ossimSrtmElevationDatabase& rhs)
   :ossimElevationCellDatabase(rhs)
   {
   }
   virtual ~ossimSrtmElevationDatabase()
   {
   }
   ossimObject* dup()const
   {
      return new ossimSrtmElevationDatabase(*this);
   }
   virtual bool open(const ossimString& connectionString);
   virtual bool pointHasCoverage(const ossimGpt& gpt) const
   {
      ossimFilename filename;
      createFullPath(filename, gpt);
      
      return filename.exists();
   }
   /**
    * METHODS: accuracyLE90(), accuracyCE90()
    * Returns the vertical and horizontal accuracy (90% confidence) in the
    * region of gpt:
    */
   virtual double getAccuracyLE90(const ossimGpt& /* gpt */) const
   {
      std::cout << "ossimSrtmDatabase::getAccuracyLE90 \n";
      return 0.0;
   }
   virtual double getAccuracyCE90(const ossimGpt& /* gpt */) const
   {
      std::cout << "ossimSrtmDatabase::getAccuracyCE90 \n";
      return 0.0;
   }
   virtual double getHeightAboveMSL(const ossimGpt&);
   virtual double getHeightAboveEllipsoid(const ossimGpt& gpt);
   virtual ossim_uint64 createId(const ossimGpt& pt)const
   {
      ossim_uint64 y = static_cast<ossim_uint64>(ossim::wrap(pt.latd(), -90.0, 90.0)+90.0);
      ossim_uint64 x = static_cast<ossim_uint64>(ossim::wrap(pt.lond(),-180.0,180.0)+180.0);
      // map the extreme edge to the same ID ax the 179 west cell and the same for the 89
      // degree north cell.
      //
      x = x==360?359:x;
      y = y==180?179:y;
      // Srtm databases are 1x1 degree cells and we will use a world 
      // grid for id generation.
      //
      return (y*360+x);
   }
   virtual bool loadState(const ossimKeywordlist& kwl, const char* prefix = 0);
   virtual bool saveState(ossimKeywordlist& kwl, const char* prefix = 0)const;
   
protected:
   bool openSrtmDirectory(const ossimFilename& dir);
   void createRelativePath(ossimFilename& file, const ossimGpt& gpt)const;
   void createFullPath(ossimFilename& file, const ossimGpt& gpt)const
   {
      ossimFilename relativeFile;
      createRelativePath(relativeFile, gpt);
      file = ossimFilename(m_connectionString).dirCat(relativeFile);
   }

   ossimRefPtr<ossimElevCellHandler> createCell(const ossimGpt& gpt);

   TYPE_DATA
};
#endif
