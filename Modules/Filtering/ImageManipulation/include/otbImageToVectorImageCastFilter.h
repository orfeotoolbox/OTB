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
#ifndef otbImageToVectorImageCastFilter_h
#define otbImageToVectorImageCastFilter_h

#include "itkUnaryFunctorImageFilter.h"
#include "itkVariableLengthVector.h"

namespace otb
{
namespace Functor
{
/**
 * \class VectorCast
 * \brief This functor can be used to convert scalar value to VariableLengthVector with another precision
 *  a size of 1.
 * \ingroup Functor
 *
 *
 * \ingroup OTBImageManipulation
 */
template <class TInput, class TOutput>
class VectorCast
{
public:
  typedef typename TOutput::ValueType OutputValueType;
  VectorCast() {}
  virtual ~VectorCast() {}

  bool operator !=(const VectorCast&) const
  {
    return false;
  }
  bool operator ==(const VectorCast& other) const
  {
    return (*this != other);
  }
  inline TOutput operator ()(const TInput& A)
  {
    TOutput output;
    output.SetSize(1);
    output[0] = static_cast<OutputValueType>(A);
    return output;
  }
};
} // End namespace Functor
/**
 * \class ImageToVectorImageCastFilter
 * \brief This is a helper class that convert an otb::Image into a single-channel otb::VectorImage.
 * \ingroup Singlethreaded
 *
 *
 * \ingroup OTBImageManipulation
 */
template <class TInputImage, class TOutputVectorImage>
class ITK_EXPORT ImageToVectorImageCastFilter
  : public itk::UnaryFunctorImageFilter<TInputImage, TOutputVectorImage,
      Functor::VectorCast<typename TInputImage::PixelType,
          typename TOutputVectorImage::PixelType> >
{
public:
  /// Standard class typedefs
  typedef ImageToVectorImageCastFilter Self;
  typedef itk::UnaryFunctorImageFilter<TInputImage, TOutputVectorImage,
      Functor::VectorCast<typename TInputImage::PixelType,
          typename TOutputVectorImage
          ::PixelType> > Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /// Method for creation through the object factory
  itkNewMacro(Self);

  /// Runtime information
  itkTypeMacro(ImageToVectorImageCastFilter, itk::UnaryFunctorImageFilter);

protected:
  /// Constructor
  ImageToVectorImageCastFilter() {}
  /// Detructor
  ~ImageToVectorImageCastFilter() ITK_OVERRIDE {}
  /// Additionnal output information for allocation
  void GenerateOutputInformation(void) ITK_OVERRIDE
  {
    Superclass::GenerateOutputInformation();
    this->GetOutput()->SetNumberOfComponentsPerPixel(1);
  }
  /// Copy output requested region to input requested region
  void GenerateInputRequestedRegion(void) ITK_OVERRIDE
  {
    if (this->GetInput())
      {
      typename TInputImage::Pointer input = const_cast<TInputImage *>(this->GetInput());
      typename TInputImage::RegionType inputRegion;
      this->CallCopyOutputRegionToInputRegion(inputRegion, this->GetOutput()->GetRequestedRegion());
      input->SetRequestedRegion(inputRegion);
      }
  }

private:
  ImageToVectorImageCastFilter(const Self &); //purposely not implemented
  void operator =(const Self&);  // purposely not implemented
};
} // end namespace otb
#endif
