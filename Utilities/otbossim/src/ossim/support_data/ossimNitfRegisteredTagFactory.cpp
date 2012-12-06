//*******************************************************************
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
// 
// Author: Garrett Potts
//
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfRegisteredTagFactory.cpp 20689 2012-03-14 18:31:20Z gpotts $

#include <ossim/support_data/ossimNitfRegisteredTagFactory.h>

#include <ossim/support_data/ossimNitfAcftbTag.h>
#include <ossim/support_data/ossimNitfAimidbTag.h>
#include <ossim/support_data/ossimNitfBlockaTag.h>
#include <ossim/support_data/ossimNitfCscrnaTag.h>
#include <ossim/support_data/ossimNitfCsdidaTag.h>
#include <ossim/support_data/ossimNitfCsexraTag.h>
#include <ossim/support_data/ossimNitfCsexraTag.h>
#include <ossim/support_data/ossimNitfEngrdaTag.h>
#include <ossim/support_data/ossimNitfGeoPositioningTag.h>
#include <ossim/support_data/ossimNitfIchipbTag.h>
#include <ossim/support_data/ossimNitfJ2klraTag.h>
#include <ossim/support_data/ossimNitfLocalGeographicTag.h>
#include <ossim/support_data/ossimNitfLocalCartographicTag.h>
#include <ossim/support_data/ossimNitfMstgtaTag.h>
#include <ossim/support_data/ossimNitfPiaimcTag.h>
#include <ossim/support_data/ossimNitfProjectionParameterTag.h>
#include <ossim/support_data/ossimNitfRpcBTag.h>
#include <ossim/support_data/ossimNitfRpcATag.h>
#include <ossim/support_data/ossimNitfSensraTag.h>
#include <ossim/support_data/ossimNitfStdidcTag.h>
#include <ossim/support_data/ossimNitfUse00aTag.h>
#include <ossim/support_data/ossimNitfHistoaTag.h>
#include <ossim/support_data/ossimNitfCsccgaTag.h>
#include <ossim/support_data/ossimNitfCsproaTag.h>
#include <ossim/support_data/ossimNitfExoptaTag.h>

RTTI_DEF1(ossimNitfRegisteredTagFactory, "ossimNitfRegisteredTagFactory", ossimNitfTagFactory);

ossimNitfRegisteredTagFactory* ossimNitfRegisteredTagFactory::theInstance = NULL;

static const char ACFTB_TAG[]                = "ACFTB";
static const char AIMIDB_TAG[]               = "AIMIDB";
static const char BLOCKA_TAG[]               = "BLOCKA";
static const char CSCRNA_TAG[]               = "CSCRNA";
static const char CSDIDA_TAG[]               = "CSDIDA";
static const char CSEXRA_TAG[]               = "CSEXRA";
static const char ENGRDA_TAG[]               = "ENGRDA";
static const char GEO_POSITIONING_TAG[]      = "GEOPSB";
static const char ICHIPB_TAG[]               = "ICHIPB";
static const char J2KLRA_TAG[]               = "J2KLRA";
static const char LOCAL_GEOGRAPHIC_TAG[]     = "GEOLOB";
static const char LOCAL_CARTOGRAPHIC_TAG[]   = "MAPLOB";
static const char MSTGTA_TAG[]               = "MSTGTA";
static const char PIAIMC_TAG[]               = "PIAIMC";
static const char PROJECTION_PARAMETER_TAG[] = "PRJPSB";
static const char RPCB_TAG[]                 = "RPC00B";
static const char RPCA_TAG[]                 = "RPC00A";
static const char SENSRA_TAG[]               = "SENSRA";
static const char STDIDC_TAG[]               = "STDIDC";
static const char USE00A_TAG[]               = "USE00A";
static const char HISTOA_TAG[]               = "HISTOA";
static const char CSCCGA_TAG[]               = "CSCCGA";
static const char CSPROA_TAG[]               = "CSPROA";
static const char EXOPTA_TAG[]               = "EXOPTA";

ossimNitfRegisteredTagFactory::ossimNitfRegisteredTagFactory()
{
   theInstance = this;
}

ossimNitfRegisteredTagFactory::~ossimNitfRegisteredTagFactory()
{
   theInstance = NULL;
}

ossimNitfRegisteredTagFactory* ossimNitfRegisteredTagFactory::instance()
{
   if(!theInstance)
   {
      theInstance = new ossimNitfRegisteredTagFactory;
   }

   return theInstance;
}

ossimRefPtr<ossimNitfRegisteredTag> ossimNitfRegisteredTagFactory::create(
   const ossimString& tagName)const
{
   ossimString name = ossimString(tagName).trim().upcase();
   
   if(tagName == ACFTB_TAG)
   {
      return new ossimNitfAcftbTag;
   }
   else if(tagName == AIMIDB_TAG)
   {
      return new ossimNitfAimidbTag;
   }
   else if(tagName == BLOCKA_TAG)
   {
      return new ossimNitfBlockaTag;
   }
   else if(tagName == CSCRNA_TAG)
   {
      return new ossimNitfCscrnaTag;
   }
   else if(tagName == CSDIDA_TAG)
   {
      return new ossimNitfCsdidaTag;
   }
   else if(tagName == CSEXRA_TAG)
   {
      return new ossimNitfCsexraTag;
   }
   else if(tagName == ENGRDA_TAG)
   {
      return new ossimNitfEngrdaTag;
   }
   else if(tagName == GEO_POSITIONING_TAG)
   {
      return new ossimNitfGeoPositioningTag;
   }
   else if(tagName == ICHIPB_TAG)
   {
      return new ossimNitfIchipbTag;
   }
   else if(tagName == J2KLRA_TAG)
   {
      return new ossimNitfJ2klraTag;
   }
   else if(tagName == LOCAL_GEOGRAPHIC_TAG)
   {
      return new ossimNitfLocalGeographicTag;
   }
   else if(tagName == LOCAL_CARTOGRAPHIC_TAG)
   {
      return new ossimNitfLocalCartographicTag;
   }
   else if(tagName == MSTGTA_TAG)
   {
      return new ossimNitfMstgtaTag;
   }
   else if(tagName == PIAIMC_TAG)
   {
      return new ossimNitfPiaimcTag;
   }
   else if(tagName == PROJECTION_PARAMETER_TAG)
   {
      return new ossimNitfProjectionParameterTag;
   }
   else if(tagName == RPCB_TAG)
   {
      return new ossimNitfRpcBTag;
   }
   else if(tagName == RPCA_TAG)
   {
      return new ossimNitfRpcATag;
   }
   else if (tagName == SENSRA_TAG)
   {
      return new ossimNitfSensraTag;
   }
   else if (tagName == STDIDC_TAG)
   {
      return new ossimNitfStdidcTag;
   }
   else if (tagName == USE00A_TAG)
   {
      return new ossimNitfUse00aTag;
   }
   else if(tagName == HISTOA_TAG)
   {
      return new ossimNitfHistoaTag;
   }
   else if(tagName == CSCCGA_TAG)
   {
      return new ossimNitfCsccgaTag;
   }
   else if(tagName == CSPROA_TAG)
   {
      return new ossimNitfCsproaTag;
   }
   else if(tagName == EXOPTA_TAG)
   {
      return new ossimNitfExoptaTag;
   }
   return NULL;
}
