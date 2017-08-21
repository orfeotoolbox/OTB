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

#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"

#include "otbVectorRescaleIntensityImageFilter.h"
#include "itkCastImageFilter.h"
#include "otbUnaryImageFunctorWithVectorImageFilter.h"
#include "otbStreamingShrinkImageFilter.h"
#include "itkListSample.h"
#include "otbListSampleToHistogramListGenerator.h"
#include "itkImageRegionConstIterator.h"

#include "otbImageListToVectorImageFilter.h"
#include "otbMultiToMonoChannelExtractROI.h"
#include "otbImageList.h"

namespace otb
{
namespace Wrapper
{

namespace Functor
{
template< class TScalar >
class ITK_EXPORT LogFunctor
{
public:
  LogFunctor(){};
  ~LogFunctor(){};
  TScalar operator() (const TScalar& v) const
  {
    return vcl_log(v);
  }
};
} // end namespace Functor


class Convert : public Application
{
public:
  /** Standard class typedefs. */
  typedef Convert                       Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(Convert, otb::Application);

  /** Filters typedef */
  typedef itk::Statistics::ListSample<FloatVectorImageType::PixelType> ListSampleType;
  typedef itk::Statistics::DenseFrequencyContainer2 DFContainerType;
  typedef ListSampleToHistogramListGenerator<ListSampleType,
                                             FloatVectorImageType::InternalPixelType,
                                             DFContainerType> HistogramsGeneratorType;
  typedef StreamingShrinkImageFilter<FloatVectorImageType,
                                     FloatVectorImageType> ShrinkFilterType;
  typedef Functor::LogFunctor<FloatVectorImageType::InternalPixelType> TransferLogFunctor;
  typedef UnaryImageFunctorWithVectorImageFilter<FloatVectorImageType,
                                                 FloatVectorImageType,
                                                 TransferLogFunctor>   TransferLogType;

  typedef otb::ImageList<FloatImageType>                               ImageListType;
  typedef ImageListToVectorImageFilter<ImageListType,
                                       FloatVectorImageType >          ListConcatenerFilterType;
  typedef MultiToMonoChannelExtractROI<FloatVectorImageType::InternalPixelType,
                                       FloatImageType::PixelType>      ExtractROIFilterType;
  typedef ObjectList<ExtractROIFilterType>                             ExtractROIFilterListType;


private:

  std::string m_channelMode;

  Convert() : m_channelMode("default") {}


