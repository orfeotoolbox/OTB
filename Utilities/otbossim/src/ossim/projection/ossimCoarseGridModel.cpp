//*****************************************************************************
// FILE: ossimCoarseGridModel.cc
//
// License:  See LICENSE.txt file in the top level directory.
//
// AUTHOR: Oscar Kramer
//
// DESCRIPTION:
//   Contains implementation of class ossimCoarseGridModel. This is an
//   implementation of an interpolation sensor model. 
//
//   IMPORTANT: The lat/lon grid is for ground points on the ellipsoid.
//   The dLat/dHgt and dLon/dHgt partials therefore are used against
//   elevations relative to the ellipsoid.
//
//*****************************************************************************
//  $Id: ossimCoarseGridModel.cpp 11955 2007-10-31 16:10:22Z gpotts $

#include <ossim/projection/ossimCoarseGridModel.h>

RTTI_DEF1(ossimCoarseGridModel, "ossimCoarseGridModel", ossimSensorModel);

#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimDatumFactory.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/elevation/ossimElevManager.h>

#include <cstdio>
#include <fstream>

//***
// Define Trace flags for use within this file:
//***
#include <ossim/base/ossimTrace.h>

static ossimTrace traceExec  ("ossimCoarseGridModel:exec");
static ossimTrace traceDebug ("ossimCoarseGridModel:debug");

static const char* MODEL_TYPE = "ossimCoarseGridModel";
static const char* GRID_FILE_NAME_KW = "grid_file_name";
static const ossimFilename DEFAULT_GEOM_FILE_EXT ("_ocg.geom");
static const ossimFilename DEFAULT_GRID_FILE_EXT ("_ocg.dat");
static const ossimFilename DEFAULT_MET_GRID_NAME ("ecg.grid");
static const ossimString   MET_ECG_MAGIC_NUMBER  ("eCG");

double ossimCoarseGridModel::theInterpolationError = .1;
ossim_int32 ossimCoarseGridModel::theMinGridSpacing     = 100;

//*****************************************************************************
//  DEFAULT CONSTRUCTOR: ossimCoarseGridModel()
//  
//*****************************************************************************
ossimCoarseGridModel::ossimCoarseGridModel()
   :
      ossimSensorModel(),
      theDlatDparamGrid (0),
      theDlonDparamGrid (0),
      theHeightEnabledFlag(true)
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG)
      << "DEBUG ossimCoarseGridModel::ossimCoarseGridModel: entering..."
      << std::endl;

   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG)
      << "DEBUG ossimCoarseGridModel::ossimCoarseGridModel: returning..."
      << std::endl;
}

//*****************************************************************************
//  COPY CONSTRUCTOR: ossimCoarseGridModel(ossimCoarseGridModel)
//  
//*****************************************************************************
ossimCoarseGridModel::ossimCoarseGridModel(const ossimCoarseGridModel& model)
   :
      ossimSensorModel  (model),
      theGridFilename   (model.theGridFilename),
      theLatGrid        (model.theLatGrid),
      theLonGrid        (model.theLonGrid),
      theDlatDhGrid     (model.theDlatDhGrid),
      theDlonDhGrid     (model.theDlonDhGrid),
      theHeightEnabledFlag(true)
{
   if (traceExec()) ossimNotify(ossimNotifyLevel_DEBUG)
      << "DEBUG ossimCoarseGridModel::ossimCoarseGridModel(model): entering..."
      << std::endl;

   int numberOfParams = getNumberOfAdjustableParameters();
   if(numberOfParams)
   {
      //***
      // Allocate adjustable parameter partials grids then assign:
      //***
      theDlatDparamGrid = new ossimDblGrid [numberOfParams];
      theDlonDparamGrid = new ossimDblGrid [numberOfParams];
      
      for (int i=0; i<numberOfParams; i++)
      {
         theDlatDparamGrid[i] = model.theDlatDparamGrid[i];
         theDlonDparamGrid[i] = model.theDlonDparamGrid[i];
      }
   }
   
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG)
      << "DEBUG ossimCoarseGridModel::ossimCoarseGridModel: returning..."
      << std::endl;
}

//*****************************************************************************
//  CONSTRUCTOR: ossimCoarseGridModel(filename)
//  
//  Constructs model from geometry file
//  
//*****************************************************************************
ossimCoarseGridModel::ossimCoarseGridModel(const ossimFilename& geom_file)
   :
      ossimSensorModel(),
      theDlatDparamGrid (0),
      theDlonDparamGrid (0),
      theHeightEnabledFlag(true)
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimCoarseGridModel::ossimCoarseGridModel(geom_file): entering..." << std::endl;

   loadGeomFile(geom_file);

   if (traceExec())   ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimCoarseGridModel::ossimCoarseGridModel(geom_file): returning..." << std::endl;
   return;
}

//*****************************************************************************
//  CONSTRUCTOR: ossimCoarseGridModel(kwl)
//  
//  Constructs model from keywordlist geometry file
//  
//*****************************************************************************
ossimCoarseGridModel::ossimCoarseGridModel(const ossimKeywordlist& geom_kwl)
   :
      ossimSensorModel(),
      theDlatDparamGrid (0),
      theDlonDparamGrid (0),
      theHeightEnabledFlag(true)
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimCoarseGridModel::ossimCoarseGridModel(geom_kwl): entering..." << std::endl;

   //***
   // Parse keywordlist for geometry:
   //***
   loadState(geom_kwl);
}

