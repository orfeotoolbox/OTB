//*******************************************************************
// Copyright (C) 2001 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//*******************************************************************
// $Id: ossimImageRenderer.h,v 1.63 2005/11/09 14:31:23 gpotts Exp $

#ifndef ossimImageRenderer_HEADER
#define ossimImageRenderer_HEADER
#include <imaging/tile_sources/ossimImageSourceFilter.h>
#include <base/common/ossimViewInterface.h>
#include <base/data_types/ossimRationalNumber.h>

class ossimImageViewTransform;
class ossimImageData;
class ossimDiscreteConvolutionKernel;
class ossimFilterResampler;

class OSSIMDLLEXPORT ossimImageRenderer : public ossimImageSourceFilter,
                                          public ossimViewInterface
{
public:
   ossimImageRenderer();
   ossimImageRenderer(ossimImageSource* inputSource,
                      ossimImageViewTransform* imageViewTrans = NULL);
   virtual ~ossimImageRenderer();

   virtual ossimString getLongName()  const;
   virtual ossimString getShortName() const;

   virtual void enableSource();
   
   /**
    * the resampler will need the tile request to come from the view.
    * It will use the view to transoform this to world and then use the
    * image's projection to get it into final line sample.  This way
    * the resampler can fill a tile width by height on the screen
    * correctly.
    */
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& origin,
                                               ossim_uint32 resLevel=0);

   virtual void initialize();

   /**
    * TheResampler will adjust the rect to whatever the view is.  So it
    * will project the full image rect onto the view and return the upright
    * bounding rect.
    */
   virtual ossimIrect getBoundingRect(ossim_uint32 resLevel=0)const;

   /**
    * Method to save the state of an object to a keyword list.
    * Return true if ok or false on error.
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;
   
   /**
    * Method to the load (recreate) the state of an object from a keyword
    * list.  Return true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);

   void setImageViewTransform(ossimImageViewTransform* transform);
   ossimImageViewTransform* getImageViewTransform(){return theImageViewTransform;}

   virtual bool getImageGeometry(ossimKeywordlist& kwl,
                                 const char* prefix=0);
   virtual bool setView(ossimObject* baseObject,
                        bool ownsTheView = false);
   ossimFilterResampler* getResampler()
      {
         return theResampler;
      }
   virtual ossimObject* getView();
   virtual const ossimObject* getView()const;

   virtual void getDecimationFactor(ossim_uint32 resLevel,
                                    ossimDpt& result)const;
  virtual void getDecimationFactors(vector<ossimDpt>& decimations)const;
  virtual ossim_uint32 getNumberOfDecimationLevels()const;
  

   virtual void setAutoUpdateInputTransformFlag(bool flag)
      {
         theAutoUpdateInputTransform = flag;
      }
   /**
    * ordering specifies how the vertices should be arranged.
    * valid image vertices is basically the tightly fit convex hull
    * of the image.  Usually an image has NULL values and are
    * internally not upright rectangular.  This can cause
    * problems some spatial filters.
    *
    * We need to make sure that the resampler overrides this method.
    * it needs to transform the vertices to the view's side.
    */
   virtual void getValidImageVertices(vector<ossimIpt>& validVertices,
                                      ossimVertexOrdering ordering=OSSIM_CLOCKWISE_ORDER,
                                      ossim_uint32 resLevel=0)const;

   void connectInputEvent(ossimConnectionEvent& event);
   void disconnectInputEvent(ossimConnectionEvent& event);
   void propertyEvent(ossimPropertyEvent& event);
   
   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;

