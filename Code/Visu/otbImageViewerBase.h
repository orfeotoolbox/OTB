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
#ifndef __otbImageViewerBase_h
#define __otbImageViewerBase_h

#include "vcl_deprecated_header.h"

#include <string>
#include "itkObject.h"
#include "otbImageViewerScrollWidget.h"
#include "otbImageViewerZoomWidget.h"
#include "otbImageViewerFullWidget.h"
#include "otbImageViewerHistogramAndTransferFunctionWidget.h"
#include "otbStreamingShrinkImageFilter.h"
#include "otbImageWidgetBoxForm.h"
#include "otbImageWidgetPolygonForm.h"
#include "otbImageWidgetPolylineForm.h"
#include "otbImageWidgetCircleForm.h"
#include "itkListSample.h"
#include "otbObjectList.h"
#include "itkCovarianceSampleFilter.h"
#include "itkMacro.h"
#include <FL/Fl_Multiline_Output.H>
#include "otbImage.h"
#include "otbImageToVectorImageCastFilter.h"
#include <map>
#include "itkImageRegionIterator.h"
#include "itkSampleToHistogramFilter.h"
#include "itkDenseFrequencyContainer2.h"
#include "itkHistogram.h"

namespace otb
{
/** \class ImageViewerBase
   * \brief <b>DEPRECATED</b>: Simple image viewer base class.
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
 * Unkike most OTB filters, ImageViewerBase is templated over the input pixel type and will take as input an otb::VectorImage<PixelType>.
 * Be aware of that.
 *
   *  \deprecated use the new Visualization framework instead.
   * \sa StandardImageViewer
   *
 * \sa ImageViewerFullWidget, ImageViewerScrollWidget, ImageViewerZoomWidget
 */
template <class TPixel, class TLabel = double>
class ITK_EXPORT ImageViewerBase
  : public itk::ProcessObject
{
public:
  /** Standard class typedefs */
  typedef ImageViewerBase               Self;
  typedef itk::ProcessObject            Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageViewerBase, itk::ProcessObject);

  /// Template pixel type
  typedef TPixel                                                  InputPixelType;
  typedef TLabel                                                  LabelType;
  typedef otb::ImageViewerScrollWidget<InputPixelType, LabelType> ScrollWidgetType;
  typedef otb::ImageViewerZoomWidget<InputPixelType, LabelType>   ZoomWidgetType;
  typedef otb::ImageViewerFullWidget<InputPixelType, LabelType>   FullWidgetType;
  typedef Fl_Output *                                             FlOutputPointerType;

  typedef typename ScrollWidgetType::ImageType                  ImageType;
  typedef typename ScrollWidgetType::OverlayImageType           OverlayImageType;
  typedef typename ScrollWidgetType::TransferFunctionListType   TransferFunctionListType;
  typedef typename TransferFunctionListType::Pointer            TransferFunctionListPointerType;
  typedef typename ScrollWidgetType::AffineTransferFunctionType AffineTransferFunctionType;
  typedef typename ImageType::Pointer                           ImagePointerType;
  typedef typename OverlayImageType::Pointer                    OverlayImagePointerType;
  typedef typename ImageType::IndexType                         IndexType;
  typedef typename ImageType::PointType                         PointType;
  typedef typename ImageType::SizeType                          SizeType;
  typedef typename ImageType::PixelType                         PixelType;
  typedef typename ImageType::RegionType                        RegionType;
  typedef typename ImageType::OffsetType                        OffsetType;
  typedef typename ScrollWidgetType::VectorPixelType            VectorPixelType;
  typedef typename ScrollWidgetType::ViewModelType              ViewModelType;
  typedef typename ScrollWidgetType::Pointer                    ScrollWidgetPointerType;
  typedef typename ZoomWidgetType::Pointer                      ZoomWidgetPointerType;
  typedef typename FullWidgetType::Pointer                      FullWidgetPointerType;
  itkStaticConstMacro(ImageDimension, unsigned int, ImageType::ImageDimension);

  typedef itk::ImageRegionConstIterator<ImageType>                              InputIteratorType;
  typedef itk::Vector<typename itk::NumericTraits<InputPixelType>::RealType, 1> MeasurementVectorType;
  typedef itk::Statistics::ListSample<MeasurementVectorType>                    ListSampleType;
  typedef float                                                                 HistogramMeasurementType;

  typedef itk::Statistics::DenseFrequencyContainer2 FrequencyContainerType;
  typedef itk::Statistics::Histogram< HistogramMeasurementType, FrequencyContainerType > HistogramType;