//*************************************************************************************************
//! Assigns the grid data given a projection (typically a rigorous sensor model)
//*************************************************************************************************
void ossimCoarseGridModel::buildGrid(const ossimDrect& imageBounds,
                                     ossimProjection* proj,
                                     double heightDelta,
                                     bool enableHeightFlag,
                                     bool makeAdjustableFlag)
{
   theHeightEnabledFlag =  enableHeightFlag;
   
   if(proj&&(!imageBounds.hasNans()))
   {
     // don't let it get any smaller than 100, 100 pixels
     // on the input projector
     //
     // may want this to be adjusted by outside
     //
     const ossimDatum* targetDatum = ossimDatumFactory::instance()->wgs84();
     ossimIpt gridSize(2,2);
     ossimDpt gridOrigin(0,0);
     ossimGpt gpt;
     ossimGpt gpt2;
     ossimGpt bilinearGpt;
     resizeAdjustableParameterArray(0);
     double normSplit = 1.0;
     ossimDpt imageSize(imageBounds.width(),
                        imageBounds.height());
     double error = 0.0;

     // int y=0;
     // int x=0;
     // double ht=0, vt=0;
     // double w = imageBounds.width();
     // double h = imageBounds.height();
     ossimIpt imageOrigin = imageBounds.ul();
     // ossimDpt metersPerPixel = proj->getMetersPerPixel();
     ossimDpt spacing ((double)(imageBounds.width()-1)/(gridSize.x-1),
                       (double)(imageBounds.height()-1)/(gridSize.y-1));

     if(theDlatDparamGrid)
     {
        delete [] theDlatDparamGrid;
        theDlatDparamGrid = NULL;
     }
     if(theDlonDparamGrid)
     {
        delete [] theDlonDparamGrid;
        theDlonDparamGrid = NULL;
     }
     
     proj->lineSampleToWorld(imageBounds.midPoint(), gpt);

     do
     {
        if(traceDebug())
        {
           ossimNotify(ossimNotifyLevel_DEBUG) << "Checking grid size " << gridSize << std::endl;
        }
        
        spacing = ossimDpt((double)(imageBounds.width()-1)/(gridSize.x-1),
                           (double)(imageBounds.height()-1)/(gridSize.y-1));

        theLatGrid.setNullValue(ossim::nan());
        theLonGrid.setNullValue(ossim::nan());
        theDlatDhGrid.setNullValue(0.0);
        theDlonDhGrid.setNullValue(0.0);
        theLatGrid.initialize(gridSize, gridOrigin, spacing);
        theLonGrid.initialize(gridSize, gridOrigin, spacing);
        theDlatDhGrid.initialize(gridSize, gridOrigin, spacing);
        theDlonDhGrid.initialize(gridSize, gridOrigin, spacing);
        ossim_int32 x, y;
        
        for(y = 0; y < gridSize.y; ++y)
        {
           for(x = 0; x < gridSize.x; ++x)
           {
              ossimDpt norm((double)x/(double)(gridSize.x-1),
                            (double)y/(double)(gridSize.y-1));
              
              ossimDpt pt(imageOrigin.x + norm.x*(imageSize.x-1),
                          imageOrigin.y + norm.y*(imageSize.y-1));
              
              proj->lineSampleToWorld(pt, gpt);
              double h = gpt.height();
              if(ossim::isnan(h))
              {
                 h += heightDelta;
              }
              proj->lineSampleHeightToWorld(pt, h, gpt2);
              gpt.changeDatum(targetDatum);
              gpt2.changeDatum(targetDatum);
              
              theLatGrid.setNode(x, y, gpt.latd());
              theLonGrid.setNode(x, y, gpt.lond());

              theDlatDhGrid.setNode(x, y, (gpt2.latd() - gpt.latd())/heightDelta);
              theDlonDhGrid.setNode(x, y, (gpt2.lond() - gpt.lond())/heightDelta);
           }
        }
        ossim_int32 upperY = 2*gridSize.y;
        ossim_int32 upperX = 2*gridSize.x;
        error = 0.0;

        ossimDpt v[4];
        v[0].lat = theLatGrid.getNode(0,0);
        v[0].lon = theLonGrid.getNode(0,0);
        v[1].lat = theLatGrid.getNode(gridSize.x-1, 0);
        v[1].lon = theLonGrid.getNode(gridSize.x-1, 0);
        v[2].lat = theLatGrid.getNode(gridSize.x-1, gridSize.y-1);
        v[2].lon = theLonGrid.getNode(gridSize.x-1, gridSize.y-1);
        v[3].lat = theLatGrid.getNode(0, gridSize.y-1);
        v[3].lon = theLonGrid.getNode(0, gridSize.y-1);
        theBoundGndPolygon = ossimPolygon(4, v);
        
        
        theImageSize  = ossimDpt(imageBounds.width(), imageBounds.height());
        theRefImgPt   = imageBounds.midPoint();
        theRefGndPt   = ossimGpt(theLatGrid(theRefImgPt),
                                 theLonGrid(theRefImgPt));
        ossimDpt ref_ip_dx(theRefImgPt.x+1.0, theRefImgPt.y);
        ossimDpt ref_ip_dy(theRefImgPt.x, theRefImgPt.y+1.0);
        ossimGpt ref_gp_dx(theLatGrid(ref_ip_dx), theLonGrid(ref_ip_dx));
        ossimGpt ref_gp_dy(theLatGrid(ref_ip_dy), theLonGrid(ref_ip_dy));

        theGSD.x   = theRefGndPt.distanceTo(ref_gp_dx);
        theGSD.y   = theRefGndPt.distanceTo(ref_gp_dy);

        theMeanGSD = (theGSD.line + theGSD.samp)/2.0;
        theImageClipRect  = imageBounds;
        theSubImageOffset = ossimIpt(0,0);

        for(y = 0; ((y < upperY)&&(error < theInterpolationError)); ++y)
        {
           for(x = 0; ((x < upperX)&&(error<theInterpolationError)); ++x)
           {
              ossimDpt norm((double)x/(double)(upperX-1),
                            (double)y/(double)(upperY-1));
              
              ossimDpt imagePoint(imageOrigin.x + norm.x*(imageSize.x-1),
                                  imageOrigin.y + norm.y*(imageSize.y-1));
              ossimDpt testIpt;

               proj->lineSampleToWorld(imagePoint, gpt);
               worldToLineSample(gpt, testIpt);
//               lineSampleToWorld(imagePoint, bilinearGpt);
//               gpt.changeDatum(targetDatum);
//               double dist = gpt.distanceTo(bilinearGpt);

//               error = ossim::max(ossim::max(dist/theMeanGSD, dist/theMeanGSD), error);
               error = (testIpt-imagePoint).length();
           }
        }

        gridSize.x *= 2;
        gridSize.y *= 2;
        normSplit *= .5;
     }while((error > theInterpolationError)&&
            ((imageSize.x*normSplit) > theMinGridSpacing)&&
            ((imageSize.y*normSplit) > theMinGridSpacing));


     gridSize = theLatGrid.size();

     ossimAdjustableParameterInterface* adjustableParameters = PTR_CAST(ossimAdjustableParameterInterface,
                                                                        proj);
     removeAllAdjustments();
     if(adjustableParameters&&makeAdjustableFlag)
     {
        if(adjustableParameters->getNumberOfAdjustableParameters() > 0)
        {
           newAdjustment(adjustableParameters->getNumberOfAdjustableParameters());

           int numberOfParams = getNumberOfAdjustableParameters();
           if(numberOfParams)
           {
              //***
              // Allocate adjustable parameter partials grids then assign:
              //***
              theDlatDparamGrid = new ossimDblGrid [numberOfParams];
              theDlonDparamGrid = new ossimDblGrid [numberOfParams];
              ossim_uint32 paramIdx = 0;
              for(paramIdx = 0; paramIdx < getNumberOfAdjustableParameters(); ++ paramIdx)
              {
                 theDlonDparamGrid[paramIdx].setNullValue(0.0);
                 theDlatDparamGrid[paramIdx].setNullValue(0.0);
                 theDlatDparamGrid[paramIdx].initialize(gridSize, gridOrigin, spacing);
                 theDlonDparamGrid[paramIdx].initialize(gridSize, gridOrigin, spacing);
                 setAdjustableParameter(paramIdx,
                                        0.0);
                 setParameterSigma(paramIdx,
                                   adjustableParameters->getParameterSigma(paramIdx));
                 setParameterUnit(paramIdx,
                                  adjustableParameters->getParameterUnit(paramIdx));
                 setParameterCenter(paramIdx,
                                    0.0);
                 setParameterDescription(paramIdx,
                                         adjustableParameters->getParameterDescription(paramIdx));

                 double oldParameter = adjustableParameters->getAdjustableParameter(paramIdx);
                 adjustableParameters->setAdjustableParameter(paramIdx, 1.0, true);
                 double adjust = adjustableParameters->computeParameterOffset(paramIdx);
                 double deltaLat = 0;
                 double deltaLon = 0;
                 if(adjust != 0.0)
                 {
                    for(int y = 0; y < gridSize.y; ++y)
                    {
                       for(int x = 0; x < gridSize.x; ++x)
                       {
                          ossimDpt norm((double)x/(double)(gridSize.x-1),
                                        (double)y/(double)(gridSize.y-1));
                          
                          ossimDpt pt(imageOrigin.x + norm.x*(imageSize.x-1),
                                      imageOrigin.y + norm.y*(imageSize.y-1));
                          
                          proj->lineSampleHeightToWorld(pt, 0.0, gpt);

                          gpt.changeDatum(targetDatum);
                          gpt2.latd(theLatGrid(pt));
                          gpt2.lond(theLonGrid(pt));
                          deltaLat = gpt.latd()-gpt2.latd();
                          deltaLon = gpt.lond()-gpt2.lond();

                          theDlatDparamGrid[paramIdx].setNode(x, y,
                                                              deltaLat/adjust);
                          theDlonDparamGrid[paramIdx].setNode(x, y,
                                                              deltaLon/adjust);
                       }
                    }
                 }
                 adjustableParameters->setAdjustableParameter(paramIdx, oldParameter, true);
              }
           }
        }
     }
     getAdjustment(theInitialAdjustment);
  }
}