private:
   

   class ossimRendererSubRectInfo
   {
   public:
      friend std::ostream& operator <<(std::ostream& out,
                                       const ossimRendererSubRectInfo& rhs)
         {
            return out << "vul:   " << rhs.theVul << endl
                       << "vur:   " << rhs.theVur << endl
                       << "vlr:   " << rhs.theVlr << endl
                       << "vll:   " << rhs.theVll << endl
                       << "iul:   " << rhs.theIul << endl
                       << "iur:   " << rhs.theIur << endl
                       << "ilr:   " << rhs.theIlr << endl
                       << "ill:   " << rhs.theIll << endl
                       << "scale: " << rhs.theViewToImageScale << endl;
               
         }
      ossimRendererSubRectInfo()
         {
            theVul.makeNan();
            theVur.makeNan();
            theVlr.makeNan();
            theVll.makeNan();
            theIul.makeNan();
            theIur.makeNan();
            theIlr.makeNan();
            theIll.makeNan();
            theViewToImageScale.makeNan();
            theImageToViewScale.makeNan();            
         }
      ossimRendererSubRectInfo(const ossimDpt& vul,
                               const ossimDpt& vur,
                               const ossimDpt& vlr,
                               const ossimDpt& vll)
         :theVul(vul),
          theVur(vur),
          theVlr(vlr),
          theVll(vll)
         {
            theIul.makeNan();
            theIur.makeNan();
            theIlr.makeNan();
            theIll.makeNan();
            theViewToImageScale.makeNan();
            theImageToViewScale.makeNan();            
         }
         
      bool imageHasNans()const
         {
            return ( theIul.hasNans()||
                     theIur.hasNans()||
                     theIlr.hasNans()||
                     theIll.hasNans());
         }
      
      bool imageIsNan()const
         {
            return ( theIul.hasNans()&&
                     theIur.hasNans()&&
                     theIlr.hasNans()&&
                     theIll.hasNans());
         }
      bool viewHasNans()const
      {
         return ( theVul.hasNans()||
                  theVur.hasNans()||
                  theVlr.hasNans()||
                  theVll.hasNans());
      }
      bool viewIsNan()const
      {
         return ( theVul.hasNans()&&
                  theVur.hasNans()&&
                  theVlr.hasNans()&&
                  theVll.hasNans());
      }
      void splitView(ossimImageViewTransform* transform,
                     ossimRendererSubRectInfo& ulRect,
                     ossimRendererSubRectInfo& urRect,
                     ossimRendererSubRectInfo& lrRect,
                     ossimRendererSubRectInfo& llRect)const;
      
      void transformViewToImage(ossimImageViewTransform* transform);
      void transformImageToView(ossimImageViewTransform* transform);
      
      void roundToInteger()
         {
            theIul = ossimIpt(theIul);
            theIur = ossimIpt(theIur);
            theIlr = ossimIpt(theIlr);
            theIll = ossimIpt(theIll);

            theVul = ossimIpt(theVul);
            theVur = ossimIpt(theVur);
            theVlr = ossimIpt(theVlr);
            theVll = ossimIpt(theVll);
         }
      void stretchImageOut(bool enableRound=false);
      ossimDrect getViewRect()const
         {
            return ossimDrect(theVul,
                              theVur,
                              theVlr,
                              theVll);            
         }
      ossimDrect getImageRect()const
         {
            return ossimDrect(theIul,
                              theIur,
                              theIlr,
                              theIll);
         }
      void roundImageToInteger()
         {
            theIul = ossimIpt(theIul);
            theIur = ossimIpt(theIur);
            theIlr = ossimIpt(theIlr);
            theIll = ossimIpt(theIll);
         }
      void roundViewToInteger()
         {
            theVul = ossimIpt(theVul);
            theVur = ossimIpt(theVur);
            theVlr = ossimIpt(theVlr);
            theVll = ossimIpt(theVll);
         }
      bool isViewEqual(const ossimRendererSubRectInfo& infoRect)const
         {
            return ( (theVul == infoRect.theVul)&&
                     (theVur == infoRect.theVur)&&
                     (theVlr == infoRect.theVlr)&&
                     (theVll == infoRect.theVll));
         }
      bool isViewEqual(const ossimDrect& viewRect)const
         {
            return ( (theVul == viewRect.ul())&&
                     (theVur == viewRect.ur())&&
                     (theVlr == viewRect.lr())&&
                     (theVll == viewRect.ll()));
         }
      ossimDpt getAbsValueViewToImageScales()const
         {
            if(theViewToImageScale.hasNans())
            {
               return theImageToViewScale;
            }
            return ossimDpt(fabs(theViewToImageScale.x),
                            fabs(theViewToImageScale.y));
         }
      ossimDpt getAbsValueImageToViewScales()const
         {
            if(theImageToViewScale.hasNans())
            {
               return theImageToViewScale;
            }
            
            return ossimDpt(fabs(theImageToViewScale.x),
                            fabs(theImageToViewScale.y));
         }
      bool isViewAPoint()const
         {
            return ((theVul == theVur)&&
                    (theVul == theVlr)&&
                    (theVul == theVll));
         }
      bool isIdentity()const;
      bool canBilinearInterpolate(ossimImageViewTransform* transform,
				  double error)const;

      ossimDpt getParametricCenter(const ossimDpt& ul, const ossimDpt& ur, 
				    const ossimDpt& lr, const ossimDpt& ll)const;

     void getViewMids(ossimDpt& upperMid,
		      ossimDpt& rightMid,
		      ossimDpt& bottomMid,
		      ossimDpt& leftMid,
		      ossimDpt& center)const;

     void getImageMids(ossimDpt& upperMid,
		       ossimDpt& rightMid,
		       ossimDpt& bottomMid,
		       ossimDpt& leftMid,
		       ossimDpt& center)const;

      ossimDpt theIul;
      ossimDpt theIur;
      ossimDpt theIlr;
      ossimDpt theIll;

      ossimIpt theVul;
      ossimIpt theVur;
      ossimIpt theVlr;
      ossimIpt theVll;

      ossimDpt theViewToImageScale;
      ossimDpt theImageToViewScale;
      
   };

   void recursiveResample(ossimRefPtr<ossimImageData> outputData,
                          const ossimRendererSubRectInfo& rectInfo,
			  ossim_uint32 level);
   

   void fillTile(ossimRefPtr<ossimImageData> outputData,
                 const ossimRendererSubRectInfo& rectInfo);
                 
   ossimIrect getBoundingImageRect()const;

   
   /**
    *this is called on a property event and on input connection changes.
    */
   void checkTransform();

   void computeRects();

   ossimRefPtr<ossimImageData> getTileAtResLevel(const ossimIrect& boundingRect,
                                     ossim_uint32 resLevel);
  template <class T>
  void resampleTileToDecimation(T dummyVariable,
				ossimRefPtr<ossimImageData> result,
				ossimRefPtr<ossimImageData> tile,
				ossim_uint32 multiplier);

   long computeClosestResLevel(const std::vector<ossimDpt>& decimationFactors,
                               double scale)const;
   void stretchQuadOut(const ossimDpt& amount,
                       ossimDpt& ul,
                       ossimDpt& ur,
                       ossimDpt& lr,
                       ossimDpt& ll);

   /**
    * Called on first getTile, will initialize all data needed.
    */
   void allocate();

   /**
    *  Deletes all allocated tiles.
    */
   void deallocate();

   virtual void fireProgressEvent(double percentComplete);
   
   ossimFilterResampler*       theResampler;
   ossimRefPtr<ossimImageData> theBlankTile;
   ossimRefPtr<ossimImageData> theTile;
   ossimRefPtr<ossimImageData> theTemporaryBuffer;
   ossimRefPtr<ossimImageData> theCachedTile;
   ossim_uint32                theCachedTileResLevel;

   /**
    * This is going to allow us to chain multiple
    * renderers together.  So if we have one
    * renderer doing a scale and they pass an r-level
    * down and we have another renderer within the
    * chain he will be starting at a different r-level.
    * The default will be r-level 0 request coming
    * from the right.
    */
   ossim_uint32             theStartingResLevel;
   ossimImageViewTransform *theImageViewTransform;
   mutable ossimIrect               theBoundingRect;
   mutable ossimIrect               theBoundingViewRect;

   /**
    * Holds a pre-computed input rect.
    */
//   ossimIrect                theInputRect;

   /**
    * Holds a precomputed output rect.
    */
//   ossimIrect                theOutputRect;

   ossim_uint32               theMaxRecursionLevel;
   bool                       theAutoUpdateInputTransform;
   vector<ossimDpt>           theInputDecimationFactors;
TYPE_DATA
};

#endif
