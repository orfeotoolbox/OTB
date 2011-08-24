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
#ifndef __otbMultivariateAlterationDetectorImageFilter_h
#define __otbMultivariateAlterationDetectorImageFilter_h

#include "itkImageToImageFilter.h"

#include "otbStreamingStatisticsVectorImageFilter.h"
#include "otbConcatenateVectorImageFilter.h"

#include "vnl/vnl_vector.h"
#include "vnl/vnl_matrix.h"

namespace otb
{

/** \class MultivariateAlterationDetectorImageFilter
 * \brief This filter implements the Multivariate Alteration Detector 
 * 
 * TODO: document me
 *
 * \ingroup Multithreaded
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT MultivariateAlterationDetectorImageFilter 
  : public itk::ImageToImageFilter<TInputImage,TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef MultivariateAlterationDetectorImageFilter           Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage>  Superclass;
  typedef itk::SmartPointer<Self>                             Pointer;
  typedef itk::SmartPointer<const Self>                       ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(MultivariateAlterationDetectorImageFilter, ImageToImageFilter);

  /** Some convenient typedefs. */
  typedef TInputImage                            InputImageType;
  typedef typename InputImageType::Pointer      InputImagePointer;
  typedef typename InputImageType::ConstPointer InputImageConstPointer;
  typedef typename InputImageType::RegionType   InputImageRegionType;
  typedef typename InputImageType::PixelType     InputImagePixelType;
  typedef TOutputImage                           OutputImageType;
  typedef typename OutputImageType::Pointer      OutputImagePointer;
  typedef typename OutputImageType::RegionType   OutputImageRegionType;
  typedef typename OutputImageType::PixelType    OutputImagePixelType;

  /** Internal filters types */
  typedef StreamingStatisticsVectorImageFilter<InputImageType> CovarianceEstimatorType;
  typedef typename CovarianceEstimatorType::Pointer            CovarianceEstimatorPointer;
  typedef otb::ConcatenateVectorImageFilter<InputImageType,InputImageType,InputImageType> ConcatenateImageFilterType;
  typedef typename ConcatenateImageFilterType::Pointer         ConcatenateImageFilterPointer;

  typedef typename CovarianceEstimatorType::MatrixObjectType   MatrixObjectType;
  typedef typename MatrixObjectType::ComponentType             MatrixType;
  typedef typename MatrixType::InternalMatrixType              InternalMatrixType;
  typedef typename CovarianceEstimatorType::RealPixelType      VectorType;

  typedef typename VectorType::ValueType                       RealType;

  typedef vnl_vector<RealType>                                 VnlVectorType;
  typedef vnl_matrix<RealType>                                 VnlMatrixType;

  
  /** Connect one of the operands for pixel-wise addition */
  void SetInput1(const TInputImage * image1);

  /** Connect one of the operands for pixel-wise addition */
  void SetInput2(const TInputImage * image2);

  /** Get the inputs */
  const TInputImage * GetInput1();
  const TInputImage * GetInput2();

protected:
  MultivariateAlterationDetectorImageFilter();
  virtual ~MultivariateAlterationDetectorImageFilter() {}

  virtual void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,int threadId);

  virtual void GenerateOutputInformation();

private:
  MultivariateAlterationDetectorImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  static RealType InverseValue(const RealType & value)
  {
    if(value != 0)
      return 1/value;
    else
      return 0.; // ugly
  }
  
  static RealType SignOfValue(const RealType & value)
  {
    if(value==0)
      return value;
    else
      return (value > 0 ? 1. : -1.);
  }

  CovarianceEstimatorPointer m_CovarianceEstimator;
  MatrixType                 m_CovarianceMatrix;
  VectorType                 m_MeanValues;

  VnlMatrixType              m_V1;
  VnlMatrixType              m_V2;
  VnlVectorType              m_Mean1;
  VnlVectorType              m_Mean2;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMultivariateAlterationDetectorImageFilter.txx"
#endif

#endif
