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
#ifndef __otbConcatenateScalarValueImageFilter_h
#define __otbConcatenateScalarValueImageFilter_h

#include "itkMacro.h"
#include "itkUnaryFunctorImageFilter.h"

namespace otb
{

namespace Functor {

/** \class ConcatenateScalarValueFunctor
 *
 * \brief TODO
 *
 *
 * \ingroup OTBImageManipulation
 */
template<class TInput, class TOutput>
class ConcatenateScalarValueFunctor
{
public:
  typedef TInput     InputType;
  typedef TOutput    OutputType;

  typedef typename InputType::ComponentType ComponentType;

  ConcatenateScalarValueFunctor() : m_ScalarValue(0) {}
  virtual ~ConcatenateScalarValueFunctor() {}

  const ComponentType& GetScalarValue()
  {
    return m_ScalarValue;
  }

  void SetScalarValue(const ComponentType& p)
  {
    m_ScalarValue = p;
  }

  bool operator !=(const ConcatenateScalarValueFunctor& itkNotUsed(other)) const
  {
    return false;
  }

  bool operator ==(const ConcatenateScalarValueFunctor& other) const
  {
    return !(*this != other);
  }

  OutputType operator ()(const InputType& in)
  {
    OutputType out(in.Size() + 1);
    for (unsigned int j = 0; j < in.Size(); ++j)
      {
      out[j] = in[j];
      }
    out[in.Size()] = m_ScalarValue;
    return out;
  }

private:
  ComponentType m_ScalarValue;
};
}

/** \class ConcatenateScalarValueImageFilter
 *
 * \brief Applies a projective projection to each pixel of an image
 *
 * \ingroup Streamed
 * \ingroup Threaded
 *
 * \ingroup OTBImageManipulation
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT ConcatenateScalarValueImageFilter :
  public itk::UnaryFunctorImageFilter<TInputImage, TOutputImage,
      Functor::ConcatenateScalarValueFunctor<typename TInputImage::PixelType,
          typename TOutputImage::PixelType> >
{
public:
  /** Standard class typedefs. */
  typedef ConcatenateScalarValueImageFilter Self;
  typedef itk::UnaryFunctorImageFilter
     <TInputImage,
      TOutputImage,
      Functor::ConcatenateScalarValueFunctor<
          typename TInputImage::PixelType,
          typename TOutputImage::PixelType>
     >                                  Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef Functor::ConcatenateScalarValueFunctor<
      typename TInputImage::PixelType,
      typename TOutputImage::PixelType> FunctorType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ConcatenateScalarValueImageFilter, otb::UnaryFunctorImageFilter);

  /** Pixel types. */
  typedef typename TInputImage::PixelType         InputPixelType;
  typedef typename InputPixelType::ComponentType  InputComponentType;
  typedef typename TOutputImage::PixelType        OutputPixelType;

  const InputComponentType& GetScalarValue()
  {
    return this->GetFunctor().GetScalarValue();
  }

  void SetScalarValue(const InputComponentType& p)
  {
    this->GetFunctor().SetScalarValue(p);
    this->Modified();
  }

protected:
  ConcatenateScalarValueImageFilter();

  ~ConcatenateScalarValueImageFilter() ITK_OVERRIDE {}

  void PrintSelf(std::ostream& os, itk::Indent indent) const ITK_OVERRIDE;

  void GenerateOutputInformation() ITK_OVERRIDE;

private:
  ConcatenateScalarValueImageFilter(const Self &); //purposely not implemented

  void operator =(const Self&); //purposely not implemented

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbConcatenateScalarValueImageFilter.txx"
#endif

#endif

