//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: David Burken
// TR # 136 kminear      Fix extractVertices method
//
//*************************************************************************
// $Id: ossimVertexExtractor.cpp 21184 2012-06-29 15:13:09Z dburken $

#include <fstream>
using namespace std;

#include <ossim/imaging/ossimVertexExtractor.h>
#include <ossim/imaging/ossimImageSource.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimNotifyContext.h>

static ossimTrace traceDebug("ossimVertexExtractor:degug");

RTTI_DEF2(ossimVertexExtractor, "ossimVertexExtractor",
          ossimSource, ossimProcessInterface);

ossimVertexExtractor::ossimVertexExtractor(ossimImageSource* inputSource)
      :
      ossimOutputSource(0, // owner
                        1,
                        0,
                        true,
                        true),
      ossimProcessInterface(),
      theAreaOfInterest(),
      theFilename(ossimFilename::NIL),
      theFileStream(),
      theVertice(4),
      theLeftEdge(0),
      theRightEdge(0)
{
   if (inputSource == 0)
   {
      ossimNotify(ossimNotifyLevel_WARN) << "ossimVertexExtractor::ossimVertexExtractor ERROR"
                                         << "\nNULL input image source passed to constructor!"
                                         << std::endl;
   }

   connectMyInputTo(0, inputSource);
//   theInputObjectList[0] = inputSource;
   theAreaOfInterest.makeNan();
}

ossimVertexExtractor::~ossimVertexExtractor()
{
   if (theLeftEdge)
   {
      delete [] theLeftEdge;
      theLeftEdge = 0;
   }
   if (theRightEdge)
   {
      delete [] theRightEdge;
      theRightEdge = 0;
   }
}

bool ossimVertexExtractor::execute()
{
   static const char MODULE[] = "ossimVertexExtractor::execute";

   if (traceDebug()) CLOG << " Entered..." << endl;
   
   if (theFilename == ossimFilename::NIL)
   {
      ossimNotify(ossimNotifyLevel_WARN) << "ossimVertexExtractor::execute WARNING:"
                                         << "\nOutput file name is empty.  Use setFilename method..."
                                         << std::endl;
      return false;
   }

   if (theAreaOfInterest.isNan())
   {
      ossimNotify(ossimNotifyLevel_NOTICE) << MODULE << " NOTICE ossimVertexExtractor::execute:"
                                           << "\nArea of interest is not set.  Use setAreaOfInterest method..."
                                           << "\nDefaulting to bounding rectangle of input source."
                                           << std::endl;
      ossimImageSource* src = reinterpret_cast<ossimImageSource*>(getInput(0));
      theAreaOfInterest = src->getBoundingRect(0);
   }

   setProcessStatus(ossimProcessInterface::PROCESS_STATUS_EXECUTING);
   
   if (scanForEdges())
   {
      if (extractVertices())
      {
         writeVertices();
      }
      else
      {
         return false;
      }
   }
   else
   {
      return false;
   }
   
   setProcessStatus(ossimProcessInterface::PROCESS_STATUS_NOT_EXECUTING);

   if (traceDebug()) CLOG << " Exited..." << endl;

   return true;
}

bool ossimVertexExtractor::scanForEdges()
{
   static const char MODULE[] = "ossimVertexExtractor::scanForEdges";

   if (traceDebug()) CLOG << " Entered..." << endl;

   ossimImageSource* src = reinterpret_cast<ossimImageSource*>(getInput(0));
   if (!src)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "WARN ossimVertexExtractor::scanForEdges():"
         << "\nInput source is null.  Returning..." << std::endl;
      return false;
   }
   
   // Allocate the arrays to hold the edges.
   if (theLeftEdge)  delete [] theLeftEdge;
   if (theRightEdge) delete [] theRightEdge;
   theLeftEdge  = new ossim_int32[theAreaOfInterest.height()];
   theRightEdge = new ossim_int32[theAreaOfInterest.height()];
   for (ossim_int32 i=0; i<(int)theAreaOfInterest.height(); ++i)
   {
      theLeftEdge[i]  = OSSIM_INT_NAN;
      theRightEdge[i] = OSSIM_INT_NAN;
   }
   
   // Some constants needed throughout...
   const ossim_int32 TILE_HEIGHT    = src->getTileHeight();
   const ossim_int32 TILE_WIDTH     = src->getTileWidth();
   const ossim_int32 START_LINE = theAreaOfInterest.ul().y;
   const ossim_int32 STOP_LINE  = theAreaOfInterest.lr().y;
   const ossim_int32 START_SAMP = theAreaOfInterest.ul().x;
   const ossim_int32 STOP_SAMP  = theAreaOfInterest.lr().x;

   // For percent complete status.
   double total_tiles_y_dir = theAreaOfInterest.height() / TILE_HEIGHT;
   if (theAreaOfInterest.height() % TILE_HEIGHT) total_tiles_y_dir += 1.0;
   double tiles_processed = 0.0;

   // Set the status message to be "scanning source for edges..."
   ossimNotify(ossimNotifyLevel_INFO) << "Scanning image source for edges..." << std::endl;
   
   // Start off with a percent complete at 0...
   setPercentComplete(0.0);

   // Start of loop to get the left edges.
   
   // Tile loop in line direction through area of interest.

   for (ossim_int32 line = START_LINE; line < STOP_LINE; line += TILE_HEIGHT)
   {
      const ossim_int32 START_INDEX = line - START_LINE;
      const ossim_int32 STOP_INDEX  = ( (line+TILE_HEIGHT-1) < STOP_LINE ) ?
         (START_INDEX+TILE_HEIGHT-1) :
         (theAreaOfInterest.height()-1);
      const ossim_int32 TILE_LINE_STOP = ((line+TILE_HEIGHT-1) < STOP_LINE) ?
         (line+TILE_HEIGHT-1) :  STOP_LINE;
      
      // See if all the edges have been found for these tile lines.
      bool all_edges_found  = true;
      for (ossim_int32 i = START_INDEX; i <= STOP_INDEX; ++i)
      {
         if (theLeftEdge[i] == OSSIM_INT_NAN)
         {
            all_edges_found = false;
            break;
         }
      }
      
      // Extract the left edges...
      if (all_edges_found == false)
      {
         //***
         // Tile loop in sample direction through area of interest
         // (left edges).
         //***
         for (ossim_int32 samp = START_SAMP; samp < STOP_SAMP; samp += TILE_WIDTH)
         {
            // Get the tile.
            ossimRefPtr<ossimImageData> id =
               src->getTile(ossimIpt(samp, line));
            
            if (!id.valid() || id->getDataObjectStatus() == OSSIM_NULL ||
                id->getDataObjectStatus() == OSSIM_EMPTY)
            {
               continue; // Nothing to do...
            }
            
            // Line loop through tile.
            const ossim_int32 TILE_SAMP_STOP = ((samp+TILE_WIDTH-1) < STOP_SAMP) ?
               (samp+TILE_WIDTH-1) : STOP_SAMP;
            
            for (ossim_int32 y = line; y <= TILE_LINE_STOP; ++y)
            {
               ossim_int32 edge_index = y - START_LINE;
               if (theLeftEdge[edge_index] == OSSIM_INT_NAN)
               {
                  for (ossim_int32 x = samp; x <= TILE_SAMP_STOP; ++x)
                  {
                     if (! (id->isNull(ossimIpt(x,y))))
                     {
                        // Capture the first valid pixel.
                        theLeftEdge[edge_index] = x - START_SAMP;
                        break;
                     }
                  }
               }
            }
         }  // End of tile loop in sample direction through area of interest.
      }  // End of (all_lines_checked == false) (left edges)

      // See if all the right edges have been found for these tile lines.
      all_edges_found = true;
      for (ossim_int32 i = START_INDEX; i <= STOP_INDEX; ++i)
      {
         if (theRightEdge[i] == OSSIM_INT_NAN)
         {
            all_edges_found = false;
            break;
         }
      }

      // Extract the right edges...
      if (all_edges_found == false)
      {
         //***
         // Tile loop in sample direction through area of interest
         // (right edges).
         //***

         // Get the x origin of the last tile in a row.
         const ossim_int32 END_TILE_X = START_SAMP + ((theAreaOfInterest.width()/
                                                 TILE_WIDTH)*TILE_WIDTH);
         

         for (ossim_int32 samp = END_TILE_X; samp >= START_SAMP; samp -= TILE_WIDTH)
         {
            // Get the tile.
            ossimRefPtr<ossimImageData> id =
               src->getTile(ossimIpt(samp, line));
            
            if (!id.valid() || id->getDataObjectStatus() == OSSIM_NULL ||
                id->getDataObjectStatus() == OSSIM_EMPTY)
            {
               continue; // Nothing to do...
            }
            
            // Line loop through tile.
            const ossim_int32 TILE_SAMP_STOP = ((samp+TILE_WIDTH-1) < STOP_SAMP) ?
               (samp+TILE_WIDTH-1) : STOP_SAMP;
            
            for (ossim_int32 y = line; y <= TILE_LINE_STOP; ++y)
            {
               ossim_int32 edge_index = y - START_LINE;
               if (theRightEdge[edge_index] == OSSIM_INT_NAN)
               {
                  for (ossim_int32 x = TILE_SAMP_STOP; x >= samp; --x)
                  {
                     if (! (id->isNull(ossimIpt(x,y))))
                     {
                        // Capture the first valid pixel.
                        theRightEdge[edge_index] = x - START_SAMP;
                        break;
                     }
                  }
               }
            }
            
         }  // End of tile loop in sample direction through area of interest.
         
      }  // End of (all_lines_checked == false)

      // Set the percent complete.
      tiles_processed += 1.0;
      setPercentComplete(tiles_processed/total_tiles_y_dir*100.0);
   }  // End of loop through tiles in line direction.

   setPercentComplete(100.0);
      
   if (traceDebug())
   {
      CLOG << "DEBUG:" << endl;
      for (ossim_int32 i=0; i<(int)theAreaOfInterest.height(); ++i)
      {
         ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG: left[" << i << "]:  "
                                             << theLeftEdge[i]
                                             << " right[" << i << "]:  "
                                             << theRightEdge[i]
                                             << std::endl;
      }
   }

   
   return true;
}

