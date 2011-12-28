//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  Mickael Savinaud <mickael.savinaud@c-s.fr>
//          based on Spot5 model
//
// Description:
//
// Contains declaration of class ossimPleiadesModel.
//
//*****************************************************************************
// FIXME $Id: ossimSpot5Model.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimPleiadesModel_HEADER
#define ossimPleiadesModel_HEADER

#include <ossimPluginConstants.h>

#include <ossim/projection/ossimRpcModel.h>
#include "ossimPleiadesDimapSupportData.h"

#include <ossim/base/ossimFilename.h>

#include <iostream>


namespace ossimplugins
{
class ossimPleiadesDimapSupportData;

class OSSIM_PLUGINS_DLL ossimPleiadesModel : public ossimRpcModel
{
public:
   /*!
    * CONSTRUCTORS:
    */
    /** @brief default constructor */
    ossimPleiadesModel();

    /** @brief copy constructor */
    ossimPleiadesModel(const ossimPleiadesModel& rhs);

    /** @brief Destructor */
   virtual ~ossimPleiadesModel();

   bool open(const ossimFilename& file);

   /*!
    * Returns pointer to a new instance, copy of this.
    * Not implemented yet!  Returns NULL...
    */
   virtual ossimObject* dup() const;

   /*!
    * Extends base-class implementation. Dumps contents of object to ostream.
    */
   virtual std::ostream& print(std::ostream& out) const;

   /*!
    * Fulfills ossimObject base-class pure virtuals. Loads and saves geometry
    * KWL files. Returns true if successful.
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=NULL) const;

   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=NULL);

   void setSupportData(ossimPleiadesDimapSupportData* supportData)
   {
      theSupportData = supportData;
   }
   ossimPleiadesDimapSupportData* getSupportData()
   {
      return theSupportData.get();
   }
   const ossimPleiadesDimapSupportData* getSupportData()const
   {
      return theSupportData.get();
   }

protected:

   bool parseMetaData(const ossimFilename& file);

   bool parseRpcData (const ossimFilename& file);

   // In the future
   //bool parseJP2File (const ossimFilename& file);

   void finishConstruction();

   ossimRefPtr<ossimPleiadesDimapSupportData> theSupportData;

   ossimFilename _productXmlFile;


TYPE_DATA
};
}
#endif /* #ifndef ossimPleiadesModel_HEADER */
