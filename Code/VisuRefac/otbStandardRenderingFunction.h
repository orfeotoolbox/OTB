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
#ifndef __otbStandardRenderingFunction_h
#define __otbStandardRenderingFunction_h

#include "otbRenderingFunction.h"

namespace otb
{
namespace Function
{
/** \class Identiy
* \brief Default math functor parameter for rendering function.
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

/**\class StandardRenderingFunction
 * \brief Standard rendering.
 * If the input image is an Image, the function
 * renders it with R, G and B channels all equals. 
 * If it is a VectorImage, the function renders
 * the selected channels.
 */
template <class TPixelPrecision, class TRGBPixel, class TTransferFunction = Identity<TPixelPrecision,TPixelPrecision> >
class StandardRenderingFunction
  : public RenderingFunction<TPixelPrecision,TRGBPixel>
{
public:
  /** Standard class typedefs */
  typedef StandardRenderingFunction                   Self;
  typedef RenderingFunction<TPixelPrecision,TRGBPixel> Superclass;
  typedef itk::SmartPointer<Self>                      Pointer;
  typedef itk::SmartPointer<const Self>                ConstPointer;

  /** type macro */
  itkTypeMacro(StandardRenderingFunction,RenderingFunction);

  /** new macro */
  itkNewMacro(Self);

  /** PixelType macros */
  typedef TRGBPixel                                  OutputPixelType;
  typedef typename OutputPixelType::ValueType        OutputValueType;
  typedef TPixelPrecision                            ScalarPixelType;
  typedef itk::VariableLengthVector<ScalarPixelType> VectorPixelType;
  /** Extrema vector */
  typedef std::vector<ScalarPixelType>               ExtremaVectorType;
  typedef TTransferFunction                          TransferFunctionType;

  /** Evaluate method (scalar version) */
  virtual const OutputPixelType Evaluate(ScalarPixelType spixel)
  {
    // Update transfered min/max if necessary
    if(!m_TransferedMinMaxUpToDate)
      {
      this->UpdateTransferedMinMax();
      }

    OutputPixelType resp;
    resp.Fill(this->Evaluate(m_TransferFunction(spixel),m_TransferedMinimum[0],m_TransferedMaximum[0]));
    return resp;
  }
  /** Evaluate method (vector version) */
  virtual const OutputPixelType Evaluate(const VectorPixelType & vpixel)
  {
    // Update transfered min/max if necessary
    if(!m_TransferedMinMaxUpToDate)
      {
      this->UpdateTransferedMinMax();
      }
    
    OutputPixelType resp;
    resp.SetRed(Evaluate(m_TransferFunction(vpixel[m_RedChannelIndex]),m_TransferedMinimum[m_RedChannelIndex],m_TransferedMaximum[m_RedChannelIndex]));
    resp.SetBlue(Evaluate(m_TransferFunction(vpixel[m_BlueChannelIndex]),m_TransferedMinimum[m_BlueChannelIndex],m_TransferedMaximum[m_BlueChannelIndex]));
    resp.SetGreen(Evaluate(m_TransferFunction(vpixel[m_GreenChannelIndex]),m_TransferedMinimum[m_GreenChannelIndex],m_TransferedMaximum[m_GreenChannelIndex]));
    return resp;
  }

  /** Get the transfer function for tuning */
  TransferFunctionType & GetTransferFunction()
  {
    return m_TransferFunction;
  }

  /** Set the red channel index (vector mode only) */
  void SetRedChannelIndex(unsigned int index)
  {
    m_RedChannelIndex = index;
  }
  
  /** Get the red channel index (vector mode only) */
  unsigned int GetRedChannelIndex(void)
  {
    return m_RedChannelIndex;
  }

  /** Set the blue channel index (vector mode only) */
  void SetBlueChannelIndex(unsigned int index)
  {
    m_BlueChannelIndex = index;
  }
  
  /** Get the blue channel index (vector mode only) */
  unsigned int GetBlueChannelIndex(void)
  {
    return m_BlueChannelIndex;
  }

  /** Set the green channel index (vector mode only) */
  void SetGreenChannelIndex(unsigned int index)
  {
    m_GreenChannelIndex = index;
  }
  
  /** Get the green channel index (vector mode only) */
  unsigned int GetGreenChannelIndex(void)
  {
    return m_GreenChannelIndex;
  }
  
  /** Set all channels (grayscale mode) */
  void SetAllChannels(unsigned int index)
  {
    m_RedChannelIndex   = index;
    m_BlueChannelIndex  = index;
    m_GreenChannelIndex = index; 
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
  virtual void SetTransferedMinimum(ScalarPixelType spixel)
  {
    m_TransferedMinimum.clear();
    m_TransferedMinimum.push_back(spixel);
    m_UserDefinedTransferedMinMax = true;
    m_TransferedMinMaxUpToDate = true;
  }
  
  /** Set the transfered maximum (scalar version) */
  virtual void SetTransferedMaximum(ScalarPixelType spixel)
  {
    m_TransferedMaximum.clear();
    m_TransferedMaximum.push_back(spixel);
    m_UserDefinedTransferedMinMax = true;
    m_TransferedMinMaxUpToDate = true;
  }

  /** Set transfered minimum (vector version) */
  virtual void SetTransferedMinimum(const VectorPixelType & vpixel)
  {
    m_TransferedMinimum.clear();
    for(unsigned int i = 0; i < vpixel.Size();++i)
      {
      m_TransferedMinimum.push_back(vpixel[i]);
      }
    m_UserDefinedTransferedMinMax = true;
    m_TransferedMinMaxUpToDate = true;
  }

  /** Set transfered maximum (vector version) */
  virtual void SetTransferedMaximum(const VectorPixelType & vpixel)
  {
    m_TransferedMaximum.clear();
    for(unsigned int i = 0; i < vpixel.Size();++i)
      {
      m_TransferedMaximum.push_back(vpixel[i]);
      }
    m_UserDefinedTransferedMinMax = true;
    m_TransferedMinMaxUpToDate = true;
  }

  /** Set the minimum (scalar version) */
  virtual void SetMinimum(ScalarPixelType spixel)
  {
    Superclass::SetMinimum(spixel);
    m_TransferedMinMaxUpToDate = false;
  }
  
  /** Set the maximum (scalar version) */
  virtual void SetMaximum(ScalarPixelType spixel)
  {
    Superclass::SetMaximum(spixel);
    m_TransferedMinMaxUpToDate = false;
  }
  
 /** Set minimum (vector version) */
  virtual void SetMinimum(const VectorPixelType & vpixel)
  {
    Superclass::SetMinimum(vpixel);
    m_TransferedMinMaxUpToDate = false;
  }
  
  /** Set maximum (vector version) */
  virtual void SetMaximum(const VectorPixelType & vpixel)
  {
    Superclass::SetMaximum(vpixel);
    m_TransferedMinMaxUpToDate = false;
  }

/** Set minimum (std::vector version) */
  virtual void SetMinimum(const ExtremaVectorType & vpixel)
  {
    Superclass::SetMinimum(vpixel);
    m_TransferedMinMaxUpToDate = false;
  }
  
  /** Set maximum (std::vector version) */
  virtual void SetMaximum(const ExtremaVectorType & vpixel)
  {
    Superclass::SetMaximum(vpixel);
    m_TransferedMinMaxUpToDate = false;
  }

  /** Update transfered min and max */
  void UpdateTransferedMinMax()
  {
    if(!m_UserDefinedTransferedMinMax)
      {
      typename ExtremaVectorType::iterator minIt = this->m_Minimum.begin();
      typename ExtremaVectorType::iterator maxIt = this->m_Maximum.begin();
      
      m_TransferedMinimum.clear();
      m_TransferedMaximum.clear();
      
      while(minIt != this->m_Minimum.end() && maxIt != this->m_Maximum.end())
	{
	double v1 = m_TransferFunction(*minIt);
	double v2 = m_TransferFunction(*maxIt);
	m_TransferedMinimum.push_back(std::min(v1,v2));
	m_TransferedMaximum.push_back(std::max(v1,v2));
	++minIt;
	++maxIt;
	}
      }
      m_TransferedMinMaxUpToDate = true;
  }
     

protected:
  /** Constructor */
  StandardRenderingFunction() : m_RedChannelIndex(0), m_GreenChannelIndex(1), m_BlueChannelIndex(2),
				m_UserDefinedTransferedMinMax(false), m_TransferedMinMaxUpToDate(false),
				m_TransferedMinimum(), m_TransferedMaximum()
  {}
  /** Destructor */
  ~StandardRenderingFunction() {}
  /** Perform the computation for a single value (this is done in
   * order to have the same code for vector and scalar version)
   */
  const OutputValueType Evaluate(ScalarPixelType input, ScalarPixelType min, ScalarPixelType max)
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
      return static_cast<OutputValueType>(vcl_floor(255.*(static_cast<double>(input)-static_cast<double>(min))/(static_cast<double>(max)-static_cast<double>(min))+0.5));
      }
  }

private:
  StandardRenderingFunction(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** Index of the channels to display (vector mode only, has no effet
   *  on scalar mode)
   */
  unsigned int m_RedChannelIndex;
  unsigned int m_GreenChannelIndex;
  unsigned int m_BlueChannelIndex;

  /** Transfert function */
  TransferFunctionType m_TransferFunction;

  /** If true, values mapped by the transfert function are clamped to
      user defined min/max */
  bool m_UserDefinedTransferedMinMax;

  /** True if transfered min max are up-to-date.*/
  bool m_TransferedMinMaxUpToDate;

  /** Transfered min and max */
  ExtremaVectorType m_TransferedMinimum;
  ExtremaVectorType m_TransferedMaximum;
};
} // end namespace Functor
} // end namespace otb

#endif