void ossimCoarseGridModel::setInterpolationError(double error)
{
   theInterpolationError = error;
}

void ossimCoarseGridModel::setMinGridSpacing(ossim_int32 minSpacing)
{
   theMinGridSpacing = minSpacing;
}

//*****************************************************************************
//  DESTRUCTOR: ~ossimCoarseGridModel()
//  
//*****************************************************************************
ossimCoarseGridModel::~ossimCoarseGridModel()
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG)
      << "DEBUG ossimCoarseGridModel::~ossimCoarseGridModel: entering..."
      << std::endl;

   if(theDlatDparamGrid&&theDlonDparamGrid)
   {
      //***
      // Deallocate memory:
      //***
      delete [] theDlatDparamGrid;
      delete [] theDlonDparamGrid;
      theDlatDparamGrid = NULL;
      theDlonDparamGrid = NULL;
   }
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG)
      << "DEBUG ossimCoarseGridModel::~ossimCoarseGridModel: returning..."
      << std::endl;
}

void ossimCoarseGridModel::lineSampleToWorld(const ossimDpt& image_point,
                                             ossimGpt&       gpt) const
{
   if(!theHeightEnabledFlag)
   {
      //
      // Extrapolate if image point is outside image:
      //
      if (!insideImage(image_point))
      {
         gpt = extrapolate(image_point);
         return;
      }
      
      lineSampleHeightToWorld(image_point, 0.0, gpt);
   }
   else
   {
      ossimSensorModel::lineSampleToWorld(image_point, gpt);
   }
}
//*****************************************************************************
//  METHOD: ossimCoarseGridModel::lineSampleHeightToWorld()
//  
//  Establishes the ground point corresponding to the input image_point and
//  specified elevation above MSL
//
//*****************************************************************************
void
ossimCoarseGridModel::lineSampleHeightToWorld(const ossimDpt& lineSampPt,
                                              const double&   arg_hgt_above_ellipsoid,
                                              ossimGpt&       worldPt) const
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimCoarseGridModel::lineSampleHeightToWorld: entering..." << std::endl;
   
   if(theLatGrid.size().x < 1 ||
      theLatGrid.size().y < 1)
   {
      worldPt.makeNan();
      return;
   }

   double height = (ossim::isnan(arg_hgt_above_ellipsoid)) ? 0.0 : arg_hgt_above_ellipsoid;

   // Extrapolate if point is outside image:
   if (!insideImage(lineSampPt))
   {
      worldPt = extrapolate(lineSampPt, height);
      if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimCoarseGridModel::lineSampleHeightToWorld: returning..." << std::endl;
      return;
   }

   // The image point may correspond to an offset sub-image. Need to convert
   // to full image space before anything:
   ossimDpt ip = lineSampPt + theSubImageOffset;
   
   // Establish the interpolated values from the grids:
   worldPt.lat = theLatGrid(ip);
   worldPt.lon = theLonGrid(ip);
   worldPt.hgt = height;

   if(theHeightEnabledFlag)
   {
      // Adjust horizontally due to elevation:
      worldPt.lat += theDlatDhGrid(ip)*height;
      worldPt.lon += theDlonDhGrid(ip)*height;
   }
   int numberOfParams = getNumberOfAdjustableParameters();
 
   // Now add increments due to adjustable parameter deltas:
   for (int p=0; p<numberOfParams; p++)
   {
       worldPt.lat += (theDlatDparamGrid[p](ip) * computeParameterOffset(p));
       worldPt.lon += (theDlonDparamGrid[p](ip) * computeParameterOffset(p));
   }

   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimCoarseGridModel::lineSampleHeightToWorld: returning..." << std::endl;
}

