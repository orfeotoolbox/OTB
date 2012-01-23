//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  Mickael Savinaud
//
// Description:
//
// Contains definition of class ossimPleiadesModel.
//
//*****************************************************************************
// FIXME $Id: ossimSpot5Model.cpp 19658 2011-05-26 13:16:06Z gpotts $

#include "ossimPleiadesModel.h"

#include <cmath>
#include <cstdio>

#include <ossimPleiadesModel.h>
#include <ossimPleiadesDimapSupportData.h>

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
static ossimTrace traceExec  ("ossimPleiadesModel:exec");
static ossimTrace traceDebug ("ossimPleiadesModel:debug");


RTTI_DEF1(ossimPleiadesModel, "ossimPleiadesModel", ossimRpcModel);

//*************************************************************************************************
// Constructor
//*************************************************************************************************
ossimPleiadesModel::ossimPleiadesModel()
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
ossimPleiadesModel::ossimPleiadesModel(const ossimPleiadesModel& rhs)
   :ossimRpcModel      (rhs),
    theSupportData        (0)
{
}

//*************************************************************************************************
// Destructor
//*************************************************************************************************
ossimPleiadesModel::~ossimPleiadesModel()
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG DESTRUCTOR: ~ossimPleiadesModel(): entering..." << std::endl;

   theSupportData = 0;

   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG DESTRUCTOR: ~ossimPleiadesModel(): returning..." << std::endl;
}
//*************************************************************************************************
// Infamous DUP
//*************************************************************************************************
ossimObject* ossimPleiadesModel::dup() const
{
   return new ossimPleiadesModel(*this);
}

//*************************************************************************************************
// Print
//*************************************************************************************************
std::ostream& ossimPleiadesModel::print(std::ostream& out) const
{
   // Capture stream flags since we are going to mess with them.
   std::ios_base::fmtflags f = out.flags();

   out << "\nDump of ossimPleiadesModel at address " << (hex) << this
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
bool ossimPleiadesModel::saveState(ossimKeywordlist& kwl,
                          const char* prefix) const
{
  if(theSupportData.valid())
  {
     ossimString supportPrefix = ossimString(prefix) + "support_data.";
     theSupportData->saveState(kwl, supportPrefix);
     //std::cout << "ossimPleiadesModel::saveState: " << std::endl;
     //kwl.print(std::cout);
  }

  if (theSupportData->getProcessingLevel() == "SENSOR")
   return ossimRpcModel::saveState(kwl, prefix);
  else
   return true;
}

//*************************************************************************************************
// Load State
//*************************************************************************************************
bool ossimPleiadesModel::loadState(const ossimKeywordlist& kwl,
                                const char* prefix)
{
   if(!theSupportData)
   {
      theSupportData = new ossimPleiadesDimapSupportData;
   }

   ossimString supportPrefix = ossimString(prefix) + "support_data.";
   theSupportData->loadState(kwl, supportPrefix);

   if (theSupportData->getProcessingLevel() == "SENSOR")
    return ossimRpcModel::loadState(kwl, prefix);
   else
    return true;
}



bool
ossimPleiadesModel::open(const ossimFilename& file)
{
  static const char MODULE[] = "ossimPleiadesModel::open";
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
  if (file.ext().downcase() != "jp2")
    {
    //not a jpeg2000 file
    return false;
    }
  else
    {
    if (file.isFile())
      {
      DIMxmlFile = file.path();
      RPCxmlFile = file.path();

      ossimFilename DIMxmlFileTmp = file.file();
      ossimFilename RPCxmlFileTmp;

      DIMxmlFileTmp = DIMxmlFileTmp.file().replaceStrThatMatch("^IMG_", "DIM_");

      DIMxmlFileTmp = DIMxmlFileTmp.replaceStrThatMatch("_R[0-9]+C[0-9]+\\.JP2$", ".XML");

      RPCxmlFileTmp = DIMxmlFileTmp.file().replaceStrThatMatch("^DIM_", "RPC_");

      DIMxmlFile = DIMxmlFile.dirCat(DIMxmlFileTmp);
      RPCxmlFile = RPCxmlFile.dirCat(RPCxmlFileTmp);
      }
    }

  if (traceDebug())
    {
    ossimNotify(ossimNotifyLevel_DEBUG) << "metadata xml file: " << DIMxmlFile << "\n";
    ossimNotify(ossimNotifyLevel_DEBUG) << "rpc xml file: " << RPCxmlFile << "\n";
    }

  ossimString processingLevel;
  // Parse the metadata xml file
  if (DIMxmlFile.exists())
    {
    if ( !theSupportData.valid() )
       theSupportData = new ossimPleiadesDimapSupportData();

    if(!theSupportData->parseXmlFile(DIMxmlFile))
    {
       theSupportData = 0; // ossimRefPtr
       ossimNotify(ossimNotifyLevel_WARN) << "ossimPleiadesModel::open WARNING:"
          << "\nCould not open correctly DIMAP file" << std::endl;
       return false;
    }

    theSensorID = theSupportData->getSensorID();
    theImageID = theSupportData->getImageID();
    // Get the processing level (ORTHO or SENSOR or perhaps MOSAIC ?)
    processingLevel = theSupportData->getProcessingLevel();
    }
  else
    return false;

  // Parse the RPC xml file if necessary
  if (RPCxmlFile.exists() && (processingLevel == "SENSOR"))
    {
    if (!theSupportData->parseXmlFile(RPCxmlFile))
      {
      theSupportData = 0; // ossimRefPtr
      ossimNotify(ossimNotifyLevel_WARN) << "ossimPleiadesModel::open WARNING:"
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

  //print(std::cout);
  return result;
}

//*************************************************************************************************
//! Collects common code among all parsers
//*************************************************************************************************
void ossimPleiadesModel::finishConstruction()
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
            << "ossimPleiadesModel::finishConstruction -- caught exception:\n"
            << e.what() << std::endl;
      }
   }
}

}
