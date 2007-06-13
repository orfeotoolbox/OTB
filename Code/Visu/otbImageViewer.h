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
#ifndef __otbImageViewer_h
#define __otbImageViewer_h

#include <string>
#include "itkObject.h"
#include "otbImageViewerScrollWidget.h"
#include "otbImageViewerZoomWidget.h"
#include "otbImageViewerFullWidget.h"
#include "otbStreamingShrinkImageFilter.h"
#include "otbImageWidgetBoxForm.h"
#include "itkListSample.h"
#include "otbObjectList.h"
#include "itkCovarianceCalculator.h"
#include "itkMacro.h"
#include <FL/Fl_Output.H>
#include "otbImage.h"
#include "otbImageToVectorImageCastFilter.h"


namespace otb
{
  /** 
   * \brief Simple image viewer.
   *
   * This class is a simple image viewer using fltk gui and opengl drawing.
   * It is divided into four parts :
   * - The full window that display a part of the image at full resolution,
   * - If necessary, a scroll window to allow the user to navigate through the whole image,
   * - A zoom window whose zoom can be adjusted with mouse scroll,
   * - a Pixel/value window to display information about the pointed pixel.
   *
   * The viewer uses the StreamingShrinkImageFilter to generate a quicklook of possibly very large image
   * without flooding the memory.
   *
   * Channel index and other various parameters can be set.
   * 
   * To use the viewer, setup it with all your parameters, then call the Build(), Show() and the Update() methods.
   * A call to Fl::run() is mandatory to hang the program execution until the viewer is closed.
   *
   * Unkike most OTB filters, ImageViewer is templated over the input pixel type and will take as input an otb::VectorImage<PixelType>.
   * Be aware of that.
   *
   * \sa ImageViewerFullWidget, ImageViewerScrollWidget, ImageViewerZoomWidget
   */
template <class TPixel>
class ITK_EXPORT ImageViewer 
  : public itk::ProcessObject            
{
 public:
  /** Standard class typedefs */
  typedef ImageViewer    Self;
  typedef itk::ProcessObject                 Superclass;
  typedef itk::SmartPointer<Self>            Pointer;
  typedef itk::SmartPointer<const Self>      ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageViewer,itk::ProcessObject);
  
  /// Template pixel type
  typedef TPixel InputPixelType;
  typedef otb::ImageViewerScrollWidget<InputPixelType> ScrollWidgetType;
  typedef otb::ImageViewerZoomWidget<InputPixelType> ZoomWidgetType;
  typedef otb::ImageViewerFullWidget<InputPixelType> FullWidgetType;

  typedef typename ScrollWidgetType::ImageType ImageType;
  typedef typename ImageType::Pointer ImagePointerType;
  typedef typename ImageType::IndexType IndexType;
  typedef typename ImageType::SizeType SizeType;
  typedef typename ImageType::PixelType PixelType;
  typedef typename ImageType::RegionType RegionType;
  typedef typename ImageType::OffsetType OffsetType;
  typedef typename ScrollWidgetType::VectorPixelType VectorPixelType;
  typedef typename ScrollWidgetType::Pointer ScrollWidgetPointerType;
  typedef typename ZoomWidgetType::Pointer ZoomWidgetPointerType;
  typedef typename FullWidgetType::Pointer FullWidgetPointerType;
  itkStaticConstMacro(ImageDimension,unsigned int,ImageType::ImageDimension);

  /// Support for conversion from otb::Image to otb::VectorImage
  typedef typename PixelType::ValueType ValueType;
  typedef otb::Image<ValueType,ImageDimension> SingleImageType;
  typedef typename SingleImageType::Pointer SingleImagePointerType;
  typedef otb::ImageToVectorImageCastFilter<SingleImageType,ImageType> VectorCastFilterType;
  typedef typename VectorCastFilterType::Pointer VectorCastFilterPointerType;

  /// Input image types
  typedef itk::Vector<InputPixelType,3> MeasurementVectorType;
  typedef itk::Statistics::ListSample<VectorPixelType> ListSampleType;
  typedef itk::Statistics::CovarianceCalculator<ListSampleType> CovarianceCalculatorType;

  /// Definition of streaming shrink image filter
  typedef otb::StreamingShrinkImageFilter<ImageType,ImageType> ShrinkFilterType;
  typedef typename ShrinkFilterType::Pointer ShrinkFilterPointerType;

  /// Definition of box form to represent displayed regions.
  typedef otb::ImageWidgetBoxForm BoxType;
  typedef typename BoxType::Pointer BoxPointerType;
  typedef typename BoxType::ColorType ColorType;
  
  /// List of linked viewer typedef
  typedef otb::ObjectList<Self> ViewerListType;
  typedef typename ViewerListType::Pointer ViewerListPointerType;

  /// Linked viewer offset list
  typedef std::vector<OffsetType> OffsetListType;

  /// Accessors
  itkGetMacro(Built,bool);
  itkGetMacro(ShrinkFactor,unsigned int);
  itkSetMacro(RedChannelIndex,unsigned int);
  itkGetMacro(RedChannelIndex,unsigned int);
  itkSetMacro(GreenChannelIndex,unsigned int);
  itkGetMacro(GreenChannelIndex,unsigned int);
  itkSetMacro(BlueChannelIndex,unsigned int);
  itkGetMacro(BlueChannelIndex,unsigned int);
  itkSetMacro(Label,char *);
  itkGetMacro(Label,const char *);
  itkSetMacro(QuicklookRatioCoef,double);
  itkGetMacro(QuicklookRatioCoef,double);
  itkSetMacro(NormalizationFactor,double);
  itkGetMacro(NormalizationFactor,double);
  itkGetMacro(Updating,bool);
 