  void DoInit() ITK_OVERRIDE
  {
    SetName("Convert");
    SetDescription("Convert an image to a different format, optionally rescaling the data"
                   " and/or changing the pixel type.");
    // Documentation
    SetDocName("Image Conversion");
    SetDocLongDescription("This application performs an image pixel type conversion "
      " (short, ushort, uchar, int, uint, float and double types are handled). "
      "The output image is written in the specified format (ie. that corresponds "
      "to the given extension).\n The conversion can include a rescale using "
      "the image 2 percent minimum and maximum values. The rescale can be linear or log2.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("Rescale");

    AddDocTag(Tags::Manip);
    AddDocTag("Conversion");
    AddDocTag("Image Dynamic");

    AddParameter(ParameterType_InputImage,  "in",   "Input image");
    SetParameterDescription("in", "Input image");

    AddParameter(ParameterType_Choice, "type", "Rescale type");
    SetParameterDescription("type", "Transfer function for the rescaling");
    AddChoice("type.none", "None");
    AddChoice("type.linear", "Linear");
    AddChoice("type.log2", "Log2");
    SetParameterString("type", "none", false);

    AddParameter(ParameterType_Float,"type.linear.gamma","Gamma correction factor");
    SetParameterDescription("type.linear.gamma","Gamma correction factor");
    SetDefaultParameterFloat("type.linear.gamma",1.0);
    MandatoryOff("type.linear.gamma");

    AddParameter(ParameterType_InputImage,  "mask",   "Input mask");
    SetParameterDescription("mask", "The masked pixels won't be used to adapt the dynamic "
      "(the mask must have the same dimensions as the input image)");
    MandatoryOff("mask");
    DisableParameter("mask");

    AddParameter(ParameterType_Group,"hcp","Histogram Cutting Parameters");
    SetParameterDescription("hcp","Parameters to cut the histogram edges before rescaling");

    AddParameter(ParameterType_Float, "hcp.high", "High Cut Quantile");
    SetParameterDescription("hcp.high", "Quantiles to cut from histogram high values "
      "before computing min/max rescaling (in percent, 2 by default)");
    MandatoryOff("hcp.high");
    SetDefaultParameterFloat("hcp.high", 2.0);
    DisableParameter("hcp.high");

    AddParameter(ParameterType_Float, "hcp.low", "Low Cut Quantile");
    SetParameterDescription("hcp.low", "Quantiles to cut from histogram low values "
      "before computing min/max rescaling (in percent, 2 by default)");
    MandatoryOff("hcp.low");
    SetDefaultParameterFloat("hcp.low", 2.0);
    DisableParameter("hcp.low");

    AddParameter(ParameterType_OutputImage, "out",  "Output Image");
    SetParameterDescription("out", "Output image");
    SetDefaultOutputPixelType("out",ImagePixelType_uint8);

    // TODO add parameter descriptions 
    AddParameter(ParameterType_Choice, "channels", "Channels selection");
    SetParameterDescription("channels", "Channels selection");

    AddChoice("channels.default", "Default mode");
    SetParameterDescription("channels.default", "Select all bands in the input image.");

    AddChoice("channels.mono", "Channels selection ...");

    AddChoice("channels.rgb", "Channels selection RGB");
    AddParameter(ParameterType_Int, "channels.rgb.red", "Red Channel");
    SetParameterDescription("channels.rgb.red", "TODO");
    SetDefaultParameterInt("channels.rgb.red", 1);
    AddParameter(ParameterType_Int, "channels.rgb.green", "Green Channel");
    SetParameterDescription("channels.rgb.green", "TODO");
    SetDefaultParameterInt("channels.rgb.green", 2);
    AddParameter(ParameterType_Int, "channels.rgb.blue", "Blue Channel");
    SetParameterDescription("channels.rgb.blue", "TODO");
    SetDefaultParameterInt("channels.rgb.blue", 3);

    m_ExtractorList = ExtractROIFilterListType::New();
    m_ImageList = ImageListType::New();
    m_Concatener = ListConcatenerFilterType::New();

    AddRAMParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "QB_Toulouse_Ortho_XS.tif");
    SetDocExampleParameterValue("out", "otbConvertWithScalingOutput.png");
    SetDocExampleParameterValue("type", "linear");
    SetDocExampleParameterValue("channels", "rgb");

    SetOfficialDocLink();
  }

  void DoUpdateParameters() ITK_OVERRIDE
  {
    m_ImageList = ImageListType::New();
    m_Concatener = ListConcatenerFilterType::New();
    m_ExtractorList = ExtractROIFilterListType::New();
  }

