//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
//
//*******************************************************************
//  $Id: ossimImageViewAffineTransform.h,v 1.9 2005/08/30 11:50:05 gpotts Exp $
#ifndef ossimImageViewAffineTransform_HEADER
#define ossimImageViewAffineTransform_HEADER
#include "ossimImageViewTransform.h"
#include "matrix/newmat.h"

class OSSIMDLLEXPORT ossimImageViewAffineTransform: public ossimImageViewTransform
{
public:
   ossimImageViewAffineTransform(double rotateDegrees = 0,
                                 double scaleXValue = 1,
                                 double scaleYValue = 1,
                                 double translateXValue = 0,
                                 double translateYValue = 0,
                                 double translateOriginXValue = 0,
                                 double translateOriginYValue = 0);
   virtual ~ossimImageViewAffineTransform();
   
   virtual void imageToView(const ossimDpt& imagePoint,
                            ossimDpt&       viewPoint)const;
   virtual void viewToImage(const ossimDpt& viewPoint,
                            ossimDpt&       imagePoint)const;
   void setMatrix(NEWMAT::Matrix& matrix);
   const NEWMAT::Matrix& getMatrix()const;
   
   virtual bool isIdentity()const
   {
      return ((theTransform[0][0] == 1.0)&&
              (theTransform[0][1] == 0.0)&&
              (theTransform[0][2] == 0.0)&&
              (theTransform[1][0] == 0.0)&&
              (theTransform[1][1] == 1.0)&&
              (theTransform[1][2] == 0.0)&&
              (theTransform[2][0] == 0.0)&&
              (theTransform[2][1] == 0.0)&&
              (theTransform[2][2] == 1.0));
   }

   virtual bool isValid()const;
   virtual bool setView(ossimObject* obj, bool ownsProjection=false);
   virtual ossimObject* getView();
   virtual const ossimObject* getView()const;
   
   /** @return (1, 1) ???????(drb) */
   virtual ossimDpt getInputMetersPerPixel()const;

   /** @return (nan, nan) ????????? (drb) */
   virtual ossimDpt getOutputMetersPerPixel()const;
   
   /*!
    * Translate in the x and y direction.
    */ 
   virtual void translate(double deltaX, double deltaY);
   
   /*!
    * Translate in the x direction.
    */
   virtual void translateX(double deltaX);
   
   /*!
    * Translate in the Y direction.
    */
   virtual void translateY(double deltaY);
   
   /*!
    * Translate the origin for rotation in the x and y direction.
    */
   virtual void translateOrigin(double originX, double originY);
   
   /*!
    * Translate the origin for rotation in the x direction.
    */
   virtual void translateOriginX(double originX);
   
   /*!
    * Translate the origin for rotation in the y direction.
    */
   virtual void translateOriginY(double originY);
   
   /*!
    * will allow you to specify a scale
    * for both the x and y direction.
    */
   virtual void scale(double x, double y);
   
   /*!
    * will alow you to specify a scale
    * along the X direction.
    */
   virtual void scaleX(double x);
   
   /*!
    * Will allow you to scale along the Y
    * direction.
    */
   virtual void scaleY(double y);
   
   /*!
    * Will apply a rotation
    */
   virtual void rotate(double degrees);
   
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix =0);
   
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix =0)const;
   
protected:
   void buildCompositeTransform();
   
   /*!
    * This is the transformation from image to
    * viewing coordinates.  If this matrix is
    * changed it will perform an inverse to solve
    * the inverse transform.
    */
   NEWMAT::Matrix theTransform;
   NEWMAT::Matrix theInverseTransform;
   ossim_float64 theRotation;
   ossimDpt      theScale;
   ossimDpt      theTranslate;
   ossimDpt      theTranslateOrigin;
//   ossimDpt theTranslateRotationOriginPoint;

TYPE_DATA
};

#endif
