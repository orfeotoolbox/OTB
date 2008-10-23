//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts
// Description:
//
//*************************************************************************
// $Id: ossimPropertyInterfaceFactory.h 9968 2006-11-29 14:01:53Z gpotts $
#ifndef ossimPropertyInterfaceFactory_HEADER
#define ossimPropertyInterfaceFactory_HEADER


#include <ossim/base/ossimRtti.h>
#include <ossim/base/ossimObjectFactory.h>
class OSSIMDLLEXPORT ossimObject;
class OSSIMDLLEXPORT ossimPropertyInterface;

class OSSIMDLLEXPORT ossimPropertyInterfaceFactory : public ossimObjectFactory
{
public:
   ossimPropertyInterfaceFactory()
      :ossimObjectFactory()
      {}
   virtual ~ossimPropertyInterfaceFactory()
      {}

   virtual ossimPropertyInterface* createInterface( const ossimObject* obj ) const = 0;

   
TYPE_DATA
};

#endif
