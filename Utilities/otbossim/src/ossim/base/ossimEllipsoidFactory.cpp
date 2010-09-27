//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
// 
//*******************************************************************
//  $Id: ossimEllipsoidFactory.cpp 17815 2010-08-03 13:23:14Z dburken $

#include <ossim/base/ossimEllipsoidFactory.h>
#include <ossim/base/ossimEllipsoid.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimString.h>

ossimEllipsoidFactory* ossimEllipsoidFactory::theInstance = 0;

ossimEllipsoidFactory* ossimEllipsoidFactory::instance()
{
   if(!theInstance)
   {
      theInstance = new ossimEllipsoidFactory;
      theInstance->initializeTable();
   }
 
   return theInstance; 
} 

ossimEllipsoidFactory::ossimEllipsoidFactory()
{
}

ossimEllipsoidFactory::~ossimEllipsoidFactory()
{
   deleteAll();
   theInstance = 0;
}

const ossimEllipsoid* ossimEllipsoidFactory::create(const ossimString &code)const
{
   // Can be 2-letter code or EPSG integer code for ellipsoids (7000-series). Make sure it is alpha:
   if ( code.size() )
   {
      ossimString alphaCode;

      ossim_uint32 epsg_code = code.toUInt32();
      if (epsg_code)
      {
         // EPSG was specified, fetch the equivalent alpha code from the map:
         EpsgTableType::const_iterator iter = theEpsgTable.find(epsg_code);
         if (iter != theEpsgTable.end())
         {
            alphaCode = (*iter).second;
         }
      }
      else
      {
         // Alpha code was specified, try to establish an equivalent EPSG code:
         alphaCode = code;
         epsg_code = findEpsgCode(alphaCode);
      }

      // Established two-letter code. Now fetch the ellipsoid instance:
      ossimEllipsoid* ellipsoid = 0;
      TableType::const_iterator iter = theEllipsoidTable.find(alphaCode);
      if(iter != theEllipsoidTable.end())
      {
         ellipsoid = iter->second;
         if (ellipsoid)
         {
            ellipsoid->setEpsgCode(epsg_code);
         }
         return ellipsoid;
      }
      else
      {
         ossimNotify(ossimNotifyLevel_WARN) << "ossimEllipsoidFactory::create WARNING:"
            << "\nDid not create ellipsoid for code: " << code << std::endl;
      }
   }
   
   return 0;
}

