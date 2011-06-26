//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
// 
//*******************************************************************
//  $Id: ossimEllipsoidFactory.cpp 19682 2011-05-31 14:21:20Z dburken $

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
   if ( code.empty() )
      return 0;

   const ossimEllipsoid* ellipsoid = 0;
   ossimString alphaCode = code;

   // Check if the code passed in was actually numeric EPSG (vs. two char alpha code):
      ossim_uint32 epsg_code = code.toUInt32();
      if (epsg_code)
      {
         // EPSG was specified, fetch the equivalent alpha code from the map:
         EpsgTableType::const_iterator iter = theEpsgTable.find(epsg_code);
         if (iter != theEpsgTable.end())
            alphaCode = (*iter).second;
         }

      // Established two-letter code. Now fetch the ellipsoid instance:
      TableType::const_iterator iter = theEllipsoidTable.find(alphaCode);
      if(iter != theEllipsoidTable.end())
         ellipsoid = iter->second;
      else
      {
         ossimNotify(ossimNotifyLevel_WARN) << "ossimEllipsoidFactory::create WARNING:"
            << "\nDid not create ellipsoid for code: " << code << std::endl;
      }
   
   return ellipsoid;
}

void ossimEllipsoidFactory::initializeTable()
{
   // Do nothing if tables were already initialized:
   if (!theEllipsoidTable.empty())
      return;

   // This table bridges between EPSG 7000-series codes to two-letter code:
   theEpsgTable.insert(std::make_pair(7001, std::string("AA")));   
   theEpsgTable.insert(std::make_pair(7002, std::string("AM")));
   theEpsgTable.insert(std::make_pair(7003, std::string("AN")));
   theEpsgTable.insert(std::make_pair(7004, std::string("BR")));
   theEpsgTable.insert(std::make_pair(7006, std::string("BN")));
   theEpsgTable.insert(std::make_pair(7008, std::string("CC")));
   theEpsgTable.insert(std::make_pair(7012, std::string("CD")));
   theEpsgTable.insert(std::make_pair(7011, std::string("CE")));
   theEpsgTable.insert(std::make_pair(7015, std::string("EA")));
   theEpsgTable.insert(std::make_pair(7016, std::string("EB")));
   theEpsgTable.insert(std::make_pair(7044, std::string("EC")));
   theEpsgTable.insert(std::make_pair(7056, std::string("ED")));
   theEpsgTable.insert(std::make_pair(7018, std::string("EE")));
   theEpsgTable.insert(std::make_pair(7019, std::string("RF")));
   theEpsgTable.insert(std::make_pair(7020, std::string("HE")));
   theEpsgTable.insert(std::make_pair(7053, std::string("HO")));
   theEpsgTable.insert(std::make_pair(7021, std::string("ID")));
   theEpsgTable.insert(std::make_pair(7022, std::string("IN")));
   theEpsgTable.insert(std::make_pair(7024, std::string("KA")));
   theEpsgTable.insert(std::make_pair(7003, std::string("SA"))); // Same as "AN"
   theEpsgTable.insert(std::make_pair(7043, std::string("WD")));
   theEpsgTable.insert(std::make_pair(7030, std::string("WE")));
   theEpsgTable.insert(std::make_pair(7059, std::string("PV")));

      theEllipsoidTable.insert(std::make_pair(std::string("AA"),
      new ossimEllipsoid(std::string("Airy"), std::string("AA"), 
      6377563.396, 6356256.9090, 7001)));   
      theEllipsoidTable.insert(std::make_pair(std::string("AM"),
      new ossimEllipsoid(std::string("Modified Airy"), std::string("AM"), 
      6377340.189, 6356034.448, 7002)));
      theEllipsoidTable.insert(std::make_pair(std::string("AN"),
      new ossimEllipsoid(std::string("Australian National"), std::string("AN"), 
      6378160.000, 6356774.7190, 7003)));
      theEllipsoidTable.insert(std::make_pair(std::string("BR"),
      new ossimEllipsoid(std::string("Bessel 1841"), std::string("BR"), 
      6377397.155, 6356078.9630, 7004)));
      theEllipsoidTable.insert(std::make_pair(std::string("BN"),
      new ossimEllipsoid(std::string("Bessel 1841(Namibia)"), std::string("BN"), 
      6377483.865, 6356165.3830, 7006)));
      theEllipsoidTable.insert(std::make_pair(std::string("CC"),
      new ossimEllipsoid(std::string("Clarke 1866"), std::string("CC"), 
      6378206.400, 6356583.8000, 7008)));
      theEllipsoidTable.insert(std::make_pair(std::string("CD"),
      new ossimEllipsoid(std::string("Clarke 1880"), std::string("CD"), 
      6378249.145, 6356514.8700, 7012)));
      theEllipsoidTable.insert(std::make_pair(std::string("CE"),
      new ossimEllipsoid(std::string("Clarke 1880 (IGN)EPSG 7011"), std::string("CE"), 
      6378249.200, 6356515.0000, 7011)));
      theEllipsoidTable.insert(std::make_pair(std::string("EA"),
      new ossimEllipsoid(std::string("Everest"), std::string("EA"), 
      6377276.345, 6356075.4130, 7015)));
      theEllipsoidTable.insert(std::make_pair(std::string("EB"),
      new ossimEllipsoid(std::string("Everest (E. Malasia, Brunei)"), std::string("EB"), 
      6377298.556, 6356097.55, 7016)));
      theEllipsoidTable.insert(std::make_pair(std::string("EC"),
      new ossimEllipsoid(std::string("Everest 1956 (India)"), std::string("EC"), 
      6377301.243, 6356100.228, 7044)));
      theEllipsoidTable.insert(std::make_pair(std::string("ED"),
      new ossimEllipsoid(std::string("Everest 1969 (West Malasia)"), std::string("ED"), 
      6377295.664, 6356094.668, 7056)));
      theEllipsoidTable.insert(std::make_pair(std::string("EE"),
      new ossimEllipsoid(std::string("Everest 1948(W.Mals. & Sing.)"), std::string("EE"), 
      6377304.063, 6356103.039, 7018)));
      theEllipsoidTable.insert(std::make_pair(std::string("EF"),
      new ossimEllipsoid(std::string("Everest (Pakistan)"), std::string("EF"), 
      6377309.613, 6356109.571)));
      theEllipsoidTable.insert(std::make_pair(std::string("FA"),
      new ossimEllipsoid(std::string("Mod. Fischer 1960(South Asia)"), std::string("FA"), 
      6378155.0, 6356773.32)));
      theEllipsoidTable.insert(std::make_pair(std::string("RF"),
      new ossimEllipsoid(std::string("GRS 80"), std::string("RF"), 
      6378137.0, 6356752.3141, 7019)));
      theEllipsoidTable.insert(std::make_pair(std::string("HE"),
      new ossimEllipsoid(std::string("Helmert 1906"), std::string("HE"), 
      6378200.0, 6356818.17, 7020)));
      theEllipsoidTable.insert(std::make_pair(std::string("HO"),
      new ossimEllipsoid(std::string("Hough"), std::string("HO"), 
      6378270.0, 6356794.3430, 7053)));
      theEllipsoidTable.insert(std::make_pair(std::string("ID"),
      new ossimEllipsoid(std::string("Indonesian 1974"), std::string("ID"), 
      6378160.0, 6356774.504, 7021)));
      theEllipsoidTable.insert(std::make_pair(std::string("IN"),
      new ossimEllipsoid(std::string("International 1924"), std::string("IN"), 
      6378388.0, 6356911.946, 7022)));
      theEllipsoidTable.insert(std::make_pair(std::string("KA"),
      new ossimEllipsoid(std::string("Krassovsky"), std::string("KA"), 
      6378245.0, 6356863.0190, 7024)));
      theEllipsoidTable.insert(std::make_pair(std::string("SA"),
      new ossimEllipsoid(std::string("South American 1969"),std::string("SA"), 
      6378160.0, 6356774.719, 7003)));
      theEllipsoidTable.insert(std::make_pair(std::string("WD"),
      new ossimEllipsoid(std::string("WGS 72"), std::string("WD"), 
      6378135.000, 6356750.5200, 7043)));
      theEllipsoidTable.insert(std::make_pair(std::string("WE"),
      new ossimEllipsoid(std::string("WGS 84"), std::string("WE"), 
      6378137.000, 6356752.3142, 7030)));
      theEllipsoidTable.insert(std::make_pair(std::string("PV"),
      new ossimEllipsoid(std::string("Popular Visualization Sphere"), std::string("7059"), 
      6378137.000, 6378137.000, 7059)));

   theWgs84Ellipsoid = new ossimEllipsoid(std::string("WGS 84"), std::string("WE"), 
      6378137.000, 6356752.3142, 7030);
   theWgs72Ellipsoid = new ossimEllipsoid(std::string("WGS 72"), std::string("WD"), 
      6378135.000, 6356750.5200, 7043);
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
      if (iter->second == alpha_code.string())
      {
         epsg_code = iter->first;
      }
      iter++;
   }
   return epsg_code;
}
