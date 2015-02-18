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
#ifndef __otbChannelSelectorFunctor_h
#define __otbChannelSelectorFunctor_h
#include <cassert>

#include "itkVariableLengthVector.h"
#include "itkRGBPixel.h"
#include "itkRGBAPixel.h"
#include "itkObject.h"
#include "itkObjectFactory.h"

namespace otb
{
namespace Function
{
/** \class ChannelSelectorFunctor
* \brief Base class for pixel representation functions.
*
*  \ingroup Visualization
*
* \sa AmplitudeFunctor PhaseFunctor
*/
template <class TInputPixel>
class ChannelSelectorFunctor : public itk::Object
{
public:
  /** Standard class typedefs */
  typedef ChannelSelectorFunctor        Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  /** Method for creation through the object factory */
  itkNewMacro(Self);

  /** Runtime information */
  itkTypeMacro(ChannelSelectorFunctor, itk::Object);

  typedef TInputPixel                                       PixelType;
  typedef typename itk::NumericTraits<PixelType>::ValueType ScalarType;
  typedef itk::VariableLengthVector<ScalarType>             VectorPixelType;
  typedef itk::RGBPixel<ScalarType>                         RGBPixelType;
  typedef itk::RGBAPixel<ScalarType>                        RGBAPixelType;

  typedef VectorPixelType OutputPixelType;

  typedef std::vector<unsigned int> ChannelListType;

  const char *GetDescription() const
  {return "Channel Selection"; }

  virtual OutputPixelType operator ()(const VectorPixelType& inPixel) const
  {
//           otbMsgDevMacro(<<"Channel list "<< m_ChannelList[0]);
    OutputPixelType outPixel;
    outPixel.SetSize(m_ChannelList.size());
    for (unsigned int i = 0; i < m_ChannelList.size(); ++i)
      {
      // assert as the verification should be done outside and only
      // once for the image, not for every pixel, when we reach this point
      // the m_ChannelList MUST be valid
      assert(m_ChannelList[i] < inPixel.Size());
      outPixel[i] = inPixel[m_ChannelList[i]];
      }
    return outPixel;
  }

  virtual OutputPixelType operator ()(ScalarType inPixel) const
  {
    OutputPixelType outPixel;
    outPixel.SetSize(1);
    for (unsigned int i = 0; i < m_ChannelList.size(); ++i)
      {
      assert(m_ChannelList[i] < 1);
      outPixel[0] = inPixel;
      }
    return outPixel;
  }

  virtual OutputPixelType operator ()(const RGBPixelType& inPixel) const
  {
    OutputPixelType outPixel;
    outPixel.SetSize(m_ChannelList.size());
    for (unsigned int i = 0; i < m_ChannelList.size(); ++i)
      {
      assert(m_ChannelList[i] < 3);
      outPixel[i] = inPixel[m_ChannelList[i]];
      }
    return outPixel;
  }

  virtual OutputPixelType operator ()(const RGBAPixelType& inPixel) const
  {
    OutputPixelType outPixel;
    outPixel.SetSize(m_ChannelList.size());
    for (unsigned int i = 0; i < m_ChannelList.size(); ++i)
      {
      assert(m_ChannelList[i] < 4);
      outPixel[i] = inPixel[m_ChannelList[i]];
      }
    return outPixel;
  }

  virtual unsigned int GetOutputSize() const
  {
    return m_ChannelList.size();
  }

  virtual std::vector<unsigned int> GetChannelList() const
  {
    return m_ChannelList;
  }

  virtual void SetChannelList(std::vector<unsigned int> channels)
  {
    m_ChannelList = channels;
    usingDefaultParameters = false;
  }

  virtual void SetChannelIndex(unsigned int channelPosition, unsigned int channel)
  {
    if (m_ChannelList.size() < channelPosition + 1)
      {
      m_ChannelList.resize(channelPosition + 1, 0);
      }
    m_ChannelList[channelPosition] = channel;
    usingDefaultParameters = false;
  }

  virtual unsigned int GetChannelIndex(unsigned int channelPosition) const
  {
    if (channelPosition >= m_ChannelList.size())
      {
      itkExceptionMacro(
        << "Can't get channel " << channelPosition << ", there is only " << m_ChannelList.size() <<
        " element in the list");
      }
    return m_ChannelList[channelPosition];

  }

  /** Only for backward compatibility but should not be used*/
  virtual void SetAllChannels(unsigned int channel)
  {
    if (m_ChannelList.size() < 3)
      {
      m_ChannelList.resize(3, 0);
      }
    m_ChannelList[0] = channel;
    m_ChannelList[1] = channel;
    m_ChannelList[2] = channel;
    usingDefaultParameters = false;
  }
  virtual void SetRedChannelIndex(unsigned int channel)
  {
    if (m_ChannelList.size() < 1)
      {
      m_ChannelList.resize(3, 0);
      }
    m_ChannelList[0] = channel;
    usingDefaultParameters = false;
  }

  virtual void SetGreenChannelIndex(unsigned int channel)
  {
    if (m_ChannelList.size() < 2)
      {
      m_ChannelList.resize(3, 0);
      }
    m_ChannelList[1] = channel;
    usingDefaultParameters = false;
  }

  virtual void SetBlueChannelIndex(unsigned int channel)
  {
    if (m_ChannelList.size() < 3)
      {
      m_ChannelList.resize(3, 0);
      }
    m_ChannelList[2] = channel;
    usingDefaultParameters = false;
  }

  virtual unsigned int GetRedChannelIndex() const
  {
    return m_ChannelList[0];
  }
  virtual unsigned int GetGreenChannelIndex() const
  {
    return m_ChannelList[1];
  }
  virtual unsigned int GetBlueChannelIndex() const
  {
    return m_ChannelList[2];
  }

  virtual bool IsUsingDefaultParameters()
  {
    return usingDefaultParameters;
  }
protected:

  /** Constructor */
  ChannelSelectorFunctor() :
    usingDefaultParameters(true)
  {
    if (std::string(typeid(PixelType).name()).find("RGBAPixel") != std::string::npos)
      {
      m_ChannelList.push_back(0);
      m_ChannelList.push_back(1);
      m_ChannelList.push_back(2);
      m_ChannelList.push_back(3);
      }
    else if (std::string(typeid(PixelType).name()).find("RGBPixel") != std::string::npos)
      {
      m_ChannelList.push_back(0);
      m_ChannelList.push_back(1);
      m_ChannelList.push_back(2);
      }
    else if (std::string(typeid(PixelType).name()).find("VariableLengthVector") != std::string::npos)
      {
      m_ChannelList.push_back(0);
      }
    else
      {
      m_ChannelList.push_back(0);
      }
  }

  /** Destructor */
  virtual ~ChannelSelectorFunctor() {}

private:

  ChannelListType m_ChannelList;
  bool            usingDefaultParameters;

};

}
}

#endif
