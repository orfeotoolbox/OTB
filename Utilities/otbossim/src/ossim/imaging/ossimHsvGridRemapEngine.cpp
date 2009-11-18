//*****************************************************************************
// FILE: ossimHsvGridRemapEngine.cc
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
//  $Id: ossimHsvGridRemapEngine.cpp 15833 2009-10-29 01:41:53Z eshirschorn $

#include <ossim/imaging/ossimHsvGridRemapEngine.h>

RTTI_DEF1(ossimHsvGridRemapEngine, "ossimHsvGridRemapEngine",
          ossimGridRemapEngine);

#include <ossim/imaging/ossimGridRemapSource.h>
#include <ossim/imaging/ossimAtbPointSource.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimDblGrid.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/base/ossimRgbVector.h>
#include <ossim/base/ossimHsvVector.h>

//***
// Define Trace flags for use within this file:
//***
#include <ossim/base/ossimTrace.h>
static ossimTrace traceExec  ("ossimHsvGridRemapEngine:exec");
static ossimTrace traceDebug ("ossimHsvGridRemapEngine:debug");

//*****************************************************************************
//  METHOD: ossimHsvGridRemapEngine::remapTile
//  
//*****************************************************************************
ossimObject* ossimHsvGridRemapEngine::dup() const
{
   return new ossimHsvGridRemapEngine;
}

//*****************************************************************************
//  METHOD: ossimHsvGridRemapEngine::remapTile
//  
//*****************************************************************************
void ossimHsvGridRemapEngine::remapTile(const ossimDpt&       origin,
                                        ossimGridRemapSource* remapper,
                                        ossimRefPtr<ossimImageData>& tile)
{
   static const char MODULE[] = "ossimHsvGridRemapEngine::remapTile";
   if (traceExec())  CLOG << "entering..." << endl;

   //***
   // Fetch tile size and NULL pixel value:
   //***
   int    width         = tile->getWidth();
   int    height        = tile->getHeight();
   int    offset        = 0;
   
   void* red_buf = tile->getBuf(0);
   void* grn_buf = tile->getBuf(1);
   void* blu_buf = tile->getBuf(2);

   ossimDblGrid& gridH = *(remapper->getGrid(0));
   ossimDblGrid& gridS = *(remapper->getGrid(1));
   ossimDblGrid& gridV = *(remapper->getGrid(2));
      
   //---
   // Remap according to pixel type:
   //---
   switch(tile->getScalarType())
   {
      case OSSIM_UINT8:
      {
         for (double line=origin.line; line<origin.line+height; line+=1.0)
         {
            for (double samp=origin.samp; samp<origin.samp+width; samp+=1.0)
            {
               //---
               // Fetch pixel from the input tile band buffers and convert
               // to HSV:
               //---
               ossimRgbVector rgb_pixel (((ossim_uint8*)red_buf)[offset],
                                         ((ossim_uint8*)grn_buf)[offset],
                                         ((ossim_uint8*)blu_buf)[offset]);
               ossimHsvVector hsv_pixel (rgb_pixel);
               
               //---
               // Remap pixel HSV  with spatially variant bias value:
               //---
               hsv_pixel.setH(hsv_pixel.getH() + gridH(samp,line));
               hsv_pixel.setS(hsv_pixel.getS() + gridS(samp,line));
               hsv_pixel.setV(hsv_pixel.getV() + gridV(samp,line));
               
               //---
               // Convert back to RGB and write to the tile:
               //---
               rgb_pixel = hsv_pixel;  // auto-clamped
               ((ossim_uint8*)red_buf)[offset] = rgb_pixel.getR();
               ((ossim_uint8*)grn_buf)[offset] = rgb_pixel.getG();
               ((ossim_uint8*)blu_buf)[offset] = rgb_pixel.getB();
               
               offset++;
            }
         }
         break;
      }
      
      case OSSIM_USHORT11:
         break;
         
      case OSSIM_UINT16:
         break;
         
      case OSSIM_SINT16:
         break;	

      case OSSIM_FLOAT64:
         break;	

      case OSSIM_NORMALIZED_DOUBLE:
         break;	

      case OSSIM_FLOAT32:
         break;	

      case OSSIM_NORMALIZED_FLOAT:
         break;	

      case OSSIM_SCALAR_UNKNOWN:
      default:
         break;

   }   // end switch statement

   if (traceExec())  CLOG << "returning..." << endl;
   return;
};

//*****************************************************************************
//  METHOD: ossimHsvGridRemapEngine::assignRemapValues
//
//  This engine defines the target value as an HSV vector of doubles, computed
//  as the mean of all contributor HSV values.
//  
//*****************************************************************************
void ossimHsvGridRemapEngine::assignRemapValues (
   vector<ossimAtbPointSource*>& sources_list)
{
   static const char MODULE[] = "ossimHsvGridRemapEngine::assignRemapValues";
   if (traceExec())  CLOG << "entering..." << endl;

   int i; // index to individual sources

   //***
   // Declare a 2D array that will contain all of the contributing sources'
   // HSV mean values. Also declare the accumulator target vector.
   //***
   int num_contributors = (int)sources_list.size();
   double** contributor_pixel = new double* [num_contributors];
   for (i=0; i<num_contributors; i++)
      contributor_pixel[i] = new double[3];
   double target_pixel[3] = {0.0, 0.0, 0.0};

   //***
   // Now loop over each remaining contributor and sum in its contribution:
   //***
   vector<ossimAtbPointSource*>::iterator source;
   i = 0;
   for(source  = sources_list.begin();
       source != sources_list.end();
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
   for(source  = sources_list.begin();
       source != sources_list.end();
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
//  METHOD: ossimHsvGridRemapEngine::computeSourceValue
//  
//*****************************************************************************
void ossimHsvGridRemapEngine::computeSourceValue(
   ossimRefPtr<ossimImageData>& source, void* result)
{
   static const char MODULE[]="ossimHsvGridRemapEngine::computeSourceValue";
   if (traceExec())  CLOG << "entering..." << endl;

   //***
   // This engine defines "value" as the HSV vector corresponding to the mean
   // RGB pixel value of the source data:
   //***
   ossimRgbVector rgb_vector;
   rgb_vector.setR((unsigned char) source->computeAverageBandValue(0));
   rgb_vector.setG((unsigned char) source->computeAverageBandValue(1));
   rgb_vector.setB((unsigned char) source->computeAverageBandValue(2));

   //***
   // Assign the HSV components to the result vector:
   //***
   ossimHsvVector hsv_vector (rgb_vector);
   ((double*)result)[0] = (double) hsv_vector.getH();
   ((double*)result)[1] = (double) hsv_vector.getS();
   ((double*)result)[2] = (double) hsv_vector.getV();
   
   if (traceExec())  CLOG << "returning..." << endl;
   return;
}

//*****************************************************************************
//  METHOD: ossimHsvGridRemapEngine::computeRemapNode
//
//  This engine defines the remap value as the difference between the target
//  HSV vector and the individual point source's value vector.
//
//*****************************************************************************
void ossimHsvGridRemapEngine::computeRemapNode(ossimAtbPointSource* ps,
                                              void* source_value,
                                              void* target_value)
{
   static const char MODULE[] = "ossimHsvGridRemapEngine::computeRemapNode";
   if (traceExec())  CLOG << "entering..." << endl;

   //***
   // Compute the remap grid node value specific to this HSV implementation:
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

