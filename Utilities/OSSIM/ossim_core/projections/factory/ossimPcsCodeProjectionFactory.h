//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
// 
// Author:  David Burken
//
// Description:
//
// Class declaration of projection factory to return projection from
// a Projection Coordinate System (PCS) code.
//
// For current documentation on pcs codes see:
// 
// http://www.remotesensing.org/geotiff/spec/geotiff6.html#6.3.3.1
// 
//----------------------------------------------------------------------------
//  $Id: ossimPcsCodeProjectionFactory.h,v 1.1 2005/11/25 20:30:48 dburken Exp $

#ifndef ossimPcsCodeProjectionFactory_HEADER
#define ossimPcsCodeProjectionFactory_HEADER

#include <projections/factory/ossimProjectionFactoryBase.h>

class ossimProjection;
class ossimString;

class OSSIMDLLEXPORT ossimPcsCodeProjectionFactory : public ossimProjectionFactoryBase
{
public:
   /*!
    * METHOD: instance()
    * Instantiates singleton instance of this class:
    */
   static ossimPcsCodeProjectionFactory* instance();

   virtual ossimProjection* createProjection(const ossimFilename& filename,
                                             ossim_uint32 entryIdx)const;
   /*!
    * METHOD: create()
    * Attempts to create an instance of the projection specified by name.
    * Returns successfully constructed projection or NULL.
    */

   /**
    * Creates projection from a pcs code string.
    *
    * @return ossimProjection* or null if code is not handled.
    */
   virtual ossimProjection* createProjection(const ossimString& name)const;
   
   virtual ossimProjection* createProjection(const ossimKeywordlist& kwl,
                                             const char* prefix = 0)const;

   virtual ossimObject* createObject(const ossimString& typeName)const;

   /*!
    * Creates and object given a keyword list.
    */
   virtual ossimObject* createObject(const ossimKeywordlist& kwl,
                                     const char* prefix=0)const;
   
   /*!
    * This should return the type name of all objects in all factories.
    * This is the name used to construct the objects dynamially and this
    * name must be unique.
    */
   virtual void getTypeNameList(std::vector<ossimString>& typeList)const;

   /*!
    * METHOD: getList()
    * Returns list of all projections represented by this factory:
    */
   virtual list<ossimString> getList()const;

protected:
   ossimPcsCodeProjectionFactory();
   
   static ossimPcsCodeProjectionFactory*  theInstance;
 
};

#endif
