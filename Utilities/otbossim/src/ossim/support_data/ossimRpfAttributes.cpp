#include <iostream>
 
#include <ossim/support_data/ossimRpfAttributes.h>

std::ostream& operator<<(std::ostream& out,
                    const ossimRpfAttributes& data)
{
   data.print(out);
   
   return out;
}

ossimRpfAttributes::ossimRpfAttributes()
{
   clearFields();
}
ossimRpfAttributes::~ossimRpfAttributes()
{
}

void ossimRpfAttributes::print(std::ostream& out)const
{
   out << "theCurrencyDate:                     " << theCurrencyDate << std::endl;
   out << "theProductionDate:                   " << theProductionDate << std::endl;
   out << "theSignificantDate:                  " << theSignificantDate << std::endl;
   out << "theChartSeriesCode:                  " << theChartSeriesCode << std::endl;
   out << "theMapDesignationCode:               " << theMapDesignationCode << std::endl;
   out << "theOldHorDatum:                      " << theOldHorDatum << std::endl;
   out << "theEdition:                          " << theEdition << std::endl;
   out << "theProjectionCode:                   " << theProjectionCode << std::endl;
   out << "theProjectionA:                      " << theProjectionA << std::endl;
   out << "theProjectionB:                      " << theProjectionB << std::endl;
   out << "theProjectionC:                      " << theProjectionC << std::endl;
   out << "theProjectionD:                      " << theProjectionD << std::endl;
   out << "theVertDatumCode:                    " << theVertDatumCode<< std::endl;
   out << "theHorDatumCode:                     " << theHorDatumCode << std::endl;
   out << "theVertAbsAccuracy:                  " << theVertAbsAccuracy << std::endl;
   out << "theVertAbsUnits:                     " << theVertAbsUnits << std::endl;
   out << "theHorAbsAccuracy:                   " << theHorAbsAccuracy << std::endl;
   out << "theHorAbsUnits:                      " << theHorAbsUnits << std::endl;
   out << "theVertRelAccuracy:                  " << theVertRelAccuracy << std::endl;
   out << "theVertRelUnits:                     " << theVertRelUnits << std::endl;
   out << "theHorRelAccuracy:                   " << theHorRelAccuracy << std::endl;
   out << "theHorRelUnits:                      " << theHorRelUnits << std::endl;
   out << "ellipsoidCode:                       " << ellipsoidCode << std::endl;
   out << "theSoundingDatumCode:                " << theSoundingDatumCode << std::endl;
   out << "theNavSystemCode:                    " << theNavSystemCode << std::endl;
   out << "theGridCode:                         " << theGridCode << std::endl;
   out << "theEeasterlyMagChange:               " << theEeasterlyMagChange << std::endl;
   out << "theEasterlyMagChangeUnits:           " << theEasterlyMagChangeUnits << std::endl;
   out << "theWesterlyMagChange:                " << theWesterlyMagChange << std::endl;
   out << "theWesterlyMagChangeUnits:           " << theWesterlyMagChangeUnits << std::endl;
   out << "theMagAngle:                         " << theMagAngle << std::endl;
   out << "theGridConver:                       " << theGridConver << std::endl;
   out << "theGridConverUnits:                  " << theGridConverUnits << std::endl;
   out << "theHighElevation:                    " << theHighElevation << std::endl;
   out << "theHighElevationUnits:               " << theHighElevationUnits << std::endl;
   out << "theHighLat:                          " << theHighLat << std::endl;
   out << "theHighLon:                          " << theHighLon << std::endl;
   out << "theLegendFileName:                   " << theLegendFileName << std::endl;
   out << "theDataSource:                       " << theDataSource << std::endl;
   out << "theGsd:                              " << theGsd << std::endl;
   out << "theDataLevel:                        " << theDataLevel << std::endl;
}

bool ossimRpfAttributes::isEmpty()const
{
   for(std::map<ossim_uint32, bool>::const_iterator iter= theAttributeIdBoolMap.begin();
       iter != theAttributeIdBoolMap.end();++iter)
   {
      if(iter->second!=false)
      {
         return false;
      }
   }
   return true;
}

void ossimRpfAttributes::clearFields()
{
   theCurrencyDate    = "";
   theProductionDate  = "";
   theSignificantDate = "";
   theChartSeriesCode = "";
   theMapDesignationCode = "";
   theOldHorDatum = "";
   theEdition = "";
   theProjectionCode = "";
   theProjectionA = 0.0;
   theProjectionB = 0.0;
   theProjectionC = 0.0;
   theProjectionD = 0.0;
   theVertDatumCode = "";
   theHorDatumCode = "";
   theVertAbsAccuracy = 0;
   theVertAbsUnits = 0;
   theHorAbsAccuracy = 0;
   theHorAbsUnits    = 0;
   theVertRelAccuracy = 0;
   theVertRelUnits = 0;
   theHorRelAccuracy = 0;
   theHorRelUnits = 0;
   ellipsoidCode = "";
   theSoundingDatumCode = "";
   theNavSystemCode = 0;
   theGridCode = "";
   theEeasterlyMagChange = 0.0;
   theEasterlyMagChangeUnits = 0;
   theWesterlyMagChange = 0.0;
   theWesterlyMagChangeUnits = 0;
   theMagAngle = 0.0;
   theMagAngleUnits = 0;
   theGridConver = 0.0;
   theGridConverUnits = 0;
   theHighElevation = 0.0;
   theHighElevationUnits = 0;
   theHighLat = 0.0;
   theHighLon = 0.0;
   theLegendFileName = "";
   theDataSource = "";
   theGsd = 0;
   theDataLevel = 0;

   theAttributeIdBoolMap.clear();
}

void ossimRpfAttributes::setAttributeFlag(ossim_uint32 id,
                                          bool flag)
{
   std::map<ossim_uint32, bool>::iterator iter = theAttributeIdBoolMap.find(id);

   if(iter != theAttributeIdBoolMap.end())
   {
      if(flag)
      {
         iter->second = flag;
      }
      else
      {
         theAttributeIdBoolMap.erase(iter); 
      }
   }
   else if(flag)
   {
	   theAttributeIdBoolMap.insert(std::make_pair(id, flag));
   }
}

bool ossimRpfAttributes::getAttributeFlag(ossim_uint32 id)const
{
   std::map<ossim_uint32, bool>::const_iterator iter = theAttributeIdBoolMap.find(id);

   if(iter != theAttributeIdBoolMap.end())
   {
      return iter->second;
   }

   return false;
}
