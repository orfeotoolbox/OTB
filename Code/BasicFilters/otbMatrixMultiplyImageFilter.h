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
#ifndef __otbMatrixMultiplyImageFilter_h
#define __otbMatrixMultiplyImageFilter_h

#include "otbUnaryFunctorImageFilter.h"

namespace otb
{


namespace Functor {

/** \class MatrixMultiplyFunctor
 *
 * \brief Apply a matrix multiplication to a VariableLengthVector
 *
 */
template<class TInput, class TOutput, class TPrecision>
class MatrixMultiplyFunctor
{
public:
  typedef TInput     InputType;
  typedef TOutput    OutputType;
  typedef TPrecision PrecisionType;

  typedef vnl_vector<PrecisionType> VectorType;
  typedef vnl_matrix<PrecisionType> MatrixType;

  MatrixMultiplyFunctor() {}
  virtual ~MatrixMultiplyFunctor() {}

  unsigned int GetOutputSize()
  {
    return m_Matrix.rows();
  }

  bool operator !=(const MatrixMultiplyFunctor& other) const
  {
    return false;
  }

  bool operator ==(const MatrixMultiplyFunctor& other) const
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
  }

  OutputType operator ()(const InputType& in)
  {
    // TODO : support different types !!!
    VectorType inVector(in.GetDataPointer(), in.Size());
    VectorType outVector = m_Matrix * inVector;
    return OutputType(outVector.data_block(), outVector.size());
  }

private:
  MatrixType m_Matrix;
};
}

/** \class MatrixMultiplyImageFilter
 *
 * \brief Apply a matrix multiplication to a VectorImage
 *
 * \ingroup Streamed
 * \ingroup Threaded
 */
template <class TInputImage, class TOutputImage, class TPrecision>
class ITK_EXPORT MatrixMultiplyImageFilter :
  public otb::UnaryFunctorImageFilter<TInputImage, TOutputImage,
      Functor::MatrixMultiplyFunctor<typename TInputImage::PixelType,
          typename TOutputImage::PixelType, TPrecision> >
{
public:
  /** Standard class typedefs. */
  typedef MatrixMultiplyImageFilter Self;
  typedef otb::UnaryFunctorImageFilter
     <TInputImage,
      TOutputImage,
      Functor::MatrixMultiplyFunctor<
          typename TInputImage::PixelType,
          typename TOutputImage::PixelType,
          TPrecision>
     >                                 Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef Functor::MatrixMultiplyFunctor<
      typename TInputImage::PixelType,
      typename TOutputImage::PixelType,
      TPrecision> FunctorType;
  typedef typename FunctorType::MatrixType MatrixType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(MatrixMultiplyImageFilter, itk::UnaryFunctorImageFilter);

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
  MatrixMultiplyImageFilter();

  virtual ~MatrixMultiplyImageFilter() {}

  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  MatrixMultiplyImageFilter(const Self &); //purposely not implemented

  void operator =(const Self&); //purposely not implemented

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMatrixMultiplyImageFilter.txx"
#endif

#endif