  typedef itk::Statistics::SampleToHistogramFilter<ListSampleType, HistogramType> HistogramGeneratorType;
  typedef otb::ObjectList<ListSampleType>
  ListSampleListType;
  typedef otb::ObjectList<HistogramGeneratorType>
  HistogramGeneratorListType;
  typedef typename HistogramGeneratorListType::Pointer
  HistogramGeneratorListPointerType;
  typedef otb::ImageViewerHistogramAndTransferFunctionWidget<HistogramType, InputPixelType,
      LabelType> HistogramWidgetType;
  typedef typename HistogramWidgetType::Pointer
  HistogramWidgetPointerType;

  /// Support for conversion from otb::Image to otb::VectorImage
  typedef typename PixelType::ValueType                                 ValueType;
  typedef otb::Image<ValueType, ImageDimension>                         SingleImageType;
  typedef typename SingleImageType::Pointer                             SingleImagePointerType;
  typedef otb::ImageToVectorImageCastFilter<SingleImageType, ImageType> VectorCastFilterType;
  typedef typename VectorCastFilterType::Pointer                        VectorCastFilterPointerType;

  /// Definition of streaming shrink image filter
  typedef otb::StreamingShrinkImageFilter<ImageType, ImageType>               ShrinkFilterType;
  typedef typename ShrinkFilterType::Pointer                                  ShrinkFilterPointerType;
  typedef otb::StreamingShrinkImageFilter<OverlayImageType, OverlayImageType> OverlayShrinkFilterType;
  typedef typename OverlayShrinkFilterType::Pointer                           OverlayShrinkFilterPointerType;

  /// Definition of box form to represent displayed regions.
  typedef otb::ImageWidgetBoxForm   BoxType;
  typedef typename BoxType::Pointer BoxPointerType;

  /// List of linked viewer typedef
  typedef otb::ObjectList<Self>            ViewerListType;
  typedef typename ViewerListType::Pointer ViewerListPointerType;

  /// Linked viewer offset list
  typedef std::vector<OffsetType> OffsetListType;

  /// Type for the overlay list
  typedef typename FullWidgetType::FormListType FormListType;
  typedef typename FormListType::Pointer        FormListPointerType;
  typedef typename FormListType::Iterator       FormListIteratorType;

  /// type for the list of the polygon ROI
  typedef otb::Polygon<LabelType>                           PolygonType;
  typedef typename PolygonType::VertexListConstIteratorType PolygonIteratorType;
  typedef otb::ObjectList<PolygonType>                      PolygonListType;
  typedef typename PolygonListType::Pointer                 PolygonListPointerType;
  typedef typename PolygonListType::Iterator                PolygonListIteratorType;
  typedef ImageWidgetPolygonForm<LabelType>                 ImageWidgetPolygonFormType;
  typedef typename ImageWidgetPolygonFormType::Pointer      ImageWidgetPolygonFormPointerType;
  typedef typename ImageWidgetPolygonFormType::ColorType    ColorType;
  typedef ImageWidgetCircleForm                             ImageWidgetCircleFormType;
  typedef typename ImageWidgetCircleFormType::Pointer       ImageWidgetCircleFormPointerType;
  typedef std::map<LabelType, ColorType>                    ROIColorMapType;

  typedef otb::PolyLineParametricPathWithValue<double, 2> PathType;
  typedef typename PathType::VertexListConstIteratorType  PathIteratorType;
  typedef otb::ObjectList<PathType>                       PathListType;
  typedef typename PathListType::Pointer                  PathListPointerType;
  typedef typename PathListType::Iterator                 PathListIteratorType;
  typedef ImageWidgetPolylineForm<double>                 ImageWidgetPolylineFormType;
  typedef typename ImageWidgetPolylineFormType::Pointer   ImageWidgetPolylineFormPointerType;

