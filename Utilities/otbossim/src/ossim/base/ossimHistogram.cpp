//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Ken Melero (kmelero@imagelinks.com)
//         Orginally developed by:
//                   Copyright (c) 1997 TargetJr Consortium
//               GE Corporate Research and Development (GE CRD)
//                             1 Research Circle
//                            Niskayuna, NY 12309
//         Adapted from:  IUE v4.1.2
// Description: 
//      A ossimHistogram contains an array of "buckets", which represent finite
// segments of some value axis, along with a corresponding array of
// frequency counts for each of these buckets.
//
//********************************************************************
// $Id: ossimHistogram.cpp 22124 2013-01-27 16:03:52Z dburken $

#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimHistogram.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/base/ossimThinPlateSpline.h>
#include <ossim/base/ossimDpt.h>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;



// nonstandard versions that use operator>, so they behave differently
// than std:::min/max and ossim::min/max.  kept here for now for that
// reason.
#ifndef MAX
#  define MAX(x,y) ((x)>(y)?(x):(y))
#  define MIN(x,y) ((x)>(y)?(y):(x))
#endif


static const int MEAN_FLAG = 1, SD_FLAG = 2;
RTTI_DEF1(ossimHistogram, "ossimHistogram", ossimObject);
ossimHistogram::ossimHistogram()
   :
   stats_consistent(MEAN_FLAG | SD_FLAG),
   vals(new float [1]),
   counts(new float [1]),
   num(0),
   delta(0.0),
   vmin(0),
   vmax(0),
   mean(0.0),
   standard_dev(0.0)
{
   vals[0] = 0.0;
   counts[0] = 0.0;
}

ossimHistogram::ossimHistogram(int xres, float val1, float val2)
   :
   stats_consistent(MEAN_FLAG | SD_FLAG),
   vals(new float [xres]),
   counts(new float [xres]),
   num(xres),
   delta(0.0),
   vmin(0),
   vmax(0),
   mean(0.0),
   standard_dev(0.0)
{
   vmax = MAX(val1, val2);
   vmin = MIN(val1, val2);
   delta = (vmax - vmin + 1) / xres;
   mean = (float)((vmax + vmin)/2.0);
   standard_dev = (float)((vmax - vmin)/(2.0*sqrt(3.0)));
   int i = 0;

   if (vals == NULL || counts == NULL)
   {
      fprintf(stderr, "Histogram : Ran out of memory for arrays.\n");
      vals = NULL;
      counts = NULL;
      num = 0;
      vmin = 0;
      vmax = 0;
      delta = 0.0;
   }
   else
   {
      for(i = 0; i < xres; i++)
      {
         vals[i] = vmin + delta * (float)(i + 0.5);
         counts[i] = 0.0;
      }
   }
}

ossimHistogram::ossimHistogram(float* uvals, float* ucounts, int xres)
   :
   stats_consistent(MEAN_FLAG | SD_FLAG),
   vals(uvals),
   counts(ucounts),
   num(xres),
   delta(0.0),
   vmin(0),
   vmax(0),
   mean(0.0),
   standard_dev(0.0)
{
   if ( ( xres >= 2 ) && uvals && ucounts )
   {
      delta = vals[1] - vals[0]; // Changed this from delta = 1.0
      //  vmax = GetMaxVal();
      //  vmin = GetMinVal(); JAF version
      vmin = uvals[0] - .5f*delta;
      vmax = uvals[num-1] + .5f*delta;
      mean = GetMean();
      standard_dev = GetStandardDev();
   }   
}
//-----------------------------------------------------------
// -- Copy constructor
ossimHistogram::ossimHistogram(const ossimHistogram& his)
{

   int i = 0;
   num = his.GetRes();

   vals = new float[num];
   const float* his_vals = his.GetVals();

   counts = new float[num];
   const float* his_counts = his.GetCounts();

   if (vals == NULL || counts == NULL)
   {
      fprintf(stderr, "Histogram : Ran out of memory for arrays.\n");
      vals = NULL;
      counts = NULL;
      num = 0;
      vmin = 0;
      vmax = 0;
      delta = 0.0;
      stats_consistent = 0;
      return;
   }

   mean = his.GetMean();
   standard_dev = his.GetStandardDev();

   for(i=0; i<num; i++)
   {
      vals[i] = his_vals[i];
      counts[i] = his_counts[i];
   }
   vmax = his.GetMaxVal();
   vmin = his.GetMinVal();
   delta = his.GetBucketSize();

   stats_consistent = 0;
   stats_consistent |= (MEAN_FLAG | SD_FLAG);
}


//---------------------------------------
// -- Resample a histogram

