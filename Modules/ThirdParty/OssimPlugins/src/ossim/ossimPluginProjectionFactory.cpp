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

//***
// Note to programmer: To add a new model, search this file for "ADD_MODEL"
// to locate places requiring editing. Functional example below...
//
// ADD_MODEL: Include all sensor model headers here:
//***
#include "ossimPluginProjectionFactory.h"
#include "ossimRadarSatModel.h"
#include "ossimEnvisatAsarModel.h"
#include "ossimTerraSarModel.h"
#include "ossimTerraSarXSarSensorModel.h"
#include "ossimRadarSat2Model.h"
#include "ossimErsSarModel.h"
#include "ossimAlosPalsarModel.h"
#include "ossimPleiadesModel.h"
#include "ossimTileMapModel.h"
#include "ossimSpot6Model.h"
#include "ossimSentinel1Model.h"
#include "ossimStringUtilities.h"
#include "ossimTraceHelpers.h"
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/projection/ossimProjection.h>
//#include <ossim/projection/ossimCosmoSkymedModel.h>
//***
// Define Trace flags for use within this file:
//***
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/base/ossimTrace.h>
static ossimTrace traceExec  = ossimTrace("ossimPluginProjectionFactory:exec");
static ossimTrace traceDebug = ossimTrace("ossimPluginProjectionFactory:debug");


#include <ossimFormosatModel.h>
#include <ossimFormosatDimapSupportData.h>

namespace ossimplugins
{
   bool ossimPluginProjectionFactory::initialized_;

   ossimPluginProjectionFactory ossimPluginProjectionFactory::factoryInstance;

   ossimPluginProjectionFactory* ossimPluginProjectionFactory::instance()
   {
      return initialized_ ? &factoryInstance : 0;
   }

   template <typename ProjectionType>
      inline
      ossimRefPtr<ossimProjection> doUpcastModelToProjection(ossimRefPtr<ProjectionType> model)
      {
         return model.get();
      }

   template <typename ProjectionType>
      inline
      ossimRefPtr<ossimProjection> doUpcastModelToProjectionWhenOCG(ossimRefPtr<ProjectionType> model)
      {
         ossimRefPtr<ossimProjection> projection = model->getReplacementOcgModel().get();
#if 0
         if (projection.valid())
            model = 0; // Have OCG, don't need this one anymore
         else
            projection = model.get();
#else
         if (!projection) {
            projection = model.get();
         }
#endif
         return projection;
      }

   template <> inline
      ossimRefPtr<ossimProjection> doUpcastModelToProjection<ossimRadarSat2Model>(ossimRefPtr<ossimRadarSat2Model> model)
      {
         return doUpcastModelToProjectionWhenOCG(model);
      }
#if 1
   template <> inline
      ossimRefPtr<ossimProjection> doUpcastModelToProjection<ossimTerraSarModel>(ossimRefPtr<ossimTerraSarModel> model)
      {
         return doUpcastModelToProjectionWhenOCG(model);
      }
#endif


   template <typename ProjectionType>
      inline
      ossimRefPtr<ossimProjection> doBuildProjection(ossimFilename const& filename)
      {
         static const char MODULE[] = "ossimPluginProjectionFactory::createProjection(ossimFilename& filename)";
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << MODULE << " DEBUG: testing " << STATIC_TYPE_NAME(ProjectionType) << "\n";
         }

         ossimRefPtr<ProjectionType> model = new ProjectionType();
         if ( model->open(filename) )
         {
            return model.get();
         }
         else
         {
            return 0;
         }
      }