  template<class TImageType>
  void GenericDoExecute()
  {
    m_channelMode = GetParameterString("channels");
    std::string rescaleType = this->GetParameterString("type");

    typename TImageType::Pointer castIm;

    if( (rescaleType != "none") && (rescaleType != "linear") && (rescaleType != "log2") )
      {
      itkExceptionMacro("Unknown rescale type "<<rescaleType<<".");
      }

    if( rescaleType == "none" )
      {
      castIm = this->GetParameterImage<TImageType>("in");
      }
    else // linear or log2
      {
      m_Input = this->GetParameterImage("in");

      FloatVectorImageType::Pointer mask;
      bool useMask = false;
      if (IsParameterEnabled("mask"))
        {
        mask = this->GetParameterImage("mask");
        useMask = true;
        }

      // seleted channel
      GetSelectedChannels();

      const unsigned int nbComp(m_TempImage->GetNumberOfComponentsPerPixel());

      typedef otb::VectorRescaleIntensityImageFilter<FloatVectorImageType, TImageType> RescalerType;
      typename TImageType::PixelType minimum;
      typename TImageType::PixelType maximum;
      minimum.SetSize(nbComp);
      maximum.SetSize(nbComp);
      minimum.Fill( itk::NumericTraits<typename TImageType::InternalPixelType>::min() );
      maximum.Fill( itk::NumericTraits<typename TImageType::InternalPixelType>::max() );

      typename RescalerType::Pointer rescaler = RescalerType::New();

      rescaler->SetOutputMinimum(minimum);
      rescaler->SetOutputMaximum(maximum);

      // We need to subsample the input image in order to estimate its
      // histogram

      typename ShrinkFilterType::Pointer shrinkFilter = ShrinkFilterType::New();

      // Shrink factor is computed so as to load a quicklook of 1000
      // pixels square at most
      typename FloatVectorImageType::SizeType imageSize = m_TempImage->GetLargestPossibleRegion().GetSize();
      unsigned int shrinkFactor =
        std::max(imageSize[0], imageSize[1]) < 1000 ? 1 : std::max(imageSize[0], imageSize[1])/1000;

      otbAppLogDEBUG( << "Shrink factor used to compute Min/Max: "<<shrinkFactor );

      otbAppLogDEBUG( << "Shrink starts..." );

      shrinkFilter->SetShrinkFactor(shrinkFactor);
      shrinkFilter->GetStreamer()->SetAutomaticAdaptativeStreaming(GetParameterInt("ram"));
      AddProcess(shrinkFilter->GetStreamer(), "Computing shrink Image for min/max estimation...");

      if ( rescaleType == "log2")
        {
        //define the transfer log
        m_TransferLog = TransferLogType::New();
        m_TransferLog->SetInput(m_TempImage);
        m_TransferLog->UpdateOutputInformation();

        shrinkFilter->SetInput(m_TransferLog->GetOutput());
        rescaler->SetInput(m_TransferLog->GetOutput());
        shrinkFilter->Update();
        }
      else
        {
        shrinkFilter->SetInput(m_TempImage);
        rescaler->SetInput(m_TempImage);
        shrinkFilter->Update();
        }

      ShrinkFilterType::Pointer maskShrinkFilter = ShrinkFilterType::New();
      if (useMask)
        {
        maskShrinkFilter->SetShrinkFactor(shrinkFactor);
        maskShrinkFilter->SetInput(mask);
        maskShrinkFilter->GetStreamer()->SetAutomaticAdaptativeStreaming(GetParameterInt("ram"));
        maskShrinkFilter->Update();
        }

      otbAppLogDEBUG( << "Shrink done" );


      otbAppLogDEBUG( << "Evaluating input Min/Max..." );
      itk::ImageRegionConstIterator<FloatVectorImageType>
        it(shrinkFilter->GetOutput(), shrinkFilter->GetOutput()->GetLargestPossibleRegion());
      itk::ImageRegionConstIterator<FloatVectorImageType> itMask;
      if (useMask)
        {
        itMask = itk::ImageRegionConstIterator<FloatVectorImageType>(
          maskShrinkFilter->GetOutput(),maskShrinkFilter->GetOutput()->GetLargestPossibleRegion());
        }

      typename ListSampleType::Pointer listSample = ListSampleType::New();
      listSample->SetMeasurementVectorSize(m_TempImage->GetNumberOfComponentsPerPixel());

      // Now we generate the list of samples
      if (useMask)
        {
        // Remove masked pixels
        it.GoToBegin();
        itMask.GoToBegin();
        while (!it.IsAtEnd())
          {
          // float values, so the threshold is set to 0.5
          if (itMask.Get()[0] < 0.5)
            {
            listSample->PushBack(it.Get());
            }
          ++it;
          ++itMask;
          }
        }
      else
        {
        for(it.GoToBegin(); !it.IsAtEnd(); ++it)
          {
          listSample->PushBack(it.Get());
          }
        }

      // if all pixels were masked, we assume a wrong mask and then include all image
      if (listSample->Size() == 0)
        {
        otbAppLogINFO( << "All pixels were masked, the application assume a wrong mask "
          "and include all the image");
        for(it.GoToBegin(); !it.IsAtEnd(); ++it)
          {
          listSample->PushBack(it.Get());
          }
        }

      // And then the histogram
      typename HistogramsGeneratorType::Pointer histogramsGenerator = HistogramsGeneratorType::New();
      histogramsGenerator->SetListSample(listSample);
      histogramsGenerator->SetNumberOfBins(255);
      histogramsGenerator->NoDataFlagOn();
      histogramsGenerator->Update();

      // And extract the lower and upper quantile
      typename FloatVectorImageType::PixelType inputMin(nbComp), inputMax(nbComp);

      for(unsigned int i = 0; i < nbComp; ++i)
        {
        inputMin[i] =
          histogramsGenerator->GetOutput()->GetNthElement(i)->Quantile(0,
                                                                0.01 * GetParameterFloat("hcp.low"));
        inputMax[i] =
          histogramsGenerator->GetOutput()->GetNthElement(i)->Quantile(0,
                                                                1.0 - 0.01 * GetParameterFloat("hcp.high"));
        }

      otbAppLogDEBUG( << std::setprecision(5) << "Min/Max computation done : min=" << inputMin
                      << " max=" << inputMax );

      rescaler->AutomaticInputMinMaxComputationOff();
      rescaler->SetInputMinimum(inputMin);
      rescaler->SetInputMaximum(inputMax);

      if ( rescaleType == "linear")
        {
        rescaler->SetGamma(GetParameterFloat("type.linear.gamma"));
        }

      m_TmpFilter = rescaler;
      castIm = rescaler->GetOutput();

      }

    SetParameterOutputImage<TImageType>("out", castIm);
  }


