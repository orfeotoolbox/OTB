//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Description:  Contains class definition for ossim2dTo2dIdentityTransform.
// 
//*******************************************************************
//  $Id$
#ifndef ossim2dTo2dIdentityTransform_HEADER
#define ossim2dTo2dIdentityTransform_HEADER
#include "ossim2dTo2dTransform.h"
/**
 * This is the identity transform and just passes the input to the output.
 */
class OSSIM_DLL ossim2dTo2dIdentityTransform : public ossim2dTo2dTransform
{
public:
   /**
    * forward transform just passes the point to the output.
    */
   virtual void forward(const ossimDpt& input,
                        ossimDpt& output) const
   {
      output = input;
   }
   
   /**
    * forward transform nothing is modified on the input point.
    */
   virtual void forward(ossimDpt&  /* modify_this */) const
   {
      // do nothing this is identity
   }
   
   /**
    * inverse transform just passes the point to the output.
    */
   virtual void inverse(const ossimDpt& input,
                        ossimDpt&       output) const
   {
      output = input;
   }
   
   /**
    * inverse transform nothing is modified on the input point.
    */
   virtual void inverse(ossimDpt&  /* modify_this */) const
   {
      // do nothing this is identity
   }
   
   /**
    * Pass equality to the parent
    */
   virtual const ossim2dTo2dIdentityTransform& operator=(
                                                 const ossim2dTo2dIdentityTransform& rhs)
   {
      ossim2dTo2dTransform::operator =(rhs);
      
      return *this;
   }
   
protected:
   TYPE_DATA
};
#endif
