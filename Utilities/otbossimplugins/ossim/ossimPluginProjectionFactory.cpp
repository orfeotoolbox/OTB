//----------------------------------------------------------------------------
//
// "Copyright Centre National d'Etudes Spatiales"
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------
// $Id$


#include "ossimPluginProjectionFactory.h"
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/projection/ossimProjection.h>
#include "ossimRadarSatModel.h"
#include "ossimEnvisatAsarModel.h"
#include "ossimTerraSarModel.h"
//#include <ossim/projection/ossimCosmoSkymedModel.h>
#include "ossimRadarSat2Model.h"
#include "ossimErsSarModel.h"
#include "ossimAlosPalsarModel.h"

namespace ossimplugins
{


ossimPluginProjectionFactory* ossimPluginProjectionFactory::instance()
{
   static ossimPluginProjectionFactory* factoryInstance =
      new ossimPluginProjectionFactory();

   return factoryInstance;
}

ossimProjection* ossimPluginProjectionFactory::createProjection(
   const ossimFilename& filename, ossim_uint32 /*entryIdx*/)const
{
  ossimRefPtr<ossimProjection> result = 0;

   if ( !result )
   {
      ossimRefPtr<ossimRadarSat2Model> model = new ossimRadarSat2Model();
      if ( model->open(filename) )
      {
	       result = model.get();
      }
      else
      {
         model = 0;
      }
   }

   if ( !result )
   {
      ossimRefPtr<ossimTerraSarModel> model = new ossimTerraSarModel();

     if ( model->open(filename) )
      {
         result = model.get();
      }
      else
      {
         model = 0;
      }
   }

   if ( !result )
   {
      ossimRefPtr<ossimErsSarModel> model = new ossimErsSarModel();
      if ( model->open(filename) )
      {
         result = model.get();
      }
      else
      {
         model = 0;
      }
   }

   if (!result)
   {
     ossimRefPtr<ossimEnvisatAsarModel> model = new ossimEnvisatAsarModel();
     if (model->open(filename))
     {
       result = model.get();
     }
     else
     {
       model = 0;
     }
   }

   if (!result)
   {
     ossimRefPtr<ossimRadarSatModel> model = new ossimRadarSatModel();
     if (model->open(filename))
     {
       result = model.get();
     }
     else
     {
       model = 0;
     }
   }

   if (!result)
   {
     ossimRefPtr<ossimAlosPalsarModel> model = new ossimAlosPalsarModel();
     if (model->open(filename))
     {
       result = model.get();
     }
     else
     {
       model = 0;
     }
   }

   return result.release();
}

ossimProjection* ossimPluginProjectionFactory::createProjection(
   const ossimString& name)const
{
   //   else if (name == STATIC_TYPE_NAME(ossimCosmoSkymedModel))
   //    {
   //      return new ossimCosmoSkymedModel;
   //   }
   if (name == STATIC_TYPE_NAME(ossimRadarSat2Model))
   {
      return new ossimRadarSat2Model();
   }
   else if (name == STATIC_TYPE_NAME(ossimTerraSarModel))
   {
      return new ossimTerraSarModel();
   }
   else if (name == STATIC_TYPE_NAME(ossimErsSarModel))
   {
     return new ossimErsSarModel;
   }
   else if (name == STATIC_TYPE_NAME(ossimEnvisatAsarModel))
   {
     return new ossimEnvisatAsarModel;
   }
   else if (name == STATIC_TYPE_NAME(ossimRadarSatModel))
   {
     return new ossimRadarSatModel;
   }
   else if (name == STATIC_TYPE_NAME(ossimAlosPalsarModel))
   {
     return new ossimRadarSatModel;
   }
   return 0;
}

ossimProjection* ossimPluginProjectionFactory::createProjection(
   const ossimKeywordlist& kwl, const char* prefix)const
{
   ossimRefPtr<ossimProjection> result = 0;

   const char* lookup = kwl.find(prefix, ossimKeywordNames::TYPE_KW);
   if (lookup)
   {
      ossimString type = lookup;

      if (type == "ossimRadarSat2Model")
      {
         result = new ossimRadarSat2Model();
         if ( !result->loadState(kwl, prefix) )
         {
            result = 0;
         }
      }
      else if (type == "ossimTerraSarModel")
      {
         result = new ossimTerraSarModel();
         if ( !result->loadState(kwl, prefix) )
         {
            result = 0;
         }
      }
      else if (type == "ossimErsSarModel")
      {
         result = new ossimErsSarModel();
         if ( !result->loadState(kwl, prefix) )
         {
            result = 0;
         }
      }
      else if (type == "ossimEnvisatAsarModel")
      {
         result = new ossimEnvisatAsarModel();
         if ( !result->loadState(kwl, prefix) )
         {
            result = 0;
         }
      }
      else if (type == "ossimRadarSatModel")
      {
         result = new ossimRadarSatModel();
         if ( !result->loadState(kwl, prefix) )
         {
            result = 0;
         }
      }
      else if (type == "ossimAlosPalsarModel")
      {
         result = new ossimAlosPalsarModel();
         if ( !result->loadState(kwl, prefix) )
         {
            result = 0;
         }
      }
   }

   return result.release();
}

ossimObject* ossimPluginProjectionFactory::createObject(
   const ossimString& typeName)const
{
   return createProjection(typeName);
}

ossimObject* ossimPluginProjectionFactory::createObject(
   const ossimKeywordlist& kwl, const char* prefix)const
{
   return createProjection(kwl, prefix);
}


void ossimPluginProjectionFactory::getTypeNameList(std::vector<ossimString>& typeList)const
{
   typeList.push_back(STATIC_TYPE_NAME(ossimRadarSatModel));
   typeList.push_back(STATIC_TYPE_NAME(ossimRadarSat2Model));
   typeList.push_back(STATIC_TYPE_NAME(ossimTerraSarModel));
   //   result.push_back(STATIC_TYPE_NAME(ossimCosmoSkymedModel));
   typeList.push_back(STATIC_TYPE_NAME(ossimEnvisatAsarModel));
   typeList.push_back(STATIC_TYPE_NAME(ossimErsSarModel));
   typeList.push_back(STATIC_TYPE_NAME(ossimAlosPalsarModel));
}
}
