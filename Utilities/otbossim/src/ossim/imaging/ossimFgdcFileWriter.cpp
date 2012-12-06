//----------------------------------------------------------------------------
// Copyright (c) 2005, David Burken, all rights reserved.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: Class to write out meta data in a Federal Geographic Data
// Committe (FGDC) format.
//
//----------------------------------------------------------------------------
// $Id: ossimFgdcFileWriter.cpp 21631 2012-09-06 18:10:55Z dburken $


#include <ossim/imaging/ossimFgdcFileWriter.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimXmlNode.h>
#include <ossim/imaging/ossimImageGeometry.h>
#include <ossim/imaging/ossimImageSource.h>
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/projection/ossimMapProjectionInfo.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <fstream>
using namespace std;

RTTI_DEF1(ossimFgdcFileWriter, "ossimFgdcFileWriter", ossimMetadataFileWriter)

static ossimTrace traceDebug("ossimFgdcFileWriter:debug");

   
ossimFgdcFileWriter::ossimFgdcFileWriter()
   :
   theIndentionLevel(0),
   theTabString("\t"),
   theTemplate(),
   theSourceImageFilename()
{
}

ossimFgdcFileWriter::~ossimFgdcFileWriter()
{
}

bool ossimFgdcFileWriter::writeFile()
{
   if(theFilename == ossimFilename::NIL)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimFgdcFileWriter::writeFile no filename set!" << endl;
      return false;
   }
   
   if( !theInputConnection )
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimFgdcFileWriter::writeFile no input connection!" << endl;
      return false;
   }

   ofstream os(theFilename.c_str());
   if (!os)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimFgdcFileWriter::writeFile no input connection!" << endl;
      return false;
   }

   // Get the geometry from the input.
   ossimMapProjection* mapProj = 0;
   ossimRefPtr<ossimImageGeometry> inputGeom = theInputConnection->getImageGeometry();
   if ( inputGeom.valid() ) mapProj = PTR_CAST(ossimMapProjection, inputGeom->getProjection());
   if (!mapProj)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimFgdcFileWriter::writeFile Not map projected."
         << endl;
      return false;
   }

   ossimMapProjectionInfo mpi(mapProj, theInputConnection->getBoundingRect());

   os << "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" ?>\n"
      << "<?xml-stylesheet type=\"text/xsl\" href=\"/common/fgdc_classic.xsl\" ?>\n";
   openTag(os, ossimString("metadata"),  true);
   openTag(os, ossimString("idinfo"),    true);
   openTag(os, ossimString("citation"),  true);
   openTag(os, ossimString("citeinfo"),  true);
   
   openTag(os, ossimString("origin"),    false);
   os << find(ossimString("/metadata/idinfo/citation/citeinfo/origin"));
   closeTag(os, ossimString("origin"),   false);
   
   openTag(os, ossimString("pubdate"),    false);
   os << find(ossimString("/metadata/idinfo/citation/citeinfo/pubdate"));
   closeTag(os, ossimString("pubdate"),   false);

   openTag(os, ossimString("title"),    false);
   // os << find(ossimString("/metadata/idinfo/citation/citeinfo/title"));
   os << theSourceImageFilename.c_str();
   closeTag(os, ossimString("title"),   false);

   openTag(os, ossimString("edition"),    false);
   os << find(ossimString("/metadata/idinfo/citation/citeinfo/edition"));
   closeTag(os, ossimString("edition"),   false);

   openTag(os, ossimString("geoform"),    false);
   os << find(ossimString("/metadata/idinfo/citation/citeinfo/geoform"));
   closeTag(os, ossimString("geoform"),   false);

   openTag(os, ossimString("serinfo"),  true);
   openTag(os, ossimString("sername"),    false);
   os << find(ossimString("/metadata/idinfo/citation/citeinfo/serinfo/sername"));
   closeTag(os, ossimString("sername"),   false);

   openTag(os, ossimString("issue"),    false);
   os << theSourceImageFilename.c_str();
   closeTag(os, ossimString("issue"),   false);

   closeTag(os, ossimString("serinfo"),  true);

   openTag(os, ossimString("pubinfo"),  true);

   openTag(os, ossimString("pubplace"),    false);
   os << find(ossimString("/metadata/idinfo/citation/citeinfo/pubinfo/pubplace"));
   closeTag(os, ossimString("pubplace"),   false);

   openTag(os, ossimString("publish"),    false);
   os << find(ossimString("/metadata/idinfo/citation/citeinfo/pubinfo/publish"));
   closeTag(os, ossimString("publish"),   false);

   closeTag(os, ossimString("pubinfo"),  true);
   closeTag(os, ossimString("citeinfo"),  true);
   closeTag(os, ossimString("citation"),  true);

   openTag(os, ossimString("descript"),  true);

   openTag(os, ossimString("abstract"),    false);
   os << find(ossimString("/metadata/idinfo/descript/abstract"));
   closeTag(os, ossimString("abstract"),   false);

   openTag(os, ossimString("purpose"),    false);
   os << find(ossimString("/metadata/idinfo/descript/purpose"));
   closeTag(os, ossimString("purpose"),   false);

   openTag(os, ossimString("supplinf"),    false);
   os << find(ossimString("/metadata/idinfo/descript/supplinf"));
   closeTag(os, ossimString("supplinf"),   false);

   closeTag(os, ossimString("descript"),  true);

   openTag(os, ossimString("timeperd"),  true);
   openTag(os, ossimString("timeinfo"),  true);
   openTag(os, ossimString("sngdate"),  true);

   openTag(os, ossimString("caldate"),    false);
   os << find(ossimString("/metadata/idinfo/timeperd/timeinfo/sngdate/caldate"));
   closeTag(os, ossimString("caldate"),   false);
   
   closeTag(os, ossimString("sngdate"),  true);
   closeTag(os, ossimString("timeinfo"),  true);

   
   openTag(os, ossimString("current"),    false);
   os << find(ossimString("/metadata/idinfo/timeperd/current"));
   closeTag(os, ossimString("current"),   false);

   closeTag(os, ossimString("timeperd"),  true);

   openTag(os, ossimString("status"),  true);

   openTag(os, ossimString("progress"),    false);
   os << find(ossimString("/metadata/idinfo/status/progress"));
   closeTag(os, ossimString("progress"),   false);

   openTag(os, ossimString("update"),    false);
   os << find(ossimString("/metadata/idinfo/status/update"));
   closeTag(os, ossimString("update"),   false);

   closeTag(os, ossimString("status"),  true);

   openTag(os, ossimString("spdom"),  true);
   openTag(os, ossimString("bounding"),  true);

   if (mapProj->isGeographic())
   {
      openTag(os, ossimString("westbc"),    false);
      os << ossimString::toString(mpi.ulGroundPt().lon);
      closeTag(os, ossimString("westbc"),   false);
      
      openTag(os, ossimString("eastbc"),    false);
      os << ossimString::toString(mpi.lrGroundPt().lon);
      closeTag(os, ossimString("eastbc"),   false);
      
      openTag(os, ossimString("northbc"),    false);
      os << ossimString::toString(mpi.ulGroundPt().lat);
      closeTag(os, ossimString("northbc"),   false);
      
      openTag(os, ossimString("southbc"),    false);
      os << ossimString::toString(mpi.lrGroundPt().lat);
      closeTag(os, ossimString("southbc"),   false);
   }
   else
   {
      openTag(os, ossimString("westbc"),    false);
      os << ossimString::toString(mpi.ulEastingNorthingPt().x);
      closeTag(os, ossimString("westbc"),   false);
      
      openTag(os, ossimString("eastbc"),    false);
      os << ossimString::toString(mpi.lrEastingNorthingPt().x);
      closeTag(os, ossimString("eastbc"),   false);
      
      openTag(os, ossimString("northbc"),    false);
      os << ossimString::toString(mpi.ulEastingNorthingPt().y);
      closeTag(os, ossimString("northbc"),   false);
      
      openTag(os, ossimString("southbc"),    false);
      os << ossimString::toString(mpi.ulEastingNorthingPt().y);
      closeTag(os, ossimString("southbc"),   false);
   }
   
   closeTag(os, ossimString("bounding"),  true);
   closeTag(os, ossimString("spdom"),    true);

   openTag(os, ossimString("keywords"), true);

   openTag(os, ossimString("theme"), true);

   openTag(os, ossimString("themekt"), false);
   os << find(ossimString("/metadata/idinfo/keywords/theme/themekt"));
   closeTag(os, ossimString("themekt"), false);

   openTag(os, ossimString("themekey"), false);
   os << find(ossimString("/metadata/idinfo/keywords/theme/themekey"));
   closeTag(os, ossimString("themekey"), false);
   
   closeTag(os, ossimString("theme"), true);

   closeTag(os, ossimString("keywords"), true);

   openTag(os, ossimString("accconst"), false);
   os << find(ossimString("/metadata/idinfo/accconst"));
   closeTag(os, ossimString("accconst"), false);

   openTag(os, ossimString("useconst"), false);
   os << find(ossimString("/metadata/idinfo/useconst"));
   closeTag(os, ossimString("useconst"), false);

   openTag(os, ossimString("ptcontac"), true);
   
   openTag(os, ossimString("cntinfo"), true);
   
   openTag(os, ossimString("cntperp"), true);
   openTag(os, ossimString("cntper"), false);
   os << find(ossimString("/metadata/idinfo/ptcontac/cntinfo/cntperp/cntper"));
   closeTag(os, ossimString("cntper"), false);
   closeTag(os, ossimString("cntperp"), true);

   openTag(os, ossimString("cntaddr"), true);

   openTag(os, ossimString("addrtype"), false);
   os << find(ossimString("/metadata/idinfo/ptcontac/cntinfo/cntaddr/addrtype"));
   closeTag(os, ossimString("addrtype"), false);

   openTag(os, ossimString("address"), false);
   os << find(ossimString("/metadata/idinfo/ptcontac/cntinfo/cntaddr/address"));
   closeTag(os, ossimString("address"), false);

   openTag(os, ossimString("city"), false);
   os << find(ossimString("/metadata/idinfo/ptcontac/cntinfo/cntaddr/city"));
   closeTag(os, ossimString("city"), false);

   openTag(os, ossimString("state"), false);
   os << find(ossimString("/metadata/idinfo/ptcontac/cntinfo/cntaddr/state"));
   closeTag(os, ossimString("state"), false);

   openTag(os, ossimString("postal"), false);
   os << find(ossimString("/metadata/idinfo/ptcontac/cntinfo/cntaddr/postal"));
   closeTag(os, ossimString("postal"), false);

   openTag(os, ossimString("country"), false);
   os << find(ossimString("/metadata/idinfo/ptcontac/cntinfo/cntaddr/country"));
   closeTag(os, ossimString("country"), false);

   closeTag(os, ossimString("cntaddr"), true);

   openTag(os, ossimString("cntvoice"), false);
   os << find(ossimString("/metadata/idinfo/ptcontac/cntinfo/cntvoice"));
   closeTag(os, ossimString("cntvoice"), false);
  
   openTag(os, ossimString("cntfax"), false);

   os << find(ossimString("/metadata/idinfo/ptcontac/cntinfo/cntfax"));
   closeTag(os, ossimString("cntfax"), false);
  
   openTag(os, ossimString("cntemail"), false);
   os << find(ossimString("/metadata/idinfo/ptcontac/cntinfo/cntemail"));
   closeTag(os, ossimString("cntemail"), false);
   
   closeTag(os, ossimString("cntinfo"), true);
   closeTag(os, ossimString("ptcontac"), true);

   openTag(os, ossimString("secinfo"), true);

   openTag(os, ossimString("secclass"), false);
   os << find(ossimString("/metadata/idinfo/secinfo/secclass"));
   closeTag(os, ossimString("secclass"), false);

   openTag(os, ossimString("secsys"), false);
   os << find(ossimString("/metadata/idinfo/secinfo/secsys"));
   closeTag(os, ossimString("secsys"), false);

   openTag(os, ossimString("sechandl"), false);
   os << find(ossimString("/metadata/idinfo/secinfo/sechandl"));
   closeTag(os, ossimString("sechandl"), false);

   closeTag(os, ossimString("secinfo"), true);
   closeTag(os, ossimString("idinfo"), true);
   
   openTag(os, ossimString("distinfo"), true);
   openTag(os, ossimString("distrib"), true);
   openTag(os, ossimString("cntinfo"), true);
   openTag(os, ossimString("cntperp"), true);
   openTag(os, ossimString("cntper"), false);
   os << find(ossimString("/metadata/distinfo/distrib/cntinfo/cntperp/cntper"));

   closeTag(os, ossimString("cntper"), false);
   closeTag(os, ossimString("cntperp"), true);

   openTag(os, ossimString("cntaddr"), true);

   openTag(os, ossimString("addrtype"), false);
   os << find(ossimString("/metadata/distinfo/distrib/cntinfo/cntaddr/addrtype"));
   closeTag(os, ossimString("addrtype"), false);

   openTag(os, ossimString("address"), false);
   os << find(ossimString("/metadata/distinfo/distrib/cntinfo/cntaddr/address"));
   closeTag(os, ossimString("address"), false);

   openTag(os, ossimString("city"), false);
   os << find(ossimString("/metadata/distinfo/distrib/cntinfo/cntaddr/city"));
   closeTag(os, ossimString("city"), false);

   openTag(os, ossimString("state"), false);
   os << find(ossimString("/metadata/distinfo/distrib/cntinfo/cntaddr/state"));
   closeTag(os, ossimString("state"), false);

   openTag(os, ossimString("postal"), false);
   os << find(ossimString("/metadata/distinfo/distrib/cntinfo/cntaddr/postal"));
   closeTag(os, ossimString("postal"), false);

   openTag(os, ossimString("country"), false);
   os << find(ossimString("/metadata/distinfo/distrib/cntinfo/cntaddr/country"));
   closeTag(os, ossimString("country"), false);

   closeTag(os, ossimString("cntaddr"), true);

   openTag(os, ossimString("cntvoice"), false);
   os << find(ossimString("/metadata/distinfo/distrib/cntinfo/cntvoice"));
   closeTag(os, ossimString("cntvoice"), false);
  
   openTag(os, ossimString("cntfax"), false);
   os << find(ossimString("/metadata/distinfo/distrib/cntinfo/cntfax"));
   closeTag(os, ossimString("cntfax"), false);
  
   openTag(os, ossimString("cntemail"), false);
   os << find(ossimString("/metadata/distinfo/distrib/cntinfo/cntemail"));
   closeTag(os, ossimString("cntemail"), false);
   
   closeTag(os, ossimString("cntinfo"), true);
   closeTag(os, ossimString("distrib"), true);

   openTag(os, ossimString("resdesc"), false);
   os << find(ossimString("/metadata/distinfo/resdesc"));
   closeTag(os, ossimString("resdesc"), false);

   openTag(os, ossimString("distliab"), false);
   os << find(ossimString("/metadata/distinfo/distliab"));
   closeTag(os, ossimString("distliab"), false);

   openTag(os, ossimString("stdorder"), true);
   openTag(os, ossimString("digform"), true);
   openTag(os, ossimString("digtinfo"), true);
   
   openTag(os, ossimString("formname"), false);
   os << find(ossimString("/metadata/distinfo/stdorder/digform/digtinfo/formname"));
   closeTag(os, ossimString("formname"), false);

   closeTag(os, ossimString("digtinfo"), true);

   openTag(os, ossimString("digtopt"), true);
   openTag(os, ossimString("onlinopt"), true);   
   openTag(os, ossimString("computer"), true);
   openTag(os, ossimString("networka"), true);

   openTag(os, ossimString("networkr"), false);
   os << find(ossimString("/metadata/distinfo/stdorder/digform/digtopt/onlinopt/computer/networka/networkr"));
   closeTag(os, ossimString("networkr"), false);

   closeTag(os, ossimString("networka"), true);
   closeTag(os, ossimString("computer"), true);
   closeTag(os, ossimString("onlinopt"), true);
   closeTag(os, ossimString("digtopt"), true);
   closeTag(os, ossimString("digform"), true);

   openTag(os, ossimString("fees"), false);
   os << find(ossimString("/metadata/distinfo/stdorder/fees"));
   closeTag(os, ossimString("fees"), false);

   closeTag(os, ossimString("stdorder"), true);
   closeTag(os, ossimString("distinfo"), true);

   openTag(os, ossimString("metainfo"), true);

   openTag(os, ossimString("metd"), false);
   os << find(ossimString("/metadata/metainfo/metd"));
   closeTag(os, ossimString("metd"), false);

   openTag(os, ossimString("metc"), true);
   openTag(os, ossimString("cntinfo"), true);
   openTag(os, ossimString("cntperp"), true);
   
   openTag(os, ossimString("cntper"), false);
   os << find(ossimString("/metadata/metainfo/metc/cntinfo/cntperp/cntper"));
   closeTag(os, ossimString("cntper"), false);

   closeTag(os, ossimString("cntperp"), true);
   
   openTag(os, ossimString("cntaddr"), true);

   openTag(os, ossimString("addrtype"), false);
   os << find(ossimString("/metadata/metainfo/metc/cntinfo/cntaddr/addrtype"));
   closeTag(os, ossimString("addrtype"), false);

   openTag(os, ossimString("address"), false);
   os << find(ossimString("/metadata/metainfo/metc/cntinfo/cntaddr/address"));
   closeTag(os, ossimString("address"), false);

   openTag(os, ossimString("city"), false);
   os << find(ossimString("/metadata/metainfo/metc/cntinfo/cntaddr/city"));
   closeTag(os, ossimString("city"), false);

   openTag(os, ossimString("state"), false);
   os << find(ossimString("/metadata/metainfo/metc/cntinfo/cntaddr/state"));
   closeTag(os, ossimString("state"), false);

   openTag(os, ossimString("postal"), false);
   os << find(ossimString("/metadata/metainfo/metc/cntinfo/cntaddr/postal"));
   closeTag(os, ossimString("postal"), false);

   openTag(os, ossimString("country"), false);
   os << find(ossimString("/metadata/metainfo/metc/cntinfo/cntaddr/country"));
   closeTag(os, ossimString("country"), false);

   closeTag(os, ossimString("cntaddr"), true);

   openTag(os, ossimString("cntvoice"), false);
   os << find(ossimString("/metadata/metainfo/metc/cntinfo/cntvoice"));
   closeTag(os, ossimString("cntvoice"), false);
  
   openTag(os, ossimString("cntfax"), false);
   os << find(ossimString("/metadata/metainfo/metc/cntinfo/cntfax"));
   closeTag(os, ossimString("cntfax"), false);
  
   openTag(os, ossimString("cntemail"), false);
   os << find(ossimString("/metadata/metainfo/metc/cntinfo/cntemail"));
   closeTag(os, ossimString("cntemail"), false);
   
   closeTag(os, ossimString("cntinfo"), true);
   closeTag(os, ossimString("metc"), true);

   openTag(os, ossimString("metstdn"), false);
   os << find(ossimString("/metadata/metainfo/metstdn"));
   closeTag(os, ossimString("metstdn"), false);

   openTag(os, ossimString("metstdv"), false);
   os << find(ossimString("/metadata/metainfo/metstdv"));
   closeTag(os, ossimString("metstdv"), false);

   closeTag(os, ossimString("metainfo"), true);
   closeTag(os, ossimString("metadata"), true);

   os.close();

   return true;
}