ossimProjection* ossimPluginProjectionFactory::createProjection(
   const ossimFilename& filename, ossim_uint32 /*entryIdx*/)const
{
   static const char MODULE[] = "ossimPluginProjectionFactory::createProjection(ossimFilename& filename)";
   SCOPED_LOG(traceDebug, MODULE);
   ossimRefPtr<ossimProjection> projection = 0;

   // TODO: use a type-list to simplify this chain factory.

   // Sentinel1
   if ( !projection )
   {
      projection = doBuildProjection<ossimSentinel1Model>(filename);
   }

   if ( !projection )
   {
      projection = doBuildProjection<ossimRadarSat2Model>(filename);
   }

   // Pleiades
   if ( !projection )
   {
      projection = doBuildProjection<ossimPleiadesModel>(filename);
   }

   // Terra-SAR-X
#if 1
   if ( !projection )
   {
      projection = doBuildProjection<ossimTerraSarXSarSensorModel>(filename);
   }
#endif

#if 0
   // old Terra-SAR-X
   if ( !projection )
   {
      projection = doBuildProjection<ossimTerraSarModel>(filename);
   }
#endif
   // ErsSar
   if ( !projection )
   {
      projection = doBuildProjection<ossimErsSarModel>(filename);
   }

   if (!projection)
   {
      projection = doBuildProjection<ossimEnvisatAsarModel>(filename);
   }

   if (!projection)
   {
      projection = doBuildProjection<ossimRadarSatModel>(filename);
   }

   if (!projection)
   {
      projection = doBuildProjection<ossimAlosPalsarModel>(filename);
   }

   if (!projection)
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << MODULE << " DEBUG: testing ossimFormosatModel" << std::endl;
      }

      ossimFilename formosatTest = filename;
      formosatTest = formosatTest.setExtension("geom");
      if(!formosatTest.exists())
      {
         formosatTest = filename.path();
         formosatTest = formosatTest.dirCat(ossimFilename("METADATA.DIM"));
         if (formosatTest.exists() == false)
         {
            formosatTest = filename.path();
            formosatTest = formosatTest.dirCat(ossimFilename("metadata.dim"));
         }
      }
      if(formosatTest.exists())
      {
         ossimRefPtr<ossimFormosatDimapSupportData> meta =
            new ossimFormosatDimapSupportData;
         if(meta->loadXmlFile(formosatTest))
         {
            ossimRefPtr<ossimFormosatModel> model = new ossimFormosatModel(meta.get());
            if(!model->getErrorStatus())
            {
               projection = model.get();
            }
            model = 0;
         }
      }
   }

   if (!projection)
   {
      projection = doBuildProjection<ossimTileMapModel>(filename);
   }

   // Spot6
   if ( !projection )
   {
      projection = doBuildProjection<ossimSpot6Model>(filename);
   }


   //***
   // ADD_MODEL: (Please leave this comment for the next programmer)
   // Or, use doBuildProjection<MY_NEW_MODEL>(filename);
   //***
   //if(traceDebug())
   //{
    //  ossimNotify(ossimNotifyLevel_DEBUG)
     //        << MODULE << " DEBUG: testing MY_NEW_MODEL" << std::endl;
   //}

   // MY_NEW_MODEL
   //if ( !projection )
   //{
    //  ossimRefPtr<MY_NEW_MODEL> model = new MY_NEW_MODEL();
     // if ( model->open(filename) )
      //{
       //  projection = model.get();
      //}
   //   else
   //   {
  //       model = 0;
  //    }
   //}

   return projection.release();
}

ossimProjection* ossimPluginProjectionFactory::createProjection(
   const ossimString& name)const
{
   static const char MODULE[] = "ossimPluginProjectionFactory::createProjection(ossimString& name)";

   if(traceDebug())
   {
        ossimNotify(ossimNotifyLevel_DEBUG)
                   << MODULE << " DEBUG: Entering ...." << std::endl;
   }

   // else if (name == STATIC_TYPE_NAME(ossimCosmoSkymedModel))
   // {
   //    return new ossimCosmoSkymedModel;
   // }
   // TOTO: use type list/map
   if (name == STATIC_TYPE_NAME(ossimRadarSat2Model))
   {
      return new ossimRadarSat2Model();
   }
   else if (name == STATIC_TYPE_NAME(ossimTerraSarXSarSensorModel))
   {
      return new ossimTerraSarXSarSensorModel();
   }
#if 0
   else if (name == STATIC_TYPE_NAME(ossimTerraSarModel))
   {
      return new ossimTerraSarModel();
   }
#endif
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
     return new ossimAlosPalsarModel;
   }
   else if (name == STATIC_TYPE_NAME(ossimFormosatModel))
   {
     return new ossimFormosatModel;
   }
   else if (name == STATIC_TYPE_NAME(ossimTileMapModel))
   {
     return new ossimTileMapModel;
   }
   else if (name == STATIC_TYPE_NAME(ossimPleiadesModel))
   {
     return new ossimPleiadesModel;
   }
   else if (name == STATIC_TYPE_NAME(ossimSpot6Model))
   {
     return new ossimSpot6Model;
   }
   else if (name == STATIC_TYPE_NAME(ossimSentinel1Model))
   {
     return new ossimSentinel1Model;
   }

   //***
   // ADD_MODEL: (Please leave this comment for the next programmer)
   //***