void ossimEllipsoidFactory::initializeTable()
{
   if(theEllipsoidTable.size()<1)
   {
      theEllipsoidTable.insert(std::make_pair(ossimString("AA"),
         new ossimEllipsoid(ossimString("Airy"), ossimString("AA"), 6377563.396, 6356256.9090)));   
      theEllipsoidTable.insert(std::make_pair(ossimString("AM"),
         new ossimEllipsoid(ossimString("Modified Airy"), ossimString("AM"), 6377340.189, 6356034.448)));
      theEllipsoidTable.insert(std::make_pair(ossimString("AN"),
         new ossimEllipsoid(ossimString("Australian National"), ossimString("AN"), 6378160.000, 6356774.7190)));
      theEllipsoidTable.insert(std::make_pair(ossimString("BR"),
         new ossimEllipsoid(ossimString("Bessel 1841"), ossimString("BR"), 6377397.155, 6356078.9630)));
      theEllipsoidTable.insert(std::make_pair(ossimString("BN"),
         new ossimEllipsoid(ossimString("Bessel 1841(Namibia)"), ossimString("BN"), 6377483.865, 6356165.3830)));
      theEllipsoidTable.insert(std::make_pair(ossimString("CC"),
         new ossimEllipsoid(ossimString("Clarke 1866"), ossimString("CC"), 6378206.400, 6356583.8000)));
      theEllipsoidTable.insert(std::make_pair(ossimString("CD"),
         new ossimEllipsoid(ossimString("Clarke 1880"), ossimString("CD"), 6378249.145, 6356514.8700)));
      theEllipsoidTable.insert(std::make_pair(ossimString("CE"),
         new ossimEllipsoid(ossimString("Clarke 1880 (IGN)EPSG 7011"), ossimString("CE"), 6378249.200, 6356515.0000)));
      theEllipsoidTable.insert(std::make_pair(ossimString("EA"),
         new ossimEllipsoid(ossimString("Everest"), ossimString("EA"), 6377276.345, 6356075.4130)));
      theEllipsoidTable.insert(std::make_pair(ossimString("EB"),
         new ossimEllipsoid(ossimString("Everest (E. Malasia, Brunei)"), ossimString("EB"), 6377298.556, 6356097.55)));
      theEllipsoidTable.insert(std::make_pair(ossimString("EC"),
         new ossimEllipsoid(ossimString("Everest 1956 (India)"), ossimString("EC"), 6377301.243, 6356100.228)));
      theEllipsoidTable.insert(std::make_pair(ossimString("ED"),
         new ossimEllipsoid(ossimString("Everest 1969 (West Malasia)"), ossimString("ED"), 6377295.664, 6356094.668)));
      theEllipsoidTable.insert(std::make_pair(ossimString("EE"),
         new ossimEllipsoid(ossimString("Everest 1948(W.Mals. & Sing.)"), ossimString("EE"), 6377304.063, 6356103.039)));
      theEllipsoidTable.insert(std::make_pair(ossimString("EF"),
         new ossimEllipsoid(ossimString("Everest (Pakistan)"), ossimString("EF"), 6377309.613, 6356109.571)));
      theEllipsoidTable.insert(std::make_pair(ossimString("FA"),
         new ossimEllipsoid(ossimString("Mod. Fischer 1960(South Asia)"), ossimString("FA"), 6378155.0, 6356773.32)));
      theEllipsoidTable.insert(std::make_pair(ossimString("RF"),
         new ossimEllipsoid(ossimString("GRS 80"), ossimString("RF"), 6378137.0, 6356752.3141)));
      theEllipsoidTable.insert(std::make_pair(ossimString("HE"),
         new ossimEllipsoid(ossimString("Helmert 1906"), ossimString("HE"), 6378200.0, 6356818.17)));
      theEllipsoidTable.insert(std::make_pair(ossimString("HO"),
         new ossimEllipsoid(ossimString("Hough"), ossimString("HO"), 6378270.0, 6356794.3430)));
      theEllipsoidTable.insert(std::make_pair(ossimString("ID"),
         new ossimEllipsoid(ossimString("Indonesian 1974"), ossimString("ID"), 6378160.0, 6356774.504)));
      theEllipsoidTable.insert(std::make_pair(ossimString("IN"),
         new ossimEllipsoid(ossimString("International 1924"), ossimString("IN"), 6378388.0, 6356911.946)));
      theEllipsoidTable.insert(std::make_pair(ossimString("KA"),
         new ossimEllipsoid(ossimString("Krassovsky"), ossimString("KA"), 6378245.0, 6356863.0190)));
      theEllipsoidTable.insert(std::make_pair(ossimString("SA"),
         new ossimEllipsoid(ossimString("South American 1969"),ossimString("SA"), 6378160.0, 6356774.719)));
      theEllipsoidTable.insert(std::make_pair(ossimString("WD"),
         new ossimEllipsoid(ossimString("WGS 72"), ossimString("WD"), 6378135.000, 6356750.5200)));

      theEllipsoidTable.insert(std::make_pair(ossimString("WE"),
         new ossimEllipsoid(ossimString("WGS 84"), ossimString("WE"), 6378137.000, 6356752.3142)));
      
      theEllipsoidTable.insert(std::make_pair(ossimString("PV"),
         new ossimEllipsoid(ossimString("Popular Visualisation Sphere"), ossimString("7059"), 6378137.000, 6378137.000)));

      theWgs84Ellipsoid = new ossimEllipsoid(ossimString("WGS 84"), ossimString("WE"), 6378137.000, 6356752.3142);
      theWgs72Ellipsoid = new ossimEllipsoid(ossimString("WGS 72"), ossimString("WD"), 6378135.000, 6356750.5200);

      // This table bridges between EPSG 7000-series codes to two-letter code:
      theEpsgTable.insert(std::make_pair(7001, ossimString("AA")));   
      theEpsgTable.insert(std::make_pair(7002, ossimString("AM")));
      theEpsgTable.insert(std::make_pair(7003, ossimString("AN")));
      theEpsgTable.insert(std::make_pair(7004, ossimString("BR")));
      theEpsgTable.insert(std::make_pair(7006, ossimString("BN")));
      theEpsgTable.insert(std::make_pair(7008, ossimString("CC")));
      theEpsgTable.insert(std::make_pair(7012, ossimString("CD")));
      theEpsgTable.insert(std::make_pair(7011, ossimString("CE")));
      theEpsgTable.insert(std::make_pair(7015, ossimString("EA")));
      theEpsgTable.insert(std::make_pair(7016, ossimString("EB")));
      theEpsgTable.insert(std::make_pair(7044, ossimString("EC")));
      theEpsgTable.insert(std::make_pair(7056, ossimString("ED")));
      theEpsgTable.insert(std::make_pair(7018, ossimString("EE")));
      theEpsgTable.insert(std::make_pair(7019, ossimString("RF")));
      theEpsgTable.insert(std::make_pair(7020, ossimString("HE")));
      theEpsgTable.insert(std::make_pair(7053, ossimString("HO")));
      theEpsgTable.insert(std::make_pair(7021, ossimString("ID")));
      theEpsgTable.insert(std::make_pair(7022, ossimString("IN")));
      theEpsgTable.insert(std::make_pair(7024, ossimString("KA")));
      theEpsgTable.insert(std::make_pair(7003, ossimString("SA"))); // Same as "AN"
      theEpsgTable.insert(std::make_pair(7043, ossimString("WD")));
      theEpsgTable.insert(std::make_pair(7030, ossimString("WE")));
      theEpsgTable.insert(std::make_pair(7059, ossimString("PV")));
   }
}

void ossimEllipsoidFactory::deleteAll()
{
   TableType::const_iterator ellipsoid = theEllipsoidTable.begin();
   
   while(ellipsoid != theEllipsoidTable.end())
   {
      delete (*ellipsoid).second;
      ++ellipsoid;
   }

   theEllipsoidTable.clear();
   theEpsgTable.clear();

   if (theWgs84Ellipsoid)
   {
      delete theWgs84Ellipsoid;
      theWgs84Ellipsoid = 0;
   }
   if (theWgs72Ellipsoid)
   {
      delete theWgs72Ellipsoid;
      theWgs72Ellipsoid = 0;
   }
}

//*************************************************************************************************
// Given an alpha code (for example "WE" for WGS84), returns the corresponding EPSG code or 0
// if not found.
//*************************************************************************************************
ossim_uint32 ossimEllipsoidFactory::findEpsgCode(const ossimString& alpha_code) const
{
   ossim_uint32 epsg_code = 0;
   if (alpha_code.empty())
      return 0;

   EpsgTableType::const_iterator iter = theEpsgTable.begin();
   while ((iter != theEpsgTable.end()) && (epsg_code == 0))
   {
      if (iter->second == alpha_code)
      {
         epsg_code = iter->first;
      }
      iter++;
   }
   return epsg_code;
}
