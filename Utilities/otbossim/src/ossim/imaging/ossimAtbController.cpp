//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// AUTHOR: Oscar Kramer (okramer@imagelinks.com)
//
// DESCRIPTION: Contains implementation of class ossimAtbController. This is the
//    controller managing the tonal balancing process. It's product is the
//    initialization of specialized remappers in each of the mosaic's member
//    images that results in a tonally balanced mosaic.
//
// LIMITATIONS: None.
//
//*****************************************************************************
//  $Id: ossimAtbController.cpp 17195 2010-04-23 17:32:18Z dburken $

#include <ossim/imaging/ossimAtbController.h>

RTTI_DEF2(ossimAtbController, "ossimAtbController", ossimProcessInterface, ossimConnectableObject );

#include <stdio.h>
#include <ossim/imaging/ossimGridRemapEngineFactory.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/imaging/ossimAtbMatchPoint.h>
#include <ossim/imaging/ossimGridRemapSource.h>
#include <ossim/base/ossimConnectableContainer.h>
#include <ossim/imaging/ossimImageCombiner.h>

//***
// Define Trace flags for use within this file:
//***
#include <ossim/base/ossimTrace.h>
static ossimTrace traceExec  ("ossimAtbController:exec");
static ossimTrace traceDebug ("ossimAtbController:debug");

static const ossimIpt DEFAULT_GRID_SPACING (64,64);
static const char* DEFAULT_ATB_REMAP_ENGINE = "ossimHsvGridRemapEngine";

//*****************************************************************************
//  CONSTRUCTOR:  ossimAtbController()
//   
//*****************************************************************************
ossimAtbController::ossimAtbController()
   :
      ossimConnectableObject(NULL, 0, 0, false, true),
      theContainer (0),
      theGridRemapEngine (0),
      theGridSpacing (DEFAULT_GRID_SPACING)
{
   static const char MODULE[] = "ossimAtbController Default Constructor";
   if (traceExec())  CLOG << "entering..." << endl;
   
   //***
   // Instantiate a default remap engine:
   //***
   theGridRemapEngine
     = ossimGridRemapEngineFactory::create(DEFAULT_ATB_REMAP_ENGINE);
   
   if (traceExec())  CLOG << "returning..." << endl;
}

//*****************************************************************************
//  CONSTRUCTOR:  ossimAtbController()
//   
//*****************************************************************************
ossimAtbController::ossimAtbController(ossimImageCombiner* combiner,
                                       ossimGridRemapEngine* engine)
   :
      ossimConnectableObject(NULL, 0, 0, false, true),
      theContainer (0),
      theGridRemapEngine (0),
      theGridSpacing (DEFAULT_GRID_SPACING)
{
   static const char MODULE[] = "ossimAtbController Constructor #1";
   if (traceExec())  CLOG << "entering..." << endl;
   
   initializeWithCombiner(combiner, engine);
   
   if (traceExec())  CLOG << "returning..." << endl;
}

//*****************************************************************************
//  CONSTRUCTOR:  ossimAtbController()
//   
//*****************************************************************************
ossimAtbController::ossimAtbController(ossimGridRemapEngine* engine)
   :
      theContainer (0),
      theGridRemapEngine (0),
      theGridSpacing (DEFAULT_GRID_SPACING)
{
   static const char MODULE[] = "ossimAtbController Constructor #1";
   if (traceExec())  CLOG << "entering..." << endl;
   
   //***
   // Assign default engine if non provided:
   //***
   if (engine)
   {
      theGridRemapEngine = engine;
   }
   else
   {
      theGridRemapEngine
      = ossimGridRemapEngineFactory::create(DEFAULT_ATB_REMAP_ENGINE);
   }
   
   if (traceExec())  CLOG << "returning..." << endl;
}

//*****************************************************************************
//  DESTRUCTOR: ~ossimAtbController()
//
//  The remappers are left behind, owned by the combiner's container. If the
//  combiner was owned by something other than a container (see initialize()),
//  then this controller will assume ownership of the allocated remappers and
//  delete them here.
//
//*****************************************************************************
ossimAtbController::~ossimAtbController()
{
   theContainer = 0;
   theGridRemapEngine = 0;;
}

