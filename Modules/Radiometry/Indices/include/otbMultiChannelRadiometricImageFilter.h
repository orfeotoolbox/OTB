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

#ifndef otbMultiChannelRadiometricImageFilter_h
#define otbMultiChannelRadiometricImageFilter_h

#include "itkUnaryFunctorImageFilter.h"

#include "otbMacro.h"
#include "otbBandName.h"

namespace otb
{

/** \class MultiChannelRadiometricImageFilter
   * \brief Implements multi channel radiometric indices operation on one vector image.
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
template <class TInputImage, class TOutputImage, class TFunction>
class MultiChannelRadiometricImageFilter
  : public itk::UnaryFunctorImageFilter<TInputImage, TOutputImage, TFunction>
{
public:
  /** Standard class typedefs. */
  typedef MultiChannelRadiometricImageFilter                                 Self;
  typedef itk::UnaryFunctorImageFilter<TInputImage, TOutputImage, TFunction> Superclass;
  typedef itk::SmartPointer<Self>                                            Pointer;
  typedef itk::SmartPointer<const Self>                                      ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(MultiChannelRadiometricImageFilter, UnaryFunctorImageFilter);

  /** Some typedefs. */
  typedef TFunction FunctorType;

  /** Set index, generic method */
  virtual void SetIndex(BandName::BandName band, unsigned int channel)
  {
    switch (band)
      {
      case BandName::BLUE:
        m_BlueIndex = channel;
        break;
      case BandName::GREEN:
        m_GreenIndex = channel;
        break;
      case BandName::RED:
        m_RedIndex = channel;
        break;
      case BandName::NIR:
        m_NIRIndex = channel;
        break;
      case BandName::MIR:
        m_MIRIndex = channel;
        break;
      default:
        otbMsgDevMacro(<< "Band type not expected, you should probably do something about it!");
      }
  }
  /** Get index, generic method */
  virtual unsigned int GetIndex(BandName::BandName band) const
  {
    switch (band)
      {
      case BandName::BLUE:
        return m_BlueIndex;
        break;
      case BandName::GREEN:
        return m_GreenIndex;
        break;
      case BandName::RED:
        return m_RedIndex;
        break;
      case BandName::NIR:
        return m_NIRIndex;
        break;
      case BandName::MIR:
        return m_MIRIndex;
        break;
      default:
        otbMsgDevMacro(<< "Band type not expected, you should probably do something about it!");
      }
    return 0;
  }

protected:
  /// Constructor
  MultiChannelRadiometricImageFilter() :
    m_BlueIndex(0), m_GreenIndex(0), m_RedIndex(0), m_NIRIndex(0), m_MIRIndex(0) {};
  /// Destructor
  virtual ~MultiChannelRadiometricImageFilter() {}
  /// Before generating data, set functor parameters
  virtual void BeforeThreadedGenerateData()
  {
//    unsigned int lNbChan = this->GetInput()->GetNumberOfComponentsPerPixel();
/*    if (m_RedIndex < 1 || m_NIRIndex < 1 ||
        m_RedIndex > lNbChan || m_NIRIndex > lNbChan)
      {
      itkExceptionMacro(<< "Channel indices must belong to range [1, ...[");
      }*/
    this->GetFunctor().SetIndex(BandName::BLUE, m_BlueIndex);
    this->GetFunctor().SetIndex(BandName::GREEN, m_GreenIndex);
    this->GetFunctor().SetIndex(BandName::RED, m_RedIndex);
    this->GetFunctor().SetIndex(BandName::NIR, m_NIRIndex);
    this->GetFunctor().SetIndex(BandName::MIR, m_MIRIndex);
  }
  /// PrintSelf Method
  void PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    this->Superclass::PrintSelf(os, indent);
    os << indent << "Blue index: " << m_BlueIndex << std::endl;
    os << indent << "Green index: " << m_GreenIndex << std::endl;
    os << indent << "Red index: " << m_RedIndex << std::endl;
    os << indent << "NIR index: " << m_NIRIndex << std::endl;
    os << indent << "MIR index: " << m_MIRIndex << std::endl;
  }
private:
  MultiChannelRadiometricImageFilter(const Self &) = delete;
  void operator =(const Self&) = delete;
  /** Blue channel index */
  unsigned int m_BlueIndex;
  /** Green channel index */
  unsigned int m_GreenIndex;
  /** Red channel index */
  unsigned int m_RedIndex;
  /** NIR channel index */
  unsigned int m_NIRIndex;
  /** MIR channel index */
  unsigned int m_MIRIndex;

};
} // end namespace otb

#endif
