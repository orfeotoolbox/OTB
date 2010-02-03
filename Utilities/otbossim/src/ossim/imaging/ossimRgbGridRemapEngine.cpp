//*****************************************************************************
// FILE: ossimRgbGridRemapEngine.cc
//
// Copyright (C) 2001 ImageLinks, Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// AUTHOR: Oscar Kramer
//
// DESCRIPTION: Contains implementation of class 
//
// LIMITATIONS: None.
//
//*****************************************************************************
//  $Id: ossimRgbGridRemapEngine.cpp 15833 2009-10-29 01:41:53Z eshirschorn $

#include <ossim/imaging/ossimRgbGridRemapEngine.h>

RTTI_DEF1(ossimRgbGridRemapEngine, "ossimRgbGridRemapEngine",
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
static ossimTrace traceExec  ("ossimRgbGridRemapEngine:exec");
static ossimTrace traceDebug ("ossimRgbGridRemapEngine:debug");

//*****************************************************************************
//  METHOD: ossimRgbGridRemapEngine::dup
//  
//*****************************************************************************
ossimObject* ossimRgbGridRemapEngine::dup() const
{
   return new ossimRgbGridRemapEngine;
}

//*****************************************************************************
//  METHOD: ossimRgbGridRemapEngine::remapTile
//  
//*****************************************************************************
void ossimRgbGridRemapEngine::remapTile(const ossimDpt&       origin,
                                        ossimGridRemapSource* remapper,
                                        ossimRefPtr<ossimImageData>& tile)
{
   static const char MODULE[] = "ossimRgbGridRemapEngine::remapTile";
   if (traceExec())  CLOG << "entering..." << endl;

   //***
   // Fetch tile size and NULL pixel value:
   //***
   int    width         = tile->getWidth();
   int    height        = tile->getHeight();
   int    offset        = 0;
   double null[3];

   //***
   // Determine null pixel values so that we can recognize a null coming in and
   // not remap it:
   //***
   null[0] = tile->getNullPix(0);
   null[1] = tile->getNullPix(1);
   null[2] = tile->getNullPix(2);
   
   ossimDblGrid& gridR = *(remapper->getGrid(0));
   ossimDblGrid& gridG = *(remapper->getGrid(1));
   ossimDblGrid& gridB = *(remapper->getGrid(2));
      
   //***
   // Remap according to pixel type:
   //***
   switch(tile->getScalarType())
   {
   case OSSIM_UCHAR:
   {
      ossim_uint8* red_buf  = (ossim_uint8*)tile->getBuf(0);
      ossim_uint8* grn_buf  = (ossim_uint8*)tile->getBuf(1);
      ossim_uint8* blu_buf  = (ossim_uint8*)tile->getBuf(2);
      short  pixel_buffer[3];
      
      for (double line=origin.line; line<origin.line+height; line+=1.0)
      {
         for (double samp=origin.samp; samp<origin.samp+width; samp+=1.0)
         {
            //***
            // Scan for null pixel before adding remap delta:
            //***
            if ((red_buf[offset] != (ossim_uint8) null[0]) &&
                (grn_buf[offset] != (ossim_uint8) null[1]) &&
                (blu_buf[offset] != (ossim_uint8) null[2]))
            {
               //***
               // Remap RGB pixel with spatially variant bias value:
               //***
               pixel_buffer[0] = red_buf[offset] + (short) gridR(samp,line);
               pixel_buffer[1] = grn_buf[offset] + (short) gridG(samp,line);
               pixel_buffer[2] = blu_buf[offset] + (short) gridB(samp,line);

               //***
               // Clamp:
               //***
               if      (pixel_buffer[0]<0)   red_buf[offset] = 0;
               else if (pixel_buffer[0]>255) red_buf[offset] = 255;
               else                          red_buf[offset] = pixel_buffer[0];
               

               if      (pixel_buffer[1]<0)   grn_buf[offset] = 0;
               else if (pixel_buffer[1]>255) grn_buf[offset] = 255;
               else                          grn_buf[offset] = pixel_buffer[1];
               

               if      (pixel_buffer[2]<0)   blu_buf[offset] = 0;
               else if (pixel_buffer[2]>255) blu_buf[offset] = 255;
               else                          blu_buf[offset] = pixel_buffer[2];
               
               //***
               // Avoid NULLS:
               //***
               if (red_buf[offset] == (ossim_uint8) null[0]) red_buf[offset]++;
               if (grn_buf[offset] == (ossim_uint8) null[1]) grn_buf[offset]++;
               if (blu_buf[offset] == (ossim_uint8) null[2]) blu_buf[offset]++;
            }
            
            offset++;
         }
      }
      break;
   }
   
   default:
   {
	   ossimNotify(ossimNotifyLevel_WARN) << "WARNING ossimRgbGridRemapEngine::remapTile: Scalar type not handled" << std::endl;
	   break;
   }

   }   // end switch statement

   if (traceExec())  CLOG << "returning..." << endl;
   return;
};

//*****************************************************************************
//  METHOD: ossimRgbGridRemapEngine::assignRemapValues
//
//  This engine defines the target value as an RGB vector of doubles, computed
//  as the mean of all contributor RGB values.
//  
//*****************************************************************************
void ossimRgbGridRemapEngine::assignRemapValues (
   vector<ossimAtbPointSource*>& sources_list)
{
   static const char MODULE[] = "ossimRgbGridRemapEngine::assignRemapValues";
   if (traceExec())  CLOG << "entering..." << endl;

   int i; // index to individual sources

   //***
   // Declare a 2D array that will contain all of the contributing sources'
   // RGB mean values. Also declare the accumulator target vector.
   //***
   int num_contributors =  (int)sources_list.size();
   double** contributor_pixel = new double* [num_contributors];
   for (i=0; i<num_contributors; i++)
      contributor_pixel[i] = new double[3];
   double target_pixel[3] = {0.0, 0.0, 0.0};

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

      target_pixel[0] += contributor_pixel[i][0]/(double)num_contributors;
      target_pixel[1] += contributor_pixel[i][1]/(double)num_contributors;
      target_pixel[2] += contributor_pixel[i][2]/(double)num_contributors;

      i++;
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
      computeRemapNode(*source, contributor_pixel[i], target_pixel);
      i++;
   }

   //***
   // Delete locally allocated memory:
   //***
   for (i=0; i<num_contributors; i++)
      delete [] contributor_pixel[i];
   delete [] contributor_pixel;
   
   if (traceExec())  CLOG << "returning..." << endl;
   return;
}

