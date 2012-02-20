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
// $Id: ossimImageRenderer.h 20236 2011-11-09 15:36:38Z dburken $

#ifndef ossimImageRenderer_HEADER
#define ossimImageRenderer_HEADER
#include <ossim/imaging/ossimImageSourceFilter.h>
#include <ossim/projection/ossimImageViewTransform.h>
#include <ossim/base/ossimDrect.h>
#include <ossim/base/ossimViewInterface.h>
#include <ossim/base/ossimRationalNumber.h>

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

   virtual ossimString getLongName()  const;
   virtual ossimString getShortName() const;

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
    * m_Resampler will adjust the rect to whatever the view is.  So it
    * will project the full image rect onto the view and return the upright
    * bounding rect.
    */
   virtual ossimIrect getBoundingRect(ossim_uint32 resLevel=0)const;

   /**
    * @brief Gets the bounding rectangle of the source.
    *
    * This is the output view bounds.
    * 
    * @param rect Initialized with bounding rectangle by this.
    * @param resLevel Reduced resolution level if applicable.
    */
   virtual void getBoundingRect(ossimIrect& rect,
                                ossim_uint32 resLevel=0) const;

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
   ossimImageViewTransform* getImageViewTransform() { return m_ImageViewTransform.get(); }

   //! Returns instance to the input image geometry. This may be a NULL pointer.
   //! This is only valid if the IVT is a projection type IVT (IVPT) 
   virtual ossimRefPtr<ossimImageGeometry> getImageGeometry();

   virtual bool setView(ossimObject* baseObject);
   ossimFilterResampler* getResampler() { return m_Resampler; }
   virtual ossimObject* getView();
   virtual const ossimObject* getView()const;

   virtual void getDecimationFactor(ossim_uint32 resLevel, ossimDpt& result)const;
  virtual void getDecimationFactors(vector<ossimDpt>& decimations)const;
  virtual ossim_uint32 getNumberOfDecimationLevels()const;
  
   virtual void setAutoUpdateInputTransformFlag(bool flag){ m_AutoUpdateInputTransform = flag; }
   
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

   void setMaxLevelsToCompute(ossim_uint32 maxLevels);
   ossim_uint32 getMaxLevelsToCompute()const;
   
   void connectInputEvent(ossimConnectionEvent& event);
   void disconnectInputEvent(ossimConnectionEvent& event);
   void propertyEvent(ossimPropertyEvent& event);
   virtual void refreshEvent(ossimRefreshEvent& event);
   
   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;

   /**
    * @brief Enables source.
    *
    * Overrides ossimSource::enableSource so bounding rects are recomputed on
    * state change.
    */
   virtual void enableSource();

   /**
    * @brief Disables source.
    *
    * Overrides ossimSource::disableSource so bounding rects are recomputed on
    * state change.
    */
   virtual void disableSource();

   /**
    * @brief Sets the enable flag.
    *
    * @param flag True to enable, false to disable.
    * 
    * Overrides ossimSource::setEnableFlag so bounding rects are recomputed on
    * state change.
    */
   virtual void setEnableFlag(bool flag);

   
protected:
   virtual ~ossimImageRenderer();