void ossimCoarseGridModel::initAdjustableParameters()
{
   if(getNumberOfAdjustableParameters() < 1)
   {
      addAdjustment(theInitialAdjustment, true);
   }
   else
   {
      setAdjustment(theInitialAdjustment, true);
   }
}

void ossimCoarseGridModel::imagingRay(const ossimDpt& image_point,
                                      ossimEcefRay&   image_ray) const
{
   ossimSensorModel::imagingRay(image_point, image_ray);
}


//*****************************************************************************
//  METHOD: ossimCoarseGridModel::print()
//  
//  Formatted dump of data members.
//  
//*****************************************************************************
std::ostream& ossimCoarseGridModel::print(std::ostream& out) const 
{
   out << "\nDump of ossimCoarseGridModel object at: " << this << "\n"
       << "\n           Grid File Name: " << theGridFilename 
       << "\n                 Image ID: " << theImageID 
       << "\n                   Sensor: " << theSensorID
       << "\n  Image Size (rows, cols): " << theImageSize
       << "\n      Ref Pt (samp, line): " << theRefImgPt 
       << "\n   Ref Pt (lat, lon, hgt): " << theRefGndPt 
       << "\n           GSD (row, col): " << theGSD 
       << "\n  Bounding Ground Polygon: " << theBoundGndPolygon << endl;
//      << "\n         Number of Params: " << theNumParams << "\n"<<endl;
   
   char buf[256];
   ossimIpt size (theLatGrid.size());
   ossimDpt spacing (theLatGrid.spacing());
   int line, samp;
   ossimIpt node;
   
   out << "[ line,  samp]        lat        lon         dLat/dH      dLon/dH\n"
       << "-------------------------------------------------------------------"
       <<endl;

   for (node.y=0; node.y<size.y; node.y++)
   {
      line = (int) (node.y*spacing.y);
      
      for (node.x=0; node.x<size.x; node.x++)
      {
         samp = (int) (node.x*spacing.x);
         
         sprintf(buf, "[%5d, %5d]    %+9.5f  %+10.5f    %+11.4e  %+11.4e",
                  line, samp,
                  theLatGrid.getNode(node),
                  theLonGrid.getNode(node),
                  theDlatDhGrid.getNode(node),
                  theDlonDhGrid.getNode(node));
         out << buf << endl;
      }
      out <<"-----------------------------------------------------------------"
          <<endl;
   }

   out << "\n\nDump of lat/lon Partials w.r.t. Adjustable Parameters:"<<endl;
   out << "\nEnd Dump of ossimCoarseGridModel.\n" <<  endl;
   return out;
}

//*****************************************************************************
//  METHOD: ossimCoarseGridModel::loadGeomFile
//  
//  When a geometry file is specified, this function determines the proper
//  method to invoke for reading contents.
//
//  Returns true if successful.
//  
//*****************************************************************************
bool ossimCoarseGridModel::loadGeomFile(const ossimFilename& geom_file)
{
   if (traceExec())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "DEBUG ossimCoarseGridModel::loadGeomFile: entering..."
         << "\ngeom_file:  " << geom_file << std::endl;

   }
   
   bool rtn_status;
   ossimFilename geomFile = geom_file;
   //***
   // Open the file and test for format:
   //***
   ifstream ifs (geom_file);
      
   if (!ifs)
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "DEBUG : "
            << "Failed attempt trying to open ossimCoarseGridModel geom "
            << "file at: " << geom_file << ". Aborting construction..."
            << std::endl;
      }

      theErrorStatus++;
      if (traceExec())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "DEBUG ossimCoarseGridModel::loadGeomFile: returning..."
            << std::endl;
      }
      
      return false;
   }
   
   //***
   // Test for MET ECG magic number, and hand off to met ECG loader if present:
   //***
   // can't use ifs >> magic_number since on binary files this could go for a while.
   // must use explicit byte count for test.
   //
      
      char ecgMagic[4];
      ifs.read((char*)ecgMagic, 3);
      ecgMagic[3] = '\0';

   if(ossimString(ecgMagic)== MET_ECG_MAGIC_NUMBER)
   {
      ossimFilename grid_file = geom_file;
      grid_file.setExtension("");
      grid_file.setFile(DEFAULT_MET_GRID_NAME);
      rtn_status = loadMetEcgGeom(geom_file, grid_file);

      if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimCoarseGridModel::loadGeomFile: returning..." << std::endl;
      return rtn_status;
   }
   ifs.close();

   //***
   // Otherwise, assume the geom file is an OCG keywordlist:
   //***
   ossimKeywordlist kwl (geom_file);

   rtn_status = loadState(kwl);
   if(kwl.getErrorStatus() != ossimErrorCodes::OSSIM_OK)
   {
      rtn_status = false;
   }

   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimCoarseGridModel::loadGeomFile: returning..." << std::endl;
   return rtn_status;
}

