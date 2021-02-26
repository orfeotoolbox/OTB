/*
 * Copyright (C) 2005-2019 by Centre National d'Etudes Spatiales (CNES)
 *
 * This file is licensed under MIT license:
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


//***
// Note to programmer: To add a new model, search this file for "ADD_MODEL"
// to locate places requiring editing. Functional example below...
//
// ADD_MODEL: Include all sensor model headers here:
//***
#include "ossimPluginProjectionFactory.h"
#include "ossimRadarSatModel.h"
#include "ossimEnvisatAsarModel.h"
#include "ossimTerraSarXSarSensorModel.h"
#include "ossimRadarSat2Model.h"
#include "ossimErsSarModel.h"
#include "ossimAlosPalsarModel.h"
#include "ossimPleiadesModel.h"
#include "ossimTileMapModel.h"
#include "ossimSpot6Model.h"
#include "ossimSentinel1Model.h"
#include "ossimCosmoSkymedModel.h"

#include "ossim/ossimStringUtilities.h"
#include "ossim/ossimTraceHelpers.h"

#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/projection/ossimProjection.h>
//***
// Define Trace flags for use within this file:
//***
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/base/ossimTrace.h>
static ossimTrace traceExec  = ossimTrace("ossimPluginProjectionFactory:exec");
static ossimTrace traceDebug = ossimTrace("ossimPluginProjectionFactory:debug");


#include <ossimFormosatModel.h>
#include <ossimFormosatDimapSupportData.h>

#ifndef _WIN32
#include <limits.h> //PATH_MAX
#include <stdlib.h> //realpath
 #include <errno.h>
#endif
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
   const ossimFilename& file_name, ossim_uint32 /*entryIdx*/)const
{
   static const char MODULE[] = "ossimPluginProjectionFactory::createProjection(ossimFilename& filename)";
   ossimRefPtr<ossimProjection> projection = 0;
   //traceDebug.setTraceFlag(true);

#ifndef _WIN32
   char real_path[PATH_MAX + 1];
   char *ret_path = NULL;
   ret_path = realpath(file_name, real_path);
   if( ret_path == NULL)
   {
      if(traceExec())
      {
         ossimNotify(ossimNotifyLevel_FATAL)
            << MODULE
            << " error calling realpath(" << file_name << "). errno "
            << strerror(errno) << std::endl;
      }

      return NULL;
   }
   const ossimFilename abs_file_name( real_path );
#else
   //GetFullPathName function is not thread safe. better use absolute path on windows
   //https://msdn.microsoft.com/en-us/library/windows/desktop/aa364963%28v=vs.85%29.aspx
   const ossimFilename abs_file_name( file_name );
#endif

   // TODO: use a type-list to simplify this chain factory.

     // Cosmo
   if ( !projection )
   {
     projection = doBuildProjection<ossimCosmoSkymedModel>(abs_file_name);
   }

   // Sentinel1
   if ( !projection )
   {
      projection = doBuildProjection<ossimSentinel1Model>(abs_file_name);
   }

   if ( !projection )
   {
#if 1
      projection = doBuildProjection<ossimRadarSat2Model>(abs_file_name);
#else
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << MODULE << " DEBUG: testing ossimRadarSat2Model" << std::endl;
      }

      ossimRefPtr<ossimRadarSat2Model> model = new ossimRadarSat2Model();
      if ( model->open(abs_file_name) )
      {
         // Check if a coarse grid was generated, and use it instead:
         projection = model->getReplacementOcgModel().get();
         if (projection.valid())
            model = 0; // Have OCG, don't need this one anymore
         else
            projection = model.get();
      }
      else
      {
         model = 0;
      }
#endif
   }

   // Pleiades
   if ( !projection )
   {
      projection = doBuildProjection<ossimPleiadesModel>(abs_file_name);
   }

   if ( !projection )
   {
#if 0
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << MODULE << " DEBUG: testing ossimTerraSarXSarSensorModel" << std::endl;
      }

      ossimRefPtr<ossimTerraSarXSarSensorModel> model = new ossimTerraSarXSarSensorModel();

      if ( model->open(abs_file_name) )
      {
         // Check if a coarse grid was generated, and use it instead:
         projection = model->getReplacementOcgModel().get();
         if (projection.valid())
            model = 0; // Have OCG, don't need this one anymore
         else
            projection = model.get();
      }
      else
      {
         model = 0;
      }
#else
      projection = doBuildProjection<ossimTerraSarXSarSensorModel>(abs_file_name);
#endif
   }

   // ErsSar
   if ( !projection )
   {
      projection = doBuildProjection<ossimErsSarModel>(abs_file_name);
   }

   if (!projection)
   {
      projection = doBuildProjection<ossimEnvisatAsarModel>(abs_file_name);
   }

   if (!projection)
   {
      projection = doBuildProjection<ossimRadarSatModel>(abs_file_name);
   }

   if (!projection)
   {
      projection = doBuildProjection<ossimAlosPalsarModel>(abs_file_name);
   }

   if (!projection)
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << MODULE << " DEBUG: testing ossimFormosatModel" << std::endl;
      }

      ossimFilename formosatTest = abs_file_name;
      formosatTest = formosatTest.setExtension("geom");
      if(!formosatTest.exists())
      {
         formosatTest = abs_file_name.path();
         formosatTest = formosatTest.dirCat(ossimFilename("METADATA.DIM"));
         if (formosatTest.exists() == false)
         {
            formosatTest = abs_file_name.path();
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
      projection = doBuildProjection<ossimTileMapModel>(abs_file_name);
   }

   // Spot6
   if ( !projection )
   {
      projection = doBuildProjection<ossimSpot6Model>(abs_file_name);
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


   // TOTO: use type list/map
   if (name == STATIC_TYPE_NAME(ossimRadarSat2Model))
   {
      return new ossimRadarSat2Model();
   }
   else if (name == STATIC_TYPE_NAME(ossimTerraSarXSarSensorModel))
   {
      return new ossimTerraSarXSarSensorModel();
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
   else if (name == STATIC_TYPE_NAME(ossimCosmoSkymedModel))
   {
     return new ossimCosmoSkymedModel;
   }
   else if(name == STATIC_TYPE_NAME(ossimSarSensorModel))
   {
     return new ossimSarSensorModel;
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
      else if (type == "ossimCosmoSkymedModel")
      {
         result = new ossimCosmoSkymedModel();
      }

      else if (type == "ossimSarSensorModel")
      {
         result = new ossimSarSensorModel();
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
   typeList.push_back(STATIC_TYPE_NAME(ossimTerraSarXSarSensorModel));
   typeList.push_back(STATIC_TYPE_NAME(ossimEnvisatAsarModel));
   typeList.push_back(STATIC_TYPE_NAME(ossimErsSarModel));
   typeList.push_back(STATIC_TYPE_NAME(ossimAlosPalsarModel));
   typeList.push_back(STATIC_TYPE_NAME(ossimFormosatModel));
   typeList.push_back(STATIC_TYPE_NAME(ossimTileMapModel));
   typeList.push_back(STATIC_TYPE_NAME(ossimPleiadesModel));
   typeList.push_back(STATIC_TYPE_NAME(ossimSentinel1Model));
   typeList.push_back(STATIC_TYPE_NAME(ossimCosmoSkymedModel));
   typeList.push_back(STATIC_TYPE_NAME(ossimSarSensorModel));
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


} // ossimplugins namespace
