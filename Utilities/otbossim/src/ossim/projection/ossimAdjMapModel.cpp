//----------------------------------------------------------------------------
// Copyright (c) 2005, Oscar Kramer, all rights reserved.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Oscar Kramer
//
// Description:
// 
// Special "sensor model" that provides a conventional map projection but
// with additional capability for adjusting the map x,y coordinates with
// offset, scale, and rotation transform.  Funtions to control notifications,
// information and error output.
//
//----------------------------------------------------------------------------
// $Id: ossimAdjMapModel.cpp 15766 2009-10-20 12:37:09Z gpotts $

#include <cstdlib> /* for atof */
#include <ossim/projection/ossimAdjMapModel.h>
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/projection/ossimMapProjectionFactory.h>
#include <ossim/elevation/ossimElevManager.h>

// Geometry File Keywords:
static const char* PARAMETER_KEYWORDS[] = {"map_offset_x",
                                           "map_offset_y",
                                           "map_rotation",
                                           "map_scale_x",
                                           "map_scale_y"};
static const char* MAP_PROJ_FILE_KW    = "map_proj_filename";

RTTI_DEF1(ossimAdjMapModel, "ossimAdjMapModel", ossimSensorModel);

//----------------------------------------------------------------------------
//! Constructs to uninitialized state (needed by factory).
//----------------------------------------------------------------------------

ossimAdjMapModel::ossimAdjMapModel()
   :
   ossimSensorModel(),
   theAdjParams(NUM_ADJ_PARAMS),
   theMapProjection(),
   theCosTheta(0.0),
   theSinTheta(0.0)
{
   theImageSize = ossimIpt(0,0);
   initAdjustableParameters();
   theErrorStatus++;
}

//----------------------------------------------------------------------------
//! Constructs with map projection and image rectangle size.
//----------------------------------------------------------------------------

ossimAdjMapModel::ossimAdjMapModel(ossimMapProjection* map_proj,
                                   const ossimIpt& image_size)
   :
   ossimSensorModel(),
   theAdjParams(NUM_ADJ_PARAMS),
   theMapProjection(map_proj),
   theCosTheta(0.0),
   theSinTheta(0.0)
{
   initializeFromMap();
   theImageSize = image_size;
   initAdjustableParameters();
}

//----------------------------------------------------------------------------
//! Copy constructor
//----------------------------------------------------------------------------

ossimAdjMapModel::ossimAdjMapModel(const ossimAdjMapModel& copy_this)
   :
   ossimSensorModel(copy_this),
   theAdjParams(NUM_ADJ_PARAMS),
   theMapProjection(copy_this.theMapProjection),
   theCosTheta(0.0),
   theSinTheta(0.0)
{
   theAdjParams = copy_this.theAdjParams;
}

//----------------------------------------------------------------------------
//! Constructs from Geom KWL
//----------------------------------------------------------------------------
ossimAdjMapModel::ossimAdjMapModel(const ossimKeywordlist& kwl,
                                   const char* prefix)
   :
   ossimSensorModel(kwl),
   theAdjParams(NUM_ADJ_PARAMS),
   theMapProjection(),
   theCosTheta(0.0),
   theSinTheta(0.0)      
                
{
   initAdjustableParameters();
   loadState(kwl, prefix);
}

//----------------------------------------------------------------------------
//! Constructs from Geom KWL
//----------------------------------------------------------------------------

ossimAdjMapModel::ossimAdjMapModel(const ossimFilename& kwl_filename)
   :
   ossimSensorModel(),
   theAdjParams(NUM_ADJ_PARAMS),
   theMapProjection(),
   theCosTheta(0.0),
   theSinTheta(0.0)      
{
   initAdjustableParameters();
   ossimKeywordlist kwl (kwl_filename);
   loadState(kwl);
}

//----------------------------------------------------------------------------
//! Initializes base class data members after map model established. Returns TRUE if all's well.
//----------------------------------------------------------------------------
bool ossimAdjMapModel::initializeFromMap()
{
   if (!theMapProjection)
   {
      theErrorStatus++;
      return false;
   }
   theSensorID = "AdjMapModel";
   theGSD = theMapProjection->getMetersPerPixel();
   theMeanGSD = 0.5*(theGSD.x + theGSD.y);
   theRefGndPt = theMapProjection->origin();

   return true;
}

//----------------------------------------------------------------------------
//! Destructor
//----------------------------------------------------------------------------
ossimAdjMapModel::~ossimAdjMapModel()
{
   theAdjParams.CleanUp();
}

