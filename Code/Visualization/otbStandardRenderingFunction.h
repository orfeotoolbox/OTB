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

#include <cassert>
#include <iomanip>
#include <vector>

#include "otbMacro.h"
#include "otbI18n.h"
#include "otbChannelSelectorFunctor.h"
#include "otbImageMetadataInterfaceFactory.h"
#include "otbImageMetadataInterfaceBase.h"
#include "otbRenderingFunction.h"

#include "itkMetaDataDictionary.h"

namespace otb
{

namespace Function
{

/** \class Identity
* \brief Default math functor parameter for rendering function.
*  \ingroup Visualization
 */
template <class TInputPixel, class TOutputPixel>
class Identity
{
public:
  Identity(){}
  virtual ~Identity(){}
  bool operator !=(const Identity&) const
  {
    return false;
  }
  bool operator ==(const Identity& other) const
  {
    return !(*this != other);
  }

  inline TOutputPixel operator ()(const TInputPixel& A) const
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
 *
 * In general, three main steps occur for the production of the final RGB output
 * - channel selection, by default using the ChannelSelectorFunctor;
 * - transfer function, by default an identity function;
 * - clamping of the value between 0 and 255, if necessary using the min/max
 * retrieved from the histogram.
 *
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
  typedef StandardRenderingFunction            Self;
  typedef RenderingFunction<TPixel, TRGBPixel> Superclass;
  typedef itk::SmartPointer<Self>              Pointer;
  typedef itk::SmartPointer<const Self>        ConstPointer;

  /** type macro */
  itkTypeMacro(StandardRenderingFunction, RenderingFunction);

  /** new macro */
  itkNewMacro(Self);

  /** PixelType macros */
  typedef TRGBPixel                                         OutputPixelType;
  typedef typename OutputPixelType::ValueType               OutputValueType;
  typedef TPixel                                            PixelType;
  typedef typename itk::NumericTraits<PixelType>::ValueType ScalarType;
  typedef itk::VariableLengthVector<ScalarType>             VectorPixelType;
  typedef itk::RGBPixel<ScalarType>                         RGBPixelType;
  typedef itk::RGBAPixel<ScalarType>                        RGBAPixelType;
  typedef typename itk::NumericTraits<ScalarType>::RealType RealScalarType;
  typedef itk::VariableLengthVector<RealScalarType>         InternalPixelType;
  typedef typename Superclass::ParametersType               ParametersType;

  /** Extrema vector */
  typedef std::vector<RealScalarType>                               ExtremaVectorType;
  typedef TTransferFunction                                         TransferFunctionType;
  typedef TPixelRepresentationFunction                              PixelRepresentationFunctionType;
  typedef typename PixelRepresentationFunctionType::Pointer         PixelRepresentationFunctionPointerType;
  typedef typename PixelRepresentationFunctionType::ChannelListType ChannelListType;

  typedef itk::MetaDataDictionary MetaDataDictionaryType;

  /** Convert the input pixel to a pixel representation that can be displayed on
    *  RGB. For example, channel selection, modulus computation, etc.
    */
  virtual InternalPixelType EvaluatePixelRepresentation(const PixelType&  spixel) const
  {
    return (*m_PixelRepresentationFunction)(spixel);
  }

