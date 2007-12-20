//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  Garrett Potts
//
//*******************************************************************
//  $Id: ossimImageRenderer.cpp 11955 2007-10-31 16:10:22Z gpotts $

#include <iostream>
using namespace std;

#include <ossim/imaging/ossimImageRenderer.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimDpt3d.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimProcessProgressEvent.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimConnectableContainerInterface.h>
#include <ossim/base/ossimViewController.h>
#include <ossim/base/ossimStringProperty.h>
#include <ossim/base/ossimNumericProperty.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimDiscrete3x3HatFilter.h>
#include <ossim/imaging/ossimDiscreteNearestNeighbor.h>
#include <ossim/imaging/ossimFilterResampler.h>
#include <ossim/imaging/ossimResampler.h>
#include <ossim/projection/ossimImageViewTransform.h>
#include <ossim/projection/ossimImageViewProjectionTransform.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/projection/ossimImageViewTransformFactory.h>
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/projection/ossimEquDistCylProjection.h>

#ifdef OSSIM_ID_ENABLED
static const char OSSIM_ID[] = "$Id: ossimImageRenderer.cpp 11955 2007-10-31 16:10:22Z gpotts $";
#endif

static ossimTrace traceDebug("ossimImageRenderer:debug");

RTTI_DEF2(ossimImageRenderer, "ossimImageRenderer", ossimImageSourceFilter, ossimViewInterface);


void ossimImageRenderer::ossimRendererSubRectInfo::splitView(ossimImageViewTransform* transform,
                                                             ossimRendererSubRectInfo& ulRect,
                                                             ossimRendererSubRectInfo& urRect,
                                                             ossimRendererSubRectInfo& lrRect,
                                                             ossimRendererSubRectInfo& llRect)const
{
#if 0
   ossimNotify(ossimNotifyLevel_DEBUG)
      << "ulRect = " << ulRect
      << "\nurRect = " << urRect
      << "\nlrRect = " << lrRect
      << "\nllRect = " << llRect << endl;
#endif
   
   ossimIrect vrect(theVul,
                    theVur,
                    theVlr,
                    theVll);
   
   ossimIrect tempUlRect;
   ossimIrect tempUrRect;
   ossimIrect tempLrRect;
   ossimIrect tempLlRect;
   
   if((vrect.width() == 1)&&
      (vrect.height() == 1))
   {
      tempUlRect = vrect;
      tempUrRect = vrect;
      tempLrRect = vrect;
      tempLlRect = vrect;
   }
   else
   {
      ossim_int32 w  = vrect.width();
      ossim_int32 h  = vrect.height();
      ossim_int32 w2 = w>>1;
      ossim_int32 h2 = h>>1;
      
      if((w%2) == 0) // if even
      {
         if((h%2) == 0) // if even
         {
            tempUlRect = ossimIrect(theVul.x,
                                    theVul.y,
                                    theVul.x + (w2 - 1),
                                    theVul.y + (h2 - 1));
            
            tempUrRect = ossimIrect(theVul.x + w2,
                                    theVul.y,
                                    theVul.x + w2 + (w2 - 1),
                                    theVul.y + (h2 - 1));
            
            tempLrRect = ossimIrect(theVul.x + w2,
                                    theVul.y + h2,
                                    theVul.x + w2 + (w2 - 1),
                                    theVul.y + h2 + (h2 - 1));
            
            tempLlRect = ossimIrect(theVul.x,
                                    theVul.y + h2,
                                    theVul.x + (w2 - 1),
                                    theVul.y + h2 + (h2 - 1));
         }
         else // odd
         {
            tempUlRect = ossimIrect(theVul.x,
                                    theVul.y,
                                    theVul.x + (w2 - 1),
                                    theVul.y + (h2));
            
            tempUrRect = ossimIrect(theVul.x + w2,
                                    theVul.y,
                                    theVul.x + w2 + (w2 - 1),
                                    theVul.y + (h2));
            
            tempLrRect = ossimIrect(theVul.x + w2,
                                    theVul.y + h2 + 1,
                                    theVul.x + w2 + (w2 - 1),
                                    theVul.y + h2 + (h2));
            
            tempLlRect = ossimIrect(theVul.x,
                                    theVul.y + h2 + 1,
                                    theVul.x + (w2 - 1),
                                    theVul.y + h2 + (h2));
         }
      }
      else // odd
      {
         if((h%2) == 0) // if even
         {
            tempUlRect = ossimIrect(theVul.x,
                                    theVul.y,
                                    theVul.x + (w2),
                                    theVul.y + (h2 - 1));
            
            tempUrRect = ossimIrect(theVul.x + w2 + 1,
                                    theVul.y,
                                    theVul.x + w2 + (w2),
                                    theVul.y + (h2 - 1));
            
            tempLrRect = ossimIrect(theVul.x + w2+1,
                                    theVul.y + h2,
                                    theVul.x + w2 + (w2),
                                    theVul.y + h2 + (h2 - 1));
            
            tempLlRect = ossimIrect(theVul.x,
                                    theVul.y + h2,
                                    theVul.x + (w2),
                                    theVul.y + h2 + (h2 - 1));
            
         }
         else // odd
         {
            tempUlRect = ossimIrect(theVul.x,
                                    theVul.y,
                                    theVul.x + (w2),
                                    theVul.y + (h2));
            
            tempUrRect = ossimIrect(theVul.x + w2+1,
                                    theVul.y,
                                    theVul.x + w2 + (w2),
                                    theVul.y + (h2));
            
            tempLrRect = ossimIrect(theVul.x + w2+1,
                                    theVul.y + h2+1,
                                    theVul.x + w2 + (w2),
                                    theVul.y + h2 + (h2));
            
            tempLlRect = ossimIrect(theVul.x,
                                    theVul.y + h2+1,
                                    theVul.x + (w2),
                                    theVul.y + h2 + (h2));
         }
      }
   }
#if 0
   ossimNotify(ossimNotifyLevel_DEBUG)
      << "SPLIT++++++++++++++++++++++++++++++++++++++++++++" << endl
      << "current = " << vrect << endl
      << "ul      = " << tempUlRect << endl
      << "ur      = " << tempUrRect << endl
      << "lr      = " << tempLrRect << endl
      << "ll      = " << tempLlRect << endl;
#endif

   ulRect.theVul = tempUlRect.ul();
   ulRect.theVur = tempUlRect.ur();
   ulRect.theVlr = tempUlRect.lr();
   ulRect.theVll = tempUlRect.ll();
   
   urRect.theVul = tempUrRect.ul();
   urRect.theVur = tempUrRect.ur();
   urRect.theVlr = tempUrRect.lr();
   urRect.theVll = tempUrRect.ll();
   
   lrRect.theVul = tempLrRect.ul();
   lrRect.theVur = tempLrRect.ur();
   lrRect.theVlr = tempLrRect.lr();
   lrRect.theVll = tempLrRect.ll();

   llRect.theVul = tempLlRect.ul();
   llRect.theVur = tempLlRect.ur();
   llRect.theVlr = tempLlRect.lr();
   llRect.theVll = tempLlRect.ll();
   
   ulRect.transformViewToImage(transform);
   urRect.transformViewToImage(transform);
   lrRect.transformViewToImage(transform);
   llRect.transformViewToImage(transform);
}