//*****************************************************************************
//  METHOD: ossimAtbController::initializeWithCombiner()
//  
//*****************************************************************************
void ossimAtbController::initializeWithCombiner(ossimImageCombiner* combiner,
						ossimGridRemapEngine* engine)
{
   static const char MODULE[] = "ossimAtbController::initializeWithCombiner()";
   if (traceExec())  CLOG << "entering..." << endl;
   
   //***
   // Assign default engine if non provided:
   //***
   if (engine)
      theGridRemapEngine = (ossimGridRemapEngine*)engine->dup();
   else
      theGridRemapEngine
         = ossimGridRemapEngineFactory::create(DEFAULT_ATB_REMAP_ENGINE);
   
   //***
   // This is the first combiner initializing the controller. The combiner
   // will have all of the input sources connected already. We attach to
   // these:
   //***
   theContainer = PTR_CAST(ossimConnectableContainer, combiner->getOwner());
   
   //***
   // Need to scan the input connections of the combiner and insert a 
   // remapper if there is not one already at the end of the input chain:
   //***
   vector<ossimConnectableObject*> del_list;
   vector<ossimConnectableObject*> add_list;
   int numInputs = combiner->getNumberOfInputs();
   for(int i=0; i<numInputs; i++)
   {
      ossimImageSource* input_image = PTR_CAST(ossimImageSource,
                                               combiner->getInput(i));
      
      if (input_image)
      {
         ossimGridRemapSource* remapper = PTR_CAST(ossimGridRemapSource,
                                                   input_image);
         if (remapper)
         {
            //***
            // Remapper already present, set its remap engine to this
            // controller's remap engine:
            //***
            remapper->setRemapEngine(theGridRemapEngine);
         }
         
         else
         {
            //***
            // Need to instantiate remapper:
            //***
            remapper = new ossimGridRemapSource(input_image,
                                                theGridRemapEngine);
            if (theContainer.valid())
               theContainer->addChild(remapper);
            
            add_list.push_back(remapper);
            del_list.push_back(input_image);
         }

         connectMyInputTo(remapper);
      }            
   }
   
   //***
   // Need to delete the old inputs and add the new remappers to the
   // combiner's input list:
   //***
   vector<ossimConnectableObject*>::iterator add_iter = add_list.begin();
   vector<ossimConnectableObject*>::iterator del_iter = del_list.begin();
   while (add_iter != add_list.end())
   {
      (*add_iter)->connectMyInputTo(*del_iter);
      combiner->disconnect(*del_iter);
      combiner->connectMyInputTo(*add_iter);
      add_iter++;
      del_iter++;
   }
   combiner->initialize();
      
   if (traceExec())  CLOG << "returning..." << endl;
}

//*****************************************************************************
//  METHOD: ossimAtbController::enableImage()
//  
//  Enable an image in the mosaic to be included the tonal balancing process.
//  
//*****************************************************************************
bool ossimAtbController::enableImage(unsigned int index)
{
   if (index >= getNumberOfInputs())
      return false;

   ossimSource* remapper = PTR_CAST(ossimSource,
                                    getInput(index));
   if (remapper)
      remapper->enableSource();
                                    
   return true;
}

//*****************************************************************************
//  METHOD: ossimAtbController::disableImage()
//  
//  Disable an image in the mosaic from the tonal balancing process.
//  
//*****************************************************************************
bool ossimAtbController::disableImage(unsigned int index)
{
   if (index >= getNumberOfInputs())
      return false;

   ossimSource* remapper = PTR_CAST(ossimSource,
                                    getInput(index));
   if (remapper)
      remapper->disableSource();

   return true;
}

//*****************************************************************************
//  METHOD: ossimAtbController::lockImage()
//  
//  Locks a particular source for adjustment. A locked image
//  effectively defines the target values for all overlapping imagery.
//  
//*****************************************************************************
bool ossimAtbController::lockImage(unsigned int index)
{
   if (index >= getNumberOfInputs())
      return false;
   
   ossimGridRemapSource* remapper = PTR_CAST(ossimGridRemapSource,
                                              getInput(index));
   if (remapper)
      remapper->lock();

   return true;
}


//*****************************************************************************
//  METHOD: ossimAtbController::unlockImage()
//  
//  Unlocks a particular source for allowing its adjustment.
//  
//*****************************************************************************
bool ossimAtbController::unlockImage(unsigned int index)
{
   if (index >= getNumberOfInputs())
      return false;
   
   ossimGridRemapSource* remapper = PTR_CAST(ossimGridRemapSource,
                                              getInput(index));
   if (remapper)
      remapper->lock();

   return true;
}