bool ossimVertexExtractor::extractVertices()
{
   //***
   // This method returns the non-null vertexes of the image corners.
   // This can be used to initialize the "ValidImageVertices" for an 
   // image.
   // This method does not handle images that have more than four vertexes.
   //***
   static const char MODULE[] = "ossimVertexExtractor::extractVertices";

   if (traceDebug()) CLOG << " Entered..." << endl;

   if (!theLeftEdge || !theRightEdge)
   {
      ossimNotify(ossimNotifyLevel_WARN) << "ERROR ossimVertexExtractor::extractVertices():"
                                         << "\nEdges not initialized!" << std::endl;
      return false;
   }

   // Image must be quadrilateral (i.e. 4 sided polygon)
   
   ossim_int32   total_lines = theAreaOfInterest.height();
   
   ossim_int32   topLine             = 0;    
   ossim_int32   bottomLine          = 0;
   
   ossim_int32   firstLeftSample     = 0;
   ossim_int32   firstRightSample    = 0;
   
   ossim_int32   lastLeftSample      = 0;
   ossim_int32   lastRightSample     = 0;
   
   ossim_int32   leftMostSample      = 0;
   ossim_int32   leftMostSampleLine  = 0;
   
   ossim_int32   rightMostSample     = 0;
   ossim_int32   rightMostSampleLine = 0;
   
   ossim_int32   leftCorner_A        = 0;
   ossim_int32   leftCornerLine_A    = 0;
   ossim_int32   leftCorner_B        = 0;
   ossim_int32   leftCornerLine_B    = 0;
   
   ossim_int32   rightCorner_A       = 0;
   ossim_int32   rightCornerLine_A   = 0;
   ossim_int32   rightCorner_B       = 0;
   ossim_int32   rightCornerLine_B   = 0;


   bool found_first_non_null_line = 0;
   bool found_last_non_null_line = 0;

   bool rhombus = false;
   bool square = false;
   bool left = false;
   bool right = false;

   ossim_int32 first_non_null_line = 0;
   ossim_int32 last_non_null_line = 0;

   ossim_int32 i = 0;
   while(found_last_non_null_line == 0)
   {  
      // Look for the first row with a valid DN
      // Nulls are negative
         
      if(    (theLeftEdge[i] >= 0) 
             && (found_first_non_null_line == 0) )
      {
         // Found first valid pixel at row i 
         found_first_non_null_line = 1;        
         first_non_null_line = i;
            
         // Finish walking down the rows of the image
         while(  (found_first_non_null_line == 1)
                 && (theLeftEdge[i] >= 0)
                 && (found_last_non_null_line == 0)
                 && (i <= total_lines) )
         {
            i = i+1;
         }
         found_last_non_null_line = 1;
         last_non_null_line = i-1;
      }

      i = i+1;
         
   }//end  while "found_last_non_null_line == 0"
   
   last_non_null_line = last_non_null_line -1;

   topLine = first_non_null_line;
   bottomLine = last_non_null_line;

   // Record the slope searching from the right edge
   ossim_int32 k = 0;
   ossim_int32 start = first_non_null_line;
   ossim_int32 end = last_non_null_line;
   ossim_int32 sizeDeltaSlope = last_non_null_line - first_non_null_line;
   double* rightSlope = new double[sizeDeltaSlope];
  
   for(ossim_int32 i = start; i <= (end - 100); i += 100)
   {
      ossim_int32  run = theRightEdge[i] - theRightEdge[i + 100];
      if(run != 0)
      {
         rightSlope[k] =  100.0 / run;
      }
      else
      {
         rightSlope[k]= OSSIM_INT_NAN;
      }
      
      k = k + 1;
   }
   if (traceDebug())
   {
      CLOG << "DEBUG:" << endl;
      for (ossim_int32 i=0; i<= k-1; i++)
      {
          ossimNotify(ossimNotifyLevel_DEBUG)<< setprecision(15) << "rightSlope[" << i << "]:  "
                                             << rightSlope[i]
                                             << std::endl;
      }
   }

   // Record the slope searching from the left edge
   k = 0;
   double* leftSlope = new double[sizeDeltaSlope];
  
   for(ossim_int32 i = start; i <= (end - 100); i += 100)
   {
      ossim_int32  run = theLeftEdge[i] - theLeftEdge[i + 100];
      if(run != 0)
      {
         leftSlope[k] =  100.0 / run;
      }
      else
      {
         leftSlope[k] = OSSIM_INT_NAN;
      }
      
      k = k + 1;
   }
   if (traceDebug())
   {
      CLOG << "DEBUG:" << endl;
      for (ossim_int32 i=0; i<= k-1; i++)
      {
         ossimNotify(ossimNotifyLevel_DEBUG) << setprecision(15) << "leftSlope[" << i << "]:  "
                                             << leftSlope[i]
                                             << std::endl;
      }
   }
   ossim_int32 sizeOftheSlope = k-1;
   ossim_int32 counter = 0;


      // Find the right most sample
   ossim_int32 rightMostEdgeTemp = theRightEdge[topLine];
   ossim_int32 rightMostEdgeTempLine = 0;

   for(ossim_int32 i= (topLine+1); i<bottomLine; i++)
   {     
      if(theRightEdge[i] >= rightMostEdgeTemp)         
      {
         rightMostEdgeTemp = theRightEdge[i];
         rightMostEdgeTempLine = i;
      }
   }
   rightMostSample = rightMostEdgeTemp;
   rightMostSampleLine = rightMostEdgeTempLine-1;
   lastLeftSample = theLeftEdge[bottomLine];

   // Find the left most sample
   ossim_int32 leftMostEdgeTemp = theLeftEdge[topLine];
   ossim_int32 leftMostEdgeTempLine = 0;

   for(ossim_int32 i= (topLine+1); i<bottomLine; i++)
   {     
      if(theLeftEdge[i] <= leftMostEdgeTemp)         
      {
         leftMostEdgeTemp = theLeftEdge[i];
         leftMostEdgeTempLine = i;
      }
   }
   leftMostSample = leftMostEdgeTemp;
   leftMostSampleLine = leftMostEdgeTempLine-1;
   firstLeftSample = theLeftEdge[topLine];
   
   // Case: Check for square
   for(ossim_int32 i=0; i < sizeOftheSlope; ++i)
   {
      if(leftSlope[i]==rightSlope[i])
      {
         counter = counter + 1;
      }
   }
   
   if(counter == sizeOftheSlope)
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)<<"\n\n***Case = SQUARE***"<<endl<<endl;
      }
      
      square  = 1;
      
      leftCornerLine_A = topLine;
      //leftCorner_A = theLeftEdge[topLine]; //0
      leftCorner_A = leftMostSample;

      rightCornerLine_A = topLine;
      //rightCorner_A = theRightEdge[topLine]; //1
      rightCorner_A = rightMostSample; //1

      rightCornerLine_B =  bottomLine;
      //rightCorner_B = theRightEdge[bottomLine]; //2
      rightCorner_B = rightMostSample;
        
      leftCornerLine_B = bottomLine;
      //leftCorner_B = theLeftEdge[bottomLine]; //3
      leftCorner_B = leftMostSample;
   }
   
   firstRightSample = theRightEdge[topLine];
   lastRightSample  = theRightEdge[bottomLine];


   
   ossim_int32 halfsamps = (rightMostSample - leftMostSample ) / 2;

   if(square == 0)     
   { 
      if( (firstRightSample - firstLeftSample) > halfsamps )
      {
         // Case is rhombus (clipped or not clipped)

         rhombus = 1;
         if (traceDebug())
         { 
            ossimNotify(ossimNotifyLevel_DEBUG)<<"\n\n***Case = RHOMBUS***"<<endl;
            ossimNotify(ossimNotifyLevel_DEBUG)<<"here"<<endl;
         }
         
         leftCornerLine_A = topLine;
         leftCorner_A = firstLeftSample; //0
      
         rightCornerLine_A = topLine;
         rightCorner_A= firstRightSample; //1
       
         rightCornerLine_B = bottomLine;
         rightCorner_B = lastRightSample; //2
      
         leftCornerLine_B = bottomLine;
         leftCorner_B = lastLeftSample; //3

      } 
   }
   
   
   if(square == 0 && rhombus == 0)
   {//if not square or rhombus
      
      if(firstLeftSample < lastLeftSample)
      {//if tilted right

         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)<<"\n\n***Case = TILTED RIGHT***"<<endl<<endl;
         }
         
         right = true;
         
         // Regression line 2 is the right side
         // Regression line 4 is the left side
         // These are handled differently
         // These sides may be 'uneven' or 'torn' or 'louvered'
         // This code finds the best regression line that 'cuts off'
         // this edge fringe
         
         //Regression Line 1
         
         ossim_int32 line1_pt4 = rightMostSampleLine;
         ossim_int32 line1_pt0 = topLine;
         ossim_int32 line1_pt2 = (line1_pt0 + line1_pt4)/2;
         ossim_int32 line1_pt1 = (line1_pt2 + line1_pt0)/2;
         ossim_int32 line1_pt3 = (line1_pt2 + line1_pt4)/2;
         
         double line1RegPtsSamples[] = { static_cast<double>(theRightEdge[line1_pt1]),
                                         static_cast<double>(theRightEdge[line1_pt2]),
                                         static_cast<double>(theRightEdge[line1_pt3]) };
         double line1RegPtsLines[] = { static_cast<double>(-line1_pt1),
                                       static_cast<double>(-line1_pt2),
                                       static_cast<double>(-line1_pt3) };
         
         double sum_xy_line1 = 0.0;
         double sum_x_line1 = 0.0;
         double sum_y_line1 = 0.0;
         double mean_x_line1 = 0.0;
         double mean_y_line1 = 0.0;
         double sum_x_squared_line1 = 0.0;
         double sum_squared_x_line1 = 0.0;   
         double b_line1;
         double a_line1;
         double num_elements_line1 = 3.0;
         
         for(ossim_int32 i = 0; i < num_elements_line1; ++i)
         {
            sum_xy_line1 = sum_xy_line1
               + (line1RegPtsSamples[i]*line1RegPtsLines[i]);
            
            sum_x_line1 = sum_x_line1 + line1RegPtsSamples[i];
            sum_y_line1 = sum_y_line1 + line1RegPtsLines[i];
            
            sum_squared_x_line1 = sum_squared_x_line1
               + line1RegPtsSamples[i]*line1RegPtsSamples[i];
         }
         
         sum_x_squared_line1 = sum_x_line1*sum_x_line1; 
         mean_y_line1 = sum_y_line1/num_elements_line1;
         mean_x_line1 = sum_x_line1/num_elements_line1;
         b_line1 = (sum_xy_line1 - (sum_x_line1*sum_y_line1)/num_elements_line1)
            / (sum_squared_x_line1 - (sum_x_squared_line1/num_elements_line1));
         a_line1 = mean_y_line1 - b_line1*mean_x_line1;
         
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)<<" a_line1:  "<< a_line1 <<std::endl;
            ossimNotify(ossimNotifyLevel_DEBUG)<<" b_line1:  "<< b_line1 <<std::endl;
            
            
            for(ossim_int32 i = 0; i < num_elements_line1; ++i)
            {
               
               ossimNotify(ossimNotifyLevel_DEBUG) << setprecision(15) << "line1RegPtsLines[" << i
                                                   << "]:  " <<line1RegPtsLines[i]
                                                   << "\nline1RegPtsSamples[" << i << "]:  "
                                                   <<line1RegPtsSamples[i]
                                                   << std::endl;  
               
            }
         }
         
         //Regression Line 2
         ossim_int32 line2_pt0 = rightMostSampleLine;
         ossim_int32 line2_pt4 = bottomLine;
         ossim_int32 line2_pt2 = (line2_pt0 + line2_pt4)/2;
         ossim_int32 line2_pt1 = (line2_pt2 + line2_pt0)/2;
         ossim_int32 line2_pt3 = (line2_pt2 + line2_pt4)/2;
 
         
         ossim_int32 minValuePt1;
         ossim_int32 minOffsetPt1;
         getMinAndIndex(&theRightEdge[line2_pt1-100],
                        &theRightEdge[line2_pt1+100],
                        minValuePt1, minOffsetPt1 );
         line2_pt1 = line2_pt1-100 + minOffsetPt1;
         
         ossim_int32 minValuePt2;
         ossim_int32 minOffsetPt2;
         getMinAndIndex(&theRightEdge[line2_pt2-100],
                        &theRightEdge[line2_pt2+100],
                        minValuePt2, minOffsetPt2 );
         line2_pt2 = line2_pt2-100 + minOffsetPt2;
         
         
         ossim_int32 minValuePt3;
         ossim_int32 minOffsetPt3;
         getMinAndIndex(&theRightEdge[line2_pt3-100],&theRightEdge[line2_pt3+100],
                        minValuePt3, minOffsetPt3 );
         line2_pt3 = line2_pt3-100 + minOffsetPt3;
         
         //---
         // Find the line within 100 lines of line2_ptx that has the
         // minimum sample.  This will cut off the 'fringe'
         //---
         double line2RegPtsSamples[] = { static_cast<double>(theRightEdge[line2_pt1]),
                                         static_cast<double>(theRightEdge[line2_pt2]),
                                         static_cast<double>(theRightEdge[line2_pt3]) };
         double line2RegPtsLines[] = { static_cast<double>(-line2_pt1),
                                       static_cast<double>(-line2_pt2),
                                       static_cast<double>(-line2_pt3) };
         
         double sum_xy_line2 = 0.0;
         double sum_x_line2 = 0.0;
         double sum_y_line2 = 0.0;
         double mean_x_line2 = 0.0;
         double mean_y_line2 = 0.0;
         double sum_x_squared_line2 = 0.0;
         double sum_squared_x_line2 = 0.0;   
         double b_line2;
         double a_line2;
         double num_elements_line2 = 3.0;
         
         for(ossim_int32 i = 0; i < num_elements_line2; ++i)
         {
            sum_xy_line2 = sum_xy_line2
               + (line2RegPtsSamples[i]*line2RegPtsLines[i]);
            
            sum_x_line2 = sum_x_line2 + line2RegPtsSamples[i];
            sum_y_line2 = sum_y_line2 + line2RegPtsLines[i];
            
            sum_squared_x_line2 = sum_squared_x_line2
               + line2RegPtsSamples[i]*line2RegPtsSamples[i];
         }

         sum_x_squared_line2 = sum_x_line2*sum_x_line2; 
         mean_y_line2 = sum_y_line2/num_elements_line2;
         mean_x_line2 = sum_x_line2/num_elements_line2;
         b_line2 = (sum_xy_line2 - (sum_x_line2*sum_y_line2)/num_elements_line2)
            / (sum_squared_x_line2 - (sum_x_squared_line2/num_elements_line2));
         a_line2 = mean_y_line2 - b_line2*mean_x_line2;

         if(traceDebug())
         {
            for(ossim_int32 i = 0; i < num_elements_line2; ++i)
            {
               ossimNotify(ossimNotifyLevel_DEBUG) << setprecision(15) << "line2RegPtsLines[" << i << "]:  "
                                                   <<line2RegPtsLines[i]
                                                   << "\nline2RegPtsSamples[" << i << "]:  "
                                                   <<line2RegPtsSamples[i]
                                                   <<std::endl;
            }
            ossimNotify(ossimNotifyLevel_DEBUG) << "\na_line2:  " << a_line2
                                                << "\nb_line2:  " << b_line2 << std::endl;
               
         }
         
         //End regression line2
            
         //Intersect lines 1 and 2 to get the ur corner vertex

         double ur_x = (a_line1 - a_line2)/(b_line2 - b_line1);
         double ur_y = -(a_line1 + b_line1*ur_x);
            
         //Regression Line 3
            
         ossim_int32 line3_pt0 = leftMostSampleLine;
         ossim_int32 line3_pt4 = bottomLine;
         ossim_int32 line3_pt2 = (line3_pt0 + line3_pt4)/2;
         ossim_int32 line3_pt1 = (line3_pt2 + line3_pt0)/2;
         ossim_int32 line3_pt3 = (line3_pt2 + line3_pt4)/2;
            
         double line3RegPtsSamples[] = { static_cast<double>(theLeftEdge[line3_pt1]),
                                         static_cast<double>(theLeftEdge[line3_pt2]),
                                         static_cast<double>(theLeftEdge[line3_pt3]) };
         double line3RegPtsLines[] = { static_cast<double>(-line3_pt1),
                                       static_cast<double>(-line3_pt2),
                                       static_cast<double>(-line3_pt3) };
            
         double sum_xy_line3 = 0.0;
         double sum_x_line3 = 0.0;
         double sum_y_line3 = 0.0;
         double mean_x_line3 = 0.0;
         double mean_y_line3 = 0.0;
         double sum_x_squared_line3 = 0.0;
         double sum_squared_x_line3 = 0.0;   
         double b_line3;
         double a_line3;
         double num_elements_line3 = 3.0;
            
         for(ossim_int32 i = 0; i < num_elements_line3; ++i)
         {
            sum_xy_line3 = sum_xy_line3
               + (line3RegPtsSamples[i]*line3RegPtsLines[i]);
               
            sum_x_line3 = sum_x_line3 + line3RegPtsSamples[i];
            sum_y_line3 = sum_y_line3 + line3RegPtsLines[i];
               
            sum_squared_x_line3 = sum_squared_x_line3
               + line3RegPtsSamples[i]*line3RegPtsSamples[i];
               
         }
            
         sum_x_squared_line3 = sum_x_line3*sum_x_line3; 
         mean_y_line3 = sum_y_line3/num_elements_line3;
         mean_x_line3 = sum_x_line3/num_elements_line3;
         b_line3 = (sum_xy_line3 - (sum_x_line3*sum_y_line3)/num_elements_line3)
            / (sum_squared_x_line3 - (sum_x_squared_line3/num_elements_line3));
         a_line3 = mean_y_line3 - b_line3*mean_x_line3;
            
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG) << "\n\n a_line3:  " << a_line3 <<std::endl;
            ossimNotify(ossimNotifyLevel_DEBUG) << " b_line3:  " << b_line3 <<std::endl;
               
               
            for(ossim_int32 i = 0; i < num_elements_line3; ++i)
            {
                  
               ossimNotify(ossimNotifyLevel_DEBUG) << setprecision(15) << "line3RegPtsLines[" << i << "]:  "
                                                   <<line3RegPtsLines[i]
                                                   << "\nline3RegPtsSamples[" << i << "]:  "
                                                   <<line3RegPtsSamples[i]
                                                   << std::endl;
            }
         }
         //End regression line3
            
         //Intersect lines 2 and 3 to get the lr corner vertex
            
         double lr_x = (a_line3 - a_line2)/(b_line2 - b_line3);
         double lr_y = -(a_line3 + b_line3*lr_x);
            
         //End Regression Line3
            
         //Regression Line 4
            
         ossim_int32 line4_pt0 = leftMostSampleLine;
         ossim_int32 line4_pt4 = topLine;
         ossim_int32 line4_pt2 = (line4_pt0 + line4_pt4)/2;
         ossim_int32 line4_pt1 = (line4_pt2 + line4_pt0)/2;
         ossim_int32 line4_pt3 = (line4_pt2 + line4_pt4)/2;
            