void ossimImageRenderer::ossimRendererSubRectInfo::transformImageToView(ossimImageViewTransform* transform)
{
   ossimDpt vul;
   ossimDpt vur;
   ossimDpt vlr;
   ossimDpt vll;
   transform->imageToView(theIul,
                          vul);
   transform->imageToView(theIur,
                          vur);
   transform->imageToView(theIlr,
                          vlr);
   transform->imageToView(theIll,
                          vll);
   
   theVul = vul;
   theVur = vur;
   theVlr = vlr;
   theVll = vll;
   
}

void ossimImageRenderer::ossimRendererSubRectInfo::transformViewToImage(ossimImageViewTransform* transform)
{
   
   
   ossimDpt nvul = theVul;
   ossimDpt nvur = theVur;;
   ossimDpt nvlr = theVlr;
   ossimDpt nvll = theVll;

   transform->viewToImage(nvul,
                          theIul);
   transform->viewToImage(nvur,
                          theIur);
   transform->viewToImage(nvlr,
                          theIlr);
   transform->viewToImage(nvll,
                          theIll);

   if(imageHasNans())
   {
      theViewToImageScale.makeNan();
   }
   else
   {
      theViewToImageScale = ossimDpt(1.0, 1.0);
      
      ossimDpt topDelta    = theIur - theIul;
      ossimDpt rightDelta  = theIlr - theIur;
      ossimDpt bottomDelta = theIll - theIlr;
      ossimDpt leftDelta   = theIul - theIll;
      double topLen    = topDelta.length();
      double bottomLen = bottomDelta.length();
      double rightLen  = rightDelta.length();
      double leftLen   = leftDelta.length();
      
//       double averageHoriz = ((topLen + 1)  + (bottomLen+1))*.5;
//       double averageVert  = ((leftLen+1) + (rightLen+1))*.5;
      double averageHoriz = ((topLen)  + (bottomLen))*.5;
      double averageVert  = ((leftLen) + (rightLen))*.5;
      
      
      ossimDpt deltaViewP1P2 = theVul - theVur;
      ossimDpt deltaViewP1P3 = theVul - theVll;
      
      double lengthViewP1P2 = deltaViewP1P2.length();//+1;
      double lengthViewP1P3 = deltaViewP1P3.length();//+1;

      if(lengthViewP1P2 > FLT_EPSILON)
      {
         theViewToImageScale.x = averageHoriz/lengthViewP1P2;
      }
      else
      {
         theViewToImageScale.makeNan();
      }
      if(lengthViewP1P3 > FLT_EPSILON)
      {
         theViewToImageScale.y = averageVert/lengthViewP1P3;
          
      }
      else
      {
         theViewToImageScale.makeNan();
      }

      if(!theViewToImageScale.hasNans())
      {
         theImageToViewScale.x = 1.0/theViewToImageScale.x;
         theImageToViewScale.y = 1.0/theViewToImageScale.y;
      }
      else
      {
         theImageToViewScale.makeNan();
      }
  }
}

void ossimImageRenderer::ossimRendererSubRectInfo::stretchImageOut(bool enableRound)
{
   ossimDpt topDelta    = theIur - theIul;
   ossimDpt rightDelta  = theIlr - theIur;
   ossimDpt bottomDelta = theIll - theIlr;
   ossimDpt leftDelta   = theIul - theIll;

   topDelta = topDelta*(1.0/topDelta.length());
   rightDelta = rightDelta*(1.0/rightDelta.length());
   bottomDelta = bottomDelta*(1.0/bottomDelta.length());
   leftDelta = leftDelta*(1.0/leftDelta.length());

   theIul = theIul + ((leftDelta - topDelta)*.5);
   theIur = theIur + ((topDelta - rightDelta)*.5);
   theIlr = theIlr + ((rightDelta - bottomDelta)*.5);
   theIll = theIll + ((bottomDelta - leftDelta)*.5);

   if(enableRound)
   {
      theIul = ossimIpt(ossim::round<int>(theIul.x),
                        ossim::round<int>(theIul.y));
      theIur = ossimIpt(ossim::round<int>(theIur.x),
                        ossim::round<int>(theIur.y));
      theIlr = ossimIpt(ossim::round<int>(theIlr.x),
                        ossim::round<int>(theIlr.y));
      theIll = ossimIpt(ossim::round<int>(theIll.x),
                        ossim::round<int>(theIll.y));
   }
}


bool ossimImageRenderer::ossimRendererSubRectInfo::isIdentity()const
{
//    ossimDpt deltaP1P2 = theIul - theIur;
//    ossimDpt deltaP1P3 = theIul - theIll;
   
//    ossimDpt deltaViewP1P2 = theVul - theVur;
//    ossimDpt deltaViewP1P3 = theVul - theVll;
   
//    bool horizontalSigns = ossimGetSign(deltaP1P2.x)==ossimGetSign(deltaViewP1P2.x);
//    bool verticalSigns   = ossimGetSign(deltaP1P3.y)==ossimGetSign(deltaViewP1P3.y);

//    // check first to see if any horizontal or vertical flipping
//    //
//    if(horizontalSigns && verticalSigns)
//    {
//       // check scales to see if they are 1
//       if(fabs(1-theViewToImageScale.x) <= FLT_EPSILON &&
//          fabs(1-theViewToImageScale.y) <= FLT_EPSILON)
//       {
//          return true;
//       }
//    }

    double iulDelta = (theIul-theVul).length();
    double iurDelta = (theIur-theVur).length();
    double ilrDelta = (theIlr-theVlr).length();
    double illDelta = (theIll-theVll).length();

    return ((iulDelta <= FLT_EPSILON)&&
            (iurDelta <= FLT_EPSILON)&&
            (ilrDelta <= FLT_EPSILON)&&
            (illDelta <= FLT_EPSILON));
}

bool ossimImageRenderer::ossimRendererSubRectInfo::canBilinearInterpolate(ossimImageViewTransform* transform,
									  double error)const
{
   if(imageHasNans())
   {
      return false;
   }
  ossimDpt imageToViewScale = getAbsValueImageToViewScales();

  double testScale = imageToViewScale.length();

  // if there is a large shrink or expansion then just return true.
  // You are probably not worried about error in bilinear interpolation
  //
  if((testScale > 500)||
     (testScale < 1.0/500.0))
  {
     return true;
  }
  if(imageToViewScale.hasNans()) return false;
  ossimDpt vUpper, vRight, vBottom, vLeft, vCenter;
  ossimDpt iUpper, iRight, iBottom, iLeft, iCenter;

  getViewMids(vUpper, vRight, vBottom, vLeft, vCenter);
  getImageMids(iUpper, iRight, iBottom, iLeft, iCenter);

  ossimDpt testCenter;

  ossimDpt iFullRes(iCenter.x*imageToViewScale.x,
		    iCenter.y*imageToViewScale.y);

  transform->viewToImage(vCenter, testCenter);

  if(testCenter.hasNans())
  {
     return false;
  }
  ossimDpt testFullRes(testCenter.x*imageToViewScale.x,
		       testCenter.y*imageToViewScale.y);

  double errorCheck1 = (testFullRes - iFullRes).length();

  iFullRes = ossimDpt(iUpper.x*imageToViewScale.x,
		      iUpper.y*imageToViewScale.y);

  transform->viewToImage(vUpper, testCenter);
  if(testCenter.hasNans())
  {
     return false;
  }
  testFullRes = ossimDpt(testCenter.x*imageToViewScale.x,
			 testCenter.y*imageToViewScale.y);
  double errorCheck2 = (testFullRes - iFullRes).length();

  iFullRes = ossimDpt(iRight.x*imageToViewScale.x,
		      iRight.y*imageToViewScale.y);

  transform->viewToImage(vRight, testCenter);
  if(testCenter.hasNans())
  {
     return false;
  }
  testFullRes = ossimDpt(testCenter.x*imageToViewScale.x,
			 testCenter.y*imageToViewScale.y);
  double errorCheck3 = (testFullRes - iFullRes).length();

  iFullRes = ossimDpt(iBottom.x*imageToViewScale.x,
		      iBottom.y*imageToViewScale.y);

  transform->viewToImage(vBottom, testCenter);
  if(testCenter.hasNans())
  {
     return false;
  }
  testFullRes = ossimDpt(testCenter.x*imageToViewScale.x,
			 testCenter.y*imageToViewScale.y);
  double errorCheck4 = (testFullRes - iFullRes).length();

  iFullRes = ossimDpt(iLeft.x*imageToViewScale.x,
		      iLeft.y*imageToViewScale.y);

  transform->viewToImage(vLeft, testCenter);
  testFullRes = ossimDpt(testCenter.x*imageToViewScale.x,
			 testCenter.y*imageToViewScale.y);
  double errorCheck5 = (testFullRes - iFullRes).length();


  return ((errorCheck1 < error)&&
	  (errorCheck2 < error)&&
	  (errorCheck3 < error)&&
	  (errorCheck4 < error)&&
	  (errorCheck5 < error));
	  
}