//----------------------------------------------------------------------------
//! Initializes adjustable parameters to their default values.
//! Overrides ossimAdjustableParameterInterface virtual method.
//----------------------------------------------------------------------------
void ossimAdjMapModel::initAdjustableParameters()
{
   // Initialize this adjustment as the initial geometry "adjustment":
   removeAllAdjustments();
   newAdjustment(NUM_ADJ_PARAMS);
   setAdjustmentDescription("Initial Geometry");

   // Initialize each adjustable parameter for this initial:
   setAdjustableParameter (OFFSET_X, 0.0, 1.0);
   setParameterDescription(OFFSET_X, "map_offset_x");
   setParameterCenter     (OFFSET_X, 0.0);

   setAdjustableParameter (OFFSET_Y, 0.0, 1.0);
   setParameterDescription(OFFSET_Y, "map_offset_y");
   setParameterCenter     (OFFSET_Y, 0.0);

   setAdjustableParameter (ROTATION, 0.0, 1.0);
   setParameterDescription(ROTATION, "map_rotation");
   setParameterCenter     (ROTATION, 0.0);

   setAdjustableParameter (SCALE_X, 0.0, 1.0);
   setParameterDescription(SCALE_X, "map_scale_x");
   setParameterCenter     (SCALE_X, 1.0);

   setAdjustableParameter (SCALE_Y, 0.0, 1.0);
   setParameterDescription(SCALE_Y, "map_scale_y");
   setParameterCenter     (SCALE_Y, 1.0);

   updateModel();
}

//----------------------------------------------------------------------------
//! Following a change to the adjustable parameter set, this virtual is called
//! to permit instances to compute derived quantities after parameter change.
//----------------------------------------------------------------------------
void ossimAdjMapModel::updateModel()
{
   for (int i=0; i<NUM_ADJ_PARAMS; i++)
      theAdjParams[i] = computeParameterOffset(i);

   theCosTheta = ossim::cosd(theAdjParams[ROTATION]);
   theSinTheta = ossim::sind(theAdjParams[ROTATION]);
}

//----------------------------------------------------------------------------
//! Overrides base class pure virtual.
//----------------------------------------------------------------------------
void ossimAdjMapModel::lineSampleHeightToWorld(const ossimDpt& image_point,
                                     const double&   heightEllipsoid,
                                     ossimGpt&       worldPoint) const
{
   // Just call other transform method 
   // Check for bad map projection pointer:
   if (!theMapProjection)
   {
      worldPoint = ossimGpt(ossim::nan(), ossim::nan(), ossim::nan());
      return;
   }

   // Adjust image point by transform:
   double x = theAdjParams[SCALE_X]*(image_point.x - theAdjParams[OFFSET_X]);
   double y = theAdjParams[SCALE_Y]*(image_point.y - theAdjParams[OFFSET_Y]);
   ossimDpt adjusted_point(x*theCosTheta + y*theSinTheta, 
                           y*theCosTheta - x*theSinTheta);

   // Obtain ground point given adjusted image point:
   theMapProjection->lineSampleHeightToWorld(adjusted_point,
                                             heightEllipsoid,
                                             worldPoint);
   worldPoint.height(heightEllipsoid);
}

//----------------------------------------------------------------------------
//! Overrides base class virtual.
//----------------------------------------------------------------------------
void ossimAdjMapModel::lineSampleToWorld(const ossimDpt& image_point,
                                         ossimGpt&       worldPoint) const
{
   lineSampleHeightToWorld(image_point, 0.0, worldPoint);
   if (!worldPoint.hasNans())
   {
//      worldPoint.height(theElevation->getHeightAboveEllipsoid(worldPoint));
   }
}

//----------------------------------------------------------------------------
//! Rigorous inverse transform implented, overrides base-class' iterative
//! solution.
//----------------------------------------------------------------------------
void ossimAdjMapModel::worldToLineSample(const ossimGpt& world_point,
                                         ossimDpt&       image_point) const
{
   // Check for bad map projection pointer:
   if (!theMapProjection)
   {
      image_point = ossimDpt(ossim::nan(), ossim::nan());
      return;
   }

   // Obtain non-adjusted image point given ground point:
   ossimDpt p1;
   theMapProjection->worldToLineSample(world_point, p1);

   // Adjust image point by transform:
   ossimDpt p2 (p1.x*theCosTheta - p1.y*theSinTheta, 
                p1.y*theCosTheta + p1.x*theSinTheta);
   image_point.x = p2.x/theAdjParams[SCALE_X] + theAdjParams[OFFSET_X];
   image_point.y = p2.y/theAdjParams[SCALE_Y] + theAdjParams[OFFSET_Y];
}