//             //***
//                // Find the line within 100 lines of line2_ptx that has the maximum
//                // sample.  This will cut off the 'fringe'.
//                //***
            
         ossim_int32 maxValueLine4Pt1;
         ossim_int32 maxOffsetLine4Pt1;
         getMaxAndIndex(&theLeftEdge[line4_pt1-100],&theLeftEdge[line4_pt1+100],
                        maxValueLine4Pt1, maxOffsetLine4Pt1 );
         line4_pt1 = line4_pt1-100 + maxOffsetLine4Pt1;
            
            
         ossim_int32 maxValueLine4Pt2;
         ossim_int32 maxOffsetLine4Pt2;
         getMaxAndIndex(&theLeftEdge[line4_pt2-100],&theLeftEdge[line4_pt2+100],
                        maxValueLine4Pt2, maxOffsetLine4Pt2 );
         line4_pt2 = line4_pt2-100 + maxOffsetLine4Pt2;
            
            
         ossim_int32 maxValueLine4Pt3;
         ossim_int32 maxOffsetLine4Pt3;
         getMaxAndIndex(&theLeftEdge[line4_pt3-100],&theLeftEdge[line4_pt3+100],
                        maxValueLine4Pt3, maxOffsetLine4Pt3 );
         line4_pt3 = line4_pt3-100 + maxOffsetLine4Pt3;
            
            
            
            
         double line4RegPtsSamples[] = { static_cast<double>(theLeftEdge[line4_pt1]),
                                         static_cast<double>(theLeftEdge[line4_pt2]),
                                         static_cast<double>(theLeftEdge[line4_pt3]) };
         double line4RegPtsLines[] = { static_cast<double>(-line4_pt1),
                                       static_cast<double>(-line4_pt2),
                                       static_cast<double>(-line4_pt3) };
            
         double sum_xy_line4 = 0.0;
         double sum_x_line4 = 0.0;
         double sum_y_line4 = 0.0;
         double mean_x_line4 = 0.0;
         double mean_y_line4 = 0.0;
         double sum_x_squared_line4 = 0.0;
         double sum_squared_x_line4 = 0.0;   
         double b_line4;
         double a_line4;
         double num_elements_line4 = 3.0;
         for(ossim_int32 i = 0; i < num_elements_line4; ++i)
         {
            sum_xy_line4 = sum_xy_line4
               + (line4RegPtsSamples[i]*line4RegPtsLines[i]);
               
            sum_x_line4 = sum_x_line4 + line4RegPtsSamples[i];
            sum_y_line4 = sum_y_line4 + line4RegPtsLines[i];
               
            sum_squared_x_line4 = sum_squared_x_line4
               + line4RegPtsSamples[i]*line4RegPtsSamples[i];
               
               
         }
            
         sum_x_squared_line4 = sum_x_line4*sum_x_line4; 
         mean_y_line4 = sum_y_line4/num_elements_line4;
         mean_x_line4 = sum_x_line4/num_elements_line4;
         b_line4 = (sum_xy_line4 - (sum_x_line4*sum_y_line4)/num_elements_line4)
            / (sum_squared_x_line4 - (sum_x_squared_line4/num_elements_line4));
         a_line4 = mean_y_line4 - b_line4*mean_x_line4;
            
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)<<"\na_line4:  "<< a_line4
                                               <<"\nb_line4:  "<< b_line4 <<std::endl;
               
            for(ossim_int32 i = 0; i < num_elements_line4; ++i)
            {
                  
               ossimNotify(ossimNotifyLevel_DEBUG) << setprecision(15) << "line4RegPtsLines[" << i << "]:  "
                                                   <<line4RegPtsLines[i]
                                                   << "\nline4RegPtsSamples[" << i << "]:  "
                                                   <<line4RegPtsSamples[i]
                                                   << std::endl;  
                  
            }
         }
         //End regression line4
            
         //Intersect lines 3 and 4 to get the ll corner vertex
            
         double ll_x = (a_line4 - a_line3)/(b_line3 - b_line4);
         double ll_y = -(a_line4 + b_line4*ll_x);
            
         //End Regression Line4
            
         //Intersect lines 4 and 1 to get the ul corner vertex
            
         double ul_x = (a_line4 - a_line1)/(b_line1 - b_line4);
         double ul_y = -(a_line4 + b_line4*ul_x);
            
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)<<"\n\nInitial corners for tilted right:   "
                                               <<"\ncorner0:  ("<<ul_x<<", "<<ul_y<<")"
                                               <<"\ncorner1:  ("<<ur_x<<", "<<ur_y<<")"
                                               <<"\ncorner2:  ("<<lr_x<<", "<<lr_y<<")"
                                               <<"\ncorner3:  ("<<ll_x<<", "<<ll_y<<")"<<std::endl<<std::endl;
         }
            
         leftCornerLine_A = (ossim_int32)ul_y;
         leftCorner_A = (ossim_int32)ul_x; //0
            
         rightCornerLine_A = (ossim_int32)ur_y;
         rightCorner_A = (ossim_int32)ur_x; //1
            
         rightCornerLine_B = (ossim_int32)lr_y;
         rightCorner_B =(ossim_int32)lr_x; //2
            
         leftCornerLine_B = (ossim_int32)ll_y;
         leftCorner_B =(ossim_int32)ll_x; //3
            
            
         // Check for bad corner points due to irregular shape image
            
         //for corner 0 (ul), sample should be between left and right most samples
         //                   line can go negative due to jagged edges 
            
         //for corner 1 (ur), sample can go > than num samples
         //                   line should be between top and bottom lines
            
         //for corner 2 (lr), line should be between top and bottom line + 200
         //                   sample should be between left and right most sample
            
         //for corner 3 (ll), line should be between top and bottom line
         //                   sample should can go negative due to jagged edges
            
         if(        (leftCornerLine_A < (topLine - 300))
                    || (leftCornerLine_A > total_lines)
                    || (leftCorner_A > rightMostSample)
                    || (leftCorner_A < leftMostSample)
                       
                    || (rightCornerLine_A < topLine)
                    || (rightCornerLine_A > bottomLine)
                    || (rightCorner_A < leftMostSample)
                    || (rightCorner_A > (rightMostSample + 200))
                       
                       
                    || (rightCornerLine_B < topLine)
                    || (rightCornerLine_B > (bottomLine + 200))
                    || (rightCorner_B < leftMostSample)
                    || (rightCorner_B > rightMostSample)
                       
                    || (leftCornerLine_B > bottomLine)   
                    || (leftCornerLine_B < topLine)
                    || (leftCorner_B > rightMostSample)   
                    || (leftCorner_B < (leftMostSample - 200))    )
               
               
               
         {
            right = false;
            if (traceDebug())
            {
               ossimNotify(ossimNotifyLevel_DEBUG) <<"\n\n***Case = NOT TILTED RIGHT...***"<<std::endl;
            }
         }
      }           
      
      //tilted left
      if(firstRightSample > lastRightSample)
      {//begin tilted left
         
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG) <<"\n\n***Case = TILTED LEFT***"<<std::endl;
         }
         left = true;
         
         // Regression line 1 is the right side
         // Regression line 3 is the left side
         // These are handled differently
         // These sides may be 'uneven' or 'torn' or 'louvered'
         // This code finds the best regression line that 'cuts off' this edge fringe
         
         
         //Regression Line 1
         
         ossim_int32 line1_pt4 = rightMostSampleLine;
         ossim_int32 line1_pt0 = topLine;
         ossim_int32 line1_pt2 = (line1_pt0 + line1_pt4)/2;
         ossim_int32 line1_pt1 = (line1_pt2 + line1_pt0)/2;
         ossim_int32 line1_pt3 = (line1_pt2 + line1_pt4)/2;
         
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)<<" line1_pt1:   "<<line1_pt0<<std::endl;
            ossimNotify(ossimNotifyLevel_DEBUG)<<" line1_pt2:   "<<line1_pt1<<std::endl;
            ossimNotify(ossimNotifyLevel_DEBUG)<<" line1_pt3:   "<<line1_pt2<<std::endl;
            ossimNotify(ossimNotifyLevel_DEBUG)<<" line1_pt4:   "<<line1_pt3<<std::endl;
         }
         
         ossim_int32 minValueLine1Pt1;
         ossim_int32 minOffsetLine1Pt1;
         getMinAndIndex(&theLeftEdge[line1_pt1-100],&theLeftEdge[line1_pt1+100],
                        minValueLine1Pt1, minOffsetLine1Pt1 );
         line1_pt1 = line1_pt1-100 + minOffsetLine1Pt1;
         
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG) << " line1_pt1:  "<< line1_pt1
                                                <<"   minValueLine1Pt1:  "<<minValueLine1Pt1<<"\n\n";
         }
         
         ossim_int32 minValueLine1Pt2;
         ossim_int32 minOffsetLine1Pt2;
         getMinAndIndex(&theLeftEdge[line1_pt2-100],&theLeftEdge[line1_pt2+100],
                        minValueLine1Pt2, minOffsetLine1Pt2 );
         line1_pt2 = line1_pt2-100 + minOffsetLine1Pt2;
         
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG) << " line1_pt2:  "<< line1_pt2
                                                <<"   minValueLine1Pt2:  "<<minValueLine1Pt2<<"\n\n";
         }
         
         
         ossim_int32 minValueLine1Pt3;
         ossim_int32 minOffsetLine1Pt3;
         getMinAndIndex(&theLeftEdge[line1_pt3-100],&theLeftEdge[line1_pt3+100],
                        minValueLine1Pt3, minOffsetLine1Pt3 );
         line1_pt3 = line1_pt3-100 + minOffsetLine1Pt3;
         
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG) << " line1_pt3:  "<< line1_pt3
                                                <<"   minValueLine1Pt3:  "<<minValueLine1Pt3<<std::endl<<std::endl;
         }
         
         double line1RegPtsSamples[] = { static_cast<double>(theRightEdge[line1_pt1]),
                                         static_cast<double>(theRightEdge[line1_pt2]),
                                         static_cast<double>(theRightEdge[line1_pt3])};
         double line1RegPtsLines[] = { static_cast<double>(-line1_pt1),
                                       static_cast<double>(-line1_pt2),
                                       static_cast<double>(-line1_pt3) };
         
         double sum_xy_line1 = 0.0;
         double sum_x_line1 = 0.0;
         double sum_y_line1 = 0.0;
         double mean_x_line1 = 0.0;
         double mean_y_line1 = 0.0;
         double sum_x_squared_line1 = 0.0;
         double sum_squared_x_line1 = 0.0;   
         double b_line1;
         double a_line1;
         double num_elements_line1 = 3.0;
         
         for(ossim_int32 i = 0; i < num_elements_line1; ++i)
         {
            sum_xy_line1 = sum_xy_line1
               + (line1RegPtsSamples[i]*line1RegPtsLines[i]);
            
            sum_x_line1 = sum_x_line1 + line1RegPtsSamples[i];
            sum_y_line1 = sum_y_line1 + line1RegPtsLines[i];
            
            sum_squared_x_line1 = sum_squared_x_line1
               + line1RegPtsSamples[i]*line1RegPtsSamples[i];
            
            
         }
         
         sum_x_squared_line1 = sum_x_line1*sum_x_line1; 
         mean_y_line1 = sum_y_line1/num_elements_line1;
         mean_x_line1 = sum_x_line1/num_elements_line1;
         b_line1 = (sum_xy_line1 - (sum_x_line1*sum_y_line1)/num_elements_line1)
            / (sum_squared_x_line1 - (sum_x_squared_line1/num_elements_line1));
         a_line1 = mean_y_line1 - b_line1*mean_x_line1;
         
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)<<"a_line1:  "<< a_line1
                                               <<"\nb_line1:  "<< b_line1 <<std::endl;
            
            for(ossim_int32 i = 0; i < num_elements_line1; ++i)
            {
               
               ossimNotify(ossimNotifyLevel_DEBUG) << setprecision(15) << "line1RegPtsLines[" << i << "]:  "
                                                   <<line1RegPtsLines[i]
                                                   << "\nline1RegPtsSamples[" << i << "]:  "
                                                   <<line1RegPtsSamples[i]
                                                   << std::endl;
            }
         }
         
         
         //Regression Line 2
         
         ossim_int32 line2_pt0 = rightMostSampleLine;
         ossim_int32 line2_pt4 = bottomLine;
         ossim_int32 line2_pt2 = (line2_pt0 + line2_pt4)/2;
         ossim_int32 line2_pt1 = (line2_pt2 + line2_pt0)/2;
         ossim_int32 line2_pt3 = (line2_pt2 + line2_pt4)/2;
      
         double line2RegPtsSamples[] = { static_cast<double>(theRightEdge[line2_pt1]),
                                         static_cast<double>(theRightEdge[line2_pt2]),
                                         static_cast<double>(theRightEdge[line2_pt3]) };
         double line2RegPtsLines[] = { static_cast<double>(-line2_pt1),
                                       static_cast<double>(-line2_pt2),
                                       static_cast<double>(-line2_pt3) };
      
         double sum_xy_line2 = 0.0;
         double sum_x_line2 = 0.0;
         double sum_y_line2 = 0.0;
         double mean_x_line2 = 0.0;
         double mean_y_line2 = 0.0;
         double sum_x_squared_line2 = 0.0;
         double sum_squared_x_line2 = 0.0;   
         double b_line2;
         double a_line2;
         double num_elements_line2 = 3.0;
         for(ossim_int32 i = 0; i < num_elements_line2; ++i)
         {
            sum_xy_line2 = sum_xy_line2
               + (line2RegPtsSamples[i]*line2RegPtsLines[i]);
         
            sum_x_line2 = sum_x_line2 + line2RegPtsSamples[i];
            sum_y_line2 = sum_y_line2 + line2RegPtsLines[i];
         
            sum_squared_x_line2 = sum_squared_x_line2
               + line2RegPtsSamples[i]*line2RegPtsSamples[i];
         }
      
         sum_x_squared_line2 = sum_x_line2*sum_x_line2; 
         mean_y_line2 = sum_y_line2/num_elements_line2;
         mean_x_line2 = sum_x_line2/num_elements_line2;
         b_line2 = (sum_xy_line2 - (sum_x_line2*sum_y_line2)/num_elements_line2)
            / (sum_squared_x_line2 - (sum_x_squared_line2/num_elements_line2));
         a_line2 = mean_y_line2 - b_line2*mean_x_line2;
      
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG) << "a_line2:  "<< a_line2
                                                << "\nb_line2:  "<< b_line2 <<std::endl;
         
            for(ossim_int32 i = 0; i < num_elements_line2; ++i)
            {
            
               ossimNotify(ossimNotifyLevel_DEBUG) << setprecision(15) << "line2RegPtsLines[" << i << "]:  "
                                                   <<line2RegPtsLines[i]
                                                   << "\nline2RegPtsSamples[" << i << "]:  "
                                                   <<line2RegPtsSamples[i]
                                                   << std::endl;
            } 
         }
         //End regression line2
      
         //Intersect lines 1 and 2 to get the lr corner vertex
      
         double lr_x = (a_line1 - a_line2)/(b_line2 - b_line1);
         double lr_y = -(a_line1 + b_line1*lr_x);
      
         //Regression Line 3
      
         ossim_int32 line3_pt0 = leftMostSampleLine;
         ossim_int32 line3_pt4 = bottomLine;
         ossim_int32 line3_pt2 = (line3_pt0 + line3_pt4)/2;
         ossim_int32 line3_pt1 = (line3_pt2 + line3_pt0)/2;
         ossim_int32 line3_pt3 = (line3_pt2 + line3_pt4)/2;
      
         ossim_int32 maxValueLine3Pt1;
         ossim_int32 maxOffsetLine3Pt1;
         getMaxAndIndex(&theLeftEdge[line3_pt1-100],&theLeftEdge[line3_pt1+100],
                        maxValueLine3Pt1, maxOffsetLine3Pt1 );
         line3_pt1 = line3_pt1-100 + maxOffsetLine3Pt1;
      
      
         ossim_int32 maxValueLine3Pt2;
         ossim_int32 maxOffsetLine3Pt2;
         getMaxAndIndex(&theLeftEdge[line3_pt2-100],&theLeftEdge[line3_pt2+100],
                        maxValueLine3Pt2, maxOffsetLine3Pt2 );
         line3_pt2 = line3_pt2-100 + maxOffsetLine3Pt2;
      
      
         ossim_int32 maxValueLine3Pt3;
         ossim_int32 maxOffsetLine3Pt3;
         getMaxAndIndex(&theLeftEdge[line3_pt3-100],&theLeftEdge[line3_pt3+100],
                        maxValueLine3Pt3, maxOffsetLine3Pt3 );
         line3_pt3 = line3_pt3-100 + maxOffsetLine3Pt3;
      
      
         double line3RegPtsSamples[] = { static_cast<double>(theLeftEdge[line3_pt1]),
                                         static_cast<double>(theLeftEdge[line3_pt2]),
                                         static_cast<double>(theLeftEdge[line3_pt3]) };
         double line3RegPtsLines[] = { static_cast<double>(-line3_pt1),
                                       static_cast<double>(-line3_pt2),
                                       static_cast<double>(-line3_pt3) };
      
         double sum_xy_line3 = 0.0;
         double sum_x_line3 = 0.0;
         double sum_y_line3 = 0.0;
         double mean_x_line3 = 0.0;
         double mean_y_line3 = 0.0;
         double sum_x_squared_line3 = 0.0;
         double sum_squared_x_line3 = 0.0;   
         double b_line3;
         double a_line3;
         double num_elements_line3 = 3.0;
         for(ossim_int32 i = 0; i < num_elements_line3; ++i)
         {
            sum_xy_line3 = sum_xy_line3
               + (line3RegPtsSamples[i]*line3RegPtsLines[i]);
         
            sum_x_line3 = sum_x_line3 + line3RegPtsSamples[i];
            sum_y_line3 = sum_y_line3 + line3RegPtsLines[i];
         
            sum_squared_x_line3 = sum_squared_x_line3
               + line3RegPtsSamples[i]*line3RegPtsSamples[i];
         
         
         }
      
         sum_x_squared_line3 = sum_x_line3*sum_x_line3; 
         mean_y_line3 = sum_y_line3/num_elements_line3;
         mean_x_line3 = sum_x_line3/num_elements_line3;
         b_line3 = (sum_xy_line3 - (sum_x_line3*sum_y_line3)/num_elements_line3)
            / (sum_squared_x_line3 - (sum_x_squared_line3/num_elements_line3));
         a_line3 = mean_y_line3 - b_line3*mean_x_line3;
      
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)<<"\n\n a_line3:  "<< a_line3 <<std::endl
                                               <<" b_line3:  "<< b_line3 <<endl;
            for(ossim_int32 i = 0; i < num_elements_line3; ++i)
            {
            
               ossimNotify(ossimNotifyLevel_DEBUG) << setprecision(15) << "line3RegPtsLines[" << i << "]:  "
                                                   << line3RegPtsLines[i]
                                                   << "\nline3RegPtsSamples[" << i << "]:  "
                                                   << line3RegPtsSamples[i]
                                                   << std::endl;
            }   
         }
         //End regression line3
      
         //Intersect lines 2 and 3 to get the ll corner vertex
      
         double ll_x = (a_line3 - a_line2)/(b_line2 - b_line3);
         double ll_y = -(a_line3 + b_line3*ll_x);      
      
         //End Regression Line3
      
         //Regression Line 4
      
         ossim_int32 line4_pt0 = leftMostSampleLine;
         ossim_int32 line4_pt4 = topLine;
         ossim_int32 line4_pt2 = (line4_pt0 + line4_pt4)/2;
         ossim_int32 line4_pt1 = (line4_pt2 + line4_pt0)/2;
         ossim_int32 line4_pt3 = (line4_pt2 + line4_pt4)/2;
      
      
         double line4RegPtsSamples[] = { static_cast<double>(theLeftEdge[line4_pt1]),
                                         static_cast<double>(theLeftEdge[line4_pt2]),
                                         static_cast<double>(theLeftEdge[line4_pt3])};
         double line4RegPtsLines[] = { static_cast<double>(-line4_pt1),
                                       static_cast<double>(-line4_pt2),
                                       static_cast<double>(-line4_pt3) };
      
         double sum_xy_line4 = 0.0;
         double sum_x_line4 = 0.0;
         double sum_y_line4 = 0.0;
         double mean_x_line4 = 0.0;   
         double mean_y_line4 = 0.0;
         double sum_x_squared_line4 = 0.0;
         double sum_squared_x_line4 = 0.0;   
         double b_line4;
         double a_line4;
         double num_elements_line4 = 3.0;
         for(ossim_int32 i = 0; i < num_elements_line4; ++i)
         {
            sum_xy_line4 = sum_xy_line4
               + (line4RegPtsSamples[i]*line4RegPtsLines[i]);
         
            sum_x_line4 = sum_x_line4 + line4RegPtsSamples[i];
            sum_y_line4 = sum_y_line4 + line4RegPtsLines[i];
         
            sum_squared_x_line4 = sum_squared_x_line4
               + line4RegPtsSamples[i]*line4RegPtsSamples[i];
         
         
         }
      
         sum_x_squared_line4 = sum_x_line4*sum_x_line4; 
         mean_y_line4 = sum_y_line4/num_elements_line4;
         mean_x_line4 = sum_x_line4/num_elements_line4;
         b_line4 = (sum_xy_line4 - (sum_x_line4*sum_y_line4)/num_elements_line4)
            / (sum_squared_x_line4 - (sum_x_squared_line4/num_elements_line4));
         a_line4 = mean_y_line4 - b_line4*mean_x_line4;
      
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)<< "\n\n a_line4:  "<< a_line4 <<std::endl;
            ossimNotify(ossimNotifyLevel_DEBUG)<< " b_line4:  "<< b_line4 <<std::endl;
         
            for(ossim_int32 i = 0; i < num_elements_line4; ++i)
            {
               ossimNotify(ossimNotifyLevel_DEBUG) << setprecision(15) << "line4RegPtsLines[" << i << "]:  "
                                                   <<line4RegPtsLines[i]
                                                   << "\nline4RegPtsSamples[" << i << "]:  "
                                                   <<line4RegPtsSamples[i]
                                                   << std::endl;
            }                  
         
         }
         //End regression line 4
      
         //Intersect lines 3 and 4 to get the ul corner vertex
      
         double ul_x = (a_line4 - a_line3)/(b_line3 - b_line4);
         double ul_y = -(a_line4 + b_line4*ul_x);
         //End Regression Line4
      
         //Intersect lines 4 and 1 to get the ur corner vertex
      
         double ur_x = (a_line4 - a_line1)/(b_line1 - b_line4);
         double ur_y = -(a_line4 + b_line4*ur_x);
      
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG) <<"Initial corners for tilted left:   "
                                                <<"\ncorner0:  ("<<ul_x<<", "<<ul_y<<")"
                                                <<"\ncorner1:  ("<<ur_x<<", "<<ur_y<<")"
                                                <<"\ncorner2:  ("<<lr_x<<", "<<lr_y<<")"
                                                <<"\ncorner3:  ("<<ll_x<<", "<<ll_y<<")" <<std::endl;
         }
      
         leftCornerLine_A = (ossim_int32)ul_y;
         leftCorner_A = (ossim_int32)ul_x; //0
      
         rightCornerLine_A = (ossim_int32)ur_y;
         rightCorner_A = (ossim_int32)ur_x; //1
      
         rightCornerLine_B = (ossim_int32)lr_y;
         rightCorner_B =(ossim_int32)lr_x; //2
      
         leftCornerLine_B = (ossim_int32)ll_y;
         leftCorner_B =(ossim_int32)ll_x; //3
      
         // Check for bad corner points due to irregular shape image
      
         //for corner 0 (ul), line should be between top and bottom line
         //                   sample should can go negative due to jagged edges 
      
         //for corner 1 (ur), sample should be between left and right most samples
         //                    line can go negative due to jagged edges 
      
         //for corner 2 (lr), line should be between top and bottom lines
         //                   sample can be greater than rightMost samp due to jagged edges
      
         //for corner 3 (ll), line should be between top and bottom line + 200
         //                   sample should be between left and right most sample
         if(        (leftCornerLine_A  > bottomLine)
                    || (leftCornerLine_A  < topLine)
                    || (leftCorner_A > rightMostSample)
                    || (leftCorner_A < (leftMostSample - 300))
                 
                    || (rightCornerLine_A < (topLine - 300))
                    || (rightCornerLine_A > total_lines)
                    || (rightCorner_A > rightMostSample)
                    || (rightCorner_A < leftMostSample)
                 
                 
                    || (rightCornerLine_B < topLine)
                    || (rightCornerLine_B > bottomLine)
                    || (rightCorner_B < leftMostSample)
                    || (rightCorner_B > (rightMostSample + 200))
                 
                    || (leftCornerLine_B > (bottomLine + 200))   
                    || (leftCornerLine_B < topLine)
                    || (leftCorner_B < leftMostSample)   
                    || (leftCorner_B > rightMostSample)    )
         
         {
            left = false;
         
            if (traceDebug())
            {
               ossimNotify(ossimNotifyLevel_DEBUG) <<"\n\n***Case = NOT TILTED LEFT..***"<<std::endl;
            }
         }
      }
   }

   if( (square == 0) && (rhombus == 0) && (right == 0) && (left == 0))
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)<<"\nDefault case"<<std::endl;
      }
      
      //Suboptimal UL Corner:
      leftCornerLine_A = (topLine);
      leftCorner_A =(leftMostSample); //0
      
      //Look for non-null starting at topLine and going down
      //Starting at left most samp and moving right
      
      bool foundULcornera = false;
      bool foundULcornerb = false;
      ossimImageHandler* src = reinterpret_cast<ossimImageHandler*>(getInput(0));
      
      ossimIrect tileRectUL(leftMostSample,
                            topLine,
                            leftMostSample+50,
                            (topLine + 50) );
      
      ossimRefPtr<ossimImageData> dataUL = src->getTile(tileRectUL);
      // ossimIrect imageRectangleUL = dataUL->getImageRectangle();
      
      ossim_int32 tempSamp_corner0a = 0;
      ossim_int32 tempLine_corner0a = 0;
      if (dataUL.valid())
      {
         for (ossim_int32 line = tileRectUL.ul().y; line <= tileRectUL.lr().y; ++line)
         {
            for (ossim_int32 samp = tileRectUL.ul().x; samp <=tileRectUL.ur().x; ++samp)
            {
               // uses band 0 corner points
               if(!(dataUL->isNull(ossimIpt(samp,line),0)))
               {
                  
                  foundULcornera = true;
                  
                  //Optimal Corner:
                  leftCornerLine_A = line;//0
                  leftCorner_A = samp;
                  tempLine_corner0a = line;
                  tempSamp_corner0a = samp;
                  
                  if (traceDebug())
                  {
                     ossimNotify(ossimNotifyLevel_DEBUG)
                        <<"\nFound non null in ul corner pass 1 "<<std::endl
                        <<"line:  "<<line<<std::endl
                        <<"samp:  "<<samp<<std::endl;
                  }
               }
               if(foundULcornera == true){break;}
            }
            if(foundULcornera == true){break;}
         }

         // Search in the other direction to handle the case of 'jaggered' edges.
         ossim_int32 tempSamp_corner0b = 0;
         ossim_int32 tempLine_corner0b = 0;
         for (ossim_int32 samp = tileRectUL.ul().x; samp <=tileRectUL.ur().x; ++samp)
         {
            for (ossim_int32 line = tileRectUL.ul().y; line <= tileRectUL.lr().y; ++line)
            {
               // uses band 0 corner points
               if(!(dataUL->isNull(ossimIpt(samp,line),0)))
               {
                  
                  foundULcornerb = true;
                  
                  //Optimal Corner:
                  leftCornerLine_A = line;//0
                  leftCorner_A = samp;
                  tempLine_corner0b = line;
                  tempSamp_corner0b = samp;
                  
                  if (traceDebug())
                  {
                     ossimNotify(ossimNotifyLevel_DEBUG)
                        <<"\nFound non null in ul corner pass 2 "<<std::endl
                        <<"line:  "<<line<<std::endl
                        <<"samp:  "<<samp<<std::endl;
                  }
               }
               if(foundULcornerb == true){break;}
            }
            if(foundULcornerb == true){break;}
         }
         
         //Better corner
         if(foundULcornerb == true && foundULcornera ==true)
         {
            leftCornerLine_B = (tempLine_corner0a + tempLine_corner0b)/2;
            leftCorner_B     = (tempSamp_corner0a + tempSamp_corner0b)/2;
         }
         
         
         // Suboptimal UR corner:
         rightCornerLine_A = (topLine);
         rightCorner_A = (rightMostSample); //1

      }
      //Look for non-null starting at topLine and going down
      //Starting at right most samp and moving left 
         
      bool foundURcornera = false;
      bool foundURcornerb = false;
      
      ossimIrect tileRectUR((rightMostSample - 50),
                            topLine,
                            rightMostSample,
                            (topLine + 50) );
      
      ossimRefPtr<ossimImageData> dataUR = src->getTile(tileRectUR);
      // ossimIrect imageRectangleUR = dataUR->getImageRectangle();

      ossim_int32 tempSamp_corner1a = 0;
      ossim_int32 tempLine_corner1a = 0;

      if (dataUR.valid())
      {
         for (ossim_int32 line = tileRectUR.ul().y; line <= tileRectUR.lr().y; ++line)
         {
            for (ossim_int32 samp = tileRectUR.ur().x; samp > tileRectUR.ul().x; --samp)
            {
               
               if(!(dataUR->isNull(ossimIpt(samp,line),0)))
               {
                  
                  foundURcornera = true;
                  rightCornerLine_A = line;
                  rightCorner_A = samp;
                  tempLine_corner1a = line;
                  tempSamp_corner1a = samp;
                  
                  if (traceDebug())
                  {
                     ossimNotify(ossimNotifyLevel_DEBUG)
                        <<"\nFound non null in ur corner pass 1 "<<std::endl
                        <<"line:  "<<line<<std::endl
                        <<"samp:  "<<samp<<std::endl;
                  }
               }
               if(foundURcornera == true){break;}
            }
            if(foundURcornera == true){break;}
         }
         
         // Search in the other direction to handle the case of 'jaggered' edges.
         ossim_int32 tempSamp_corner1b = 0;
         ossim_int32 tempLine_corner1b = 0;
         
         for (ossim_int32 samp = tileRectUR.ur().x; samp > tileRectUR.ul().x; --samp)
         {
            for (ossim_int32 line = tileRectUR.ul().y; line <= tileRectUR.lr().y; ++line)
               
            {
               if(!(dataUR->isNull(ossimIpt(samp,line),0)))
               {
                  
                  foundURcornerb = true;
                  rightCornerLine_A = line;
                  rightCorner_A = samp;
                  tempLine_corner1b = line;
                  tempSamp_corner1b = samp;
                  
                  if (traceDebug())
                  {
                     ossimNotify(ossimNotifyLevel_DEBUG)
                        <<"\nFound non null in ur corner pass 2 "<<std::endl
                        <<"line:  "<<line<<std::endl
                        <<"samp:  "<<samp<<std::endl;
                  }
               }
               if(foundURcornerb == true){break;}
            }
            if(foundURcornerb == true){break;}
         }
         
         //Better corner
         if(foundURcornerb == true && foundURcornera ==true)
         {
            rightCornerLine_B = (tempLine_corner1a + tempLine_corner1b)/2;
            rightCorner_B     = (tempSamp_corner1a + tempSamp_corner1b)/2;
         }
         
         // Suboptimal LR corner:
         rightCornerLine_B =  (bottomLine);
         rightCorner_B = (rightMostSample); //2

      }

      //Look for non-null starting at bottomLine and going up
      //Starting at right most samp and moving left 
      
      bool foundLRcornera = false;
      bool foundLRcornerb = false;

      ossimIrect tileRectLR((rightMostSample - 50),
                            bottomLine - 50,
                            rightMostSample,
                            (bottomLine) );
      
      ossimRefPtr<ossimImageData> dataLR = src->getTile(tileRectLR);
      // ossimIrect imageRectangleLR = dataLR->getImageRectangle();
      
      ossim_int32 tempSamp_corner2a = 0;
      ossim_int32 tempLine_corner2a = 0;

      if (dataLR.valid())
      {
         for (ossim_int32 line = tileRectLR.lr().y; line >= tileRectLR.ur().y; --line)
         {
            for (ossim_int32 samp = tileRectLR.lr().x; samp > tileRectLR.ul().x; --samp)
            {
               
               if(!(dataLR->isNull(ossimIpt(samp,line),0)))
               {
                  
                  foundLRcornera = true;
                  rightCornerLine_B = line;
                  rightCorner_B = samp;
                  tempLine_corner2a = line;
                  tempSamp_corner2a = samp;
                  
                  if (traceDebug())
                  {
                     ossimNotify(ossimNotifyLevel_DEBUG)
                        <<"\nFound non null in lr corner pass 1 "<<std::endl
                        <<"line:  "<<line<<std::endl
                        <<"samp:  "<<samp<<std::endl;
                  }
               }
               if(foundLRcornera == true){break;}
            }
            if(foundLRcornera == true){break;}
         }
         // Search in the other direction to handle the case of 'jaggered' edges.
         
         ossim_int32 tempSamp_corner2b = 0;
         ossim_int32 tempLine_corner2b = 0;
         
         for (ossim_int32 samp = tileRectLR.lr().x; samp > tileRectLR.ul().x; --samp)
         {
            for (ossim_int32 line = tileRectLR.lr().y; line >= tileRectLR.ur().y; --line)
            { 
               if(!(dataLR->isNull(ossimIpt(samp,line),0)))
               {
                  
                  foundLRcornerb = true;
                  rightCornerLine_B = line;
                  rightCorner_B = samp;
                  tempLine_corner2b = line;
                  tempSamp_corner2b = samp;
                  
                  if (traceDebug())
                  {
                     ossimNotify(ossimNotifyLevel_DEBUG)
                        <<"\nFound non null in lr corner pass 2"<<std::endl
                        <<"line:  "<<line<<std::endl
                        <<"samp:  "<<samp<<std::endl;
                  }
               }
               if(foundLRcornerb == true){break;}
            }
            if(foundLRcornerb == true){break;}
         }
         //Better corner
         if(foundLRcornerb == true && foundLRcornera ==true)
         {
            rightCornerLine_B = (tempLine_corner2a + tempLine_corner2b)/2;
            rightCorner_B     = (tempSamp_corner2a + tempSamp_corner2b)/2;
         }
         // Suboptimal LL corner:
         leftCornerLine_B = (bottomLine);
         leftCorner_B = (leftMostSample); //3

      }

      //Look for non-null starting at bottomLine and going up
      //Starting at left most samp and moving right 
      
      bool foundLLcornera = false;
      bool foundLLcornerb = false;
      
      ossimIrect tileRectLL((leftMostSample),
                            (bottomLine - 50),
                            (leftMostSample +50),
                            (bottomLine) );
      
      ossimRefPtr<ossimImageData> dataLL = src->getTile(tileRectLL);
      // ossimIrect imageRectangleLL = dataLL->getImageRectangle();
      
      ossim_int32 tempSamp_corner3a = 0;
      ossim_int32 tempLine_corner3a = 0;

      if (dataLL.valid())
      {
         for (ossim_int32 samp = tileRectLL.ul().x; samp <= tileRectLL.ur().x; ++samp)
         {
            
            for (ossim_int32 line = tileRectLL.lr().y; line >= tileRectLL.ul().y; --line)
               
            {
               
               if(!(dataLL->isNull(ossimIpt(samp,line),0)))
               {
                  
                  foundLLcornera = true;
                  leftCornerLine_B = line;
                  leftCorner_B = samp;
                  tempLine_corner3a = line;
                  tempSamp_corner3a = samp;
                  
                  if (traceDebug())
                  {
                     ossimNotify(ossimNotifyLevel_DEBUG)
                        <<"\nFound non null in ll corner pass 1"<<std::endl
                        <<"line:  "<<line<<std::endl
                        <<"samp:  "<<samp<<std::endl;
                  }
               }
               if(foundLLcornera == true){break;}
            }
            if(foundLLcornera == true){break;}
         }
         
         // Search in the other direction to handle the case of 'jaggered' edges.
         
         ossim_int32 tempSamp_corner3b = 0;
         ossim_int32 tempLine_corner3b = 0;
         for (ossim_int32 line = tileRectLL.lr().y; line >= tileRectLL.ul().y; --line)
         {
            
            for (ossim_int32 samp = tileRectLL.ul().x; samp <= tileRectLL.ur().x; ++samp)
               
            {
               
               if(!(dataLL->isNull(ossimIpt(samp,line),0)))
               {
                  
                  foundLLcornerb = true;
                  leftCornerLine_B = line;
                  leftCorner_B = samp;
                  tempLine_corner3b = line;
                  tempSamp_corner3b = samp;
                  
                  if (traceDebug())
                  {
                     ossimNotify(ossimNotifyLevel_DEBUG)
                        <<"\nFound non null in ll corner pass 2 "<<std::endl
                        <<"line:  "<<line<<std::endl
                        <<"samp:  "<<samp<<std::endl;
                  }
               }
               if(foundLLcornerb == true){break;}
            }
            if(foundLLcornerb == true){break;}
         }
         
         //Better corner
         if(foundLLcornerb == true && foundLLcornera ==true)
         {
            leftCornerLine_B = (tempLine_corner3a + tempLine_corner3b)/2;
            leftCorner_B     = (tempSamp_corner3a + tempSamp_corner3b)/2;
         }
      }
   }
   
   theVertice[0].x = leftCorner_A;
   theVertice[0].y = leftCornerLine_A;
   
   theVertice[1].x = rightCorner_A;
   theVertice[1].y = rightCornerLine_A;
   
   theVertice[3].x = leftCorner_B;
   theVertice[3].y = leftCornerLine_B;
   
   theVertice[2].x = rightCorner_B;
   theVertice[2].y = rightCornerLine_B;
   
   if (leftSlope)
   {
      delete [] leftSlope;
      leftSlope = 0;
   }
   if (rightSlope)
   {
      delete [] rightSlope;
      rightSlope = 0;
   }
   
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)<<"\nleftMostSample:      " << leftMostSample
                                         <<"\nleftMostSampleLine:  " << leftMostSampleLine
                                         <<"\n\nrightMostSample:   " << rightMostSample
                                         <<"\nrightMostSampleLine: " << rightMostSampleLine
         
                                         <<"\n\nfirstLeftSample:   " << firstLeftSample
                                         <<"\nfirstRightSample:    " << firstRightSample
         
                                         <<"\n\nlastLeftSample:    " << lastLeftSample 
                                         <<"\nlastRightSample:     " << lastRightSample
         
                                         <<"\n\ntopLine:           " << topLine
                                         <<"\nbottomLine:          " << bottomLine           
                                         <<"\ntotal_lines:         " << total_lines
         
                                         << "\n\n\ntheVertice[0]:      " << theVertice[0]
                                         << "\ntheVertice[1]:      " << theVertice[1]
                                         << "\ntheVertice[2]:      " << theVertice[2]
                                         << "\ntheVertice[3]:      " << theVertice[3]
                                         << "\n\nreturning..." << std::endl;
   }
   
   return true;
}

