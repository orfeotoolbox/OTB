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
#ifndef __otbDotProductImageFilter_h
#define __otbDotProductImageFilter_h

#include "itkMacro.h"
#include "otbUnaryFunctorImageFilter.h"

namespace otb
{

namespace Functor {

/** \class DotProductFunctor
 *
 * \brief Computes the dot product against a specific vector
 *
 */
template<class TInput, class TOutput>
class DotProductFunctor
{
public:
  typedef TInput     InputType;
  typedef TOutput    OutputType;

  DotProductFunctor() {}
  virtual ~DotProductFunctor() {}

  bool operator !=(const DotProductFunctor& other) const
  {
    return false;
  }

  bool operator ==(const DotProductFunctor& other) const
  {
    return !(*this != other);
  }

  const InputType& GetVector()
  {
    return m_Vector;
  }

  void SetVector(const InputType& m)
  {
    m_Vector = m;
  }

  OutputType operator ()(const InputType& in)
  {
    OutputType result = 0;
    for(unsigned int i = 0; i < in.Size(); ++i)
      {
      result += in[i] * m_Vector[i];
      }
    return result;
  }

private:
  InputType m_Vector;
};
}

/** \class DotProductImageFilter
 *
 * \brief Applies pixel-wise dot product to a VectorImage
 *
 * Given a vector, this filter outputs the dot product of each pixel of a multiband image
 * with respect to the specified vector
 *
 * \ingroup Streamed
 * \ingroup Threaded
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT DotProductImageFilter :
  public itk::UnaryFunctorImageFilter<TInputImage, TOutputImage,
      Functor::DotProductFunctor<typename TInputImage::PixelType,
          typename TOutputImage::PixelType> >
{
public:
  /** Standard class typedefs. */
  typedef DotProductImageFilter Self;
  typedef itk::UnaryFunctorImageFilter
     <TInputImage,
      TOutputImage,
      Functor::DotProductFunctor<
          typename TInputImage::PixelType,
          typename TOutputImage::PixelType>
     >                                 Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef Functor::DotProductFunctor<
      typename TInputImage::PixelType,
      typename TOutputImage::PixelType> FunctorType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(DotProductImageFilter, itk::UnaryFunctorImageFilter);

  /** Pixel types. */
  typedef typename TInputImage::PixelType  InputPixelType;
  typedef typename TOutputImage::PixelType OutputPixelType;

  const InputPixelType& GetVector()
  {
    return this->GetFunctor().GetVector();
  }

  void SetVector(const InputPixelType& p)
  {
    this->GetFunctor().SetVector(p);
    this->Modified();
  }

protected:
  DotProductImageFilter();

  virtual ~DotProductImageFilter() {}

  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  DotProductImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbDotProductImageFilter.txx"
#endif

#endif
