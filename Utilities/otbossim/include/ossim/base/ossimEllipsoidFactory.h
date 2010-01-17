//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
// Description:
//
// Thie file contains the ossimEllipsoidFactory.
//*******************************************************************
//  $Id: ossimEllipsoidFactory.h 9094 2006-06-13 19:12:40Z dburken $

#ifndef ossimEllipsoidFactory_HEADER
#define ossimEllipsoidFactory_HEADER
#include <map>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimString.h>

class ossimEllipsoid;
class ossimWgs84Ellipsoid;
class ossimWgs72Ellipsoid;

class OSSIMDLLEXPORT ossimEllipsoidFactory
{
public:
   typedef std::map<ossimString, ossimEllipsoid*> TableType;
   virtual ~ossimEllipsoidFactory();
   const ossimEllipsoid* create(const ossimString &code)const;
   const ossimEllipsoid* wgs84()const{return theWgs84Ellipsoid;}
   const ossimEllipsoid* wgs72()const{return theWgs72Ellipsoid;}

   static ossimEllipsoidFactory* instance();
   
private:
   static ossimEllipsoidFactory*     theInstance;
   ossimEllipsoid*       theWgs84Ellipsoid;
   ossimEllipsoid*       theWgs72Ellipsoid;
   TableType             theEllipsoidTable;
   ossimEllipsoidFactory();
   void initializeTable();
   void deleteAll();
};

#endif