//*****************************************************************************
//  METHOD: ossimRgbGridRemapEngine::computeSourceValue
//  
//*****************************************************************************
void ossimRgbGridRemapEngine::computeSourceValue(
   ossimRefPtr<ossimImageData>& source, void* result)
{
   static const char MODULE[]="ossimRgbGridRemapEngine::computeSourceValue";
   if (traceExec())  CLOG << "entering..." << endl;

   //***
   // This engine defines "value" as the RGB vector corresponding to the mean
   // RGB pixel value of the source data:
   //***
   ((double*)result)[0] = source->computeAverageBandValue(0);
   ((double*)result)[1] = source->computeAverageBandValue(1);
   ((double*)result)[2] = source->computeAverageBandValue(2);
   
   if (traceExec())  CLOG << "returning..." << endl;
   return;
}

//*****************************************************************************
//  METHOD: ossimRgbGridRemapEngine::computeRemapNode
//
//  This engine defines the remap value as the difference between the target
//  RGB vector and the individual point source's value vector.
//
//*****************************************************************************
void ossimRgbGridRemapEngine::computeRemapNode(ossimAtbPointSource* ps,
                                              void* source_value,
                                              void* target_value)
{
   static const char MODULE[] = "ossimRgbGridRemapEngine::computeRemapNode";
   if (traceExec())  CLOG << "entering..." << endl;

   //***
   // Compute the remap grid node value specific to this RGB implementation:
   //***
   double node[3];
   node[0] = ((double*)target_value)[0] - ((double*)source_value)[0];
   node[1] = ((double*)target_value)[1] - ((double*)source_value)[1];
   node[2] = ((double*)target_value)[2] - ((double*)source_value)[2];

   //***
   // Fetch a pointer to the remapper feeding this point source in order to
   // pass it the node value:
   //***
   ossimGridRemapSource* remapper = ps->getRemapSource();
   remapper->setGridNode(ps->getViewPoint(), node);

   if (traceExec())  CLOG << "returning..." << endl;
   return;
}

