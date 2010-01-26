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
// $Id: ossim2dTo2dTransform.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossim2dTo2dTransform_HEADER
#define ossim2dTo2dTransform_HEADER
#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimDpt.h>

class OSSIMDLLEXPORT ossim2dTo2dTransform : public ossimObject
{
public:
   virtual void forward(const ossimDpt& input,
                        ossimDpt& output) const=0;
   
   virtual void forward(ossimDpt&  modify_this) const;
      
   virtual void inverse(const ossimDpt& input,
                        ossimDpt&       output) const;

   virtual void inverse(ossimDpt&  modify_this) const;
      
   virtual const ossim2dTo2dTransform& operator=(
      const ossim2dTo2dTransform& rhs);
   
   /*!
    * Used by the inverse iterator.  This will give it a good initial value
    * that is within the input transforms
    */
   virtual ossimDpt getOrigin() const;

   virtual std::ostream& print(std::ostream& out) const;
   
   virtual bool saveState(ossimKeywordlist& kwl,
                   const char* prefix = 0)const;
   
   virtual bool loadState(const ossimKeywordlist& kwl,
                  const char* prefix = 0);
   
   void setConvergenceThreshold(const double& new_threshold);
   
   void setMaxIterations(int new_max_iters);

   void setDxDy(const ossimDpt& dxdy);

protected:
   ossim2dTo2dTransform();
   virtual ~ossim2dTo2dTransform(){}
   double theConvergenceThreshold;
   int    theMaxIterations;

   ossimDpt theDxDy;
   
TYPE_DATA      
};

#endif
