#include <ossim/base/ossimNadconNarDatum.h>

RTTI_DEF1(ossimNadconNarDatum, "ossimNadconNarDatum", ossimNadconGridDatum);
ossimNadconNarDatum::ossimNadconNarDatum(const ossimFilename& nadconDirectory)
   :
      ossimNadconGridDatum(nadconDirectory,
                           ossimString("NAR"),
                           ossimString("NORTH AMERICAN 1983 NADCON, CONUS GRID"),
                           ossimEllipsoidFactory::instance()->create(ossimString("RF")),
                           2.000000000000000, 2.000000000000000, 2.000000000000000, 
                           -135.0, -60.0, 15.0, 60.0,
                           0.000000000000000, 0.000000000000000, 0.000000000000000)
{
}

ossimGpt ossimNadconNarDatum::shift(const ossimGpt    &aPt)const
{
  const ossimDatum* datum = aPt.datum();
  ossimString code = datum->code();
  ossimString subCode(code.begin(),
		      code.begin() + 3);
  if(subCode == "NAR")
  {
     return aPt;
  }
  else
  {
     if(subCode == "NAS")
     {
	checkGrid(aPt);
	if(!theLatGrid.getFileOkFlag()||
	   !theLonGrid.getFileOkFlag())
        {
           return ossimThreeParamDatum::shift(aPt);
        }
	
        double shiftLat = theLatGrid.getShiftAtLatLon(aPt.latd(), aPt.lond());
        double shiftLon = theLonGrid.getShiftAtLatLon(aPt.latd(), aPt.lond());
        
        if( (ossim::isnan(shiftLat)) || (ossim::isnan(shiftLon)) )
        {
           return ossimThreeParamDatum::shift(aPt);
        }
        else
        {
           // Note the shifts are stored in the file
           // as seconds.
           //
           // convert the seconds into decimal degrees.  
           //
           shiftLat /= 3600.0;
           shiftLon /= 3600.0;
           return ossimGpt(aPt.latd() + shiftLat,
                           aPt.lond() - shiftLon,
                           aPt.height(),
                           this);
        }
     }
     else
     {
        return ossimThreeParamDatum::shift(aPt);
     }
  }
  
  return ossimThreeParamDatum::shift(aPt);
}