  // TODO comment function
  void GetChannels()
  {
    m_Channels.clear();

    m_Input->UpdateOutputInformation();
    int nbChan = GetParameterImage("in")->GetNumberOfComponentsPerPixel();

    if(m_channelMode == "mono")
    {
      m_Channels = {1,1,1};
    }
    else if (m_channelMode == "rgb")
    {
      if ((GetParameterInt("channels.rgb.red") <= nbChan)
      && ( GetParameterInt("channels.rgb.green") <= nbChan)
      && ( GetParameterInt("channels.rgb.blue")   <= nbChan))
      {
        m_Channels = {GetParameterInt("channels.rgb.red"),
                      GetParameterInt("channels.rgb.green"),
                      GetParameterInt("channels.rgb.blue")};
      }
      else
      {
        itkExceptionMacro(<< "At least one needed channel has an invalid index");
      }
    }
    else if (m_channelMode == "default")
    {
      // take all bands
    }
  }

  // TODO comment
  void GetSelectedChannels()
  {
    const bool monoChannel = IsParameterEnabled("channels.mono");
    if (IsParameterEnabled("channels.rgb") || monoChannel)
    {
      otbAppLogINFO( << "Select channels ...");
      GetChannels();

      m_Input->UpdateOutputInformation();

      for (std::vector<int>::iterator j=m_Channels.begin(); j!=m_Channels.end(); ++j)
      {
        ExtractROIFilterType::Pointer extractROIFilter = ExtractROIFilterType::New();
        extractROIFilter->SetInput(m_Input);

        if (!monoChannel) extractROIFilter->SetChannel((*j)+1);
        extractROIFilter->UpdateOutputInformation();
        m_ExtractorList->PushBack(extractROIFilter);
        m_ImageList->PushBack(extractROIFilter->GetOutput());
      }
      m_Concatener->SetInput(m_ImageList);
      m_Concatener->UpdateOutputInformation();

      m_TempImage = m_Concatener->GetOutput();
    }
    else
    {
      m_TempImage = m_Input;
    }
  }

  void DoExecute() ITK_OVERRIDE
  {
    switch ( this->GetParameterOutputImagePixelType("out") )
      {
      case ImagePixelType_uint8:
        GenericDoExecute<UInt8VectorImageType>();
        break;
      case ImagePixelType_int16:
        GenericDoExecute<Int16VectorImageType>();
        break;
      case ImagePixelType_uint16:
        GenericDoExecute<UInt16VectorImageType>();
        break;
      case ImagePixelType_int32:
        GenericDoExecute<Int32VectorImageType>();
        break;
      case ImagePixelType_uint32:
        GenericDoExecute<UInt32VectorImageType>();
        break;
      case ImagePixelType_float:
        GenericDoExecute<FloatVectorImageType>();
        break;
      case ImagePixelType_double:
        GenericDoExecute<DoubleVectorImageType>();
        break;
      default:
        itkExceptionMacro("Unknown pixel type "<<this->GetParameterOutputImagePixelType("out")<<".");
        break;
      }
  }

  itk::ProcessObject::Pointer m_TmpFilter;
  TransferLogType::Pointer m_TransferLog;
  std::vector<int> m_Channels;
  FloatVectorImageType::Pointer m_Input;
  FloatVectorImageType::Pointer m_TempImage;
  ImageListType::Pointer        m_ImageList;
  ListConcatenerFilterType::Pointer  m_Concatener;
  ExtractROIFilterListType::Pointer  m_ExtractorList;
};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::Convert)