private:
   
   class ossimRendererSubRectInfo
   {
   public:
      friend std::ostream& operator <<(std::ostream& out, const ossimRendererSubRectInfo& rhs)
      {
         return out << "vul:   " << rhs.m_Vul << endl
            << "vur:   " << rhs.m_Vur << endl
            << "vlr:   " << rhs.m_Vlr << endl
            << "vll:   " << rhs.m_Vll << endl
            << "iul:   " << rhs.m_Iul << endl
            << "iur:   " << rhs.m_Iur << endl
            << "ilr:   " << rhs.m_Ilr << endl
            << "ill:   " << rhs.m_Ill << endl
            << "scale: " << rhs.m_ViewToImageScale << endl;

      }

      ossimRendererSubRectInfo();
      ossimRendererSubRectInfo(const ossimDpt& vul,
                               const ossimDpt& vur,
                               const ossimDpt& vlr,
                               const ossimDpt& vll);
         
      bool imageHasNans()const;
      bool imageIsNan()const;
      bool viewHasNans()const;
      bool viewIsNan()const;
      void splitView(ossimImageViewTransform* transform,
                     ossimRendererSubRectInfo& ulRect,
                     ossimRendererSubRectInfo& urRect,
                     ossimRendererSubRectInfo& lrRect,
                     ossimRendererSubRectInfo& llRect)const;
      
      void transformViewToImage(ossimImageViewTransform* transform);
      void transformImageToView(ossimImageViewTransform* transform);
      
      void roundToInteger();
      void stretchImageOut(bool enableRound=false);
      ossimDrect getViewRect()const;
      ossimDrect getImageRect()const;
      void roundImageToInteger();
      void roundViewToInteger();
      bool isViewEqual(const ossimRendererSubRectInfo& infoRect)const;
      bool isViewEqual(const ossimDrect& viewRect)const;
      ossimDpt getAbsValueViewToImageScales()const;
      ossimDpt getAbsValueImageToViewScales()const;
      bool isViewAPoint()const;
      bool isIdentity()const;
      bool canBilinearInterpolate(ossimImageViewTransform* transform, double error)const;

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

      ossimDpt m_Iul;
      ossimDpt m_Iur;
      ossimDpt m_Ilr;
      ossimDpt m_Ill;

      ossimIpt m_Vul;
      ossimIpt m_Vur;
      ossimIpt m_Vlr;
      ossimIpt m_Vll;

      ossimDpt m_ViewToImageScale;
      ossimDpt m_ImageToViewScale;
      
   };

   void recursiveResample(ossimRefPtr<ossimImageData> outputData,
                          const ossimRendererSubRectInfo& rectInfo,
			  ossim_uint32 level);
   

   void fillTile(ossimRefPtr<ossimImageData> outputData,
                 const ossimRendererSubRectInfo& rectInfo);
                 
   ossimIrect getBoundingImageRect()const;

   
   //! this is called on a property event and on input connection changes.
   void checkIVT();

   /**
    * @brief Initializes m_inputR0Rect and m_viewBoundingRect and sets
    * m_rectsDirty appropriately.
    */
   void initializeBoundingRects();

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

   ossimFilterResampler*       m_Resampler;
   ossimRefPtr<ossimImageData> m_BlankTile;
   ossimRefPtr<ossimImageData> m_Tile;
   ossimRefPtr<ossimImageData> m_TemporaryBuffer;

   /**
    * This is going to allow us to chain multiple
    * renderers together.  So if we have one
    * renderer doing a scale and they pass an r-level
    * down and we have another renderer within the
    * chain he will be starting at a different r-level.
    * The default will be r-level 0 request coming
    * from the right.
    */
   ossim_uint32             m_StartingResLevel;
   ossimRefPtr<ossimImageViewTransform> m_ImageViewTransform;

   ossimIrect               m_inputR0Rect;
   ossimIrect               m_viewRect;
   bool                     m_rectsDirty;

   ossim_uint32             m_MaxRecursionLevel;
   bool                     m_AutoUpdateInputTransform;
   ossim_uint32             m_MaxLevelsToCompute;
   
TYPE_DATA
};

inline ossimImageRenderer::ossimRendererSubRectInfo::ossimRendererSubRectInfo()
{
   m_Vul.makeNan();
   m_Vur.makeNan();
   m_Vlr.makeNan();
   m_Vll.makeNan();
   m_Iul.makeNan();
   m_Iur.makeNan();
   m_Ilr.makeNan();
   m_Ill.makeNan();
   m_ViewToImageScale.makeNan();
   m_ImageToViewScale.makeNan();            
}

inline ossimImageRenderer::ossimRendererSubRectInfo::ossimRendererSubRectInfo(const ossimDpt& vul,
                         const ossimDpt& vur,
                         const ossimDpt& vlr,
                         const ossimDpt& vll)
                         :m_Vul(vul),
                         m_Vur(vur),
                         m_Vlr(vlr),
                         m_Vll(vll)
{
   m_Iul.makeNan();
   m_Iur.makeNan();
   m_Ilr.makeNan();
   m_Ill.makeNan();
   m_ViewToImageScale.makeNan();
   m_ImageToViewScale.makeNan();            
}

