#include <math.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/projection/ossimImageViewTransform.h>
#include <ossim/base/ossimDrect.h>
#include <ossim/base/ossimIrect.h>

RTTI_DEF2(ossimImageViewTransform, "ossimImageViewTransform", ossim2dTo2dTransform, ossimViewInterface)

ossimImageViewTransform::ossimImageViewTransform()
    :
       ossim2dTo2dTransform(),
       ossimViewInterface(NULL)
{
   ossimViewInterface::theObject = this;
}

void ossimImageViewTransform::forward(const ossimDpt& input,
                                      ossimDpt& output) const
{
   imageToView(input, output);
}

void ossimImageViewTransform::inverse(const ossimDpt& input,
                                      ossimDpt& output) const
{
   viewToImage(input, output);
}

void ossimImageViewTransform::imageToView(const ossimDpt& imagePoint,
                                          ossimDpt&       viewPoint)const
{
   viewPoint = imagePoint;
}

void ossimImageViewTransform::viewToImage(const ossimDpt& viewPoint,
                                          ossimDpt&       imagePoint)const
{
   ossim2dTo2dTransform::inverse(viewPoint, imagePoint);
}

ossimDpt ossimImageViewTransform::imageToView(const ossimDpt& imagePoint)const
{
   ossimDpt tempPt;
   
   imageToView(imagePoint, tempPt);
   
   return tempPt;
}

ossimDpt ossimImageViewTransform::viewToImage(const ossimDpt& viewPoint)const
{
   ossimDpt tempPt;
   
   viewToImage(viewPoint, tempPt);
   
   return tempPt;
}

void ossimImageViewTransform::getRoundTripErrorView(ossimDpt& result,
                                                    const ossimDpt& /* viewPt */)const
{
   result = ossimDpt(0,0);
}

ossimDpt ossimImageViewTransform::getRoundTripErrorView(const ossimDpt& viewPt)const
{
   ossimDpt result;
   getRoundTripErrorView(result, viewPt);
   return result;
}

void ossimImageViewTransform::getRoundTripErrorImage(ossimDpt& result,
                                                     const ossimDpt& /* imagePt */)const
{
   result = ossimDpt(0,0);
}

ossimDpt ossimImageViewTransform::getRoundTripErrorImage(const ossimDpt& imagePt)const
{
   ossimDpt result;
   getRoundTripErrorView(result, imagePt);
   return result;
}
  
bool ossimImageViewTransform::loadState(const ossimKeywordlist& kwl,
                                        const char* prefix)
{
   return ossim2dTo2dTransform::loadState(kwl, prefix);
}

bool ossimImageViewTransform::saveState(ossimKeywordlist& kwl,
                                        const char* prefix)const
{
   return ossim2dTo2dTransform::saveState(kwl, prefix);
}

void ossimImageViewTransform::getScaleChangeImageToView(ossimDpt& result,
                                                        const ossimDrect& imageRect)
{
   result.makeNan();
   if(!imageRect.hasNans())
   {
      ossimDpt vul;
      ossimDpt vur;
      ossimDpt vlr;
      ossimDpt vll;

      imageToView(imageRect.ul(),
                  vul);
      imageToView(imageRect.ur(),
                  vur);
      imageToView(imageRect.lr(),
                  vlr);
      imageToView(imageRect.ll(),
                  vll);

      if(!vul.hasNans()&&
         !vur.hasNans()&&
         !vlr.hasNans()&&
         !vll.hasNans())
      {
         
         double deltaTop    = (vul - vur).length();
         double deltaBottom = (vll - vlr).length();
         double deltaRight  = (vur - vlr).length();
         
         double w = imageRect.width();
         double h = imageRect.height();
         result.x = (deltaTop/w + deltaBottom/w)*.5;
         result.y = (deltaRight/h + deltaRight/h)*.5;
      }
   }
}

void ossimImageViewTransform::getScaleChangeViewToImage(ossimDpt& result,
                                                        const ossimDrect& viewRect)
{
   result.makeNan();
   if(!viewRect.hasNans())
   {
      ossimDpt iul;
      ossimDpt iur;
      ossimDpt ilr;
      ossimDpt ill;

      imageToView(viewRect.ul(),
                  iul);
      imageToView(viewRect.ur(),
                  iur);
      imageToView(viewRect.lr(),
                  ilr);
      imageToView(viewRect.ll(),
                  ill);

      if(!iul.hasNans()&&
         !iur.hasNans()&&
         !ilr.hasNans()&&
         !ill.hasNans())
      {
         
         double deltaTop    = (iul - iur).length();
         double deltaBottom = (ill - ilr).length();
         double deltaRight  = (iur - ilr).length();
         
         double w = viewRect.width();
         double h = viewRect.height();
         result.x = (deltaTop/w + deltaBottom/w)*.5;
         result.y = (deltaRight/h + deltaRight/h)*.5;
      }
   }
}

