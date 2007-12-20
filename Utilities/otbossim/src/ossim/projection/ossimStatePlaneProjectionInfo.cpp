//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//*******************************************************************
//  $Id: ossimStatePlaneProjectionInfo.cpp 12096 2007-11-30 20:24:13Z dburken $

#include <ossim/projection/ossimStatePlaneProjectionInfo.h>
#include <ossim/projection/ossimTransMercatorProjection.h>
#include <ossim/projection/ossimLambertConformalConicProjection.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimDatumFactory.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimUnitTypeLut.h>

ossimStatePlaneProjectionInfo::ossimStatePlaneProjectionInfo(
   const std::string&  name,
   int                 pcsCode,
   const std::string&  projName,
   const std::string&  param1,
   const std::string&  param2,
   const std::string&  param3,
   const std::string&  param4,
   double              falseEast,
   double              falseNorth,
   const std::string&  units,
   const ossimDatum*   datum)
   :
      thePcsCode        (pcsCode),
      theName           (name),
      theProjectionName (projName),
      theDatum          (datum),
      theOriginLat      (param1),
      theOriginLon      (param2),
      theOrigin         (),
      theParameter3     (0.0),
      theParameter4     (0.0),
      theFalseEasting   (falseEast),
      theFalseNorthing  (falseNorth),
      theScaleFactor    (0.0),
      theUnits          (units)
      
{
   // Set the parameters 
   theParameter3 = ossimDms(param3).getDegrees();
   theParameter4 = ossimDms(param4).getDegrees();

   if (!theDatum)
   {
      if(theName.contains("NAD27"))
      {
         // NAD 1927
         theDatum = ossimDatumFactory::instance()->create("NAS-C"); 
      }
      else
      {
         // NAD 1983
         theDatum = ossimDatumFactory::instance()->create("NAR-C");  
      }
   }

   theOrigin = ossimGpt(theOriginLat.getDegrees(),
                        theOriginLon.getDegrees(),
                        0,
                        theDatum);   
}

ossimStatePlaneProjectionInfo::ossimStatePlaneProjectionInfo(
   const std::string&  name,
   int                 pcsCode,
   const std::string&  projName,
   const std::string&  param1,
   const std::string&  param2,
   double              param3,
   double              param4,
   double              falseEast,
   double              falseNorth,
   const std::string&  units,
   const ossimDatum*   datum)
   :
      thePcsCode        (pcsCode),
      theName           (name),
      theProjectionName (projName),
      theDatum          (datum),
      theOriginLat      (param1),
      theOriginLon      (param2),
      theOrigin         (),
      theParameter3     (param3),
      theParameter4     (param4),
      theFalseEasting   (falseEast),
      theFalseNorthing  (falseNorth),
      theScaleFactor    (0.0),
      theUnits          (units)
{
   if (!theDatum)
   {
      if(theName.contains("NAD27"))
      {
         // NAD 1927
         theDatum = ossimDatumFactory::instance()->create("NAS-C"); 
      }
      else
      {
         // NAD 1983
         theDatum = ossimDatumFactory::instance()->create("NAR-C");  
      }
   }

   theOrigin = ossimGpt(theOriginLat.getDegrees(),
                        theOriginLon.getDegrees(),
                        0,
                        theDatum);   
   if(fabs(theParameter3) > FLT_EPSILON)
   {
      theScaleFactor = 1.0 - (1.0 / theParameter3);
   }
   else
   {
      theScaleFactor = 1.0;
   }
   
}

int ossimStatePlaneProjectionInfo::code() const
{
   return thePcsCode;
}

const ossimString& ossimStatePlaneProjectionInfo::name() const
{
   return theName;
}

const ossimString& ossimStatePlaneProjectionInfo::projName() const
{
   return theProjectionName;
}

double ossimStatePlaneProjectionInfo::originLat() const
{
   return theOriginLat.getDegrees();
}

double ossimStatePlaneProjectionInfo::originLon() const
{
   return theOriginLon.getDegrees();
}

const ossimDatum* ossimStatePlaneProjectionInfo::datum() const
{
   return theDatum;
}

ossimGpt ossimStatePlaneProjectionInfo::origin() const
{
   return theOrigin;
}

double ossimStatePlaneProjectionInfo::parallel1() const
{
   return theParameter3;
}

double ossimStatePlaneProjectionInfo::parallel2() const
{
   return theParameter4;
}

double ossimStatePlaneProjectionInfo::falseEasting() const
{
   return theFalseEasting;
}

double ossimStatePlaneProjectionInfo::falseNorthing() const
{
   return theFalseNorthing;
}

double ossimStatePlaneProjectionInfo::scaleFactor() const
{
   return theScaleFactor;
}

const ossimString&  ossimStatePlaneProjectionInfo::units() const
{
   return theUnits;
}

bool ossimStatePlaneProjectionInfo::isSameCode(int pcsCode) const
{
   if(pcsCode == thePcsCode)
   {
      return true;
   }

   return false;
}

double ossimStatePlaneProjectionInfo::falseEastingInMeters() const
{
   if (units()=="m")
   {
      return theFalseEasting;
   }
   return theFalseEasting * US_METERS_PER_FT;
}

double ossimStatePlaneProjectionInfo::falseNorthingInMeters() const
{
   if (units()=="m")
   {
      return theFalseNorthing;
   }

   return theFalseNorthing * US_METERS_PER_FT;
}