bool ossimVertexExtractor::writeVertices()
{
   static const char MODULE[] = "ossimVertexExtractor::writeVertices";

   if (traceDebug()) CLOG << " Entered..." << endl;
   
   if (!isOpen())
   {
      // Try to open.
      if (!open()) return false;
   }

   // Write the points...
   theFileStream << "point0.x:  " << theVertice[0].x
                 << "\npoint0.y:  " << theVertice[0].y
                 << "\npoint1.x:  " << theVertice[1].x
                 << "\npoint1.y:  " << theVertice[1].y
                 << "\npoint2.x:  " << theVertice[2].x
                 << "\npoint2.y:  " << theVertice[2].y
                 << "\npoint3.x:  " << theVertice[3].x
                 << "\npoint3.y:  " << theVertice[3].y
                 << endl;

   // Close the file...
   close();

   return true;
}


void ossimVertexExtractor::setOutputName(const ossimString& filename)
{
   ossimOutputSource::setOutputName(filename);

   if (isOpen()) close();
   
   theFilename = filename;
}

void ossimVertexExtractor::setAreaOfInterest(const ossimIrect& rect)
{
   theAreaOfInterest = rect;
}

bool ossimVertexExtractor::isOpen()const
{
   return const_cast<ofstream*>(&theFileStream)->is_open();
}