//*****************************************************************************
//  METHOD: ossimAtbController::execute()
//  
//*****************************************************************************
bool ossimAtbController::execute()
{
   static const char MODULE[] = "ossimAtbController::execute()";
   if (traceExec())  CLOG << "entering..." << endl;
   
   setPercentComplete(0.0);

   ossimGridRemapSource* source;

   //***
   // First verify that all objects needed are initialized:
   //***
   int num_images = getNumberOfInputs();
   if ((!theGridRemapEngine) || (num_images < 2))
     {
     CLOG << "WARNING: execute method was called but the controller has not "
           << "been properly initialized. Ignoring request..." << endl;
      if (traceExec())  CLOG << "returning..." << endl;
      return false;
   }
   
   //***
   // establish the remap grids:
   //***
   initializeRemappers();
   setPercentComplete(10.0);
   
   //***
   // Establish grid of matchpoints:
   //***
   int numPoints = 0;
   ossimDpt mp_view_pt;
   theMatchPoints.clear();
   for (mp_view_pt.line  = theBoundingRect.ul().line;
        mp_view_pt.line <= theBoundingRect.lr().line;
        mp_view_pt.line += theGridSpacing.line)
   {
      for (mp_view_pt.samp  = theBoundingRect.ul().samp;
           mp_view_pt.samp <= theBoundingRect.lr().samp;
           mp_view_pt.samp += theGridSpacing.samp)
      {
         ossimAtbMatchPoint* mp = new ossimAtbMatchPoint(mp_view_pt,
                                                         theGridRemapEngine);
         theMatchPoints.push_back(mp);
         numPoints++;
      }
   }
   
   //***
   // Loop over each matchpoint to determine which images contribute statistics:
   //***
   setPercentComplete(20.0);
   for (int mp=0; mp<numPoints; mp++)
   {
      ossimDpt view_point (theMatchPoints[mp]->viewPoint());
      
      //***
      // Loop over each image source to check if point lies inside its bounding
      // rectangle, and add its contribution to the matchpoint's collection:
      //***
      for (int src=0; src<num_images; src++)
      {
         source = PTR_CAST(ossimGridRemapSource, getInput(src));
         if (source)
         {
            ossimDrect image_rect (source->getBoundingRect());
            if (image_rect.pointWithin(view_point))
               theMatchPoints[mp]->addImage(source);
         }
      }

      setPercentComplete(20.0 + 50.0*(mp/numPoints));
   }

   //***
   // All contributors have been included in all matchpoints' collections.
   // Compute the target pixel value for each matchpoint and communicate it
   // to the corresponding remappers:
   //***
   for (int mp=0; mp<numPoints; mp++)
   {
      setPercentComplete(70.0 + 30.0*(mp/numPoints));
      theMatchPoints[mp]->assignRemapValues();
   }

   //***
   // Finally, enable the remappers:
   //***
   for (int src=0; src<num_images; src++)
   {
      source = PTR_CAST(ossimGridRemapSource, getInput(src));
      if (source)
         source->enableSource();
   }
   setPercentComplete(100.0);
         
   if (traceExec())  CLOG << "returning..." << endl;
   return true;
}

//*****************************************************************************
//  METHOD: ossimAtbController::abort()
//  
//*****************************************************************************
void ossimAtbController::abort()
{
   static const char MODULE[] = "ossimAtbController::abort()";
   if (traceExec())  CLOG << "entering..." << endl;
   
   CLOG << "NOTICE: this method not yet implemented." << endl;
   
   if (traceExec())  CLOG << "returning..." << endl;
}

//*****************************************************************************
//  METHOD: ossimAtbController::saveState()
//  
//*****************************************************************************
bool ossimAtbController::saveState(ossimKeywordlist& /* kwl */) const
{
   static const char MODULE[] = "ossimAtbController::saveState()";
   if (traceExec())  CLOG << "entering..." << endl;
   
   CLOG << "NOTICE: this method not yet implemented." << endl;
   
   if (traceExec())  CLOG << "returning..." << endl;
   return true;
}

//*****************************************************************************
//  METHOD: ossimAtbController::loadState()
//  
//*****************************************************************************
bool ossimAtbController::loadState(const ossimKeywordlist& /* kwl */)
{
   static const char MODULE[] = "ossimAtbController::loadState()";
   if (traceExec())  CLOG << "entering..." << endl;
   
   CLOG << "NOTICE: this method not yet implemented." << endl;
   
   if (traceExec())  CLOG << "returning..." << endl;
   return true;
}


