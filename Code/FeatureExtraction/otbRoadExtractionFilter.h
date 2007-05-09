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
#include "otbNeighborhoodScalarProductFilter.h"
#include "otbNonMaxRemovalByDirectionFilter.h"
#include "otbVectorizationPathListFilter.h"
#include "otbSimplifyPathFilter.h"
#include "otbBreakAngularPathListFilter.h"
#include "otbRemoveTortuousPathFilter.h"
#include "otbLinkPathFilter.h"
#include "otbRemoveIsolatedByDirectionFilter.h"
#include "otbRemoveWrongDirectionFilter.h"
#include "otbLikehoodPathFilter.h"

namespace otb
{
/**
 * \class RoadExtractionFilter
 * \brief This class performs the extraction of roads from an image.
 * 
 * \sa SpectralAngleDistanceImageFilter
 * \sa itk::GradientRecursiveGaussianImageFilter
 * \sa NeighborhoodScalarProductFilter
 * \sa RemoveIsolatedByDirectionFilter
 * \sa RemoveWrongDirectionFilter
 * \sa NonMaxRemovalByDirectionFilter
 * \sa VectorizationPathListFilter
 * \sa SimplifyPathFilter
 * \sa BreakAngularPathListFilter
 * \sa RemoveTortuousPathFilter
 * \sa LinkPathListFilter
 * \sa LikehoodPathFilter
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
    typedef typename InputImageType::PixelType 							InputPixelType;
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

    typedef SimplifyPathFilter<OutputPathType>                  SimplifyPathFilterType;
    typedef BreakAngularPathListFilter<OutputPathType>          BreakAngularPathListFilterType;
    typedef RemoveTortuousPathFilter<OutputPathType>            RemoveTortuousPathFilterType;
    typedef LinkPathFilter<OutputPathType>                  	LinkPathFilterType;
    typedef LikehoodPathFilter<OutputPathType, ModulusType>		LikehoodPathFilterType;
    
    /** Template parameters typedefs for internals filters */
    typedef typename GradientFilterType::RealType SigmaType;
    typedef typename VectorizationPathListFilterType::InputPixelType AmplitudeThresholdType;
    typedef typename SimplifyPathFilterType::ToleranceType ToleranceType;
    typedef typename BreakAngularPathListFilterType::MaxAngleType MaxAngleType;
    typedef typename RemoveTortuousPathFilterType::MeanDistanceThresholdType MeanDistanceThresholdType; 
    typedef typename LinkPathFilterType::RealType LinkRealType;
     
  /** Get/Set the reference pixel (use by the SpectralAngleDistanceImageFilter)*/
  itkGetConstReferenceMacro(ReferencePixel,InputPixelType);
  itkSetMacro(ReferencePixel,InputPixelType);

  /** Get/Set the sigma (use by the GradientRecursiveGaussianImageFilter)*/
  itkGetConstReferenceMacro(Sigma,SigmaType);
  itkSetMacro(Sigma,SigmaType);

  /** Get/Set the amplitude threshold to start following a path (use by the VectorizationPathListFilter)*/
  itkSetMacro(AmplitudeThreshold,AmplitudeThresholdType);
  itkGetMacro(AmplitudeThreshold,AmplitudeThresholdType);

  /** Get/Set  the tolerance for segment consistency (tolerance in terms of distance) (use by the SimplifyPathFilter)*/
  itkGetMacro(Tolerance,ToleranceType);
  itkSetMacro(Tolerance,ToleranceType);

  /** Set/Get the max angle (use bye the BreakAngularPathListFilter)*/
  itkSetMacro(MaxAngle,MaxAngleType);
  itkGetConstMacro(MaxAngle,MaxAngleType);

  /** Get/Set the tolerance for segment consistency (tolerance in terms of distance) (use by RemoveTortuousPathFilter)*/
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
    /** Main computation method */
    void GenerateData(void);
    /** PrintSelf method */
    void PrintSelf(std::ostream& os, itk::Indent indent) const;

  private :
    
    RoadExtractionFilter(const Self&); // purposely not implemented
    void operator=(const Self&); // purposely not implemented

    /** SpectralAngleDistanceImageFilter use by the composite filter */
    typename SpectralAngleDistanceImageFilterType::Pointer      m_SpectralAngleDistanceImageFilter;
    typename GradientFilterType::Pointer                        m_GradientFilter;
    typename NeighborhoodScalarProductFilterType::Pointer       m_NeighborhoodScalarProductFilter;
    typename RemoveIsolatedByDirectionFilterType::Pointer       m_RemoveIsolatedByDirectionFilter;
    typename RemoveWrongDirectionFilterType::Pointer            m_RemoveWrongDirectionFilter;
    typename NonMaxRemovalByDirectionFilterType::Pointer        m_NonMaxRemovalByDirectionFilter;
    typename VectorizationPathListFilterType::Pointer           m_VectorizationPathListFilter;
    typename SimplifyPathFilterType::Pointer                    m_FirstSimplifyPathFilter;
    typename SimplifyPathFilterType::Pointer                    m_SecondSimplifyPathFilter;
    typename BreakAngularPathListFilterType::Pointer            m_BreakAngularPathListFilter;
    typename RemoveTortuousPathFilterType::Pointer              m_FirstRemoveTortuousPathFilter;
    typename RemoveTortuousPathFilterType::Pointer              m_SecondRemoveTortuousPathFilter;
    typename LinkPathFilterType::Pointer                    	m_LinkPathFilter;
    typename LikehoodPathFilterType::Pointer 					m_LikehoodPathFilter;


  /** The reference pixel (use by the SpectralAngleDistanceImageFilter)*/
  InputPixelType m_ReferencePixel;
  /** Sigma value (use by the GradientRecursiveGaussianImageFilter)*/
  SigmaType m_Sigma;
  /** Amplitude threshold to start following a path (use by the VectorizationPathListFilter)*/
  AmplitudeThresholdType m_AmplitudeThreshold;
  /** Tolerance for segment consistency (tolerance in terms of distance) (use by the SimplifyPathFilter)*/
  ToleranceType m_Tolerance;
  /** Max angle (use bye the BreakAngularPathListFilter)*/
  MaxAngleType m_MaxAngle;
  /** Tolerance for segment consistency (tolerance in terms of distance) (use by RemoveTortuousPathFilter)*/
  MeanDistanceThresholdType m_FirstMeanDistanceThreshold;
  MeanDistanceThresholdType m_SecondMeanDistanceThreshold;
  /** The angular threshold (use by LinkPathFilter) */
  LinkRealType m_AngularThreshold;
  /** The distance threshold (use by LinkPathFilter) */
  LinkRealType m_DistanceThreshold;

  };

}// End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbRoadExtractionFilter.txx"
#endif

#endif
