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
#ifndef __otbRoadExtractionFilter_h
#define __otbRoadExtractionFilter_h


#include "otbVectorImage.h"
#include "otbSpectralAngleDistanceImageFilter.h"
#include "otbGenericRoadExtractionFilter.h"

namespace otb
{
/**
 * \class RoadExtractionFilter
 * \brief This class performs the extraction of roads from an image.
 *
 * This composite filter implements a fast and robust road extraction
 * for high resolution satellite images. This is a composite filter using the
 * SpectralAngleDistanceImageFilter and the GenericRoadExtractionFilter and is
 * intended to be used with optical high resolution data. The full method is
 * described in E. Christophe and J. Inglada, "Robust Road Extraction for High
 * Resolution Satellite Images," in IEEE International Conference on
 * Image Processing, ICIP 2007.
 *
 * The algorithm uses all spectral
 * bands based on the spectral angle with a reference pixel. Then, the line
 * detection is done using a Gaussian gradient with a scalar product to find
 * the road directions. Finally, extracted roads are vectorized and
 * processed to improve the results removing some occultations and false
 * detections.
 *
 * This filter is fast, as the detection typically takes 3 seconds for a
 * 1000 \f$ \times \f$ 1000 images with four spectral bands. Results can be
 * used as an initialization for more complex algorithms.
 *
 * \sa SpectralAngleDistanceImageFilter
 * \sa GenericRoadExtractionFilter
 *
 * \example FeatureExtraction/ExtractRoadExample.cxx
 *
 */
template <class TInputImage, class TOutputPath>
class ITK_EXPORT RoadExtractionFilter
  : public ImageToPathListFilter<TInputImage, TOutputPath>
{
public:
  /** Standard typedefs */
  typedef RoadExtractionFilter                            Self;
  typedef ImageToPathListFilter<TInputImage, TOutputPath> Superclass;
  typedef itk::SmartPointer<Self>                         Pointer;
  typedef itk::SmartPointer<const Self>                   ConstPointer;
  /** Creation through object factory macro */
  itkNewMacro(Self);
  /** Type macro */
  itkTypeMacro(RoadExtractionFilter, ImageToPathListFilter);
  /** Template parameters typedefs */
  typedef typename Superclass::InputImageType     InputImageType;
  typedef typename Superclass::OutputPathType     OutputPathType;
  typedef typename Superclass::OutputPathListType OutputPathListType;
  typedef typename InputImageType::PixelType      InputPixelType;
  typedef double                                  InternalPixelType;

  typedef otb::VectorImage<InternalPixelType, InputImageType::ImageDimension> VectorImageType;
  typedef otb::Image<InternalPixelType, InputImageType::ImageDimension>       SpectralAngleType;
  typedef otb::Image<InternalPixelType, InputImageType::ImageDimension>       ModulusType;
  typedef otb::Image<InternalPixelType, InputImageType::ImageDimension>       DirectionType;

  typedef itk::CovariantVector<InternalPixelType, InputImageType::ImageDimension>
  VectorPixelType;
  typedef otb::Image<VectorPixelType, InputImageType::ImageDimension> CovariantVectorImageType;

  /* Template parameters typedefs for composites filters */
  typedef SpectralAngleDistanceImageFilter<
      InputImageType,
      SpectralAngleType>                      SpectralAngleDistanceImageFilterType;

  typedef GenericRoadExtractionFilter<SpectralAngleType, OutputPathType>
  GenericRoadExtractionFilterType;

  /** Template parameters typedefs for internals filters */
  typedef typename GenericRoadExtractionFilterType::SigmaType                 SigmaType;
  typedef typename GenericRoadExtractionFilterType::AmplitudeThresholdType    AmplitudeThresholdType;
  typedef typename GenericRoadExtractionFilterType::ToleranceType             ToleranceType;
  typedef typename GenericRoadExtractionFilterType::MaxAngleType              MaxAngleType;
  typedef typename GenericRoadExtractionFilterType::MeanDistanceThresholdType MeanDistanceThresholdType;
  typedef typename GenericRoadExtractionFilterType::LinkRealType              LinkRealType;

  /** Get/Set the reference pixel (use by the SpectralAngleDistanceImageFilter)*/
  otbGetObjectMemberConstReferenceMacro(SpectralAngleDistanceImageFilter, ReferencePixel, InputPixelType);
  otbSetObjectMemberMacro(SpectralAngleDistanceImageFilter, ReferencePixel, InputPixelType);

  /** Get/Set the alpha value */
  otbGetObjectMemberConstReferenceMacro(GenericRoadExtractionFilter, Alpha, double);
  otbSetObjectMemberMacro(GenericRoadExtractionFilter, Alpha, double);

  /** Get/Set the amplitude threshold to start following a path (use by the VectorizationPathListFilter)*/
  otbSetObjectMemberMacro(GenericRoadExtractionFilter, AmplitudeThreshold, AmplitudeThresholdType);
  otbGetObjectMemberMacro(GenericRoadExtractionFilter, AmplitudeThreshold, AmplitudeThresholdType);

  /** Get/Set  the tolerance for segment consistency (tolerance in terms of distance) (use by the SimplifyPathListFilter)*/
  otbGetObjectMemberMacro(GenericRoadExtractionFilter, Tolerance, ToleranceType);
  otbSetObjectMemberMacro(GenericRoadExtractionFilter, Tolerance, ToleranceType);

  /** Set/Get the max angle (use bye the BreakAngularPathListFilter)*/
  otbSetObjectMemberMacro(GenericRoadExtractionFilter, MaxAngle, MaxAngleType);
  otbGetObjectMemberConstMacro(GenericRoadExtractionFilter, MaxAngle, MaxAngleType);

  /** Get/Set the tolerance for segment consistency (tolerance in terms of distance) (use by RemoveTortuousPathListFilter)*/
  otbGetObjectMemberMacro(GenericRoadExtractionFilter, FirstMeanDistanceThreshold, MeanDistanceThresholdType);
  otbSetObjectMemberMacro(GenericRoadExtractionFilter, FirstMeanDistanceThreshold, MeanDistanceThresholdType);
  otbGetObjectMemberMacro(GenericRoadExtractionFilter, SecondMeanDistanceThreshold, MeanDistanceThresholdType);
  otbSetObjectMemberMacro(GenericRoadExtractionFilter, SecondMeanDistanceThreshold, MeanDistanceThresholdType);

  /** Get/Set the angular threshold (use by LinkPathFilter)*/
  otbSetObjectMemberMacro(GenericRoadExtractionFilter, AngularThreshold, LinkRealType);
  otbGetObjectMemberMacro(GenericRoadExtractionFilter, AngularThreshold, LinkRealType);
  /** Get/Set the distance threshold (use by LinkPathFilter)*/
  otbSetObjectMemberMacro(GenericRoadExtractionFilter, DistanceThreshold, LinkRealType);
  otbGetObjectMemberMacro(GenericRoadExtractionFilter, DistanceThreshold, LinkRealType);

protected:
  /** Constructor */
  RoadExtractionFilter();
  /** Destructor */
  ~RoadExtractionFilter() {}

  /** Prepare main computation method */
  void BeforeGenerateData(void);

  /** Main computation method */
  void GenerateData(void);
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:

  RoadExtractionFilter(const Self &); // purposely not implemented
  void operator =(const Self&); // purposely not implemented

  /** SpectralAngleDistanceImageFilter use by the composite filter */
  typename SpectralAngleDistanceImageFilterType::Pointer m_SpectralAngleDistanceImageFilter;
  typename GenericRoadExtractionFilterType::Pointer m_GenericRoadExtractionFilter;

};

} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbRoadExtractionFilter.txx"
#endif

#endif
