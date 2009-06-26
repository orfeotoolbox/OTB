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


#include <cassert>
#include <iomanip>
#include <vector>


#include "otbMacro.h"
#include "otbChannelSelectorFunctor.h"
#include "otbRenderingFunction.h"

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
  class TPixelRepresentationFunction = ChannelSelectorFunctor<TPixel>,
  class TTransferFunction = Identity<
        typename itk::NumericTraits<typename itk::NumericTraits<TPixel>::ValueType>::RealType,
        typename itk::NumericTraits<typename itk::NumericTraits<TPixel>::ValueType>::RealType
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
  typedef typename itk::NumericTraits<ScalarType>::RealType       RealScalarType;
  typedef itk::VariableLengthVector<RealScalarType>               InternalPixelType;
  typedef typename Superclass::ParametersType                     ParametersType;

  /** Extrema vector */
  typedef std::vector<ScalarType>                    ExtremaVectorType;
  typedef TTransferFunction                          TransferFunctionType;
  typedef TPixelRepresentationFunction               PixelRepresentationFunctionType;

  /** Convert the input pixel to a pixel representation that can be displayed on
    *  RGB. For example, channel selection, modulus computation, etc.
    */
  virtual InternalPixelType EvaluatePixelRepresentation(const PixelType &  spixel) const
  {
    return m_PixelRepresentationFunction(spixel);
  }

  /** Convert the output of the pixel representation to a RGB pixel on unsigned char
    *  to be displayed on screen. Values are contrained to 0-255 with a transfer
    * function and a clamping operation.
    * If the pixel representation gives one band, the image is displayed on grey level
    * If it gives 3 bands, it is interpreted as Red, Green and Blue
    * If there is a fourth band, it is considered as the alpha channel and is not scaled.
    */
  virtual OutputPixelType EvaluateTransferFunction(const InternalPixelType &  spixel) const
  {
    if ((spixel.Size() != 1) && (spixel.Size() != 3) && (spixel.Size() != 4))
    {
      itkExceptionMacro( << "the PixelRepresentation function should give an output of "
       << "size 1, 3 or 4 otherwise I don't know how to make an RGB of it !" );
    }
    if (spixel.Size() != m_TransferedMinimum.size())
    {
      itkExceptionMacro( << " m_TransferedMinimum and pixel size do not correspond"
         << " spixel.Size(): " << spixel.Size()
         << " m_TransferedMinimum.size(): " << m_TransferedMinimum.size());
    }
    OutputPixelType output(255);


//     otbMsgDevMacro(<<"StandardRenderingFunction::EvaluateTransferFunction "
//            << "m_TransferFunction(spixel[0])" << m_TransferFunction(spixel[0])
//            << ", m_TransferedMinimum[0] " << m_TransferedMinimum[0]
//            << ", m_TransferedMaximum[0] " << m_TransferedMaximum[0])
    if (spixel.Size() == 1)
    {
      OutputValueType value = ClampRescale(m_TransferFunction(spixel[0]),m_TransferedMinimum[0],m_TransferedMaximum[0]);
      output[0] = value;
      output[1] = value;
      output[2] = value;
    }
    else
    {
      output[0] = ClampRescale(m_TransferFunction(spixel[0]),m_TransferedMinimum[0],m_TransferedMaximum[0]);
      output[1] = ClampRescale(m_TransferFunction(spixel[1]),m_TransferedMinimum[1],m_TransferedMaximum[1]);
      output[2] = ClampRescale(m_TransferFunction(spixel[2]),m_TransferedMinimum[2],m_TransferedMaximum[2]);
    }

    if ((spixel.Size() == 4) && (output.Size() == 4))
    {
      output[3] = spixel[3];//just copy the alpha channel
    }

    return output;
  }

  virtual unsigned int GetPixelRepresentationSize() const
  {
    return m_PixelRepresentationFunction.GetOutputSize();
  }

  virtual void Initialize()//FIXME should disappear and be automatic (IsModified())
  {
    if(m_AutoMinMax)
    {
      if ((this->GetListSample()).IsNotNull())
      {
        //the size of the Vector was unknow at construction time for the
        //m_PixelRepresentationFunction, now, we may get a better default
        if (m_PixelRepresentationFunction.IsUsingDefaultParameters())
        {
           if (this->GetListSample()->GetMeasurementVectorSize() >=3)
           {
             m_PixelRepresentationFunction.SetRedChannelIndex(0);
             m_PixelRepresentationFunction.SetGreenChannelIndex(1);
             m_PixelRepresentationFunction.SetBlueChannelIndex(2);
           }
        }
      }
      unsigned int nbComps = m_PixelRepresentationFunction.GetOutputSize();//FIXME check what happen if the m_PixelRepresentationFunction is modified AFTER the Initialize.

      otbMsgDevMacro(<<"Initialize(): "<<nbComps<<" components, quantile= "<<100*m_AutoMinMaxQuantile<<" %");
      // For each components, use the histogram to compute min and max
      m_Minimum.clear();
      m_Maximum.clear();

      if (this->GetHistogramList().IsNull())
      {
        this->RenderHistogram();
//         itkExceptionMacro( << "To Compute min/max automatically, Histogram should be "
//          <<"provided to the rendering function with SetHistogramList()" );
      }
      for(unsigned int comp = 0; comp < nbComps;++comp)
      {
        // Compute quantiles
        m_Minimum.push_back(static_cast<ScalarType>(this->GetHistogramList()->GetNthElement(comp)->Quantile(0,m_AutoMinMaxQuantile)));
        m_Maximum.push_back(static_cast<ScalarType>(this->GetHistogramList()->GetNthElement(comp)->Quantile(0,1-m_AutoMinMaxQuantile)));
        otbMsgDevMacro(<<"Initialize():"<< " component "<<comp
            <<", min= "<< static_cast< typename itk::NumericTraits<ScalarType >::PrintType>(m_Minimum.back())
            <<", max= "<<static_cast< typename itk::NumericTraits<ScalarType >::PrintType>(m_Maximum.back()));
      }

    }

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



  inline const std::string Describe(const ScalarType & spixel) const //FIXME not updated yet
  {
    itk::OStringStream oss;
    OutputPixelType output = this->Evaluate(spixel);
    oss<<"Grayscale [value: "<< static_cast<typename itk::NumericTraits<PixelType>::PrintType>(spixel)<<", displayed: "<< static_cast<unsigned int>(output[0])<<"]";
    return oss.str();
  }

  inline const std::string Describe(const VectorPixelType & vpixel) const//FIXME not updated yet
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

  inline const std::string Describe(const RGBPixelType & spixel) const //FIXME not updated yet
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

  inline const std::string Describe(const RGBAPixelType & spixel) const //FIXME not updated yet
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

   /** Set the minimum and maximum for the different bands.
     * Has to be provided as [minBand0, maxBand0, minBand1, maxBand1,...]
     */
   virtual void SetParameters( const ParametersType & parameters)
   {
     if (parameters.Size() % 2 != 0)
     {
       itkExceptionMacro( << "Min And Max should be provided for every band to display" );
     }
     for (unsigned int i=0; i< parameters.Size(); ++i)
     {
       m_Minimum.push_back(parameters[i]);
       ++i;
       m_Maximum.push_back(parameters[i]);
     }
     m_AutoMinMax = false;
     UpdateTransferedMinMax();
   }

  /** Accessor to set some specific parameters on the transfer function */
  virtual TransferFunctionType& GetTransferFunction()
  {
    return m_TransferFunction;
  }
  /** Accessor to set some specific parameters on the pixel representation function */
  virtual PixelRepresentationFunctionType& GetPixelRepresentationFunction()
  {
    return m_PixelRepresentationFunction;
  }

    /** Set/Get the AutoMinMax mode */
  itkSetMacro(AutoMinMax,bool);
  itkGetMacro(AutoMinMax,bool);
  itkBooleanMacro(AutoMinMax);


protected:
  /** Constructor */
  StandardRenderingFunction() : m_TransferedMinimum(), m_TransferedMaximum(),
                                m_RedChannelIndex(0), m_GreenChannelIndex(1), m_BlueChannelIndex(2), m_AutoMinMax(true),
                                m_AutoMinMaxQuantile(0.02), m_DefaultChannelsAreSet(false)
  {}
  /** Destructor */
  ~StandardRenderingFunction() {}
  /** Perform the computation for a single value (this is done in
   * order to have the same code for vector and scalar version)
   */
  const OutputValueType ClampRescale(RealScalarType input, RealScalarType min, RealScalarType max) const
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
      return static_cast<OutputValueType>(vcl_floor(
                                     255.*(static_cast<double>(input)-static_cast<double>(min))
                                                    /(static_cast<double>(max)-static_cast<double>(min))
                                                    +0.5));
      }
  }

  void UpdateTransferedMinMax()
  {
    if (m_Minimum.size() != m_Maximum.size())
    {
      itkExceptionMacro( << "m_Minimum and m_Maximum should have the same size" );
    }
    m_TransferedMinimum.clear();
    m_TransferedMaximum.clear();
    for (unsigned int i=0; i<m_Minimum.size(); ++i)
    {
      m_TransferedMinimum.push_back(m_TransferFunction(m_Minimum[i]));
      m_TransferedMaximum.push_back(m_TransferFunction(m_Maximum[i]));
    }
  }

private:
  StandardRenderingFunction(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

 /** Transfer function
  *  \note This member is declared mutable because some functors that
  * can be used as a transfer function but are not const correct.
  *  Since a const reference is passed to the functor anyway, it is
  * not harmful to do so and preserves const correctness of the
  *  Evaluate() methods.
  */
  mutable TransferFunctionType m_TransferFunction;
  PixelRepresentationFunctionType m_PixelRepresentationFunction;

  /** If true, values mapped by the transfert function are clamped to
      user defined min/max */
//   bool m_UserDefinedTransferedMinMax;

  /** Min and max (after pixel representation)*/
  ExtremaVectorType m_Minimum;
  ExtremaVectorType m_Maximum;

  /** Transfered min and max */
  ExtremaVectorType m_TransferedMinimum;
  ExtremaVectorType m_TransferedMaximum;

  unsigned int m_RedChannelIndex;
  unsigned int m_GreenChannelIndex;
  unsigned int m_BlueChannelIndex;

  /** Boolean for AutoMinMax*/
  bool m_AutoMinMax;

  /** Quantile used with AutoMinMax */
  double m_AutoMinMaxQuantile;

  bool m_DefaultChannelsAreSet;
};
} // end namespace Functor
} // end namespace otb

#endif


