//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  Garrett Potts
// 
//*******************************************************************
//  $Id: ossimEllipsoidFactory.cpp 11498 2007-08-06 09:20:28Z dburken $
#include <ossim/base/ossimEllipsoidFactory.h>
#include <ossim/base/ossimEllipsoid.h>
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
   theInstance = NULL;
}


const ossimEllipsoid* ossimEllipsoidFactory::create(const ossimString &code)const
{
   TableType::const_iterator ellipsoid = theEllipsoidTable.find(code);

   if(ellipsoid != theEllipsoidTable.end())
   {
      return (*ellipsoid).second;
   }
   
   return NULL;
}

void ossimEllipsoidFactory::initializeTable()
{
   if(theEllipsoidTable.size()<1)
   {
      theEllipsoidTable.insert(make_pair(ossimString("AA"),
                                         new ossimEllipsoid(ossimString("Airy"), ossimString("AA"), 6377563.396, 6356256.9090)));   
      theEllipsoidTable.insert(make_pair(ossimString("AM"),
                                         new ossimEllipsoid(ossimString("Modified Airy"), ossimString("AM"), 6377340.189, 6356034.448)));
      theEllipsoidTable.insert(make_pair(ossimString("AN"),
                                         new ossimEllipsoid(ossimString("Australian National"), ossimString("AN"), 6378160.000, 6356774.7190)));
      theEllipsoidTable.insert(make_pair(ossimString("BR"),
                                         new ossimEllipsoid(ossimString("Bessel 1841"), ossimString("BR"), 6377397.155, 6356078.9630)));
      theEllipsoidTable.insert(make_pair(ossimString("BN"),
                                         new ossimEllipsoid(ossimString("Bessel 1841(Namibia)"), ossimString("BN"), 6377483.865, 6356165.3830)));
      theEllipsoidTable.insert(make_pair(ossimString("CC"),
                                         new ossimEllipsoid(ossimString("Clarke 1866"), ossimString("CC"), 6378206.400, 6356583.8000)));
      theEllipsoidTable.insert(make_pair(ossimString("CD"),
                                         new ossimEllipsoid(ossimString("Clarke 1880"), ossimString("CD"), 6378249.145, 6356514.8700)));
      theEllipsoidTable.insert(make_pair(ossimString("CE"),
                                         new ossimEllipsoid(ossimString("Clarke 1880 (IGN)EPSG 7011"), ossimString("CE"), 6378249.200, 6356515.0000)));
      theEllipsoidTable.insert(make_pair(ossimString("EA"),
                                         new ossimEllipsoid(ossimString("Everest"), ossimString("EA"), 6377276.345, 6356075.4130)));
      theEllipsoidTable.insert(make_pair(ossimString("EB"),
                                         new ossimEllipsoid(ossimString("Everest (E. Malasia, Brunei)"), ossimString("EB"), 6377298.556, 6356097.55)));
      theEllipsoidTable.insert(make_pair(ossimString("EC"),
                                         new ossimEllipsoid(ossimString("Everest 1956 (India)"), ossimString("EC"), 6377301.243, 6356100.228)));
      theEllipsoidTable.insert(make_pair(ossimString("ED"),
                                         new ossimEllipsoid(ossimString("Everest 1969 (West Malasia)"), ossimString("ED"), 6377295.664, 6356094.668)));
      theEllipsoidTable.insert(make_pair(ossimString("EE"),
                                         new ossimEllipsoid(ossimString("Everest 1948(W.Mals. & Sing.)"), ossimString("EE"), 6377304.063, 6356103.039)));
      theEllipsoidTable.insert(make_pair(ossimString("EF"),
                                         new ossimEllipsoid(ossimString("Everest (Pakistan)"), ossimString("EF"), 6377309.613, 6356109.571)));
      theEllipsoidTable.insert(make_pair(ossimString("FA"),
                                         new ossimEllipsoid(ossimString("Mod. Fischer 1960(South Asia)"), ossimString("FA"), 6378155.0, 6356773.32)));
      theEllipsoidTable.insert(make_pair(ossimString("RF"),
                                         new ossimEllipsoid(ossimString("GRS 80"), ossimString("RF"), 6378137.0, 6356752.3141)));
      theEllipsoidTable.insert(make_pair(ossimString("HE"),
                                         new ossimEllipsoid(ossimString("Helmert 1906"), ossimString("HE"), 6378200.0, 6356818.17)));
      theEllipsoidTable.insert(make_pair(ossimString("HO"),
                                         new ossimEllipsoid(ossimString("Hough"), ossimString("HO"), 6378270.0, 6356794.3430)));
      theEllipsoidTable.insert(make_pair(ossimString("ID"),
                                         new ossimEllipsoid(ossimString("Indonesian 1974"), ossimString("ID"), 6378160.0, 6356774.504)));
      theEllipsoidTable.insert(make_pair(ossimString("IN"),
                                         new ossimEllipsoid(ossimString("International 1924"), ossimString("IN"), 6378388.0, 6356911.946)));
      theEllipsoidTable.insert(make_pair(ossimString("KA"),
                                         new ossimEllipsoid(ossimString("Krassovsky"), ossimString("KA"), 6378245.0, 6356863.0190)));
      theEllipsoidTable.insert(make_pair(ossimString("RF"),
                                         new ossimEllipsoid(ossimString("GRS 80"), ossimString("RF"), 6378137.0, 6356752.0190)));
      theEllipsoidTable.insert(make_pair(ossimString("SA"),
                                         new ossimEllipsoid(ossimString("South American 1969"),ossimString("SA"), 6378160.0, 6356774.719)));
      theEllipsoidTable.insert(make_pair(ossimString("WD"),
                                         new ossimEllipsoid(ossimString("WGS 72"), ossimString("WD"), 6378135.000, 6356750.5200)));
      theEllipsoidTable.insert(make_pair(ossimString("WE"),
                                         new ossimEllipsoid(ossimString("WGS 84"), ossimString("WE"), 6378137.000, 6356752.3142)));
      theWgs84Ellipsoid = new ossimEllipsoid(ossimString("WGS 84"), ossimString("WE"), 6378137.000, 6356752.3142);
      theWgs72Ellipsoid = new ossimEllipsoid(ossimString("WGS 72"), ossimString("WD"), 6378135.000, 6356750.5200);
   }
   
#if 0   
   if(theEllipsoidTable.size()<1)
   {
      theEllipsoidTable.insert(make_pair(ossimString("AA"),
                                         new ossimEllipsoid(ossimString("Airy"), ossimString("AA"), 6377563.396, 6356256.9090, 299.3249646)));   
      theEllipsoidTable.insert(make_pair(ossimString("AM"),
                                         new ossimEllipsoid(ossimString("Modified Airy"), ossimString("AM"), 6377340.189, 6356034.448, 299.3249646)));
      theEllipsoidTable.insert(make_pair(ossimString("AN"),
                                         new ossimEllipsoid(ossimString("Australian National"), ossimString("AN"), 6378160.000, 6356774.7190, 298.25)));
      theEllipsoidTable.insert(make_pair(ossimString("BR"),
                                         new ossimEllipsoid(ossimString("Bessel 1841"), ossimString("BR"), 6377397.155, 6356078.9630, 299.1528128)));
      theEllipsoidTable.insert(make_pair(ossimString("BN"),
                                         new ossimEllipsoid(ossimString("Bessel 1841(Namibia)"), ossimString("BN"), 6377483.865, 6356165.3830, 299.1528128)));
      theEllipsoidTable.insert(make_pair(ossimString("CC"),
                                         new ossimEllipsoid(ossimString("Clarke 1866"), ossimString("CC"), 6378206.400, 6356583.8000, 294.9786982)));
      theEllipsoidTable.insert(make_pair(ossimString("CD"),
                                         new ossimEllipsoid(ossimString("Clarke 1880"), ossimString("CD"), 6378249.145, 6356514.8700, 293.465)));
      theEllipsoidTable.insert(make_pair(ossimString("EA"),
                                         new ossimEllipsoid(ossimString("Everest"), ossimString("EA"), 6377276.345, 6356075.4130, 300.8017)));
      theEllipsoidTable.insert(make_pair(ossimString("EB"),
                                         new ossimEllipsoid(ossimString("Everest (E. Malasia, Brunei)"), ossimString("EB"), 6377298.556, 6356097.55, 300.8017)));
      theEllipsoidTable.insert(make_pair(ossimString("EC"),
                                         new ossimEllipsoid(ossimString("Everest 1956 (India)"), ossimString("EC"), 6377301.243, 6356100.228, 300.8017)));
      theEllipsoidTable.insert(make_pair(ossimString("ED"),
                                         new ossimEllipsoid(ossimString("Everest 1969 (West Malasia)"), ossimString("ED"), 6377295.664, 6356094.668, 300.8017)));
      theEllipsoidTable.insert(make_pair(ossimString("EE"),
                                         new ossimEllipsoid(ossimString("Everest 1948(W.Mals. & Sing.)"), ossimString("EE"), 6377304.063, 6356103.039, 300.8017)));
      theEllipsoidTable.insert(make_pair(ossimString("EF"),
                                         new ossimEllipsoid(ossimString("Everest (Pakistan)"), ossimString("EF"), 6377309.613, 6356109.571, 300.8017)));
      theEllipsoidTable.insert(make_pair(ossimString("FA"),
                                         new ossimEllipsoid(ossimString("Mod. Fischer 1960(South Asia)"), ossimString("FA"), 6378155.0, 6356773.32, 298.3)));
      theEllipsoidTable.insert(make_pair(ossimString("RF"),
                                         new ossimEllipsoid(ossimString("GRS 80"), ossimString("RF"), 6378137.0, 6356752.3141, 298.257222101)));
      theEllipsoidTable.insert(make_pair(ossimString("HE"),
                                         new ossimEllipsoid(ossimString("Helmert 1906"), ossimString("HE"), 6378200.0, 6356818.17, 298.3)));
      theEllipsoidTable.insert(make_pair(ossimString("HO"),
                                         new ossimEllipsoid(ossimString("Hough"), ossimString("HO"), 6378270.0, 6356794.3430, 297.0)));
      theEllipsoidTable.insert(make_pair(ossimString("ID"),
                                         new ossimEllipsoid(ossimString("Indonesian 1974"), ossimString("ID"), 6378160.0, 6356774.504, 298.247)));
      theEllipsoidTable.insert(make_pair(ossimString("IN"),
                                         new ossimEllipsoid(ossimString("International"), ossimString("IN"), 6378388.0, 6356911.946, 297.0)));
      theEllipsoidTable.insert(make_pair(ossimString("KA"),
                                         new ossimEllipsoid(ossimString("Krassovsky"), ossimString("KA"), 6378245.0, 6356863.0190, 298.3)));
      theEllipsoidTable.insert(make_pair(ossimString("SA"),
                                         new ossimEllipsoid(ossimString("South American 1969"),ossimString("SA"), 6378160.0, 6356774.719, 298.25)));
      theEllipsoidTable.insert(make_pair(ossimString("WD"),
                                         new ossimEllipsoid(ossimString("WGS 72"), ossimString("WD"), 6378135.000, 6356750.5200, 298.26)));
      theEllipsoidTable.insert(make_pair(ossimString("WE"),
                                         new ossimEllipsoid(ossimString("WGS 84"), ossimString("WE"), 6378137.000, 6356752.3142, 298.257223563)));
      theWgs84Ellipsoid = new ossimEllipsoid(ossimString("WGS 84"), ossimString("WE"), 6378137.000, 6356752.3142, 298.257223563);
      theWgs72Ellipsoid = new ossimEllipsoid(ossimString("WGS 72"), ossimString("WD"), 6378135.000, 6356750.5200, 298.26);
   }
#endif
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
}