void ossimImageRenderer::ossimRendererSubRectInfo::getViewMids(ossimDpt& upperMid,
				     ossimDpt& rightMid,
				     ossimDpt& bottomMid,
				     ossimDpt& leftMid,
				     ossimDpt& center)const
{
  
  upperMid  = (theVul + theVur)*.5;
  rightMid  = (theVur + theVlr)*.5;
  bottomMid = (theVlr + theVll)*.5;
  leftMid   = (theVul + theVll)*.5;
  center    = (theVul + theVur + theVlr + theVll)*.25;
}

void ossimImageRenderer::ossimRendererSubRectInfo::getImageMids(ossimDpt& upperMid,
				      ossimDpt& rightMid,
				      ossimDpt& bottomMid,
				      ossimDpt& leftMid,
				      ossimDpt& center)const
{
  if(imageHasNans())
    {
      upperMid.makeNan();
      rightMid.makeNan();
      bottomMid.makeNan();
      leftMid.makeNan();
      center.makeNan();
    }
  else
    {
      upperMid  = (theIul + theIur)*.5;
      rightMid  = (theIur + theIlr)*.5;
      bottomMid = (theIlr + theIll)*.5;
      leftMid   = (theIul + theIll)*.5;
      center    = (theIul + theIur + theIlr + theIll)*.25;
    }
}

ossimDpt ossimImageRenderer::ossimRendererSubRectInfo::getParametricCenter(const ossimDpt& ul, const ossimDpt& ur, 
									   const ossimDpt& lr, const ossimDpt& ll)const
{
  ossimDpt top    = ur - ul;
  ossimDpt bottom = lr - ll;
  
  ossimDpt centerTop = ul + top * .5;
  ossimDpt centerBottom = ll + bottom * .5;

  return centerBottom + (centerBottom - centerTop)*.5;
}

ossimImageRenderer::ossimImageRenderer()
   :
      ossimImageSourceFilter(),
      ossimViewInterface(NULL),
      theResampler(NULL),
      theBlankTile(NULL),
      theTile(NULL),
      theTemporaryBuffer(NULL),
      theCachedTile(NULL),
      theStartingResLevel(0),
      theImageViewTransform(NULL),
      theMaxRecursionLevel(5),
      theAutoUpdateInputTransform(true),
      theMaxLevelsToCompute(999999) // something large so it will always compute
{
   ossimViewInterface::theObject = this;
   theResampler = new ossimFilterResampler();
   theImageViewTransform = new ossimImageViewProjectionTransform;
   theBoundingRect.makeNan();
//    theInputRect.makeNan();
//    theOutputRect.makeNan();
}

ossimImageRenderer::ossimImageRenderer(ossimImageSource* inputSource,
                                       ossimImageViewTransform* imageViewTrans)
   : ossimImageSourceFilter(inputSource),
     ossimViewInterface(NULL),
     theResampler(NULL),
     theBlankTile(NULL),
     theTile(NULL),
     theTemporaryBuffer(NULL),
     theCachedTile(NULL),
     theStartingResLevel(0),
     theImageViewTransform(imageViewTrans),
     theMaxRecursionLevel(5),
     theAutoUpdateInputTransform(true),
     theMaxLevelsToCompute(999999) // something large so it will always compute
{
   ossimViewInterface::theObject = this;
   theResampler = new ossimFilterResampler();
   if(!theImageViewTransform)
   {
      theImageViewTransform = new ossimImageViewProjectionTransform;
   }
}

ossimImageRenderer::~ossimImageRenderer()
{
   if (theImageViewTransform)
   {
      delete theImageViewTransform;
      theImageViewTransform = NULL;
   }
   
   if(theResampler)
   {
      delete theResampler;
      theResampler = NULL;
   }
}

ossimRefPtr<ossimImageData> ossimImageRenderer::getTile(
   const  ossimIrect& tileRect,
   ossim_uint32 resLevel)
{
   static const char MODULE[] = "ossimImageRenderer::getTile";
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " Requesting view rect = "
         << tileRect << endl;
   }

   // long w = tileRect.width();
   // long h = tileRect.height();
   // ossimIpt origin = tileRect.ul();
   
   if(!theBlankTile.valid() || !theTile.valid())
   {
      allocate();
   }

   if(!theTile.valid())
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimImageRenderer::getTile tile allocation failure!\n"
            << endl;
      }
      return ossimImageSourceFilter::getTile(tileRect, resLevel);
   }

   if(theBlankTile.valid())
   {
      theBlankTile->setImageRectangle(tileRect);
   }

   if(!theInputConnection)
   {
      return theBlankTile;
   }
   
   if ( !isSourceEnabled()||(!theImageViewTransform)||
        (!theImageViewTransform->isValid()) )
   {
      // This tile source bypassed, return the input tile source.
      return theInputConnection->getTile(tileRect, resLevel);  
   }

   if(theBoundingViewRect.hasNans())
   {
      theBoundingViewRect = getBoundingRect();
   }
   if(theBoundingViewRect.width() < 4 &&
      theBoundingViewRect.height() < 4)
   {
      return theBlankTile;
   }
   ossimIrect rect = theBoundingViewRect;
   if( !theInputConnection || !rect.intersects(tileRect) )
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << MODULE << "No intersection, Returning...." << endl;
      }
      return theBlankTile;
   }
   
   if(!theTile)
   {
      return theInputConnection->getTile(tileRect, resLevel);
   }
   
   // long tw = theTile->getWidth();
   // long th = theTile->getHeight();
   
   theTile->setImageRectangle(tileRect);
   theTile->makeBlank();

   ossimRendererSubRectInfo subRectInfo(tileRect.ul(),
                                        tileRect.ur(),
                                        tileRect.lr(),
                                        tileRect.ll());
   
   subRectInfo.transformViewToImage(theImageViewTransform);
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " image rect = " << subRectInfo.getImageRect() << std::endl;
   }
   
   recursiveResample(theTile, subRectInfo, 1);
   
   if(theTile.valid())
   {
      theTile->validate();
   }
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << "Returning...." << endl;
   }
   return theTile;
}

