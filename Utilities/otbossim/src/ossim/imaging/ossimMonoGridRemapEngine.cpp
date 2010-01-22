//*****************************************************************************
// FILE: ossimMonoGridRemapEngine.cc
//
// Copyright (C) 2001 ImageLinks, Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// See the GPL in the COPYING.GPL file for more details.
//
// AUTHOR: Oscar Kramer
//
// DESCRIPTION: Contains implementation of class 
//
// LIMITATIONS: None.
//
//*****************************************************************************
//  $Id: ossimMonoGridRemapEngine.cpp 15833 2009-10-29 01:41:53Z eshirschorn $

#include <ossim/imaging/ossimMonoGridRemapEngine.h>

RTTI_DEF1(ossimMonoGridRemapEngine, "ossimMonoGridRemapEngine",
          ossimGridRemapEngine);

#include <ossim/imaging/ossimGridRemapSource.h>
#include <ossim/imaging/ossimAtbPointSource.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimDblGrid.h>
#include <ossim/imaging/ossimImageData.h>

//***
// Define Trace flags for use within this file:
//***
#include <ossim/base/ossimTrace.h>
static ossimTrace traceExec  ("ossimMonoGridRemapEngine:exec");
static ossimTrace traceDebug ("ossimMonoGridRemapEngine:debug");
static bool TRACE_FLAG = true;

//*****************************************************************************
//  METHOD: ossimMonoGridRemapEngine::dup
//  
//*****************************************************************************
ossimObject* ossimMonoGridRemapEngine::dup() const
{
   return new ossimMonoGridRemapEngine;
}

//*****************************************************************************
//  METHOD: ossimMonoGridRemapEngine::remapTile
//  
//*****************************************************************************
void ossimMonoGridRemapEngine::remapTile(const ossimDpt&       origin,
                                        ossimGridRemapSource* remapper,
                                        ossimRefPtr<ossimImageData>& tile)
{
   static const char MODULE[] = "ossimMonoGridRemapEngine::remapTile";
   if (traceExec())  CLOG << "entering..." << endl;

   //***
   // Fetch tile size and NULL pixel value:
   //***
   int    width         = tile->getWidth();
   int    height        = tile->getHeight();
   int    offset        = 0;
   double null;

   //***
   // Determine null pixel values so that we can recognize a null coming in and
   // not remap it:
   //***
   null = tile->getNullPix(0);
   ossimDblGrid& grid = *(remapper->getGrid(0));
   
   //***
   // Remap according to pixel type:
   //***
   switch(tile->getScalarType())
   {
   case OSSIM_UCHAR:
   {
      ossim_uint8* tile_buf  = (ossim_uint8*)tile->getBuf(0);
      short  pixel;
      
      for (double line=origin.line; line<origin.line+height; line+=1.0)
      {
         for (double samp=origin.samp; samp<origin.samp+width; samp+=1.0)
         {
            //***
            // Scan for null pixel before adding remap delta:
            //***
            if (tile_buf[offset] != (ossim_uint8) null)
            {
               //***
               // Remap MONO pixel with spatially variant bias value:
               //***
               pixel = tile_buf[offset] + (short) grid(samp,line);

               //***
               // Clamp:
               //***
               if      (pixel<0)   tile_buf[offset] = 0;
               else if (pixel>255) tile_buf[offset] = 255;
               else                tile_buf[offset] = pixel;
               
               //***
               // Avoid NULLS:
               //***
               if (tile_buf[offset] == (ossim_uint8) null) tile_buf[offset]++;
            }
            
            offset++;
         }
      }
      break;
   }
   
   case OSSIM_USHORT11:
   {
      ossim_uint16* tile_buf  = (ossim_uint16*)tile->getBuf(0);
      int  pixel;
      
      for (double line=origin.line; line<origin.line+height; line+=1.0)
      {
         for (double samp=origin.samp; samp<origin.samp+width; samp+=1.0)
         {
            //***
            // Scan for null pixel before adding remap delta:
            //***
            if (tile_buf[offset] != (ossim_uint16) null)
            {
               //***
               // Remap MONO pixel with spatially variant bias value:
               //***
               pixel = tile_buf[offset] + (int) grid(samp,line);

               //***
               // Clamp:
               //***
               if      (pixel<0)    tile_buf[offset] = 0;
               else if (pixel>2047) tile_buf[offset] = 2047;
               else                 tile_buf[offset] = pixel;
               
               //***
               // Avoid NULLS:
               //***
               if (tile_buf[offset] == (ossim_uint16) null) tile_buf[offset]++;
            }
            
            offset++;
         }
      }
      break;
   }
   
   case OSSIM_USHORT16:
   {
      ossim_uint16* tile_buf  = (ossim_uint16*)tile->getBuf(0);
      int  pixel;
      
      for (double line=origin.line; line<origin.line+height; line+=1.0)
      {
         for (double samp=origin.samp; samp<origin.samp+width; samp+=1.0)
         {
            //***
            // Scan for null pixel before adding remap delta:
            //***
            if (tile_buf[offset] != (ossim_uint16) null)
            {
               //***
               // Remap MONO pixel with spatially variant bias value:
               //***
               pixel = tile_buf[offset] + (int) grid(samp,line);

               //***
               // Clamp:
               //***
               if      (pixel<0)     tile_buf[offset] = 0;
               else if (pixel>65535) tile_buf[offset] = 65535;
               else                  tile_buf[offset] = pixel;
               
               //***
               // Avoid NULLS:
               //***
               if (tile_buf[offset] == (ossim_uint16) null) tile_buf[offset]++;
            }
            
            offset++;
         }
      }
      break;
   }
   
   case OSSIM_SSHORT16:
   {
      short* tile_buf  = (short*)tile->getBuf(0);
      int  pixel;
      
      for (double line=origin.line; line<origin.line+height; line+=1.0)
      {
         for (double samp=origin.samp; samp<origin.samp+width; samp+=1.0)
         {
            //***
            // Scan for null pixel before adding remap delta:
            //***
            if (tile_buf[offset] != (short) null)
            {
               //***
               // Remap MONO pixel with spatially variant bias value:
               //***
               pixel = tile_buf[offset] + (short) grid(samp,line);

               //***
               // Clamp:
               //***
               if      (pixel<-32766) tile_buf[offset] = -32766;
               else if (pixel> 32767) tile_buf[offset] = 32767;
               else                   tile_buf[offset] = pixel;
               
               //***
               // Avoid NULLS:
               //***
               if (tile_buf[offset] == (short) null) tile_buf[offset]++;
            }
            
            offset++;
         }
      }
      break;
   }
   default:
   {
	   ossimNotify(ossimNotifyLevel_WARN) << "WARNING ossimMonoGridRemapEngine::remapTile: Scalar type not handled" << std::endl;
	   break;
   }
	   
   }   // end switch statement

   if (traceExec())  CLOG << "returning..." << endl;
   return;
};

