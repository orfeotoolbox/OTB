/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbMultiChannelRAndBAndNIRIndexImageFilter_h
#define otbMultiChannelRAndBAndNIRIndexImageFilter_h

#include "itkUnaryFunctorImageFilter.h"
#include "otbVegetationIndicesFunctor.h"

namespace otb
{

/** \class MultiChannelRAndBAndNIRIndexImageFilter
   * \brief Implements multi channel R and B and NIR pixel-wise generic index operation on one vector image.
   *
   * This class is parameterized over the type of the input image and
   * the type of the output image.  It is also parameterized by the
   * operation to be applied, using a Functor style.
   *
   * \sa UnaryFunctorImageFilter
   * \ingroup Radiometry
 *
 * \ingroup OTBIndices
 */
template <class TInputImage, class TOutputImage,
    class TFunction = Functor::ARVI<typename TInputImage::InternalPixelType,
        typename TInputImage::InternalPixelType,
        typename TInputImage::InternalPixelType,
        typename TOutputImage::PixelType> >
class ITK_EXPORT MultiChannelRAndBAndNIRIndexImageFilter
  : public itk::UnaryFunctorImageFilter<TInputImage, TOutputImage, TFunction>
{
public:
  /** Standard class typedefs. */
  typedef MultiChannelRAndBAndNIRIndexImageFilter                            Self;
  typedef itk::UnaryFunctorImageFilter<TInputImage, TOutputImage, TFunction> Superclass;
  typedef itk::SmartPointer<Self>                                            Pointer;
  typedef itk::SmartPointer<const Self>                                      ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(MultiChannelRAndBAndNIRIndexImageFilter, UnaryFunctorImageFilter);

  /** Some typedefs. */
  typedef TFunction FunctorType;

  /** Set/Get the red channel index. Value must be in [1...[ */
  itkSetMacro(RedIndex, unsigned int);
  itkGetMacro(RedIndex, unsigned int);
  /** Set/Get the blue channel index. Value must be in [1...[ */
  itkSetMacro(BlueIndex, unsigned int);
  itkGetMacro(BlueIndex, unsigned int);
  /** Set/Get the nir channel index. Value must be in [1...[ */
  itkSetMacro(NIRIndex, unsigned int);
  itkGetMacro(NIRIndex, unsigned int);

  /** Set index, generic method */
  void SetIndex(BandName::BandName band, unsigned int channel)
  {
    if (band == BandName::RED)
      {
      m_RedIndex = channel;
      }
    if (band == BandName::BLUE)
      {
      m_BlueIndex = channel;
      }
    if (band == BandName::NIR)
      {
      m_NIRIndex = channel;
      }
  }
  /** Get index, generic method */
  unsigned int GetIndex(BandName::BandName band) const
  {
    if (band == BandName::RED)
      {
      return m_RedIndex;
      }
    if (band == BandName::BLUE)
      {
      return m_BlueIndex;
      }
    if (band == BandName::NIR)
      {
      return m_NIRIndex;
      }
  }

protected:
  /// Constructor
  MultiChannelRAndBAndNIRIndexImageFilter() : m_RedIndex(3), m_BlueIndex(1), m_NIRIndex(4) {};
  /// Destructor
  ~MultiChannelRAndBAndNIRIndexImageFilter() override {}
  /// Before generating data, set functor parameters
  void BeforeThreadedGenerateData() override
  {
    unsigned int lNbChan = this->GetInput()->GetNumberOfComponentsPerPixel();
    if (m_RedIndex < 1 || m_BlueIndex < 1 || m_NIRIndex < 1 ||
        m_RedIndex > lNbChan || m_BlueIndex > lNbChan || m_NIRIndex > lNbChan)
      {
      itkExceptionMacro(<< "Channel indices must belong to range [1, ...[");
      }
    this->GetFunctor().SetRedIndex(m_RedIndex);
    this->GetFunctor().SetBlueIndex(m_BlueIndex);
    this->GetFunctor().SetNIRIndex(m_NIRIndex);
  }
  /// PrintSelf
  void PrintSelf(std::ostream& os, itk::Indent indent) const override
  {
    this->Superclass::PrintSelf(os, indent);
    os << indent << "Red  index: " << m_RedIndex << std::endl;
    os << indent << "Blue index: " << m_BlueIndex << std::endl;
    os << indent << "NIR  index: " << m_NIRIndex << std::endl;
  }

private:
  MultiChannelRAndBAndNIRIndexImageFilter(const Self &) = delete;
  void operator =(const Self&) = delete;

  /** Red channel index */
  unsigned int m_RedIndex;
  /** Blue channel index */
  unsigned int m_BlueIndex;
  /** NIR channel index */
  unsigned int m_NIRIndex;
};

} // end namespace otb

#endif