void ossimImageRenderer::recursiveResample(ossimRefPtr<ossimImageData> outputData,
                                           const ossimRendererSubRectInfo& rectInfo,
                                           ossim_uint32 level)
{
   ossimIrect tempViewRect = rectInfo.getViewRect();
   if(rectInfo.imageIsNan())
   {
      return;
   } 
   
   if(tempViewRect.width() <2 &&
      tempViewRect.height() <2)
   {
      if(!rectInfo.imageHasNans())
      {
         fillTile(outputData,
                  rectInfo);
      }
      return;
   }
   const double error = 1;
   if(rectInfo.canBilinearInterpolate(theImageViewTransform, error))
   {                                // then draw the tile
      fillTile(outputData,
	       rectInfo);
      return;
   }
   else
   {
      // split into four subtiles
      ossimRendererSubRectInfo ulRectInfo;
      ossimRendererSubRectInfo urRectInfo;
      ossimRendererSubRectInfo lrRectInfo;
      ossimRendererSubRectInfo llRectInfo;
      
      rectInfo.splitView(theImageViewTransform,
			 ulRectInfo,
			 urRectInfo,
			 lrRectInfo,
			 llRectInfo);   
      
      ossimDrect vrect = rectInfo.getViewRect();
      
#if 0
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "vrect  = " << vrect
         << "\nwidth  = " << vrect.width()
         << "\nheight = " << vrect.height()
         << "\nlevel  = " << level << endl;
#endif
      bool scaleUlNeedsSplit = ((!ulRectInfo.canBilinearInterpolate(theImageViewTransform, error))||
				ulRectInfo.imageHasNans());
      bool scaleUrNeedsSplit = ((!urRectInfo.canBilinearInterpolate(theImageViewTransform, error))||
				urRectInfo.imageHasNans());
      bool scaleLrNeedsSplit = ((!lrRectInfo.canBilinearInterpolate(theImageViewTransform, error))||
				lrRectInfo.imageHasNans());
      bool scaleLlNeedsSplit = ((!llRectInfo.canBilinearInterpolate(theImageViewTransform, error))||
				llRectInfo.imageHasNans());
      
      bool tooSmall = (vrect.width() < 4) && (vrect.height()<4);
      //
      if(!tooSmall)
      {
         if(scaleUlNeedsSplit||
            scaleUrNeedsSplit||
            scaleLrNeedsSplit||
            scaleLlNeedsSplit)
         {
            if(scaleUlNeedsSplit)
            {
               recursiveResample(outputData,
                                 ulRectInfo,
                                 level + 1);
            }
            else
            {
               fillTile(outputData, ulRectInfo);
            }
            if(scaleUrNeedsSplit)
            {
               recursiveResample(outputData,
                                 urRectInfo,
                                 level + 1);
            }
            else
            {
               fillTile(outputData, urRectInfo);
            }
            
            if(scaleLrNeedsSplit)
            {
               recursiveResample(outputData,
                                 lrRectInfo,
                                 level + 1);
            }
            else
            {
               fillTile(outputData, lrRectInfo);
            }
            
            if(scaleLlNeedsSplit)
            {
               recursiveResample(outputData,
                                 llRectInfo,
                                 level + 1);
            }
            else
            {
               fillTile(outputData, llRectInfo);
            }
         }
         else
         {
            fillTile(outputData,
                     rectInfo);
         }
      }
      else if(!rectInfo.imageHasNans())
      {
         fillTile(outputData,
                  rectInfo);
      }
   }
}

void ossimImageRenderer::fillTile(ossimRefPtr<ossimImageData> outputData,
                                  const ossimRendererSubRectInfo& rectInfo)
{
   if(!outputData.valid() || !outputData->getBuf() || rectInfo.imageHasNans())
   {
      return;
   }
   ossimIrect vrect = rectInfo.getViewRect();

   ossimDpt imageToViewScale = rectInfo.getAbsValueImageToViewScales();

   if(imageToViewScale.hasNans()) return;
   
   ossimDpt tile_size = ossimDpt(vrect.width(), vrect.height());
   double kernelSupportX, kernelSupportY;
   double averageScale = (imageToViewScale.x + imageToViewScale.y)/2.0;
   long   closestFitResLevel = (long)floor(log(1.0/averageScale)/
                                           log(2.0));
   
   ossim_uint32 resLevel = closestFitResLevel<0?0:closestFitResLevel;
   resLevel += theStartingResLevel;
   double closestScale = 1.0/pow((double)2.0,(double)resLevel);
   
   ossimDrect requestRect = rectInfo.getImageRect()*ossimDpt(closestScale,closestScale);
   
   ossimDpt nul(rectInfo.theIul.x*closestScale,
                rectInfo.theIul.y*closestScale);
   ossimDpt nll(rectInfo.theIll.x*closestScale,
                rectInfo.theIll.y*closestScale);
   ossimDpt nlr(rectInfo.theIlr.x*closestScale,
                rectInfo.theIlr.y*closestScale);
   ossimDpt nur(rectInfo.theIur.x*closestScale,
                rectInfo.theIur.y*closestScale);
   
   ossimDpt newScale(imageToViewScale.x/closestScale,
                     imageToViewScale.y/closestScale);
   
   theResampler->setScaleFactor(newScale);

   theResampler->getKernelSupport(kernelSupportX,
                                  kernelSupportY);
   
   ossimDrect boundingRect = ossimDrect(nul,
 					nll,
 					nlr,
 					nur);
   
   boundingRect = ossimIrect((ossim_int32)floor(boundingRect.ul().x - (kernelSupportX)-.5),
                             (ossim_int32)floor(boundingRect.ul().y - (kernelSupportY)-.5),
                             (ossim_int32)ceil(boundingRect.lr().x + (kernelSupportX)+.5),
                             (ossim_int32)ceil(boundingRect.lr().y + (kernelSupportY)+.5));
//    boundingRect = ossimIrect((ossim_int32)floor(boundingRect.ul().x - (kernelSupportX)),
//                              (ossim_int32)floor(boundingRect.ul().y - (kernelSupportY)),
//                              (ossim_int32)ceil(boundingRect.lr().x + (kernelSupportX)),
//                              (ossim_int32)ceil(boundingRect.lr().y + (kernelSupportY)));
   
   requestRect = boundingRect;

   // ossimIrect reqRect = requestRect;
   
   ossimRefPtr<ossimImageData> data;
   if(theCachedTile.valid())
   {
      ossimIrect cachedTileRect = theCachedTile->getImageRectangle();
#if 0
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "cached tile rect = " << cachedTileRect << endl
         << "requestRect      = " << requestRect << endl;
#endif
#if 0      
      if(requestRect.completely_within(cachedTileRect)&&
         resLevel == theCachedTileResLevel)
      {
         // ossimNotify(ossimNotifyLevel_DEBUG)
         // << "SETTING TO CACHED RECT" << endl;
         data = theCachedTile;
      }
#endif
   }
   
   if(!data.valid())
   {
#if 0
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "--------------------Wiping out old cached rect" << endl;
#endif
//       if( (requestRect.width() < 64 ) || (requestRect.height() < 64) )
//       {
// 	 requestRect.stretchToTileBoundary(ossimIpt(64, 64));
//       }
      data = getTileAtResLevel(requestRect, resLevel);
      if(theCachedTile.valid())
      {
         theCachedTile = NULL;
      }
//       if(data)
//       {
//          theCachedTileResLevel = resLevel;
//          theCachedTile = (ossimImageData*)data->dup();
//       }
   }
   
   ossimDataObjectStatus status = OSSIM_NULL;
   if(data.valid())
   {
      status = data->getDataObjectStatus();
   }
   if( (status == OSSIM_NULL) ||
       (status == OSSIM_EMPTY))
   {
      return;
   }
   
   ossimDrect bounds = theBoundingRect;
   // setup the clamp for the resampler
   //
//    if(static_cast<ossim_uint32>(resLevel) < theInputDecimationFactors.size())
//    {
//      bounds = bounds*theInputDecimationFactors[resLevel];
//    }
//    else if(theInputDecimationFactors.size() > 0)
//    {
                           
//       bounds = bounds*theInputDecimationFactors[theInputDecimationFactors.size()-1];
//    }

//   theResampler->setBoundingInputRect(bounds);

   bounds = bounds*ossimDpt(closestScale, closestScale);
   theResampler->setBoundingInputRect(bounds);
   
   double denominatorX = 1.0;
   double denominatorY = 1.0;
   
   if(tile_size.x > 2)
   {
      denominatorX = tile_size.x-1.0;
   }
   if(tile_size.y > 2)
   {
      denominatorY = tile_size.y-1.0;
   }
   theResampler->setScaleFactor(newScale);
   theResampler->resample(data,
			  outputData,
			  rectInfo.getViewRect(),
			  nul,
			  nur,
			  ossimDpt( ( (nll.x - nul.x)/denominatorY ),
				    ( (nll.y - nul.y)/denominatorY ) ),
			  ossimDpt( ( (nlr.x - nur.x)/denominatorY ),
				    ( (nlr.y - nur.y)/denominatorY ) ),
// 			  ossimDpt( ( (nll.x - nul.x)/denominatorX ),
// 				    ( (nll.y - nul.y)/denominatorY ) ),
// 			  ossimDpt( ( (nlr.x - nur.x)/denominatorX ),
// 				    ( (nlr.y - nur.y)/denominatorY ) ),
			  tile_size);
//   outputData->validate();
}