  /** Convert the output of the pixel representation to a RGB pixel on unsigned char
    *  to be displayed on screen. Values are contrained to 0-255 with a transfer
    * function and a clamping operation.
    * If the pixel representation gives one band, the image is displayed on grey level
    * If it gives 3 bands, it is interpreted as Red, Green and Blue
    * If there is a fourth band, it is considered as the alpha channel and is not scaled.
    */
  virtual OutputPixelType EvaluateTransferFunction(const InternalPixelType&  spixel) const
  {
    if ((spixel.Size() != 1) && (spixel.Size() != 3) && (spixel.Size() != 2) && (spixel.Size() != 4))
      {
      itkExceptionMacro(<< "the PixelRepresentation function should give an output of "
                        << "size 1, 3 or 4 otherwise I don't know how to make an RGB of it !");
      }
    if (spixel.Size() != m_TransferedMinimum.size())
      {
      itkExceptionMacro(<< " m_TransferedMinimum and pixel size do not correspond"
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
      OutputValueType value = ClampRescale(m_TransferFunction(
                                             spixel[0]), m_TransferedMinimum[0], m_TransferedMaximum[0]);
      output[0] = value;
      output[1] = value;
      output[2] = value;
      }
    else
      {
      output[0] = ClampRescale(m_TransferFunction(spixel[0]), m_TransferedMinimum[0], m_TransferedMaximum[0]);
      output[1] = ClampRescale(m_TransferFunction(spixel[1]), m_TransferedMinimum[1], m_TransferedMaximum[1]);
      output[2] = ClampRescale(m_TransferFunction(spixel[2]), m_TransferedMinimum[2], m_TransferedMaximum[2]);
      }

    if ((spixel.Size() == 4) && (output.Size() == 4))
      {
      assert((spixel[3] >= 0) && (spixel[3] <= 255));
      output[3] = static_cast<OutputValueType>(spixel[3]); //just copy the alpha channel
      }

    return output;
  }

  virtual unsigned int GetPixelRepresentationSize() const
  {
    return m_PixelRepresentationFunction->GetOutputSize();
  }

  virtual void Initialize(const MetaDataDictionaryType& metadatadictionary) //FIXME should disappear and be automatic (IsModified())
  {
    if ((this->GetMTime() > m_UTime) || (this->GetPixelRepresentationFunction()->GetMTime() > m_UTime))
    //NOTE: we assume that Transfer function have no parameters
      {
      if ((this->GetListSample()).IsNotNull())
        {
        //the size of the Vector was unknown at construction time for the
        //m_PixelRepresentationFunction, now, we may get a better default
        if (m_PixelRepresentationFunction->IsUsingDefaultParameters())
          {
          //RGB rendering needs at least 3 channels
          if (this->GetListSample()->GetMeasurementVectorSize() >= 3)
            {
            typedef otb::ImageMetadataInterfaceBase ImageMetadataInterfaceType;
            ImageMetadataInterfaceType::Pointer metadataInterface = ImageMetadataInterfaceFactory::CreateIMI(
              metadatadictionary);
            std::vector<unsigned int> defaultDisplay = metadataInterface->GetDefaultDisplay();
            m_PixelRepresentationFunction->SetRedChannelIndex(defaultDisplay[0]);
            m_PixelRepresentationFunction->SetGreenChannelIndex(defaultDisplay[1]);
            m_PixelRepresentationFunction->SetBlueChannelIndex(defaultDisplay[2]);
            }
          }
        }

      unsigned int nbComps = m_PixelRepresentationFunction->GetOutputSize();
      if (m_AutoMinMax)
        {
        //FIXME check what happens if the m_PixelRepresentationFunction is modified AFTER the Initialize.

        otbMsgDevMacro(
          << "Initialize(): " << nbComps << " components, quantile= " << 100 * m_AutoMinMaxQuantile << " %");

        // For each components, use the histogram to compute min and max
        m_Minimum.clear();
        m_Maximum.clear();

        // Comment the condition cause if we change the channel list order
        // this condition doesn't allow us to recompute the histograms
        //if (this->GetHistogramList().IsNull())
        //{
        this->RenderHistogram();
        //         itkExceptionMacro( << "To Compute min/max automatically, Histogram should be "
        //          <<"provided to the rendering function with SetHistogramList()" );
        //}
        for (unsigned int comp = 0; comp < nbComps; ++comp)
          {
          // Compute quantiles
          m_Minimum.push_back(
            static_cast<ScalarType> (this->GetHistogramList()->GetNthElement(comp)->Quantile(0,
                                                                                             m_AutoMinMaxQuantile)));
          m_Maximum.push_back(static_cast<ScalarType> (this->GetHistogramList()->GetNthElement(comp)->Quantile(0, 1
                                                                                                               -
                                                                                                               m_AutoMinMaxQuantile)));
          }
        }
      else
        {
        if (m_Minimum.empty())
          {
          m_Minimum.resize(nbComps, 0);
          }
        if (m_Maximum.empty())
          {
          m_Maximum.resize(nbComps, 255);
          }
        }

      typename ExtremaVectorType::const_iterator minIt = this->m_Minimum.begin();
      typename ExtremaVectorType::const_iterator maxIt = this->m_Maximum.begin();

      m_TransferedMinimum.clear();
      m_TransferedMaximum.clear();

      while (minIt != this->m_Minimum.end() && maxIt != this->m_Maximum.end())
        {
        const double v1 = this->m_TransferFunction(*minIt);
        const double v2 = this->m_TransferFunction(*maxIt);
        m_TransferedMinimum.push_back(static_cast<ScalarType> (std::min(v1, v2)));
        m_TransferedMaximum.push_back(static_cast<ScalarType> (std::max(v1, v2)));
        ++minIt;
        ++maxIt;
        }

      m_UTime.Modified();
      }
  }

  const std::string Describe(const PixelType& spixel) const
  {
    std::ostringstream oss;
    oss << m_PixelRepresentationFunction->GetDescription() << ": ";
    typename PixelRepresentationFunctionType::ChannelListType channels;
    channels = m_PixelRepresentationFunction->GetChannelList();

    for (unsigned int i = 0; i < channels.size(); ++i)
      {
      //Describe the channel selection (numbering starts with 1)
      oss << channels[i] + 1 << " ";
      }
    oss << std::endl;

    // unsigned int inputChannels = VisualizationPixelTraits::PixelSize(spixel);

    InternalPixelType spixelRepresentation = this->EvaluatePixelRepresentation(spixel);
    OutputPixelType   spixelDisplay = this->EvaluateTransferFunction(spixelRepresentation);
    oss << otbGetTextMacro("Pixel value") << ":     "
        << static_cast<typename itk::NumericTraits<PixelType>::PrintType>(spixel) << std::endl;
    oss << otbGetTextMacro("Value computed") << ": "
        << static_cast<typename itk::NumericTraits<InternalPixelType>::PrintType>(spixelRepresentation) << std::endl;
    oss << otbGetTextMacro("Value displayed") << ": " << std::endl;
    oss << otbGetTextMacro("R") << " " << std::setw(3)
        << static_cast<typename itk::NumericTraits<OutputValueType>::PrintType>(spixelDisplay[0]) << ", ";
    oss << otbGetTextMacro("G") << " " << std::setw(3)
        << static_cast<typename itk::NumericTraits<OutputValueType>::PrintType>(spixelDisplay[1]) << ", ";
    oss << otbGetTextMacro("B") << " " << std::setw(3)
        << static_cast<typename itk::NumericTraits<OutputValueType>::PrintType>(spixelDisplay[2]);
    if (spixelDisplay.Size() == 4)
      {
      oss << ", ";
      oss << otbGetTextMacro("A") << " " << std::setw(3)
          << static_cast<typename itk::NumericTraits<OutputValueType>::PrintType>(spixelDisplay[3]);
      }
    oss << std::endl;
    return oss.str();
  }

  /** Set the minimum and maximum for the different bands.
    * Has to be provided as [minBand0, maxBand0, minBand1, maxBand1, ...]
    */
  virtual void SetParameters(const ParametersType& parameters)
  {
    //Clear the min and max vectors
    m_Minimum.clear();
    m_Maximum.clear();

    // if (parameters.Size() % 2 != 0)
    //   {
    //   itkExceptionMacro(<< "Min And Max should be provided for every band to display");
    //   }

    unsigned int nbBands = parameters.Size()/2;

    for (unsigned int i = 0; i < nbBands; ++i)
      {
      m_Minimum.push_back(parameters[2*i]);

      m_Maximum.push_back(parameters[2*i+1]);
      }

    // If number of parameters is odd, there is an additional gamma parameter
    if(parameters.Size()%2 != 0)
      {
      m_Gamma = parameters[2*nbBands];
      }

    m_AutoMinMax = false;
    UpdateTransferedMinMax();
    otbMsgDevMacro(<< "StandardRenderingFunction::SetParameters: " << m_Minimum.size() << "; " << m_Maximum.size());
    this->Modified();
  }

  /**
   * Get Parameters Min and and max for each band
   */
  virtual ParametersType GetParameters() const
  {
    unsigned int   nbBands = m_PixelRepresentationFunction->GetOutputSize();
    ParametersType param;
    param.SetSize(2 * nbBands);

    // Edit the parameters as [minBand0, maxBand0, minBand1, maxBand1, ...]
    for (unsigned int i = 0; i < nbBands; ++i)
      {
      // Min Band
      param.SetElement(2 * i, /*TransferedMinimum*/ m_Minimum[i]);
      // Max Band
      param.SetElement(2 * i + 1, /*TransferedMaximum*/ m_Maximum[i]);
      }
    return param;
  }

  virtual void SetChannelList(std::vector<unsigned int>& channels)
  {
    if (m_PixelRepresentationFunction->GetChannelList() != channels)
      {
      m_PixelRepresentationFunction->SetChannelList(channels);
      this->Modified();
      }
  }

  virtual std::vector<unsigned int> GetChannelList()
  {
    return m_PixelRepresentationFunction->GetChannelList();
  }

  /** Accessor to set some specific parameters on the transfer function */
  virtual TransferFunctionType& GetTransferFunction()
  {
    return m_TransferFunction;
  }
  /** Accessor to set some specific parameters on the pixel representation function */
  virtual PixelRepresentationFunctionPointerType& GetPixelRepresentationFunction()
  {
    return m_PixelRepresentationFunction;
  }

  /** Set/Get the AutoMinMax mode */
  virtual void SetAutoMinMax(bool val)
  {
    m_AutoMinMax = val;
    this->Modified();
  }
  itkGetMacro(AutoMinMax, bool);
  itkBooleanMacro(AutoMinMax);

  itkGetMacro(AutoMinMaxQuantile, double);

protected:
  /** Constructor */
  StandardRenderingFunction() : m_TransferedMinimum(), m_TransferedMaximum(),
    m_PixelRepresentationFunction(PixelRepresentationFunctionType::New()), m_UTime(),
    m_RedChannelIndex(0), m_GreenChannelIndex(1), m_BlueChannelIndex(2), m_AutoMinMax(true),
                                m_AutoMinMaxQuantile(0.02), m_DefaultChannelsAreSet(false), 
                                m_Gamma(1.)
  {
  }
  /** Destructor */
  virtual ~StandardRenderingFunction() {}

  /** Perform the computation for a single value (this is done in
   * order to have the same code for vector and scalar version)
   */
  virtual const OutputValueType ClampRescale(RealScalarType input, RealScalarType min, RealScalarType max) const
  {
    if (input > max)
      {
      return 255;
      }
    else if (input < min)
      {
      return 0;
      }
    else
      {
      double scaled = (static_cast<double> (input) - static_cast<double> (min))
        / (static_cast<double> (max) - static_cast<double> (min));

      return static_cast<OutputValueType> (vcl_floor(
                                             255. * vcl_pow(scaled,1./m_Gamma)
                                             + 0.5));

      }
  }

  void UpdateTransferedMinMax()
  {
    if (m_Minimum.size() != m_Maximum.size())
      {
      itkExceptionMacro(<< "m_Minimum and m_Maximum should have the same size");
      }
    m_TransferedMinimum.clear();
    m_TransferedMaximum.clear();
    for (unsigned int i = 0; i < m_Minimum.size(); ++i)
      {
      m_TransferedMinimum.push_back(m_TransferFunction(m_Minimum[i]));
      m_TransferedMaximum.push_back(m_TransferFunction(m_Maximum[i]));
      }
  }

  /** Transfered min and max */
  ExtremaVectorType m_TransferedMinimum;
  ExtremaVectorType m_TransferedMaximum;

  /** Transfer function
   *  \note This member is declared mutable because some functors that
   * can be used as a transfer function but are not const correct.
   *  Since a const reference is passed to the functor anyway, it is
   * not harmful to do so and preserves const correctness of the
   *  Evaluate() methods.
   */
  mutable TransferFunctionType m_TransferFunction;

  PixelRepresentationFunctionPointerType m_PixelRepresentationFunction;

  /** Update time */
  itk::TimeStamp m_UTime;

private:
  StandardRenderingFunction(const Self&); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  /** Min and max (after pixel representation)*/
  ExtremaVectorType m_Minimum;
  ExtremaVectorType m_Maximum;

  unsigned int m_RedChannelIndex;
  unsigned int m_GreenChannelIndex;
  unsigned int m_BlueChannelIndex;

  /** Boolean for AutoMinMax*/
  bool m_AutoMinMax;

  /** Quantile used with AutoMinMax */
  double m_AutoMinMaxQuantile;

  bool m_DefaultChannelsAreSet;

  /** Gamma value for gamma correction */
  double m_Gamma;
};
} // end namespace Functor
} // end namespace otb

#endif