//   if(name == MY_NEW_MODEL)
//      return new myNewModel;

   if(traceDebug())
   {
        ossimNotify(ossimNotifyLevel_DEBUG)
                   << MODULE << " DEBUG: Leaving ...." << std::endl;
   }

   return 0;
}

ossimProjection* ossimPluginProjectionFactory::createProjection(
   const ossimKeywordlist& kwl, const char* prefix)const
{
   ossimRefPtr<ossimProjection> result = 0;
   static const char MODULE[] = "ossimPluginProjectionFactory::createProjection(ossimKeywordlist& kwl)";

   SCOPED_LOG(traceDebug, MODULE);

   const char* lookup = kwl.find(prefix, ossimKeywordNames::TYPE_KW);
   if (lookup)
   {
      const std::string type = lookup;

      if (type == "ossimRadarSat2Model")
      {
         result = new ossimRadarSat2Model();
      }
      else if (type == "ossimTerraSarXSarSensorModel")
      {
         result = new ossimTerraSarXSarSensorModel();
      }
#if 0
      else if (type == "ossimTerraSarModel")
      {
         result = new ossimTerraSarModel();
      }
#endif
      else if (type == "ossimErsSarModel")
      {
         result = new ossimErsSarModel();
      }
      else if (type == "ossimEnvisatAsarModel")
      {
         result = new ossimEnvisatAsarModel();
      }
      else if (type == "ossimRadarSatModel")
      {
         result = new ossimRadarSatModel();
      }
      else if (type == "ossimAlosPalsarModel")
      {
         result = new ossimAlosPalsarModel();
      }
      else if (type == "ossimFormosatModel")
      {
         result = new ossimFormosatModel();
      }
      else if (type == "ossimTileMapModel")
      {
         result = new ossimTileMapModel();
      }
      else if (type == "ossimPleiadesModel")
      {
         result = new ossimPleiadesModel();
      }
      else if (type == "ossimSpot6Model")
      {
         result = new ossimSpot6Model();
      }
      else if (type == "ossimSentinel1Model")
      {
         result = new ossimSentinel1Model();
      }

   //***
   // ADD_MODEL: (Please leave this comment for the next programmer)
   //***
//      else if (type == "ossimSpot6Model")
//      {
//         result = new ossimSpot6Model();
//      }

      // Then, try to load the keyword list
      if ( result.get() && !result->loadState(kwl, prefix) )
      {
         result = 0;
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
#if 0
   typeList.push_back(STATIC_TYPE_NAME(ossimTerraSarModel));
#endif
   typeList.push_back(STATIC_TYPE_NAME(ossimTerraSarXSarSensorModel));
//   typeList.push_back(STATIC_TYPE_NAME(ossimCosmoSkymedModel));
   typeList.push_back(STATIC_TYPE_NAME(ossimEnvisatAsarModel));
   typeList.push_back(STATIC_TYPE_NAME(ossimErsSarModel));
   typeList.push_back(STATIC_TYPE_NAME(ossimAlosPalsarModel));
   typeList.push_back(STATIC_TYPE_NAME(ossimFormosatModel));
   typeList.push_back(STATIC_TYPE_NAME(ossimTileMapModel));
   typeList.push_back(STATIC_TYPE_NAME(ossimPleiadesModel));
   typeList.push_back(STATIC_TYPE_NAME(ossimSentinel1Model));
   typeList.push_back(STATIC_TYPE_NAME(ossimSpot6Model));

   //***
   // ADD_MODEL: Please leave this comment for the next programmer. Add above.
   //***
   //typeList.push_back(STATIC_TYPE_NAME(MY_NEW_MODEL));
}

bool ossimPluginProjectionFactory::isTileMap(ossimFilename filename)const
{
  filename.downcase();

  // Incorrect inheritance, some string services have been lost
  std::string const& sFilename = filename;

  const bool res
     =  ends_with(sFilename, ".otb")
     || starts_with(sFilename, "http")
     ;
  return res;
}

ossimPluginProjectionFactory::ossimPluginProjectionFactory()
{
   initialized_ = true;
   // traceDebug.setTraceFlag(true);
   // traceExec .setTraceFlag(true);
}



} // ossimplugins namespace