long ossimImageRenderer::computeClosestResLevel(const std::vector<ossimDpt>& decimationFactors,
                                                double scale)const
{
   long result = 0;
   long upper  = (long)decimationFactors.size();
   bool done   = false;
   
   if(upper > 1)
   {
      while((result < upper)&&!done)
      {
         if(scale < decimationFactors[result].x)
         {
            ++result;
         }
         else
         {
            done = true;
         }
      }
      
      // now use the higher res level and resample down
      //
      if(result)
      {
         --result;
      }
   }
   return result;
}

ossimIrect ossimImageRenderer::getBoundingRect(ossim_uint32 resLevel)const
{
#if 0
   static const char* MODULE = "ossimImageRenderer::getBoundingRect()";
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << "entered..." << endl;
   }
#endif
   
   if(!theBoundingViewRect.hasNans())
   {
      return theBoundingViewRect;
   }
   
   vector<ossimDpt> points(4);
   
//   if(!theOutputRect.hasNans()) return theOutputRect;
   ossimDrect inputRect;
   ossimIrect outputRect;
   
   inputRect.makeNan();
   outputRect.makeNan();

   if(theInputConnection&&theImageViewTransform)
   {
      inputRect = theInputConnection->getBoundingRect(resLevel);
      
      if(isSourceEnabled()&&theImageViewTransform->isValid())
      {
//          inputRect = ossimDrect(inputRect.ul().x,
//                                 inputRect.ul().y,
//                                 inputRect.lr().x,
//                                 inputRect.lr().y); 
         
// 	 theImageViewTransform->imageToView(inputRect.ul(), points[0]);
//          theImageViewTransform->imageToView(inputRect.ur(), points[1]);
//          theImageViewTransform->imageToView(inputRect.lr(), points[2]);
//          theImageViewTransform->imageToView(inputRect.ll(), points[3]);

         
         ossimDrect tempRect = theImageViewTransform->getImageToViewBounds(inputRect);
         
         theBoundingViewRect = ossimIrect(static_cast<int>(floor(tempRect.ul().x + 0.5)),
                                          static_cast<int>(floor(tempRect.ul().y + 0.5)),
                                          static_cast<int>(floor(tempRect.lr().x + 0.5)),
                                          static_cast<int>(floor(tempRect.lr().y + 0.5)));
      }
      else
      {
         theBoundingViewRect = inputRect;
      }
   }
   
#if 0
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " bounds  = "
         << outputRect
         << "\n w, h    = " << outputRect.width() << ", "
         << outputRect.height()
         << endl;
   }
#endif
   
   return theBoundingViewRect;
}

void ossimImageRenderer::initialize()
{
   //---
   // Call the base class initialize.
   // Note:  This will reset "theInputConnection" if it changed...
   //---
   ossimImageSourceFilter::initialize();

   theBoundingViewRect.makeNan();
   theBoundingRect.makeNan();

   if (theInputConnection)
   {
      theBoundingRect = theInputConnection->getBoundingRect();
   }

   if (theImageViewTransform)
   {
      ossimImageViewProjectionTransform* trans
         = PTR_CAST(ossimImageViewProjectionTransform,
                    theImageViewTransform);
      if(trans)
      {
         if(PTR_CAST(ossimMapProjection, trans->getImageProjection()) ||
            (!trans->getImageProjection())||
            (!trans->getViewProjection()))
         {
            checkTransform();
         }
      }
   }

   if (theResampler)
   {
      theResampler->setBoundingInputRect(theBoundingRect);
   }
   
   theInputDecimationFactors.clear();

   deallocate();
//    if (theInputConnection && theTile.valid())
//    {
//       if ( theInputConnection->getNumberOfOutputBands() !=
//            theTile->getNumberOfBands())
//       {
//          //---
//          // This will wide things slick and force an allocate() call on first
//          // getTile().
//          //---
//          deallocate();
//       }
//    }
}

void ossimImageRenderer::deallocate()
{
   theTile            = NULL;
   theBlankTile       = NULL;
   theTemporaryBuffer = NULL;
   theCachedTile      = NULL;
}

void ossimImageRenderer::allocate()
{
   deallocate();

   if(theInputConnection)
   {
      theTile = ossimImageDataFactory::instance()->create(this, this);
      theBlankTile  = ossimImageDataFactory::instance()->create(this, this);
      
      theTile->initialize();
      theBoundingRect = theInputConnection->getBoundingRect();
   }
}

bool ossimImageRenderer::saveState(ossimKeywordlist& kwl,
                                   const char* prefix)const
{
   if(theImageViewTransform)
   {
      ossimString newPrefix = ossimString(prefix) + ossimString("image_view_trans.");
      
      theImageViewTransform->saveState(kwl, newPrefix.c_str());
   }
   if(theResampler)
   {
      theResampler->saveState(kwl,
                              (ossimString(prefix)+"resampler.").c_str());
   }
   kwl.add(prefix,
           "max_levels_to_compute",
           theMaxLevelsToCompute);
   
   return ossimImageSource::saveState(kwl, prefix);
}

