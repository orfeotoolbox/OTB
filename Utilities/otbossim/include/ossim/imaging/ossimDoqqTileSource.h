//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
//
//*******************************************************************
//  $Id: ossimDoqqTileSource.h 12988 2008-06-04 16:49:43Z gpotts $
#ifndef ossimDoqqTileSource_HEADER
#define ossimDoqqTileSource_HEADER
#include <ossim/imaging/ossimGeneralRasterTileSource.h>
#include <ossim/support_data/ossimDoqq.h>

class OSSIM_DLL ossimDoqqTileSource: public ossimGeneralRasterTileSource
{
public:
   ossimDoqqTileSource();
   virtual ~ossimDoqqTileSource();
   
   ossimString getShortName()const;
   ossimString getLongName()const;
   
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix = NULL);
   
   virtual bool getImageGeometry(ossimKeywordlist& kwl,
                                 const char* prefix=0);
   bool open(const ossimFilename& filename);
   bool open(const ossimGeneralRasterInfo& info);

   ossimString acqdate() const;
	
   /**
    * @brief Gets a property for matching name.
    * @param name The name of the property to get.
    * @return Returns property matching "name".
    */
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   
   /**
    * @brief Gets a list of property names available.
    * @param propertyNames The list to push back names to.
    */
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;
	
protected:
  ossimRefPtr<ossimDoqq>   theHeaderInformation;
  ossimString  theAcqDate;
   
TYPE_DATA
};

#endif
