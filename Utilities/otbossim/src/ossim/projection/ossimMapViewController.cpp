//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks)
//
//*************************************************************************
// $Id: ossimMapViewController.cpp 15766 2009-10-20 12:37:09Z gpotts $
#include <ossim/projection/ossimMapViewController.h>
#include <ossim/projection/ossimEquDistCylProjection.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimNotifyContext.h>
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
      
      ossimRefPtr<ossimProjection> proj = ossimProjectionFactoryRegistry::instance()->createProjection(kwl);
      if(dynamic_cast<ossimMapProjection*>(proj.get()))
      {
         setView(proj.get());
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
   ossimRefPtr<ossimProjection> proj;
   
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
   
   if(dynamic_cast<ossimMapProjection*>(proj.get()))
   {
      setView(proj.get());
   }

   return true;
}
