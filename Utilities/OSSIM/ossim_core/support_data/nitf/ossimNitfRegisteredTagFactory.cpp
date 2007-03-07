//*******************************************************************
// Copyright (C) 2004 Intelligence Data Systems. 
//
// LICENSE: LGPL
//
// see top level LICENSE.txt
// 
// Author: Garrett Potts
//
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfRegisteredTagFactory.cpp,v 1.9 2005/10/13 21:20:02 dburken Exp $
#include <support_data/nitf/ossimNitfRegisteredTagFactory.h>
#include <support_data/nitf/ossimNitfBlockaTag.h>
#include <support_data/nitf/ossimNitfGeoPositioningTag.h>
#include <support_data/nitf/ossimNitfLocalGeographicTag.h>
#include <support_data/nitf/ossimNitfLocalCartographicTag.h>
#include <support_data/nitf/ossimNitfProjectionParameterTag.h>
#include <support_data/nitf/ossimNitfRpcBTag.h>
#include <support_data/nitf/ossimNitfRpcATag.h>
#include <support_data/nitf/ossimNitfStdidcTag.h>
#include <support_data/nitf/ossimNitfUse00aTag.h>


RTTI_DEF1(ossimNitfRegisteredTagFactory, "ossimNitfRegisteredTagFactory", ossimNitfTagFactory);

ossimNitfRegisteredTagFactory* ossimNitfRegisteredTagFactory::theInstance = NULL;

static const char BLOCKA_TAG[]               = "BLOCKA";
static const char GEO_POSITIONING_TAG[]      = "GEOPSB";
static const char LOCAL_GEOGRAPHIC_TAG[]     = "GEOLOB";
static const char LOCAL_CARTOGRAPHIC_TAG[]   = "MAPLOB";
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
   else if(tagName == GEO_POSITIONING_TAG)
   {
      return new ossimNitfGeoPositioningTag;
   }
   else if(tagName == LOCAL_GEOGRAPHIC_TAG)
   {
      return new ossimNitfLocalGeographicTag;
   }
   else if(tagName == LOCAL_CARTOGRAPHIC_TAG)
   {
      return new ossimNitfLocalCartographicTag;
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
