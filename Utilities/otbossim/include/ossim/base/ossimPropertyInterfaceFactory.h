//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts
// Description:
//
//*************************************************************************
// $Id: ossimPropertyInterfaceFactory.h 14789 2009-06-29 16:48:14Z dburken $
#ifndef ossimPropertyInterfaceFactory_HEADER
#define ossimPropertyInterfaceFactory_HEADER


#include <ossim/base/ossimRtti.h>
#include <ossim/base/ossimObjectFactory.h>
class ossimObject;
class ossimPropertyInterface;

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
