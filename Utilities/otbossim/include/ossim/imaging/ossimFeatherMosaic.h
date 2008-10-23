//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
// 
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimFeatherMosaic.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimFeatherMosaic_HEADER
#define ossimFeatherMosaic_HEADER
#include <vector>
#include <iterator>

#include <ossim/imaging/ossimImageMosaic.h>
#include <ossim/base/ossimDpt.h>


/**
 * Performs a spatial blend accross overlapping regions
 */
class OSSIMDLLEXPORT ossimFeatherMosaic : public ossimImageMosaic
{
public:
   ossimFeatherMosaic();
   ossimFeatherMosaic(vector<ossimImageSource*>& inputSources);
   virtual ~ossimFeatherMosaic();
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& origin,
                                   ossim_uint32 resLevel=0);
   virtual void initialize();
       
protected:
   /**
    * This is only visible by this class and will remain here
    * unless we want to move it later.
    */
   class ossimFeatherInputInformation
   {
      
   public:
      ossimFeatherInputInformation(const vector<ossimIpt>& validVertices=vector<ossimIpt>());

      void setVertexList(const vector<ossimIpt>& validVertices);
      
      ossimDpt        theCenter;
      ossimDpt        theAxis1;
      ossimDpt        theAxis2;
      double          theAxis1Length;
      double          theAxis2Length;
      std::vector<ossimIpt> theValidVertices;
      
   };

   friend ostream& operator<<(ostream& out,
                              const ossimFeatherInputInformation& data);
   

   /**
    * will hold an array of input information
    *
    */
   ossimFeatherInputInformation* theInputFeatherInformation;
   ossimRefPtr<ossimImageData> theAlphaSum;
   ossimRefPtr<ossimImageData> theResult;
   
   /**
    * Will hold the count for the feather information list.
    */
   long                          theFeatherInfoSize;

   /**
    * The dummy variable is used for the template type.  See the getTile
    * method for the call.
    */
   template <class T>
   ossimRefPtr<ossimImageData> combine(T dummyVariableNotUsed,
                                       const ossimIrect& tileRect,
                                       ossim_uint32 resLevel);
   
   virtual double computeWeight(long index,
                                const ossimDpt& point)const;
TYPE_DATA
};

#endif /* #ifndef ossimFeatherMosaic_HEADER */