ossimHistogram::ossimHistogram(const ossimHistogram* his, float width)
{

   stats_consistent =0;

// Attributes of original histogram

   float del = his->GetBucketSize();
   int max_index = his->GetRes() - 1;
   float minvalue = his->GetVals()[0] - del*.5f;
   float maxvalue = his->GetVals()[max_index] + del*.5f;


// Intialize a new histogram
   if(width == del) num = his->GetRes();
   else if(!(width == 0.0))
      num = (int)ceil((maxvalue - minvalue)/width);
   else
      num = 1; // This shouldn't happen anyway.

   vals = new float [num];
   counts = new float [num];
   delta = width;
   float mean_val = (maxvalue + minvalue)/2.0f;
   float half_range = (num * delta)/2.0f;
   vmax =  mean_val + half_range;
   vmin =  mean_val - half_range;
   int i = 0;

   if (vals == NULL || counts == NULL)
   {
      fprintf(stderr,
              "Histogram : Ran out of memory for arrays.\n");
      vals = NULL;
      counts = NULL;
      num = 0;
      vmin = 0;
      vmax = 0;
      delta = 0.0;
      mean = 0.0;
      standard_dev = 0.0;
      stats_consistent |= (MEAN_FLAG | SD_FLAG);
      return;
       
   }
   
   else
   {
      for(i = 0; i < num; i++)
      {
         vals[i] = vmin + delta * (i + 0.5f);
         counts[i] = 0.0;
      }
   }


// Cases:


   if(width == del)    // Then just copy his
   {
      const float* his_counts = his->GetCounts();
      for(i=0; i<num; i++)
         counts[i] = his_counts[i];
      mean = GetMean();
      standard_dev = GetStandardDev();
      stats_consistent |= (MEAN_FLAG | SD_FLAG);
      return;
   }


   if(del > width)     // Then interpolate his counts.
   {

// Boundary conditions:
//    Start
      float his_start = minvalue + .5f*del;
      float start = vmin + .5f*delta;
      float c0 = his->GetCount(his_start);
      float c1 = his->GetCount(his_start + del);
      float s0 = (c1 - c0)/del;

      for(float x = start; x <= (his_start + del + delta);)
      {
         float interp = s0 * (x - his_start) + c0;
         if(interp < 0) interp = 0; //Can be negative
         SetCount(x,interp);
         x += width;
      }
//    End
      float his_end = maxvalue - .5f*del;
      float end = vmax - .5f*delta;
      float cn = his->GetCount(his_end);
      float cn_1 = his->GetCount(his_end - del);
      float sn = (cn_1 - cn)/del;

      for(float y = end; y >= (his_end - del + delta);)
      {
         float interp = sn * (his_end - y) + cn;
         if(interp < 0) interp = 0; //Can be negative
         SetCount(y, interp);
         y -= delta;
      }
// Interior Loop

      for(float z = his_start + del; z <= (his_end - del);)
      {
         float ci = his->GetCount(z);
         float ci_1 = his->GetCount(z-del);
         float cip1 = his->GetCount(z+del);
         float deriv = (cip1 - ci_1)/(2.0f*del);
         float second_drv =
            ((cip1 + ci_1)/2.0f - ci)/(del*del);
         int fine_x_index = GetIndex(z);
         if (fine_x_index < 0)
         {
            if (z<vmin) fine_x_index = 0;
            else fine_x_index = num-1;
         }
         float fine_x = vals[fine_x_index];
         for(float xfine = fine_x; xfine < z + del;)
         {
            float interp = ci + deriv*(xfine -z) +
               second_drv*(xfine - z)*(xfine - z);

            if(interp < 0) interp = 0; //Can be negative
            SetCount(xfine, interp);
            xfine += width;
         }
         z += del;
      }
   }


   if(del < width)    //Just accumulate samples from his into larger bins
   {
      if( del != 0.0){
         float his_start = minvalue + .5f*del;
         float his_end = maxvalue - .5f*del;
         for(float x = his_start; x <= his_end;)
         {
            SetCount(x, (GetCount(x) + his->GetCount(x)));
            x += del;
         }
      }
   }
   mean = GetMean();
   standard_dev = GetStandardDev();
   stats_consistent =0;
   stats_consistent |= (MEAN_FLAG | SD_FLAG);
}

void ossimHistogram::create(int xres, float val1, float val2)
{
   // clear all the data
   deleteAll();

   // now set it up and initialize;
   xres = xres >0? xres:1;
   
   vals   = new float [xres];
   counts = new float [xres];
   num = xres;
   vmax = MAX(val1, val2);
   vmin = MIN(val1, val2);

   delta = (vmax - vmin) / xres;
   mean = (float)((vmax + vmin)/2.0);
   standard_dev = (float)((vmax - vmin)/(2.0*sqrt(3.0)));
   stats_consistent = 0;
   stats_consistent |= (MEAN_FLAG | SD_FLAG);
   int i = 0;
   if (vals == NULL || counts == NULL)
   {
      ossimNotify(ossimNotifyLevel_FATAL) << "Histogram : Ran out of memory for arrays.\n";
      vals = NULL;
      counts = NULL;
      num = 0;
      vmin = 0;
      vmax = 0;
      delta = 0.0;
   }
   else
   {
      for(i = 0; i < xres; i++)
      {
         vals[i] = vmin + delta * (float)(i + 0.5);
         counts[i] = 0.0;
      }
   }   
}
ossimHistogram* ossimHistogram::fillInteriorEmptyBins(int type)const
{
   ossimHistogram* result = new ossimHistogram(*this);
   if(num < 1) return 0;
   switch(type)
   {
      case HISTOGRAM_FILL_THIN_PLATE:
      case HISTOGRAM_FILL_DEFAULT:
      {
         ossimThinPlateSpline spline(1);
         double pvars[1];
         float* new_counts = result->GetCounts();
         ossim_int32 idxLeft = 0;
         ossim_int32 idxRight = num-1;
         while((idxLeft < num) && (new_counts[idxLeft]  == 0.0))++idxLeft;
         while((idxRight > -1) && (new_counts[idxRight] == 0.0))--idxRight;
         
         if(idxLeft < idxRight)
         {
            ossim_int32 idx = idxLeft;
            while(idx <= idxRight)
            {
               if(new_counts[idx]!=0.0)
               {
                  pvars[0] = new_counts[idx];
                  spline.addPoint(idx, 0, pvars);
               }
               ++idx;
            }
            if(spline.solve())
            {
               idx = idxLeft;
               while(idx <= idxRight)
               {
                  if(spline.getPoint(idx, 0, pvars))
                  {
                     new_counts[idx] = pvars[0];
                  }
                  ++idx;
               }
            }
         }
         
         break;
      }
   }
   
   return result;
}
//--------------------------------------------------
// -- Transform the value axis of a histogram by a
//    translation, transl, and a scale factor, scale.
//    The new histogram has the same resolution as his.

