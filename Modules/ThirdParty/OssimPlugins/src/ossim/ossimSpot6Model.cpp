//*******************************************************************
// Copyright (C) 2014  Centre National Etudes Spatiales
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
//
// Author : Christophe Palmann (christophe.palmann@c-s.fr)
//
// Description:
//
// Contains definition of class ossimSpot6Model
//
//*****************************************************************************

#include "ossimSpot6Model.h"

#include <cmath>
#include <cstdio>

#include <ossimSpot6Model.h>
#include <ossimSpot6DimapSupportData.h>

#include <ossimPluginCommon.h>

#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimXmlDocument.h>
#include <ossim/base/ossimXmlNode.h>
#include <ossim/support_data/ossimSupportFilesList.h>


namespace ossimplugins
{

// Define Trace flags for use within this file:
   static ossimTrace traceExec  ("ossimSpot6Model:exec");
   static ossimTrace traceDebug ("ossimSpot6Model:debug");


   RTTI_DEF1(ossimSpot6Model, "ossimSpot6Model", ossimRpcModel);

//*************************************************************************************************
// Constructor
//*************************************************************************************************
   ossimSpot6Model::ossimSpot6Model()
      :ossimRpcModel      (),
       theSupportData        (0)
   {
      for (unsigned int i = 0; i < 20; i++)
      {
         theLineDenCoef[i] = 0.0;
         theLineNumCoef[i] = 0.0;
         theSampNumCoef[i] = 0.0;
         theSampDenCoef[i] = 0.0;
      }
   }

//*************************************************************************************************
// Constructor
//*************************************************************************************************
   ossimSpot6Model::ossimSpot6Model(const ossimSpot6Model& rhs)
      :ossimRpcModel      (rhs),
       theSupportData        (0)
   {
   }

//*************************************************************************************************
// Destructor
//*************************************************************************************************
   ossimSpot6Model::~ossimSpot6Model()
   {
      if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG DESTRUCTOR: ~ossimSpot6Model(): entering..." << std::endl;

      theSupportData = 0;

      if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG DESTRUCTOR: ~ossimSpot6Model(): returning..." << std::endl;
   }
//*************************************************************************************************
// Infamous DUP
//*************************************************************************************************
   ossimObject* ossimSpot6Model::dup() const
   {
      return new ossimSpot6Model(*this);
   }

//*************************************************************************************************
// Print
//*************************************************************************************************
   std::ostream& ossimSpot6Model::print(std::ostream& out) const
   {
      // Capture stream flags since we are going to mess with them.
      std::ios_base::fmtflags f = out.flags();

      out << "\nDump of ossimSpot6Model at address " << (hex) << this
          << (dec)
          << "\n------------------------------------------------"
          << "\n  theImageID            = " << theImageID
          << "\n  theImageSize          = " << theImageSize
          << "\n  theRefGndPt           = " << theRefGndPt
          << "\n  theRefImgPt           = " << theRefImgPt
          << "\n  theProcessingLevel    = " << theSupportData->getProcessingLevel()
          << "\n------------------------------------------------"
          << "\n  " << endl;

      // Set the flags back.
      out.flags(f);

      if (theSupportData->getProcessingLevel() == "SENSOR")
         return ossimRpcModel::print(out);
      else
         return out;
   }

//*************************************************************************************************
// Save State
//*************************************************************************************************
   bool ossimSpot6Model::saveState(ossimKeywordlist& kwl,
                                      const char* prefix) const
   {
      if(theSupportData.valid())
      {
         ossimString supportPrefix = ossimString(prefix) + "support_data.";
         theSupportData->saveState(kwl, supportPrefix);
      }

      // If only it is a sensor product we save parameters from RPC model, its avoid to
      // propagate a empty RPC model
      if (theSupportData->getProcessingLevel() == "SENSOR")
      {
         ossimRpcModel::saveState(kwl, prefix);
         return true;
      }
      else
      {
         kwl.add(prefix, "sensor", theSensorID, true);
         return true;
      }
   }

//*************************************************************************************************
// Load State
//*************************************************************************************************
   bool ossimSpot6Model::loadState(const ossimKeywordlist& kwl,
                                      const char* prefix)
   {
      if(!theSupportData)
      {
         theSupportData = new ossimSpot6DimapSupportData;
      }

      ossimString supportPrefix = ossimString(prefix) + "support_data.";
      theSupportData->loadState(kwl, supportPrefix);

      // If only it is a sensor product we load parameters from RPC model only, its avoid to
      // add a empty RPC model
      if (theSupportData->getProcessingLevel() == "SENSOR")
      {
         ossimRpcModel::loadState(kwl, prefix);
         return true;
      }
      else
      {
         return true;
      }
   }



   bool
   ossimSpot6Model::open(const ossimFilename& file)
   {
      static const char MODULE[] = "ossimSpot6Model::open";
      //traceDebug.setTraceFlag(true);

      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " entered...\n";
      }

      // Make the gsd nan so it gets computed.
      theGSD.makeNan();

      bool result = false;

      // Filename used.
      ossimFilename DIMxmlFile;
      ossimFilename RPCxmlFile;

