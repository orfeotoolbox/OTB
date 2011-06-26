//*****************************************************************************
// FILE: ossimGridRemapper.cc
//
// License:  See top level LICENSE.txt file.
//
// AUTHOR: Oscar Kramer
//
// DESCRIPTION: Contains implementation of class ossimGridRemapper. This is
//   a spacially variant remapper that utilizes a grid for interpolating the
//   remap value given an image x, y. 
//
// LIMITATIONS: None.
//
//*****************************************************************************
//  $Id: ossimGridRemapSource.cpp 19682 2011-05-31 14:21:20Z dburken $

#include <ossim/imaging/ossimGridRemapSource.h>

RTTI_DEF1(ossimGridRemapSource,
          "ossimGridRemapSource",
          ossimImageSourceFilter);

#include <ossim/imaging/ossimImageData.h>
#include <ossim/base/ossimDrect.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimDblGrid.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/imaging/ossimGridRemapEngineFactory.h>
#include <cstdio>
#include <fstream>


//***
// Define Trace flags for use within this file:
//***
#include <ossim/base/ossimTrace.h>
static ossimTrace traceExec  ("ossimGridRemapSource:exec");
static ossimTrace traceDebug ("ossimGridRemapSource:debug");

static const char* GRID_FILENAME_KW        = "grid_remap_file";
static const char* REMAP_ENGINE_KW         = "remap_engine";

//*****************************************************************************
//  DEFAULT CONSTRUCTOR #1: ossimGridRemapper()
//  
//*****************************************************************************
ossimGridRemapSource::ossimGridRemapSource()
   :
      ossimImageSourceFilter(),
      theGridFilename (0),
      theRemapEngine (0),
      theRemapIsLockedFlag(true),
      theGridIsFilled(false)
{
   disableSource();
}

//*****************************************************************************
//  CONSTRUCTOR #2: ossimGridRemapper(inputSource)
//  
//*****************************************************************************
ossimGridRemapSource::ossimGridRemapSource(ossimImageSource* inputSource,
                                           ossimGridRemapEngine* engine)
   :
      ossimImageSourceFilter(inputSource),
      theGridFilename (0),
      theRemapEngine ((ossimGridRemapEngine*)engine->dup()),
      theRemapIsLockedFlag(true),
      theGridIsFilled(false)
{
   disableSource();
}

//*****************************************************************************
//  DESTRUCTOR: ~ossimGridRemapSource
//  
//*****************************************************************************
ossimGridRemapSource::~ossimGridRemapSource()
{
   deallocateGrids();
   theRemapEngine = 0;
}

//*****************************************************************************
//  METHOD: ossimGridRemapSource::initialize
//  
//*****************************************************************************
void ossimGridRemapSource::initialize(const ossimDrect& uv_rect,
                                      const ossimDpt&   grid_spacing)
{
   deallocateGrids();
   int num_params;
   
   if (theRemapEngine)
   {
      num_params = theRemapEngine->getNumberOfParams();
      
      for (int p=0; p<num_params; p++)
      {
         ossimDblGrid* grid = new ossimDblGrid(uv_rect, grid_spacing);
         theGrids.push_back(grid);
      }
      
      theGridFilename = "ogrs";
      theGridFilename += ossimString::toString(reinterpret_cast<ossim_int64>(this));
      theGridFilename += ".org";

      theGridIsFilled = false;
   }
}

//*****************************************************************************
//  METHOD: ossimGridRemapSource::getTile()
//  
//  Implementation of virtual method to return remapped tile.
//  
//*****************************************************************************
ossimRefPtr<ossimImageData> ossimGridRemapSource::getTile(
   const ossimIrect& rect, ossim_uint32 resLevel)
{
   if(!theInputConnection)
      return NULL;

   //***
   // Fetch tile from input source:
   //***
   ossimRefPtr<ossimImageData> tile = theInputConnection->getTile(rect,
                                                                  resLevel);

   //---
   // Bypass this filter if it is not initialized, or if input tile is bogus:
   //---
   if ((!isSourceEnabled()) || (!tile.valid()) || (!theRemapEngine) ||
       (tile->getDataObjectStatus()==OSSIM_NULL) ||
       (tile->getDataObjectStatus()==OSSIM_EMPTY))
   {
      return tile;
   }
   
   //***
   // Insure that the grid has been filled in case nodes were being randomly set
   //***
   if (!theGridIsFilled)
   {
      vector<ossimDblGrid*>::iterator grid = theGrids.begin();
      while (grid != theGrids.end())
      {
//         (*grid)->setInterpolationType(ossimDblGrid::BILINEAR);
         (*grid)->interpolateNullValuedNodes();
         grid++;
      }
      theGridIsFilled = true;
   }
   
   //***
   // hand off the actual remap to the ATB engine. This object knows how to
   // interpret the grids for the particular remap algorithm selected:
   //***
   theRemapEngine->remapTile(rect.ul(), this, tile);
   
   tile->validate();
   return tile;
}