bool ossimImageRenderer::loadState(const ossimKeywordlist& kwl,
                                   const char* prefix)
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimImageRenderer::loadState entered..." << endl;
      
#ifdef OSSIM_ID_ENABLED
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "OSSIM_ID:  " << OSSIM_ID << endl;
#endif      
   }
   
   ossimString newPrefix = ossimString(prefix) + ossimString("image_view_trans.");
   bool result = ossimImageSourceFilter::loadState(kwl, prefix);
   theInputDecimationFactors.clear();
   
   if(theResampler)
   {
      theResampler->loadState(kwl,
                              (ossimString(prefix)+"resampler.").c_str());
   }
   if(theImageViewTransform)
   {
      delete theImageViewTransform;
      theImageViewTransform = NULL;
   }
   theImageViewTransform = ossimImageViewTransformFactory::instance()->createTransform(kwl, newPrefix.c_str());
   if(!theImageViewTransform)
   {
      theImageViewTransform = new ossimImageViewProjectionTransform;
   }
   const char* maxLevelsToCompute = kwl.find(prefix,
                                             "max_levels_to_compute");
   if(maxLevelsToCompute)
   {
      theMaxLevelsToCompute = ossimString(maxLevelsToCompute).toUInt32();
   }
   
   return result;
}

void ossimImageRenderer::setImageViewTransform(ossimImageViewTransform* transform)
{
   if(theImageViewTransform)
   {
      delete theImageViewTransform;
   }
   theImageViewTransform = transform;

   ossimImageViewProjectionTransform* trans = PTR_CAST(ossimImageViewProjectionTransform,
                                                       theImageViewTransform);
   if(trans)
   {
      if(!trans->getImageProjection())
      {
         checkTransform();
      }
   }
   theBoundingViewRect.makeNan();
}

bool ossimImageRenderer::setView(ossimObject* baseObject,
                                 bool ownsTheView)
{
   bool result = false;
   theBoundingViewRect.makeNan();
   if(theImageViewTransform)
   {
      result = theImageViewTransform->setView(baseObject,
                                              ownsTheView);
   }

   return result;
}

ossimObject* ossimImageRenderer::getView()
{
   if(theImageViewTransform)
   {
      return theImageViewTransform->getView();
   }
   return NULL;
}

const ossimObject* ossimImageRenderer::getView()const
{
   if(theImageViewTransform)
   {
      return theImageViewTransform->getView();
   }
   return NULL;
}

void ossimImageRenderer::getValidImageVertices(vector<ossimIpt>& validVertices,
                                               ossimVertexOrdering ordering,
                                               ossim_uint32 resLevel)const
{
   if(theInputConnection&&theImageViewTransform&&theImageViewTransform->isValid())
   {
      theInputConnection->getValidImageVertices(validVertices, ordering, resLevel);
      if(isSourceEnabled())
      {
         ossim_uint32 inputSize = (ossim_uint32)validVertices.size();
         if(inputSize)
         {
            ossimDpt viewPt;
			ossim_uint32 idx = 0;
            // transform each point to the view
            for(idx = 0; idx < inputSize; ++idx)
            {
               theImageViewTransform->imageToView(validVertices[idx], viewPt);
               
               // round it to the nearest pixel
               //
               validVertices[idx] = ossimIpt(viewPt);
            }
         }
      }
   }
}

bool ossimImageRenderer::getImageGeometry(ossimKeywordlist& kwl,
                                          const char* prefix)
{
   if(isSourceEnabled()&&getView())
   {
      const ossimObject* view = getView();
      
      bool result = view->saveState(kwl, prefix);

      return result;
   }
   else
   {
      if(theInputConnection)
      {
         return theInputConnection->getImageGeometry(kwl, prefix);
      }
   }

   return false;
}

void ossimImageRenderer::connectInputEvent(ossimConnectionEvent& event)
{
   theInputConnection = PTR_CAST(ossimImageSourceInterface, getInput(0));
   // All this code in here will change after the demo.  For now we need a
   // way to bring every one up with a projection.  Later we will have to
   // tie to a projection source.
   //
   if(!theImageViewTransform)
   {
      theImageViewTransform  = new ossimImageViewProjectionTransform;
   }
   
   checkTransform();

   initialize();
   
}

void ossimImageRenderer::disconnectInputEvent(ossimConnectionEvent& event)
{
   ossimImageViewProjectionTransform* transform = PTR_CAST(ossimImageViewProjectionTransform,
                                                           theImageViewTransform);
   if(transform)
   {
      transform->setImageProjection(NULL, true);
   }
   
   theInputConnection = NULL;
   theInputDecimationFactors.clear();
}

void ossimImageRenderer::propertyEvent(ossimPropertyEvent& event)
{
   checkTransform();
   initialize();
}


void ossimImageRenderer::setProperty(ossimRefPtr<ossimProperty> property)
{
  ossimString tempName = property->getName();
  
  if(tempName == "Filter type")
    {
      if(theResampler)
	{
	  theResampler->setFilterType(property->valueToString());
	}
    }
//   else if(tempName == "Blur factor")
//     {
//       if(theResampler)
// 	{
// 	  theResampler->setBlurFactor(property->valueToString().toDouble());
// 	}
//     }
  else
    {
      ossimImageSourceFilter::setProperty(property);
    }
}

ossimRefPtr<ossimProperty> ossimImageRenderer::getProperty(const ossimString& name)const
{
  ossimString tempName = name;

  if(tempName == "Filter type")
    {
      std::vector<ossimString> filterNames;
      theResampler->getFilterTypes(filterNames);
      
      ossimStringProperty* stringProp = new ossimStringProperty("Filter type",
								theResampler->getMinifyFilterTypeAsString(),
								false,
								filterNames);
      stringProp->clearChangeType();
      stringProp->setReadOnlyFlag(false);
      stringProp->setCacheRefreshBit();
      
      return stringProp;
    }
//   else if(tempName == "Blur factor")
//   {
//      ossimNumericProperty* numericProperty = new ossimNumericProperty("Blur factor",
//                                                                       ossimString::toString((double)theResampler->getBlurFactor()));
     
//      numericProperty->setConstraints(0.0, 50.0);
//      numericProperty->setNumericType(ossimNumericProperty::ossimNumericPropertyType_FLOAT64);
//      numericProperty->setCacheRefreshBit();

//      return numericProperty;
//   }

  return ossimImageSourceFilter::getProperty(name);
}

void ossimImageRenderer::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
  ossimImageSourceFilter::getPropertyNames(propertyNames);

  propertyNames.push_back("Filter type");
//  propertyNames.push_back("Blur factor");
}