ossimHistogram* ossimHistogram::Scale(float scale_factor)
{

// Extract attributes of self

//    float lowvalue = vals[0];
   float highvalue = vals[num-1];

// Construct a new histogram

   ossimHistogram* scaled_his = new ossimHistogram(this, delta);
   float* new_counts = scaled_his->GetCounts();
   int i = 0;
   for(i=0; i < num; i++)  // Initialize
      new_counts[i] = 0.0;

// Compute scaled values
// We assume that the new histogram is to be scaled down from his

   float scale = scale_factor;
   if(scale_factor > 1.0) scale = 1.0;

   for(float x = highvalue; x > vmin;)
   {
      float trans_x = (x-vmin)*scale + vmin; // Scaled x.
      int index = GetIndex(trans_x);
      if (index < 0)
      {
         if (trans_x<vmin) index = 0;
         else index = num-1;
      }
      float fraction = (trans_x - vals[index])/delta;
      float abs_fraction = (float)fabs(fraction);
      int x_index = GetIndex(x);
      if (x_index < 0)
      {
         if (x<vmin) x_index = 0;
         else x_index = num-1;
      }

// Distribute the counts in proportion

      new_counts[index] += (1.0f - abs_fraction)*counts[x_index];
      if(fraction > 0)
         if(index < (num-1))
            new_counts[index + 1] +=
               abs_fraction*counts[x_index];
         else
            new_counts[index] +=
               abs_fraction*counts[x_index];
      else
         if(index > 0)
            new_counts[index - 1] +=
               abs_fraction*counts[x_index];
         else
            new_counts[index] +=
               abs_fraction*counts[x_index];
      x -= delta;
   }

// Compute new Histogram attributes

   mean = scaled_his->GetMean();
   standard_dev = scaled_his->GetStandardDev();
   return scaled_his;
}

//---------------------------------------------------------------------
// -- Assuming that "this" is a histogram of population density,
//    construct a new histogram which is the cumulative distribution.
//    Each bin, xi, in his is assumed to represent a density, i.e.,
//            {x | (xi - .5*delta) < x <= (xi + .5*delta)}
//    Each bin, xi, in the result represents a cumulative distribution, i.e.,
//            {x | x <= (xi + .5*delta)}
ossimHistogram* ossimHistogram::CumulativeGreaterThanEqual()const
{
   ossimHistogram* cum_his = new ossimHistogram(*this);
   const float* density_counts = this->GetCounts();
   int res = this->GetRes();

   // Intitialize cumulative counts
   float* cum_counts = cum_his->GetCounts();
   int i = 0;
   for(i=0; i < res; i++)
      cum_counts[i] = 0;
  
   cum_counts[res-1] = density_counts[res-1];
   for(i = res-2; i>=0; --i)
   {
      cum_counts[i] += (density_counts[i] + cum_counts[i+1]);
   }

   return cum_his;
}

ossimHistogram* ossimHistogram::CumulativeLessThanEqual()const
{
   ossimHistogram* cum_his = new ossimHistogram(*this);
   const float* density_counts = this->GetCounts();
   int res = this->GetRes();

   // Intitialize cumulative counts
   float* cum_counts = cum_his->GetCounts();
   int i = 0;
   for(i=0; i < res; i++)
      cum_counts[i] = 0;

   cum_counts[0] = density_counts[0];
   for(i = 1; i < res; i++)
   {
      cum_counts[i] += (density_counts[i] + cum_counts[i-1]);
   }
  
   return cum_his;
}

