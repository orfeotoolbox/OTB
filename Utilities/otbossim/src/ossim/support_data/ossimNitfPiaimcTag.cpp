//----------------------------------------------------------------------------
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: PIAIMC tag class declaration.
// 
// "Profile for Imagery Access Image Support Extensions"
//
// See document STDI-0002 Table 6-1 for more info.
// 
// http://164.214.2.51/ntb/baseline/docs/stdi0002/final.pdf
//
//----------------------------------------------------------------------------
// $Id: ossimNitfPiaimcTag.cpp 22013 2012-12-19 17:37:20Z dburken $

#include <cstring> /* for memcpy */
#include <iostream>
#include <iomanip>

#include <ossim/support_data/ossimNitfPiaimcTag.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimStringProperty.h>
#include <ossim/base/ossimCommon.h>

static const ossimString CLOUDCVR_KW = "CLOUDCVR";
static const ossimString SRP_KW = "SRP";
static const ossimString SENSMODE_KW = "SENSMODE";
static const ossimString SENSNAME_KW = "SENSNAME";
static const ossimString SOURCE_KW = "SOURCE";
static const ossimString COMGEN_KW = "COMGEN";
static const ossimString SUBQUAL_KW = "SUBQUAL";
static const ossimString PIAMSNNUM_KW = "PIAMSNNUM";
static const ossimString CAMSPECS_KW = "CAMSPECS";
static const ossimString PROJID_KW = "PROJID";
static const ossimString GENERATION_KW = "GENERATION";
static const ossimString ESD_KW = "ESD";
static const ossimString OTHERCOND_KW = "OTHERCOND";
static const ossimString MEANGSD_KW = "MEANGSD";
static const ossimString IDATUM_KW = "IDATUM";
static const ossimString IELLIP_KW = "IELLIP";
static const ossimString PREPROC_KW = "PREPROC";
static const ossimString IPROJ_KW = "IPROJ";
static const ossimString SATTRACK_KW = "SATTRACK";

RTTI_DEF1(ossimNitfPiaimcTag,
          "ossimNitfPiaimcTag",
          ossimNitfRegisteredTag);

ossimNitfPiaimcTag::ossimNitfPiaimcTag()
   : ossimNitfRegisteredTag(std::string("PIAIMC"), 362)
{
   clearFields();
}

void ossimNitfPiaimcTag::parseStream(std::istream& in)
{
   clearFields();
   
   in.read(theCloudcvr,   CLOUDCVR_SIZE);
   in.read(theSrp,        SRP_SIZE);
   in.read(thesSensmode,  SENSMODE_SIZE);
   in.read(theSensname,   SENSNAME_SIZE);
   in.read(theSource,     SOURCE_SIZE);
   in.read(theComgen,     COMGEN_SIZE);
   in.read(theSubqual,    SUBQUAL_SIZE);
   in.read(thePiamsnnum,  PIAMSNNUM_SIZE);
   in.read(theCamspecs,   CAMSPECS_SIZE);
   in.read(theProjid,     PROJID_SIZE);
   in.read(theGeneration, GENERATION_SIZE);
   in.read(theEsd,        ESD_SIZE);
   in.read(theOthercond,  OTHERCOND_SIZE);
   in.read(theMeanGsd,    MEANGSD_SIZE);
   in.read(theIdatum,     IDATUM_SIZE);
   in.read(theIellip,     IELLIP_SIZE);
   in.read(thePreproc,    PREPROC_SIZE);
   in.read(theIproj,      IPROJ_SIZE);
   in.read(theSattrack,   SATTRACK_SIZE);
}