void ossimImageViewTransform::getImageToViewScale(ossimDpt& resultScale,
						  const ossimDpt& imagePoint,
						  const ossimDpt& deltaImagePointXY)const
{
  ossimDpt p1 = imagePoint;
  ossimDpt p2(imagePoint.x + deltaImagePointXY.x,
	      imagePoint.y);
  ossimDpt p3(imagePoint.x,
	      imagePoint.y + deltaImagePointXY.y);
  
  ossimDpt transformedP1;
  ossimDpt transformedP2;
  ossimDpt transformedP3;

  imageToView(p1, transformedP1);
  imageToView(p2, transformedP2);
  imageToView(p3, transformedP3);

  ossimDpt deltaP1P2 = transformedP1 - transformedP2;
  ossimDpt deltaP1P3 = transformedP1 - transformedP3;
 
  // now compute the distances.
  double sumSquaredSqrtP1P2 = sqrt((deltaP1P2.x*deltaP1P2.x) +
			       (deltaP1P2.y*deltaP1P2.y));
  double sumSquaredSqrtP1P3 = sqrt((deltaP1P3.x*deltaP1P3.x) +
			       (deltaP1P3.y*deltaP1P3.y));

  resultScale.x = 0;
  resultScale.y = 0;

  if(sumSquaredSqrtP1P2 > FLT_EPSILON)
    {
      resultScale.x = sumSquaredSqrtP1P2/deltaImagePointXY.x;
    }
  if(sumSquaredSqrtP1P3 > FLT_EPSILON)
    {
      resultScale.y = sumSquaredSqrtP1P3/deltaImagePointXY.y;
    }
}

void ossimImageViewTransform::getViewToImageScale(ossimDpt& resultScale,
						  const ossimDpt& viewPoint,
						  const ossimDpt& deltaViewPointXY)const
{
  ossimDpt p1 = viewPoint;
  ossimDpt p2(viewPoint.x + deltaViewPointXY.x,
	      viewPoint.y);
  ossimDpt p3(viewPoint.x,
	      viewPoint.y + deltaViewPointXY.y);

  ossimDpt transformedP1;
  ossimDpt transformedP2;
  ossimDpt transformedP3;

  viewToImage(p1, transformedP1);
  viewToImage(p2, transformedP2);
  viewToImage(p3, transformedP3);

  ossimDpt deltaP1P2 = transformedP1 - transformedP2;
  ossimDpt deltaP1P3 = transformedP1 - transformedP3;

  // now compute the distances.
  double sumSquaredSqrtP1P2 = sqrt((deltaP1P2.x*deltaP1P2.x) +
                                   (deltaP1P2.y*deltaP1P2.y));
  double sumSquaredSqrtP1P3 = sqrt((deltaP1P3.x*deltaP1P3.x) +
                                   (deltaP1P3.y*deltaP1P3.y));

  resultScale.x = 0;
  resultScale.y = 0;

  if(sumSquaredSqrtP1P2 > FLT_EPSILON)
    {
      resultScale.x = sumSquaredSqrtP1P2/deltaViewPointXY.x;
    }
  if(sumSquaredSqrtP1P3 > FLT_EPSILON)
    {
      resultScale.y = sumSquaredSqrtP1P3/deltaViewPointXY.y;
    }
}

ossimDrect ossimImageViewTransform::getImageToViewBounds(const ossimDrect& imageRect)const
{
   ossimDpt p1;
   ossimDpt p2;
   ossimDpt p3;
   ossimDpt p4;

   imageToView(imageRect.ul(), p1);
   imageToView(imageRect.ur(), p2);
   imageToView(imageRect.lr(), p3);
   imageToView(imageRect.ll(), p4);

   return ossimDrect(p1, p2, p3, p4);
}

std::ostream& ossimImageViewTransform::print(std::ostream& out) const
{
   return out;
}

std::ostream& operator<<(std::ostream& out,
                         const ossimImageViewTransform& data)
{
   return data.print(out);
}