//*****************************************************************************
//  METHOD: ossimCoarseGridModel::saveState()
//  
//  Saves the model state to the KWL. This KWL also serves as a geometry file.
//
//  Returns true if successful.
//  
//*****************************************************************************
bool ossimCoarseGridModel::saveState(ossimKeywordlist& kwl,
                                     const char* prefix) const
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimCoarseGridModel::saveState: entering..." << std::endl;

   kwl.add(prefix, ossimKeywordNames::TYPE_KW, "ossimCoarseGridModel");
   kwl.add(prefix, GRID_FILE_NAME_KW, theGridFilename);
   kwl.add(prefix, "height_enabled_flag", theHeightEnabledFlag, true);
   ossimSensorModel::saveState(kwl, prefix);
   ossimString initAdjPrefix = ossimString(prefix) + "init_adjustment.";
   theInitialAdjustment.saveState(kwl, initAdjPrefix);
   
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimCoarseGridModel::saveState: returning..." << std::endl;

   return true;
}

//*****************************************************************************
//  METHOD: ossimCoarseGridModel::loadState()
//  
//  Restores the model's state from the KWL. This KWL also serves as a
//  geometry file.
//
//  Returns true if successful.
//  
//*****************************************************************************
bool ossimCoarseGridModel::loadState(const ossimKeywordlist& kwl,
                                     const char* prefix) 
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimCoarseGridModel::loadState: entering..." << std::endl;

   const char* value;
   bool success;
   
   //***
   // Assure this keywordlist contains correct type info:
   //***
   value = kwl.find(prefix, ossimKeywordNames::TYPE_KW);
   if (!value || (strcmp(value, "ossimCoarseGridModel")))
   {
      if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimCoarseGridModel::loadState:  returning..." << std::endl;
      theErrorStatus++;
      return false;
   }
   value = kwl.find(prefix, "height_enabled_flag");
   if(value)
   {
      theHeightEnabledFlag = ossimString(value).toBool();
   }

   //***
   // Look for specification of separate geometry file:
   //***
   value = kwl.find(prefix, ossimKeywordNames::GEOM_FILE_KW);
   if (value)
   {
      success = loadGeomFile(ossimFilename(value));
      if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimCoarseGridModel::loadState: returning..." << std::endl;
      return success;
   }
   
   //***
   // Pass on to the base-class for parsing first:
   //***
   success = ossimSensorModel::loadState(kwl, prefix);
   if (!success)
   {
      theErrorStatus++;
      if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimCoarseGridModel::loadState: returning with error..." << std::endl;
      return false;
   }

   value = kwl.find(prefix, GRID_FILE_NAME_KW);
   if (value)
      theGridFilename = value;
   else
   {
      theErrorStatus++;
//       ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimCoarseGridModel::loadState: Error encountered parsing the following required keyword: "
//                                           << "<" <<GRID_FILE_NAME_KW
//                                           << ">. Check the keywordlist for proper syntax." << std::endl;
//       if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimCoarseGridModel::loadState: returning with error..." << std::endl;
      return false;
   }

   if((theGridFilename != "") &&
      (theGridFilename.exists()))
   {
      //***
      // Load the coarse grid file specified in KWL. This method resets the
      // theErrorStatus to OSSIM_OK if successful:
      //***
      loadCoarseGrid(theGridFilename);
   }

   ossimString initAdjPrefix = ossimString(prefix) + "init_adjustment.";

   theInitialAdjustment.loadState(kwl, initAdjPrefix.c_str());

   if((ossim::isnan(theRefGndPt.hgt)) ||
      (theRefGndPt.hgt == 0))
   {
      theRefGndPt.hgt = ossimElevManager::instance()->getHeightAboveEllipsoid(theRefGndPt);
      if(theRefGndPt.hgt < 0)
      {
         theRefGndPt.hgt = fabs(theRefGndPt.hgt);
      }
   }
      
   if(theInitialAdjustment.getNumberOfAdjustableParameters() < 1)
   {
      getAdjustment(theInitialAdjustment);
   }
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimCoarseGridModel::loadState: returning..." << std::endl;
   if (theErrorStatus)
      return false;
   
   return true;
}

//*****************************************************************************
//  METHOD: ossimCoarseGridModel::saveCoarseGrid(cgFileName)
//  
//  Saves the coarse grid to disk file.
//
//  Returns true if successful.
//  
//*****************************************************************************
bool ossimCoarseGridModel::saveCoarseGrid(const ossimFilename& cgFileName)const
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimCoarseGridModel::saveCoarseGrid: entering..." << std::endl;

   //***
   // Create and open grid file as stream:
   //***
   ofstream outstream (cgFileName.chars());
   if (!outstream.is_open())
   {
      theErrorStatus++;
      ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimCoarseGridModel::saveCoarseGrid: Error encountered creating coarse grid file <" << cgFileName
                                          << ">. Check that directory exists and is writable." << std::endl;
      if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimCoarseGridModel::saveCoarseGrid: returning with error..." << std::endl;
      return false;
   }

   //***
   // Let each grid object write itself to the output file:
   //***
   theLatGrid.save(outstream, "Latitude Grid");
   theLonGrid.save(outstream, "Longitude Grid");
   theDlatDhGrid.save(outstream, "dLat/dH Grid");
   theDlonDhGrid.save(outstream, "dLon_dH Grid");

   ossimString descr;
   int numberOfParams = getNumberOfAdjustableParameters();
   for (int p=0; p<numberOfParams; p++)
   {
      descr = getParameterDescription(p) + " dLat_dParam Grid";
      theDlatDparamGrid[p].save(outstream, descr.chars());
      descr = getParameterDescription(p) + " dLon_dParam Grid";
      theDlonDparamGrid[p].save(outstream, descr.chars());
   }
   theGridFilename = cgFileName;
   
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimCoarseGridModel::saveCoarseGrid: returning..." << std::endl;
   return true;
}


