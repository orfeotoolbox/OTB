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
#ifndef __otbAmplitudeFunctor_h
#define __otbAmplitudeFunctor_h

#include "otbChannelSelectorFunctor.h"

namespace otb
{
namespace Function
{
/** \class AmplitudeFunctor
* \brief Compute the module from the selected channel in the input
*
*  \ingroup Visualization
*
* \sa PhaseFunctor ChannelSelectorFunctor
*/
template <class TInputPixel>
class AmplitudeFunctor : public ChannelSelectorFunctor<TInputPixel>
{
public:
  /** Standard class typedefs */
  typedef AmplitudeFunctor              Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory */
  itkNewMacro(Self);

  /** Runtime information */
  itkTypeMacro(AmplitudeFunctor, itk::Object);

  typedef TInputPixel                                       PixelType;
  typedef typename itk::NumericTraits<PixelType>::ValueType ScalarType;
  typedef itk::VariableLengthVector<ScalarType>             VectorPixelType;
  typedef itk::RGBPixel<ScalarType>                         RGBPixelType;
  typedef itk::RGBAPixel<ScalarType>                        RGBAPixelType;

  typedef VectorPixelType OutputPixelType;

  /** Constructor */
  AmplitudeFunctor()
  {
    std::vector<unsigned int> channels;
    channels.push_back(0);
    channels.push_back(1);
    this->SetChannelList(channels);
  }

  /** Destructor */
  virtual ~AmplitudeFunctor() {}

  const char *GetDescription() const
  {return "Amplitude"; }

  virtual unsigned int GetOutputSize() const
  {
    return 1;
  }

  virtual OutputPixelType operator ()(const VectorPixelType& inPixel) const
  {
    OutputPixelType outPixel;
    outPixel.SetSize(1);
    assert((this->GetChannelList()).size() == 2);
    assert((this->GetChannelList())[0] < inPixel.Size());
    assert((this->GetChannelList())[1] < inPixel.Size());
    outPixel[0] = ComputeAmplitude(inPixel[(this->GetChannelList())[0]], inPixel[(this->GetChannelList())[1]]);
    return outPixel;
  }

  virtual OutputPixelType operator ()(ScalarType) const
  {
    //FIXME we don't handle the std::complex<> yet
    itkExceptionMacro(<< "Can't compute amplitude from a scalar value");
  }

  virtual OutputPixelType operator ()(const RGBPixelType& inPixel) const
  {
    OutputPixelType outPixel;
    outPixel.SetSize(1);

    assert((this->GetChannelList()).size() == 2);
    assert((this->GetChannelList())[0] < 3);
    assert((this->GetChannelList())[1] < 3);

    outPixel[0] = ComputeAmplitude(inPixel[(this->GetChannelList())[0]], inPixel[(this->GetChannelList())[1]]);
    return outPixel;
  }

  virtual OutputPixelType operator ()(const RGBAPixelType& inPixel) const
  {
    OutputPixelType outPixel;
    outPixel.SetSize(1);

    assert((this->GetChannelList()).size() == 2);
    assert((this->GetChannelList())[0] < 4);
    assert((this->GetChannelList())[1] < 4);

    outPixel[0] = ComputeAmplitude(inPixel[(this->GetChannelList())[0]], inPixel[(this->GetChannelList())[1]]);
    return outPixel;
  }

private:
  inline ScalarType ComputeAmplitude(ScalarType a, ScalarType b) const
  {
    return vcl_sqrt(a * a + b * b);
  }

};
}
}

#endif
