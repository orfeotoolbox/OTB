//*****************************************************************************
// FILE: ossimGridRemapEngineFactory.cc
//
// Copyright (C) 2001 ImageLinks, Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// AUTHOR: Oscar Kramer
//
// DESCRIPTION: Contains implementation of class ossimGridRemapEngineFactory
//
// LIMITATIONS: None.
//
//*****************************************************************************
//  $Id: ossimGridRemapEngineFactory.cpp,v 1.2 2005/02/11 15:07:35 dburken Exp $

#include <imaging/tonal_balance/ossimGridRemapEngineFactory.h>
#include <imaging/tonal_balance/ossimHsvGridRemapEngine.h>
#include <imaging/tonal_balance/ossimRgbGridRemapEngine.h>
#include <imaging/tonal_balance/ossimMonoGridRemapEngine.h>

//*****************************************************************************
//  STATIC METHOD: ossimGridRemapEngineFactory::create()
//  
//*****************************************************************************
ossimGridRemapEngine* ossimGridRemapEngineFactory::create(const char* s)
{
   if (!s)
      return 0;
   
   if (strcmp(s, "ossimHsvGridRemapEngine") == 0)
      return new ossimHsvGridRemapEngine;

   else  if (strcmp(s, "ossimRgbGridRemapEngine") == 0)
      return new ossimRgbGridRemapEngine;

   else  if (strcmp(s, "ossimMonoGridRemapEngine") == 0)
      return new ossimMonoGridRemapEngine;

   return 0;
}