void ossimStatePlaneProjectionInfo::populateProjectionKeywords(
   ossimKeywordlist& kwl, const char* prefix)const
{
   kwl.add(prefix,
           ossimKeywordNames::PCS_CODE_KW,
           thePcsCode,
           true);
  
   kwl.add(prefix,
           ossimKeywordNames::FALSE_EASTING_NORTHING_UNITS_KW,
           (ossimUnitTypeLut::instance()->getEntryString(getUnitType())),
           true);

   ossimDpt pt(theFalseEasting, theFalseNorthing);
           
   kwl.add(prefix,
           ossimKeywordNames::FALSE_EASTING_NORTHING_KW,
           pt.toString().c_str(),
           true);

   kwl.add(prefix,
           ossimKeywordNames::ORIGIN_LATITUDE_KW,
           origin().latd(),
           true);
   
   kwl.add(prefix,
           ossimKeywordNames::CENTRAL_MERIDIAN_KW,
           origin().lond(),
           true);
   kwl.add(prefix,
           ossimKeywordNames::TYPE_KW,
           projName(),
           true);
   if(theDatum)
   {
      
      kwl.add(prefix,
              ossimKeywordNames::DATUM_KW,
              theDatum->code(),
              true);
   }

   if (projName()== STATIC_TYPE_NAME(ossimLambertConformalConicProjection))
   {
      kwl.add(prefix,
              ossimKeywordNames::TYPE_KW,
              STATIC_TYPE_NAME(ossimLambertConformalConicProjection),
              true);
      kwl.add(prefix,
              ossimKeywordNames::STD_PARALLEL_1_KW,
              parallel1(),
              true);
      kwl.add(prefix,
              ossimKeywordNames::STD_PARALLEL_2_KW,
              parallel2(),
              true);
   }
   else
   {
      kwl.add(prefix,
              ossimKeywordNames::SCALE_FACTOR_KW,
              theScaleFactor,
              true);
   }
}

ossimUnitType ossimStatePlaneProjectionInfo::getUnitType() const
{
   ossimUnitType type = OSSIM_METERS;
   if (theUnits == "ft")
   {
      type = OSSIM_US_SURVEY_FEET;
   }
   return type;
}

bool ossimStatePlaneProjectionInfo::matchesProjection(
   const ossimMapProjection* proj) const
{
   if (!proj)
   {
      return false;
   }

#if 0
   cout << "\ntheProjectionName: " << theProjectionName
           << "\nclassName: " << proj->getClassName()
           << "\ntheOrigin: " << theOrigin
           << "\nproj->origin(): "<< proj->origin()
           << "\nfalseEastingInMeters(): "<< falseEastingInMeters()
           << "\nproj->getFalseEasting(): " << proj->getFalseEasting()
           << "\nfalseNorthingInMeters(): " << falseNorthingInMeters()
           << "\nproj->getFalseNorthing(): " << proj->getFalseNorthing()
           << "\ndatum: " << theDatum->code() 
           << endl;
#endif
   
   if ( (theProjectionName != proj->getClassName()) ||
        ( (theOrigin == proj->origin()) == false) ||
        (falseEastingInMeters() != proj->getFalseEasting()) ||
        (falseNorthingInMeters() != proj->getFalseNorthing()) )
   {
      return false;
   }

   //---
   // NOTE:  Going from double to strings, radians to degrees and back is
   // causing a wiggle in the floating point values; hence the
   // ALLOWABLE_VARIANCE.
   //---
   const double ALLOWABLE_VARIANCE = 0.00000000000001;
   
   if (theProjectionName == "ossimTransMercatorProjection")
   {
      ossimTransMercatorProjection* tm = PTR_CAST(ossimTransMercatorProjection,
                                                  proj);
      if (!tm)
      {
         return false;
      }
      double d1 = fabs(theScaleFactor - tm->getScaleFactor());
      if (d1 < ALLOWABLE_VARIANCE)
      {
         return true;
      }
   }
   else // Lambert.
   {
      ossimLambertConformalConicProjection* lam =
         PTR_CAST(ossimLambertConformalConicProjection, proj);
      if (!lam)
      {
         return false;
      }
      double d1 = fabs(parallel1() - lam->getStandardParallel1());
      double d2 = fabs(parallel2() - lam->getStandardParallel2());
      if ( (d1 < ALLOWABLE_VARIANCE) && (d2 < ALLOWABLE_VARIANCE) )
      {
         return true;
      }
   }
   
   return false;
}

std::ostream& operator<<(std::ostream& os,
                         const ossimStatePlaneProjectionInfo& thePlane)
{
   if (thePlane.projName()== STATIC_TYPE_NAME(ossimTransMercatorProjection))
   {
      return  os << "ossimStatePlaneProjection: "
                 << "\nName:    " << thePlane.name()
                 << "\nPcsCode: " << thePlane.code()
                 << "\norigin:  " << thePlane.origin()
                 << "\nscale factor:  " << setprecision(12)
                 << thePlane.scaleFactor()
                 << "\nfalseEasting:   " << thePlane.falseEasting()
                 << "\nfalseNorthing:  " << thePlane.falseNorthing()
                 << endl;
   }
   else  // Lambert
   {
      return  os << "ossimStatePlaneProjection: "
                 << "\nName:    " << thePlane.name()
                 << "\nPcsCode: " << thePlane.code()
                 << "\norigin:  " << thePlane.origin()
                 << "\nphi1:    "
                 << setprecision(12) << thePlane.parallel1()
                 << "\nphi2:    "
                 << setprecision(12) << thePlane.parallel2()
                 << "\nfalseEasting:   " << thePlane.falseEasting()
                 << "\nfalseNorthing:  " << thePlane.falseNorthing()
                 << endl;
   }
}
