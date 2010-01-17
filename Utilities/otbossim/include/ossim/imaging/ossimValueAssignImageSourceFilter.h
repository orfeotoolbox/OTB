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
// $Id: ossimValueAssignImageSourceFilter.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimValueAssignImageSourceFilter_HEADER
#define ossimValueAssignImageSourceFilter_HEADER

#include <ossim/imaging/ossimImageSourceFilter.h>

/**
 * This allows one to assign a value to a group of bands.  It will test
 * for equality band separate or as group. For example:
 *
 * if we are testing as a group then the assign will not happen unless
 * all values equal theInputValue array
 *
 *  inputValue = <1, 2, 3>
 *  outputValue = <100, 100, 100>
 *
 *  then it will only convert to 100, 100, 100 if the input equals all
 *  values of the output.
 *
 * input <1, 55, 3>  would become <1, 55, 3>
 * input <1, 2, 3>   would become <100, 100, 100>
 *
 *  Now for band separate it means you want to change on a per band and
 *  not as a group.  For example:
 *
 * inputValue = <1, 2, 3>
 * outputValue = <100, 100, 100>
 *
 * input <1, 55, 3>  would become <100, 55, 100>
 *
 */
class OSSIMDLLEXPORT ossimValueAssignImageSourceFilter : public ossimImageSourceFilter
{
public:
   enum ossimValueAssignType
   {
      ossimValueAssignType_SEPARATE = 1,
      ossimValueAssignType_GROUP    = 2
   };
   
   ossimValueAssignImageSourceFilter(ossimObject* owner=NULL);
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& tileRect,
                                   ossim_uint32 resLevel=0);

   void setInputOutputValues(const vector<double>& inputValues,
                             const vector<double>& outputValues);

   const vector<double>& getInputValues()const;
   const vector<double>& getOutputValues()const;
   ossimValueAssignType getValueAssignType()const;
   void setValueAssignType(ossimValueAssignType type);

   virtual void initialize();

   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=NULL)const;
   
   /**
    * Method to the load (recreate) the state of an object from a keyword
    * list.  Return true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=NULL);
   
protected:
   virtual ~ossimValueAssignImageSourceFilter();

   /**
    * Called on first getTile, will initialize all data needed.
    */
   void allocate();
   
   vector<double>              theInputValueArray;
   vector<double>              theOutputValueArray;
   ossimValueAssignType        theAssignType;
   ossimRefPtr<ossimImageData> theTile;
   
   void validateArrays();
   
   template <class T>
      void executeAssign(T, ossimRefPtr<ossimImageData>& data);
   
   template <class T>
      void executeAssignSeparate(T, ossimRefPtr<ossimImageData>& data);
   
   template <class T>
      void executeAssignGroup(T, ossimRefPtr<ossimImageData>& data);

TYPE_DATA
};

#endif /* #ifndef ossimValueAssignImageSourceFilter_HEADER */
