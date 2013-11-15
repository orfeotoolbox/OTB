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

#include "itkMacro.h"
#include "otbUnaryFunctorImageFilter.h"
#include "vnl/algo/vnl_svd.h"
#include <boost/shared_ptr.hpp>

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

  UnConstrainedLeastSquareFunctor() : m_OutputSize(0) {}
  virtual ~UnConstrainedLeastSquareFunctor() {}

  unsigned int GetOutputSize() const
  {
    return m_OutputSize;
  }

  bool operator != (const UnConstrainedLeastSquareFunctor& other) const
  {
    return true;
  }

  bool operator == (const UnConstrainedLeastSquareFunctor& other) const
  {
    return !(*this != other);
  }

  void SetMatrix(const MatrixType& m)
  {
    //std::cout << "m : " << m.rows() << " " << m.cols() << std::endl;
    m_Svd.reset( new SVDType(m) );
    m_Inv = m_Svd->inverse();
    m_OutputSize = m.cols();
  }

  OutputType operator ()(const InputType& in) const
  {
    // TODO : support different types between input and output ?
    VectorType inVector(in.Size());
    for (unsigned int i = 0; i < in.GetSize(); ++i )
      {
      inVector[i] = in[i];
      }

    VectorType outVector = m_Inv * inVector;

    OutputType out(outVector.size());
    for (unsigned int i = 0; i < out.GetSize(); ++i )
      {
      out[i] = outVector[i];
      }

    return out;
  }

private:

  typedef vnl_svd<PrecisionType>     SVDType;
  typedef boost::shared_ptr<SVDType> SVDPointerType;

  unsigned int   m_OutputSize;
  SVDPointerType m_Svd;
  MatrixType     m_Inv;
};
}

/** \class UnConstrainedLeastSquareImageFilter
 *
 * \brief Solves a least square system for each pixel
 *
 * This filter takes as input a multiband image and a matrix.
 * If the matrix is called $A$, it solves, for each pixel $p$, the system
 * \f$A \cdot x = p\f$ in the least square sense, and saves the result \f$\hat{x}\f$
 * in the output pixel.
 *
 * It can be used as a simple way to unmix an hyperspectral dataset,
 * where \f$A\f$ is the matrix in which each row corresponds to an endmember signature,
 * although better algorithms can be found for this particular task.
 *
 * The number of rows in \f$A\f$ must match the input image number of bands.
 * The number of bands in the output image will be the number of columns of \f$A\f$
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
  typedef otb::UnaryFunctorImageFilter
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

