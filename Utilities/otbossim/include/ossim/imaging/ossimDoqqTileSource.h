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
//  $Id: ossimDoqqTileSource.h 17932 2010-08-19 20:34:35Z dburken $
#ifndef ossimDoqqTileSource_HEADER
#define ossimDoqqTileSource_HEADER
#include <ossim/imaging/ossimGeneralRasterTileSource.h>
#include <ossim/support_data/ossimDoqq.h>

class OSSIM_DLL ossimDoqqTileSource: public ossimGeneralRasterTileSource
{
public:
   ossimDoqqTileSource();
   
   ossimString getShortName()const;
   ossimString getLongName()const;
   
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix = NULL);
   
   //! Returns the image geometry object associated with this tile source or NULL if non defined.
   //! The geometry contains full-to-local image transform as well as projection (image-to-world)
   virtual ossimRefPtr<ossimImageGeometry> getImageGeometry();

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
   virtual ~ossimDoqqTileSource();
  ossimRefPtr<ossimDoqq>   theHeaderInformation;
  ossimString  theAcqDate;
   
TYPE_DATA
};

#endif
