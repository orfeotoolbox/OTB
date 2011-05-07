/*
 * CovPixelSelectFilter.h
 *
 * Vahine Framework
 * Copyright (C) 2008, 2009 Ludovic Léau-Mercier and Laboratoire de Planétologie de Grenoble
 * See LICENCE and COPYING for details.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * CeCILLl License for more details and http://www.cecill.info
 *
 */

#ifndef __VAHINEFILTRAGEFILTER_H_
#define __VAHINEFILTRAGEFILTER_H_

#include <otb/Utilities/ITK/Utilities/vxl/core/vnl/algo/vnl_symmetric_eigensystem.h>
#include <otb/Utilities/ITK/Utilities/vxl/core/vnl/algo/vnl_real_eigensystem.h>

#include <otb/IO/otbImage.h>
#include <otb/IO/otbVectorImage.h>
#include <otb/Utilities/ITK/Common/itkImageToImageFilter.h>

#include <otb/BasicFilters/otbStreamingStatisticsVectorImageFilter.h>
#include <otb/BasicFilters/otbImportImageFilter.h>
#include "itkVariableSizeMatrix.h"
#include "itkArray.h"
#include <otb/Utilities/ITK/Common/itkVector.h>
#include <otb/Utilities/ITK/Numerics/Statistics/itkListSample.h>
#include <otb/Utilities/ITK/Numerics/Statistics/itkGaussianMixtureModelComponent.h>
#include <otb/Utilities/ITK/Numerics/Statistics/itkExpectationMaximizationMixtureModelEstimator.h>

#include "VahineMacro.h"

namespace otb {

template <class TInputImage, class TOutputImage>
class ITK_EXPORT VahineCovPixelSelectFilter
  : public itk::ImageToImageFilter<TInputImage, TOutputImage> {
public:
  typedef TInputImage  InputImageType;
  typedef TOutputImage OutputImageType;

  typedef VahineCovPixelSelectFilter                               Self;
  typedef itk::ImageToImageFilter<InputImageType, OutputImageType> Superclass;
  typedef itk::SmartPointer<Self>                                  Pointer;
  typedef itk::SmartPointer<const Self>                            ConstPointer;

  // creation of SmartPointer
  itkNewMacro(Self);
  // runtime type information
  itkTypeMacro(VahineCovPixelSelectFilter, itk::ImageToImageFilter);

  // Input Image
  typedef typename InputImageType::Pointer                         ImagePointer;
  typedef typename InputImageType::PixelType                       PixelType;
  typedef typename InputImageType::RegionType                      RegionType;
  typedef typename InputImageType::InternalPixelType               InternalPixelType;
  typedef typename itk::NumericTraits<InternalPixelType>::RealType RealType;
  typedef typename itk::ImageRegionConstIterator<InputImageType>   ImageRegionConstIterator;

  // Output Image
  typedef typename OutputImageType::Pointer                  OutputImagePointer;
  typedef typename OutputImageType::PixelType                OutputPixelType;
  typedef typename OutputImageType::IndexType                OutputIndexType;
  typedef typename OutputImageType::SizeType                 OutputSizeType;
  typedef typename OutputImageType::RegionType               OutputRegionType;
  typedef typename itk::ImageRegionIterator<OutputImageType> OutputRegionIterator;

  typedef StreamingStatisticsVectorImageFilter<InputImageType> StatsFilterType;
  typedef typename StatsFilterType::Pointer                    StatsFilterPointer;

  typedef typename itk::VariableSizeMatrix<RealType> MatrixType;
  typedef itk::Array<double>                         ItkArrayType;
  typedef vnl_vector<RealType>                       ArrayType;
  typedef typename std::vector<MatrixType>           ArrayMatrixType;

  typedef itk::Vector<double, 1>                                                    MeasurementVectorType;
  typedef itk::Statistics::ListSample<MeasurementVectorType>                        SampleType;
  typedef itk::Statistics::GaussianMixtureModelComponent<SampleType>                ComponentType;
  typedef itk::Statistics::ExpectationMaximizationMixtureModelEstimator<SampleType> EstimatorType;

  bool GetDebug(){
    return true;
  };

  itkSetMacro( ReferenceImage, ImagePointer );
  itkGetMacro( PCAVectors, MatrixType );
  itkGetMacro( PCAValues, ArrayType*);
  itkSetMacro( Explanation, double );
  itkGetMacro( Explanation, double );
  itkGetMacro( NumberOfPCAAxes, unsigned int );
  itkGetMacro( PCAReferenceProjections, ArrayMatrixType* );
  itkGetMacro( PCAInputProjections, ArrayMatrixType* );
  itkGetMacro( Distances, MatrixType* );
  itkGetMacro( GMMMean, ItkArrayType );
  itkGetMacro( GMMCovariance, ItkArrayType );
  itkGetMacro( GMMProportion, ItkArrayType );
  itkSetMacro( BoundDistance, ItkArrayType );
  itkSetMacro( DistanceWeight, ItkArrayType );
protected:
  VahineCovPixelSelectFilter();
  virtual ~VahineCovPixelSelectFilter();
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  void GenerateData();

  unsigned int m_NumberOfPCAAxes;
  virtual void CalculatePCAVectors();

  virtual void CalculateNNS();

  virtual void UpdateNumberOfPCAAxes();

  virtual void CalculatePCAProjection(const unsigned int ImageNumberOfBands, ImagePointer image,
                                      ArrayMatrixType *matrix);

  virtual void ClassificationEM();

  virtual void WeightFilter();

private:
  ImagePointer m_ReferenceImage;
  MatrixType m_PCAVectors;
  ArrayType* m_PCAValues;
  /* pourcent of explanation */
  double m_Explanation;

  ArrayMatrixType* m_PCAReferenceProjections;
  ArrayMatrixType* m_PCAInputProjections;
  MatrixType* m_Distances;
  ItkArrayType m_GMMMean;
  ItkArrayType m_GMMCovariance;
  ItkArrayType m_GMMProportion;
  ItkArrayType m_BoundDistance;
  ItkArrayType m_DistanceWeight;

  VahineCovPixelSelectFilter(const Self&); //not implemented
  void operator=(const Self&);             //not implemented

};

} // end namespace otb

#include "CovPixelSelectFilter.txx"

#endif /* VAHINEFILTRAGEFILTER_H_ */