/*
 * Copyright (C) 1999-2011 Insight Software Consortium
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

#ifndef otbVectorRescaleIntensityImageFilter_h
#define otbVectorRescaleIntensityImageFilter_h

#include "itkUnaryFunctorImageFilter.h"
#include "itkVariableLengthVector.h"

namespace otb
{
namespace Functor
{
/** \class VectorAffineTransform
 *  \brief This functor performs a per band linear transform of its input.
 *
 *  Values upper than InputMaximum or lower than InputMinimum are clamped to OutputMaximum
 *  respectively OutputMinimum values.
 *
 *  TInput and TOutput type are supposed to be of type itk::VariableLengthVector.
 *
 *
 * \ingroup OTBImageManipulation
 */
template<typename TInput, typename  TOutput>
class VectorAffineTransform
{
public:
  /// Real type typedef
  typedef typename itk::NumericTraits<typename TInput::ValueType>::RealType RealType;

  /// Constructor
  VectorAffineTransform(): m_OutputMaximum(), m_OutputMinimum(), m_InputMinimum(), m_InputMaximum(), m_Gamma(1.){}
  /// Desctructor
  virtual ~VectorAffineTransform() {}

  /// Accessors
  void SetOutputMaximum(TOutput a)
  {
    m_OutputMaximum = a;
  }
  void SetOutputMinimum(TOutput a)
  {
    m_OutputMinimum = a;
  }
  void SetInputMinimum(TInput a)
  {
    m_InputMinimum = a;
  }
  void SetInputMaximum(TInput a)
  {
    m_InputMaximum = a;
  }
  void SetGamma(const double& gamma)
  {
    m_Gamma = gamma;
  }
  TOutput GetOutputMaximum()
  {
    return m_OutputMaximum;
  }
  TOutput GetOutputMinimum()
  {
    return m_OutputMinimum;
  }
  TInput GetInputMinimum()
  {
    return m_InputMinimum;
  }
  TInput GetInputMaximum()
  {
    return m_InputMaximum;
  }
  double GetGamma()
  {
    return m_Gamma;
  }


  bool operator !=(const VectorAffineTransform& other) const
  {
    if (m_OutputMinimum.Size() == other.GetOutputMinimum().Size())
      {
      for (unsigned int i = 0; i < m_OutputMinimum.Size(); ++i)
        {
        if (m_OutputMinimum[i] != other.GetOutputMinimum()[i])
          {
          return true;
          }
        }
      }
    if (m_OutputMaximum.Size() == other.GetOutputMaximum().Size())
      {
      for (unsigned int i = 0; i < m_OutputMaximum.Size(); ++i)
        {
        if (m_OutputMaximum[i] != other.GetOutputMaximum()[i])
          {
          return true;
          }
        }
      }
    if (m_InputMinimum.Size() == other.GetInputMinimum().Size())
      {
      for (unsigned int i = 0; i < m_InputMinimum.Size(); ++i)
        {
        if (m_InputMinimum[i] != other.GetInputMinimum()[i])
          {
          return true;
          }
        }
      }
    if (m_InputMaximum.Size() == other.GetInputMaximum().Size())
      {
      for (unsigned int i = 0; i < m_InputMaximum.Size(); ++i)
        {
        if (m_InputMaximum[i] != other.GetInputMaximum()[i])
          {
          return true;
          }
        }
      }
    return false;
  }
  bool operator ==(const VectorAffineTransform& other) const
  {
    return !(*this != other);
  }

  // main computation method
  inline TOutput operator ()(const TInput& x)
  {
    // output instantiation
    TOutput result;
    result.SetSize(x.GetSize());

    // consistency checking
    if (result.GetSize() != m_OutputMinimum.GetSize()
        || result.GetSize() != m_OutputMaximum.GetSize()
        || result.GetSize() != m_InputMinimum.GetSize()
        || result.GetSize() != m_InputMaximum.GetSize())
      {
      itkGenericExceptionMacro(<< "Pixel size different from scale or shift size !");
      }

    // transformation
    for (unsigned int i = 0; i < x.GetSize(); ++i)
      {
      if (x[i] < m_InputMinimum[i])
        {
        result[i] = m_OutputMinimum[i];
        }
      else if (x[i] > m_InputMaximum[i])
        {
        result[i] = m_OutputMaximum[i];
        }
      else if (m_InputMaximum[i] == m_InputMinimum[i])
        {
        result[i] = m_OutputMinimum[i];
        }
      else
        {
        RealType scaledComponent = static_cast<RealType>(x[i] - m_InputMinimum[i])/ static_cast<RealType> (m_InputMaximum[i] - m_InputMinimum[i]);
        scaledComponent = vcl_pow(scaledComponent,1./m_Gamma);
        scaledComponent *= static_cast<RealType> (m_OutputMaximum[i] - m_OutputMinimum[i]);
        result[i] = static_cast<typename TOutput::ValueType>(scaledComponent + m_OutputMinimum[i]);
        }
      }
    return result;
  }
private:
  TOutput m_OutputMaximum;
  TOutput m_OutputMinimum;
  TInput  m_InputMinimum;
  TInput  m_InputMaximum;
  double  m_Gamma;
};
}  // end namespace functor