void ossimNitfPiaimcTag::writeStream(std::ostream& out)
{
   out.write(theCloudcvr,   CLOUDCVR_SIZE);
   out.write(theSrp,        SRP_SIZE);
   out.write(thesSensmode,  SENSMODE_SIZE);
   out.write(theSensname,   SENSNAME_SIZE);
   out.write(theSource,     SOURCE_SIZE);
   out.write(theComgen,     COMGEN_SIZE);
   out.write(theSubqual,    SUBQUAL_SIZE);
   out.write(thePiamsnnum,  PIAMSNNUM_SIZE);
   out.write(theCamspecs,   CAMSPECS_SIZE);
   out.write(theProjid,     PROJID_SIZE);
   out.write(theGeneration, GENERATION_SIZE);
   out.write(theEsd,        ESD_SIZE);
   out.write(theOthercond,  OTHERCOND_SIZE);
   out.write(theMeanGsd,    MEANGSD_SIZE);
   out.write(theIdatum,     IDATUM_SIZE);
   out.write(theIellip,     IELLIP_SIZE);
   out.write(thePreproc,    PREPROC_SIZE);
   out.write(theIproj,      IPROJ_SIZE);
   out.write(theSattrack,   SATTRACK_SIZE);
}

void ossimNitfPiaimcTag::clearFields()
{
   memcpy(theCloudcvr, "999",   CLOUDCVR_SIZE);
   memset(theSrp,        ' ',        SRP_SIZE);
   memset(thesSensmode,  ' ',  SENSMODE_SIZE);
   memset(theSensname,   ' ',   SENSNAME_SIZE);
   memset(theSource,     ' ',     SOURCE_SIZE);
   memset(theComgen,     ' ',     COMGEN_SIZE);
   memset(theSubqual,    ' ',    SUBQUAL_SIZE);
   memset(thePiamsnnum,  ' ',  PIAMSNNUM_SIZE);
   memset(theCamspecs,   ' ',   CAMSPECS_SIZE);
   memset(theProjid,     ' ',     PROJID_SIZE);
   memset(theGeneration, ' ', GENERATION_SIZE);
   memset(theEsd,        ' ',        ESD_SIZE);
   memset(theOthercond,  ' ',  OTHERCOND_SIZE);
   memset(theMeanGsd,    ' ',    MEANGSD_SIZE);
   memset(theIdatum,     ' ',     IDATUM_SIZE);
   memset(theIellip,     ' ',     IELLIP_SIZE);
   memset(thePreproc,    ' ',    PREPROC_SIZE);
   memset(theIproj,      ' ',     IPROJ_SIZE);
   memset(theSattrack,   ' ',   SATTRACK_SIZE);

   theCloudcvr[CLOUDCVR_SIZE]     = '\0';
   theSrp[SRP_SIZE]               = '\0';
   thesSensmode[SENSMODE_SIZE]    = '\0';
   theSensname[SENSNAME_SIZE]     = '\0';
   theSource[SOURCE_SIZE]         = '\0';
   theComgen[COMGEN_SIZE]         = '\0';
   theSubqual[SUBQUAL_SIZE]       = '\0';
   thePiamsnnum[PIAMSNNUM_SIZE]   = '\0';
   theCamspecs[CAMSPECS_SIZE]     = '\0';
   theProjid[PROJID_SIZE]         = '\0';
   theGeneration[GENERATION_SIZE] = '\0';
   theEsd[ESD_SIZE]               = '\0';
   theOthercond[OTHERCOND_SIZE]   = '\0';
   theMeanGsd[MEANGSD_SIZE]       = '\0';
   theIdatum[IDATUM_SIZE]         = '\0';
   theIellip[IELLIP_SIZE]         = '\0';
   thePreproc[PREPROC_SIZE]       = '\0';
   theIproj[IPROJ_SIZE]           = '\0';
   theSattrack[SATTRACK_SIZE]     = '\0';
}

