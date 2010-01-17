//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts (gpotts@imagelinks.com)
//
//*************************************************************************
// $Id: ossimAffineTransform.h 15766 2009-10-20 12:37:09Z gpotts $

#ifndef ossimAffineTransform_HEADER
#define ossimAffineTransform_HEADER

#include <ossim/base/ossim2dTo2dTransform.h>
#include <ossim/matrix/newmat.h>

class OSSIMDLLEXPORT ossimAffineTransform : public ossim2dTo2dTransform
{
public:
   ossimAffineTransform();
   ossimAffineTransform(const ossimAffineTransform& src);

   /**
    * @brief assignment operator=
    * @param rhs The data to assign from.
    * @param A reference to this object.
    */
   virtual const ossimAffineTransform& operator=(
      const ossimAffineTransform& rhs);
   
   virtual ossimObject* dup()const
   {
      return new ossimAffineTransform(*this);
   }
   virtual void forward(const ossimDpt& input,
                        ossimDpt& output)const;
   virtual void inverse(const ossimDpt& input,
                        ossimDpt&       output)const;

   /*!
    * Used by the inverse iterator.  This will give it a good initial value
    * that is within the input transforms
    */
   virtual ossimDpt getOrigin()const
      {
         return ossimDpt(0,0);
      }
   
   bool saveState(ossimKeywordlist& kwl,
                   const char* prefix = 0)const;
   
   bool loadState(const ossimKeywordlist& kwl,
                  const char* prefix = 0);
   
   void setMatrix(double rotation,
                  const ossimDpt& scale,
                  const ossimDpt& translation);
   
   void setScale(const ossimDpt& scale)
      {
         theScale       = scale;
         computeMatrix();
      }
   
   void setRotation(double rotation)
      {
         theRotation = rotation;
         computeMatrix();
      }

   void setTranslation(const ossimDpt& translation)
      {
         theTranslation = translation;
         computeMatrix();
      }
   ossimDpt getScale()const
      {
         return theScale;
      }
   ossimDpt getTranslation()const
      {
         return theTranslation;
      }
   double getRotation()const
      {
         return theRotation;
      }
   void setIdentity();

   void setAffineEnabledFlag(bool flag)
      {
         theAffineEnabledFlag = flag;
      }
   virtual std::ostream& print(std::ostream& out) const;
      
protected:
   void computeMatrix();

   ossimDpt       theScale;
   double         theRotation;
   ossimDpt       theTranslation;
   bool           theAffineEnabledFlag;

   NEWMAT::Matrix theForwardXform;
   NEWMAT::Matrix theInverseXform;
TYPE_DATA      
};

#endif