//*****************************************************************************
//  METHOD:  ossimAtbController::setKernelSize(N)
//  
//  Hook to set the size of the kernel used by all point sources in computing
//  their mean pixel value. The kernels will be resized to NxN.
//  
//*****************************************************************************
void ossimAtbController::setKernelSize(int side_size)
{
   static const char MODULE[] = "ossimAtbController::setKernelSize(N)";
   if (traceExec())  CLOG << "entering..." << endl;

   vector<ossimRefPtr<ossimAtbMatchPoint> >::iterator mpi = theMatchPoints.begin();
   while (mpi != theMatchPoints.end())
      (*mpi)->setKernelSize(side_size);

   if (traceExec())  CLOG << "returning..." << endl;
   return;
}


//*****************************************************************************
//  PRIVATE METHOD:  ossimAtbController::initializeRemappers()
//  
//*****************************************************************************
void ossimAtbController::initializeRemappers()
{
   static const char MODULE[] = "ossimAtbController::initializeRemappers()";
   if (traceExec())  CLOG << "entering..." << endl;

   int num_images = getNumberOfInputs();

   //***
   // Fetch the overall mosaic bounding rect:
   //***
   ossimGridRemapSource* remapper = PTR_CAST(ossimGridRemapSource,
                                             getInput(0));
   theBoundingRect = remapper->getBoundingRect();
   for (int src=1; src<num_images; src++)
   {
      theBoundingRect = theBoundingRect.combine(remapper->getBoundingRect());
   }
 
   //***
   // Loop over each contribution image source chain to fetch each image
   // bounding rect:
   //***
   double dx_ul, dy_ul, dx_lr, dy_lr;
   double grid_ul_x, grid_ul_y, grid_lr_x, grid_lr_y;
   for (int src=0; src<num_images; src++)
   {
      remapper = PTR_CAST(ossimGridRemapSource, getInput(src));
      ossimDrect image_rect (remapper->getBoundingRect());

      //***
      // Arrive at the number of grid posts from mosaic origin to image's grid
      // UL and LR corners:
      //***
      dx_ul = (image_rect.ul().x - theBoundingRect.ul().x)/theGridSpacing.x;
      dy_ul = (image_rect.ul().y - theBoundingRect.ul().y)/theGridSpacing.y;
      dx_lr = (image_rect.lr().x - theBoundingRect.lr().x)/theGridSpacing.x;
      dy_lr = (image_rect.lr().y - theBoundingRect.lr().y)/theGridSpacing.y;
   
      //***
      // Establish the view coordinates for the adjusted image grid:
      //***
      grid_ul_x =  theBoundingRect.ul().x + ceil(dx_ul)*theGridSpacing.x;
      grid_ul_y =  theBoundingRect.ul().y + ceil(dy_ul)*theGridSpacing.y;
      grid_lr_x =  theBoundingRect.lr().x + floor(dx_lr)*theGridSpacing.x;
      grid_lr_y =  theBoundingRect.lr().y + floor(dy_lr)*theGridSpacing.y;

      //***
      // Have the remapper initialize a new grid with the given rectangle and
      // spacing:
      //***
      ossimDrect grid_rect (grid_ul_x, grid_ul_y, grid_lr_x, grid_lr_y);
      remapper->initialize(grid_rect, theGridSpacing);
   }
      
   if (traceExec())  CLOG << "returning..." << endl;
   return;
}

//*****************************************************************************
//  METHOD: ossimAtbController::setGridRemapEngine()
//  
//*****************************************************************************
void ossimAtbController::setGridRemapEngine(ossimGridRemapEngine* engine)
{
   static const char MODULE[] = "ossimAtbController::setGridRemapEngine()";
   if (traceExec())  CLOG << "entering..." << endl;

   theGridRemapEngine = engine;

   //***
   // Communicate the new engine to all member's interested:
   //***
   ossimGridRemapSource* remapper;
   ossimConnectableObject::ConnectableObjectList::iterator iter=theInputObjectList.begin();
   while (iter != theInputObjectList.end())
   {
      remapper = PTR_CAST(ossimGridRemapSource, (*iter).get());
      remapper->setRemapEngine(engine);
      iter++;
   }

   vector<ossimRefPtr<ossimAtbMatchPoint> >::iterator match_point = theMatchPoints.begin();
   while (match_point != theMatchPoints.end())
   {
      (*match_point)->setGridRemapEngine(engine);
      match_point++;
   }

   if (traceExec())  CLOG << "returning..." << endl;
   return;
}


//*****************************************************************************
//  METHOD: ossimAtbController::canConnectMyInputTo()
//  
//*****************************************************************************
bool ossimAtbController::canConnectMyInputTo(ossim_int32 /* myInputIndex */,
                                             const ossimConnectableObject* object) const
{
   return (object&& PTR_CAST(ossimGridRemapSource, object));
}