inline bool ossimImageRenderer::ossimRendererSubRectInfo::imageHasNans()const
{
   return ( m_Iul.hasNans()||
      m_Iur.hasNans()||
      m_Ilr.hasNans()||
      m_Ill.hasNans());
}

inline bool ossimImageRenderer::ossimRendererSubRectInfo::imageIsNan()const
{
   return ( m_Iul.hasNans()&&
      m_Iur.hasNans()&&
      m_Ilr.hasNans()&&
      m_Ill.hasNans());
}

inline bool ossimImageRenderer::ossimRendererSubRectInfo::viewHasNans()const
{
   return ( m_Vul.hasNans()||
      m_Vur.hasNans()||
      m_Vlr.hasNans()||
      m_Vll.hasNans());
}

inline bool ossimImageRenderer::ossimRendererSubRectInfo::viewIsNan()const
{
   return ( m_Vul.hasNans()&&
      m_Vur.hasNans()&&
      m_Vlr.hasNans()&&
      m_Vll.hasNans());
}

inline void ossimImageRenderer::ossimRendererSubRectInfo::roundToInteger()
{
   m_Iul = ossimIpt(m_Iul);
   m_Iur = ossimIpt(m_Iur);
   m_Ilr = ossimIpt(m_Ilr);
   m_Ill = ossimIpt(m_Ill);

   m_Vul = ossimIpt(m_Vul);
   m_Vur = ossimIpt(m_Vur);
   m_Vlr = ossimIpt(m_Vlr);
   m_Vll = ossimIpt(m_Vll);
}

inline ossimDrect ossimImageRenderer::ossimRendererSubRectInfo::getViewRect()const
{
   return ossimDrect(m_Vul,
      m_Vur,
      m_Vlr,
      m_Vll);            
}

inline ossimDrect ossimImageRenderer::ossimRendererSubRectInfo::getImageRect()const
{
   return ossimDrect(m_Iul,
      m_Iur,
      m_Ilr,
      m_Ill);
}

inline void ossimImageRenderer::ossimRendererSubRectInfo::roundImageToInteger()
{
   m_Iul = ossimIpt(m_Iul);
   m_Iur = ossimIpt(m_Iur);
   m_Ilr = ossimIpt(m_Ilr);
   m_Ill = ossimIpt(m_Ill);
}

inline void ossimImageRenderer::ossimRendererSubRectInfo::roundViewToInteger()
{
   m_Vul = ossimIpt(m_Vul);
   m_Vur = ossimIpt(m_Vur);
   m_Vlr = ossimIpt(m_Vlr);
   m_Vll = ossimIpt(m_Vll);
}

inline bool ossimImageRenderer::ossimRendererSubRectInfo::isViewEqual(const ossimRendererSubRectInfo& infoRect)const
{
   return ( (m_Vul == infoRect.m_Vul)&&
      (m_Vur == infoRect.m_Vur)&&
      (m_Vlr == infoRect.m_Vlr)&&
      (m_Vll == infoRect.m_Vll));
}

inline bool ossimImageRenderer::ossimRendererSubRectInfo::isViewEqual(const ossimDrect& viewRect)const
{
   return ( (m_Vul == viewRect.ul())&&
      (m_Vur == viewRect.ur())&&
      (m_Vlr == viewRect.lr())&&
      (m_Vll == viewRect.ll()));
}

inline ossimDpt ossimImageRenderer::ossimRendererSubRectInfo::getAbsValueViewToImageScales()const
{
   if(m_ViewToImageScale.hasNans())
   {
      return m_ImageToViewScale;
   }
   return ossimDpt(fabs(m_ViewToImageScale.x), fabs(m_ViewToImageScale.y));
}

inline ossimDpt ossimImageRenderer::ossimRendererSubRectInfo::getAbsValueImageToViewScales()const
{
   if(m_ImageToViewScale.hasNans())
   {
      return m_ImageToViewScale;
   }

   return ossimDpt(fabs(m_ImageToViewScale.x), fabs(m_ImageToViewScale.y));
}

inline bool ossimImageRenderer::ossimRendererSubRectInfo::isViewAPoint()const
{
   return ((m_Vul == m_Vur)&&
      (m_Vul == m_Vlr)&&
      (m_Vul == m_Vll));
}

#endif