  /// Accessors
  itkGetMacro(Built, bool);
  itkGetMacro(ShrinkFactor, unsigned int);
  itkSetMacro(RedChannelIndex, unsigned int);
  itkGetMacro(RedChannelIndex, unsigned int);
  itkSetMacro(GreenChannelIndex, unsigned int);
  itkGetMacro(GreenChannelIndex, unsigned int);
  itkSetMacro(BlueChannelIndex, unsigned int);
  itkGetMacro(BlueChannelIndex, unsigned int);
  itkSetMacro(Label, std::string);
  itkGetMacro(Label, std::string);
  itkSetMacro(QuicklookRatioCoef, double);
  itkGetMacro(QuicklookRatioCoef, double);
  itkSetMacro(NormalizationFactor, double);
  itkGetMacro(NormalizationFactor, double);
  itkGetMacro(Updating, bool);
  itkGetMacro(UseScroll, bool);
  itkGetObjectMacro(PolygonROIList, PolygonListType);
  itkSetObjectMacro(PolygonROIList, PolygonListType);
  itkGetObjectMacro(PathList, PathListType);
  itkSetObjectMacro(PathList, PathListType);
  itkGetObjectMacro(InterfaceBoxesList, FormListType);
  itkSetObjectMacro(InterfaceBoxesList, FormListType);
  itkSetMacro(InterfaceBoxesColor, ColorType);
  itkGetMacro(InterfaceBoxesColor, ColorType);
  itkSetMacro(DefaultROIColor, ColorType);
  itkGetMacro(DefaultROIColor, ColorType);
  itkSetMacro(NextROILabel, LabelType);
  itkGetMacro(NextROILabel, LabelType);
  itkGetMacro(FullWidget, FullWidgetPointerType);
  itkGetMacro(ScrollWidget, ScrollWidgetPointerType);
  itkGetMacro(ZoomWidget, ZoomWidgetPointerType);
  itkGetMacro(RedHistogramWidget, HistogramWidgetPointerType);
  itkGetMacro(GreenHistogramWidget, HistogramWidgetPointerType);
  itkGetMacro(BlueHistogramWidget, HistogramWidgetPointerType);
  itkGetMacro(ZoomMaxInitialSize, unsigned int);
  itkSetMacro(ZoomMaxInitialSize, unsigned int);
  itkGetMacro(ScrollMaxInitialSize, unsigned int);
  itkSetMacro(ScrollMaxInitialSize, unsigned int);
  itkGetMacro(FullMaxInitialSize, unsigned int);
  itkSetMacro(FullMaxInitialSize, unsigned int);
  itkSetMacro(ScrollLimitSize, unsigned int);
  itkGetMacro(ScrollLimitSize, unsigned int);
  itkGetMacro(PixLocOutput, FlOutputPointerType);
  itkSetMacro(PixLocOutput, FlOutputPointerType);
  itkSetMacro(RectangularROISelectionMode, bool);
  itkGetMacro(RectangularROISelectionMode, bool);
  itkSetMacro(PolygonalROISelectionMode, bool);
  itkGetMacro(PolygonalROISelectionMode, bool);
  itkGetObjectMacro(InputImage, ImageType);
  itkGetMacro(ImageGeometry, double);
  itkSetMacro(ShowZoomWidget, bool);
  itkGetMacro(ShowZoomWidget, bool);
  itkSetMacro(ShowFullWidget, bool);
  itkGetMacro(ShowFullWidget, bool);
  itkSetMacro(ShowScrollWidget, bool);
  itkGetMacro(ShowScrollWidget, bool);
  itkSetMacro(UseImageOverlay, bool);
  itkGetMacro(UseImageOverlay, bool);
  itkSetMacro(ShowHistograms, bool);
  itkGetMacro(ShowHistograms, bool);
  itkGetObjectMacro(TransferFunctionList, TransferFunctionListType);
  itkSetMacro(LineWidth, double);
  itkGetMacro(LineWidth, double);

  /** Set the input image (VectorImage version) */
  virtual void SetImage(ImageType * img);

  /** Set the input image (Image version) */
  virtual void SetImage(SingleImageType * img);

  /** Set the input image overlay (VectorImage of unsigned char image with at least 3 channels) */
  virtual void SetImageOverlay(OverlayImageType * img);

  /** Get the shrinked image if scroll is activated and else the input image */
  virtual ImageType * GetShrinkedImage(void);

  /** Show the viewer (Update) */
  virtual void Show(void);
  /** Hide all Image View Windows */
  virtual void Hide(void);

  /** Compute the normalization factors */
  virtual void ComputeNormalizationFactors(void);

  /** Build the interfaces */
  virtual void Build(void);

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
  virtual void ReportPixel(IndexType index);

  /** Clear pixel information */
  virtual void ClearPixLocVal(void);

  /** Generate overlay list */
  virtual void GenerateOverlayList(void);

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
   * Return the internal index if the viewers are linked
   * \param viewer the viewer to ask
   * \return -1 if the viewer are not linked
   */
  virtual int IsLinkedTo(Self * viewer);
  /**
   * Return the offset associated with the linked viewer at the internal index.
   * \param index the internal index
   * \return the Offset
   */
  virtual OffsetType GetOffset(int index);

  /**
   * Clear all the links of the current viewer.
   */
  virtual void ClearLinks(void);

  /**
   * \return true if view model is RGB
   */
  virtual ViewModelType GetViewModel(void);

  /**
   * Set the view model
   * \param viewModel True to turn on RGB view model
   */
  virtual void SetViewModel(ViewModelType viewModel);

  /**
   * Initialize view model
   */
  virtual void InitializeViewModel(void);

  /**
   * Force opengl buffers reset
   */
  virtual void Reset(void);

  /**
  * Force redraw
  */
  virtual void ChangeTransferFunctions(void);

