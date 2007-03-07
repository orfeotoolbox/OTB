//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License 
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software 
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author: Garrett Potts (gpotts@imagelinks)
//
//*************************************************************************
// $Id: ossimMapViewController.cpp,v 1.21 2004/12/20 15:07:25 gpotts Exp $
#include <projections/ossimMapViewController.h>
#include <projections/map_projections/ossimEquDistCylProjection.h>
#include <projections/factory/ossimProjectionFactoryRegistry.h>
#include <base/common/ossimKeywordNames.h>
#include <base/context/ossimNotifyContext.h>
#include <iostream>
#include <fstream>
using namespace std;

RTTI_DEF1(ossimMapViewController, "ossimMapViewController", ossimViewController);

ossimMapViewController::ossimMapViewController()
   :ossimViewController()
{
}

ossimMapViewController::ossimMapViewController(ossimObject* owner,
                                               ossim_uint32 inputListSize,
                                               ossim_uint32 outputListSize,
                                               bool inputListFixedFlag,
                                               bool outputListFixedFlag)
   :ossimViewController(owner,
                        inputListSize,
                        outputListSize,
                        inputListFixedFlag,
                        outputListFixedFlag)
{
   if(theOwner)
   {
      const ossimObject* obj = findFirstViewOfType(STATIC_TYPE_INFO(ossimMapProjection));
      ossimMapProjection* proj = PTR_CAST(ossimMapProjection, obj);
      
      if(proj)
      {
         // we are initialized to the first view.
         theView = proj->dup();
      }
      else
      {
         // initialize the controller to true geographic
         theView = new ossimEquDistCylProjection;
      }
   }
   else
   {
      // initialize the controller to true geographic
      theView = new ossimEquDistCylProjection;
   }
}

ossimMapViewController::~ossimMapViewController()
{
}

bool ossimMapViewController::setView(ossimObject* object)
{
   if(PTR_CAST(ossimMapProjection, object))
   {
      return ossimViewController::setView(object);
   }
   
   return false;
}

void ossimMapViewController::changeOwner(ossimObject* owner)
{
   ossimConnectableObject::changeOwner(owner);
   
   if(!theView)
   {
      // initialize the controller to geographic
      const ossimObject* obj   = findFirstViewOfType(STATIC_TYPE_INFO(ossimMapProjection));
      ossimMapProjection* proj = PTR_CAST(ossimMapProjection, obj);
      
      if(proj)
      {
         // we are initialized to the first view.
         setView(proj->dup());
         propagateView();
      }
      else
      {
         // initialize the controller to true geographic
         setView(new ossimEquDistCylProjection);
         propagateView();
      }
   }
   else
   {
      propagateView();
   }   
}

void ossimMapViewController::setFilename(const ossimFilename& file)
{
   if(file.exists())
   {
      ossimKeywordlist kwl;
      kwl.addFile(file.c_str());
      
      ossimProjection* proj = ossimProjectionFactoryRegistry::instance()->createProjection(kwl);
      if(PTR_CAST(ossimMapProjection, proj))
      {
         setView(proj);
      }
      else
      {
         if(proj)
         {
            delete proj;
         }
      }
   }

   theGeometryFile = file;
}


bool ossimMapViewController::saveState(ossimKeywordlist& kwl,
                                       const char* prefix)const
{
   ossimSource::saveState(kwl, prefix);
   ossimMapProjection* proj = PTR_CAST(ossimMapProjection, getView());

   ossimString newPrefix = prefix;
   newPrefix += "projection.";
   
   if(proj)
   {
      if(theGeometryFile == "")
      {
         proj->saveState(kwl, newPrefix.c_str());
      }
      else
      {
         ofstream output(theGeometryFile.c_str());
         if(output)
         {
            kwl.add(prefix,
                    ossimKeywordNames::FILENAME_KW,
                    theGeometryFile);
            ossimKeywordlist kwl2;
            proj->saveState(kwl2);
            kwl2.writeToStream(output);
         }
         else
         {
            ossimNotify(ossimNotifyLevel_WARN) << "WARNING ossimMapViewController::saveState can't open filename " << theGeometryFile
                                               << "\nsaving inline to passed in keywordlist" << std::endl;
            proj->saveState(kwl, newPrefix);
         }
      }
   }
   
   return true;
}

bool ossimMapViewController::loadState(const ossimKeywordlist& kwl,
                                       const char* prefix)
{
   ossimSource::loadState(kwl, prefix);

   theGeometryFile = kwl.find(prefix, ossimKeywordNames::FILENAME_KW);
   ossimProjection* proj=(ossimProjection*)NULL;
   
   if(theGeometryFile == "")
   {
      ossimString newPrefix = prefix;
      newPrefix += "projection.";
      proj = ossimProjectionFactoryRegistry::instance()->createProjection(kwl, newPrefix.c_str());
   }
   else
   {
      ossimKeywordlist kwl2;
      kwl2.addFile(theGeometryFile);

      proj = ossimProjectionFactoryRegistry::instance()->createProjection(kwl2);
   }
   
   if(PTR_CAST(ossimMapProjection, proj))
   {
      setView(proj);
   }
   else
   {
      if(proj)
      {
         delete proj;
      }      
   }

   return true;
}
