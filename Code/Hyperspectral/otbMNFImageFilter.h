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
#include "otbStreamingStatisticsVectorImageFilter2.h"
#include "otbMatrixMultiplyImageFilter.h"
#include "otbPCAImageFilter.h"

namespace otb {

/** \class MNFImageFilter
 * \brief Performs a Minimum Noise Fraction analysis of a vector image.
 *
 * The internal structure of this filter is a filter-to-filter like structure. 
 * The estimation of the covariance matrix has persistent capabilities...
 *
 * The high pass filter which has to be used for the noise estimation is templated
 * for a better scalability...
 *
 * TODO? Utiliser une 2e entree pour donner directement une image de bruit ??
 *
 * \sa otbStreamingStatisticsVectorImageFilter2
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
  itkStaticConstMacro(DirectionOfTransformation,TransformDirectionEnumType,TDirectionOfTransformation);

  /** Template parameters typedefs */
  typedef TInputImage  InputImageType;
  typedef TOutputImage OutputImageType;

  /** Filter types and related */
  typedef PCAImageFilter< InputImageType, OutputImageType, TDirectionOfTransformation > PCAImageFilterType;
  typedef typename PCAImageFilterType::Pointer PCAImageFilterPointerType;

  typedef typename PCAImageFilterType::CovarianceEstimatorFilterType CovarianceEstimatorFilterType;
  typedef typename PCAImageFilterType::CovarianceEstimatorFilterPointerType CovarianceEstimatorFilterPointerType;

  typedef typename PCAImageFilterType::TransformFilterType TransformFilterType;
  typedef typename PCAImageFilterType::TransformFilterPointerType TransformFilterPointerType;

  typedef TNoiseImageFilter NoiseImageFilterType;
  typedef typename NoiseImageFilterType::Pointer NoiseImageFilterPointerType;

  typedef typename PCAImageFilterType::RealType RealType;
  typedef typename PCAImageFilterType::VectorType VectorType;
  typedef typename PCAImageFilterType::MatrixType MatrixType;

  /** 
   * Set/Get the number of required largest principal components. 
   * The noise removal is not concerned by this part, only the PCA part...
   */
  void SetNumberOfPrincipalComponentsRequired ( unsigned int num )
  {
    this->GetPCAImageFilter()->SetNumberOfPrincipalComponentsRequired( num );
  }

  unsigned int GetNumberOfPrincipalComponentsRequired () const
  {
    return this->GetPCAImageFilter()->GetNumberOfPrincipalComponentsRequired();
  }

  itkGetConstMacro(PCAImageFilter, PCAImageFilterType *);
  itkGetMacro(PCAImageFilter, PCAImageFilterType *);
  itkGetMacro(NoiseCovarianceEstimator, CovarianceEstimatorFilterType *);
  itkGetMacro(Transformer, TransformFilterType *);
  itkGetMacro(NoiseImageFilter, NoiseImageFilterType *);

  MatrixType GetCovarianceMatrix () const
  {
    return this->GetPCAImageFilter()->GetGovarianceMatrix();
  }
  
  void SetCovarianceMatrix ( const MatrixType & cov )
  {
    this->GetPCAImageFilter()->SetCovarianceMatrix();
  }

  itkGetMacro(NoiseCovarianceMatrix, MatrixType);

  void SetNoiseCovarianceMatrix ( const MatrixType & mat )
  {
    m_NoiseCovarianceMatrix = mat;
    m_GivenNoiseCovarianceMatrix = true;
  }

  MatrixType GetTransformationMatrix () const
  {
    return this->GetPCAImageFilter()->GetTransformationMatrix();
  }

  void SetTransformationMatrix( const MatrixType & transf, bool isForward = true )
  {
    return this->GetPCAImageFilter()->SetTransformationMatrix( transf, isForward );
  }

  /** If the NoiseTransformation is given, the NoiseImageFilter is useless */
  itkGetMacro( NoiseTransformationMatrix, MatrixType );
  void SetNoiseTransformationMatrix ( const MatrixType & mat )
  {
    m_NoiseTransformationMatrix = mat;
    m_GivenNoiseTransformationMatrix = true;
  }

  itkGetConstMacro(NoiseRatioValues,VectorType);

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

  void GetTransformationMatrixFromCovarianceMatrix();

  /** Internal attributes */
  bool m_GivenNoiseCovarianceMatrix;
  bool m_GivenNoiseTransformationMatrix;

  MatrixType m_NoiseCovarianceMatrix;
  MatrixType m_NoiseTransformationMatrix;
  VectorType m_NoiseRatioValues;

  NoiseImageFilterPointerType m_NoiseImageFilter;
  PCAImageFilterPointerType m_PCAImageFilter;
  CovarianceEstimatorFilterPointerType m_NoiseCovarianceEstimator;
  TransformFilterPointerType m_Transformer;
}; // end of class

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMNFImageFilter.txx"
#endif


#endif



