//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License 
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software 
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author: Ken Melero (kmelero@imagelinks.com)
//         Orginally developed by:
//                   Copyright (c) 1997 TargetJr Consortium
//               GE Corporate Research and Development (GE CRD)
//                             1 Research Circle
//                            Niskayuna, NY 12309
//         Adapted from:  IUE v4.1.2
// Description: 
//      A Histogram contains an array of "buckets", which represent finite
// segments of some value axis, along with a corresponding array of
// frequency counts for each of these buckets.
//
//********************************************************************
// $Id: ossimHistogram.h,v 1.11 2005/09/01 13:59:47 gpotts Exp $
//

#ifndef ossimHistogram_HEADER
#define ossimHistogram_HEADER
#include <base/common/ossimObject.h>
#include "ossimString.h"
#include "ossimFilename.h"
#include "ossimKeywordlist.h"
class OSSIMDLLEXPORT ossimHistogram : public ossimObject
{
  private:

    mutable int stats_consistent; // A 2 bit state flag  Mean =1 | StandDev = 2

  protected:

   virtual void deleteAll();
   
    float * vals;            // histogram x array
                             // (value = midpoint of each bucket)
    float * counts;          // histogram y array ie. count[i] is
                             // the number of pixels with value within range
                             // of bucket i

    int num;                 // number of indices

    float delta;             // "Width" of each bucket on value axis
    float vmin, vmax;        // Maximum and minimum values on plot
    mutable float mean;               // Mean value of the distribution
    mutable float standard_dev;       // 
  protected:


   class ossimProprietaryHeaderInformation
   {
   public:
      ossimProprietaryHeaderInformation(){clear();}

      bool parseStream(istream& in);

      long getNumberOfBins()
         {
            return theNumberOfBins.toLong();
         }
      void clear()
         {
            theFileType      = "";
            theVersion       = "";
            theMapperType    = "";
            theNumberOfBins  = "";
         }
      ossimString theFileType;
      ossimString theVersion;
      ossimString theMapperType;
      ossimString theNumberOfBins;
   };
   
  public:
// Constructors
    ossimHistogram();
    ossimHistogram(int xres, float min, float max);
    ossimHistogram(float*, float*, int);
    ossimHistogram(const ossimHistogram& his); // Copy constructor
    ossimHistogram(const ossimHistogram*, float width); // Resampling constructor

    virtual int GetIndex(float)const;
// Other histogram formation operations    
    ossimHistogram* Scale(float scale_factor); // Scale Transformation
    ossimHistogram* CumulativeGreaterThanEqual()const;// From density to cumulative
    ossimHistogram* CumulativeLessThanEqual()const;// From density to cumulative
    //Suppress non-peak values.
   ossimHistogram* NonMaximumSupress(int radius = 1, bool cyclic = false);
   void create(int xres, float val1, float val2);
  
// Attribute accessors
    void UpCount(float newval);
    float GetCount(float uval)const;
    float SetCount(float pixelval, float count);

    float GetMinVal()const;
    float GetMaxVal()const;
    float GetMaxCount()const;

    float GetRangeMin()const
    {
       return vmin;
    }
    float GetRangeMax()const
    {
       return vmax;
    }
    float * GetVals()
    {
	stats_consistent = 0; // Values might change.
	return vals; 
    }
    const float * GetVals()const
    {
	stats_consistent = 0; // Values might change.
	return vals; 
    }

    float * GetCounts()
    { 
	stats_consistent = 0; // Counts might change.
	return counts; 
    }

   const float * GetCounts()const
    { 
	stats_consistent = 0; // Counts might change.
	return counts; 
    }

    int GetRes()const
    { return num; }

    float GetBucketSize()const { return delta; }

    float * GetMinValAddr()
    { return vals+GetIndex(GetMinVal());  }

    float * GetMinCountAddr()
    { return counts+GetIndex(GetMinVal());  }

   const float * GetMinValAddr()const
    { return vals+GetIndex(GetMinVal());  }

    const float * GetMinCountAddr()const
    { return counts+GetIndex(GetMinVal());  }

    float ComputeArea(float low, float high)const;// bounded area
    float ComputeArea()const;//total area

    /*!
     * Returns the fraction of accumulation up to and including "val" bucket
     * from min divided by the total accumulation.
     * returns OSSIM_FLT_NAN if "val" is not between GetMinVal and GetMaxVal.
     */
    float getLowFractionFromValue(float val) const;
    
    /*!
     * Returns the fraction of accumulation down to and including "val" bucket
     * from max divided by the total accumulation.
     * returns OSSIM_FLT_NAN if "val" is not between GetMin() and GetMax().
     */
    float getHighFractionFromValue(float val) const;
    
    //Find bounds that clip off a given percent of the area
    float LowClipVal(float clip_fraction)const;
    float HighClipVal(float clip_fraction)const;

    int GetValIndex(float val)const;

    float GetMean()const;
    float GetStandardDev()const;

    void Print()const;
    void Dump(char *)const;
    int  WritePlot(const char* fname)const;
   virtual ~ossimHistogram();


   virtual bool importHistogram(const ossimFilename& inputFile);
   virtual bool importHistogram(istream& in);
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);
TYPE_DATA   
};

#endif

