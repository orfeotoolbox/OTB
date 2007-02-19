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
#include "itkCovarianceCalculator.h"
#include "itkMacro.h"
#include <FL/Fl_Output.H>


namespace otb
{
  /** 
   *
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
  typedef typename ScrollWidgetType::VectorPixelType VectorPixelType;
  typedef typename ScrollWidgetType::Pointer ScrollWidgetPointerType;
  typedef typename ZoomWidgetType::Pointer ZoomWidgetPointerType;
  typedef typename FullWidgetType::Pointer FullWidgetPointerType;


  typedef itk::Vector<InputPixelType,3> MeasurementVectorType;
  typedef itk::Statistics::ListSample<VectorPixelType> ListSampleType;
  typedef itk::Statistics::CovarianceCalculator<ListSampleType> CovarianceCalculatorType;

  typedef otb::StreamingShrinkImageFilter<ImageType,ImageType> ShrinkFilterType;
  typedef typename ShrinkFilterType::Pointer ShrinkFilterPointerType;

  typedef otb::ImageWidgetBoxForm BoxType;
  typedef typename BoxType::Pointer BoxPointerType;
  typedef typename BoxType::ColorType ColorType;
  
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
 

   /** Set the input image */
  virtual void SetImage(itk::ImageBase<2> * img);
  /** Show the viewer (Update) */
  virtual void Show(void);

  virtual void ComputeNormalizationFactors(void);

  void Build(void);
  virtual void Update(void);

  virtual void UpdateFullWidget(void);
  virtual void UpdateScrollWidget(void);
  virtual void UpdateZoomWidget(void);

  virtual void ChangeZoomViewedRegion(IndexType clickedIndex);
  virtual void ChangeFullViewedRegion(IndexType clickedIndex);
  virtual RegionType ComputeConstrainedRegion(RegionType smallRegion, RegionType bigRegion);

  virtual void PrintPixLocVal(std::string str);

protected:
  // Constructor and destructor
  ImageViewer();
  ~ImageViewer();
 
  /** Hide all Image View Windows */
  virtual void Hide(void);
 		
 private:
  /// zoom widget component
  ZoomWidgetPointerType m_ZoomWidget;
  /// scroll widget component
  ScrollWidgetPointerType m_ScrollWidget;
  /// full widget component
  FullWidgetPointerType m_FullWidget;
  /// The image to view
  ImagePointerType m_InputImage;
  bool m_UseScroll;
  unsigned int m_ScrollMaxInitialSize;
  unsigned int m_FullMaxInitialSize;
  unsigned m_ZoomMaxInitialSize;
  double m_ImageGeometry;
  unsigned int m_ScrollLimitSize;
  Fl_Window * m_FullWindow;
  Fl_Window * m_ScrollWindow;
  Fl_Window * m_ZoomWindow;
  Fl_Window * m_PixLocWindow;
  Fl_Output * m_PixLocOutput;

  ShrinkFilterPointerType m_Shrink;
  unsigned int m_ShrinkFactor;
  ColorType m_Color;
  bool m_Built;

  VectorPixelType m_MinComponentValue;
  VectorPixelType m_MaxComponentValue;

  unsigned int m_RedChannelIndex;
  unsigned int m_GreenChannelIndex;
  unsigned int m_BlueChannelIndex;
  const char *  m_Label;
};


} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageViewer.txx"
#endif

#endif