//*****************************************************************************
//  METHOD: ossimCoarseGridModel::loadCoarseGrid(cgFileName)
//  
//  Loads the coarse grid from disk file.
//
//  Returns true if successful.
//  
//*****************************************************************************
bool ossimCoarseGridModel::loadCoarseGrid(const ossimFilename& cgFileName)
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimCoarseGridModel::loadCoarseGrid: entering..." << std::endl;

   ossimDpt v[4];
   ossimIpt grid_size;

   //***
   // Open existing grid file:
   //***
   ifstream instream (cgFileName.chars());
   if (!instream.is_open())
   {
      theErrorStatus++;
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimCoarseGridModel::loadCoarseGrid: Error encountered opening coarse grid file <" << cgFileName
                                          << ">. Check that the file exists and is readable." << std::endl;
      if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "CEBUG ossimCoarseGridModel::loadCoarseGrid: returning with error..." << std::endl;
      return false;
   }
   theGridFilename = cgFileName;
   if(theDlatDparamGrid)
     {
       delete [] theDlatDparamGrid;
       theDlatDparamGrid = NULL;
     }
   if(theDlonDparamGrid)
     {
       delete [] theDlonDparamGrid;
       theDlonDparamGrid = NULL;
     }
   //
   // Reallocate memory:
   //
   int numberOfParams = getNumberOfAdjustableParameters();
   if(numberOfParams)
   {
      theDlatDparamGrid = new ossimDblGrid [numberOfParams];
      theDlonDparamGrid = new ossimDblGrid [numberOfParams];
   }
   //***
   // Let each grid object read itself from the input file:
   //***
   if (!theLatGrid.load(instream))
   {
      ++theErrorStatus;
      return false;
   }
   if (!theLonGrid.load(instream))
   {
      ++theErrorStatus;
      return false;
   }
   if (!theDlatDhGrid.load(instream))
   {
      ++theErrorStatus;
      return false;
   }
   if (!theDlonDhGrid.load(instream))
   {
      ++theErrorStatus;
      return false;
   }

   for (int p=0; p<numberOfParams; p++)
   {
      if (!theDlatDparamGrid[p].load(instream))
      {
         ++theErrorStatus;
         return false;
      }
      if (!theDlonDparamGrid[p].load(instream))
      {
         ++theErrorStatus;
         return false;
      }
   }

   //***
   // Initialize the bounging ground rectangle:
   //***
   grid_size = theLatGrid.size();


   v[0].lat = theLatGrid(0,0);
   v[0].lon = theLonGrid(0,0);
   v[1].lat = theLatGrid(theImageSize.x-1, 0);
   v[1].lon = theLonGrid(theImageSize.x-1, 0);
   v[2].lat = theLatGrid(theImageSize.x-1, theImageSize.y-1);
   v[2].lon = theLonGrid(theImageSize.x-1, theImageSize.y-1);
   v[3].lat = theLatGrid(0, theImageSize.y-1);
   v[3].lon = theLonGrid(0, theImageSize.y-1);


//     ossimGpt gpt;
//     lineSampleHeightToWorld(ossimDpt(0,0), 0, gpt);
//     v[0].lat = gpt.latd();
//     v[0].lon = gpt.lond();
//     lineSampleHeightToWorld(ossimDpt(grid_size.x-1,0), 0.0, gpt);
//     v[1].lat = gpt.latd();
//     v[1].lon = gpt.lond();
//     lineSampleHeightToWorld(ossimDpt(grid_size.x-1, grid_size.y-1), 0.0, gpt);
//     v[2].lat = gpt.latd();
//     v[2].lon = gpt.lond();
//     lineSampleHeightToWorld(ossimDpt(0,grid_size.y-1), 0.0,gpt);
//     v[3].lat = gpt.latd();
//     v[3].lon = gpt.lond();
   
   theBoundGndPolygon = ossimPolygon(4, v);

   
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimCoarseGridModel::loadCoarseGrid: returning..." << std::endl;
   return true;

   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG rossimCoarseGridModel::loadCoarseGrid: eturning with error..." << std::endl;
   return false;
}


//*****************************************************************************
//  METHOD: ossimCoarseGridModel::reallocateGrid()
//  
//  Deletes existing grid arrays and allocates new ones.
//  
//*****************************************************************************
void ossimCoarseGridModel::reallocateGrid(const ossimIpt& grid_size)
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimCoarseGridModel::reallocateGrid:entering..." << endl;


   //***
   // Deallocate memory:
   //***
   if(theDlatDparamGrid)
     {
       delete [] theDlatDparamGrid;
       theDlatDparamGrid = NULL;
     }
   if(theDlonDparamGrid)
     {
       delete [] theDlonDparamGrid;
       theDlonDparamGrid = NULL;
     }
   //***
   // determine grid spacing given new info:
   //***
   ossimDpt spacing ((double)(theImageSize.x-1)/(double)(grid_size.x-1),
                     (double)(theImageSize.y-1)/(double)(grid_size.y-1));

   //***
   // Allocate all:
   //***
   ossimDpt grid_origin(0.0, 0.0);
   theLatGrid.setNullValue(ossim::nan());
   theLonGrid.setNullValue(ossim::nan());
   theDlatDhGrid.setNullValue(0.0);
   theDlonDhGrid.setNullValue(0.0);
   theLatGrid.initialize(grid_size, grid_origin, spacing);
   theLonGrid.initialize(grid_size, grid_origin, spacing);
   theDlatDhGrid.initialize(grid_size, grid_origin, spacing);
   theDlonDhGrid.initialize(grid_size, grid_origin, spacing);

   
   int numberOfParams = getNumberOfAdjustableParameters();
   if(numberOfParams)
   {
      
      theDlatDparamGrid = new ossimDblGrid [numberOfParams];
      theDlonDparamGrid = new ossimDblGrid [numberOfParams];
   }
   for (int p=0; p<numberOfParams; p++)
   {
      theDlonDparamGrid[p].setNullValue(0.0);
      theDlatDparamGrid[p].setNullValue(0.0);
      theDlatDparamGrid[p].initialize(grid_size, grid_origin, spacing);
      theDlonDparamGrid[p].initialize(grid_size, grid_origin, spacing);
   }

   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimCoarseGridModel::reallocateGrid: returning..." << std::endl;
   return;
}

