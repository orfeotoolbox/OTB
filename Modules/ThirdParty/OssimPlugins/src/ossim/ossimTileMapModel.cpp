//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// AUTHOR: Jordi Inglada
//
// DESCRIPTION:
//
// SOFTWARE HISTORY:
//   23JAN2008  Jordi Inglada, CNES
//              Initial coding.
//
//*****************************************************************************

#include "ossimTileMapModel.h"

#include <stdio.h>
#include <cstdlib>

#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimKeywordNames.h>

//***
// Define Trace flags for use within this file:
//***
#include <ossim/base/ossimTrace.h>
static ossimTrace traceExec  ("ossimTileMapModel:exec");
static ossimTrace traceDebug ("ossimTileMapModel:debug");

namespace ossimplugins
{

   RTTI_DEF1(ossimTileMapModel, "ossimTileMapModel", ossimSensorModel);

//*****************************************************************************
//  DEFAULT CONSTRUCTOR: ossimTileMapModel()
//
//*****************************************************************************
   ossimTileMapModel::ossimTileMapModel()
      :
      ossimSensorModel(),
      qDepth (1)

   {
      if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimTileMapModel::ossimTileMapModel: entering..." << std::endl;

//   initAdjustableParameters();

//    std::cout << "TileMapModel constructor" << std::endl;

      if (traceExec()) ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimTileMapModel::ossimTileMapModel: returning..." << std::endl;
   }


   bool ossimTileMapModel::open(const ossimFilename& file)
   {
      static const char MODULE[] = "ossimTileMapModel::open";

      ossimString os = file.beforePos(4);
  
      if (traceDebug())
      {
         CLOG << " Entered..." << std::endl
              << " trying to open file " << file << std::endl;
      }
      if(os == "http" || file.ext() == "otb")
      {
         return true;
      }
  
      return false;
   }


//*****************************************************************************
//  CONSTRUCTOR: ossimTileMapModel(kwl)
//
//  Constructs model from keywordlist geometry file
//
//*****************************************************************************
   ossimTileMapModel::ossimTileMapModel(const ossimKeywordlist& geom_kwl)
      :
      ossimSensorModel(),
      qDepth (1)
   {
      if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimTileMapModel::ossimTileMapModel(geom_kwl): entering..." << std::endl;


      //***
      // Parse keywordlist for geometry:
      //***
      loadState(geom_kwl);

      if (traceExec()) ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimTileMapModel::ossimTileMapModel(geom_kwl): Exited..." << std::endl;
   }

//*****************************************************************************
//  COPY CONSTRUCTOR:
//*****************************************************************************
   ossimTileMapModel::ossimTileMapModel(const ossimTileMapModel& rhs)
      :
      ossimSensorModel   (rhs),
      qDepth (rhs.qDepth)
   {
      if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimTileMapModel::ossimTileMapModel(rhs): entering..." << std::endl;

      //initAdjustableParameters();

      if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimTileMapModel::ossimTileMapModel(rhs): returning..." << std::endl;
   }

//*****************************************************************************
//  METHOD: ossimTileMapModel::lineSampleHeightToWorld()
//
//  Performs the line/sample to groundpoint projection given an elevation.
//
//*****************************************************************************
   void ossimTileMapModel::lineSampleToWorld(const ossimDpt& image_point,
                                             ossimGpt&       gpt) const
   {
      lineSampleHeightToWorld(image_point, 0.0, gpt);
   }
   void ossimTileMapModel::lineSampleHeightToWorld(const ossimDpt& image_point,
                                                   const double&   /* height */,
                                                   ossimGpt&       gpt) const
   {
      if(!image_point.hasNans())
      {
         gpt.lon = static_cast<double>(image_point.samp)/(1 << qDepth)/256 *360.0-180.0;
         double y = static_cast<double>(image_point.line)/(1 << qDepth)/256;
         double ex = exp(4*M_PI*(y-0.5));
         gpt.lat = -180.0/M_PI*asin((ex-1)/(ex+1));
      }
      else
      {
         gpt.makeNan();
      }
      return;
   }

