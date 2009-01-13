/*=========================================================================

Program:   ORFEO Toolbox
Language:  C++
Date:      $Date$
Version:   $Revision$


Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
See OTBCopyright.txt for details.


This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbImageAlternateViewer_h
#define __otbImageAlternateViewer_h

#include "FL/Fl_Gl_Window.H"
#include "itkObject.h"
#include "otbImageList.h"
#include "itkInterpolateImageFunction.h"
#include "itkLinearInterpolateImageFunction.h"
#include "itkVectorImageToImageAdaptor.h"
#include "otbVectorImageToImageListFilter.h"
#include "otbImage.h"

#include <FL/gl.h>
#include "itkImageRegionSplitter.h"


namespace otb
{
/** \class ImageAlternateViewer
 *  \brief
 *
 * \ingroup Visu
 */
template <class TPixel>
class ITK_EXPORT ImageAlternateViewer
  : public Fl_Gl_Window, public itk::Object
{
 public:
  /** Standard typedefs */
  typedef ImageAlternateViewer            Self;
  typedef itk::Object                     Superclass;
  typedef itk::SmartPointer<Self>         Pointer;
  typedef itk::SmartPointer<const Self>   ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(ImageAlternateViewer,Object);

  /** Template related typedef */
    typedef TPixel     PixelType;
    typedef otb::VectorImage<PixelType,2> ImageType;
    typedef otb::Image<PixelType,2> SingleImageType;
    typedef typename ImageType::PixelType VectorPixelType;
    typedef typename ImageType::Pointer ImagePointerType;
    typedef typename ImageType::SizeType SizeType;
    typedef typename ImageType::IndexType IndexType;
    typedef typename ImageType::RegionType RegionType;
    typedef typename ImageType::SpacingType SpacingType;
    typedef typename ImageType::PointType PointType;

    typedef otb::ImageList<SingleImageType> ImageListType;
    typedef otb::VectorImageToImageListFilter<ImageType,ImageListType> VectorImageDecompositionFilterType;
    typedef typename VectorImageDecompositionFilterType::Pointer VectorImageDecompositionFilterPointerType;

    typedef itk::InterpolateImageFunction<SingleImageType,double> InterpolatorType;
    typedef typename InterpolatorType::Pointer InterpolatorPointerType;
    typedef itk::LinearInterpolateImageFunction<SingleImageType,double> DefaultInterpolatorType;

    typedef itk::ImageRegionSplitter<2>  SplitterType;
    typedef typename SplitterType::Pointer SplitterPointerType;

    itkSetMacro(RedChannelIndex,unsigned int);
    itkGetMacro(RedChannelIndex,unsigned int);

    itkSetMacro(GreenChannelIndex,unsigned int);
    itkGetMacro(GreenChannelIndex,unsigned int);

    itkSetMacro(BlueChannelIndex,unsigned int);
    itkGetMacro(BlueChannelIndex,unsigned int);

    itkSetMacro(MaxComponentValues,VectorPixelType);
    itkGetMacro(MaxComponentValues,VectorPixelType);

    itkSetMacro(MinComponentValues,VectorPixelType);
    itkGetMacro(MinComponentValues,VectorPixelType);

    itkSetMacro(SpacingZoomFactor,double);
    itkGetMacro(SpacingZoomFactor,double);

    itkSetMacro(OpenGlIsotropicZoom,double);
    itkGetMacro(OpenGlIsotropicZoom,double);

    void SetViewedRegionCenter(const IndexType & index);
    itkGetMacro(ViewedRegionCenter,IndexType);

    itkSetObjectMacro(Image,ImageType);
    itkGetObjectMacro(Image,ImageType);

    itkSetObjectMacro(SecondImage,ImageType);
    itkGetObjectMacro(SecondImage,ImageType);


    itkSetMacro(DisplayExtent,RegionType);
    itkGetMacro(DisplayExtent,RegionType);

    itkGetObjectMacro(ZoomInInterpolator,InterpolatorType);
    itkSetObjectMacro(ZoomInInterpolator,InterpolatorType);
    itkGetObjectMacro(ZoomOutInterpolator,InterpolatorType);
    itkSetObjectMacro(ZoomOutInterpolator,InterpolatorType);

    /**
     * Set view mode to RGB.
     */
    void SetViewModelToRGB(void);
    /**
     * Set view mode to Grayscale.
     */
    void SetViewModelToGrayscale(void);

    /**
     * Set insight view model to RGB
     */
    void SetInsightViewModelToRGB(void);

    /**
     * Set insight view model to grayscale
     */
    void SetInsightViewModelToGrayscale(void);

    /** Normalization function */
    unsigned char Normalize(PixelType value, unsigned int channelIndex);

    void Show(void);

    void Reset(void);

    /** Initialize the widget */
    virtual void Init(int x, int y, int w, int h, const char * l);

  /** Resize the widget */
  virtual void resize(int x, int y, int w, int h);

protected:
  /** Constructor */
  ImageAlternateViewer();
  /** Destructor */
  virtual ~ImageAlternateViewer();

  /** Draw the widget */
  virtual void draw(void);

  /** Partial draw routine */
  virtual void Draw(unsigned char * buffer,RegionType& region);

  /** Reset opengl context */
  virtual void ResetOpenGlContext(void);

  /** Handle */
  virtual int  handle(int event);

 /*  /\** Rebuild opengl buffer *\/ */
/*   virtual void RebuildOpenGlBuffer(void); */



  virtual void IncrementalOpenGlBufferUpdate(void);

  RegionType GetAdditionalBufferRegion(unsigned int i);

  unsigned char * CreateAdditionalBuffer(RegionType region,ImagePointerType image,bool rgb);

  virtual void MergeBuffersAndFreeMemory(std::vector<unsigned char *> bufferList,
           std::vector<RegionType> bufferRegionList);

  virtual void AdditionalRedraw(void);

  virtual void DecorationRedraw(void);

  virtual void DrawRegionBoundary(RegionType& region);

  virtual long IndexInOldGrid(PointType point, PointType oldUpperLeft, SpacingType spacing, SizeType size);

  RegionType ComputeRequestedRegion(RegionType &region);
 /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  ImageAlternateViewer(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** Pointer to the viewed image */
  ImagePointerType m_Image;
  /** OpenGl Buffer */
  unsigned char * m_OpenGlBuffer;
  /** OpenGl Zoom factor */
  double m_OpenGlIsotropicZoom;
  /** Flag for RGB/ GRAYSCALE view mode */
  bool m_ViewModelIsRGB;
  bool m_InsightViewModelIsRGB;
  /** Red channel index */
  unsigned int m_RedChannelIndex;
  /** Green channel index */
  unsigned int m_GreenChannelIndex;
  /** Blue channel index */
  unsigned int m_BlueChannelIndex;
  /** Max value for normalization */
  VectorPixelType m_MaxComponentValues;
  /** Min value for normalization */
  VectorPixelType m_MinComponentValues;
  /** Viewed region center */
  IndexType m_ViewedRegionCenter;
  /** Old Viewed region center */
  IndexType m_OldViewedRegionCenter;
  /** Spacing zoom factor */
  double m_SpacingZoomFactor;
  /** Display extent */
  RegionType m_DisplayExtent;
  /** Zoom in interpolator */
  InterpolatorPointerType m_ZoomInInterpolator;
  /** Zoom out interpolator */
  InterpolatorPointerType m_ZoomOutInterpolator;
  /** Zoom step when wheel mouse is triggered */
  double m_ZoomStep;
  /** Zoom state (helps computing spacing zoom value */
  int m_ZoomState;
  /** Decomposition filter */
  VectorImageDecompositionFilterPointerType m_DecompositionFilter;
  /** Region requested for decomposition */
  RegionType m_RequestedRegion;

  /** buffered region */
  RegionType m_BufferedRegion;

  /** buffered region */
  RegionType m_OldDisplayExtent;

  /** old spacing for buffer */
  double m_OldSpacingZoomFactor;

  /** old buffered region */
  RegionType m_OldBufferedRegion;

  std::vector<unsigned char *> m_BufferList;
  std::vector<RegionType> m_BufferRegionList;

  /** Prevent messing around with simultaneous update */
  bool m_Updating;
  /** Region splitter */
  SplitterPointerType m_Splitter;
  /** Swith the drag mode */
  bool m_Drag;
  /** Count drag events */
  unsigned int m_DragEventCounter;
  /** Remember the mouse pos in drag mode */
  IndexType m_OldMousePos;

  /** The second image */
  ImagePointerType m_SecondImage;
  /** The subwindow region */
  RegionType m_SubWindowRegion;
  /** subwindow mode switch */
  bool m_SubWindowMode;
/** subwindow mode switch */
  bool m_SubWindowMove;

};
}// End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageAlternateViewer.txx"
#endif

#endif
