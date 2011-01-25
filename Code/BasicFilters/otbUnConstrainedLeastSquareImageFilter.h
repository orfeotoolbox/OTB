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
#ifndef __otbUnConstrainedLeastSquareImageFilter_h
#define __otbUnConstrainedLeastSquareImageFilter_h

#include "otbUnaryFunctorImageFilter.h"
#include "vnl/algo/vnl_matrix_inverse.h"


namespace otb
{


namespace Functor {

/** \class UnConstrainedLeastSquareFunctor
 *
 * \brief TODO
 *
 */
template<class TInput, class TOutput, class TPrecision>
class UnConstrainedLeastSquareFunctor
{
public:
  typedef TInput     InputType;
  typedef TOutput    OutputType;
  typedef TPrecision PrecisionType;

  typedef vnl_vector<PrecisionType> VectorType;
  typedef vnl_matrix<PrecisionType> MatrixType;

  UnConstrainedLeastSquareFunctor() {}
  virtual ~UnConstrainedLeastSquareFunctor() {}

  unsigned int GetOutputSize()
  {
    return m_Matrix.rows();
  }

  bool operator !=(const UnConstrainedLeastSquareFunctor& other) const
  {
    return false;
  }

  bool operator ==(const UnConstrainedLeastSquareFunctor& other) const
  {
    return !(*this != other);
  }

  const MatrixType& GetMatrix()
  {
    return m_Matrix;
  }

  void SetMatrix(const MatrixType& m)
  {
    m_Matrix = m;
    m_MatrixPseudoInverse = vnl_matrix_inverse<PrecisionType>(m);
  }

  OutputType operator ()(const InputType& in)
  {
    // TODO : support different types !!!
    VectorType inVector(in.GetDataPointer(), in.Size());
    VectorType outVector = m_MatrixPseudoInverse * inVector;
    return OutputType(outVector.data_block(), m_MatrixPseudoInverse.rows());
  }

private:
  MatrixType m_Matrix;
  MatrixType m_MatrixPseudoInverse;
};
}

/** \class UnConstrainedLeastSquareImageFilter
 *
 * \brief Solves a least square system for each pixel
 *
 * This filter takes as input a multiband image and a matrix.
 * If the matrix is called $A$, it solves, for each pixel $p$, the system
 * $A \cdot x = p$ in the least square sense, and saves the result $\hat{x}$
 * in the output pixel.
 *
 * It can be used as a simple way to unmix an hyperspectral dataset,
 * where $A$ is the matrix in which each row corresponds to an endmember signature,
 * although better algorithm can be found for this particular task.
 *
 * The number of rows in $A$ must match the input image number of bands.
 * The number of bands in the output image will be the number of columns of $A$
 *
 * \ingroup Streamed
 * \ingroup Threaded
 */
template <class TInputImage, class TOutputImage, class TPrecision>
class ITK_EXPORT UnConstrainedLeastSquareImageFilter :
  public otb::UnaryFunctorImageFilter<TInputImage, TOutputImage,
      Functor::UnConstrainedLeastSquareFunctor<typename TInputImage::PixelType,
          typename TOutputImage::PixelType, TPrecision> >
{
public:
  /** Standard class typedefs. */
  typedef UnConstrainedLeastSquareImageFilter Self;
  typedef itk::UnaryFunctorImageFilter
     <TInputImage,
      TOutputImage,
      Functor::UnConstrainedLeastSquareFunctor<
          typename TInputImage::PixelType,
          typename TOutputImage::PixelType,
          TPrecision>
     >                                 Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef Functor::UnConstrainedLeastSquareFunctor<
      typename TInputImage::PixelType,
      typename TOutputImage::PixelType,
      TPrecision> FunctorType;
  typedef typename FunctorType::MatrixType MatrixType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(UnConstrainedLeastSquareImageFilter, otb::UnaryFunctorImageFilter);

  /** Pixel types. */
  typedef typename TInputImage::PixelType  InputPixelType;
  typedef typename TOutputImage::PixelType OutputPixelType;

  const MatrixType& GetMatrix()
  {
    return this->GetFunctor().GetMatrix();
  }

  void SetMatrix(const MatrixType& m)
  {
    this->GetFunctor().SetMatrix(m);
    this->Modified();
  }

protected:
  UnConstrainedLeastSquareImageFilter();

  virtual ~UnConstrainedLeastSquareImageFilter() {}

  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  UnConstrainedLeastSquareImageFilter(const Self &); //purposely not implemented

  void operator =(const Self&); //purposely not implemented

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbUnConstrainedLeastSquareImageFilter.txx"
#endif

#endif

