/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#include "otbVectorImageToImageListFilter.h"
#include "otbImageListToVectorImageFilter.h"
#include "otbStreamingStatisticsVectorImageFilter.h"
#include "otbStreamingStatisticsImageFilter.h"
#include "otbUnaryFunctorImageFilter.h"
#include "itkStreamingImageFilter.h"
#include "otbInPlacePassFilter.h"

#include "otbComputeHistoFilter.h"
#include "otbComputeGainLutFilter.h"
#include "otbApplyGainFilter.h"
#include "otbImageFileWriter.h"
#include "itkImageRegionIterator.h"
#include <string>
#include "otbStreamingHistogramVectorImageFilter.h"

namespace otb
{

namespace Wrapper
{

namespace Functor
{

class LuminanceOperator
{
typedef FloatVectorImageType::PixelType OutPixel;
typedef FloatVectorImageType::PixelType InPixel;
public:
  LuminanceOperator() {}
  unsigned int GetOutputSize()
  {
    return 1;
  }
  virtual ~LuminanceOperator() { }

 OutPixel operator() (  InPixel  input )
  { 
  OutPixel out(1);  
  out[0] = m_LumCoef[0] * input[m_Rgb[0]] + 
           m_LumCoef[1] * input[m_Rgb[1]] + 
           m_LumCoef[2] * input[m_Rgb[2]] ;
  return out;
  } // end operator ()


  void SetRgb( std::vector<unsigned int> rgb)
    {
    m_Rgb = rgb;
    }

  void SetLumCoef(std::vector<float> lumCoef)
    {
    m_LumCoef = lumCoef;
    }
  std::vector<float> GetLumCoef()
    {
    return m_LumCoef;
    }
private:
  std::vector<unsigned int> m_Rgb;
  std::vector<float> m_LumCoef;
}; // end of functor class  MultiplyOperator

}  // end of functor 

class ContrastEnhancement : public Application
{
public:
  /** Standard class typedefs. */
  typedef ContrastEnhancement	              Self;
  typedef Application	                      Superclass;
  typedef itk::SmartPointer < Self >	      Pointer;
  typedef itk::SmartPointer < const Self >	ConstPointer;

  typedef otb::VectorImage < unsigned int , 2 > HistogramType;
  typedef otb::VectorImage < double , 2 > LutType;

  typedef FloatImageType::PixelType ImagePixelType;

  typedef otb::ComputeHistoFilter < FloatImageType , 
                                    HistogramType > 
          HistoFilterType;
  typedef otb::ComputeGainLutFilter < HistogramType , 
                                      LutType > 
          GainLutFilterType;
  typedef otb::ApplyGainFilter < FloatImageType , 
                                 LutType , FloatImageType > 
          ApplyFilterType;
  typedef otb::ImageList < FloatImageType > ImageListType;

  typedef otb::VectorImageToImageListFilter < FloatVectorImageType, 
                                              ImageListType > 
          VectorToImageListFilterType;

  typedef otb::ImageListToVectorImageFilter < ImageListType, 
                                              FloatVectorImageType > 
          ImageListToVectorFilterType;

  typedef otb::StreamingStatisticsVectorImageFilter < FloatVectorImageType >
          VectorStatsFilterType;

  typedef otb::StreamingStatisticsImageFilter < FloatImageType >
          StatsFilterType;

  typedef otb::UnaryFunctorImageFilter < FloatVectorImageType ,
          FloatVectorImageType , Functor::LuminanceOperator > 
          LuminanceFunctorType;

  typedef itk::StreamingImageFilter < LutType , LutType > 
          StreamingImageFilterType;

  typedef otb::InPlacePassFilter < FloatImageType > BufferFilterType;

  typedef otb::StreamingHistogramVectorImageFilter < FloatVectorImageType > 
      HistoPersistentFilterType;

  /** Standard macro */
  itkNewMacro( Self );
 
  itkTypeMacro( ContrastEnhancement , otb::Application );

private:

