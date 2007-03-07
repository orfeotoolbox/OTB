//**************************************************************************
// FILE: ossimDtedFactory.cc
//
// Copyright (C) 2002 ImageLinks, Inc.
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description:
//
// Class definition for ossimDtedFactory.
//
// This is the base class interface for elevation source factories.  Contains
// pure virtual methods that all elevation source factories must implement.
//
//**************************************************************************
// $Id: ossimDtedFactory.cpp,v 1.13 2005/11/18 20:24:49 gpotts Exp $

#include <sstream>
#include <iomanip>
#include <elevation/formats/dted_cell/ossimDtedFactory.h>
#include <elevation/formats/dted_cell/ossimDtedHandler.h>
#include <base/common/ossimTrace.h>
#include <base/data_types/ossimGpt.h>
#include <elevation/ossimElevManager.h>
#include <base/context/ossimNotifyContext.h>

static ossimTrace traceDebug ("ossimDtedFactory:debug");

RTTI_DEF1(ossimDtedFactory, "ossimDtedFactory", ossimElevSourceFactory)

ossimDtedFactory::ossimDtedFactory()
   : ossimElevSourceFactory()
{}

ossimDtedFactory::ossimDtedFactory(const ossimFilename& dir)
   : ossimElevSourceFactory()
{
   theDirectory = dir;
}

ossimDtedFactory::~ossimDtedFactory()
{}

ossimElevSource* ossimDtedFactory::getNewElevSource(const ossimGpt& gpt) const
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "DEBUG ossimDtedFactory::getNewElevSource: Entered..."
         << std::endl;
   }
   
   ossimElevManager* mgr = ossimElevManager::instance();
   
   ossimElevSource* dted_ptr = NULL;
   
   if (theDirectory == ossimFilename::NIL)
   {
      ossimNotify(ossimNotifyLevel_FATAL)
         << "FATAL ossimDtedFactory::getNewElevSource: "
         << "DTED directory has not been set!"
         << "\nReturning null elevation source..."
         << std::endl;
      return dted_ptr;
   }
   
   // Build up a dted file name.
   ossimFilename dted_file_base;
   
   int ilon = static_cast<int>(floor(gpt.lond()));
   
   if (ilon < 0)
   {
      dted_file_base = "w";
   }
   else
   {
      dted_file_base = "e";
   }
   
   ilon = abs(ilon);
   ostringstream  s1;
   s1<< setfill('0')<<setw(3)<< ilon;
   
   dted_file_base += s1.str().c_str();//ossimString::toString(ilon);
   dted_file_base += "/";
   
   int ilat =  static_cast<int>(floor(gpt.latd()));
   if (ilat < 0)
   {
      dted_file_base += "s";
   }
   else
   {
      dted_file_base += "n";
   }

   ilat = abs(ilat);
   ostringstream  s2;

   s2<< setfill('0')<<setw(2)<<ilat;
   
   dted_file_base += s2.str().c_str();

   // Look for a dted file with a level 2 extension first.
   ossimFilename dted_name = theDirectory.dirCat(dted_file_base);
   dted_name += ".dt2";

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "DEBUG ossimDtedFactory::getNewElevSource:"
         << "\nSearching for file:  " << dted_name
         << std::endl;
   }
   
   if (dted_name.exists() && !(mgr->isCellOpen(dted_name)) )
   {
      dted_ptr = new ossimDtedHandler(dted_name);
      if ( (!(dted_ptr->getErrorStatus())) &&
           dted_ptr->pointHasCoverage(gpt) )
      {
         return dted_ptr;
      }
      else
      {
         delete dted_ptr;
         dted_ptr = NULL;
      }
   }

   // Look for a dted file with a level 1 extension next.
   dted_name = theDirectory.dirCat(dted_file_base);
   dted_name += ".dt1";

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "DEBUG ossimDtedFactory::getNewElevSource:"
         << "\nSearching for file:  " << dted_name
         << std::endl;
   }
   
   if (dted_name.exists() && !(mgr->isCellOpen(dted_name)) )
   {
      dted_ptr = new ossimDtedHandler(dted_name);
      if ( (!(dted_ptr->getErrorStatus())) &&
           dted_ptr->pointHasCoverage(gpt) )
      {
         return dted_ptr;
      }
      else
      {
         delete dted_ptr;
         dted_ptr = NULL;
      }
   }

   return dted_ptr;
}

void ossimDtedFactory::createIndex()
{
}
