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
#ifndef __otbImageViewerBase_txx
#define __otbImageViewerBase_txx

#include "otbImageViewer.h"
#include "otbFltkFilterWatcher.h"
#include "otbMacro.h"

#include <sstream>
#include <iomanip>

namespace otb
{
/// Constructor
template <class TPixel, class TLabel>
ImageViewerBase<TPixel, TLabel>
::ImageViewerBase()
{
  m_UseScroll = false;
  m_ScrollMaxInitialSize = 300;
  m_FullMaxInitialSize = 600;
  m_ZoomMaxInitialSize = 200;
  m_ImageGeometry = 1.0;
  m_ScrollLimitSize = 600;
  m_InterfaceBoxesColor[0] = 1.0;
  m_InterfaceBoxesColor[1] = 0;
  m_InterfaceBoxesColor[2] = 0;
  m_DefaultROIColor[0] = 0;
  m_InterfaceBoxesColor[3] = 1.0;
  m_DefaultROIColor[1] = 0;
  m_DefaultROIColor[2] = 1.0;
  m_DefaultROIColor[3] = 0.5;
  m_NextROILabel = 0;
  m_ShrinkFactor = 1;
  m_RedChannelIndex = 0;
  m_GreenChannelIndex = 1;
  m_BlueChannelIndex = 2;
  m_Built = false;
  m_Shrink = ShrinkFilterType::New();
  m_PixLocOutput = NULL;
  m_NormalizationFactor = 0.02;
  m_QuicklookRatioCoef = 2;
  m_VectorCastFilter = NULL;
  m_LinkedViewerList = ViewerListType::New();
  m_Updating = false;
  m_PolygonROIList = PolygonListType::New();
  m_PathList = PathListType::New();
  m_InterfaceBoxesList = FormListType::New();
  m_ShowFullWidget = true;
  m_ShowScrollWidget = true;
  m_ShowZoomWidget = true;
  m_Label = "Default";
  m_RectangularROISelectionMode = false;
  m_PolygonalROISelectionMode = false;
  m_UseImageOverlay = false;
  m_HistogramGeneratorList = HistogramGeneratorListType::New();
  m_TransferFunctionList = TransferFunctionListType::New();
  m_RedHistogramWidget = HistogramWidgetType::New();
  m_GreenHistogramWidget = HistogramWidgetType::New();
  m_BlueHistogramWidget = HistogramWidgetType::New();
  m_ShowHistograms = true;
}

/// Destructor
template <class TPixel, class TLabel>
ImageViewerBase<TPixel, TLabel>
::~ImageViewerBase()
{}

/// Compute the normalization factor
template <class TPixel, class TLabel>
void
ImageViewerBase<TPixel, TLabel>
::ComputeNormalizationFactors(void)
{

  typename ListSampleListType::Pointer sl =  ListSampleListType::New();

  sl->Reserve(m_InputImage->GetNumberOfComponentsPerPixel());

  InputIteratorType it;
  // if scroll is activated, compute the factors from the quicklook
  if (m_UseScroll)
    {
    it = InputIteratorType(m_Shrink->GetOutput(), m_Shrink->GetOutput()->GetLargestPossibleRegion());
    it.GoToBegin();
    }
  // else, compute the factors from the full viewed region
  else
    {
    m_InputImage->SetRequestedRegion(m_FullWidget->GetViewedRegion());
    m_InputImage->PropagateRequestedRegion();
    m_InputImage->UpdateOutputData();
    it = InputIteratorType(m_InputImage, m_InputImage->GetRequestedRegion());
    it.GoToBegin();
    }

  if (this->GetViewModel() == ScrollWidgetType::COMPLEX_MODULUS)
    {
    sl->PushBack(ListSampleType::New());
    while (!it.IsAtEnd())
      {
      PixelType pixel = it.Get();
      double    re = static_cast<double>(pixel[m_RedChannelIndex]);
      double    im = static_cast<double>(pixel[m_GreenChannelIndex]);
      sl->GetNthElement(0)->PushBack(vcl_sqrt(static_cast<double>(re * re + im * im)));
      ++it;
      }
    }
  else if (this->GetViewModel() == ScrollWidgetType::COMPLEX_PHASE)
    {
    sl->PushBack(ListSampleType::New());
    while (!it.IsAtEnd())
      {
      PixelType pixel = it.Get();
      sl->GetNthElement(0)->PushBack(vcl_atan2(static_cast<double>(pixel[m_GreenChannelIndex]),
                                               static_cast<double>(pixel[m_RedChannelIndex])));
      ++it;
      }

    }
  else
    {
    for (unsigned int i = 0; i < m_InputImage->GetNumberOfComponentsPerPixel(); ++i)
      {
      sl->PushBack(ListSampleType::New());
      }
    while (!it.IsAtEnd())
      {
      PixelType pixel = it.Get();
      for (unsigned int i = 0; i < m_InputImage->GetNumberOfComponentsPerPixel(); ++i)
        {
        sl->GetNthElement(i)->PushBack(pixel[i]);
        }
      ++it;
      }
    }

  m_HistogramGeneratorList->Clear();
  m_TransferFunctionList->Clear();
  otbMsgDebugMacro(<< "Nb bands: " << m_InputImage->GetNumberOfComponentsPerPixel());

  for (unsigned int i = 0; i < sl->Size(); ++i)
    {
    typename HistogramGeneratorType::Pointer generator = HistogramGeneratorType::New();
    generator->SetInput(sl->GetNthElement(i));
    typename HistogramGeneratorType::HistogramType::SizeType size;
    //      size.Fill(static_cast<unsigned int>(vcl_ceil(1/m_NormalizationFactor)*10));
    size.SetSize(sl->GetNthElement(i)->GetMeasurementVectorSize());
    size.Fill(256);
    generator->SetHistogramSize(size);
    generator->Update();
    m_HistogramGeneratorList->PushBack(generator);
    m_TransferFunctionList->PushBack(AffineTransferFunctionType::New());
    double min = generator->GetOutput()->Quantile(0, m_NormalizationFactor);
    double max = generator->GetOutput()->Quantile(0, 1 - m_NormalizationFactor);
    m_TransferFunctionList->Back()->SetLowerBound(static_cast<InputPixelType>(min));
    m_TransferFunctionList->Back()->SetUpperBound(static_cast<InputPixelType>(max));
    }

  if (m_UseScroll)
    {
    m_ScrollWidget->SetTransferFunctionList(m_TransferFunctionList);
    }
  m_ZoomWidget->SetTransferFunctionList(m_TransferFunctionList);
  m_FullWidget->SetTransferFunctionList(m_TransferFunctionList);
}

/// Build the HMI
template <class TPixel, class TLabel>
void
ImageViewerBase<TPixel, TLabel>
::Build(void)
{
  otbMsgDebugMacro(<< "Entering build method");
  if (!m_InputImage)
    {
    itkExceptionMacro(<< "No input image !");
    }

  if (m_UseImageOverlay && !m_InputImageOverlay)
    {
    itkExceptionMacro(<< "UseImageOverlay option toggled, but no image overlay specified !");
    }

  if (m_UseImageOverlay
      && m_InputImage->GetLargestPossibleRegion().GetSize()
      != m_InputImageOverlay->GetLargestPossibleRegion().GetSize())
    {
    itkExceptionMacro(<< "Input image and input image overlay do not have the same size !");
    }

  std::ostringstream oss;
  // Get the image dimension
  typename ImageType::SizeType size = m_InputImage->GetLargestPossibleRegion().GetSize();
  m_ImageGeometry = static_cast<double>(size[0]) / static_cast<double>(size[1]);

  // initiate windows dimensions
  int wscroll = 200;
  int hscroll = 0;
  int wfull = (size[0] < m_FullMaxInitialSize ? size[0] : m_FullMaxInitialSize);
  int hfull = (size[1] < m_FullMaxInitialSize ? size[1] : m_FullMaxInitialSize);

  // decide wether to use scroll view or not
  if (size[0] < m_ScrollLimitSize && size[1] < m_ScrollLimitSize)
    {
    m_UseScroll = false;
    }
  else
    {
    m_UseScroll = true;
    // Compute scroll size :
    if (m_ImageGeometry < 1)
      {
      hscroll = m_ScrollMaxInitialSize;
      wscroll = static_cast<int>(static_cast<double>(m_ScrollMaxInitialSize) * m_ImageGeometry);
      }
    else
      {
      wscroll = m_ScrollMaxInitialSize;
      hscroll = static_cast<int>(static_cast<double>(m_ScrollMaxInitialSize) / m_ImageGeometry);
      }
    // Create the quicklook
    m_Shrink->SetInput(m_InputImage);
    if (size[0] / hscroll < size[1] / wscroll)
      {
      m_ShrinkFactor =
        static_cast<unsigned int>(vcl_ceil((static_cast<double>(size[0]) /
                                            static_cast<double>(wscroll)) / m_QuicklookRatioCoef));
      }
    else
      {
      m_ShrinkFactor =
        static_cast<unsigned int>(vcl_ceil((static_cast<double>(size[1]) /
                                            static_cast<double>(hscroll)) / m_QuicklookRatioCoef));
      }

    otbMsgDebugMacro(<< "Shrink factor: " << m_ShrinkFactor);
    m_Shrink->SetShrinkFactor(m_ShrinkFactor);
    typedef otb::FltkFilterWatcher WatcherType;
    WatcherType watcher(m_Shrink, wfull - 200, hfull / 2, 200, 20, "Generating Quicklook ...");
    m_Shrink->Update();
    }

  // Create full windows
  m_FullWidget = FullWidgetType::New();
  m_FullWidget->SetParent(this);
  m_FullWidget->SetInput(m_InputImage);
  if (m_UseImageOverlay)
    {
    m_FullWidget->SetInputOverlay(m_InputImageOverlay);
    m_FullWidget->SetImageOverlayVisible(true);
    }
  m_FullWidget->Init(0, 0, wfull, hfull, "");
  m_FullWidget->box(FL_EMBOSSED_BOX);
  m_FullWidget->SetFormOverlayVisible(true);

  // Create the zoom window
  m_ZoomWidget = ZoomWidgetType::New();
  m_ZoomWidget->SetParent(this);
  m_ZoomWidget->SetZoomFactor(4.0);
  m_ZoomWidget->SetInput(m_InputImage);
  if (m_UseImageOverlay)
    {
    m_ZoomWidget->SetInputOverlay(m_InputImageOverlay);
    m_ZoomWidget->SetImageOverlayVisible(true);
    }
  m_ZoomWidget->Init(0, 0, m_ZoomMaxInitialSize, m_ZoomMaxInitialSize, "");
  m_ZoomWidget->box(FL_EMBOSSED_BOX);
  m_ZoomWidget->SetFormOverlayVisible(true);

  // Create the zoom selection mode
  if (m_ShowZoomWidget)
    {
    BoxPointerType zoomBox = BoxType::New();
    SizeType       zoomBoxSize;
    IndexType      zoomBoxIndex;
    zoomBoxSize[0] = (m_ZoomWidget->GetViewedRegion().GetSize()[0] + 2);
    zoomBoxSize[1] = (m_ZoomWidget->GetViewedRegion().GetSize()[1] + 2);
    zoomBoxIndex[0] = (m_ZoomWidget->GetViewedRegion().GetIndex()[0] - 1);
    zoomBoxIndex[1] = (m_ZoomWidget->GetViewedRegion().GetIndex()[1] - 1);
    zoomBox->SetIndex(zoomBoxIndex);
    zoomBox->SetSize(zoomBoxSize);
    zoomBox->SetColor(m_InterfaceBoxesColor);
    m_InterfaceBoxesList->PushBack(zoomBox);
    }

  if (m_UseScroll)
    {
    // Create the scroll window
    m_ScrollWidget = ScrollWidgetType::New();
    m_ScrollWidget->SetInput(m_Shrink->GetOutput());
    m_ScrollWidget->SetParent(this);
    m_ScrollWidget->Init(0, 0, wscroll, hscroll, oss.str().c_str());
    m_ScrollWidget->box(FL_EMBOSSED_BOX);
    m_ScrollWidget->SetFormOverlayVisible(true);
    m_ScrollWidget->SetSubSamplingRate(m_ShrinkFactor);

    if (m_UseImageOverlay)
      {
      m_ShrinkOverlay = OverlayShrinkFilterType::New();
      m_ShrinkOverlay->SetInput(m_InputImageOverlay);
      m_ShrinkOverlay->SetShrinkFactor(m_ShrinkFactor);
      typedef otb::FltkFilterWatcher WatcherType;
      WatcherType watcher(m_ShrinkOverlay, wfull - 200, hfull / 2, 200, 20, "Generating Overlay Quicklook ...");
      m_ShrinkOverlay->Update();
      m_ScrollWidget->SetInputOverlay(m_ShrinkOverlay->GetOutput());
      m_ScrollWidget->SetImageOverlayVisible(true);
      }

    // Create the scroll selection box
    BoxPointerType box = BoxType::New();
    SizeType       scrollBoxSize;
    IndexType      scrollBoxIndex;
    scrollBoxSize[0] = (m_FullWidget->GetViewedRegion().GetSize()[0] + 2);
    scrollBoxSize[1] = (m_FullWidget->GetViewedRegion().GetSize()[1] + 2);
    scrollBoxIndex[0] = (m_FullWidget->GetViewedRegion().GetIndex()[0] - 1);
    scrollBoxIndex[1] = (m_FullWidget->GetViewedRegion().GetIndex()[1] - 1);
    otbMsgDebugMacro(<< "Scroll box: " << scrollBoxIndex << " " << scrollBoxSize);
    box->SetSize(scrollBoxSize);
    box->SetIndex(scrollBoxIndex);
    box->SetColor(m_InterfaceBoxesColor);
    m_InterfaceBoxesList->PushBack(box);
    }

  InitializeViewModel();

  // Compute the normalization factors
  ComputeNormalizationFactors();

  m_RedHistogramWidget->SetParent(this);
  m_BlueHistogramWidget->SetParent(this);
  m_GreenHistogramWidget->SetParent(this);

  m_RedHistogramWidget->resize(0, 0, m_FullMaxInitialSize / 2, m_ZoomMaxInitialSize);
  m_BlueHistogramWidget->resize(0, 0, m_FullMaxInitialSize / 2, m_ZoomMaxInitialSize);
  m_GreenHistogramWidget->resize(0, 0, m_FullMaxInitialSize / 2, m_ZoomMaxInitialSize);

  GenerateOverlayList();

  m_Built = true;
}
/// Set the image (VectorImage version)
template <class TPixel, class TLabel>
void
ImageViewerBase<TPixel, TLabel>
::SetImage(ImageType * img)
{
  m_InputImage = dynamic_cast<ImageType *>(img);
}
/// Set the image overlay (VectorImage version)
template <class TPixel, class TLabel>
void
ImageViewerBase<TPixel, TLabel>
::SetImageOverlay(OverlayImageType * img)
{
  m_InputImageOverlay = dynamic_cast<OverlayImageType *>(img);
}

template <class TPixel, class TLabel>
typename ImageViewerBase<TPixel, TLabel>
::ImageType *
ImageViewerBase<TPixel, TLabel>
::GetShrinkedImage(void)
{
  if (m_UseScroll)
    {
    return m_Shrink->GetOutput();
    }
  else
    {
    return m_InputImage;
    }
}
/// Set the image (Image version)
template <class TPixel, class TLabel>
void
ImageViewerBase<TPixel, TLabel>
::SetImage(SingleImageType * img)
{
  m_VectorCastFilter = VectorCastFilterType::New();
  m_VectorCastFilter->SetInput(img);
  m_VectorCastFilter->UpdateOutputInformation();
  m_InputImage = m_VectorCastFilter->GetOutput();
}

/// Show the app
template <class TPixel, class TLabel>
void
ImageViewerBase<TPixel, TLabel>
::Show(void)
{
  //comment: std::cout<<"Entering show method"<<std::endl;
  if (!m_Built)
    {
    this->Build();
    }
  Fl::check();
  if (m_UseScroll && m_ShowScrollWidget)
    {
    m_ScrollWidget->Show();
    }

  if (m_ShowFullWidget)
    {
    m_FullWidget->Show();
    }

  if (m_ShowZoomWidget)
    {
    m_ZoomWidget->Show();
    }
  if (m_ShowHistograms)
    {
    m_RedHistogramWidget->show();

    if (this->GetViewModel() == ScrollWidgetType::RGB)
      {
      m_GreenHistogramWidget->show();
      m_BlueHistogramWidget->show();
      }
    }

  Fl::check();
  //comment: std::cout<<"Leaving show method"<<std::endl;
}
/** This is a helper class that performs a Show() and Fl::run() in order to ease
    the use of the class for example in wrappings.*/
template <class TPixel, class TLabel>
int
ImageViewerBase<TPixel, TLabel>
::FlRun(void)
{
  this->Show();
  return Fl::run();
}

/// Hide the app
template <class TPixel, class TLabel>
void
ImageViewerBase<TPixel, TLabel>
::Hide(void)
{
  Fl::check();
  if (m_UseScroll)
    {
    m_ScrollWidget->hide();
    }
  m_FullWidget->hide();
  m_ZoomWidget->hide();

  m_RedHistogramWidget->hide();

  if (this->GetViewModel() == ScrollWidgetType::RGB)
    {
    m_GreenHistogramWidget->hide();
    m_BlueHistogramWidget->hide();
    }

  Fl::check();

}

template <class TPixel, class TLabel>
void
ImageViewerBase<TPixel, TLabel>
::GenerateOverlayList(void)
{
  FormListPointerType new_list = FormListType::New();
  if (m_ShowZoomWidget)
    {
    BoxPointerType zoomBox = BoxType::New();
    SizeType       zoomBoxSize;
    IndexType      zoomBoxIndex;
    zoomBoxSize[0] = (m_ZoomWidget->GetViewedRegion().GetSize()[0] + 2);
    zoomBoxSize[1] = (m_ZoomWidget->GetViewedRegion().GetSize()[1] + 2);
    zoomBoxIndex[0] = (m_ZoomWidget->GetViewedRegion().GetIndex()[0] - 1);
    zoomBoxIndex[1] = (m_ZoomWidget->GetViewedRegion().GetIndex()[1] - 1);
    zoomBox->SetIndex(zoomBoxIndex);
    zoomBox->SetSize(zoomBoxSize);
    zoomBox->SetColor(m_InterfaceBoxesColor);
    m_InterfaceBoxesList->SetNthElement(0, zoomBox);
    }

  if (m_UseScroll)
    {
    BoxPointerType box = BoxType::New();
    SizeType       scrollBoxSize;
    IndexType      scrollBoxIndex;
    scrollBoxSize[0] = (m_FullWidget->GetViewedRegion().GetSize()[0] + 2);
    scrollBoxSize[1] = (m_FullWidget->GetViewedRegion().GetSize()[1] + 2);
    scrollBoxIndex[0] = (m_FullWidget->GetViewedRegion().GetIndex()[0] - 1);
    scrollBoxIndex[1] = (m_FullWidget->GetViewedRegion().GetIndex()[1] - 1);
    box->SetSize(scrollBoxSize);
    box->SetIndex(scrollBoxIndex);
    box->SetColor(m_InterfaceBoxesColor);
    int idx;
    if (m_ShowZoomWidget)
      {
      idx = 1;
      }
    else
      {
      idx = 0;
      }
    m_InterfaceBoxesList->SetNthElement(idx, box);
    }

  for (FormListIteratorType it1 = m_InterfaceBoxesList->Begin();
       it1 != m_InterfaceBoxesList->End(); ++it1)
    {
    new_list->PushBack(it1.Get());
    }

  for (PolygonListIteratorType it2 = m_PolygonROIList->Begin();
       it2 != m_PolygonROIList->End(); ++it2)
    {
    ImageWidgetPolygonFormPointerType new_poly = ImageWidgetPolygonFormType::New();
    new_poly->SetPolygon(it2.Get());

    ColorType color;

    if (this->GetROIColorMapEntry(it2.Get()->GetValue(), color))
      {
      new_poly->SetColor(color);
      }
    else
      {
      new_poly->SetColor(m_DefaultROIColor);
      }

    for (PolygonIteratorType pIt = it2.Get()->GetVertexList()->Begin();
         pIt != it2.Get()->GetVertexList()->End(); ++pIt)
      {
      ImageWidgetCircleFormPointerType new_circle = ImageWidgetCircleFormType::New();

      new_circle->SetCenter(pIt.Value());
      new_circle->SetRadius(2);
      new_circle->SetSolid(false);
      new_circle->SetColor(m_InterfaceBoxesColor);
      new_list->PushBack(new_circle);

      }
    new_list->PushBack(new_poly);
    }

  for (PathListIteratorType it2 = m_PathList->Begin();
       it2 != m_PathList->End(); ++it2)
    {
    ImageWidgetPolylineFormPointerType new_line = ImageWidgetPolylineFormType::New();
    new_line->SetPolyline(it2.Get());
//           new_line->SetInternalValueToAlphaChannel(true);

    new_line->SetColor(m_DefaultROIColor);
    new_line->SetLineWidth(m_LineWidth);

    new_list->PushBack(new_line);
    }

  if (m_UseScroll)
    {
    m_ScrollWidget->SetFormListOverlay(new_list);
    }

  m_FullWidget->SetFormListOverlay(new_list);
  m_ZoomWidget->SetFormListOverlay(new_list);
}

/// Update the display
template <class TPixel, class TLabel>
void
ImageViewerBase<TPixel, TLabel>
::Update(void)
{
  m_Updating = true;
  //comment: std::cout<<"Update: "<<m_Label<<" Updating flag on"<<std::endl;
  GenerateOverlayList();
  Fl::check();
  UpdateScrollWidget();
  UpdateFullWidget();
  UpdateZoomWidget();

  // Update histogram widgets

  m_RedHistogramWidget->redraw();

  if (this->GetViewModel() == ScrollWidgetType::RGB)
    {
    m_GreenHistogramWidget->redraw();
    m_BlueHistogramWidget->redraw();
    }

  // update the linked viewer
  typename ViewerListType::Iterator linkedIt = m_LinkedViewerList->Begin();
  typename OffsetListType::iterator offIt = m_LinkedViewerOffsetList.begin();

  while (linkedIt != m_LinkedViewerList->End() && offIt != m_LinkedViewerOffsetList.end())
    {
    if (!linkedIt.Get()->GetUpdating() && linkedIt.Get()->GetBuilt())
      {
      linkedIt.Get()->Update();
      }
    ++linkedIt;
    ++offIt;
    }
  Fl::check();
  //comment: std::cout<<"Update: "<<m_Label<<" Updating flag off"<<std::endl;
  m_Updating = false;
}

template <class TPixel, class TLabel>
void
ImageViewerBase<TPixel, TLabel>
::ReportPixel(IndexType index)
{
  //comment: std::cout<<"Entering report pixel: "<<m_Label<<std::endl;
  if (m_PixLocOutput == NULL)
    {
    //comment: std::cout<<"PixLocOutput == NULL, returning ..."<<std::endl;
    return;
    }
  std::ostringstream oss;
  oss << index << " (image position)" << std::endl;
  PointType point;
  m_InputImage->TransformIndexToPhysicalPoint(index, point);
  oss << std::setprecision(15) << point << " (physical coordinates)" << std::endl;

  if (m_InputImage->GetBufferedRegion().IsInside(index))
    {
    //comment: std::cout<<"Index: "<<index<<std::endl;

    typename ImageType::PixelType newPixel = m_InputImage->GetPixel(index);

    if (this->GetViewModel() == ScrollWidgetType::RGB || this->GetViewModel() == ScrollWidgetType::GRAYSCALE)
      {
      oss << newPixel << " (" << m_Label << " pixel values)" << std::endl;
      }
    else if (this->GetViewModel() == ScrollWidgetType::COMPLEX_MODULUS)
      {
      double re = static_cast<double>(newPixel[m_RedChannelIndex]);
      double im = static_cast<double>(newPixel[m_GreenChannelIndex]);
      double modulus = vcl_sqrt(re * re + im * im);
      oss << modulus << std::setprecision(3) << " (" << m_Label << " modulus value)" << std::endl;

      }
    else if (this->GetViewModel() == ScrollWidgetType::COMPLEX_PHASE)
      {
      double re = static_cast<double>(newPixel[m_RedChannelIndex]);
      double im = static_cast<double>(newPixel[m_GreenChannelIndex]);
      double phase = vcl_atan2(im, re);
      oss << phase << std::setprecision(3) << " (" << m_Label << " phase value)" << std::endl;
      }
    }
  else
    {
    IndexType shrinkIndex;
    shrinkIndex[0] = index[0] / m_ShrinkFactor;
    shrinkIndex[1] = index[1] / m_ShrinkFactor;

    if (m_Shrink->GetOutput()->GetBufferedRegion().IsInside(shrinkIndex))
      {
      typename ImageType::PixelType newPixel = m_Shrink->GetOutput()->GetPixel(shrinkIndex);
      oss << newPixel << " (" << m_Label << " pixel values)" << std::endl;
      }
    }

  typename ViewerListType::Iterator linkedIt = m_LinkedViewerList->Begin();
  typename OffsetListType::iterator offIt = m_LinkedViewerOffsetList.begin();

  while (linkedIt != m_LinkedViewerList->End() && offIt != m_LinkedViewerOffsetList.end())
    {
    IndexType currentIndex = index + (*offIt);
    //comment: std::cout<<"CurrentIndex: "<<currentIndex<<std::endl;
    if (linkedIt.Get()->GetInputImage()->GetBufferedRegion().IsInside(currentIndex))
      {
      typename ImageType::PixelType newPixel = linkedIt.Get()->GetInputImage()->GetPixel(currentIndex);
      oss << newPixel << " (" << linkedIt.Get()->GetLabel() << " pixel values)" << std::endl;
      }
    else
      {
      IndexType shrinkIndex;
      shrinkIndex[0] = currentIndex[0] / m_ShrinkFactor;
      shrinkIndex[1] = currentIndex[1] / m_ShrinkFactor;

      if (linkedIt.Get()->GetShrinkedImage()->GetBufferedRegion().IsInside(shrinkIndex))
        {
        typename ImageType::PixelType newPixel = linkedIt.Get()->GetShrinkedImage()->GetPixel(shrinkIndex);
        oss << newPixel << " (" << linkedIt.Get()->GetLabel() << " pixel values)" << std::endl;
        }
      }
    ++linkedIt;
    ++offIt;
    }
  if (oss.good())
    {
    m_PixLocOutput->value(oss.str().c_str());
    m_PixLocOutput->redraw();
    Fl::check();
    }
  //comment: std::cout<<"Leaving report pixel: "<<m_Label<<std::endl;
}

template <class TPixel, class TLabel>
void
ImageViewerBase<TPixel, TLabel>
::ClearPixLocVal(void)
{
  if (m_PixLocOutput == NULL)
    {
    //comment: std::cout<<"PixLocOutput == NULL, returning ..."<<std::endl;
    return;
    }
  m_PixLocOutput->value("");
  m_PixLocOutput->redraw();
  Fl::check();
}

template <class TPixel, class TLabel>
void
ImageViewerBase<TPixel, TLabel>
::SetImageOverlayOpacity(unsigned char opacity)
{
  if (m_UseImageOverlay)
    {
    m_FullWidget->SetImageOverlayOpacity(opacity);
    m_ZoomWidget->SetImageOverlayOpacity(opacity);

    if (m_UseScroll)
      {
      m_ScrollWidget->SetImageOverlayOpacity(opacity);
      }
    }
}

template <class TPixel, class TLabel>
void
ImageViewerBase<TPixel, TLabel>
::UpdateZoomWidget(void)
{
  m_ZoomWidget->redraw();
}

template <class TPixel, class TLabel>
void
ImageViewerBase<TPixel, TLabel>
::UpdateFullWidget(void)
{
  m_FullWidget->redraw();
}

template <class TPixel, class TLabel>
void
ImageViewerBase<TPixel, TLabel>
::UpdateScrollWidget(void)
{
  if (m_UseScroll)
    {
    m_ScrollWidget->redraw();
    }
}

template <class TPixel, class TLabel>
typename ImageViewerBase<TPixel, TLabel>
::RegionType
ImageViewerBase<TPixel, TLabel>
::ComputeConstrainedRegion(RegionType smallRegion, RegionType bigRegion)
{
  // This function assumes that smallRegion is inside huge region
  if (smallRegion.GetSize()[0] > bigRegion.GetSize()[0]
      || smallRegion.GetSize()[1] > bigRegion.GetSize()[1])
    {
    itkExceptionMacro("Small region not inside big region !");
    }
  else
    {
    RegionType resp;
    IndexType  index = smallRegion.GetIndex();
    SizeType   size = smallRegion.GetSize();

    if (smallRegion.GetIndex()[0] < bigRegion.GetIndex()[0])
      {
      index[0] = bigRegion.GetIndex()[0];
      }
    if (smallRegion.GetIndex()[1] < bigRegion.GetIndex()[1])
      {
      index[1] = bigRegion.GetIndex()[1];
      }
    if (index[0] + size[0] >= bigRegion.GetIndex()[0] + bigRegion.GetSize()[0])
      {
      index[0] = bigRegion.GetIndex()[0] + bigRegion.GetSize()[0] - size[0];
      }
    if (index[1] + size[1] >= bigRegion.GetIndex()[1] + bigRegion.GetSize()[1])
      {
      index[1] = bigRegion.GetIndex()[1] + bigRegion.GetSize()[1] - size[1];
      }
    resp.SetSize(size);
    resp.SetIndex(index);
    return resp;
    }
}

template <class TPixel, class TLabel>
void
ImageViewerBase<TPixel, TLabel>
::ChangeFullViewedRegion(IndexType clickedIndex)
{
  m_Updating = true;
  //comment: std::cout<<"ChangeFullViewedRegion: "<<m_Label<<" Updating flag on"<<std::endl;
  RegionType region = m_FullWidget->GetViewedRegion();
  IndexType  newIndex;
  newIndex[0] = clickedIndex[0] - region.GetSize()[0] / 2;
  newIndex[1] = clickedIndex[1] - region.GetSize()[1] / 2;

  region.SetIndex(newIndex);

  RegionType newRegion = ComputeConstrainedRegion(region, m_InputImage->GetLargestPossibleRegion());
  m_FullWidget->SetUpperLeftCorner(newRegion.GetIndex());

  typename ViewerListType::Iterator linkedIt = m_LinkedViewerList->Begin();
  typename OffsetListType::iterator offIt = m_LinkedViewerOffsetList.begin();

  while (linkedIt != m_LinkedViewerList->End() && offIt != m_LinkedViewerOffsetList.end())
    {
    if (!linkedIt.Get()->GetUpdating())
      {
      IndexType linkedIndex;
      linkedIndex[0] = clickedIndex[0] + (*offIt)[0];
      linkedIndex[1] = clickedIndex[1] + (*offIt)[1];
      linkedIt.Get()->ChangeFullViewedRegion(linkedIndex);
      }
    ++offIt;
    ++linkedIt;
    }
//comment: std::cout<<"ChangeFullViewedRegion: "<<m_Label<<" Updating flag off"<<std::endl;
  m_Updating = false;
}

template <class TPixel, class TLabel>
void
ImageViewerBase<TPixel, TLabel>
::ChangeZoomViewedRegion(IndexType clickedIndex)
{

  m_Updating = true;
  //comment: std::cout<<"ChangeZoomViewedRegion: "<<m_Label<<" Updating flag on"<<std::endl;
  if (m_ShowZoomWidget)
    {
    RegionType region = m_ZoomWidget->GetViewedRegion();
    IndexType  newIndex;
    newIndex[0] = clickedIndex[0] - region.GetSize()[0] / 2;
    newIndex[1] = clickedIndex[1] - region.GetSize()[1] / 2;

    region.SetIndex(newIndex);
    RegionType newRegion = ComputeConstrainedRegion(region, m_FullWidget->GetViewedRegion());
    m_ZoomWidget->SetZoomUpperLeftCorner(newRegion.GetIndex());

    typename ViewerListType::Iterator linkedIt = m_LinkedViewerList->Begin();
    typename OffsetListType::iterator offIt = m_LinkedViewerOffsetList.begin();

    while (linkedIt != m_LinkedViewerList->End() && offIt != m_LinkedViewerOffsetList.end())
      {
      if (!linkedIt.Get()->GetUpdating())
        {
        IndexType linkedIndex;
        linkedIndex[0] = clickedIndex[0] + (*offIt)[0];
        linkedIndex[1] = clickedIndex[1] + (*offIt)[1];
        linkedIt.Get()->ChangeZoomViewedRegion(linkedIndex);
        }
      ++offIt;
      ++linkedIt;
      }
    }
  //comment: std::cout<<"ChangeZoomViewedRegion: "<<m_Label<<" Updating flag off"<<std::endl;
  m_Updating = false;
}

template <class TPixel, class TLabel>
void
ImageViewerBase<TPixel, TLabel>
::Link(Self * viewer, OffsetType offset, bool backwardLinkFlag)
{
  //comment: std::cout<<"Entering link: "<<viewer->GetLabel()<<" flag: "<<backwardLinkFlag<<std::endl;
  // Search if this viewer is already linked
  typename ViewerListType::Iterator it = m_LinkedViewerList->Begin();
  while (it != m_LinkedViewerList->End())
    {
    if (it.Get() == viewer)
      {
      otbMsgDebugMacro(<< "This viewer is already linked !");
      return;
      }
    ++it;
    }
  // If not, add it with its offset
  m_LinkedViewerList->PushBack(viewer);
  m_LinkedViewerOffsetList.push_back(offset);

  // If backward link flag is set, add the backward link
  if (backwardLinkFlag)
    {
    OffsetType invertOffset;
    invertOffset[0] = -offset[0];
    invertOffset[1] = -offset[1];
    viewer->Link(this, invertOffset, false);
    }
  //comment: std::cout<<"Leaving link: "<<viewer->GetLabel()<<" flag: "<<backwardLinkFlag<<std::endl;
}

template <class TPixel, class TLabel>
void
ImageViewerBase<TPixel, TLabel>
::Link(Self * viewer, OffsetType offset)
{
  this->Link(viewer, offset, true);
}

template <class TPixel, class TLabel>
void
ImageViewerBase<TPixel, TLabel>
::Link(Self * viewer)
{
  OffsetType offset;
  offset.Fill(0);
  this->Link(viewer, offset, true);
}

template <class TPixel, class TLabel>
void
ImageViewerBase<TPixel, TLabel>
::Unlink(Self * viewer, bool backwardLinkFlag)
{
  unsigned int counter = 0;
  bool         found = false;
  // Search the given viewer in the linked list
  typename ViewerListType::Iterator it = m_LinkedViewerList->Begin();
  while (!found && it != m_LinkedViewerList->End())
    {
    if (it.Get() == viewer)
      {
      found = true;
      }
    else
      {
      ++counter;
      }
    ++it;
    }

  // If found, erase
  m_LinkedViewerList->Erase(counter);
  m_LinkedViewerOffsetList.erase(m_LinkedViewerOffsetList.begin() + counter);

  // If backward link flag is set, remove the backward link
  if (backwardLinkFlag)
    {
    viewer->Unlink(this, false);
    }
}
template <class TPixel, class TLabel>
void
ImageViewerBase<TPixel, TLabel>
::Unlink(Self * viewer)
{
  this->Unlink(viewer, true);
}

template <class TPixel, class TLabel>
int
ImageViewerBase<TPixel, TLabel>
::IsLinkedTo(Self * viewer)
{
  int                               counter = 0;
  typename ViewerListType::Iterator it = m_LinkedViewerList->Begin();
  while (it != m_LinkedViewerList->End())
    {
    if (it.Get() == viewer)
      {
      return counter;
      }
    ++counter;
    ++it;
    }
  return -1;
}

template<class TPixel, class TLabel>
void
ImageViewerBase<TPixel, TLabel>
::ClearLinks(void)
{
  typename ViewerListType::Iterator it = m_LinkedViewerList->Begin();
  while (it != m_LinkedViewerList->End())
    {
    it.Get()->Unlink(this, false);
    ++it;
    }
  m_LinkedViewerList->Clear();
  m_LinkedViewerOffsetList.clear();
}

template<class TPixel, class TLabel>
typename ImageViewerBase<TPixel, TLabel>
::OffsetType
ImageViewerBase<TPixel, TLabel>
::GetOffset(int index)
{
  return m_LinkedViewerOffsetList[index];
}

template<class TPixel, class TLabel>
typename ImageViewerBase<TPixel, TLabel>
::ViewModelType
ImageViewerBase<TPixel, TLabel>
::GetViewModel()
{
  return m_FullWidget->GetViewModel();
}

template<class TPixel, class TLabel>
void
ImageViewerBase<TPixel, TLabel>
::SetViewModel(ViewModelType viewModel)
{
  // Channel indices outofbound check
  if (m_RedChannelIndex >= m_InputImage->GetNumberOfComponentsPerPixel())
    {
    itkExceptionMacro(<< "Red channel index out of bound.");
    }
  if (viewModel != ScrollWidgetType::GRAYSCALE && m_GreenChannelIndex >= m_InputImage->GetNumberOfComponentsPerPixel())
    {
    itkExceptionMacro(<< "Green channel index out of bound.");
    }
  if (viewModel == ScrollWidgetType::RGB && m_BlueChannelIndex >= m_InputImage->GetNumberOfComponentsPerPixel())
    {
    itkExceptionMacro(<< "Blue channel index out of bound.");
    }

  switch (viewModel)
    {
    case ScrollWidgetType::RGB:
      {
//       if(m_InputImage->GetNumberOfComponentsPerPixel()>2)
//   { //NOTE: No reason to prevent a 2 bands image to be displayed in RGB
//       and the image is not necessarily loaded at that time.
      if (m_UseScroll)
        {
        m_ScrollWidget->SetViewModel(viewModel);
        m_ScrollWidget->SetRedChannelIndex(m_RedChannelIndex);
        m_ScrollWidget->SetGreenChannelIndex(m_GreenChannelIndex);
        m_ScrollWidget->SetBlueChannelIndex(m_BlueChannelIndex);
        }
      m_FullWidget->SetViewModel(viewModel);
      m_ZoomWidget->SetViewModel(viewModel);
      m_ZoomWidget->SetRedChannelIndex(m_RedChannelIndex);
      m_ZoomWidget->SetGreenChannelIndex(m_GreenChannelIndex);
      m_ZoomWidget->SetBlueChannelIndex(m_BlueChannelIndex);
      m_FullWidget->SetRedChannelIndex(m_RedChannelIndex);
      m_FullWidget->SetGreenChannelIndex(m_GreenChannelIndex);
      m_FullWidget->SetBlueChannelIndex(m_BlueChannelIndex);

      ComputeNormalizationFactors();

      typename HistogramWidgetType::ColorType blue, red, green;

      red[0] = 0.5;
      red[1] = 0;
      red[2] = 0;

      green[0] = 0;
      green[1] = 0.5;
      green[2] = 0;

      blue[0] = 0;
      blue[1] = 0;
      blue[2] = 0.5;

      m_RedHistogramWidget->SetHistogram(m_HistogramGeneratorList->GetNthElement(m_RedChannelIndex)->GetOutput());
      m_RedHistogramWidget->SetTransferFunction(m_TransferFunctionList->GetNthElement(m_RedChannelIndex));
      m_RedHistogramWidget->SetLabel("Red channel");
      m_RedHistogramWidget->SetTransferFunctionLabel("Affine");
      m_RedHistogramWidget->SetHistogramColor(red);
      m_RedHistogramWidget->SetTextColor(red);

      m_BlueHistogramWidget->SetHistogram(m_HistogramGeneratorList->GetNthElement(m_BlueChannelIndex)->GetOutput());
      m_BlueHistogramWidget->SetTransferFunction(m_TransferFunctionList->GetNthElement(m_BlueChannelIndex));
      m_BlueHistogramWidget->SetLabel("Blue channel");
      m_BlueHistogramWidget->SetTransferFunctionLabel("Affine");
      m_BlueHistogramWidget->SetHistogramColor(blue);
      m_BlueHistogramWidget->SetTextColor(blue);
      m_GreenHistogramWidget->SetHistogram(m_HistogramGeneratorList->GetNthElement(m_GreenChannelIndex)->GetOutput());
      m_GreenHistogramWidget->SetTransferFunction(m_TransferFunctionList->GetNthElement(m_GreenChannelIndex));
      m_GreenHistogramWidget->SetLabel("Green channel");
      m_GreenHistogramWidget->SetTransferFunctionLabel("Affine");
      m_GreenHistogramWidget->SetHistogramColor(green);
      m_GreenHistogramWidget->SetTextColor(green);
      break;
//   }
      }
    case ScrollWidgetType::GRAYSCALE:
      {
      if (m_UseScroll)
        {
        m_ScrollWidget->SetViewModel(viewModel);
        m_ScrollWidget->SetRedChannelIndex(m_RedChannelIndex);
        }
      m_FullWidget->SetViewModel(viewModel);
      m_ZoomWidget->SetViewModel(viewModel);
      m_ZoomWidget->SetRedChannelIndex(m_RedChannelIndex);
      m_FullWidget->SetRedChannelIndex(m_RedChannelIndex);

      ComputeNormalizationFactors();

      typename HistogramWidgetType::ColorType gray;
      gray.Fill(0.5);

      m_RedHistogramWidget->SetHistogram(m_HistogramGeneratorList->GetNthElement(m_RedChannelIndex)->GetOutput());
      m_RedHistogramWidget->SetTransferFunction(m_TransferFunctionList->GetNthElement(m_RedChannelIndex));
      m_RedHistogramWidget->SetLabel("Grayscale channel");
      m_RedHistogramWidget->SetTransferFunctionLabel("Affine");
      m_RedHistogramWidget->SetHistogramColor(gray);
      m_RedHistogramWidget->SetTextColor(gray);
      break;
      }
    case ScrollWidgetType::COMPLEX_MODULUS:
      {
      if (m_UseScroll)
        {
        m_ScrollWidget->SetViewModel(viewModel);
        m_ScrollWidget->SetRedChannelIndex(m_RedChannelIndex);
        m_ScrollWidget->SetGreenChannelIndex(m_GreenChannelIndex);
        }
      m_FullWidget->SetViewModel(viewModel);
      m_ZoomWidget->SetViewModel(viewModel);
      m_ZoomWidget->SetRedChannelIndex(m_RedChannelIndex);
      m_FullWidget->SetRedChannelIndex(m_RedChannelIndex);
      m_ZoomWidget->SetGreenChannelIndex(m_GreenChannelIndex);
      m_FullWidget->SetGreenChannelIndex(m_GreenChannelIndex);

      ComputeNormalizationFactors();

      typename HistogramWidgetType::ColorType gray;
      gray.Fill(0.5);

      m_RedHistogramWidget->SetHistogram(m_HistogramGeneratorList->GetNthElement(0)->GetOutput());
      m_RedHistogramWidget->SetTransferFunction(m_TransferFunctionList->GetNthElement(0));
      m_RedHistogramWidget->SetLabel("Modulus");
      m_RedHistogramWidget->SetTransferFunctionLabel("Affine");
      m_RedHistogramWidget->SetHistogramColor(gray);
      m_RedHistogramWidget->SetTextColor(gray);
      break;
      }
    case ScrollWidgetType::COMPLEX_PHASE:
      {
      if (m_UseScroll)
        {
        m_ScrollWidget->SetViewModel(viewModel);
        m_ScrollWidget->SetRedChannelIndex(m_RedChannelIndex);
        m_ScrollWidget->SetGreenChannelIndex(m_GreenChannelIndex);
        }
      m_FullWidget->SetViewModel(viewModel);
      m_ZoomWidget->SetViewModel(viewModel);
      m_ZoomWidget->SetRedChannelIndex(m_RedChannelIndex);
      m_FullWidget->SetRedChannelIndex(m_RedChannelIndex);
      m_ZoomWidget->SetGreenChannelIndex(m_GreenChannelIndex);
      m_FullWidget->SetGreenChannelIndex(m_GreenChannelIndex);

      ComputeNormalizationFactors();

      typename HistogramWidgetType::ColorType gray;
      gray.Fill(0.5);

      m_RedHistogramWidget->SetHistogram(m_HistogramGeneratorList->GetNthElement(0)->GetOutput());
      m_RedHistogramWidget->SetTransferFunction(m_TransferFunctionList->GetNthElement(0));
      m_RedHistogramWidget->SetLabel("Phase");
      m_RedHistogramWidget->SetTransferFunctionLabel("Affine");
      m_RedHistogramWidget->SetHistogramColor(gray);
      m_RedHistogramWidget->SetTextColor(gray);
      break;
      }
    }
}

template<class TPixel, class TLabel>
void
ImageViewerBase<TPixel, TLabel>
::InitializeViewModel(void)
{
  if (m_InputImage->GetNumberOfComponentsPerPixel() > 2)
    {
    this->SetViewModel(ScrollWidgetType::RGB);
    }
  else
    {
    this->SetViewModel(ScrollWidgetType::GRAYSCALE);
    }
}

template<class TPixel, class TLabel>
void
ImageViewerBase<TPixel, TLabel>
::Reset(void)
{
  m_FullWidget->Reset();
  m_ZoomWidget->Reset();
  if (m_UseScroll)
    {
    m_ScrollWidget->Reset();
    }
}

template<class TPixel, class TLabel>
void
ImageViewerBase<TPixel, TLabel>
::ChangeTransferFunctions(void)
{
  m_TransferFunctionList->SetNthElement(m_RedChannelIndex, m_RedHistogramWidget->GetTransferFunction());
  if (m_FullWidget->GetViewModel() == ScrollWidgetType::RGB)
    {
    m_TransferFunctionList->SetNthElement(m_BlueChannelIndex, m_BlueHistogramWidget->GetTransferFunction());
    m_TransferFunctionList->SetNthElement(m_GreenChannelIndex, m_GreenHistogramWidget->GetTransferFunction());
    }
  m_FullWidget->ClearBufferedRegion();
  m_ZoomWidget->ClearBufferedRegion();
  if (m_UseScroll)
    {
    m_ScrollWidget->ClearBufferedRegion();
    }
}

template<class TPixel, class TLabel>
void
ImageViewerBase<TPixel, TLabel>
::AddROIColorMapEntry(const LabelType& label, const ColorType& color)
{
  m_ROIColorMap[label] = color;
}

template<class TPixel, class TLabel>
void
ImageViewerBase<TPixel, TLabel>
::RemoveROIColorMapEntry(const LabelType& label)
{
  m_ROIColorMap.erase(label);
}

template<class TPixel, class TLabel>
bool
ImageViewerBase<TPixel, TLabel>
::GetROIColorMapEntry(const LabelType& label, ColorType& color)
{
  typename ROIColorMapType::iterator it = m_ROIColorMap.find(label);
  bool                               resp = false;

  if (it != m_ROIColorMap.end())
    {
    color = it->second;
    resp = true;
    }

  return resp;
}

template<class TPixel, class TLabel>
void
ImageViewerBase<TPixel, TLabel>
::ClearROIColorMap(void)
{
  m_ROIColorMap.clear();
}

} // end namespace otb
#endif