//Provides the correct values for histogram counts when the bin index 
//extends outside the valid range of the counts array.  This function
//permits easy array access logic for the NonMaximumSuppression algorithm.
//The cyclic flag indicates that the counts array index is circular, i.e,
//cnts[0] equivalent to cnts[n_bins-1]
inline float GetExtendedCount(int bin, int n_bins, float* cnts, bool cyclic)
{
   int nbm = n_bins-1;
   if(!cyclic)
   {
      if(bin < 0)
         return cnts[0];
      if(bin >= n_bins)
         return cnts[nbm];
   }
   else
   {
      if(bin<0)
         return cnts[nbm+bin];
      if(bin >= n_bins)
         return cnts[bin-n_bins];
   }
   return cnts[bin];
}
//Prune any sequences of more than one maxium value
//That is, it is possible to have a "flat" top peak with an arbitarily
//long sequence of equal, but maximum values. The cyclic flag indictates
//that the sequence wraps around, i.e. cnts[0] equivalent to cnts[nbins-1]
inline void RemoveFlatPeaks(int nbins, float* cnts, bool cyclic)
{
   int nbm = nbins-1;

   //Here we define a small state machine - parsing for runs of peaks
   //init is the state corresponding to an initial run (starting at i ==0)
   bool init=GetExtendedCount(0, nbins, cnts, cyclic);
   int init_end =0;

   //start is the state corresponding to any other run of peaks
   bool start=false;  
   int start_index=0; 
   int i = 0;

   //The scan of the state machine
   for(i = 0; i < nbins; i++)
   {
      float v = GetExtendedCount(i, nbins, cnts, cyclic);

      //State init: a string of non-zeroes at the begining.
      if(init&&v!=0)
         continue;

      if(init&&v==0)
      {
         init_end = i;
         init = false;
         continue;
      }

      //State !init&&!start: a string of "0s"
      if(!start&&v==0)
         continue;

      //State !init&&start: the first non-zero value
      if(!start&&v!=0)
      {
         start_index = i;
         start = true;
         continue;
      }
      //State ending flat peak: encountered a subsequent zero after starting
      if(start&&v==0)
      {
         int peak_location = (start_index+i-1)/2;//The middle of the run
         int k = 0;
         for(k = start_index; k<=(i-1); k++)
	    if(k!=peak_location)
               cnts[k] = 0;
         start = false;
      }
   }
   //Now handle the boundary conditions
   //The non-cyclic case
   if(!cyclic)
   {
      if(init_end!=0)  //Was there an initial run of peaks?
      {
         int init_location = (init_end-1)/2;
         int k = 0;
         for(k = 0; k<init_end; k++)
	    if(k!=init_location)
               cnts[k] = 0;
      }
      if(start)       // Did we reach the end of the array in a run of pks?
      {
         int end_location = (start_index + nbm)/2;
         int k = 0;
         for(k = start_index; k<nbins; k++)
	    if(k!=end_location)
               cnts[k] = 0;
      }
   }
   else  //The cyclic case
   {
      if(init_end!=0)  //Is there a run which crosses the cyclic cut?
      {
         if(start)    
         { //Yes, so define the peak location accordingly
	    int peak_location = (start_index + init_end - nbm -1)/2;
	    int k;
	    if(peak_location < 0) //Is the peak to the left of the cut?
            {// Yes, to the left
               peak_location += nbm; 
               for( k = 0; k< init_end; k++)
		  cnts[k]=0;
               for( k= start_index; k <nbins; k++)
		  if(k!=peak_location)
                     cnts[k] = 0;
            }
	    else   
            {//No, on the right.
               for( k = start_index; k< nbins; k++)
		  cnts[k]=0;
               for( k= 0; k < init_end; k++)
		  if(k!=peak_location)
                     cnts[k] = 0;
            }
         }
         else  
         {//There wasn't a final run so just clean up the initial run
	    int init_location = (init_end-1)/2;
	    int k = 0;
	    for(k = start_index; k<init_end; k++)
               if(k!=init_location)
                  cnts[k] = 0;
         }
      }
   }
}

//----------------------------------------------------------
// -- Suppress values in the Histogram which are not locally 
//    a maxium. The neighborhood for computing the local maximum
//    is [radius X radius], e.g. for radius =1 the neighborhood
//    is [-X-], for radius = 2, the neighborhood is [--X--], etc.
//    If the cyclic flag is true then the index space is assumed to
//    be equivalent to a circle. That is, elements "0" and (n_buckets-1)
//    are in correspondence.
ossimHistogram* ossimHistogram::NonMaximumSupress(int radius, bool cyclic)
{
   if((2*radius +1)> num/2)
   {
      ossimNotify(ossimNotifyLevel_WARN)<<"ossimHistogram::NonMaximumSupress the radius is too large \n";
      return NULL;
   }
   //Get the counts array of "this"
   ossimHistogram* h_new = new ossimHistogram(*this);
   int n_buckets = h_new->GetRes();
   float* counts_old = this->GetCounts();

   //Make a new Histogram for the suppressed version
   float* counts_new = h_new->GetCounts();
   int i;
   for( i =0; i < n_buckets; i++)
      counts_new[i] = 0;
  
   //Find local maxima
   for( i = 0; i<  n_buckets; i++)
   {
      //find the maxium value in the current kernel
      float max_count = counts_old[i];
      int k = 0;
      for(k = -radius; k <= radius ;k++)
      {
         int index = i+k;
         float c = GetExtendedCount(index, n_buckets, counts_old, cyclic);
         if( c > max_count)
	    max_count = c;
      }
      //Is position i a local maxium?
      if(max_count == counts_old[i])
         counts_new[i] = max_count;//Yes. So set the counts to the max value
   }
   RemoveFlatPeaks(n_buckets, counts_new, cyclic);
   return h_new;
}
//----------------------------------------------------------
// -- Compute the mean of the histogram population
float ossimHistogram::GetMean()const
{
   float xsum = 0.0;

   if(MEAN_FLAG&stats_consistent)
      return mean;
   else
   {
      if( this->GetBucketSize() > 0.0){
         for(float x=this->GetMinVal(); x<= this->GetMaxVal(); x +=this->GetBucketSize())
            xsum += x*GetCount(x);
      }

      float area = ComputeArea(vmin, vmax);
      if(area <= 0.0)
      {
         //	      fprintf(stderr, "Histogram : Area <= 0.0\n");
         return 0.0;
      }
      else
      {
         stats_consistent |=1;
         mean = xsum/area;
         return mean;
      }
   }
}



