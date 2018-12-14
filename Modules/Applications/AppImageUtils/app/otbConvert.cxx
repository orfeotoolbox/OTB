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

#include <numeric>

#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"

#include "otbVectorRescaleIntensityImageFilter.h"
#include "otbFunctorImageFilter.h"
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

private:

  void DoInit() override
  {
    SetName("Convert");
    SetDescription("Convert an image to a different format, optionally rescaling the data"
                   " and/or changing the pixel type.");
    // Documentation
    SetDocName("Image Conversion");
    SetDocLongDescription("This application performs an image pixel type conversion "
      "(short, ushort, uchar, int, uint, float and double types are handled). "
      "The output image is written in the specified format (ie. that corresponds "
      "to the given extension).\n The conversion can include a rescale of the data range, "
      "by default it's set from 2% to 98% of the data values. The rescale can be linear or log2. \n "
      "The choice of the output channels can be done with the extended filename, "
      "but less easy to handle. To do this, a 'channels' parameter allows you to select "
      "the desired bands at the output. There are 3 modes, the available choices are: \n"
        " * grayscale :  to display mono image as standard color image \n"
        " * rgb : select 3 bands in the input image (multi-bands) \n"
        " * all : keep all bands.");
    SetDocLimitations("The application does not support complex pixel types as output.");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("Rescale");

    AddDocTag(Tags::Deprecated);
    AddDocTag(Tags::Manip);
    AddDocTag("Conversion");
    AddDocTag("Image Dynamic");

    AddParameter(ParameterType_InputImage, "in", "Input image");
    SetParameterDescription("in", "Input image");

    AddParameter(ParameterType_Choice, "type", "Rescale type");
    SetParameterDescription("type", "Transfer function for the rescaling");
    AddChoice("type.none", "None");
    AddChoice("type.linear", "Linear");
    AddChoice("type.log2", "Log2");
    SetParameterString("type", "none");

    AddParameter(ParameterType_Float,"type.linear.gamma","Gamma correction factor");
    SetParameterDescription("type.linear.gamma","Gamma correction factor");
    SetDefaultParameterFloat("type.linear.gamma",1.0);
    MandatoryOff("type.linear.gamma");

    AddParameter(ParameterType_InputImage,  "mask",   "Input mask");
    SetParameterDescription("mask",
      "Optional mask to indicate which pixels are valid for computing the histogram quantiles. "
      "Only pixels where the mask is less than 0.5 will contribute to the histogram. "
      "The mask must have the same dimensions as the input image.");
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

    AddParameter(ParameterType_Choice, "channels", "Channels selection");
    SetParameterDescription("channels", "It's possible to select the channels "
      "of the output image. There are 3 modes, the available choices are:");

    AddChoice("channels.all", "Default mode");
    SetParameterDescription("channels.all", "Select all bands in the input image, (1,...,n).");

    AddChoice("channels.grayscale", "Grayscale mode");
    SetParameterDescription("channels.grayscale", "Display single channel as standard color image.");
    AddParameter(ParameterType_Int, "channels.grayscale.channel", "Grayscale channel");
    SetDefaultParameterInt("channels.grayscale.channel", 1);
    SetMinimumParameterIntValue("channels.grayscale.channel", 1);

    AddChoice("channels.rgb", "RGB composition");
    SetParameterDescription("channels.rgb", "Select 3 bands in the input image "
      "(multi-bands), by default (1,2,3).");

    AddParameter(ParameterType_Int, "channels.rgb.red", "Red Channel");
    SetParameterDescription("channels.rgb.red", "Red channel index.");
    SetMinimumParameterIntValue("channels.rgb.red", 1);
    AddParameter(ParameterType_Int, "channels.rgb.green", "Green Channel");
    SetParameterDescription("channels.rgb.green", "Green channel index.");
    SetMinimumParameterIntValue("channels.rgb.green", 1);
    AddParameter(ParameterType_Int, "channels.rgb.blue", "Blue Channel");
    SetParameterDescription("channels.rgb.blue", "Blue channel index.");
    SetMinimumParameterIntValue("channels.rgb.blue", 1);

    AddRAMParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "QB_Toulouse_Ortho_XS.tif");
    SetDocExampleParameterValue("out", "otbConvertWithScalingOutput.png");
    SetDocExampleParameterValue("type", "linear");
    SetDocExampleParameterValue("channels", "rgb");

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {
    // Read information
    if ( HasValue("in") )
      {
      typedef otb::ImageMetadataInterfaceBase ImageMetadataInterfaceType;
      ImageMetadataInterfaceType::Pointer metadataInterface =
      ImageMetadataInterfaceFactory::CreateIMI(GetParameterImage("in")->GetMetaDataDictionary());

      int nbBand = GetParameterImage("in")->GetNumberOfComponentsPerPixel();
      SetMaximumParameterIntValue("channels.grayscale.channel", nbBand);
      SetMaximumParameterIntValue("channels.rgb.red", nbBand);
      SetMaximumParameterIntValue("channels.rgb.green", nbBand);
      SetMaximumParameterIntValue("channels.rgb.blue", nbBand);

      if (nbBand > 1)
        {
        // get band index : Red/Green/Blue
        int bandRed = metadataInterface->GetDefaultDisplay()[0] + 1;
        int bandGreen = metadataInterface->GetDefaultDisplay()[1] + 1;
        int bandBlue = metadataInterface->GetDefaultDisplay()[2] + 1;
        SetDefaultParameterInt("channels.rgb.red", bandRed);
        SetDefaultParameterInt("channels.rgb.green", bandGreen);
        SetDefaultParameterInt("channels.rgb.blue", bandBlue);
        }
      }


  }

  template<class TImageType>
  void GenericDoExecute()
  {

    // Clear previously registered filters
    m_Filters.clear();
    
    std::string rescaleType = this->GetParameterString("type");

    if( (rescaleType != "none") && (rescaleType != "linear") && (rescaleType != "log2") )
      {
      itkExceptionMacro("Unknown rescale type "<<rescaleType<<".");
      }

    if( rescaleType == "none" )
      {
      // selected channel
      typename TImageType::Pointer tempImage;
      tempImage = GetSelectedChannels<TImageType>();

      SetParameterOutputImage<TImageType>("out", tempImage);

      }
    else // linear or log2
      {
      FloatVectorImageType::Pointer mask;

      if (IsParameterEnabled("mask")) mask = this->GetParameterImage("mask");

      // selected channel
      typename FloatVectorImageType::Pointer tempImage;
      tempImage = GetSelectedChannels<FloatVectorImageType>();

      const unsigned int nbComp(tempImage->GetNumberOfComponentsPerPixel());

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
      typename FloatVectorImageType::SizeType imageSize = tempImage->GetLargestPossibleRegion().GetSize();
      unsigned int shrinkFactor =
        std::max(imageSize[0], imageSize[1]) < 1000 ? 1 : std::max(imageSize[0], imageSize[1])/1000;

      otbAppLogDEBUG( << "Shrink factor used to compute Min/Max: "<<shrinkFactor );

      otbAppLogDEBUG( << "Shrink starts..." );

      shrinkFilter->SetShrinkFactor(shrinkFactor);
      shrinkFilter->GetStreamer()->SetAutomaticAdaptativeStreaming(GetParameterInt("ram"));
      AddProcess(shrinkFilter->GetStreamer(), "Computing shrink Image for min/max estimation...");

      if ( rescaleType == "log2")
        {
	  // define lambda function that applies a log to all bands of the input pixel
	  auto logFunction = [](FloatVectorImageType::PixelType & vectorOut, const FloatVectorImageType::PixelType & vectorIn) {
	    assert(vectorOut.Size() == vectorIn.Size() && "Input vector types don't have the same size");

	    for (unsigned int i = 0; i < vectorIn.Size() ; i++) {
	      vectorOut[i] = std::log(vectorIn[i]);
	    }
	    
	  };
	  // creates functor filter
	  auto transferLogFilter = NewFunctorFilter(logFunction,tempImage->GetNumberOfComponentsPerPixel(),{{0,0}});

	  // save a reference to the functor
	  m_Filters.push_back(transferLogFilter.GetPointer());
	  
	  transferLogFilter->SetVariadicInputs(tempImage);
	  transferLogFilter->UpdateOutputInformation();
	
	  shrinkFilter->SetInput(transferLogFilter->GetOutput());
	  rescaler->SetInput(transferLogFilter->GetOutput());
	  shrinkFilter->Update();
        }
      else
        {
	  shrinkFilter->SetInput(tempImage);
	  rescaler->SetInput(tempImage);
	  shrinkFilter->Update();
        }
      
      ShrinkFilterType::Pointer maskShrinkFilter = ShrinkFilterType::New();

      otbAppLogDEBUG( << "Evaluating input Min/Max..." );
      itk::ImageRegionConstIterator<FloatVectorImageType>
        it(shrinkFilter->GetOutput(), shrinkFilter->GetOutput()->GetLargestPossibleRegion());
      itk::ImageRegionConstIterator<FloatVectorImageType> itMask;

      typename ListSampleType::Pointer listSample = ListSampleType::New();
      listSample->SetMeasurementVectorSize(tempImage->GetNumberOfComponentsPerPixel());

      // Now we generate the list of samples
      if (IsParameterEnabled("mask"))
        {
        maskShrinkFilter->SetShrinkFactor(shrinkFactor);
        maskShrinkFilter->SetInput(mask);
        maskShrinkFilter->GetStreamer()->SetAutomaticAdaptativeStreaming(GetParameterInt("ram"));
        maskShrinkFilter->Update();

        itMask = itk::ImageRegionConstIterator<FloatVectorImageType>(
          maskShrinkFilter->GetOutput(),maskShrinkFilter->GetOutput()->GetLargestPossibleRegion());

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
        if (listSample->Size() == 0)
          {
          otbAppLogINFO( << "All pixels were masked, the application assume a wrong mask "
            "and include all the image");
          }
        }

      // if mask is disable and all pixels were masked
      if ((!IsParameterEnabled("mask")) || (listSample->Size() == 0))
        {
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

      auto histOutput = histogramsGenerator->GetOutput();
      assert(histOutput);

      for(unsigned int i = 0; i < nbComp; ++i)
        {
        auto && elm = histOutput->GetNthElement(i);
        assert(elm);
        inputMin[i] = elm->Quantile(0, 0.01 * GetParameterFloat("hcp.low"));
        inputMax[i] = elm->Quantile(0, 1.0 - 0.01 * GetParameterFloat("hcp.high"));
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

      m_Filters.push_back(rescaler.GetPointer());

      SetParameterOutputImage<TImageType>("out", rescaler->GetOutput());
      }
  }

  // Get the bands order
  std::vector<int> GetChannels()
  {
    std::vector<int> channels;

    int nbChan = GetParameterImage("in")->GetNumberOfComponentsPerPixel();
    std::string channelMode = GetParameterString("channels");

    if(channelMode == "grayscale")
    {
      if (GetParameterInt("channels.grayscale.channel") <= nbChan)
      {
        channels = {GetParameterInt("channels.grayscale.channel"),
                    GetParameterInt("channels.grayscale.channel"),
                    GetParameterInt("channels.grayscale.channel")};
      }
      else
      {
        itkExceptionMacro(<< "The channel has an invalid index");
      }
    }
    else if (channelMode == "rgb")
    {
      if ((GetParameterInt("channels.rgb.red") <= nbChan)
      && ( GetParameterInt("channels.rgb.green") <= nbChan)
      && ( GetParameterInt("channels.rgb.blue")   <= nbChan))
      {
        channels = {GetParameterInt("channels.rgb.red"),
                    GetParameterInt("channels.rgb.green"),
                    GetParameterInt("channels.rgb.blue")};
      }
      else
      {
        itkExceptionMacro(<< "At least one needed channel has an invalid index");
      }
    }
    else if (channelMode == "all")
    {
      // take all bands
      channels.resize(nbChan);
      std::iota(channels.begin(), channels.end(), 1);
    }
    return channels;
  }

  // return an image with the bands order modified of the input image
  template<class TImageType>
  typename TImageType::Pointer GetSelectedChannels()
  {
    typedef MultiToMonoChannelExtractROI<FloatVectorImageType::InternalPixelType,
                                         typename TImageType::InternalPixelType>  ExtractROIFilterType;
    typedef otb::ImageList<otb::Image<typename TImageType::InternalPixelType> >   ImageListType;
    typedef ImageListToVectorImageFilter<ImageListType,
                                         TImageType >                             ListConcatenerFilterType;

    typename ImageListType::Pointer             imageList;
    typename ListConcatenerFilterType::Pointer  concatener;

    imageList = ImageListType::New();
    concatener = ListConcatenerFilterType::New();

    //m_Filters.push_back(imageList.GetPointer());
    m_Filters.push_back(concatener.GetPointer());
    
    const bool monoChannel = IsParameterEnabled("channels.grayscale");

    // get band order
    std::vector<int> channels = GetChannels();

    for (auto && channel : channels)
    {
      typename ExtractROIFilterType::Pointer extractROIFilter = ExtractROIFilterType::New();
      m_Filters.push_back(extractROIFilter.GetPointer());
      extractROIFilter->SetInput(GetParameterImage("in"));
      if (!monoChannel) extractROIFilter->SetChannel(channel);
      extractROIFilter->UpdateOutputInformation();
      imageList->PushBack(extractROIFilter->GetOutput());
    }

    concatener->SetInput(imageList);
    concatener->UpdateOutputInformation();

    return concatener->GetOutput();
  }


  void DoExecute() override
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
        itkExceptionMacro("Unknown pixel type " << this->GetParameterOutputImagePixelType("out") <<"." << std::endl
                          << "The Convert application does not support complex pixel type as output." << std::endl
                          << "You can use instead the ExtractROI application to perform complex image conversion.");
        break;
      }
  }

  std::vector<itk::LightObject::Pointer> m_Filters;
};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::Convert)

