//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks)
//
//*************************************************************************
// $Id: ossimMapViewController.h 9968 2006-11-29 14:01:53Z gpotts $
#ifndef ossimMapViewController_HEADER
#define ossimMapViewController_HEADER
#include <ossim/base/ossimViewController.h>
#include <ossim/base/ossimFilename.h>

class ossimMapViewController : public ossimViewController
{
public:
   ossimMapViewController();
   ossimMapViewController(ossimObject* owner,
                          ossim_uint32 inputListSize,
                          ossim_uint32 outputListSize,
                          bool         inputListFixedFlag=true,
                          bool         outputListFixedFlag=false);

   virtual ~ossimMapViewController();
   virtual bool setView(ossimObject* object);
   virtual void changeOwner(ossimObject* owner);
   virtual void setFilename(const ossimFilename& file);
   virtual const ossimFilename& getFilename()const
      {
         return theGeometryFile;
      }
   
   virtual ossimString getShortName()const
      {
         return ossimString("Map View");
      }
   
   virtual ossimString getLongName()const
      {
         return ossimString("Map View controller");
      }
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;

   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);
protected:

   /*!
    * If the file is not "" (empty) then it will
    * use this filename to load and save the geometry
    * projection.
    */
   ossimFilename theGeometryFile;
   
TYPE_DATA
};
#endif