float ossimHistogram::GetStandardDev()const
{
   float sum = 0.0;

   if(SD_FLAG&stats_consistent)
      return standard_dev;
   else
   {
      float xm = this -> GetMean(); // Force an Update of Mean

      if( this->GetBucketSize() > 0.0){
         for(float x=this->GetMinVal();
             x<= this->GetMaxVal();
             x +=this->GetBucketSize())

            sum += (x-xm)*(x-xm)*GetCount(x);
      }

      float area = ComputeArea(vmin, vmax);
      if(area <= 0.0)
      {
         //	      fprintf(stderr, "Histogram : Area <= 0.0\n");
         return 0.0;
      }
      else
      {
         stats_consistent |= 2;
         standard_dev = (float)sqrt(sum/area);
         return standard_dev;
      }
   }
}

int ossimHistogram::GetIndex(float pixelval)const
{
   if ((pixelval > vmax) || (pixelval < vmin)||(num==0))
   {
      return -1;
   }
//   ossim_float32 d = vmax-vmin;
   int bandIdx = (ossim_int32)((pixelval-vmin)/delta);
   return bandIdx<GetRes()?bandIdx:-1;
//    if(bandIdx == num)
//    {
//       return num-1;
//    }
//    else if(bandIdx < num)
//    {
//       return bandIdx;
//    }
//    return -1;
#if 0
   if ((pixelval > vmax) || (pixelval < vmin))
      return -1;

   int idx = 0;
   int i = 0;

   for(i = 0; i < num; i++)
   {
      // RWMC: This is very dangerous - might get an intermediate
      // value which is between vals[i]+0.5*delta and
      // vals[i+1]-0.5*delta, which would then return index of 0.
      // Changed to check range one-sided, which is safe because of
      // previous check on range.
      //       if ((pixelval > (vals[i] - 0.5 * delta)) &&
      //           (pixelval <= (vals[i] + 0.5 * delta)))
      if (pixelval <= (vals[i] + 0.5 * delta))
      {
         idx = i;
         break;
      }
   }

   return idx;
#endif
}


int ossimHistogram::GetValIndex(float pixelval)const
{
   if ((pixelval > vmax) || (pixelval < vmin))
      return -1;

   int idx = 0;
   int i = 0;

   for(i = 0; i < num; i++)
   {
      if ((pixelval > (vals[i] - 0.5 * delta)) &&
          (pixelval <= (vals[i] + 0.5 * delta)))
      {
         idx = i;
         break;
      }
   }

   return idx;
}



float ossimHistogram::GetCount(float pixelval)const
{
   int index = GetIndex(pixelval);

   if (index < 0)
      return -1;
   else
      return counts[index];
}



float ossimHistogram::GetMinVal()const
{
   register int i=0;

   while (i<num-1 && !counts[i])
      i++;

   return vals[i];
}




float ossimHistogram::GetMaxVal()const
{
   register int i=num-1;

   while (i>0 && !counts[i])
      i--;

   if (i < 0)
      return 0.0;

   return vals[i];
}


float ossimHistogram::GetMaxCount()const
{
   register int i=0;
   float max;
   max = 0.0;
   for (i=0; i < num; i++)
      if (counts[i] > max)
         max = counts[i];
   return max;
}




float ossimHistogram::SetCount(float pixelval, float count)
{
   stats_consistent = 0;

   int index = GetIndex(pixelval);

   if (index < 0)
      return -1;
   else
   {
      counts[index] = count;
      return count;
   }
}


void ossimHistogram::UpCount(float pixelval)
{

   stats_consistent = 0;
   int idx = GetIndex(pixelval);
   if (idx >= 0)  // Originally (index > 0)
   {
      counts[idx] += 1.0;
   }
}

float ossimHistogram::ComputeArea(float low, float high)const
{
   float maxval = GetMaxVal();
   float minval = GetMinVal();

   if (low < minval) low = minval;
   if (high > maxval) high = maxval;

   if (low <= high)
   {
      int indexlow, indexhigh;
      indexlow = (int) GetIndex(low);
      if (indexlow < 0)
      {
         if (low<vmin) indexlow = 0;
         else indexlow = num-1;
      }
      indexhigh = (int) GetIndex(high);
      if (indexhigh < 0)
      {
         if (high<vmin) indexhigh = 0;
         else indexhigh = num-1;
      }
      register int i=indexlow;
      float sum = 0.0;

      while (i<=indexhigh)
      {
         sum+= counts[i];
         i++;
      }
      return sum;
   }
   else
   {
      //      fprintf(stderr, "Histogram : Range for ComputeArea is out of bounds.\n");
      return 0.0;
   }
}
//----------------------------------------------------------------------
// --Compute the total area under the histogram
//
float ossimHistogram::ComputeArea()const
{
   float vmin = this->GetMinVal();
   float vmax = this->GetMaxVal();
   if(vmin>vmax)
   {
      float temp = vmin;
      vmin = vmax;
      vmax = temp;
   }
   return this->ComputeArea(vmin, vmax);
}

