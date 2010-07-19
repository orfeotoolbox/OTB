//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts
// 
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfTagFactory.h 17207 2010-04-25 23:21:14Z dburken $
#ifndef ossimNitfTagFactory_HEADER
#define ossimNitfTagFactory_HEADER

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimRefPtr.h>

class ossimNitfRegisteredTag;

class OSSIM_DLL ossimNitfTagFactory : public ossimObject
{
public:
   ossimNitfTagFactory();
   virtual ~ossimNitfTagFactory();
   virtual ossimRefPtr<ossimNitfRegisteredTag> create(const ossimString &tagName)const=0;
private:
   /*!
    * Hide this.
    */
   ossimNitfTagFactory(const ossimNitfTagFactory & /* rhs */){}

   /*!
    * Hide this.
    */ 
   ossimNitfTagFactory& operator =(const ossimNitfTagFactory & /* rhs */){return *this;}
TYPE_DATA
};
#endif