  /** Set the input image (VectorImage version) */
  virtual void SetImage(ImageType * img);

  /** Set the input image (Image version) */
  virtual void SetImage(SingleImageType * img);
  
  /** Show the viewer (Update) */
  virtual void Show(void);
   /** Hide all Image View Windows */
  virtual void Hide(void);

  /** Compute the normalization factors */
  virtual void ComputeNormalizationFactors(void);

  /** Build the interfaces */
  void Build(void);
  
  /** Update the widgets */
  virtual void Update(void);

  /** Update the full widget */
  virtual void UpdateFullWidget(void);
  
  /** Update the scroll widget */
  virtual void UpdateScrollWidget(void);

  /** Update the ZoomWidget */
  virtual void UpdateZoomWidget(void);

  /** Change the ZoomViewedRegion 
   * \param clickedIndex The new center of the region
   **/
  virtual void ChangeZoomViewedRegion(IndexType clickedIndex);
  /** 
   * Change the Full Viewed region 
   * \param clickedIndex The new center of the region
   */
  virtual void ChangeFullViewedRegion(IndexType clickedIndex);

  /** Compute the constrained region */
  virtual RegionType ComputeConstrainedRegion(RegionType smallRegion, RegionType bigRegion);

  /** Display pixel information */
  virtual void PrintPixLocVal(IndexType index, PixelType pixel);

  /** Clear pixel information */
  virtual void ClearPixLocVal(void);

  /** This is a helper class that performs a Show() and Fl::run() in order to ease 
   *  the use of the class for example in wrappings.
   * \return The return code from fltk.
   */
  int FlRun(void);  
  /**
   * Link this viewer with the given viewer.
   * \param viewer The viewer to link with.
   */
  virtual void Link(Self * viewer);
  /**
   * Link this viewer with the given viewer.
   * \param viewer The viewer to link with.
   * \param offset Offset between two viewers
   */
  virtual void Link(Self * viewer, OffsetType offset);

  /**
   * Unlink this viewer with the given viewer.
   * \param viewer The viewer to link with
   */
  virtual void Unlink(Self * viewer);

  /**
   * Clear all the links of the current viewer.
   */
  virtual void ClearLinks(void);

protected:

   /**
   * Link this viewer with the given viewer.
   * \param viewer The viewer to link with
   * \param offset Offset between two viewers
   * \param backwardLinkFlag Link back to this viewer.
   */
  virtual void Link(Self * viewer, OffsetType offset, bool backwardLinkFlag);

  /**
   * Unlink this viewer with the given viewer.
   * \param viewer The viewer to link with.
   * \param backwardLinkFlag Link back to this viewer.
   */
  virtual void Unlink(Self * viewer,bool backwardLinkFlag);



  // Constructor and destructor
  ImageViewer();
  ~ImageViewer();
 
 private:
  ImageViewer(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /// zoom widget component
  ZoomWidgetPointerType m_ZoomWidget;
  /// scroll widget component
  ScrollWidgetPointerType m_ScrollWidget;
  /// full widget component
  FullWidgetPointerType m_FullWidget;
  /// The image to view
  ImagePointerType m_InputImage;
  /// true if scroll widget is used
  bool m_UseScroll;
  /// Intial sizes
  unsigned int m_ScrollMaxInitialSize;
  unsigned int m_FullMaxInitialSize;
  unsigned m_ZoomMaxInitialSize;
  /// Image geomerty ratio
  double m_ImageGeometry;
  /// Limit size for the scroll view
  unsigned int m_ScrollLimitSize;
  /// Fltk surrounding windows
  Fl_Window * m_FullWindow;
  Fl_Window * m_ScrollWindow;
  Fl_Window * m_ZoomWindow;
  Fl_Window * m_PixLocWindow;
  Fl_Output * m_PixLocOutput;
  
  /// Pointer to the shrink filter
  ShrinkFilterPointerType m_Shrink;
  /// The shrink factor 
  unsigned int m_ShrinkFactor;
  /// Color of the boxes
  ColorType m_Color;
  /// true if the Gui has been built.
  bool m_Built;

  /// Max and min values for normalization 
  VectorPixelType m_MinComponentValue;
  VectorPixelType m_MaxComponentValue;

  /// Channel indices
  unsigned int m_RedChannelIndex;
  unsigned int m_GreenChannelIndex;
  unsigned int m_BlueChannelIndex;
  
  /// Quicklook quality factor
  double       m_QuicklookRatioCoef;
  /// Normalization quality factor
  double       m_NormalizationFactor;
  /// Label to be shown in th full and scroll windows headers.
  const char *  m_Label;

  /// Converter from otb::Image to otb::VectorImage
  VectorCastFilterPointerType m_VectorCastFilter;

  /// Wether the viewer is updating or not
  bool m_Updating;

  /// The list of viewer with which this viewer is linked
  ViewerListPointerType m_LinkedViewerList;

  OffsetListType m_LinkedViewerOffsetList;
};


} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageViewer.txx"
#endif

#endif