//*****************************************************************************
//  METHOD: ossimMonoGridRemapEngine::assignRemapValues
//
//  This engine defines the target value as an MONO vector of doubles, computed
//  as the mean of all contributor MONO values.
//  
//*****************************************************************************
void ossimMonoGridRemapEngine::assignRemapValues (
   vector<ossimAtbPointSource*>& sources_list)
{
   static const char MODULE[] = "ossimMonoGridRemapEngine::assignRemapValues";
   if (traceExec())  CLOG << "entering..." << endl;

   int i; // index to individual sources

   //***
   // Declare a 2D array that will contain all of the contributing sources'
   // MONO mean values. Also declare the accumulator target vector.
   //***
   int num_contributors = (int)sources_list.size();
   double** contributor_pixel = new double* [num_contributors];
   for (i=0; i<num_contributors; i++)
      contributor_pixel[i] = new double[1];
   double target_pixel = 0.0;

   //***
   // Now loop over each remaining contributor and sum in its contribution:
   //***
   vector<ossimAtbPointSource*>::iterator source;
   i = 0;
   for(source=sources_list.begin();
       source!=sources_list.end();
       source++)
   {
      (*source)->getSourceValue(contributor_pixel[i]);
      target_pixel += contributor_pixel[i][0]/(double)num_contributors;
      ++i;
   }

   //***
   // The target pixel has been established. Now need to compute the actual
   // remap quantities that will be written to the appropriate remap grids:
   //***
   i = 0;
   for(source=sources_list.begin();
       source!=sources_list.end();
       source++)
   {
      computeRemapNode(*source, contributor_pixel[i], &target_pixel);
      ++i;
   }

   // ***
   // Delete locally allocated memory:
   // ***
   for (i=0; i<num_contributors; ++i)
   {
      delete [] contributor_pixel[i];
   }
   delete [] contributor_pixel;
   
   if (traceExec())  CLOG << "returning..." << endl;
   return;
}

//*****************************************************************************
//  METHOD: ossimMonoGridRemapEngine::computeSourceValue
//  
//*****************************************************************************
void ossimMonoGridRemapEngine::computeSourceValue(
   ossimRefPtr<ossimImageData>& source, void* result)
{
   static const char MODULE[]="ossimMonoGridRemapEngine::computeSourceValue";
   if (traceExec())  CLOG << "entering..." << endl;

   //***
   // This engine defines "value" as the MONO vector corresponding to the mean
   // MONO pixel value of the source data:
   //***
   ((double*)result)[0] = source->computeAverageBandValue(0);
   
   if (traceExec())  CLOG << "returning..." << endl;
   return;
}

//*****************************************************************************
//  METHOD: ossimMonoGridRemapEngine::computeRemapNode
//
//  This engine defines the remap value as the difference between the target
//  MONO vector and the individual point source's value vector.
//
//*****************************************************************************
void ossimMonoGridRemapEngine::computeRemapNode(ossimAtbPointSource* ps,
                                              void* source_value,
                                              void* target_value)
{
   static const char MODULE[] = "ossimMonoGridRemapEngine::computeRemapNode";
   if (traceExec())  CLOG << "entering..." << endl;

   //***
   // Compute the remap grid node value specific to this MONO implementation:
   //***
   double node;
   node = ((double*)target_value)[0] - ((double*)source_value)[0];

   //***
   // Fetch a pointer to the remapper feeding this point source in order to
   // pass it the node value:
   //***
   ossimGridRemapSource* remapper = ps->getRemapSource();
   remapper->setGridNode(ps->getViewPoint(), &node);
                         
   if (traceDebug() || TRACE_FLAG)
   {
      CLOG << "DEBUG -- "
           << "\n\t ps->getViewPoint() = "<<ps->getViewPoint()
           << "\n\t source_value = "<<((double*)source_value)[0]
           << "\n\t target_value = "<<((double*)target_value)[0]
           << "\n\t node = "<<node
           << "\n\t remapper at "<<remapper<<endl;
   }

   if (traceExec())  CLOG << "returning..." << endl;
   return;
}