//----------------------------------------------------------------------------
//! Fulfills ossimObject base-class pure virtuals. Saves geometry
//! KWL files. Returns true if successful.
//----------------------------------------------------------------------------
bool ossimAdjMapModel::saveState(ossimKeywordlist& kwl, const char* prefix) const
{
   kwl.add(prefix, ossimKeywordNames::TYPE_KW, "ossimAdjMapModel");

   // Hand off to base class for common stuff:
   ossimSensorModel::saveState(kwl, prefix);

   // Save off data members:
   for (int i=0; i<NUM_ADJ_PARAMS; i++)
      kwl.add(prefix, PARAMETER_KEYWORDS[i], theAdjParams[i]);

   // Save off map projection info:
   if (theMapProjection.valid())
   {
      theMapProjection->saveState(kwl, prefix);
   }

   return true;
}

//----------------------------------------------------------------------------
//! Fulfills ossimObject base-class pure virtuals. Loads geometry
//! KWL files. Returns true if successful.
//----------------------------------------------------------------------------
bool ossimAdjMapModel::loadState(const ossimKeywordlist& kwl, const char* prefix)
{
   static const ossimString MODULE ("ossimAdjMapModel::loadState() -- ");
   bool success = true;
   const char* value_str;
   double value;
   ossimString error_msg (MODULE+"Error encountered reading keyword: ");

   try
   {
      // Verify correct model type:
      value_str = kwl.find(prefix, ossimKeywordNames::TYPE_KW);
      if ((!value_str) || (strcmp(value_str, TYPE_NAME(this))))
      {
         throw (error_msg + ossimKeywordNames::TYPE_KW);
      }

      //---
      // Instantiate the map projection via the factory if one has not been
      // initialized yet:
      //---
      if (!theMapProjection)
      {
         // should find the map projection filename in the KWL:
         value_str = kwl.find(prefix, MAP_PROJ_FILE_KW);
         if (!value_str)
         {
            throw (error_msg + MAP_PROJ_FILE_KW);
         }
         theMapProjection = PTR_CAST(ossimMapProjection,
            ossimMapProjectionFactory::instance()->createProjection(ossimKeywordlist(value_str)));
         if (!theMapProjection)
         {
            throw "Error encountered instantiating map ";
         }
      }

      // Everything OK so far, just load in the adjustable parameters. This involves modifying the
      // center value of the bsae class adjustable parameter:
      for (int i=0; i<NUM_ADJ_PARAMS; i++)
      {
         value_str = kwl.find(prefix, PARAMETER_KEYWORDS[i]);
         if (!value_str)
         {
            throw (error_msg + PARAMETER_KEYWORDS[i]).chars();
         }
         value = atof(value_str);
         setParameterCenter(i, value);
      }
   }
   catch (const char* /*message*/)
   {
      theErrorStatus++;
      success = false;
   }

   //---
   // updateModel() reads the base class adjustable parameters and initializes
   // theAdjParams
   // with the unnormalized, biased (to the "center" set above) quantities
   // used in the projection:
   //---
   if (success)
   {
      initializeFromMap();
      updateModel();
   }

   return success;
}  

//----------------------------------------------------------------------------
//! Writes a template of geom keywords processed by loadState and saveState to
//! output stream.
//----------------------------------------------------------------------------
void ossimAdjMapModel::writeGeomTemplate(ostream& os)
{
   os <<
      "//*****************************************************************\n"
      "// Template for Adjustable Map Model keywordlist\n"
      "//*****************************************************************\n"
      << ossimKeywordNames::TYPE_KW << ": " << "ossimFcsiModel" << endl;

   ossimSensorModel::writeGeomTemplate(os);

   os << "//***\n"
      << "// Derived-class AdjMapModel Keywords:\n"
      << "//***\n"
      << PARAMETER_KEYWORDS[OFFSET_X] << ": <east offset meters>\n"
      << PARAMETER_KEYWORDS[OFFSET_Y] << ": <north offset meters>\n"
      << PARAMETER_KEYWORDS[ROTATION] << ": <degrees CW>\n"
      << PARAMETER_KEYWORDS[SCALE_X]  << ": <ratio>\n"
      << PARAMETER_KEYWORDS[SCALE_Y]  << ": <ratio>\n"
      << endl;

   return;
}

//----------------------------------------------------------------------------
//! Extends base-class implementation. Dumps contents of object to ostream.
//----------------------------------------------------------------------------
std::ostream& ossimAdjMapModel::print(std::ostream& out) const
{      
   ossimSensorModel::print(out);
   for (int i=0; i<NUM_ADJ_PARAMS; i++)
      out << PARAMETER_KEYWORDS[i]  << ": " << theAdjParams[i] << endl;
   out << endl;

   return out;
}

//----------------------------------------------------------------------------
//! Fetches the GSD from the internal map projection.
//----------------------------------------------------------------------------
ossimDpt ossimAdjMapModel::getMetersPerPixel () const
{
   if (theMapProjection.valid())
   {
      return theMapProjection->getMetersPerPixel();
   }
   
   return ossimDpt(0,0);
}

ossimObject* ossimAdjMapModel::dup() const
{
   return new ossimAdjMapModel(*this);
}