float ossimHistogram::getLowFractionFromValue(float val) const
{
   
   float min = floor(GetMinVal());
   float max = ceil(GetMaxVal());
   if (val < min || val > max)
   {
      return ossim::nan();
   }

   int total_buckets = GetRes();
   int cutoff_bucket = GetValIndex(val);
   float partial_sum = 0.0;
   float total_sum   = 0.0;
   
   for(int i = 0; i < total_buckets; ++i)
   {
      total_sum += counts[i];
      if (i <= cutoff_bucket)
      {
         partial_sum += counts[i];
      }
   }

   return (partial_sum/total_sum);
}

float ossimHistogram::getHighFractionFromValue(float val) const
{
   float min = floor(GetMinVal());
   float max = ceil(GetMaxVal());
   if (val < min || val > max)
   {
      return ossim::nan();
   }

   int total_buckets = GetRes();
   int cutoff_bucket = GetValIndex(val);
   float partial_sum = 0.0;
   float total_sum   = 0.0;
   
   for(int i = (total_buckets-1); i >= 0; --i)
   {
      total_sum += counts[i];
      if (i >= cutoff_bucket)
      {
         partial_sum += counts[i];
      }
   }

   return (partial_sum/total_sum);
}

//----------------------------------------------------------------------
//  -- Finds the lower bound value which elminates a given fraction of
//     histogram area.
//
float ossimHistogram::LowClipVal(float clip_fraction)const
{
   if(clip_fraction<0) clip_fraction=0.0;
   if(clip_fraction>1.0) clip_fraction=1.0;
   float area = this->ComputeArea();
   if(area==0.0) return this->GetMinVal();
   if(clip_fraction==0.0) return this->GetMinVal();
   if(clip_fraction==1.0) return this->GetMaxVal();
   float clip_area = area*clip_fraction;
   const float* COUNTS = this->GetCounts();
   const float* VALS = this->GetVals();
   int res = this->GetRes();
   float sum = 0;
   int i=0;
   for(; i<res; i++)
   {
      sum+=COUNTS[i];
      if(sum>=clip_area)
         break;
   }
   return VALS[i];
}

//----------------------------------------------------------------------
//  -- Finds the lower bound value which elminates a given fraction of
//     histogram area.
//
float ossimHistogram::HighClipVal(float clip_fraction)const
{
   if(clip_fraction<0) clip_fraction=0.0;
   if(clip_fraction>1.0) clip_fraction=1.0;
   float area = this->ComputeArea();
   if(area==0.0) return this->GetMaxVal();
   if(clip_fraction==0.0) return this->GetMaxVal();
   if(clip_fraction==1.0) return this->GetMinVal();
   float clip_area = area*clip_fraction;
   const float* COUNTS = this->GetCounts();
   const float* VALS = this->GetVals();
   int res = this->GetRes();
   float sum = 0;
   int i = (res-1);
   for(; i>=0; i--)
   {
      sum+=COUNTS[i];
      if(sum>=clip_area)
         break;
   }
   return VALS[i];
}

//--------------------------------------------------------------------------
// -- Prints histogram counts onto cout
void ossimHistogram::Print()const
{
   ostream& out = ossimNotify(ossimNotifyLevel_INFO);
   const float* VALS = this->GetVals();
   const float* COUNTS = this->GetCounts();
   int res = this->GetRes();
   int width = 0;
   int i = 0;
   for(i =0; i < res; i++)
   {
      if(width++ > 5)
      {
         width = 0;
         out << "\n";
      }
      out << VALS[i] << " "
          << COUNTS[i] << " | " ;
   }
   out << "\n MaxVal " << this->GetMaxVal() << "\n";
   out << " MinVal " << this->GetMinVal() << "\n";
   out << " BucketSize " << this->GetBucketSize() << "\n";
   out << " Resolution " << this->GetRes() << "\n";
   out << " Area "
       << this->ComputeArea(this->GetMinVal(),this->GetMaxVal()) << "\n";
   out << "------------------------------------------------\n\n";
}

//---------------------------------------------------------------------------
// --- dumps histogram  values  to file.

void ossimHistogram::Dump(char *dumpfile)const
{
   FILE *dumpfp = fopen(dumpfile, "w");

   if (!dumpfp)
   {
      fprintf(stderr, "Error opening histogram data file.\n");
      return;
   }
   int i = 0;

   for(i = 0; i < num; i++)
      fprintf(dumpfp, "%f %f\n", vals[i], counts[i]);
  
   fclose(dumpfp);
   return;
}

//---------------------------------------------------------------------------
// -- Writes histogram in format suitable for plotting tools like Gnuplot.

