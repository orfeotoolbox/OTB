#include <ossim/base/ossimNadconGridDatum.h>

RTTI_DEF1(ossimNadconGridDatum, "ossimNadconGridDatum", ossimThreeParamDatum);
ossimNadconGridDatum::ossimNadconGridDatum(const ossimFilename& datumDirectory,
					   const ossimString &code, const ossimString &name,
					   const ossimEllipsoid* anEllipsoid,
					   double sigmaX,
					   double sigmaY,
					   double sigmaZ,
					   double westLongitude,
					   double eastLongitude,
					   double southLatitude,
					   double northLatitude,
					   double param1,
					   double param2,
					   double param3)
  :ossimThreeParamDatum(code,
			name,
			anEllipsoid,
			sigmaX,
			sigmaY,
			sigmaZ,
			westLongitude,
			eastLongitude,
			southLatitude,
			northLatitude,
			param1,
			param2,
			param3),
   theDatumDirectory(datumDirectory)
{
  theCurrentGridRect.makeNan();

  ossimNadconGridHeader header;
  theConusRect.makeNan();
  theHawaiiRect.makeNan();
  theAlaskaRect.makeNan();
  theStGeorgeRect.makeNan();
  theLatStLaurenceRect.makeNan();
  theStPaulRect.makeNan();
  thePrviRect.makeNan();

  if(header.readHeader(theDatumDirectory.dirCat("conus.las")))
    {
      theConusRect = header.getBoundingRect();
    }
  if(header.readHeader(theDatumDirectory.dirCat("hawaii.las")))
    {
      theHawaiiRect = header.getBoundingRect();
    }
  if(header.readHeader(theDatumDirectory.dirCat("alaska.las")))
    {
      theAlaskaRect = header.getBoundingRect();
    }
  if(header.readHeader(theDatumDirectory.dirCat("stgeorge.las")))
    {
      theStGeorgeRect = header.getBoundingRect();
    }
  if(header.readHeader(theDatumDirectory.dirCat("stlrnc.las")))
    {
      theLatStLaurenceRect = header.getBoundingRect();
    }
  if(header.readHeader(theDatumDirectory.dirCat("stpaul.las")))
    {
      theStPaulRect = header.getBoundingRect();
    }
  if(header.readHeader(theDatumDirectory.dirCat("prvi.las")))
    {
      thePrviRect = header.getBoundingRect();
    }
}

void ossimNadconGridDatum::checkGrid(const ossimDpt& latLon)const
{
  if(theCurrentGridRect.pointWithin(latLon))
    {
      return;
    }
  else if(theConusRect.pointWithin(latLon))
  {
    theLatGrid.open(theDatumDirectory.dirCat("conus.las"));
    theLonGrid.open(theDatumDirectory.dirCat("conus.los"));
		   
    theCurrentGridRect = theConusRect;
  }
  else if(theHawaiiRect.pointWithin(latLon))
  {
    theLatGrid.open(theDatumDirectory.dirCat("hawaii.las"));
    theLonGrid.open(theDatumDirectory.dirCat("hawaii.los"));
		   
    theCurrentGridRect = theHawaiiRect;
  }
  else if(theAlaskaRect.pointWithin(latLon))
    {
      theLatGrid.open(theDatumDirectory.dirCat("alaska.las"));
      theLonGrid.open(theDatumDirectory.dirCat("alaska.los"));
      
      theCurrentGridRect = theAlaskaRect;
    }
  else if(theStGeorgeRect.pointWithin(latLon))
    {
      theLatGrid.open(theDatumDirectory.dirCat("stgeorge.las"));
      theLonGrid.open(theDatumDirectory.dirCat("stgeorge.los"));
      
      theCurrentGridRect = theStGeorgeRect;
    }
  else if(theLatStLaurenceRect.pointWithin(latLon))
    {
      theLatGrid.open(theDatumDirectory.dirCat("stlrnc.las"));
      theLonGrid.open(theDatumDirectory.dirCat("stlrnc.los"));
      
      theCurrentGridRect = theLatStLaurenceRect;
    }
  else if(theStPaulRect.pointWithin(latLon))
    {
      theLatGrid.open(theDatumDirectory.dirCat("stpaul.las"));
      theLonGrid.open(theDatumDirectory.dirCat("stpaul.los"));
      
      theCurrentGridRect = theStPaulRect;
    }
  else if(thePrviRect.pointWithin(latLon))
    {
      theLatGrid.open(theDatumDirectory.dirCat("prvi.las"));
      theLonGrid.open(theDatumDirectory.dirCat("prvi.los"));
      
      theCurrentGridRect = thePrviRect;
    }
}