      // Generate metadata and rpc filename
      if ( (file.ext().downcase() != "jp2" && file.ext().downcase() != "tif")
          || !file.exists())
      {
         //not a valid file
         return false;
      }
      else
      {
        // DIMAPv2
        DIMxmlFile = file.path();
        RPCxmlFile = file.path();
        ossimFilename DIMxmlFileTmp = file.file();
        ossimFilename RPCxmlFileTmp;

        DIMxmlFileTmp = DIMxmlFileTmp.file().replaceStrThatMatch("^IMG_", "DIM_");
        DIMxmlFileTmp = DIMxmlFileTmp.replaceStrThatMatch("_R[0-9]+C[0-9]+\\.(JP2|TIF)$", ".XML");
        // Check if it is an XML extension
        if( DIMxmlFileTmp.ext() != "XML")
          return false;

        RPCxmlFileTmp = DIMxmlFileTmp.file().replaceStrThatMatch("^DIM_", "RPC_");

        DIMxmlFile = DIMxmlFile.dirCat(DIMxmlFileTmp);
        RPCxmlFile = RPCxmlFile.dirCat(RPCxmlFileTmp);

        if (!DIMxmlFile.exists())
        {
          if (traceDebug())
          {
            ossimNotify(ossimNotifyLevel_DEBUG) << "SPOT main DIMAP file " << DIMxmlFile << " doesn't exist ...\n";
          }
          return false;
        }
      }

      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG) << "Metadata xml file: " << DIMxmlFile << "\n";
         ossimNotify(ossimNotifyLevel_DEBUG) << "RPC xml file: " << RPCxmlFile << "\n";
      }

      ossimString processingLevel;
      // Parse the metadata xml file
      if ( !theSupportData.valid() )
         theSupportData = new ossimSpot6DimapSupportData();

      if(!theSupportData->parseXmlFile(DIMxmlFile))
      {
         theSupportData = 0; // ossimRefPtr
         if (traceDebug())
         {
           ossimNotify(ossimNotifyLevel_DEBUG) << "ossimSpot6Model::open DEBUG:"
                                               << "\nCould not open correctly DIMAP file" << std::endl;
         }
         return false;
      }

      theSensorID = theSupportData->getSensorID();
      theImageID = theSupportData->getImageID();
      // Get the processing level (ORTHO or SENSOR or perhaps MOSAIC ?)
      processingLevel = theSupportData->getProcessingLevel();

      // Parse the RPC xml file if necessary
      if (RPCxmlFile.exists() && processingLevel == "SENSOR")
      {
         if (!theSupportData->parseXmlFile(RPCxmlFile))
         {
            theSupportData = 0; // ossimRefPtr
            ossimNotify(ossimNotifyLevel_WARN) << "ossimSpot6Model::open WARNING:"
                                               << "\nCould not open correctly RPC file" << std::endl;
            return false;
         }

         thePolyType = B;

         for (unsigned int i = 0 ; i < 20; i++ )
         {
            theLineNumCoef[i] = theSupportData->getLineNumCoeff()[i];
            theLineDenCoef[i] = theSupportData->getLineDenCoeff()[i];
            theSampNumCoef[i] = theSupportData->getSampNumCoeff()[i];
            theSampDenCoef[i] = theSupportData->getSampDenCoeff()[i];
         }

         theLineScale  = theSupportData->getLineScale();
         theSampScale  = theSupportData->getSampScale();
         theLatScale   = theSupportData->getLatScale();
         theLonScale   = theSupportData->getLonScale();
         theHgtScale   = theSupportData->getHeightScale();
         theLineOffset = theSupportData->getLineOffset();
         theSampOffset = theSupportData->getSampOffset();
         theLatOffset  = theSupportData->getLatOffset();
         theLonOffset  = theSupportData->getLonOffset();
         theHgtOffset  = theSupportData->getHeightOffset();
      }

      // TODO MSD Check if this part is necessary
      _productXmlFile = DIMxmlFile;
      ossimSupportFilesList::instance()->add(_productXmlFile);

      // TODO MSD WARNING File with multi tiles are not well managed
      theSupportData->getImageRect(theImageClipRect);
      theSupportData->getImageSize(theImageSize);

      finishConstruction();
      clearErrorStatus();

      result = true;
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " exit status = " << (result ? "true" : "false\n") << std::endl;
      }

      /*std::cout << "---------------------------" << std::endl;
        print(std::cout);
        std::cout << "---------------------------" << std::endl;*/
      return result;
   }

//*************************************************************************************************
//! Collects common code among all parsers
//*************************************************************************************************
   void ossimSpot6Model::finishConstruction()
   {
      theImageSize.line = theImageClipRect.height();
      theImageSize.samp = theImageClipRect.width();
      theRefImgPt.line = theImageClipRect.midPoint().y;
      theRefImgPt.samp = theImageClipRect.midPoint().x;
      theRefGndPt.lat = theLatOffset;
      theRefGndPt.lon = theLonOffset;
      theRefGndPt.hgt = theHgtOffset;

      //---
      // NOTE:  We must call "updateModel()" to set parameter used by base
      // ossimRpcModel prior to calling lineSampleHeightToWorld or all
      // the world points will be same.
      //---
      updateModel();

      ossimGpt v0, v1, v2, v3;
      lineSampleHeightToWorld(theImageClipRect.ul(), theHgtOffset, v0);
      lineSampleHeightToWorld(theImageClipRect.ur(), theHgtOffset, v1);
      lineSampleHeightToWorld(theImageClipRect.lr(), theHgtOffset, v2);
      lineSampleHeightToWorld(theImageClipRect.ll(), theHgtOffset, v3);

      theBoundGndPolygon = ossimPolygon (ossimDpt(v0), ossimDpt(v1), ossimDpt(v2), ossimDpt(v3));

      // Set the ground reference point using the model.
      lineSampleHeightToWorld(theRefImgPt, theHgtOffset, theRefGndPt);

      if( theGSD.hasNans() )
      {
         try
         {
            // This will set theGSD and theMeanGSD. Method throws ossimException.
            computeGsd();
         }
         catch (const ossimException& e)
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << "ossimSpot6Model::finishConstruction -- caught exception:\n"
               << e.what() << std::endl;
         }
      }
   }

}