int ossimHistogram::WritePlot(const char *fname)const
{
   FILE *fp = fopen(fname, "w");

   if (!fp)
   {
      fprintf(stderr, "Error opening histogram plot file.\n");
      return 0;
   }

   int stat_res = this->GetRes();

   float * x = new float[2*stat_res];
   float * y = new float[2*stat_res];

   const float * temp_x = this->GetVals();
   const float * temp_y = this->GetCounts();
   float     delt = this->GetBucketSize();

   for (register int i=0; i < stat_res ;i++)
   {
      x[2*i] = temp_x[i] - 0.5f * delt;
      x[2*i+1] = temp_x[i] + 0.5f * delt;
      y[2*i] = temp_y[i];
      y[2*i+1] = temp_y[i];
   }


   for(register int j = 0; j < 2*stat_res; j++)
      fprintf(fp, "%f %f\n", x[j], y[j]);

   delete [] x;
   delete [] y;

   fclose(fp);
   return 1;
}

void ossimHistogram::deleteAll()
{
   if (vals)
   {
      delete []vals;
      vals = NULL;
   }
   if (counts)
   {
      delete []counts;
      counts = NULL;
   }  
}

ossimHistogram::~ossimHistogram()
{
   deleteAll();
}


bool ossimHistogram::importHistogram(istream& in)
{
   ossimProprietaryHeaderInformation header;
   bool binsCreated = false;
   
   if(header.parseStream(in))
   {
      long numberOfBins = header.getNumberOfBins();
      
      if(numberOfBins)
      {
         create(numberOfBins, 0, numberOfBins - 1);
         binsCreated = true;

         if(binsCreated)
         {
            ossimString buffer;
            ossimString binNumber;
            ossimString count;

            while(in.good() &&
                  !in.eof() &&
                  *binNumber.c_str() != '.')
            {
               
               getline(in, buffer);

               istringstream s(buffer);

               s >> binNumber >> count;
               if(*binNumber.c_str() != (char)'.')
               {
                  SetCount((float)binNumber.toDouble(),
                           (float)count.toDouble());
               }
            }
         }
      }
      else
      {
         return false;
      }
   }  
   return true;
}

bool ossimHistogram::importHistogram(const ossimFilename& inputFile)
{
   if(inputFile.exists())
   {
      ifstream input(inputFile.c_str());

      return importHistogram(input);
   }

   return false;  
}