//*****************************************************************************
//  METHOD: ossimCoarseGridModel::loadMetEcgGeom(ecgGeomFile, ecgGridFile)
//  
//  Loads a met ECG geometry and grid, and outputs it to an
//  ossimCoarseGridModel format KWL and grid file with the same base name.
//  
//*****************************************************************************
bool ossimCoarseGridModel::loadMetEcgGeom(const ossimFilename& ecgGeomName,
                                          const ossimFilename& ecgGridName) 
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimCoarseGridModel::loadMetEcgGeom: entering..." << std::endl;

   ossimNotify(ossimNotifyLevel_INFO) << "INFO ossimCoarseGridModel::loadMetEcgGeom: Converting MET ECG geometry file... "<< std::endl;
   
   //***
   // Open existing MET ECG geom file:
   //***
   FILE* geomFile = fopen(ecgGeomName.chars(), "r");
   if (!geomFile)
   {
      theErrorStatus++;
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimCoarseGridModel::loadMetEcgGeom: Error encountered opening ECG geometry file <" << ecgGeomName
                                          << ">. Check that the file exists and is readable." << std::endl;
      
      if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimCoarseGridModel::loadMetEcgGeom: returning with error..." << std::endl;
      return false;
   }

   //***
   // Open existing MET ECG grid file:
   //***
   FILE* gridFile = fopen(ecgGridName.chars(), "r");
   if (!gridFile)
   {
      theErrorStatus++;
      ossimNotify(ossimNotifyLevel_FATAL) << "fATAL ossimCoarseGridModel::loadMetEcgGeom: Error encountered opening ECG grid file <" << ecgGridName
                                          << ">. Check that the file exists and is readable." << std::endl;
      if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimCoarseGridModel::loadMetEcgGeom: returning with error..." << std::endl;
      return false;
   }

   //***
   // Read from ECG geometry file:
   //***
   char    strbuf[128], desc[128];
   int     version;
   fscanf(geomFile,"%s %d %s", strbuf, &version, desc);
   theImageID = strbuf;
   
   if (version > 1)
   {
      fscanf(geomFile,"%s %s", strbuf, desc);
      theSensorID = strbuf;
   }
   else
      theSensorID = "UNKNOWN";

   ossimIpt grid_size;
   int numParams;
   fscanf(geomFile,"%d %s", (int*) &grid_size.line, desc);
   fscanf(geomFile,"%d %s", (int*) &grid_size.samp, desc);
   fscanf(geomFile,"%d %s", &numParams,     desc);

   removeAllAdjustments();
   newAdjustment(numParams);
   //***
   // Let base class allocate memory for parameter-associated arrays:
   //***
