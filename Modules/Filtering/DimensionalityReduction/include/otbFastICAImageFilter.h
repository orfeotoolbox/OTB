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
#ifndef __otbFastICAImageFilter_h
#define __otbFastICAImageFilter_h

#include "itkImageToImageFilter.h"
#include "otbPCAImageFilter.h"
#include "otbFastICAInternalOptimizerVectorImageFilter.h"

namespace otb
{
/** \class FastICAImageFilter
 * \brief Performs a Independent Component Analysis
 *
 * The internal structure of this filter is a filter-to-filter like structure.
 * The estimation of the covariance matrix has persistent capabilities...
 *
 * \sa PCAImageFilter
 */
template <class TInputImage, class TOutputImage, Transform::TransformDirection TDirectionOfTransformation >
class ITK_EXPORT FastICAImageFilter
  : public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard typedefs */
  typedef FastICAImageFilter                                 Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                            Pointer;
  typedef itk::SmartPointer<const Self>                      ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(FastICAImageFilter, ImageToImageFilter);

  /** Dimension */
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int, TOutputImage::ImageDimension);

  typedef Transform::TransformDirection TransformDirectionEnumType;
  itkStaticConstMacro(DirectionOfTransformation, TransformDirectionEnumType, TDirectionOfTransformation);

  /** typedefs */
  typedef TInputImage InputImageType;
  typedef TOutputImage OutputImageType;

  typedef PCAImageFilter< InputImageType, OutputImageType, TDirectionOfTransformation >
      PCAFilterType;
  typedef typename PCAFilterType::Pointer PCAFilterPointerType;

  typedef typename PCAFilterType::RealType RealType;
  typedef typename PCAFilterType::VectorType VectorType;
  typedef typename PCAFilterType::MatrixType MatrixType;
  typedef typename MatrixType::InternalMatrixType InternalMatrixType;
  typedef typename InternalMatrixType::element_type MatrixElementType;

  typedef MatrixImageFilter< TInputImage, TOutputImage > TransformFilterType;
  typedef typename TransformFilterType::Pointer TransformFilterPointerType;

  typedef FastICAInternalOptimizerVectorImageFilter< InputImageType, InputImageType >
      InternalOptimizerType;
  typedef typename InternalOptimizerType::Pointer InternalOptimizerPointerType;

  typedef StreamingStatisticsVectorImageFilter< InputImageType > MeanEstimatorFilterType;
  typedef typename MeanEstimatorFilterType::Pointer MeanEstimatorFilterPointerType;

  typedef double (*ContrastFunctionType) ( double );

  /**
   * Set/Get the number of required largest principal components.
   */
  itkGetMacro(NumberOfPrincipalComponentsRequired, unsigned int);
  itkSetMacro(NumberOfPrincipalComponentsRequired, unsigned int);

  itkGetConstMacro(PCAFilter, PCAFilterType *);
  itkGetMacro(PCAFilter, PCAFilterType *);

  itkGetConstMacro(TransformFilter, TransformFilterType *);
  itkGetMacro(TransformFilter, TransformFilterType *);

  VectorType GetMeanValues () const
  {
    return this->GetPCAFilter()->GetMeanValues();
  }
  void SetMeanValues ( const VectorType & vec )
  {
    m_PCAFilter->SetMeanValues(vec);
  }

  VectorType GetStdDevValues ( ) const
  {
    return this->GetPCAFilter()->GetStdDevValues();
  }
  void SetStdDevValues ( const VectorType & vec )
  {
    m_PCAFilter->SetStdDevValues(vec);
  }

  MatrixType GetPCATransformationMatrix () const
  {
    return this->GetPCAFilter()->GetTransformationMatrix();
  }
  void SetPCATransformationMatrix ( const MatrixType & mat, bool isForward = true )
  {
    m_PCAFilter->SetTransformationMatrix(mat, isForward);
  }

  itkGetConstMacro(TransformationMatrix, MatrixType);
  itkGetMacro(TransformationMatrix, MatrixType);
  void SetTransformationMatrix ( const MatrixType & mat, bool isForward = true )
  {
    m_IsTransformationForward = isForward;
    m_GivenTransformationMatrix = true;
    m_TransformationMatrix = mat;
    this->Modified();
  }

  itkGetMacro(NumberOfIterations, unsigned int);
  itkSetMacro(NumberOfIterations, unsigned int);

  itkGetMacro(ConvergenceThreshold, double);
  itkSetMacro(ConvergenceThreshold, double);

  itkGetMacro(ContrastFunction, ContrastFunctionType);

  itkGetMacro(Mu, double);
  itkSetMacro(Mu, double);

protected:
  FastICAImageFilter ();
  virtual ~FastICAImageFilter() { }

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

  /** Internal methods */
  virtual void ForwardGenerateData();
  virtual void ReverseGenerateData();

  /** this is the specific part of FastICA */
  virtual void GenerateTransformationMatrix();

  unsigned int m_NumberOfPrincipalComponentsRequired;

  /** Transformation matrix refers to the ICA step (not PCA) */
  bool m_GivenTransformationMatrix;
  bool m_IsTransformationForward;
  MatrixType m_TransformationMatrix;

  /** FastICA parameters */
  unsigned int m_NumberOfIterations; // def is 50
  double m_ConvergenceThreshold; // def is 1e-4
  ContrastFunctionType m_ContrastFunction; // see g() function in the biblio. Def is tanh
  double m_Mu; // def is 1. in [0, 1]

  PCAFilterPointerType m_PCAFilter;
  TransformFilterPointerType m_TransformFilter;

private:
  FastICAImageFilter( const Self & ); //purposely not implemented
  void operator =( const Self & ); //purposely not implemented
}; // end of class

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbFastICAImageFilter.txx"
#endif


#endif