void ossimImageRenderer::checkTransform()
{
  theBoundingViewRect.makeNan();
   if(!isSourceEnabled())
   {
      return;
   }

   
      
   // All this code in here might change after the demo.  For now we need a
   // way to bring every one up with a projection.  Later we will have to
   // tie to a projection source.
   ossimImageViewProjectionTransform* transform = PTR_CAST(ossimImageViewProjectionTransform,
                                                           theImageViewTransform);
   ossimImageSourceInterface* inter = PTR_CAST(ossimImageSourceInterface,
                                               getInput(0));

   // we will only check for projection transforms
   if(!transform||!inter) return;

   ossimProjection* proj = NULL;
   
//   if(theAutoUpdateInputTransform)//||
//      (!transform->getImageProjection()))
//    {
   if(!transform->getImageProjection()||
      PTR_CAST(ossimMapProjection, transform->getImageProjection()))
   {
      ossimKeywordlist kwl;
      inter->getImageGeometry(kwl);
      
      proj = ossimProjectionFactoryRegistry::instance()->createProjection(kwl);
      transform->setImageProjection(proj, true);
   }
   else
   {
      proj = transform->getImageProjection();
   }
//    }
   
   if(proj)
   {
      if(!transform->getView()) // check to see if we found a view controller and the view was set
      {                         // if not then we will create a default output projection
         ossimProjection* newProj = (ossimProjection*)NULL;
         ossimMapProjection* mapProj = PTR_CAST(ossimMapProjection, proj);
         if(mapProj&&!mapProj->hasModelTransform())
         {
            newProj = (ossimProjection*)proj->dup(); // Make a copy...
         }
         else
         {
            newProj = new ossimEquDistCylProjection;
         }
         mapProj = PTR_CAST(ossimMapProjection, newProj);
         if(mapProj) 
         {
            ossimDpt meters = proj->getMetersPerPixel();
	    
            double GSD = (meters.x + meters.y)/2.0;
            meters.x = GSD;
            meters.y = GSD;
            mapProj->setUlGpt(proj->origin());
            mapProj->setOrigin(proj->origin());
            mapProj->setMetersPerPixel(meters);
         }
         transform->setViewProjection(newProj);
      }
//       }
   }
   else
   {
      transform->setImageProjection(NULL, true);
   }
}


void ossimImageRenderer::getDecimationFactor(ossim_uint32 resLevel,
					     ossimDpt& result)const
{
  if(isSourceEnabled())
    {
      result = ossimDpt(1,1);
    }
  else
    {
      ossimImageSourceFilter::getDecimationFactor(resLevel,
						  result);
    }
}

void ossimImageRenderer::getDecimationFactors(vector<ossimDpt>& decimations)const
{
  if(isSourceEnabled())
    {
      decimations.push_back(ossimDpt(1,1));
    }
  else
    {
      ossimImageSourceFilter::getDecimationFactors(decimations);
    }
}

ossim_uint32 ossimImageRenderer::getNumberOfDecimationLevels()const
{
  if(isSourceEnabled())
    {
      return 1;
    }

  return ossimImageSourceFilter::getNumberOfDecimationLevels();
}
void ossimImageRenderer::stretchQuadOut(const ossimDpt& amount,
                                        ossimDpt& ul,
                                        ossimDpt& ur,
                                        ossimDpt& lr,
                                        ossimDpt& ll)
{
      ossimDpt upper  = ur - ul;
      ossimDpt right  = lr - ur;
      ossimDpt bottom = ll - lr;
      ossimDpt left   = ul - ll;

      upper = (upper*(1.0 / upper.length()));
      upper.x *= amount.x;
      upper.y *= amount.y;
      right = (right*(1.0 / right.length()));
      right.x *= amount.x;
      right.y *= amount.y;
      bottom = (bottom*(1.0 / bottom.length()));
      bottom.x *= amount.x;
      bottom.y *= amount.y;
      left = (left*(1.0 / left.length()));
      left.x *= amount.x;
      left.y *= amount.y;
      

      ul = ul - upper + left;
      ur = ur + upper - right;
      lr = lr + left - bottom;
      ll = ll - left + bottom;
}

ossimRefPtr<ossimImageData>  ossimImageRenderer::getTileAtResLevel(const ossimIrect& boundingRect,
                                                       ossim_uint32 resLevel)
{
   if(!theInputConnection)
   {
      theBlankTile->setImageRectangle(boundingRect);
      
      return theBlankTile;
   }
   
   ossim_uint32 levels = theInputConnection->getNumberOfDecimationLevels();
   
   // ossim_uint32 maxValue = (ossim_uint32)ossim::max((ossim_uint32)theBoundingRect.width(),
   //                                            (ossim_uint32)theBoundingRect.height());
   
   if(resLevel == 0)
   {
      return theInputConnection->getTile(boundingRect);
   }
   if(resLevel < levels)
   {
      if(theInputConnection)
      {
         return theInputConnection->getTile(boundingRect,
                                            resLevel);
      }
      else
      {
         return theBlankTile;
      }
   }
   else if((resLevel - levels) < theMaxLevelsToCompute)
   {
      // check to see how many decimations we must achiev for the
      // request
      //
      int decimations = (resLevel - (levels-1));
      ossimIpt tileSize(theInputConnection->getTileWidth(),
                        theInputConnection->getTileHeight());
      
      ossim_int32 multiplier = (1 << decimations);
      
      ossim_int32 xIndex = 0;
      ossim_int32 yIndex = 0;
      
      // adjust the tilesize so it at least will cover the multiplier
      // We will probably come up with something better later but for now
      // this will do.
      if(multiplier > tileSize.x)
      {
         tileSize.x = multiplier;
      }
      if(multiplier > tileSize.y)
      {
         tileSize.y = multiplier;
      }
      
      // set the rect that covers the requested RLevel to the valid RLevel. 
      // the valid RLevel is what is available on the input side.
      //
      ossimIrect requestedRectAtValidRLevel = boundingRect;
      requestedRectAtValidRLevel.stretchToTileBoundary(tileSize);
      ossimIrect mappedRequestedRect = requestedRectAtValidRLevel;
      requestedRectAtValidRLevel  = requestedRectAtValidRLevel*((double)multiplier);
      
      if(!theTemporaryBuffer)
      {
         theTemporaryBuffer = (ossimImageData*)theBlankTile->dup();
         theTemporaryBuffer->setImageRectangle(mappedRequestedRect);
         theTemporaryBuffer->initialize();
      }
      else
      {
         theTemporaryBuffer->setImageRectangle(mappedRequestedRect);
         theTemporaryBuffer->makeBlank();
      }
      
      ossim_uint32 totalCount   = ((requestedRectAtValidRLevel.lr().y-requestedRectAtValidRLevel.ul().y)*
                                   (requestedRectAtValidRLevel.lr().x-requestedRectAtValidRLevel.ul().x));
      ossim_uint32 currentCount = 0;
      ossimIrect boundingRect = theInputConnection->getBoundingRect(levels-1);
      for(yIndex = requestedRectAtValidRLevel.ul().y;yIndex < requestedRectAtValidRLevel.lr().y; yIndex += tileSize.y)
      {
         for(xIndex = requestedRectAtValidRLevel.ul().x; xIndex < requestedRectAtValidRLevel.lr().x; xIndex+=tileSize.x)
         {
            ossimIrect request(xIndex,
                               yIndex,
                               xIndex + (tileSize.x-1),
                               yIndex + (tileSize.y-1));
            ossimRefPtr<ossimImageData> data = theInputConnection->getTile(request, levels-1);
            
            if(data.valid() && (data->getDataObjectStatus()!=OSSIM_EMPTY)&&
               data->getBuf()&&
               boundingRect.intersects(request))
            {
               switch(data->getScalarType())
               {
                  case OSSIM_UINT8:
                  {
                     resampleTileToDecimation((ossim_uint8)0,
                                              theTemporaryBuffer,
                                              data,
                                              multiplier);
                     break;
                  }
                  case OSSIM_SINT16:
                  {
                     resampleTileToDecimation((ossim_sint16)0,
                                              theTemporaryBuffer,
                                              data,
                                              multiplier);
                     break;
                  }
                  case OSSIM_UINT16:
                  case OSSIM_USHORT11:
                  {
                     resampleTileToDecimation((ossim_uint16)0,
                                              theTemporaryBuffer,
                                              data,
                                              multiplier);
                     break;
                  }
                  case OSSIM_FLOAT32:
                  case OSSIM_NORMALIZED_FLOAT:
                  {
                     resampleTileToDecimation((ossim_float32)0,
                                              theTemporaryBuffer,
                                              data,
                                              multiplier);
                     break;
                  }
                  case OSSIM_FLOAT64:
                  case OSSIM_NORMALIZED_DOUBLE:
                  {
                     resampleTileToDecimation((ossim_float64)0,
                                              theTemporaryBuffer,
                                              data,
                                              multiplier);
                     break;
                  }
                  case OSSIM_SCALAR_UNKNOWN:
                  default:
                  {
                     break;
                  }
               }
            }
            ++currentCount;
            fireProgressEvent((double)currentCount/
                              (double)totalCount);
            
         }
      }
      theTemporaryBuffer->validate();
      return theTemporaryBuffer;
   }

   return 0;
}

