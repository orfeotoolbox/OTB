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
#ifndef __otbPhaseFunctor_h
#define __otbPhaseFunctor_h

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
    std::vector<unsigned int> channels;
    channels.push_back(0);
    channels.push_back(1);
    this->SetChannelList(channels);
  }

  /** Destructor */
  virtual ~PhaseFunctor() {}

  const char *GetDescription() const
  {return "Phase"; }

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
    outPixel[0] = ComputePhase(inPixel[(this->GetChannelList())[0]], inPixel[(this->GetChannelList())[1]]);
    return outPixel;
  }

  virtual OutputPixelType operator ()(ScalarType /*inPixel*/) const
  {
    //FIXME we don't handle the std::complex<> yet
    itkExceptionMacro(<< "Can't compute amplitude from a scalar value");
  }

  virtual OutputPixelType operator ()(const RGBPixelType& inPixel) const
  {
    OutputPixelType outPixel;
    outPixel.SetSize(1);

    assert((this->GetChannelList())[0] < 3);
    assert((this->GetChannelList())[1] < 3);

    outPixel[0] = ComputePhase(inPixel[(this->GetChannelList())[0]], inPixel[(this->GetChannelList())[1]]);
    return outPixel;
  }

  virtual OutputPixelType operator ()(const RGBAPixelType& inPixel) const
  {
    OutputPixelType outPixel;
    outPixel.SetSize(1);

    assert((this->GetChannelList())[0] < 4);
    assert((this->GetChannelList())[1] < 4);

    outPixel[0] = ComputePhase(inPixel[(this->GetChannelList())[0]], inPixel[(this->GetChannelList())[1]]);
    return outPixel;
  }

private:
  inline ScalarType ComputePhase(ScalarType a, ScalarType b) const
  {
    return vcl_atan2(b, a);
  }

};
}
}

#endif