//*****************************************************************************
//  METHOD: ossimGridRemapSource::loadState()
//  
//*****************************************************************************
bool ossimGridRemapSource::loadState(const ossimKeywordlist& kwl,
                                     const char* prefix)
{
   static const char MODULE[] = "ossimGridRemapSource::loadState()";
   if (traceExec())  CLOG << "entering..." << endl;
   
   bool successful = false;
   const char* value;

   //***
   // Reset object in preparation for reassignment:
   //***
   deallocateGrids();
   theRemapEngine = 0;
   
   //***
   // Read the remap engine type:
   //***
   value = kwl.find(prefix, REMAP_ENGINE_KW);
   theRemapEngine = ossimGridRemapEngineFactory::create(value);
   if (!theRemapEngine)
   {
//      CLOG << "ERROR: could not instantiate remap engine. Aborting..." << endl;
//      if (traceExec())  CLOG << "returning..." << endl;
//      return false;
   }
   
   //***
   // Read the grid filename and open input stream:
   //***
   theGridIsFilled = false;
   value = kwl.find(prefix, GRID_FILENAME_KW);
   if (value)
   {
      theGridFilename = value;
      ifstream is (theGridFilename.chars());
      
      //***
      // Create an input stream from the grid file to pass to the
      // corresponding grid:
      //***
      int num_grids = theRemapEngine->getNumberOfParams();
      for (int i=0; (i<num_grids) && successful; i++)
      {
         ossimDblGrid* grid = new ossimDblGrid;
         successful = grid->load(is);
         if (successful)
            theGrids.push_back(grid);
      }
      if (!successful)
      {
         CLOG << "ERROR: Encountered errorloading remap grids at file: "
              << theGridFilename << ". Remapper disabled." << endl;
         if (traceExec())  CLOG << "returning..." << endl;
         return false;
      }
      theGridIsFilled = true;
   }
   
   //***
   // Allow base class to parse list:
   //***
   bool rtn_stat = ossimImageSourceFilter::loadState(kwl, prefix);

   //***
   // Enable source only if KWL read was successful:
   //***
   if (theRemapEngine && successful && rtn_stat)
      enableSource();

   if (traceExec())  CLOG << "returning..." << endl;
   return rtn_stat;
}

//*****************************************************************************
//  METHOD: ossimGridRemapSource::saveState()
//  
//*****************************************************************************
bool ossimGridRemapSource::saveState(ossimKeywordlist& kwl,
                                     const char*       prefix) const
{
   static const char MODULE[] = "ossimGridRemapSource::saveState()";

   //***
   // No filename indicates that this remapper has not been initialized:
   //***
   if (!theGridFilename)
      return false;
   
   //***
   // Write the remap engine type:
   //***
   if (theRemapEngine)
      kwl.add(prefix, REMAP_ENGINE_KW, theRemapEngine->getClassName());

   //***
   // Write the filename to the KWL:
   //***
   if (!theGridFilename.empty())
   {
      kwl.add(prefix, GRID_FILENAME_KW, theGridFilename);

      //***
      // Loop over each remap component to write out the grid:
      //***
      ofstream os (theGridFilename.c_str());
      bool successful = true;
      int num_components = theRemapEngine->getNumberOfParams();
      for (int p=0; (p<num_components)&&successful; p++)
         successful = theGrids[p]->save(os, "Remap-Grid");

      if (!successful)
      {
         CLOG << "ERROR: Encountered saving remap grids to file: "
              << theGridFilename << ". State not properly saved."
              << endl;
         return false;
      }
   }
   
   return ossimImageSourceFilter::saveState(kwl, prefix);   
}

//*****************************************************************************
//  METHOD: ossimGridRemapSource::setGridNode()
//  
//  Sets a node of the member grid to the value specified.
//  
//*****************************************************************************
void ossimGridRemapSource::setGridNode(const ossimDpt& view_pt,
                                       const double*   value)
{
   int numGrids = (int)theGrids.size();
   for (int i=0; i<numGrids; i++)
      theGrids[i]->setNearestNode(view_pt, value[i]);

   theGridIsFilled = false;
   
   return;
}

//*****************************************************************************
//  METHOD: ossimGridRemapSource::getGrid()
//  
//*****************************************************************************
ossimDblGrid* ossimGridRemapSource::getGrid(unsigned int index)
{
   if (index >= theGrids.size())
      return 0;

   return theGrids[index];
}
   
//*****************************************************************************
//  METHOD: ossimGridRemapSource::deallocateGrids()
//  
//*****************************************************************************
void ossimGridRemapSource::deallocateGrids()
{
   static const char MODULE[] = "ossimGridRemapSource::deallocateMemory()";
   if (traceExec())  CLOG << "entering..." << endl;

   vector<ossimDblGrid*>::iterator grid = theGrids.begin();
   while (grid != theGrids.end())
   {
      delete *grid;
      grid++;
   }
   theGrids.clear();

   if (traceExec())  CLOG << "returning..." << endl;
   return;
}


//*****************************************************************************
//  METHOD: ossimGridRemapSource::setRemapEngine()
//  
//*****************************************************************************
void ossimGridRemapSource::setRemapEngine(ossimGridRemapEngine* engine)
{

   theRemapEngine = engine;
   theGridIsFilled = false;
}

//*****************************************************************************
//  METHOD: ossimGridRemapSource::setGridFilename()
//  
//*****************************************************************************
void ossimGridRemapSource::setGridFilename(const ossimFilename& grid_filename)
{
   theGridFilename = grid_filename;
   //***
   // NOT COMPLETE###
   //***
}