bool ossimVertexExtractor::open()
{
   if(isOpen())
   {
      close();
   }

   if (theFilename == ossimFilename::NIL)
   {
      ossimNotify(ossimNotifyLevel_WARN) << "ossimVertexExtractor::open WARNING:"
                                          << "\nOutput file has not been set.  Call setFilename() method..."
                                          << std::endl;
      return false;
   }
   
   theFileStream.open(theFilename.c_str());
         
   return theFileStream.good();
}

void ossimVertexExtractor::close()
{
   if (isOpen()) theFileStream.close();
}

template<class T1>
void ossimVertexExtractor::getMinAndIndex(T1* start, T1* end,
                                          T1& minValue, ossim_int32& offsetFromStart)
{
   
   offsetFromStart =0;
   ossim_int32 currentOffset = 0;
   minValue  = *start;
   while(start < end)
   {
      if(*start < minValue)
      {
         
         minValue = *start;
         offsetFromStart = currentOffset;
      }
      ++start;
      ++currentOffset;
   }
}

template<class T2>
void ossimVertexExtractor::getMaxAndIndex(T2* start, T2* end,
                                          T2& maxValue, ossim_int32& offsetFromStart)
{
   
   offsetFromStart =0;
   ossim_int32 currentOffset = 0;
   maxValue  = *start;
   while(start < end)
   {
      if(*start > maxValue)
      {
         maxValue = *start;
         offsetFromStart = currentOffset;
      }
      ++start;
      ++currentOffset;
   }
}
