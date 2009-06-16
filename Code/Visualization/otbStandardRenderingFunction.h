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
//
=========================================================================*/
#ifndef __otbStandardRenderingFunction_h
#define __otbStandardRenderingFunction_h

#include "otbRenderingFunction.h"
#include "otbMacro.h"
#include <cassert>
#include <iomanip>
#include <vector>

#include "otbChannelSelectorFunctor.h"

namespace otb
{



namespace Function
{
/** \class Identiy
* \brief Default math functor parameter for rendering function.
*  \ingroup Visualization
 */
template <class TInputPixel, class TOutputPixel>
class Identity
{
public:
  Identity(){};
  ~Identity(){};
  bool operator !=(const Identity &) const
  {
    return false;
  }
  bool operator ==(const Identity & other) const
  {
    return !(*this != other);
  }

  inline TOutputPixel operator()(const TInputPixel & A) const
  {
    return static_cast<TOutputPixel>(A);
  }
};

/** \class StandardRenderingFunction
 * \brief Standard rendering.
 * If the input image is an Image, the function
 * renders it with R, G and B channels all equals.
 * If it is a VectorImage, the function renders
 * the selected channels.
 *  \ingroup Visualization
 */
template <class TPixel, class TRGBPixel,
  class TPixelRepresentationFunction = ChannelSelectorFunctor<
        TPixel>,
  class TTransferFunction = Identity<
        typename itk::NumericTraits<TPixel>::ValueType,
        typename itk::NumericTraits<TPixel>::ValueType
        > >
class StandardRenderingFunction
  : public RenderingFunction<TPixel, TRGBPixel>
{
public:
  /** Standard class typedefs */
  typedef StandardRenderingFunction                    Self;
  typedef RenderingFunction<TPixel,TRGBPixel>          Superclass;
  typedef itk::SmartPointer<Self>                      Pointer;
  typedef itk::SmartPointer<const Self>                ConstPointer;

  /** type macro */
  itkTypeMacro(StandardRenderingFunction,RenderingFunction);

  /** new macro */
  itkNewMacro(Self);

  /** PixelType macros */
  typedef TRGBPixel                                               OutputPixelType;
  typedef typename OutputPixelType::ValueType                     OutputValueType;
  typedef TPixel                                                  PixelType;
  typedef typename itk::NumericTraits<PixelType>::ValueType       ScalarType;
  typedef itk::VariableLengthVector<ScalarType>                   VectorPixelType;
  typedef itk::RGBPixel<ScalarType>                               RGBPixelType;
  typedef itk::RGBAPixel<ScalarType>                              RGBAPixelType;
  typedef VectorPixelType                                         InternalPixelType;

  /** Extrema vector */
  typedef std::vector<ScalarType>                    ExtremaVectorType;
  typedef TTransferFunction                          TransferFunctionType;
  typedef TPixelRepresentationFunction               PixelRepresentationFunctionType;

  InternalPixelType EvaluatePixelRepresentation(const PixelType &  spixel) const
  {
    return m_PixelRepresentationFunction(spixel);
  }

  /** This method is available to allow implementation of
   * preprocessing.
   */
  virtual void Initialize(ImageInformationType imageInformation)
  {
    switch(imageInformation)
    {
      case SCALAR:
      {
        m_PixelRepresentationFunction.SetAllChannels(0);
        break;
      }
      case TWOBANDS:
      {
        m_PixelRepresentationFunction.SetAllChannels(0);
        break;
      }
      case THREEBANDS:
      {
        m_PixelRepresentationFunction.SetRedChannelIndex(0);
        m_PixelRepresentationFunction.SetGreenChannelIndex(1);
        m_PixelRepresentationFunction.SetBlueChannelIndex(2);
        break;
      }
      case SENSORINVERTED:
      {
        // Handle Spot like channel order
        m_PixelRepresentationFunction.SetRedChannelIndex(0);//XS3
        m_PixelRepresentationFunction.SetGreenChannelIndex(1);//XS2
        m_PixelRepresentationFunction.SetBlueChannelIndex(2);//XS1
        break;
      }
      case SENSORWAVELENTHORDER:
      {
        // Handle quickbird like channel order (wavelenght order)
        m_PixelRepresentationFunction.SetRedChannelIndex(2);
        m_PixelRepresentationFunction.SetGreenChannelIndex(1);
        m_PixelRepresentationFunction.SetBlueChannelIndex(0);
        break;
      }
      default:
      {
        //Discard
        break;
      }
    }
  }


  /** Evaluate method (scalar version) */
  inline virtual const OutputPixelType Evaluate(ScalarType spixel) const
  {
    OutputPixelType resp;
    resp.Fill(itk::NumericTraits<typename OutputPixelType::ValueType>::max());
    OutputValueType value = this->ClampRescale(
                this->EvaluateTransferFunction(this->EvaluatePixelRepresentation(spixel)),
                m_TransferedMinimum[0],m_TransferedMaximum[0]);
    resp.SetRed(value);
    resp.SetGreen(value);
    resp.SetBlue(value);
    return resp;
  }
  /** Evaluate method (vector version) */
  inline virtual const OutputPixelType Evaluate(const VectorPixelType & vpixel) const
  {
    OutputPixelType resp;
    resp.Fill(itk::NumericTraits<typename OutputPixelType::ValueType>::max());
    resp.SetRed(ClampRescale(this->m_TransferFunction(vpixel[m_RedChannelIndex]),m_TransferedMinimum[m_RedChannelIndex],m_TransferedMaximum[m_RedChannelIndex]));
    resp.SetGreen(ClampRescale(this->m_TransferFunction(vpixel[m_GreenChannelIndex]),m_TransferedMinimum[m_GreenChannelIndex],m_TransferedMaximum[m_GreenChannelIndex]));
    resp.SetBlue(ClampRescale(this->m_TransferFunction(vpixel[m_BlueChannelIndex]),m_TransferedMinimum[m_BlueChannelIndex],m_TransferedMaximum[m_BlueChannelIndex]));
    return resp;
  }
  /** Evaluate method (RGB pixel version) */
  inline virtual const OutputPixelType Evaluate(const RGBPixelType & vpixel) const
  {
    OutputPixelType resp;
    resp.Fill(itk::NumericTraits<typename OutputPixelType::ValueType>::max());
    resp.SetRed(ClampRescale(this->m_TransferFunction(vpixel[m_RedChannelIndex]),m_TransferedMinimum[m_RedChannelIndex],m_TransferedMaximum[m_RedChannelIndex]));
    resp.SetGreen(ClampRescale(this->m_TransferFunction(vpixel[m_GreenChannelIndex]),m_TransferedMinimum[m_GreenChannelIndex],m_TransferedMaximum[m_GreenChannelIndex]));
    resp.SetBlue(ClampRescale(this->m_TransferFunction(vpixel[m_BlueChannelIndex]),m_TransferedMinimum[m_BlueChannelIndex],m_TransferedMaximum[m_BlueChannelIndex]));
    return resp;
  }
  /** Evaluate method (RGBA pixel version) */
  inline virtual const OutputPixelType Evaluate(const RGBAPixelType & vpixel) const
  {
    OutputPixelType resp;
//     resp.Fill(itk::NumericTraits<typename OutputPixelType::ValueType>::max());
    if (OutputPixelType::Length == 4)
    {//Propagate the alpha channel
      resp[3] = static_cast<OutputValueType>(vpixel[3]);
    }
    resp.SetRed(ClampRescale(this->m_TransferFunction(vpixel[m_RedChannelIndex]),m_TransferedMinimum[m_RedChannelIndex],m_TransferedMaximum[m_RedChannelIndex]));
    resp.SetGreen(ClampRescale(this->m_TransferFunction(vpixel[m_GreenChannelIndex]),m_TransferedMinimum[m_GreenChannelIndex],m_TransferedMaximum[m_GreenChannelIndex]));
    resp.SetBlue(ClampRescale(this->m_TransferFunction(vpixel[m_BlueChannelIndex]),m_TransferedMinimum[m_BlueChannelIndex],m_TransferedMaximum[m_BlueChannelIndex]));
    return resp;
  }

  inline const std::string Describe(const ScalarType & spixel) const
  {
    itk::OStringStream oss;
    OutputPixelType output = this->Evaluate(spixel);
    oss<<"Grayscale [value: "<< static_cast<typename itk::NumericTraits<PixelType>::PrintType>(spixel)<<", displayed: "<< static_cast<unsigned int>(output[0])<<"]";
    return oss.str();
  }

  inline const std::string Describe(const VectorPixelType & vpixel) const
  {
    itk::OStringStream oss;
    OutputPixelType output = this->Evaluate(vpixel);

    for(unsigned int channel = 0; channel < vpixel.Size();++channel)
      {
      oss<<"c= "<< channel << ", ";
      if(channel == m_RedChannelIndex)
        {
        oss <<"R= " << std::setw(3) <<(int)output[0]<< ", ";
        }
      else if(channel == m_GreenChannelIndex)
        {
        oss <<"G= " << std::setw(3) <<(int)output[1]<< ", ";
        }
      else if(channel == m_BlueChannelIndex)
        {
        oss <<"B= " << std::setw(3) <<(int)output[2]<< ", ";
        }
      else
        {
        oss <<"       ";
        }
        oss<<"v= "<<static_cast<typename itk::NumericTraits<PixelType>::PrintType>(vpixel[channel])<<std::endl;
      }
    return oss.str();
  }

  inline const std::string Describe(const RGBPixelType & spixel) const
  {
    itk::OStringStream oss;
    OutputPixelType output = this->Evaluate(spixel);
    oss<<"RGB value: "<< static_cast<typename itk::NumericTraits<ScalarType>::PrintType>(spixel[0])
        << ", "<< static_cast<typename itk::NumericTraits<ScalarType>::PrintType>(spixel[1])
        << ", "<< static_cast<typename itk::NumericTraits<ScalarType>::PrintType>(spixel[2])
        << std::endl;
    oss <<"   displayed: "<< static_cast<typename itk::NumericTraits<OutputPixelType>::PrintType>(output[0])
        << ", "<< static_cast<typename itk::NumericTraits<OutputPixelType>::PrintType>(output[1])
        << ", "<< static_cast<typename itk::NumericTraits<OutputPixelType>::PrintType>(output[2])
        <<std::endl;
    return oss.str();
  }

  inline const std::string Describe(const RGBAPixelType & spixel) const
  {
    itk::OStringStream oss;
    OutputPixelType output = this->Evaluate(spixel);
    oss<<"RGBA value: "<< static_cast<typename itk::NumericTraits<ScalarType>::PrintType>(spixel[0])
        << ", "<< static_cast<typename itk::NumericTraits<ScalarType>::PrintType>(spixel[1])
        << ", "<< static_cast<typename itk::NumericTraits<ScalarType>::PrintType>(spixel[2])
        << " alpha: "<< static_cast<typename itk::NumericTraits<ScalarType>::PrintType>(spixel[3])
        << std::endl;
    oss <<"   displayed: "<< static_cast<typename itk::NumericTraits<OutputPixelType>::PrintType>(output[0])
        << ", "<< static_cast<typename itk::NumericTraits<OutputPixelType>::PrintType>(output[1])
        << ", "<< static_cast<typename itk::NumericTraits<OutputPixelType>::PrintType>(output[2])
        << " alpha: "<< static_cast<typename itk::NumericTraits<OutputPixelType>::PrintType>(output[3])
        <<std::endl;
    return oss.str();
  }


  /** Togle the UserDefinedTransferedMinMax mode */
  void SetUserDefinedTransferedMinMax(bool val)
  {
    m_UserDefinedTransferedMinMax = val;
  }

  /** Togle the UserDefinedTransferedMinMax mode */
  bool GetUserDefinedTransferedMinMax(void)
  {
    return m_UserDefinedTransferedMinMax;
  }

  /** Set the transfered minimum (scalar version) */
  virtual void SetTransferedMinimum(ScalarType spixel)
  {
    m_TransferedMinimum.clear();
    m_TransferedMinimum.push_back(spixel);
  }

  /** Set the transfered maximum (scalar version) */
  virtual void SetTransferedMaximum(ScalarType spixel)
  {
    m_TransferedMaximum.clear();
    m_TransferedMaximum.push_back(spixel);
  }

  /** Set transfered minimum (vector version) */
  virtual void SetTransferedMinimum(const VectorPixelType & vpixel)
  {
    m_TransferedMinimum.clear();
    for(unsigned int i = 0; i < vpixel.Size();++i)
      {
      m_TransferedMinimum.push_back(vpixel[i]);
      }
  }

  /** Set transfered maximum (vector version) */
  virtual void SetTransferedMaximum(const VectorPixelType & vpixel)
  {
    m_TransferedMaximum.clear();
    for(unsigned int i = 0; i < vpixel.Size();++i)
      {
      m_TransferedMaximum.push_back(vpixel[i]);
      }
  }

  /** Update transfered min and max */
  virtual void Initialize()
  {
    if(!m_UserDefinedTransferedMinMax)
      {
      typename ExtremaVectorType::const_iterator minIt = this->m_Minimum.begin();
      typename ExtremaVectorType::const_iterator maxIt = this->m_Maximum.begin();

      m_TransferedMinimum.clear();
      m_TransferedMaximum.clear();

      while(minIt != this->m_Minimum.end() && maxIt != this->m_Maximum.end())
        {
        const double v1 = this->m_TransferFunction(*minIt);
        const double v2 = this->m_TransferFunction(*maxIt);
        m_TransferedMinimum.push_back(static_cast<ScalarType>(std::min(v1,v2)));
        m_TransferedMaximum.push_back(static_cast<ScalarType>(std::max(v1,v2)));
        ++minIt;
        ++maxIt;
        }
      }
  }

protected:
  /** Constructor */
  StandardRenderingFunction() : m_UserDefinedTransferedMinMax(false),         m_TransferedMinimum(), m_TransferedMaximum(),
                                m_RedChannelIndex(0), m_GreenChannelIndex(1), m_BlueChannelIndex(2)
  {}
  /** Destructor */
  ~StandardRenderingFunction() {}
  /** Perform the computation for a single value (this is done in
   * order to have the same code for vector and scalar version)
   */
  const OutputValueType ClampRescale(ScalarType input, ScalarType min, ScalarType max) const //FIXME should it be part of the transfer function
  {
    if(input > max)
      {
      return 255;
      }
    else if(input < min)
      {
      return 0;
      }
    else
      {
      return static_cast<OutputValueType>(vcl_floor(255.*(static_cast<double>(input)-static_cast<double>(min))
                                                    /(static_cast<double>(max)-static_cast<double>(min))+0.5));
      }
  }

private:
  StandardRenderingFunction(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

//  /** Transfer function
//   *  \note This member is declared mutable because some functors that
//   * can be used as a transfer function but are not const correct.
//  *  Since a const reference is passed to the functor anyway, it is
//   * not harmful to do so and preserves const correctness of the
//   *  Evaluate() methods.
//   */
//   mutable TransferFunctionType this->m_TransferFunction;
  TransferFunctionType m_TranferFunction;
  PixelRepresentationFunctionType m_PixelRepresentationFunction;

  /** If true, values mapped by the transfert function are clamped to
      user defined min/max */
  bool m_UserDefinedTransferedMinMax;

  /** Transfered min and max */
  ExtremaVectorType m_TransferedMinimum;
  ExtremaVectorType m_TransferedMaximum;

  unsigned int m_RedChannelIndex;
  unsigned int m_GreenChannelIndex;
  unsigned int m_BlueChannelIndex;

};
} // end namespace Functor
} // end namespace otb

#endif


