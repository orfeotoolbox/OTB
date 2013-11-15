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
#ifndef __otbMNFImageFilter_h
#define __otbMNFImageFilter_h

#include "otbMacro.h"
#include "itkImageToImageFilter.h"
#include "otbStreamingStatisticsVectorImageFilter.h"
#include "otbMatrixImageFilter.h"
#include "otbNormalizeVectorImageFilter.h"
#include "otbPCAImageFilter.h"


namespace otb {

/** \class MNFImageFilter
 * \brief Performs a Maximum Noise Fraction analysis of a vector image.
 *
 * The internal structure of this filter is a filter-to-filter like structure.
 * The estimation of the covariance matrix is streamed
 *
 * The high pass filter which has to be used for the noise estimation is templated
 * for a better scalability.
 *
 * TODO? Utiliser une 2e entree pour donner directement une image de bruit ?
 *
 * \sa otbStreamingStatisticsVectorImageFilter
 * \sa PCAImageFiler
 */
template <class TInputImage, class TOutputImage,
            class TNoiseImageFilter,
            Transform::TransformDirection TDirectionOfTransformation >
class ITK_EXPORT MNFImageFilter
  : public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard typedefs */
  typedef MNFImageFilter                                     Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                            Pointer;
  typedef itk::SmartPointer<const Self>                      ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(MNFImageFilter, ImageToImageFilter);

  /** Dimension */
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int, TOutputImage::ImageDimension);

  typedef Transform::TransformDirection TransformDirectionEnumType;
  itkStaticConstMacro(DirectionOfTransformation, TransformDirectionEnumType, TDirectionOfTransformation);

  /** Template parameters typedefs */
  typedef TInputImage  InputImageType;
  typedef TOutputImage OutputImageType;

  /** Filter types and related */
  typedef StreamingStatisticsVectorImageFilter< InputImageType > CovarianceEstimatorFilterType;
  typedef typename CovarianceEstimatorFilterType::Pointer CovarianceEstimatorFilterPointerType;

  typedef typename CovarianceEstimatorFilterType::RealType RealType;
  typedef typename CovarianceEstimatorFilterType::RealPixelType VectorType;
  typedef typename CovarianceEstimatorFilterType::MatrixObjectType MatrixObjectType;
  typedef typename MatrixObjectType::ComponentType MatrixType;
  typedef typename MatrixType::InternalMatrixType InternalMatrixType;
  typedef typename InternalMatrixType::element_type MatrixElementType;

  typedef MatrixImageFilter< InputImageType, OutputImageType > TransformFilterType;
  typedef typename TransformFilterType::Pointer TransformFilterPointerType;

  typedef TNoiseImageFilter NoiseImageFilterType;
  typedef typename NoiseImageFilterType::Pointer NoiseImageFilterPointerType;

  typedef NormalizeVectorImageFilter< InputImageType, OutputImageType > NormalizeFilterType;
  typedef typename NormalizeFilterType::Pointer NormalizeFilterPointerType;

  /**
   * Set/Get the number of required largest principal components.
   */
  itkGetMacro(NumberOfPrincipalComponentsRequired, unsigned int);
  itkSetMacro(NumberOfPrincipalComponentsRequired, unsigned int);

  itkGetConstMacro(Normalizer, NormalizeFilterType*);
  itkGetMacro(Normalizer, NormalizeFilterType*);
  itkGetMacro(NoiseCovarianceEstimator, CovarianceEstimatorFilterType *);
  itkGetMacro(Transformer, TransformFilterType *);
  itkGetMacro(NoiseImageFilter, NoiseImageFilterType *);

  /** Normalization only impact the use of variance. The data is always centered */
  itkGetMacro(UseNormalization, bool);
  itkSetMacro(UseNormalization, bool);

  itkGetConstMacro(MeanValues, VectorType);
  void SetMeanValues ( const VectorType & vec )
  {
    m_MeanValues = vec;
    m_GivenMeanValues = true;
  }

  itkGetConstMacro(StdDevValues, VectorType);
  void SetStdDevValues ( const VectorType & vec )
  {
    m_StdDevValues = vec;
    m_UseNormalization = true;
    m_GivenStdDevValues = true;
  }

  itkGetConstMacro(CovarianceMatrix, MatrixType);
  void SetCovarianceMatrix ( const MatrixType & cov )
  {
    m_CovarianceMatrix = cov;
    m_GivenCovarianceMatrix = true;
  }

  itkGetConstMacro(NoiseCovarianceMatrix, MatrixType);
  void SetNoiseCovarianceMatrix ( const MatrixType & mat )
  {
    m_NoiseCovarianceMatrix = mat;
    m_GivenNoiseCovarianceMatrix = true;
  }

  itkGetConstMacro(TransformationMatrix, MatrixType);
  void SetTransformationMatrix( const MatrixType & transf, bool isForward = true )
  {
    m_TransformationMatrix = transf;
    m_GivenTransformationMatrix = true;
    m_IsTransformationMatrixForward = isForward;
  }

  itkGetConstMacro(EigenValues, VectorType);

protected:
  MNFImageFilter();
  virtual ~MNFImageFilter() { }

  /** GenerateOutputInformation
   * Propagate vector length info and modify if needed
   * NumberOfPrincipalComponentsRequired
   *
   * In REVERSE mode, the covariance matrix or the transformation matrix
   * (which may not be square) has to be given,
   * otherwize, GenerateOutputInformation throws an itk::ExceptionObject
   */
  virtual void GenerateOutputInformation();

  /** GenerateData
   * Through a filter of filter structure
   */
  virtual void GenerateData ();

  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** Internal methods */
  virtual void ForwardGenerateData();
  virtual void ReverseGenerateData();

  /** Specific functionality of MNF */
  virtual void GenerateTransformationMatrix();

  /** Internal attributes */
  unsigned int m_NumberOfPrincipalComponentsRequired;

  bool m_UseNormalization;
  bool m_GivenMeanValues;
  bool m_GivenStdDevValues;

  bool m_GivenCovarianceMatrix;
  bool m_GivenNoiseCovarianceMatrix;
  bool m_GivenTransformationMatrix;
  bool m_IsTransformationMatrixForward;

  VectorType m_MeanValues;
  VectorType m_StdDevValues;
  MatrixType m_CovarianceMatrix;
  MatrixType m_NoiseCovarianceMatrix;
  MatrixType m_TransformationMatrix;
  VectorType m_EigenValues;

  NormalizeFilterPointerType m_Normalizer;
  NoiseImageFilterPointerType m_NoiseImageFilter;
  CovarianceEstimatorFilterPointerType m_CovarianceEstimator;
  CovarianceEstimatorFilterPointerType m_NoiseCovarianceEstimator;
  TransformFilterPointerType m_Transformer;

private:
  MNFImageFilter( const Self & ); // not implemented
  void operator=( const Self & ); // not implemented
}; // end of class

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMNFImageFilter.txx"
#endif


#endif