  /**
   * Add a new color-label combination in the ROI color map. Note
   * that if the entry already exists, it will be overwritten.
   * \param label The label
   * \param color the color to associate the label with
   */
  virtual void AddROIColorMapEntry(const LabelType& label, const ColorType& color);

  /**
   * Remove a new color-label combination in the ROI color map. If the label
   * is not present, does nothing.
   * \param label The label
   */
  virtual void RemoveROIColorMapEntry(const LabelType& label);

  /**
   * Fill the color parameter with the color entry associated with the label. Returns
   * true if the entry exists, and false otherwise.
   * \param label The label
   * \param color The color
   * \return true if the label was found.
   */
  virtual bool GetROIColorMapEntry(const LabelType& label, ColorType& color);

  /**
   * Clear the ROI color map.
   */
  virtual void ClearROIColorMap(void);

  /**
   * Set the image overlay opacity of all widgets
   * \param opacity
   */
  virtual void SetImageOverlayOpacity(unsigned char opacity);

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
  virtual void Unlink(Self * viewer, bool backwardLinkFlag);

  // Constructor and destructor
  ImageViewerBase();
  virtual ~ImageViewerBase();

  /** Line width*/
  double m_LineWidth;

private:
  ImageViewerBase(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  /// Label
  std::string m_Label;
  /// zoom widget component
  ZoomWidgetPointerType m_ZoomWidget;
  /// Show zoom widget flag
  bool m_ShowZoomWidget;
  /// scroll widget component
  ScrollWidgetPointerType m_ScrollWidget;
  /// Show scroll widget flag
  bool m_ShowScrollWidget;
  /// full widget component
  FullWidgetPointerType m_FullWidget;
  /// Show full widget flag
  bool m_ShowFullWidget;
  /// The image to view
  ImagePointerType m_InputImage;
  /// The image overlay
  OverlayImagePointerType m_InputImageOverlay;
  /// true if scroll widget is used
  bool m_UseScroll;
  /// Show the histograms
  bool m_ShowHistograms;

  /// Intial sizes
  unsigned int m_ScrollMaxInitialSize;
  unsigned int m_FullMaxInitialSize;
  unsigned int m_ZoomMaxInitialSize;
  /// Image geomerty ratio
  double m_ImageGeometry;
  /// Limit size for the scroll view
  unsigned int        m_ScrollLimitSize;
  FlOutputPointerType m_PixLocOutput;
  /// Pointer to the shrink filters
  ShrinkFilterPointerType        m_Shrink;
  OverlayShrinkFilterPointerType m_ShrinkOverlay;

  /// The shrink factor
  unsigned int m_ShrinkFactor;
  /// true if the Gui has been built.
  bool m_Built;
  /// Channel indices
  unsigned int m_RedChannelIndex;
  unsigned int m_GreenChannelIndex;
  unsigned int m_BlueChannelIndex;
  /// Quicklook quality factor
  double m_QuicklookRatioCoef;
  /// Normalization quality factor
  double m_NormalizationFactor;
  /// Converter from otb::Image to otb::VectorImage
  VectorCastFilterPointerType m_VectorCastFilter;
  VectorCastFilterPointerType m_VectorCastFilterOverlay;
  /// Wether the viewer is updating or not
  bool m_Updating;
  /// The list of viewer with which this viewer is linked
  ViewerListPointerType m_LinkedViewerList;
  /// Linked viewer offset list
  OffsetListType m_LinkedViewerOffsetList;
  /// PolygonList
  PolygonListPointerType m_PolygonROIList;
  /// PathList
  PathListPointerType m_PathList;
  /// Interface boxes
  FormListPointerType m_InterfaceBoxesList;
  /// Next ROI color
  ColorType m_DefaultROIColor;
  /// Interfaces box color
  ColorType m_InterfaceBoxesColor;
  /// Label of the next ROI
  LabelType m_NextROILabel;
  /// Toogle the rectangular ROI selection mode
  bool m_RectangularROISelectionMode;
  /// Toogle the polygonal ROI selection mode
  bool m_PolygonalROISelectionMode;
  /// Map used to associate a label with a color
  ROIColorMapType m_ROIColorMap;
  /// True if an image overlay is used
  bool m_UseImageOverlay;
  /// List of histogram generators (one per channel)
  HistogramGeneratorListPointerType m_HistogramGeneratorList;
  /// Transfer function list
  TransferFunctionListPointerType m_TransferFunctionList;

  /// Histogram widgets
  HistogramWidgetPointerType m_RedHistogramWidget;
  HistogramWidgetPointerType m_BlueHistogramWidget;
  HistogramWidgetPointerType m_GreenHistogramWidget;

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageViewerBase.txx"
#endif

#endif