//   allocateAdjParams();
   
   if (version >3)
   {
      //***
      // values for image offsets are doubles
      //***
      fscanf(geomFile,"%lf %s", &theSubImageOffset.line, desc);
      fscanf(geomFile,"%lf %s", &theSubImageOffset.samp, desc);
   }
   else
   {
      if (version==3)
      {
         //***
         // values for patch offsets are integers
         //***
         int intOffsetLine;
         int intOffsetSample;
         fscanf(geomFile,"%d %s", &intOffsetLine, desc);
         fscanf(geomFile,"%d %s", &intOffsetSample, desc);
         theSubImageOffset.line = intOffsetLine;
         theSubImageOffset.samp = intOffsetSample;
      }
      else
      {
         //***
         // patch offsets not supported:
         //***
         theSubImageOffset.line = 0.0;
         theSubImageOffset.samp = 0.0;
      }
   }
   fscanf(geomFile, "%d %s",  (int*) &theImageSize.line, desc);
   fscanf(geomFile, "%d %s",  (int*) &theImageSize.samp, desc);

   //***
   // Initialize other data members given theImageSize:
   //***
   theImageClipRect = ossimDrect(0, 0, theImageSize.x-1, theImageSize.y-1);
   theRefImgPt = ossimDpt((double)(theImageSize.x-1)/2.0,
                          (double)(theImageSize.y-1)/2.0);

   double pos_error, illumAz, illumElev;
   int    illumDataFlag;
   
   fscanf(geomFile, "%lf %s", &theRefGndPt.hgt, desc);
   fscanf(geomFile, "%lf %s", &pos_error, desc);
   fscanf(geomFile, "%d %s",  &illumDataFlag,desc);
   fscanf(geomFile, "%lf %s", &illumAz,desc);
   fscanf(geomFile, "%lf %s", &illumElev,desc);

   //***
   // Read adjustable parameters from ECG geom file:
   //***
   int i;
   double param=0.0;
   for(i=0; i<numParams; i++)
   {
      fscanf(geomFile, "%lf %s", &param, desc);
      setParameterCenter(i, param, false);
      setAdjustableParameter(i, 0.0, false);
      setParameterDescription(i, desc);
      setParameterUnit(i, OSSIM_UNIT_UNKNOWN);
   }

   //***
   // store the sigma values of the adjustable parameters in the input
   // geometry file in Model_type. 
   //***
      for(i=0; i<numParams; i++)
      {
         double sigma = 0.0;
         fscanf(geomFile,"%lf %s", &sigma, desc);
         setParameterSigma(i, sigma, false);
      }

   //***
   // store the values of the adjustable parameters as well as the description 
   // of the parameters in the input geometry file in Model_type.
   // NOTE: Version 5 format implies the adjustable parameters are zero.
   //***
   if (version < 5)
   {
      for(i=0; i<numParams; i++)
      {
	fscanf(geomFile,"%lf %s", &param, desc);
	setParameterCenter(i, getParameterCenter(i) + param, false);
      }
   }
   
   //***
   // Allocate memory for all arrays:
   //***
   reallocateGrid(grid_size);
      
   //***
   // Read the enhanced coarse grid data --
   // First read the phi, lambda, dphi_dh, dlambda_dh coarse grid entries from 
   // the enhanced coarse grid file.
   //***
   fscanf(gridFile,"%s %s %s %s %s %s %s %s", desc, desc, desc, desc, 
          desc, desc, desc, desc);
   int intBuf;
   double lat, lon, dlat_dh, dlon_dh, dlat_dp, dlon_dp;
   for(int y=0; y<grid_size.y; y++)
   {
      for(int x=0; x<grid_size.x; x++)
      {
         fscanf(gridFile,"%d %d %lf %lf %lf %lf",
                &intBuf, &intBuf, &lat, &lon, &dlat_dh, &dlon_dh);
         theLatGrid.setNode(x, y, lat);
         theLonGrid.setNode(x, y, lon);
         theDlatDhGrid.setNode(x, y, dlat_dh);
         theDlonDhGrid.setNode(x, y, dlon_dh);
      }
   }

   //***
   // read dphi_dp and dlambda_dp together
   //***
   fscanf(gridFile,"%s %s %s %s %s %s %s\n", desc, desc, desc, desc, 
          desc, desc, desc);
   for(int p=0; p<numParams; p++)
   {
      for(int y=0; y<grid_size.y; y++)
      {
         for(int x=0; x<grid_size.x; x++)
         {
            //***
            // x=0, y=0 treated special:
            //***
            if ((x == 0) && (y == 0))
            {
               fscanf(gridFile,"%s %d %d %lf %lf",
                      desc, &intBuf, &intBuf, &dlat_dp, &dlon_dp);
            }
            else
            {
               fscanf(gridFile,"%d %d %lf %lf",
                      &intBuf, &intBuf, &dlat_dp, &dlon_dp);
            }

            theDlatDparamGrid[p].setNode(x, y, dlat_dp);
            theDlonDparamGrid[p].setNode(x, y, dlon_dp);
         }
      }
   }

   fclose (geomFile);
   fclose (gridFile);

   //***
   // Establish reference ground point as center of image:
   //***
   theRefGndPt.lat = theLatGrid(theRefImgPt);
   theRefGndPt.lon = theLonGrid(theRefImgPt);
   //***
   // Establish the GSD at image center:
   //***
   ossimDpt ref_ip_dx(theRefImgPt.x+1.0, theRefImgPt.y);
   ossimDpt ref_ip_dy(theRefImgPt.x, theRefImgPt.y+1.0);
   ossimGpt ref_gp_dx(theLatGrid(ref_ip_dx), theLonGrid(ref_ip_dx));
   ossimGpt ref_gp_dy(theLatGrid(ref_ip_dy), theLonGrid(ref_ip_dy));
   theGSD.x = theRefGndPt.distanceTo(ref_gp_dx);
   theGSD.y = theRefGndPt.distanceTo(ref_gp_dy);
   theMeanGSD = (theGSD.line + theGSD.samp)/2.0;
   
   theGSD.x = theMeanGSD;
   theGSD.y = theMeanGSD;

   //***
   // Initialize the bounging ground rectangle:
   //***
   ossimDpt v[4];
   v[0].lat = theLatGrid.getNode(0,0);
   v[0].lon = theLonGrid.getNode(0,0);
   v[1].lat = theLatGrid.getNode(grid_size.x-1,0);
   v[1].lon = theLonGrid.getNode(grid_size.x-1,0);
   v[2].lat = theLatGrid.getNode(grid_size.x-1,grid_size.y-1);
   v[2].lon = theLonGrid.getNode(grid_size.x-1,grid_size.y-1);
   v[3].lat = theLatGrid.getNode(0,grid_size.y-1);
   v[3].lon = theLonGrid.getNode(0,grid_size.y-1);
   theBoundGndPolygon = ossimPolygon(4, v);
   
   //***
   // Now save state and grid. First extract base name:
   //***
   ossimFilename path (ecgGeomName.path());
   ossimFilename base (ecgGeomName.fileNoExtension());
   ossimFilename ocgGeomName (path.dirCat(base) + DEFAULT_GEOM_FILE_EXT);
   theGridFilename = ossimFilename(path.dirCat(base) + DEFAULT_GRID_FILE_EXT);

   ossimString drivePart;
   ossimString pathPart;
   ossimString filePart;
   ossimString extPart;
   ecgGeomName.split(drivePart,
                     pathPart,
                     filePart,
                     extPart);
   
   ossimFilename ocgGeoName;
   ocgGeomName.merge(drivePart,
                    pathPart,
                    filePart+DEFAULT_GEOM_FILE_EXT,
                    "");
   theGridFilename.merge(drivePart,
                    pathPart,
                    filePart+DEFAULT_GRID_FILE_EXT,
                    "");

   getAdjustment(theInitialAdjustment);
   
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimCoarseGridModel::loadMetEcgGeom: returning..." << std::endl;
   return true;
}

//*****************************************************************************
// STATIC METHOD: ossimCoarseGridModel::writeGeomTemplate
//  
//  Writes a sample geometry KWL to the output stream.
//  
//*****************************************************************************
void ossimCoarseGridModel::writeGeomTemplate(ostream& os)
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimCoarseGridModel::writeGeomTemplate: entering..." << std::endl;

   os <<
      "//**************************************************************\n"
      "// Template for OCG model kewordlist\n"
      "//**************************************************************\n"
      << ossimKeywordNames::TYPE_KW << ": " << MODEL_TYPE << endl;

   ossimSensorModel::writeGeomTemplate(os);
   
   os << "//\n"
      << "// Derived-class ossimCoarseGridModel Keywords:\n"
      << "//\n"
      << GRID_FILE_NAME_KW << ": <string>\n" << endl;

   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimCoarseGridModel::writeGeomTemplate: returning..." << std::endl;
   return;
}
   

      