bool ossimHistogram::ossimProprietaryHeaderInformation::parseStream(istream& in)
{
   ossimString inputLine;
   
   getline(in, inputLine);  
   if(inputLine.find("File Type") != string::npos)
   {
      std::string::size_type index = inputLine.find(":");
      if(index != std::string::npos)
      {
         theFileType = inputLine.substr(index+1);
         theFileType = theFileType.trim();
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

   getline(in, inputLine);  
   if(inputLine.find("Version") != string::npos)
   {
      std::string::size_type index = inputLine.find(":");
      if(index != std::string::npos)
      {
         theVersion = inputLine.substr(index+1);
         theVersion = theVersion.trim();
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

   getline(in, inputLine);  
   if(inputLine.find("Mapper Type") != string::npos)
   {
      std::string::size_type index = inputLine.find(":");
      if(index != std::string::npos)
      {
         theMapperType = inputLine.substr(index+1);
         theMapperType = theMapperType.trim();
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

   getline(in, inputLine);  
   if(inputLine.find("Number of Bins") != string::npos)
   {
      std::string::size_type index = inputLine.find(":");
      if(index != std::string::npos)
      {
         theNumberOfBins = inputLine.substr(index+1);
         theNumberOfBins = theNumberOfBins.trim();
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
   
   return true;   
}

bool ossimHistogram::saveState(ossimKeywordlist& kwl,
                               const char* prefix)const
{
   kwl.add(prefix,
           "type",
           "ossimHistogram",
           true);
   kwl.add(prefix,
           "number_of_bins",
           num,
           true);
   kwl.add(prefix,
           "min_value",
           vmin,
           true);
   kwl.add(prefix,
           "max_value",
           vmax,
           true);
   


   ossimString binArrayList = "(";
   bool firstValue = true;

   for(ossim_int32 index = 0; index < num; ++index)
   {
      if(fabs(counts[index]) > FLT_EPSILON)
      {

         if(!firstValue)
         {
            binArrayList += ",";
         }
         else
         {
            firstValue = false;
         }
         binArrayList += "("+ossimString::toString(index)+","+ossimString::toString(counts[index])+")";
     }
   }

   binArrayList += ")";

   kwl.add(prefix, "bins", binArrayList, true);
#if 0
   ossimString binValue = "";
   for(ossim_int32 index = 0; index < num; ++index)
   {
      if(fabs(counts[index]) > FLT_EPSILON)
      {
         //     binValue = prefix;
         binValue = "bin";
         binValue += ossimString::toString(index);
         
         kwl.add(prefix,
                 binValue.c_str(),
                 counts[index],
                 true);
      }
   }
#endif   
   return true;
}

bool ossimHistogram::loadState(const ossimKeywordlist& kwl,
                               const char* prefix)
{
   const char* number_of_bins = kwl.find(prefix, "number_of_bins");

   if(number_of_bins)
   {
      ossim_uint32 bins = ossimString(number_of_bins).toUInt32();

      if(bins > 0)
      {
         // setup some defaults
         float minValue = 0;
         float maxValue = bins - 1;

         // see if there is a range set for the data
         const char* min_value = kwl.find(prefix, "min_value");
         const char* max_value = kwl.find(prefix, "max_value");

         if(min_value)
         {
            minValue = (ossim_float32)ossimString(min_value).toDouble();
         }
         if(max_value)
         {
            maxValue = (ossim_float32)ossimString(max_value).toDouble();
         }

         create((int)bins, minValue, maxValue);
         float* countsPtr = GetCounts();
         memset(countsPtr, '\0', bins*sizeof(float));
         // this is new style histogram creation
         //
         ossimString binsString = kwl.find(prefix, "bins");
         if(!binsString.empty())
         {
            std::vector<ossimDpt> result;
            ossim::toVector(result, binsString);
            if(!result.empty())
            {
               ossim_uint32 idx = 0;
               for(idx = 0; idx < result.size();++idx)
               {
                  ossim_uint32 binIdx = static_cast<ossim_uint32>(result[idx].x);
                  if(binIdx < bins)
                  {
                     countsPtr[binIdx] = result[idx].y;
                  }
               }
            }
         }
         else
         {
            ossimKeywordlist binsKwl;
            ossim_uint32 offset = (ossim_uint32)(ossimString(prefix)+"bin").size();
            ossimString regExpression =  ossimString("^(") + ossimString(prefix) + "bin[0-9]+)";
            kwl.extractKeysThatMatch(binsKwl,regExpression);
            const ossimKeywordlist::KeywordMap& kwlMap = binsKwl.getMap();
            ossimKeywordlist::KeywordMap::const_iterator iter = kwlMap.begin();
            while(iter != kwlMap.end())
            {
               ossimString numberStr(iter->first.begin() + offset,
                                     iter->first.end());
               countsPtr[numberStr.toUInt32()] = ossimString(iter->second).toDouble();
               ++iter;
            }
         }
         
         return true;
#if 0
         // create the bins
         ossimString binNumber = "";
         ossimString regExpression =  ossimString("^(") + ossimString(prefix) + "bin[0-9]+)";
         vector<ossimString> keys = kwl.getSubstringKeyList( regExpression );
         ossim_uint32 numberOfBins = (ossim_uint32)keys.size();
         ossim_uint32 offset = (ossim_uint32)(ossimString(prefix)+"bin").size();

         std::vector<ossim_uint32> theNumberList(numberOfBins);
         ossim_uint32 idx = 0;
         for(idx = 0; idx < theNumberList.size();++idx)
         {
            ossimString numberStr(keys[idx].begin() + offset,
                                  keys[idx].end());
            theNumberList[idx] = numberStr.toUInt32();

         }
         
         float* countsPtr = GetCounts();
         memset(countsPtr, '\0', bins*sizeof(float));
         for(idx = 0; idx < numberOfBins;++idx)
         {
            const char* binCount = kwl.find(prefix, ossimString("bin") + ossimString::toString(theNumberList[idx]));
            countsPtr[theNumberList[idx]] = (float)ossimString(binCount).toDouble();
         }
#endif
      }
   }
   return true;
}

bool ossimHistogram::loadState(const ossimRefPtr<ossimXmlNode> xmlNode)
{
   ossimRefPtr<ossimXmlNode> binValues =  xmlNode->findFirstNode("binValues");
   ossimRefPtr<ossimXmlNode> minValueNode  =  xmlNode->findFirstNode("minValue");
   ossimRefPtr<ossimXmlNode> maxValueNode  =  xmlNode->findFirstNode("maxValue");

   if(binValues.valid())
   {
      ossim_uint32 count = 0;
      float minValue = 0.0;
      float maxValue = 0.0;
      std::vector<float> floatValues;
      std::istringstream in(binValues->getText());
      ossimString vString;
      while(!in.fail())
      {
         in>>vString;
         if(!in.fail())
         {
            floatValues.push_back(vString.toFloat32());
         }
      }
      count = (ossim_uint32)floatValues.size();
     
      if(count)
      {
         minValue = 0;
         maxValue = count - 1;

         if(minValueNode.valid())
         {
            minValue = minValueNode->getText().toFloat32();
         }
         if(maxValueNode.valid())
         {
            maxValue = maxValueNode->getText().toFloat32();
         }

         create(count, minValue, maxValue);
         float* countsPtr = GetCounts();
         ossim_uint32 idx = 0;
         for(idx = 0; idx < count; ++idx)
         {
            countsPtr[idx] = floatValues[idx];
         }
         return true;
      }
   }

   return false;
}

bool ossimHistogram::saveState(ossimRefPtr<ossimXmlNode> xmlNode)const
{
   ossimRefPtr<ossimXmlNode> binValues = new ossimXmlNode;
   xmlNode->setTag("ossimHistogram");
   xmlNode->addChildNode("minValue", ossimString::toString(vmin));
   xmlNode->addChildNode("maxValue", ossimString::toString(vmax));
   xmlNode->addChildNode("standardDeviation", ossimString::toString(standard_dev));
   xmlNode->addChildNode("mean", ossimString::toString(mean));
   binValues->setTag("binValues");
   std::ostringstream out;

   ossim_int32 idx = 0;
   if(num > 0)
   {
      for(idx = 0; idx < num;++idx)
      {
         out << ossimString::toString(counts[idx], 8) << " ";
      }
      binValues->setText(out.str());
   }
   xmlNode->addChildNode(binValues.get());
   
   return true;
}
