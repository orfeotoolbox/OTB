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
// $Id: ossimNitfRegisteredTagFactory.cpp 14241 2009-04-07 19:59:23Z dburken $
#include <ossim/support_data/ossimNitfRegisteredTagFactory.h>
#include <ossim/support_data/ossimNitfBlockaTag.h>
#include <ossim/support_data/ossimNitfCscrnaTag.h>
#include <ossim/support_data/ossimNitfCsdidaTag.h>
#include <ossim/support_data/ossimNitfCsexraTag.h>
#include <ossim/support_data/ossimNitfGeoPositioningTag.h>
#include <ossim/support_data/ossimNitfIchipbTag.h>
#include <ossim/support_data/ossimNitfLocalGeographicTag.h>
#include <ossim/support_data/ossimNitfLocalCartographicTag.h>
#include <ossim/support_data/ossimNitfPiaimcTag.h>
#include <ossim/support_data/ossimNitfProjectionParameterTag.h>
#include <ossim/support_data/ossimNitfRpcBTag.h>
#include <ossim/support_data/ossimNitfRpcATag.h>
#include <ossim/support_data/ossimNitfStdidcTag.h>
#include <ossim/support_data/ossimNitfUse00aTag.h>


RTTI_DEF1(ossimNitfRegisteredTagFactory, "ossimNitfRegisteredTagFactory", ossimNitfTagFactory);

ossimNitfRegisteredTagFactory* ossimNitfRegisteredTagFactory::theInstance = NULL;

static const char BLOCKA_TAG[]               = "BLOCKA";
static const char CSCRNA_TAG[]               = "CSCRNA";
static const char CSDIDA_TAG[]               = "CSDIDA";
static const char CSEXRA_TAG[]               = "CSEXRA";
static const char GEO_POSITIONING_TAG[]      = "GEOPSB";
static const char ICHIPB_TAG[]               = "ICHIPB";
static const char LOCAL_GEOGRAPHIC_TAG[]     = "GEOLOB";
static const char LOCAL_CARTOGRAPHIC_TAG[]   = "MAPLOB";
static const char PIAIMC_TAG[]               = "PIAIMC";
static const char PROJECTION_PARAMETER_TAG[] = "PRJPSB";
static const char RPCB_TAG[]                 = "RPC00B";
static const char RPCA_TAG[]                 = "RPC00A";
static const char STDIDC_TAG[]               = "STDIDC";
static const char USE00A_TAG[]               = "USE00A";

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
   
   if(tagName == BLOCKA_TAG)
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
   else if(tagName == GEO_POSITIONING_TAG)
   {
      return new ossimNitfGeoPositioningTag;
   }
   else if(tagName == ICHIPB_TAG)
   {
      return new ossimNitfIchipbTag;
   }
   else if(tagName == LOCAL_GEOGRAPHIC_TAG)
   {
      return new ossimNitfLocalGeographicTag;
   }
   else if(tagName == LOCAL_CARTOGRAPHIC_TAG)
   {
      return new ossimNitfLocalCartographicTag;
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
   else if (tagName == STDIDC_TAG)
   {
      return new ossimNitfStdidcTag;
   }
   else if (tagName == USE00A_TAG)
   {
      return new ossimNitfUse00aTag;
   }
   
   return NULL;
}
