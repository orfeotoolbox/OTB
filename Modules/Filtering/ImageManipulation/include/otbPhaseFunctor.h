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

#ifndef otbPhaseFunctor_h
#define otbPhaseFunctor_h

#include "otbChannelSelectorFunctor.h"

namespace otb
{
namespace Function
{
/** \class PhaseFunctor
* \brief Compute the module from the selected channel in the input
*
*  \ingroup Visualization
*
* \sa AmplitudeFunctor ChannelSelectorFunctor
 *
 * \ingroup OTBImageManipulation
*/
template <class TInputPixel>
class PhaseFunctor : public ChannelSelectorFunctor<TInputPixel>
{
public:
  /** Standard class typedefs */
  typedef PhaseFunctor                  Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory */
  itkNewMacro(Self);

  /** Runtime information */
  itkTypeMacro(PhaseFunctor, itk::Object);

  typedef TInputPixel                                       PixelType;
  typedef typename itk::NumericTraits<PixelType>::ValueType ScalarType;
  typedef itk::VariableLengthVector<ScalarType>             VectorPixelType;
  typedef itk::RGBPixel<ScalarType>                         RGBPixelType;
  typedef itk::RGBAPixel<ScalarType>                        RGBAPixelType;

  typedef VectorPixelType OutputPixelType;

  /** Constructor */
  PhaseFunctor()
  {
    this->m_ChannelList.resize(2,1);
    this->m_ChannelList[1] = 1;
  }

  /** Destructor */
  ~PhaseFunctor() override = default;

  const char *GetDescription() const
  {return "Phase"; }

  unsigned int GetOutputSize() const override
  {
    return 1;
  }

  OutputPixelType operator ()(const VectorPixelType& inPixel) const override
  {
    OutputPixelType outPixel(1);
    assert(this->m_ChannelList.size() == 2);
    assert(this->m_ChannelList[0] < inPixel.Size());
    assert(this->m_ChannelList[1] < inPixel.Size());
    outPixel[0] = ComputePhase(inPixel[this->m_ChannelList[0]], inPixel[this->m_ChannelList[1]]);
    return outPixel;
  }

  OutputPixelType operator ()(ScalarType /*inPixel*/) const override
  {
    //FIXME we don't handle the std::complex<> yet
    itkExceptionMacro(<< "Can't compute amplitude from a scalar value");
  }

  OutputPixelType operator ()(const RGBPixelType& inPixel) const override
  {
    OutputPixelType outPixel(1);

    assert(this->m_ChannelList[0] < 3);
    assert(this->m_ChannelList[1] < 3);

    outPixel[0] = ComputePhase(inPixel[this->m_ChannelList[0]], inPixel[this->m_ChannelList[1]]);
    return outPixel;
  }

  OutputPixelType operator ()(const RGBAPixelType& inPixel) const override
  {
    OutputPixelType outPixel(1);

    assert(this->m_ChannelList[0] < 4);
    assert(this->m_ChannelList[1] < 4);

    outPixel[0] = ComputePhase(inPixel[this->m_ChannelList[0]], inPixel[this->m_ChannelList[1]]);
    return outPixel;
  }

private:
  ScalarType ComputePhase(ScalarType a, ScalarType b) const
  {
    return std::atan2(b, a);
  }

};
}
}

#endif
