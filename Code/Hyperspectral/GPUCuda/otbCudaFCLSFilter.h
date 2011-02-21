/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANT2ABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbCudaFCLSFilter_h
#define __otbCudaFCLSFilter_h

#include "itkMacro.h"
#include "otbUnaryFunctorImageFilter.h"
#include "vnl/algo/vnl_svd.h"

namespace otb
{
namespace Functor
{
template <class TInputPixel, class TOutputPixel>
class Dummy
{
public:

  typedef vnl_vector<float> VectorType;
  typedef vnl_matrix<float> MatrixType;

  Dummy(){}
  virtual ~Dummy(){}

  void SetMatrix(const MatrixType& U)
  {
    m_U = U;
    m_Ut = m_U.transpose();
    m_Uinv = vnl_svd<float>(m_U).inverse();
  }

  unsigned int GetOutputSize() const
  {
    return m_U.cols();
  }

  const MatrixType& GetMatrix() const
  {
    return m_U;
  }

  const MatrixType& GetMatrixT() const
  {
    return m_Ut;
  }

  const MatrixType& GetMatrixInv() const
  {
    return m_Uinv;
  }

  bool operator !=(const Dummy&) const
  {
    return true;
  }
  bool operator ==(const Dummy& other) const
  {
    return !(*this != other);
  }

  inline TOutputPixel operator ()(const TInputPixel& A) const
  {
    TOutputPixel B;
    return static_cast<TOutputPixel>(B);
  }

private:
  MatrixType m_U;
  MatrixType m_Ut;
  MatrixType m_Uinv;
};
}


template<class TInputImage, class TOutputImage>
class ITK_EXPORT CudaFCLSFilter: public otb::UnaryFunctorImageFilter<TInputImage, TOutputImage, Functor::Dummy<
    typename TInputImage::PixelType, typename TOutputImage::PixelType> >
{
public:
  /** Standard class typedefs. */
  typedef CudaFCLSFilter Self;
  typedef itk::UnaryFunctorImageFilter<TInputImage, TOutputImage,Functor::Dummy<
  typename TInputImage::PixelType, typename TOutputImage::PixelType> > Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(CudaFCLSFilter, UnaryFunctorImageFilter);

  typedef TInputImage  InputImageType;
  typedef TOutputImage OutputImageType;

  typedef itk::ImageRegionConstIterator<InputImageType> InputImageConstIterator;
  typedef itk::ImageRegionIterator<InputImageType> InputImageIterator;

  typedef itk::ImageRegionConstIterator<OutputImageType> OutputImageConstIterator;
  typedef itk::ImageRegionIterator<OutputImageType> OutputImageIterator;
  typedef typename  Superclass::OutputImageRegionType   OutputImageRegionType;

  typedef typename OutputImageType::RegionType RegionType;
  typedef typename OutputImageType::SizeType   SizeType;

  typedef Functor::Dummy<
      typename TInputImage::PixelType,
      typename TOutputImage::PixelType
      > FunctorType;

  typedef typename FunctorType::VectorType VectorType;
  typedef typename FunctorType::MatrixType MatrixType;

  itkSetMacro(BlockSize, int);
  itkGetMacro(BlockSize, int);

  itkSetMacro(MaxIter, int);
  itkGetMacro(MaxIter, int);

  void SetMatrix(const MatrixType& U)
  {
    this->GetFunctor().SetMatrix(U);
    this->Modified();
  }

protected:
  /// Constructor
  CudaFCLSFilter();

  /// Destructor
  virtual ~CudaFCLSFilter() {}

  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                            int threadId );

private:
  CudaFCLSFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  int m_BlockSize;
  int m_MaxIter;

  float*   m_GPUInputImage;
  float*   m_GPUOutputImage;
  SizeType m_GPUImageSize;

  float*   m_GPUU;
  float*   m_GPUUt;
  float*   m_GPUUinv;

};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbCudaFCLSFilter.txx"
#endif

#endif