bool ossimFgdcFileWriter::writeTemplate(const ossimFilename& file) const
{
   ofstream os(file.c_str());
   if (!os)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimFgdcFileWriter::writeTemplate Could not open: " << file
         << endl;
      return false;
   }

   os << "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" ?>\n"
      << "<?xml-stylesheet type=\"text/xsl\" href=\"/common/fgdc_classic.xsl\" ?>\n";
   openTag(os, ossimString("metadata"),  true);
   openTag(os, ossimString("idinfo"),    true);
   openTag(os, ossimString("citation"),  true);
   openTag(os, ossimString("citeinfo"),  true);
   
   openTag(os, ossimString("origin"),    false);
   closeTag(os, ossimString("origin"),   false);
   
   openTag(os, ossimString("pubdate"),    false);
   closeTag(os, ossimString("pubdate"),   false);

   openTag(os, ossimString("title"),    false);
   closeTag(os, ossimString("title"),   false);

   openTag(os, ossimString("edition"),    false);
   closeTag(os, ossimString("edition"),   false);

   openTag(os, ossimString("geoform"),    false);
   closeTag(os, ossimString("geoform"),   false);

   openTag(os, ossimString("serinfo"),  true);
   openTag(os, ossimString("sername"),    false);

   closeTag(os, ossimString("sername"),   false);

   openTag(os, ossimString("issue"),    false);
   closeTag(os, ossimString("issue"),   false);

   closeTag(os, ossimString("serinfo"),  true);

   openTag(os, ossimString("pubinfo"),  true);

   openTag(os, ossimString("pubplace"),    false);

   closeTag(os, ossimString("pubplace"),   false);

   openTag(os, ossimString("publish"),    false);
   closeTag(os, ossimString("publish"),   false);

   closeTag(os, ossimString("pubinfo"),  true);
   closeTag(os, ossimString("citeinfo"),  true);
   closeTag(os, ossimString("citation"),  true);

   openTag(os, ossimString("descript"),  true);

   openTag(os, ossimString("abstract"),    false);
   closeTag(os, ossimString("abstract"),   false);

   openTag(os, ossimString("purpose"),    false);
   closeTag(os, ossimString("purpose"),   false);

   openTag(os, ossimString("supplinf"),    false);
   closeTag(os, ossimString("supplinf"),   false);

   closeTag(os, ossimString("descript"),  true);

   openTag(os, ossimString("timeperd"),  true);
   openTag(os, ossimString("timeinfo"),  true);
   openTag(os, ossimString("sngdate"),  true);

   openTag(os, ossimString("caldate"),    false);
   closeTag(os, ossimString("caldate"),   false);
   
   closeTag(os, ossimString("sngdate"),  true);
   closeTag(os, ossimString("timeinfo"),  true);

   
   openTag(os, ossimString("current"),    false);
   closeTag(os, ossimString("current"),   false);

   closeTag(os, ossimString("timeperd"),  true);

   openTag(os, ossimString("status"),  true);

   openTag(os, ossimString("progress"),    false);
   closeTag(os, ossimString("progress"),   false);

   openTag(os, ossimString("update"),    false);
   closeTag(os, ossimString("update"),   false);

   closeTag(os, ossimString("status"),  true);

   openTag(os, ossimString("spdom"),  true);
   openTag(os, ossimString("bounding"),  true);

   openTag(os, ossimString("westbc"),    false);
   closeTag(os, ossimString("westbc"),   false);
   openTag(os, ossimString("eastbc"),    false);

   closeTag(os, ossimString("eastbc"),   false);
   openTag(os, ossimString("northbc"),    false);

   closeTag(os, ossimString("northbc"),   false);
   openTag(os, ossimString("southbc"),    false);

   closeTag(os, ossimString("southbc"),   false);

   closeTag(os, ossimString("bounding"),  true);
   closeTag(os, ossimString("spdom"),    true);

   openTag(os, ossimString("keywords"), true);

   openTag(os, ossimString("theme"), true);

   openTag(os, ossimString("themekt"), false);
   closeTag(os, ossimString("themekt"), false);

   openTag(os, ossimString("themekey"), false);
   closeTag(os, ossimString("themekey"), false);
   
   closeTag(os, ossimString("theme"), true);

   closeTag(os, ossimString("keywords"), true);

   openTag(os, ossimString("accconst"), false);
   closeTag(os, ossimString("accconst"), false);

   openTag(os, ossimString("useconst"), false);
   closeTag(os, ossimString("useconst"), false);

   openTag(os, ossimString("ptcontac"), true);
   
   openTag(os, ossimString("cntinfo"), true);
   
   openTag(os, ossimString("cntperp"), true);
   openTag(os, ossimString("cntper"), false);
   closeTag(os, ossimString("cntper"), false);
   closeTag(os, ossimString("cntperp"), true);

   openTag(os, ossimString("cntaddr"), true);

   openTag(os, ossimString("addrtype"), false);
   closeTag(os, ossimString("addrtype"), false);

   openTag(os, ossimString("address"), false);
   closeTag(os, ossimString("address"), false);

   openTag(os, ossimString("city"), false);
   closeTag(os, ossimString("city"), false);

   openTag(os, ossimString("state"), false);
   closeTag(os, ossimString("state"), false);

   openTag(os, ossimString("postal"), false);
   closeTag(os, ossimString("postal"), false);

   openTag(os, ossimString("country"), false);
   closeTag(os, ossimString("country"), false);

   closeTag(os, ossimString("cntaddr"), true);

   openTag(os, ossimString("cntvoice"), false);
   closeTag(os, ossimString("cntvoice"), false);
  
   openTag(os, ossimString("cntfax"), false);
   closeTag(os, ossimString("cntfax"), false);
  
   openTag(os, ossimString("cntemail"), false);
   closeTag(os, ossimString("cntemail"), false);
   
   closeTag(os, ossimString("cntinfo"), true);
   
   closeTag(os, ossimString("ptcontac"), true);

   openTag(os, ossimString("secinfo"), true);
   openTag(os, ossimString("secclass"), false);
   closeTag(os, ossimString("secclass"), false);
   openTag(os, ossimString("secsys"), false);
   closeTag(os, ossimString("secsys"), false);
   openTag(os, ossimString("sechandl"), false);
   closeTag(os, ossimString("sechandl"), false);
   closeTag(os, ossimString("secinfo"), true);

   closeTag(os, ossimString("idinfo"), true);

   openTag(os, ossimString("distinfo"), true);
   openTag(os, ossimString("distrib"), true);
   openTag(os, ossimString("cntinfo"), true);
   openTag(os, ossimString("cntperp"), true);
   openTag(os, ossimString("cntper"), false);
   closeTag(os, ossimString("cntper"), false);
   closeTag(os, ossimString("cntperp"), true);

   openTag(os, ossimString("cntaddr"), true);

   openTag(os, ossimString("addrtype"), false);
   closeTag(os, ossimString("addrtype"), false);

   openTag(os, ossimString("address"), false);
   closeTag(os, ossimString("address"), false);

   openTag(os, ossimString("city"), false);
   closeTag(os, ossimString("city"), false);

   openTag(os, ossimString("state"), false);
   closeTag(os, ossimString("state"), false);

   openTag(os, ossimString("postal"), false);
   closeTag(os, ossimString("postal"), false);

   openTag(os, ossimString("country"), false);
   closeTag(os, ossimString("country"), false);

   closeTag(os, ossimString("cntaddr"), true);

   openTag(os, ossimString("cntvoice"), false);
   closeTag(os, ossimString("cntvoice"), false);
  
   openTag(os, ossimString("cntfax"), false);
   closeTag(os, ossimString("cntfax"), false);
  
   openTag(os, ossimString("cntemail"), false);
   closeTag(os, ossimString("cntemail"), false);
   
   closeTag(os, ossimString("cntinfo"), true);
   closeTag(os, ossimString("distrib"), true);

   openTag(os, ossimString("resdesc"), false);
   closeTag(os, ossimString("resdesc"), false);

   openTag(os, ossimString("distliab"), false);
   closeTag(os, ossimString("distliab"), false);

   openTag(os, ossimString("stdorder"), true);
   openTag(os, ossimString("digform"), true);
   openTag(os, ossimString("digtinfo"), true);
   
   openTag(os, ossimString("formname"), false);
   closeTag(os, ossimString("formname"), false);

   closeTag(os, ossimString("digtinfo"), true);

   openTag(os, ossimString("digtopt"), true);
   openTag(os, ossimString("onlinopt"), true);   
   openTag(os, ossimString("computer"), true);
   openTag(os, ossimString("networka"), true);

   openTag(os, ossimString("networkr"), false);
   closeTag(os, ossimString("networkr"), false);

   closeTag(os, ossimString("networka"), true);
   closeTag(os, ossimString("computer"), true);
   closeTag(os, ossimString("onlinopt"), true);
   closeTag(os, ossimString("digtopt"), true);
   closeTag(os, ossimString("digform"), true);

   openTag(os, ossimString("fees"), false);
   closeTag(os, ossimString("fees"), false);

   closeTag(os, ossimString("stdorder"), true);
   closeTag(os, ossimString("distinfo"), true);

   openTag(os, ossimString("metainfo"), true);

   openTag(os, ossimString("metd"), false);
   closeTag(os, ossimString("metd"), false);

   openTag(os, ossimString("metc"), true);
   openTag(os, ossimString("cntinfo"), true);
   openTag(os, ossimString("cntperp"), true);
   
   openTag(os, ossimString("cntper"), false);
   closeTag(os, ossimString("cntper"), false);

   closeTag(os, ossimString("cntperp"), true);
   
   openTag(os, ossimString("cntaddr"), true);

   openTag(os, ossimString("addrtype"), false);
   closeTag(os, ossimString("addrtype"), false);

   openTag(os, ossimString("address"), false);
   closeTag(os, ossimString("address"), false);

   openTag(os, ossimString("city"), false);
   closeTag(os, ossimString("city"), false);

   openTag(os, ossimString("state"), false);
   closeTag(os, ossimString("state"), false);

   openTag(os, ossimString("postal"), false);
   closeTag(os, ossimString("postal"), false);

   openTag(os, ossimString("country"), false);
   closeTag(os, ossimString("country"), false);

   closeTag(os, ossimString("cntaddr"), true);

   openTag(os, ossimString("cntvoice"), false);
   closeTag(os, ossimString("cntvoice"), false);
  
   openTag(os, ossimString("cntfax"), false);
   closeTag(os, ossimString("cntfax"), false);
  
   openTag(os, ossimString("cntemail"), false);
   closeTag(os, ossimString("cntemail"), false);
   
   closeTag(os, ossimString("cntinfo"), true);
   closeTag(os, ossimString("metc"), true);

   openTag(os, ossimString("metstdn"), false);
   closeTag(os, ossimString("metstdn"), false);

   openTag(os, ossimString("metstdv"), false);
   closeTag(os, ossimString("metstdv"), false);

   closeTag(os, ossimString("metainfo"), true);
   closeTag(os, ossimString("metadata"), true);

   os.close();

	if(traceDebug())
	{
		ossimNotify(ossimNotifyLevel_NOTICE)
      << "Wrote file:  " << file.c_str() << endl;
	}
   
   return true;
}
   