std::ostream& ossimNitfPiaimcTag::print(std::ostream& out,
                                        const std::string& prefix) const
{
   std::string pfx = prefix;
   pfx += getTagName();
   pfx += ".";
   
   out << setiosflags(std::ios::left)
       << pfx << std::setw(24) << "CETAG:" << getTagName() << "\n"
       << pfx << std::setw(24) << "CEL:"   << getTagLength() << "\n"
       << pfx << std::setw(24) << "CLOUDCVR:"  << theCloudcvr << "\n"
       << pfx << std::setw(24) << "SRP:"       << theSrp << "\n"
       << pfx << std::setw(24) << "SENSMODE:"   << thesSensmode << "\n"
       << pfx << std::setw(24) << "SENSNAME:"   << theSensname << "\n"
       << pfx << std::setw(24) << "SOURCE:"     << theSource << "\n"
       << pfx << std::setw(24) << "COMGEN:"     << theComgen << "\n"
       << pfx << std::setw(24) << "SUBQUAL:"    << theSubqual << "\n"
       << pfx << std::setw(24) << "PIAMSNNUM:"  << thePiamsnnum << "\n"
       << pfx << std::setw(24) << "CAMSPECS:"   << theCamspecs << "\n"
       << pfx << std::setw(24) << "PROJID:"     << theProjid << "\n"
       << pfx << std::setw(24) << "GENERATION:" << theGeneration << "\n"
       << pfx << std::setw(24) << "ESD:"        << theEsd << "\n"
       << pfx << std::setw(24) << "OTHERCOND:"  << theOthercond << "\n"
       << pfx << std::setw(24) << "MEANGSD:"    << theMeanGsd << "\n"
       << pfx << std::setw(24) << "IDATUM:"     << theIdatum << "\n"
       << pfx << std::setw(24) << "IELLIP:"     << theIellip << "\n"
       << pfx << std::setw(24) << "PREPROC:"    << thePreproc << "\n"
       << pfx << std::setw(24) << "IPROJ:"     << theIproj << "\n"
       << pfx << std::setw(24) << "SATTRACK:"  << theSattrack << "\n";

   return out;
}

ossimString ossimNitfPiaimcTag::getCloudcvrString() const
{
   return ossimString(theCloudcvr);
}

ossimString ossimNitfPiaimcTag::getSrpString() const
{
   return ossimString(theSrp);
}
   
ossimString ossimNitfPiaimcTag::getSensmodeString() const
{
   return ossimString(thesSensmode);
}

ossimString ossimNitfPiaimcTag::getSensnameString() const
{
   return ossimString(theSensname);
}
   
ossimString ossimNitfPiaimcTag::getSourceString() const
{
   return ossimString(theSource);
}
  
ossimString ossimNitfPiaimcTag::getComgenString() const
{
   return ossimString(theComgen);
}

ossimString ossimNitfPiaimcTag::getSubqualString() const
{
   return ossimString(theSubqual);
}

ossimString ossimNitfPiaimcTag::getPiamsnnumString() const
{
   return ossimString(thePiamsnnum);
}

ossimString ossimNitfPiaimcTag::getCamspecsString() const
{
   return ossimString(theCamspecs);
}

ossimString ossimNitfPiaimcTag::getProjidString() const
{
   return ossimString(theProjid);
}

ossimString ossimNitfPiaimcTag::getGenerationString() const
{
   return ossimString(theGeneration);
}

ossimString ossimNitfPiaimcTag::getEsdString() const
{
   return ossimString(theEsd);
}
   
ossimString ossimNitfPiaimcTag::getOthercondString() const
{
   return ossimString(theOthercond);
}

ossimString ossimNitfPiaimcTag::getMeadGsdString() const
{
   return ossimString(theMeanGsd);
}

ossim_float64 ossimNitfPiaimcTag::getMeanGsdInMeters() const
{
   ossim_float64 gsd = 0.0;
   
   ossimString s = theMeanGsd;
   if (s.size())
   {
      ossim_float64 d = s.toFloat64();
      if ( (d != 0.0) && (ossim::isnan(d) == false) )
      {
         gsd = d / 12.0 * MTRS_PER_FT;
      }
   }
   
   return gsd;
}

ossimString ossimNitfPiaimcTag::getIdatumString() const
{
   return ossimString(theIdatum);
}
   
ossimString ossimNitfPiaimcTag::getIellipString() const
{
   return ossimString(theIellip);
}
   
ossimString ossimNitfPiaimcTag::getPreprocString() const
{
   return ossimString(thePreproc);
}
   
