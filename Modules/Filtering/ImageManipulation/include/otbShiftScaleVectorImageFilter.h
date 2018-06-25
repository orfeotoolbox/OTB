/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbShiftScaleImageFilter_h
#define otbShiftScaleImageFilter_h

#include "itkUnaryFunctorImageFilter.h"
#include "itkVariableLengthVector.h"

namespace otb
{
namespace Functor
{
/** \class VectorShiftScale
 *  \brief This functor performs a per band linear transform of its input.
 *
 *  For each band, the following formula is applied :
 *
 *  \f[ output = \frac{input - shift}{scale} \f]
 *
 * Standard casting is applied between input and output type.
 *
 * Shifts and scales can be set via the SetShiftValues() and SetScaleValues() methods.
 *
 *  TInput and TOutput type are supposed to be of type itk::VariableLengthVector.
 *
 *
 * \ingroup OTBImageManipulation
 */
template<class TInput, class TOutput>
class VectorShiftScale
{
public:
public:
  /// Real type typedef
  typedef typename itk::NumericTraits<typename TInput::ValueType>::RealType RealType;

  /// Constructor
  VectorShiftScale() {}

  /// Constructor
  virtual ~VectorShiftScale() {}

  /// Accessors
  void SetShiftValues(TInput value)
  {
    m_Shift = value;
  }
  void SetScaleValues(TInput value)
  {
    m_Scale = value;
  }
  TInput GetShiftValues()
  {
    return m_Shift;
  }
  TInput GetScaleValues()
  {
    return m_Scale;
  }

  bool operator !=(const VectorShiftScale& other) const
  {
    if (m_Shift.Size() == other.GetShiftValues().Size())
          {
          for (unsigned int i = 0; i < m_Shift.Size(); ++i)
            {
            if (m_Shift[i] != other.GetShiftValues()[i])
              {
              return true;
              }
            }
          }
    if (m_Scale.Size() == other.GetScaleValues().Size())
      {
      for (unsigned int i = 0; i < m_Scale.Size(); ++i)
        {
        if (m_Scale[i] != other.GetScaleValues()[i])
          {
          return true;
          }
        }
      }
    return false;
  }

  bool operator==(const VectorShiftScale & other) const
  {
    return !(*this != other);
  }

  // main computation method
  inline TOutput operator()(const TInput & x) const
  {
    // output instantiation
    TOutput result;
    result.SetSize(x.GetSize());

    // consistency checking
    if (result.GetSize() != m_Scale.GetSize()
        || result.GetSize() != m_Shift.GetSize())
      {
      itkGenericExceptionMacro(<< "Pixel size different from scale or shift size !");
      }

    // transformation
    for (unsigned int i = 0; i < x.GetSize(); ++i)
      {
      if ( m_Scale[i] > 1e-10)
        {
        const RealType invertedScale = 1 / m_Scale[i];
        result[i] = static_cast<typename TOutput::ValueType> (invertedScale * (x[i] - m_Shift[i]) );
        }
      else
        {
        result[i] = static_cast<typename TOutput::ValueType> (x[i] - m_Shift[i]);
        }
      }
    return result;
  }

private:
  TInput  m_Shift;
  TOutput m_Scale;
};
} // End namespace Functor

/** \class ShiftScaleVectorImageFilter
 *  \brief This filter performs a shift and scaling of a vector image on a per band basis.
 *
 *  For each band, the following formula is applied :
 *
 *  \f[ output = \frac{input - shift}{scale} \f]
 *
 *  Beware that the behaviour differs from itk::ShiftScaleImageFilter
 *  (which add shift instead of subtracting it).
 *
 *  \sa VectorShiftScale
 *  \ingroup IntensityImageFilters
 *  \ingroup MultiThreaded
 *
 * \ingroup OTBImageManipulation
 */
template <class TInputImage, class TOutputImage = TInputImage>
class ITK_EXPORT ShiftScaleVectorImageFilter :
    public itk::UnaryFunctorImageFilter<TInputImage, TOutputImage,
                  Functor::VectorShiftScale<typename TInputImage::PixelType,
                                            typename TOutputImage::PixelType> >
{
public:
  /** Standard class typedefs. */
  typedef ShiftScaleVectorImageFilter                                        Self;
  typedef Functor::VectorShiftScale< typename TInputImage::PixelType,
                                      typename TOutputImage::PixelType>
                                                                             FunctorType;
  typedef itk::UnaryFunctorImageFilter<TInputImage, TOutputImage,
                                       FunctorType >                         Superclass;
  typedef itk::SmartPointer<Self>                                            Pointer;
  typedef itk::SmartPointer<const Self>                                      ConstPointer;

  typedef typename TOutputImage::PixelType                                   OutputPixelType;
  typedef typename TInputImage::PixelType                                    InputPixelType;
  typedef typename InputPixelType::ValueType                                 InputValueType;
  typedef typename OutputPixelType::ValueType                                OutputValueType;
  typedef typename itk::NumericTraits<InputValueType>::RealType              InputRealType;
  typedef typename itk::NumericTraits<OutputValueType>::RealType             OutputRealType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(ShiftScaleImageFilter,
               itk::UnaryFunctorImageFilter);

  itkGetMacro(Scale, InputPixelType);
  itkSetMacro(Scale, InputPixelType);

  itkGetMacro(Shift, InputPixelType);
  itkSetMacro(Shift, InputPixelType);

protected:
  ShiftScaleVectorImageFilter() {}
  ~ShiftScaleVectorImageFilter() override {}

  /** Process to execute before entering the multithreaded section */
  void BeforeThreadedGenerateData(void) override;

  /** Generate output information */
  void GenerateOutputInformation(void) override;

  /** Generate input requested region */
  void GenerateInputRequestedRegion(void) override;

private:
  ShiftScaleVectorImageFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  InputPixelType  m_Scale;
  InputPixelType  m_Shift;

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbShiftScaleVectorImageFilter.hxx"
#endif

#endif
