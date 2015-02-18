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
#ifndef __otbKMeansAttributesLabelMapFilter_h
#define __otbKMeansAttributesLabelMapFilter_h

#include "itkLabelMapFilter.h"
#include "otbLabelMapWithClassLabelToLabeledSampleListFilter.h"
#include "itkListSample.h"
#include "itkWeightedCentroidKdTreeGenerator.h"
#include "itkKdTreeBasedKmeansEstimator.h"

namespace otb {

/** \class KMeansAttributesLabelMapFilter
 * \brief Execute a KMeans on the attributes of a itk::LabelMap<otb::AttributesMapLabelObject>
 */
template<class TInputImage>
class ITK_EXPORT KMeansAttributesLabelMapFilter :
    public itk::Object
{
public:
  /** Standard class typedefs. */
  typedef KMeansAttributesLabelMapFilter                Self;
  typedef itk::LabelMapFilter<TInputImage, TInputImage> Superclass;
  typedef itk::SmartPointer<Self>                       Pointer;
  typedef itk::SmartPointer<const Self>                 ConstPointer;

  /** Some convenient typedefs. */
  typedef TInputImage                              InputImageType;
  typedef typename InputImageType::Pointer         InputImagePointer;
  typedef typename InputImageType::ConstPointer    InputImageConstPointer;
  typedef typename InputImageType::RegionType      InputImageRegionType;
  typedef typename InputImageType::PixelType       InputImagePixelType;
  typedef typename InputImageType::LabelObjectType LabelObjectType;
  typedef itk::DataObject                          DataObjectType;
  typedef DataObjectType::Pointer                  DataObjectPointerType;

  // LabelObject attributes
  typedef typename LabelObjectType::AttributesValueType     AttributesValueType;
  typedef typename LabelObjectType::ClassLabelType          ClassLabelType;

  // LabelMapToSampleList
  typedef itk::VariableLengthVector<AttributesValueType>    VectorType;
  typedef itk::FixedArray<ClassLabelType, 1>                 ClassLabelVectorType;

  typedef itk::Statistics::ListSample<VectorType>           ListSampleType;
  typedef itk::Statistics::ListSample<ClassLabelVectorType> TrainingListSampleType;
  typedef otb::LabelMapWithClassLabelToLabeledSampleListFilter<
            InputImageType,
            ListSampleType,
            TrainingListSampleType>                         LabelMapToSampleListFilterType;
  typedef typename LabelMapToSampleListFilterType::MeasurementFunctorType MeasurementFunctorType;

  // KMeans
  typedef itk::Statistics::WeightedCentroidKdTreeGenerator<ListSampleType>         TreeGeneratorType;
  typedef typename TreeGeneratorType::KdTreeType                                   TreeType;
  typedef itk::Statistics::KdTreeBasedKmeansEstimator<TreeType>                    EstimatorType;
  typedef itk::Statistics::EuclideanDistanceMetric<VectorType>                     DistanceType;
  typedef std::vector<VectorType>                                                  CentroidsVectorType;

  /** ImageDimension constants */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);

  /** Standard New method. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(KMeansAttributesLabelMapFilter,
               itk::Object);

  /** Return the centroids resulting from the KMeans */
  CentroidsVectorType& GetCentroids()
  {
    return m_Centroids;
  }
  const CentroidsVectorType& GetCentroids() const
  {
    return m_Centroids;
  }

  itkSetObjectMacro(InputLabelMap, InputImageType);
  itkGetObjectMacro(InputLabelMap, InputImageType);

  /** Set the number of classes of the input sample list.
   *  It will be used to choose the number of centroids.
   *  In the one-class case, 10 centroids is chosen. Otherwise,
   *  a number of centroids equal to the number of classes */
  itkSetMacro(NumberOfClasses, unsigned int);
  itkGetMacro(NumberOfClasses, unsigned int);

  MeasurementFunctorType& GetMeasurementFunctor()
  {
    return m_LabelMapToSampleListFilter->GetMeasurementFunctor();
  }

  void SetMeasurementFunctor(MeasurementFunctorType& functor)
  {
    m_LabelMapToSampleListFilter->SetMeasurementFunctor(functor);
  }

  void Compute();

protected:
  KMeansAttributesLabelMapFilter();
  ~KMeansAttributesLabelMapFilter() {};


private:
  KMeansAttributesLabelMapFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  InputImagePointer m_InputLabelMap;
  CentroidsVectorType m_Centroids;

  typename LabelMapToSampleListFilterType::Pointer m_LabelMapToSampleListFilter;
  unsigned int m_NumberOfClasses;

}; // end of class

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbKMeansAttributesLabelMapFilter.txx"
#endif

#endif