/** \class VectorRescaleIntensityImageFilter
 *  \brief This filter performs a rescaling of a vector image on a per band basis.
 *
 * This filter rescales each band to match the [OutputMinimum, OutputMaximum] range.
 * In order to avoid odd values to alter the intensity extent, one can set a clamp percentage.
 * The clamp percentage is set to 0.01 by default.
 *
 * Values lower than the first quantile of this percentage are set to the OutputMinimum.
 * Values upper than the last quantile of this percentage are set to the OutputMaximum.
 *
 * This filter differ from itk::VectorRescaleIntensityImageFilter.
 * Instead of setting only the OutputMaximumMagnitude, you can set the minimum and maximum values for the input and output images.
 * There is also the possibility to set a Gamma value and change the clamp percentage.
 *
 *  \ingroup IntensityImageFilters
 *  \ingroup MultiThreaded
 *
 * \ingroup OTBImageManipulation
 */
template <class TInputImage, class TOutputImage = TInputImage>
class ITK_EXPORT VectorRescaleIntensityImageFilter
  :  public itk::UnaryFunctorImageFilter<TInputImage, TOutputImage,
      Functor::VectorAffineTransform<
          typename TInputImage::PixelType,
          typename TOutputImage::PixelType> >
{
public:
  /** Standard class typedefs. */
  typedef VectorRescaleIntensityImageFilter Self;
  typedef itk::UnaryFunctorImageFilter<TInputImage, TOutputImage,
      Functor::VectorAffineTransform<typename TInputImage::PixelType,
          typename TOutputImage::PixelType> >  Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef TInputImage    InputImageType;
  typedef typename InputImageType::ConstPointer InputImagePointer;
  typedef typename TOutputImage::PixelType                       OutputPixelType;
  typedef typename InputImageType::PixelType                     InputPixelType;
  typedef typename InputPixelType::ValueType                     InputValueType;
  typedef typename OutputPixelType::ValueType                    OutputValueType;
  typedef typename itk::NumericTraits<InputValueType>::RealType  InputRealType;
  typedef typename itk::NumericTraits<OutputValueType>::RealType OutputRealType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  itkSetMacro(OutputMaximum, OutputPixelType);
  itkGetConstReferenceMacro(OutputMaximum, OutputPixelType);
  itkSetMacro(OutputMinimum, OutputPixelType);
  itkGetConstReferenceMacro(OutputMinimum, OutputPixelType);
  itkSetMacro(AutomaticInputMinMaxComputation, bool);
  itkGetMacro(AutomaticInputMinMaxComputation, bool);
  itkBooleanMacro(AutomaticInputMinMaxComputation);

  itkGetMacro(ClampThreshold, double);
  itkSetMacro(ClampThreshold, double);

  itkGetMacro(InputMinimum, InputPixelType);
  itkSetMacro(InputMinimum, InputPixelType);

  itkGetMacro(InputMaximum, InputPixelType);
  itkSetMacro(InputMaximum, InputPixelType);

  itkSetMacro(Gamma,double);
  itkGetConstReferenceMacro(Gamma,double);

  /** Process to execute before entering the multithreaded section */
  void BeforeThreadedGenerateData(void) override;

  /** Generate output information */
  void GenerateOutputInformation(void) override;

  /** Generate input requested region */
  void GenerateInputRequestedRegion(void) override;

  /** Print internal ivars */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

protected:
  VectorRescaleIntensityImageFilter();
  ~VectorRescaleIntensityImageFilter() override {}

private:
  VectorRescaleIntensityImageFilter(const Self &) = delete;
  void operator =(const Self&) = delete;

  OutputPixelType m_OutputMinimum;
  OutputPixelType m_OutputMaximum;
  InputPixelType  m_InputMinimum;
  InputPixelType  m_InputMaximum;
  double          m_ClampThreshold;
  bool            m_AutomaticInputMinMaxComputation;
  double          m_Gamma;

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbVectorRescaleIntensityImageFilter.hxx"
#endif

#endif