bool ossimFgdcFileWriter::loadState (const ossimKeywordlist& kwl,
                                     const char* prefix)
{
   const char* lookup = kwl.find(prefix, "template");
   if (lookup)
   {
      setTemplate(ossimFilename(lookup));
   }
   lookup = kwl.find(prefix, "source_image_filename");
   if (lookup)
   {
      theSourceImageFilename = lookup;
   }
   lookup = kwl.find(prefix, "tab_string");
   if (lookup)
   {
      theTabString = lookup;
   }
   lookup = kwl.find(prefix, "fgdc_template_doc");
   if (lookup)
   {
      setTemplate(ossimFilename(lookup));
   }
   
   return ossimMetadataFileWriter::loadState(kwl, prefix);
}

ossim_uint32 ossimFgdcFileWriter::getIndentionLevel() const
{
   return theIndentionLevel;
}

void ossimFgdcFileWriter::setIndentionLevel(ossim_uint32 level)
{
   theIndentionLevel = level;
}

ossimString ossimFgdcFileWriter::getTabString() const
{
   return theTabString;
}

void ossimFgdcFileWriter::setTabString(const ossimString& tabString)
{
   theTabString = tabString;
}

void ossimFgdcFileWriter::openTag(std::ostream& os,
                                  const ossimString& tag,
                                  bool newLine) const
{
   if ( theIndentionLevel && theTabString.size() )
   {
      ossimString tab;
      for (ossim_uint32 i = 0; i < theIndentionLevel; ++i)
      {
         tab += theTabString;
      }

      os << tab.c_str();
   }
   
   os << "<" << tag.c_str() << ">";

   if (newLine)
   {
      os << "\n";
      ++theIndentionLevel;
   }
}

