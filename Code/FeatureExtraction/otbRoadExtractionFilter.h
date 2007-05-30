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
#ifndef _otbRoadExtractionFilter_h
#define _otbRoadExtractionFilter_h

#include "itkCovariantVector.h"
#include "itkGradientRecursiveGaussianImageFilter.h"

#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageToPathListFilter.h"
#include "otbSpectralAngleDistanceImageFilter.h"
#include "itkSqrtImageFilter.h"
#include "otbNeighborhoodScalarProductFilter.h"
#include "otbNonMaxRemovalByDirectionFilter.h"
#include "otbVectorizationPathListFilter.h"
#include "otbSimplifyPathListFilter.h"
#include "otbBreakAngularPathListFilter.h"
#include "otbRemoveTortuousPathListFilter.h"
#include "otbLinkPathListFilter.h"
#include "otbRemoveIsolatedByDirectionFilter.h"
#include "otbRemoveWrongDirectionFilter.h"
#include "otbLikehoodPathListFilter.h"

namespace otb
{
/**
 * \class RoadExtractionFilter
 * \brief This class performs the extraction of roads from an image.
 * 
 * This composite filter implement a fast and robust road extraction
 * for high resolution satellite images. The algorithm uses all spectral
 * bands based on the spectral angle with a reference pixel. The line 
 * detection done using a Gaussian gradient with a scalar product to find
 * the road directions. Finally, extracted roads are vectorized and 
 * processed to improve the results: remove some occultations and false 
 * detections.
 *
 * This filter is fast as the detection typically takes 3 seconds for a 
 * 1000 \f$ \times \f$ 1000 images with four spectral bands. Results can be
 * used as initialization for more complex algorithms.
 *
 * \sa SpectralAngleDistanceImageFilter
 * \sa itk::SqrtImageFilter
 * \sa itk::GradientRecursiveGaussianImageFilter
 * \sa NeighborhoodScalarProductFilter
 * \sa RemoveIsolatedByDirectionFilter
 * \sa RemoveWrongDirectionFilter
 * \sa NonMaxRemovalByDirectionFilter
 * \sa VectorizationPathListFilter
 * \sa SimplifyPathListFilter
 * \sa BreakAngularPathListFilter
 * \sa RemoveTortuousPathListFilter
 * \sa LinkPathListFilter
 * \sa LikehoodPathListFilter
 */
template <class TInputImage, class TOutputPath>
  class ITK_EXPORT RoadExtractionFilter
  : public ImageToPathListFilter<TInputImage,TOutputPath>
  {
    public :
    /** Standard typedefs */ 
    typedef RoadExtractionFilter                                Self;
    typedef ImageToPathListFilter<TInputImage,TOutputPath>      Superclass;
    typedef itk::SmartPointer<Self>                             Pointer;
    typedef itk::SmartPointer<const Self>                       ConstPointer;
    /** Creation through object factory macro */
    itkNewMacro(Self);
    /** Type macro */
    itkTypeMacro(RoadExtractionFilter,ImageToPathListFilter);
    /** Template parameters typedefs */
    typedef typename Superclass::InputImageType                         InputImageType;
    typedef typename Superclass::OutputPathType                         OutputPathType;
    typedef typename Superclass::OutputPathListType                     OutputPathListType;
    typedef typename InputImageType::PixelType 				InputPixelType;
    typedef double                                                      InternalPixelType;
    
    typedef otb::VectorImage<InternalPixelType,InputImageType::ImageDimension>  VectorImageType;
    typedef otb::Image<InternalPixelType,InputImageType::ImageDimension>        SpectralAngleType;
    typedef otb::Image<InternalPixelType,InputImageType::ImageDimension>        ModulusType;
    typedef otb::Image<InternalPixelType,InputImageType::ImageDimension>        DirectionType;
    
    typedef itk::CovariantVector<InternalPixelType,InputImageType::ImageDimension> 
                                                                        VectorPixelType;
    typedef otb::Image<VectorPixelType,InputImageType::ImageDimension>  CovariantVectorImageType;

    /* Template parameters typedefs for composites filters */
    typedef SpectralAngleDistanceImageFilter<
                        InputImageType,
                        SpectralAngleType>                      SpectralAngleDistanceImageFilterType;

    typedef itk::SqrtImageFilter<
                        SpectralAngleType,
                        SpectralAngleType>                      SquareRootImageFilterType;

    typedef itk::GradientRecursiveGaussianImageFilter<
                        SpectralAngleType,
                        CovariantVectorImageType>               GradientFilterType;

    typedef NeighborhoodScalarProductFilter<
                        CovariantVectorImageType,
                        ModulusType,
                        DirectionType>                          NeighborhoodScalarProductFilterType;

    typedef RemoveIsolatedByDirectionFilter<
                        ModulusType,
                        DirectionType,
                        ModulusType >                           RemoveIsolatedByDirectionFilterType;

    typedef RemoveWrongDirectionFilter<
                        ModulusType,
                        DirectionType,
                        ModulusType>                            RemoveWrongDirectionFilterType;

    typedef NonMaxRemovalByDirectionFilter<
                        ModulusType,
                        DirectionType,
                        ModulusType >                           NonMaxRemovalByDirectionFilterType;

    typedef VectorizationPathListFilter<
                        ModulusType,
                        DirectionType,
                        OutputPathType >                        VectorizationPathListFilterType;

    typedef SimplifyPathListFilter<OutputPathType>              SimplifyPathListFilterType;
    typedef BreakAngularPathListFilter<OutputPathType>          BreakAngularPathListFilterType;
    typedef RemoveTortuousPathListFilter<OutputPathType>        RemoveTortuousPathListFilterType;
    typedef LinkPathListFilter<OutputPathType>                  LinkPathListFilterType;
    typedef LikehoodPathListFilter<OutputPathType, ModulusType>	LikehoodPathListFilterType;
    
    /** Template parameters typedefs for internals filters */
    typedef typename GradientFilterType::RealType SigmaType;
    typedef typename VectorizationPathListFilterType::InputPixelType AmplitudeThresholdType;
    typedef typename SimplifyPathListFilterType::ToleranceType ToleranceType;
    typedef typename BreakAngularPathListFilterType::MaxAngleType MaxAngleType;
    typedef typename RemoveTortuousPathListFilterType::MeanDistanceThresholdType MeanDistanceThresholdType; 
    typedef typename LinkPathListFilterType::RealType LinkRealType;
     
  /** Get/Set the reference pixel (use by the SpectralAngleDistanceImageFilter)*/
  itkGetConstReferenceMacro(ReferencePixel,InputPixelType);
  itkSetMacro(ReferencePixel,InputPixelType);

  /** Get/Set the alpha value */
  itkGetConstReferenceMacro(Alpha,double);
  itkSetMacro(Alpha,double);

  /** Get/Set the amplitude threshold to start following a path (use by the VectorizationPathListFilter)*/
  itkSetMacro(AmplitudeThreshold,AmplitudeThresholdType);
  itkGetMacro(AmplitudeThreshold,AmplitudeThresholdType);

  /** Get/Set  the tolerance for segment consistency (tolerance in terms of distance) (use by the SimplifyPathListFilter)*/
  itkGetMacro(Tolerance,ToleranceType);
  itkSetMacro(Tolerance,ToleranceType);

  /** Set/Get the max angle (use bye the BreakAngularPathListFilter)*/
  itkSetMacro(MaxAngle,MaxAngleType);
  itkGetConstMacro(MaxAngle,MaxAngleType);

  /** Get/Set the tolerance for segment consistency (tolerance in terms of distance) (use by RemoveTortuousPathListFilter)*/
  itkGetMacro(FirstMeanDistanceThreshold,MeanDistanceThresholdType);
  itkSetMacro(FirstMeanDistanceThreshold,MeanDistanceThresholdType);
  itkGetMacro(SecondMeanDistanceThreshold,MeanDistanceThresholdType);
  itkSetMacro(SecondMeanDistanceThreshold,MeanDistanceThresholdType);
 
  /** Get/Set the angular threshold (use by LinkPathFilter)*/
  itkSetMacro(AngularThreshold,LinkRealType);
  itkGetMacro(AngularThreshold,LinkRealType);
  /** Get/Set the distance threshold (use by LinkPathFilter)*/
  itkSetMacro(DistanceThreshold,LinkRealType);
  itkGetMacro(DistanceThreshold,LinkRealType);
  
  
  protected:
    /** Constructor */
    RoadExtractionFilter();
    /** Destructor */
    ~RoadExtractionFilter() {};

    /** Prepare main computation method */
    void BeforeGenerateData(void);

    /** Main computation method */
    void GenerateData(void);
    /** PrintSelf method */
    void PrintSelf(std::ostream& os, itk::Indent indent) const;

  private :
    
    RoadExtractionFilter(const Self&); // purposely not implemented
    void operator=(const Self&); // purposely not implemented

    /** SpectralAngleDistanceImageFilter use by the composite filter */
    typename SpectralAngleDistanceImageFilterType::Pointer      m_SpectralAngleDistanceImageFilter;
    typename SquareRootImageFilterType::Pointer                 m_SquareRootImageFilter;
    typename GradientFilterType::Pointer                        m_GradientFilter;
    typename NeighborhoodScalarProductFilterType::Pointer       m_NeighborhoodScalarProductFilter;
    typename RemoveIsolatedByDirectionFilterType::Pointer       m_RemoveIsolatedByDirectionFilter;
    typename RemoveWrongDirectionFilterType::Pointer            m_RemoveWrongDirectionFilter;
    typename NonMaxRemovalByDirectionFilterType::Pointer        m_NonMaxRemovalByDirectionFilter;
    typename VectorizationPathListFilterType::Pointer           m_VectorizationPathListFilter;
    typename SimplifyPathListFilterType::Pointer                m_FirstSimplifyPathListFilter;
    typename SimplifyPathListFilterType::Pointer                m_SecondSimplifyPathListFilter;
    typename BreakAngularPathListFilterType::Pointer            m_BreakAngularPathListFilter;
    typename RemoveTortuousPathListFilterType::Pointer          m_FirstRemoveTortuousPathListFilter;
    typename RemoveTortuousPathListFilterType::Pointer          m_SecondRemoveTortuousPathListFilter;
    typename LinkPathListFilterType::Pointer                    m_LinkPathListFilter;
    typename LikehoodPathListFilterType::Pointer 		m_LikehoodPathListFilter;

  /** The reference pixel (use by the SpectralAngleDistanceImageFilter)*/
  InputPixelType m_ReferencePixel;
  /** Amplitude threshold to start following a path (use by the VectorizationPathListFilter)*/
  AmplitudeThresholdType m_AmplitudeThreshold;
  /** Tolerance for segment consistency (tolerance in terms of distance) (use by the SimplifyPathListFilter)*/
  ToleranceType m_Tolerance;
  /** Max angle (use bye the BreakAngularPathListFilter)*/
  MaxAngleType m_MaxAngle;
  /** Tolerance for segment consistency (tolerance in terms of distance) (use by RemoveTortuousPathListFilter)*/
  MeanDistanceThresholdType m_FirstMeanDistanceThreshold;
  MeanDistanceThresholdType m_SecondMeanDistanceThreshold;
  /** The angular threshold (use by LinkPathListFilter) */
  LinkRealType m_AngularThreshold;

  /** The distance threshold (use by LinkPathListFilter) */
  double m_DistanceThreshold;
  
  /** Alpha. Use to calculate the sigma value used by the GradientRecursiveGaussianImageFilter */
  double m_Alpha;

  /** Resolution of the image. Use to calculate the sigma value used by the GradientRecursiveGaussianImageFilter 
  and the m_DistanceThreshold value used by the LinkPathListFilter
  This value is set bye the image's spacing.*/
  double m_Resolution;

  };

}// End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbRoadExtractionFilter.txx"
#endif

#endif
