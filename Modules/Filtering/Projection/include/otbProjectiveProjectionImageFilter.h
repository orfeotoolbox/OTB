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
#ifndef __otbProjectiveProjectionImageFilter_h
#define __otbProjectiveProjectionImageFilter_h

#include "itkMacro.h"
#include "otbUnaryFunctorImageFilter.h"

namespace otb
{

namespace Functor {

/** \class ProjectiveProjectionFunctor
 *
 * \brief TODO
 *
 */
template<class TInput, class TOutput, class TPrecision>
class ProjectiveProjectionFunctor
{
public:
  typedef TInput     InputType;
  typedef TOutput    OutputType;
  typedef TPrecision PrecisionType;

  ProjectiveProjectionFunctor() : m_OutputSize(0) {}
  virtual ~ProjectiveProjectionFunctor() {}

  unsigned int GetOutputSize()
  {
    return m_OutputSize;
  }

  const InputType& GetProjectionDirection()
  {
    return m_ProjectionDirection;
  }

  void SetProjectionDirection(const InputType& p)
  {
    m_ProjectionDirection = p;
    m_OutputSize = m_ProjectionDirection.Size();
  }

  bool operator !=(const ProjectiveProjectionFunctor& other) const
  {
    return false;
  }

  bool operator ==(const ProjectiveProjectionFunctor& other) const
  {
    return !(*this != other);
  }

  OutputType operator ()(const InputType& in)
  {
    PrecisionType dotProduct = 0;
    for (unsigned int i = 0; i < in.Size(); ++i)
      {
      dotProduct += in[i] * m_ProjectionDirection[i];
      }

    OutputType projected(in.Size());
    for (unsigned int j = 0; j < in.Size(); ++j)
      {
      projected[j] = in[j] / dotProduct;
      }

    return projected;
  }

private:
  unsigned int   m_OutputSize;
  InputType      m_ProjectionDirection;
};
}

/** \class ProjectiveProjectionImageFilter
 *
 * \brief Applies a projective projection to each pixel of an image
 *
 * \ingroup Streamed
 * \ingroup Threaded
 */
template <class TInputImage, class TOutputImage, class TPrecision>
class ITK_EXPORT ProjectiveProjectionImageFilter :
  public otb::UnaryFunctorImageFilter<TInputImage, TOutputImage,
      Functor::ProjectiveProjectionFunctor<typename TInputImage::PixelType,
          typename TOutputImage::PixelType, TPrecision> >
{
public:
  /** Standard class typedefs. */
  typedef ProjectiveProjectionImageFilter Self;
  typedef otb::UnaryFunctorImageFilter
     <TInputImage,
      TOutputImage,
      Functor::ProjectiveProjectionFunctor<
          typename TInputImage::PixelType,
          typename TOutputImage::PixelType,
          TPrecision>
     >                                 Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef Functor::ProjectiveProjectionFunctor<
      typename TInputImage::PixelType,
      typename TOutputImage::PixelType,
      TPrecision> FunctorType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ProjectiveProjectionImageFilter, otb::UnaryFunctorImageFilter);

  /** Pixel types. */
  typedef typename TInputImage::PixelType  InputPixelType;
  typedef typename TOutputImage::PixelType OutputPixelType;

  const InputPixelType& GetProjectionDirection()
  {
    return this->GetFunctor().GetProjectionDirection();
  }

  void SetProjectionDirection(const InputPixelType& p)
  {
    this->GetFunctor().SetProjectionDirection(p);
    this->Modified();
  }

protected:
  ProjectiveProjectionImageFilter();

  virtual ~ProjectiveProjectionImageFilter() {}

  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  virtual void GenerateOutputInformation()
  {
    Superclass::GenerateOutputInformation();
    typename Superclass::OutputImagePointer outputPtr = this->GetOutput();
    typename Superclass::InputImagePointer  inputPtr = this->GetInput();
    outputPtr->SetNumberOfComponentsPerPixel( // propagate vector length info
      this->GetInput()->GetNumberOfComponentsPerPixel());
  }

private:
  ProjectiveProjectionImageFilter(const Self &); //purposely not implemented

  void operator =(const Self&); //purposely not implemented

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbProjectiveProjectionImageFilter.txx"
#endif

#endif

