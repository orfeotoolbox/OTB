//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file
//
// Author:  Mingjie Su
//
// Description: Interface class for ESRI Shape file.
//
//----------------------------------------------------------------------------
// $Id: ossimEsriShapeFileInterface.h 1916 2010-12-16 16:08:33Z ming.su $

#ifndef ossimEsriShapeFileInterface_HEADER
#define ossimEsriShapeFileInterface_HEADER

//Std Includes
#include <map>

//Ossim Includes
#include <ossim/base/ossimConstants.h> /* for OSSIM_DLL macro */
#include <ossim/base/ossimRtti.h>

class ossimAnnotationObject;
class ossimString;

class OSSIMDLLEXPORT ossimEsriShapeFileInterface
{
public:
   
   /**
    * Pure virtual getFeatureTable method.
    *
    */
   virtual std::multimap<long, ossimAnnotationObject*> getFeatureTable() = 0;

   /**
    * Pure virtual setQuery method.
    *
    */
   virtual void setQuery(const ossimString& query) = 0;

   /**
    * Pure virtual setGeometryBuffer method.
    *
    */
   virtual void setGeometryBuffer(ossim_float64 distance, ossimUnitType type) = 0;
  
   TYPE_DATA
};

#endif /* #ifndef ossimEsriShapeFileInterface_HEADER */