void ossimImageRenderer::setMaxLevelsToCompute(ossim_uint32 maxLevels)
{
   theMaxLevelsToCompute = maxLevels;
}

ossim_uint32 ossimImageRenderer::getMaxLevelsToCompute()const
{
   return theMaxLevelsToCompute;
}

template <class T>
void ossimImageRenderer::resampleTileToDecimation(T dummyVariable,
						  ossimRefPtr<ossimImageData> result,
						  ossimRefPtr<ossimImageData> tile,
						  ossim_uint32 multiplier)
{
  if(tile->getDataObjectStatus() == OSSIM_EMPTY ||
     !tile->getBuf())
    {
      return;
    }

  ossim_int32 maxX     = (ossim_int32)tile->getWidth();
  ossim_int32 maxY     = (ossim_int32)tile->getHeight();
  ossim_int32 resultHeight = result->getHeight();
  ossim_int32* offsetX = new ossim_int32[maxX];
  ossim_int32* offsetY = new ossim_int32[maxY];
  ossim_int32 i        = 0;
  ossim_int32 resultWidth     = (ossim_int32)result->getWidth();
  ossimIpt tileOrigin   = tile->getOrigin();
  ossimIpt resultOrigin = result->getOrigin();

  // create a lookup table. that maps the tile to the result
  for(i = 0; i < maxX; ++i)
    {
       offsetX[i] = (i+tileOrigin.x)/(ossim_int32)multiplier - resultOrigin.x;
       if(offsetX[i] < 0 )
       {
          offsetX[i] = 0;
       }
       else if(offsetX[i] >= resultWidth)
       {
          offsetX[i] = resultWidth-1;
       }
    }
  for(i = 0; i < maxY; ++i)
    {
      offsetY[i] = ( ((i+tileOrigin.y)/(ossim_int32)multiplier) - resultOrigin.y);
      if(offsetY[i] < 0 )
      {
         offsetY[i] = 0;
      }
      else if(offsetY[i] >= resultHeight)
      {
         offsetY[i] = resultHeight-1;
      }
      offsetY[i] *= resultWidth;
    }
  
  if(tile->getDataObjectStatus() == OSSIM_FULL)
    {
      ossim_int32 numberOfBands = (ossim_int32)std::min(result->getNumberOfBands(),
					    tile->getNumberOfBands());
      ossim_int32 band = 0;
      for(band = 0; band < numberOfBands; ++band)
	{
	  T* tileBuf   = static_cast<T*>(tile->getBuf(band));
	  T* resultBuf = static_cast<T*>(result->getBuf(band));
	  ossim_int32 dx = 0;
	  ossim_int32 dy = 0;
	  ossim_int32 boxAverageX = 0;
	  ossim_int32 boxAverageY = 0;
	  for(dy = 0; dy < maxY; dy+=multiplier)
	    {
	      for(dx = 0; dx < maxX; dx+=multiplier)
		{
		  double sum = 0.0;
		  for(boxAverageY = 0; 
		      ((boxAverageY < (ossim_int32)multiplier)&& 
		       ((boxAverageY+dy)<maxY)); ++boxAverageY)
		    {
		      for(boxAverageX = 0; 
			  ((boxAverageX < (ossim_int32)multiplier)&& 
			   ((boxAverageX+dx)<maxX)); ++boxAverageX)
			{
			  sum += tileBuf[((boxAverageY+dy)*maxX + boxAverageX + dx)];
			}
		    }
		  sum /= (double)(multiplier*multiplier);
		  resultBuf[ offsetX[dx] + offsetY[dy] ] = (T)sum;
		}
	    }
	}
    }
  else
    {
      ossim_int32 numberOfBands = (ossim_int32)std::min(result->getNumberOfBands(),
					    tile->getNumberOfBands());
      ossim_int32 band = 0;
      for(band = 0; band < numberOfBands; ++band)
	{
	  T* tileBuf   = static_cast<T*>(tile->getBuf(band));
	  T* resultBuf = static_cast<T*>(result->getBuf(band));
	  T tileBufNp  = static_cast<T>(tile->getNullPix(band));
	  ossim_int32 dx = 0;
	  ossim_int32 dy = 0;
	  ossim_int32 boxAverageX = 0;
	  ossim_int32 boxAverageY = 0;
	  for(dy = 0; dy < maxY; dy+=multiplier)
	    {
	      for(dx = 0; dx < maxX; dx+=multiplier)
		{
		  double sum = 0.0;
		  if(tileBuf[((dy+(multiplier>>1))*maxX + dx+(multiplier>>1))] != tileBufNp)
		    {
		      ossim_uint32 nullCount = 0;
		      for(boxAverageY = 0; 
			  ((boxAverageY < (ossim_int32)multiplier)&& 
			   ((boxAverageY+dy)<maxY)); ++boxAverageY)
			{
			  for(boxAverageX = 0; 
			      ((boxAverageX < (ossim_int32)multiplier)&& 
			       ((boxAverageX+dx)<maxX)); ++boxAverageX)
			    {
			      T value = tileBuf[((boxAverageY+dy)*maxX + boxAverageX + dx)];
			      if(value != tileBufNp)
				{
				  sum += value;
				}
			      else
				{
				  ++nullCount;
				}
			    }
			}
		      ossim_uint32 area = multiplier*multiplier;
		      sum /= (double)(area);
		      if(nullCount!= area)
			{
			  resultBuf[ offsetX[dx] + offsetY[dy] ] = (T)sum;
			}
		    }
		}
	    }
	}
      
    }

  delete [] offsetX;
  delete [] offsetY;
}

ossimString ossimImageRenderer::getLongName() const
{
   return ossimString("Image Renderer");
}

ossimString ossimImageRenderer::getShortName() const
{
   return ossimString("Image Renderer"); 
}

 void ossimImageRenderer::enableSource()
{
   ossimImageSourceFilter::enableSource();
   checkTransform();
   initialize();
}

void ossimImageRenderer::fireProgressEvent(double percentComplete)
{
   ossimProcessProgressEvent event(this, percentComplete);
   fireEvent(event);
}
