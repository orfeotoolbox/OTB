#include <ossim/base/ossimNadconNasDatum.h>

RTTI_DEF1(ossimNadconNasDatum, "ossimNadconNasDatum", ossimNadconGridDatum);
ossimNadconNasDatum::ossimNadconNasDatum(const ossimFilename& nadconDirectory)
   :
      ossimNadconGridDatum(nadconDirectory,
                           ossimString("NAS"),
                           ossimString("NORTH AMERICAN 1927 NADCON, CONUS GRID"),
                           ossimEllipsoidFactory::instance()->create(ossimString("CC")),
                           5.000000000000000, 5.000000000000000, 6.000000000000000, 
                           -135, -60.0, 15.0, 60.0, -8.0, 160.0, 176.0)
{
}

ossimGpt ossimNadconNasDatum::shift(const ossimGpt& aPt)const
{
  const ossimDatum* datum = aPt.datum();
  ossimString code = datum->code();
  ossimString subCode(code.begin(),
		      code.begin() + 3);
  if(subCode == "NAS")
    {
      return aPt;
    }
  else
  {
     if(subCode == "NAR")
     {
       checkGrid(aPt);
       if(!theLatGrid.getFileOkFlag()||
	  !theLonGrid.getFileOkFlag())
	 {
	   return ossimThreeParamDatum::shift(aPt);
	 }
        ossimDpt pt;
        ossimDpt tempPt;
        ossimDpt deltaPt;
        double shiftLat;
        double shiftLon;
	double minLat = theCurrentGridRect.ll().lat;
	double maxLat = theCurrentGridRect.ul().lat;
	double minLon = theCurrentGridRect.ul().lon;
	double maxLon = theCurrentGridRect.ur().lon;
        int maxIter = 20;
        double epsilon = 1.0e-9;
        int c = 0;
        
        pt.lat = aPt.latd();
        pt.lon = aPt.lond();
        ossimDpt pt2;
        tempPt = pt;
        ossimDpt delta2;
        while(c < maxIter)
        {
	  if(tempPt.lat < minLat) tempPt.lat = minLat;
	  if(tempPt.lat > maxLat) tempPt.lat = maxLat;
	  if(tempPt.lon < minLon) tempPt.lon = minLon;
	  if(tempPt.lon > maxLon) tempPt.lon = maxLon;

           shiftLat = theLatGrid.getShiftAtLatLon(tempPt.lat, tempPt.lon);
           shiftLon = theLonGrid.getShiftAtLatLon(tempPt.lat, tempPt.lon);

	   
	   pt2.lat = tempPt.lat + shiftLat/3600.0;
	   pt2.lon = tempPt.lon - shiftLon/3600.0;
           
           delta2  = pt2-pt;
           if(c == 0)
           {
              if(fabs(deltaPt.lon) > epsilon)
              {
                 tempPt.lon = pt.lon - shiftLon/3600.0;
              }
              if(fabs(deltaPt.lat) > epsilon)
              {
                 tempPt.lat = pt.lat + shiftLat/3600.0;
              }
           }
           else
           {
              if(fabs(delta2.lon) > epsilon)
              {
                 tempPt.lon = tempPt.lon - delta2.lon;
              }
              if(fabs(delta2.lat) > epsilon)
              {
                 tempPt.lat =tempPt.lat - delta2.lat;
              }
           }
           if((fabs(delta2.lat) < epsilon)&&
              (fabs(delta2.lon) < epsilon))
           {
              break;
           }
           else
           {
              ++c;
           }
        }
        return ossimGpt(tempPt.lat,
                        tempPt.lon,
                        aPt.height(),
                        this);
     }
  }
  return ossimThreeParamDatum::shift(aPt);
}