ossimString ossimNitfPiaimcTag::getIprojString() const
{
   return ossimString(theIproj);
}
   
ossimString ossimNitfPiaimcTag::getSattrackString() const
{
   return ossimString(theSattrack);
}

void ossimNitfPiaimcTag::setProperty(ossimRefPtr<ossimProperty> property)
{
   ossimNitfRegisteredTag::setProperty(property);
}

ossimRefPtr<ossimProperty> ossimNitfPiaimcTag::getProperty(const ossimString& name)const
{
   ossimProperty* result = 0;

   if(name == CLOUDCVR_KW)
   {
      result = new ossimStringProperty(name, theCloudcvr);
   }
   else if(name == SRP_KW)
   {
      result = new ossimStringProperty(name, theSrp);
   }
   else if(name == SENSMODE_KW)
   {
      result = new ossimStringProperty(name, thesSensmode);
   }
   else if(name == SENSNAME_KW)
   {
      result = new ossimStringProperty(name, theSensname);
   }
   else if(name == SOURCE_KW)
   {
      result = new ossimStringProperty(name, theSource);
   }
   else if(name == COMGEN_KW)
   {
      result = new ossimStringProperty(name, theComgen);
   }
   else if(name == SUBQUAL_KW)
   {
      result = new ossimStringProperty(name, theSubqual);
   }
   else if(name == PIAMSNNUM_KW)
   {
      result = new ossimStringProperty(name, thePiamsnnum);
   }
   else if(name == CAMSPECS_KW)
   {
      result = new ossimStringProperty(name, theCamspecs);
   }
   else if(name == PROJID_KW)
   {
      result = new ossimStringProperty(name, theProjid);
   }
   else if(name == GENERATION_KW)
   {
      result = new ossimStringProperty(name, theGeneration);
   }
   else if(name == ESD_KW)
   {
      result = new ossimStringProperty(name, theEsd);
   }
   else if(name == OTHERCOND_KW)
   {
      result = new ossimStringProperty(name, theOthercond);
   }
   else if(name == MEANGSD_KW)
   {
      result = new ossimStringProperty(name, theMeanGsd);
   }
   else if(name == IDATUM_KW)
   {
      result = new ossimStringProperty(name, theIdatum);
   }
   else if(name == IELLIP_KW)
   {
      result = new ossimStringProperty(name, theIellip);
   }
   else if(name == PREPROC_KW)
   {
      result = new ossimStringProperty(name, thePreproc);
   }
   else if(name == IPROJ_KW)
   {
      result = new ossimStringProperty(name, theIproj);
   }
   else if(name == SATTRACK_KW)
   {
      result = new ossimStringProperty(name, theSattrack);
   }
   else
   {
      return ossimNitfRegisteredTag::getProperty(name);
   }

   return result;
}

void ossimNitfPiaimcTag::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   ossimNitfRegisteredTag::getPropertyNames(propertyNames);
   
   propertyNames.push_back(CLOUDCVR_KW);
   propertyNames.push_back(SRP_KW);
   propertyNames.push_back(SENSMODE_KW);
   propertyNames.push_back(SENSNAME_KW);
   propertyNames.push_back(SOURCE_KW);
   propertyNames.push_back(COMGEN_KW);
   propertyNames.push_back(SUBQUAL_KW);
   propertyNames.push_back(PIAMSNNUM_KW);
   propertyNames.push_back(CAMSPECS_KW);
   propertyNames.push_back(PROJID_KW);
   propertyNames.push_back(GENERATION_KW);
   propertyNames.push_back(ESD_KW);
   propertyNames.push_back(OTHERCOND_KW);
   propertyNames.push_back(MEANGSD_KW);
   propertyNames.push_back(IDATUM_KW);
   propertyNames.push_back(IELLIP_KW);
   propertyNames.push_back(PREPROC_KW);
   propertyNames.push_back(IPROJ_KW);
   propertyNames.push_back(SATTRACK_KW);
}