void ossimFgdcFileWriter::closeTag(std::ostream& os,
                                   const ossimString& tag,
                                   bool decrementLevel) const
{
   if ( decrementLevel && (theIndentionLevel > 1) && theTabString.size() )
   {
      ossimString tab;
      for (ossim_uint32 i = 0; i < theIndentionLevel-1; ++i)
      {
         tab += theTabString;
      }

      os << tab.c_str();
   }
   
   os << "</" << tag.c_str() << ">\n";

   if (theIndentionLevel && decrementLevel)
   {
      --theIndentionLevel;
   }
}

void ossimFgdcFileWriter::setTemplate(const ossimFilename& xmlTemplateFile)
{
   theTemplate.openFile(xmlTemplateFile);
}

ossimString ossimFgdcFileWriter::find(const ossimString& xpath) const
{
   ossimString result;

   if (theTemplate.getErrorStatus())
   {
      return result;
   }
   
   vector< ossimRefPtr<ossimXmlNode> > xml_nodes;
   theTemplate.findNodes(xpath, xml_nodes);

   if (xml_nodes.size())
   {
      if (xml_nodes[0].valid())
      {
         result = xml_nodes[0]->getText();
      }
   }
   return result;
}

void ossimFgdcFileWriter::getMetadatatypeList(
   std::vector<ossimString>& metadatatypeList) const
{
   metadatatypeList.push_back(ossimString("ossim_fgdc")); 
}

bool ossimFgdcFileWriter::hasMetadataType(
   const ossimString& metadataType)const
{
   return (metadataType == "ossim_fgdc");
}