   void ossimTileMapModel::worldToLineSample(const ossimGpt& ground_point,
                                             ossimDpt&       img_pt) const
   {
      // if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimTileMapModel::worldToLineSample(): entering..." << std::endl;

      if(ground_point.isLatNan() || ground_point.isLonNan() )
      {
         img_pt.makeNan();
         return;
      }

      double x = (180.0 + ground_point.lon) / 360.0;
      double y = - ground_point.lat * M_PI / 180; // convert to radians
      y = 0.5 * log((1+sin(y)) / (1 - sin(y)));
      y *= 1.0/(2 * M_PI); // scale factor from radians to normalized
      y += 0.5; // and make y range from 0 - 1

      img_pt.samp = floor(x*pow(2.,static_cast<double>(qDepth))*256);

      img_pt.line = floor(y*pow(2.,static_cast<double>(qDepth))*256);

      return;
   }



//*****************************************************************************
//  METHOD: ossimTileMapModel::print()
//
//  Formatted dump of data members.
//
//*****************************************************************************
   std::ostream& ossimTileMapModel::print(std::ostream& os) const
   {
      os << "\nDump of ossimTileMapModel object at "
         << hex << this << ":\n"
         << "\nTileMapModel -- Dump of all data members: "
         << "\n        theImageID: " << theImageID.chars()
         << "\n        theImageSize: " << theImageSize
         << "\n        theRefImgPt: " << theRefImgPt
         << "\n        theRefGndPt: " << theRefGndPt
         << "\n        theGSD.line: " << theGSD.line
         << "\n        theGSD.samp: " << theGSD.samp
         << "\n        qDepth: " << qDepth
         << endl;

      return ossimSensorModel::print(os);
   }

//*****************************************************************************
//  METHOD: ossimTileMapModel::saveState()
//
//  Saves the model state to the KWL. This KWL also serves as a geometry file.
//
//*****************************************************************************
   bool ossimTileMapModel::saveState(ossimKeywordlist& kwl,
                                     const char* prefix) const
   {
      if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimTileMapModel::saveState: entering..." << std::endl;

      kwl.add(prefix, ossimKeywordNames::TYPE_KW, TYPE_NAME(this));

      kwl.add(prefix, "depth", qDepth);

      //***
      // Hand off to base class for common stuff:
      //***
      ossimSensorModel::saveState(kwl, prefix);


      if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimTileMapModel::saveState: returning..." << std::endl;
      return true;
   }

//*****************************************************************************
//  METHOD: ossimTileMapModel::loadState()
//
//  Restores the model's state from the KWL. This KWL also serves as a
//  geometry file.
//
//*****************************************************************************
   bool ossimTileMapModel::loadState(const ossimKeywordlist& kwl,
                                     const char* prefix)
   {
      if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimTileMapModel::loadState: entering..." << std::endl;

      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimTileMapModel::loadState:"
                                             << "\nInput kwl:  " << kwl
                                             << std::endl;
      }

      const char* value = NULL;
      //const char* keyword =NULL;
      bool success;

      //***
      // Assure this keywordlist contains correct type info:
      //***
      value = kwl.find(prefix, ossimKeywordNames::TYPE_KW);
      if (!value || (strcmp(value, TYPE_NAME(this))))
      {
         theErrorStatus = 1;
         return false;
      }

      value = kwl.find(prefix, "depth");
      qDepth = atoi(value);

      //***
      // Pass on to the base-class for parsing first:
      //***
      success = ossimSensorModel::loadState(kwl, prefix);
      if (!success)
      {
         theErrorStatus++;
         return false;
      }


      updateModel();

      if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimTileMapModel::loadState: returning..." << std::endl;
      return true;
   }

//*****************************************************************************
// STATIC METHOD: ossimTileMapModel::writeGeomTemplate
//
//  Writes a sample kwl to output stream.
//
//*****************************************************************************
   void ossimTileMapModel::writeGeomTemplate(ostream& os)
   {
      if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimTileMapModel::writeGeomTemplate: entering..." << std::endl;

      os <<
         "//**************************************************************\n"
         "// Template for TileMap model keywordlist\n"
         "//**************************************************************\n"
         << ossimKeywordNames::TYPE_KW << ": " << "ossimTileMapModel" << endl;


      if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimTileMapModel::writeGeomTemplate: returning..." << std::endl;
      return;
   }
} // End: namespace ossimplugins