	void DoInit() override
	{
		SetName("ContrastEnhancement");
    SetDescription("This application is the implementation of the histogram "
      "equalization algorithm. It can be used to enhance contrast in an image "
      "or to reduce the dynamic of the image without losing too much contrast. "
      "It offers several options as a no data value, "
      "a contrast limitation factor, a local version of the algorithm and "
      "also a mode to equalize the luminance of the image.");

    // Documentation
    SetDocName("Contrast Enhancement");
    SetDocLongDescription("This application is the implementation of the "
      "histogram equalization algorithm. The idea of the algorithm is to use "
      "the whole available dynamic. In order to do so it computes a histogram "
      "over the image and then use the whole dynamic: meaning flattening the "
      "histogram. That gives us gain for each bin that transform the original "
      "histogram into the flat one. This gain is then apply on the original "
      "image.\n\n"
      "The application proposes several options to allow a finer result:\n\n"
      "* There is an option to limit contrast. We choose to limit the contrast "
      "by modifying the original histogram. To do so we clip the histogram at a "
      "given height and redistribute equally among the bins the clipped population. "
      "Then we add a local version of the algorithm.\n"
      "* It is possible to apply the algorithm on tiles of the image, instead "
      "of on the whole image. That gives us gain depending on "
      "the value of the pixel and its position in the image. In order to "
      "smoothen the result we interpolate the gain between tiles.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag(Tags::Filter);

    AddParameter(ParameterType_InputImage,  "in",   "Input Image");
    SetParameterDescription("in", "Input image.");
    
    AddParameter(ParameterType_OutputImage, "out",  "Output Image");
    SetParameterDescription("out", "Output image.");
    
    AddParameter(ParameterType_Int , "bins" , "Number of bins");
    SetDefaultParameterInt("bins", 256);
    SetParameterDescription("bins",
      "Number of bins in the histogram");

    AddParameter(ParameterType_Float , "hfact" , "Contrast Limitation");  
    SetParameterDescription("hfact","This parameter will set the maximum "
      "height accepted in a bin on the input image histogram. "
      "The maximum height will be computed as hfact*eqHeight where eqHeight "
      "is the height of the theoretical flat histogram. The higher hfact, the "
      "higher the contrast."
      "\nWhen using 'luminance mode', it is recommended to limit this factor to a small value (ex: 4)");
    MandatoryOff("hfact");

    AddParameter(ParameterType_Float , "nodata" , "Nodata Value");
    SetParameterDescription("nodata","If there is a value in the "
      "image that has no visualization meaning, it can be ignored by the "
      "algorithm.");
    MandatoryOff("nodata");

    AddParameter(ParameterType_Choice , "spatial" , "Spatial parameters "
      "for the histogram computation");
    AddChoice( "spatial.local" , "Local" );
    SetParameterDescription("spatial.local" , "The histograms will be "
      "computed on each thumbnail. Each of the histogram will be "
      "equalized and the corresponding gain will be interpolated.");
    AddChoice( "spatial.global" , "Global" );
    SetParameterDescription("spatial.global" , "The histogram will be "
      "computed on the whole image. The equalization will be computed on "
      "this histogram.");

 
    AddParameter(ParameterType_Int,"spatial.local.h" , 
      "Thumbnail height");
    SetParameterDescription("spatial.local.h","Height of the thumbnail "
      "over which the histogram will be computed. The value is in pixels.");
    AddParameter(ParameterType_Int,"spatial.local.w" , 
      "Thumbnail width");
    SetParameterDescription("spatial.local.w","Width of the thumbnail "
      "over which the histogram will be computed. The value is in pixels.");

    AddParameter(ParameterType_Choice , "minmax" , "Minimum and maximum "
      "settings");
    SetParameterDescription("minmax","Minimum and maximum value that will "
      "bound the histogram and thus the dynamic of the resulting image. Values "
      "over those boundaries will be clipped.");
    AddChoice( "minmax.auto" , "Automatic" );
    SetParameterDescription("minmax.auto" , "Minimum and maximum value will "
      "be computed on the image (nodata value won't be taken "
      "into account) . Each band will have a minimum and a maximum.");
    AddParameter(ParameterType_Bool, "minmax.auto.global", "Global");
    SetParameterDescription("minmax.auto.global" , 
      "Min/max computation will result in the same minimum and maximum for "
      "all the bands.");
    AddChoice( "minmax.manual" , "Manual settings for min/max values" );
    SetParameterDescription("minmax.manual","Minimum and maximum value will be "
      "set by the user");
    AddParameter(ParameterType_Float , "minmax.manual.min" , "Minimum value");
    AddParameter(ParameterType_Float , "minmax.manual.max" , "Maximum value");
    // SetDefaultParameterFloat("minmax.manual.min", 0 );
    // SetDefaultParameterFloat("minmax.manual.max", 255 );
    MandatoryOff("minmax.manual.min");
    MandatoryOff("minmax.manual.max");

    AddParameter(ParameterType_Choice , "mode" , "What to equalized");
    AddChoice( "mode.each" , "Channels" );
    SetParameterDescription( "mode.each" ,
      "Each channel is equalized independently" );
    AddChoice( "mode.lum" , "Luminance" );
    SetParameterDescription( "mode.lum" ,
      "The relative luminance is computed according to the coefficients."
      "Then the histogram is equalized and the gain is applied to each of the "
      "channels. The channel gain will depend on "
      "the weight (coef) of the channel in the luminance." 
      "\nNote that default values come from color space theories "
      "on how human eyes perceive colors)" 

);
    AddParameter(ParameterType_Group , "mode.lum.red" , "Red channel" );
    AddParameter(ParameterType_Int , "mode.lum.red.ch" , "Red channel" );
    SetDefaultParameterInt("mode.lum.red.ch", 0 );
    AddParameter(ParameterType_Float , "mode.lum.red.coef" ,
      "Value for luminance computation for the red channel" );
    SetDefaultParameterFloat("mode.lum.red.coef", 0.21 );

    AddParameter(ParameterType_Group , "mode.lum.green" , "Green channel" );
    AddParameter(ParameterType_Int , "mode.lum.green.ch" , "Green channel" );
    SetDefaultParameterInt("mode.lum.green.ch", 1 );
    AddParameter(ParameterType_Float , "mode.lum.green.coef" ,
      "Value for luminance computation of the green channel" );
    SetDefaultParameterFloat("mode.lum.green.coef", 0.71 );

    AddParameter(ParameterType_Group , "mode.lum.blue" , "Blue channel" );
    AddParameter(ParameterType_Int , "mode.lum.blue.ch" , "Blue channel" );
    SetDefaultParameterInt("mode.lum.blue.ch", 2 );
    AddParameter(ParameterType_Float , "mode.lum.blue.coef" ,
      "Value for luminance computation of the blue channel" );
    SetDefaultParameterFloat("mode.lum.blue.coef", 0.08 );

    SetDefaultParameterInt( "spatial.local.w" , 256 );
    SetDefaultParameterInt( "spatial.local.h" , 256 );

    SetMinimumParameterIntValue("mode.lum.red.ch", 0);
    SetMinimumParameterIntValue("mode.lum.green.ch", 0);
    SetMinimumParameterIntValue("mode.lum.blue.ch", 0);
    SetMinimumParameterIntValue("bins", 2);
    SetMinimumParameterIntValue("spatial.local.h", 1);
    SetMinimumParameterIntValue("spatial.local.w", 1);

    SetExampleComment( "Local contrast enhancement by luminance" , 0 );
    SetDocExampleParameterValue( "in" , "colours.tif" );
    SetDocExampleParameterValue( "out" , "equalizedcolors.tif float" );
    SetDocExampleParameterValue( "bins" , "256" );
    SetDocExampleParameterValue( "spatial.local.w" , "500" );
    SetDocExampleParameterValue( "spatial.local.h" , "500");
    SetDocExampleParameterValue( "mode" , "lum" );

    AddRAMParameter(); 
  }

  void DoUpdateParameters() override
  {
    if ( HasValue("in") )
      {
      FloatVectorImageType * inImage = GetParameterImage("in");
      FloatVectorImageType::RegionType::SizeType size;
      size = inImage->GetLargestPossibleRegion().GetSize() ;

      if ( GetParameterString("spatial") == "local" &&
           HasValue("spatial.local.h") && HasValue("spatial.local.w") &&
           HasValue("bins") )
        CheckValidity();
      
      
      if ( !HasUserValue("nodata") && IsParameterEnabled("nodata"))
        SetDefaultValue( inImage , "NODATA" );

      if ( GetParameterString( "mode" ) == "lum" && 
           !HasUserValue("mode.lum.red.ch") &&
           !HasUserValue("mode.lum.green.ch") &&
           !HasUserValue("mode.lum.blue.ch") )
        SetDefaultValue( inImage , "RGB" );
      }

    if ( GetParameterString("minmax") == "manual" )
      {
      MandatoryOn("minmax.manual.min");
      MandatoryOn("minmax.manual.max");
      }
    else if ( GetParameterString("minmax") == "auto" )
      {
      MandatoryOff("minmax.manual.min");
      MandatoryOff("minmax.manual.max");
      }
  }

  void DoExecute() override
  {
    m_MinMaxMode = GetParameterString("minmax");
    m_EqMode = GetParameterString("mode");
    m_SpatialMode = GetParameterString("spatial");
    FloatVectorImageType * inImage = GetParameterImage("in");
    WarningGlobalOrNot( inImage );
    WarningMinMax();
    LogInfo();
    ImageListType::Pointer outputImageList ( ImageListType::New() ); 
    m_VectorToImageListFilter = VectorToImageListFilterType::New() ;
    m_VectorToImageListFilter->SetInput( inImage );
    m_VectorToImageListFilter->UpdateOutputInformation();
    ImageListType::Pointer inputImageList = 
                  m_VectorToImageListFilter->GetOutput();
    unsigned int nbChannel = inImage->GetVectorLength ();

    if ( m_EqMode == "each")
      {
      // Each channel will be equalized
      m_GainLutFilter.resize(nbChannel);
      m_ApplyFilter.resize(nbChannel);
      m_BufferFilter.resize(nbChannel);
      m_StreamingFilter.resize(nbChannel);
      PerBandEqualization( inImage , inputImageList , 
                           nbChannel , outputImageList );
      }
    else if ( m_EqMode == "lum")
      {
      std::vector< unsigned int > rgb( 3 , 0 );
      rgb[0] = GetParameterInt("mode.lum.red.ch");
      rgb[1] = GetParameterInt("mode.lum.green.ch");
      rgb[2] = GetParameterInt("mode.lum.blue.ch");
      if ( !( nbChannel > std::max( rgb[0] , std::max( rgb[1] , rgb[2] ) ) ) )
        {
        std::ostringstream oss;
        oss<<"One of the selected channel needed for luminance computation "
        "exceed the number of component of the image.";
        otbAppLogFATAL( << oss.str() )
        }
      ComputeLuminance( inImage , rgb );
      LuminanceEqualization( inputImageList , rgb , outputImageList );
      }

    m_ImageListToVectorFilterOut = ImageListToVectorFilterType::New() ;
    m_ImageListToVectorFilterOut->SetInput(outputImageList);
    SetParameterOutputImage( "out" , 
        m_ImageListToVectorFilterOut->GetOutput() );
  }

  // Look for default values in the image metadata
  void SetDefaultValue( const FloatVectorImageType * inImage ,
                        std::string what)
  {
    typedef ImageMetadataInterfaceBase ImageMetadataInterfaceType;
    ImageMetadataInterfaceType::Pointer metadataInterface = 
          ImageMetadataInterfaceFactory
                ::CreateIMI(inImage->GetMetaDataDictionary());
    if ( what == "NODATA" )
      {
      std::vector<double> values;
      std::vector<bool> flags;

      bool ret = metadataInterface->GetNoDataFlags(flags,values);

      if(ret && !values.empty() && !flags.empty() && flags[0])
        {
        SetParameterFloat( "nodata" , static_cast<float>( values[0] ) );
        }
      else
        {
        SetParameterFloat( "nodata" , 0 );
        }
      }
    else if ( what == "RGB" )
      {
      std::vector<unsigned int> rgb = 
                    metadataInterface->GetDefaultDisplay() ;
      unsigned int m = inImage->GetVectorLength ();
      SetParameterInt( "mode.lum.red.ch" , rgb[0] );
      SetParameterInt( "mode.lum.green.ch" , rgb[1] );
      SetParameterInt( "mode.lum.blue.ch" , rgb[2] );
      if( m < rgb[ 0 ] )
        {
        SetParameterFloat ("mode.lum.red.coef" , 0.0 );
        SetParameterInt( "mode.lum.red.ch" , 0 );
        }
      if( m < rgb[ 1 ] )
        {
        SetParameterFloat ("mode.lum.green.coef" , 0.0 );
        SetParameterInt( "mode.lum.gre.ch" , 0 );
        }
      if( m < rgb[ 2 ] )
        {
        SetParameterFloat ("mode.lum.blue.coef" , 0.0 );
        SetParameterInt( "mode.lum.blue.ch" , 0 );
        }
      }
  }

  // Log info for the user
  void LogInfo()
  {
    std::ostringstream oss;
    oss << "The application has been launched with the following parameters :"
    <<std::endl;
    oss << "- number of bins : " << GetParameterInt("bins") << std::endl;
    if ( HasValue("hfact") )
      {
      oss << "- contrast limtaition factor : "
      << GetParameterFloat("hfact") << std::endl;
      }
    else
      {
      oss << "- no contrast limitation factor" << std::endl;
      }
    oss << "- spatial parameters : " << m_SpatialMode ;
    if ( m_SpatialMode == "local" )
      {
      oss<< " with a thumbnail of " << m_ThumbSize[0] <<" X "<< m_ThumbSize[1] ;
      }
    oss << std::endl << "- equalisation of ";
    if ( m_EqMode == "each" )
      {
      oss << "each channel";
      }
    else
      {
      oss << "the luminance";
      }
    oss << std::endl << "- Min/Max parameters : ";
    if ( m_MinMaxMode == "auto" )
      {
      oss << "automatic";
      if ( GetParameterInt( "minmax.auto.global" ) )
        { 
        oss << " and global";
        }
      }
    else 
      {
      oss << GetParameterFloat("minmax.manual.min") << "/" << 
        GetParameterFloat("minmax.manual.max");
      }

    otbAppLogINFO( << oss.str() );
  }

  // Force global computation if the thumbsize is equal to the largest region
  void WarningGlobalOrNot( FloatVectorImageType * input)
  {
    auto size = input->GetLargestPossibleRegion().GetSize();
    if ( m_SpatialMode == "global" )
      {
      m_ThumbSize[0] = size[0];
      m_ThumbSize[1] = size[1];
      }  
    else 
      {
      m_ThumbSize[0] = GetParameterInt("spatial.local.w");
      m_ThumbSize[1] = GetParameterInt("spatial.local.h");
      if ( size[0] == m_ThumbSize[0] && size[1] == m_ThumbSize[1] )
        {
        std::ostringstream oss;
        oss<<"Warning you choose to compute the histogram with a local "
        "method whereas you have selected the whole image for the thumbnail "
        "size. In order to use less memory consider using the global option for "
        "histogram computation.";
        otbAppLogWARNING( << oss.str() );
        }
      }
  }

  // Check for min max validity 
  void WarningMinMax()
  {
    if ( m_MinMaxMode == "manual" &&  
         GetParameterFloat( "minmax.manual.min" ) > 
         GetParameterFloat( "minmax.manual.max" ) )
      {
      std::ostringstream oss;
      oss<<"The minimum (" << GetParameterFloat( "minmax.manual.min" ) <<
      ") is superior to the maximum (" 
      << GetParameterFloat( "minmax.manual.max" )
      << ") please correct this error or allow the application to compute "
      "those parameters";
      otbAppLogFATAL( << oss.str() )
      }
  }

  // Check if the image size is a multiple of the thumbnail size
  void CheckValidity()
  {
    std::ostringstream oss;
    long nbPixel = GetParameterInt("spatial.local.w") * 
                   GetParameterInt("spatial.local.h");
    if ( nbPixel < 10 * GetParameterInt("bins"))
      {   
      oss<<"Warning in parameters selection the thumbnail size is small "
      "in comparison with the number of bin. Histogram may not have much sens. "
      "For better result enlarge thumbnail size or reduce number of bin.";
      otbAppLogINFO( << oss.str() );
      }
  }

  // Compute min max from a vector image
  void ComputeVectorMinMax( const FloatVectorImageType::Pointer inImage ,
                            FloatVectorImageType::PixelType & max ,
                            FloatVectorImageType::PixelType & min )
  {
    if ( m_MinMaxMode == "manual" )
      {
      min.Fill( GetParameterFloat("minmax.manual.min") );
      max.Fill( GetParameterFloat("minmax.manual.max") );
      }
    else
      {
      VectorStatsFilterType::Pointer 
                statFilter ( VectorStatsFilterType::New() );
      statFilter->SetIgnoreInfiniteValues(true);
      if( IsParameterEnabled("nodata") )
        {
        statFilter->SetIgnoreUserDefinedValue(true);
        statFilter->SetUserIgnoredValue( GetParameterFloat("nodata") );
        }
      statFilter->SetInput( inImage );
      AddProcess(statFilter->GetStreamer(), "Computing statistics");
      statFilter->Update();
      min = statFilter->GetMinimum();
      max = statFilter->GetMaximum();
      if ( GetParameterInt("minmax.auto.global") )
        {
        float temp(min[0]);
        for ( unsigned int i = 1 ; i < min.GetSize() ; i++ )
          {
          temp = std::min(temp , min[i]);
          }
        min.Fill(temp);
        temp = max[0];
        for ( unsigned int i = 1 ; i < max.GetSize() ; i++ )
          {
          temp = std::max(temp , max[i]);
          }
        max.Fill(temp);
        }
      }
    std::ostringstream oss;
    oss<<"Minimum and maximum are for each channel : ";
    if ( GetParameterInt("minmax.auto.global") || 
          m_MinMaxMode == "manual" )
      {
      oss<<std::endl<<min[0]<<" and "<<max[0];
      }
    else 
      {
      for ( unsigned int i = 0 ; i < min.GetSize() ; i++ )
        {
          oss<<std::endl<<min[i]<<" and "<<max[i];
        }
      }
    otbAppLogINFO( << oss.str() );
  }

  // Prepare the first half of the pipe that is common to every methode of 
  // equalization
  void SetUpPipeline( unsigned int channel ,
                      const FloatImageType::Pointer input )
  {
    m_GainLutFilter[channel] = GainLutFilterType::New();
    m_ApplyFilter[channel] = ApplyFilterType::New();
    m_StreamingFilter[channel] = StreamingImageFilterType::New();
    m_BufferFilter[channel] = BufferFilterType::New();
    m_BufferFilter[channel]->SetInput( input );
    m_GainLutFilter[channel]->SetInput ( m_Histogram[channel] );
    m_StreamingFilter[channel]->SetInput( 
      m_GainLutFilter[channel]->GetOutput() );
    m_ApplyFilter[channel]->SetInputImage ( 
      m_BufferFilter[channel]->GetOutput() );
    m_ApplyFilter[channel]->SetInputLut( 
      m_StreamingFilter[channel]->GetOutput() );
  }

  // Function corresponding to the "each" mode
  void PerBandEqualization( const FloatVectorImageType::Pointer inImage ,
                            const ImageListType::Pointer inputImageList ,
                            const unsigned int nbChannel,
                            ImageListType::Pointer outputImageList )
  {
    FloatVectorImageType::PixelType min(nbChannel) , max(nbChannel);
    min.Fill(0);
    max.Fill(0);
    ComputeVectorMinMax( inImage , max , min );

    if ( m_SpatialMode == "global" )
      PersistentComputation( inImage , nbChannel , max , min );
    else
      {
      float thresh (-1);
      if ( HasValue("hfact") )
        {
        thresh = GetParameterInt("hfact");
        }

      m_HistoFilter.resize(nbChannel);
      m_Histogram.resize(nbChannel);
      for (unsigned int channel = 0 ; channel < nbChannel ; channel++)
        {
        m_HistoFilter[channel] = HistoFilterType::New();
        m_HistoFilter[channel]->SetInput( 
            inputImageList->GetNthElement(channel) );
        SetHistoFilterParameter( m_HistoFilter[channel] ,
                                 min[channel] ,
                                 max[channel] ,
                                 GetParameterInt("bins") ,
                                 thresh );
        m_Histogram[channel] = m_HistoFilter[channel]->GetHistoOutput();
        }
      }

    for ( unsigned int channel = 0 ; channel < nbChannel ; channel++ ) 
      {
      SetUpPipeline( channel , inputImageList->GetNthElement(channel) );

      if ( min[channel] == max[channel] )
        {
          std::ostringstream oss;
          oss<< "Channel " <<channel << " is constant : " << "min = " <<
          min[channel] << " and max = " << max[channel] ;
          otbAppLogINFO( << oss.str() );
          m_BufferFilter[channel]->SetInput( 
                inputImageList->GetNthElement(channel) );
          outputImageList->PushBack( m_BufferFilter[channel]->GetOutput() );
          continue;
        }

      SetGainLutFilterParameter( m_GainLutFilter[channel] ,
                                 min[channel] ,
                                 max[channel]);
      SetApplyFilterParameter( m_ApplyFilter[channel] ,
                               min[channel] ,
                               max[channel]);

      outputImageList->PushBack( m_ApplyFilter[channel]->GetOutput() );
      }
  }

  // Compute the luminance with user parameters
  void ComputeLuminance( const FloatVectorImageType::Pointer inImage ,
                         std::vector < unsigned int > rgb )
  {
    // Retrieve coeffs for each channel
    std::vector < float > lumCoef( 3 , 0.0 );
    lumCoef[0] = GetParameterFloat("mode.lum.red.coef");
    lumCoef[1] = GetParameterFloat("mode.lum.green.coef");
    lumCoef[2] = GetParameterFloat("mode.lum.blue.coef");
    // Normalize those coeffs
    float sum = std::accumulate( lumCoef.begin() , lumCoef.end() , 0.0 );
    assert(sum>0);
    for (int i = 0 ; i<3 ; i++ )
      {
      lumCoef[i] /= sum;
      }
    m_LuminanceFunctor =  LuminanceFunctorType::New() ;
    m_LuminanceFunctor->GetFunctor().SetRgb( rgb );
    m_LuminanceFunctor->GetFunctor().SetLumCoef( lumCoef );
    m_LuminanceFunctor->SetInput( inImage );
    m_LuminanceFunctor->UpdateOutputInformation();
  }

  // Equalize the luminance and apply the corresponding gain on each channel
  // used to compute this luminance
  void LuminanceEqualization( const ImageListType::Pointer inputImageList ,
                              const std::vector < unsigned int > rgb ,
                              ImageListType::Pointer outputImageList )
  {
    m_GainLutFilter.resize( 1 , GainLutFilterType::New() );
    m_HistoFilter.resize( 1 , HistoFilterType::New() );
    m_StreamingFilter.resize( 1 , StreamingImageFilterType::New() );
    m_ApplyFilter.resize(3);
    m_BufferFilter.resize(3);
    FloatVectorImageType::PixelType min(1) , max(1);
    ComputeVectorMinMax( m_LuminanceFunctor->GetOutput() , max , min );

    if ( m_SpatialMode == "global" )
      PersistentComputation( m_LuminanceFunctor->GetOutput() , 1 , max , min );
    else
      {
        float thresh (-1);
        if ( HasValue("hfact") )
          {
          thresh = GetParameterInt("hfact");
          }
        m_Histogram.resize(1);
        m_HistoFilter[0] = HistoFilterType::New();
        m_LuminanceToImageListFilter = VectorToImageListFilterType::New();
        m_LuminanceToImageListFilter->SetInput( 
          m_LuminanceFunctor->GetOutput() );
        SetHistoFilterParameter( m_HistoFilter[0] ,
                                 min[0] ,
                                 max[0] ,
                                 GetParameterInt("bins") ,
                                 thresh ); 
        m_LuminanceToImageListFilter->UpdateOutputInformation();
        m_HistoFilter[0]->SetInput( 
          m_LuminanceToImageListFilter->GetOutput()->GetNthElement(0) ) ;
        m_Histogram[0] = m_HistoFilter[0]->GetHistoOutput();
      }

    m_GainLutFilter[0]->SetInput ( m_Histogram[0] );
    m_StreamingFilter[0]->SetInput( m_GainLutFilter[0]->GetOutput() );

    SetGainLutFilterParameter( m_GainLutFilter[0] ,
                               min[0] ,
                               max[0] );

    for ( int channel = 0 ; channel < 3 ; channel++ ) 
      {

      m_BufferFilter[channel] = BufferFilterType::New();
      m_ApplyFilter[channel] = ApplyFilterType::New();
      SetApplyFilterParameter( m_ApplyFilter[channel] ,
                               min[0] ,
                               max[0] );

      m_BufferFilter[channel]->SetInput( 
          inputImageList->GetNthElement( rgb[channel] ) );
      m_ApplyFilter[channel]->SetInputImage ( 
          m_BufferFilter[channel]->GetOutput() );
      m_ApplyFilter[channel]->SetInputLut( 
          m_StreamingFilter[0]->GetOutput() );
      

      outputImageList->PushBack( m_ApplyFilter[channel]->GetOutput() );
    }
  }

  // Function that compute histograms with HistoPersistentFilterType
  void PersistentComputation( const FloatVectorImageType::Pointer inImage ,
                              const unsigned int nbChannel ,
                              const FloatVectorImageType::PixelType & max ,
                              const FloatVectorImageType::PixelType & min)
  { 

    HistoPersistentFilterType::Pointer histoPersistent (
        HistoPersistentFilterType::New());
    unsigned int nbBin ( GetParameterInt( "bins" ) );
    histoPersistent->SetInput( inImage );
    FloatVectorImageType::PixelType pixel( nbChannel ) , step( nbChannel );
    pixel.Fill( nbBin );
    histoPersistent->GetFilter()->SetNumberOfBins( pixel );
    if ( IsParameterEnabled("nodata") )
      {
      histoPersistent->GetFilter()->NoDataFlagOn();
      histoPersistent->GetFilter()->SetNoDataValue( 
                GetParameterFloat( "nodata" ) );
      }
    step = ( max - min ) / nbBin;
    pixel = min - 0.5 * step;
    histoPersistent->GetFilter()->SetHistogramMin(pixel);
    pixel = max + 0.5 * step;
    histoPersistent->GetFilter()->SetHistogramMax(pixel);
    AddProcess(histoPersistent->GetStreamer(), "Computing histogram");
    histoPersistent->Update();
    HistoPersistentFilterType::HistogramListType * histoList = 
            histoPersistent->GetHistogramList();

    Transfer( histoList );

    if ( HasValue("hfact") )
      {
      Threshold( histoList , nbBin );
      }
  }

  // Threshold function that is normally done in ComputeHistoFilter  and here is
  // used on the output of HistoPersistentFilterType.
  void Threshold( HistoPersistentFilterType::HistogramListType * histoList ,
                  unsigned int nbBin )
  {
    for ( unsigned int j = 0 ; j < histoList->Size() ; j++ )
      {
      unsigned int rest(0) , height ( static_cast<unsigned int>( 
          GetParameterFloat( "hfact" ) * 
          ( histoList->GetNthElement(j)->GetTotalFrequency() / nbBin ) ) );

      HistogramType::IndexType zero;
      HistogramType::Pointer & histoToThresh = m_Histogram[j];
      zero.Fill(0);
      for ( unsigned int i = 0 ; i < nbBin ; i++ )
        {
        if ( histoToThresh->GetPixel(zero)[i] > height )
          {
          rest += histoToThresh->GetPixel(zero)[i] - height ;
          histoToThresh->GetPixel(zero)[i] = height ;
          }
        }
      height = rest / nbBin;
      rest = rest % nbBin;
      for ( unsigned int i = 0 ; i < nbBin ; i++ )
        {
        histoToThresh->GetPixel(zero)[i] += height ;
        if ( i > (nbBin - rest)/2 && i <= (nbBin - rest)/2 + rest )
          {
          ++histoToThresh->GetPixel(zero)[i];
          }
        }
      }
  }

  // Transfer the output of the HistoPersistentFilterType to the input type
  // of ComputeGainLutFilter
  void Transfer( HistoPersistentFilterType::HistogramListType * histoList )
  {
    unsigned int nbBin( GetParameterInt( "bins" ) );

    HistoPersistentFilterType::HistogramType::Pointer histo;
    FloatImageType::SpacingType inputSpacing ( 
      GetParameterImage("in")->GetSignedSpacing() );
    FloatImageType::PointType inputOrigin ( 
      GetParameterImage("in")->GetOrigin() );

    HistogramType::SpacingType histoSpacing ;
    histoSpacing[0] = inputSpacing[0] * m_ThumbSize[0] ;
    histoSpacing[1] = inputSpacing[1] * m_ThumbSize[1] ;
    HistogramType::PointType histoOrigin ;
    histoOrigin[0] = histoSpacing[0] / 2 + 
      inputOrigin[0] - inputSpacing[0] / 2 ;
    histoOrigin[1] = histoSpacing[1] / 2 + 
      inputOrigin[1] - inputSpacing[1] / 2 ;

    for ( unsigned int i = 0 ; i < histoList->Size() ; i++ )
      {
      HistogramType::Pointer histoVectorImage =
        CreateHistogramFrom( histoList->GetNthElement( i ) ,
                             nbBin ,
                             histoSpacing ,
                             histoOrigin );
     
      m_Histogram.push_back( histoVectorImage );
      }
  }

  // Creating a histogram (VectorImage) from an itkHistogram
  HistogramType::Pointer CreateHistogramFrom( 
      HistoPersistentFilterType::HistogramType::Pointer histo ,
      unsigned int nbBin ,
      HistogramType::SpacingType histoSpacing ,
      HistogramType::PointType histoOrigin )
  {
    HistogramType::SizeType sizeOne;
    sizeOne.Fill( 1 );
    HistogramType::IndexType index;
    index.Fill(0);

    HistogramType::PixelType histoPixel( nbBin );

    HistogramType::Pointer histoVectorImage ( 
            HistogramType::New() );

    histoVectorImage->SetVectorLength( nbBin );
    histoVectorImage->SetBufferedRegion( sizeOne );
    histoVectorImage->SetRequestedRegion( sizeOne );
    histoVectorImage->SetLargestPossibleRegion( sizeOne );
    histoVectorImage->SetSignedSpacing( histoSpacing ) ;
    histoVectorImage->SetOrigin( histoOrigin );
    histoVectorImage->Allocate();
    for (unsigned int j = 0 ; j < nbBin ; j++ )
      {
      histoPixel[j] = histo->GetFrequency( j );
      }
    histoVectorImage->SetPixel( index , histoPixel );
    return histoVectorImage;
  }

  // Set correct parameters for the ComputeHistoFilter
  void SetHistoFilterParameter( HistoFilterType::Pointer histoFilter ,
                                float min ,
                                float max ,
                                unsigned int nbBin ,
                                float thresh = -1 )
  {
    histoFilter->SetMin( min );
    histoFilter->SetMax( max );
    histoFilter->SetNbBin( nbBin );
    histoFilter->SetThumbSize( m_ThumbSize );
    histoFilter->SetThreshold( thresh );
    if ( IsParameterEnabled("nodata") )
      {
      histoFilter->SetNoData( GetParameterFloat( "nodata" ) );
      histoFilter->SetNoDataFlag( true );
      }
  }

  // Set correct parameters for the ComputeGainLutFilter
  void SetGainLutFilterParameter( GainLutFilterType::Pointer gainLutFilter ,
                                  ImagePixelType min ,
                                  ImagePixelType max )
  {
    gainLutFilter->SetMin( min );
    gainLutFilter->SetMax( max );
    gainLutFilter->SetNbPixel( m_ThumbSize[0]*m_ThumbSize[1] );
  }

  // Set correct parameters for the ApplyGainFilter
  void SetApplyFilterParameter( ApplyFilterType::Pointer applyFilter ,
                                ImagePixelType min ,
                                ImagePixelType max )
  {
    applyFilter->SetMin( min );
    applyFilter->SetMax( max );
    applyFilter->SetThumbSize( m_ThumbSize );
    if ( IsParameterEnabled("nodata") )
      {
      applyFilter->SetNoData( GetParameterFloat( "nodata" ) );
      applyFilter->SetNoDataFlag( true );
      }
  }

  std::string m_SpatialMode , m_MinMaxMode , m_EqMode ;
  FloatImageType::SizeType m_ThumbSize;
  ImageListToVectorFilterType::Pointer m_ImageListToVectorFilterOut;
  LuminanceFunctorType::Pointer m_LuminanceFunctor;
  VectorToImageListFilterType::Pointer m_LuminanceToImageListFilter;
  VectorToImageListFilterType::Pointer m_VectorToImageListFilter;
  std::vector < GainLutFilterType::Pointer > m_GainLutFilter;
  std::vector < HistoFilterType::Pointer > m_HistoFilter;
  std::vector < HistogramType::Pointer > m_Histogram;
  std::vector < ApplyFilterType::Pointer > m_ApplyFilter;
  std::vector < StreamingImageFilterType::Pointer > m_StreamingFilter;
  std::vector < BufferFilterType::Pointer > m_BufferFilter;

};



} //End namespace Wrapper
} //End namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::ContrastEnhancement)
