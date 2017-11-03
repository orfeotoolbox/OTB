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


  void SetRgb( std::vector<int> rgb)
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
  std::vector<int> m_Rgb;
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
		SetName("Contrast Enhancement");
    SetDescription("");

    // Documentation
    SetDocName("Contrast Enhancement");
    SetDocLongDescription("");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("");

    AddDocTag(Tags::Filter);

    AddParameter(ParameterType_InputImage,  "in",   "Input Image");
    SetParameterDescription("in", "Input image.");
    
    AddParameter(ParameterType_OutputImage, "out",  "Output Image");
    SetParameterDescription("out", "Output image.");
    
    AddParameter(ParameterType_Int , "bin" , "Number of bin");
    SetDefaultParameterInt("bin", 256);
    SetParameterDescription("bin","Number of bin used to create the histogram");

    AddParameter(ParameterType_Float , "hfact" , "Contrast Limitation");  
    SetParameterDescription("hfact","This parameter will set the maximum "
      "height accepted in a bin on the input image histogram. "
      "The maximum height will be computed as hfact*eqHeight where eqHeight "
      "is the height of the theoretical flat histogram. ");
    MandatoryOff("hfact");

    AddParameter(ParameterType_Float , "nodata" , "Nodata Value");
    SetParameterDescription("nodata","If there is a value in the "
      "image that has no visualization it is ignored by the algorithm.");
    MandatoryOff("nodata");

    AddParameter(ParameterType_Empty , "global" , "Global equalization" );
    SetParameterDescription("global","");

    AddParameter(ParameterType_Group, "thumb", "Thumbnail size");
    SetParameterDescription("thumb","The algorithm can be local : "
      "in each of those thumbnails a histogram will be computed "
      "and equalized. The result gain will be interpolated.");
 
    AddParameter(ParameterType_Int,"thumb.h" , "Thumbnail height in pixel");
    AddParameter(ParameterType_Int,"thumb.w" , "Thumbnail width in pixel");

    AddParameter(ParameterType_Choice , "minmax" , "Minimum and maximum "
      "definition");
    SetParameterDescription("minmax","Minimum and maximum value that will "
      "bound the histogram.");
    AddChoice( "minmax.auto" , "Automatic" );
    SetParameterDescription("minmax.auto" , "Minimum and maximum value will "
      "be the real one computed on the image (nodata value won't be taken "
      "into account) .");
    AddParameter(ParameterType_Empty, "minmax.auto.global", "Global");
    SetParameterDescription("minmax.auto.global" , "Automatic "
      "min/max computation will result in the same minimum and maximum for "
      "all the bands. Otherwise it is one minimum and one maximum "
      "for each band.");
    AddChoice( "minmax.man" , "Manuel" );
    SetParameterDescription("minmax.auto","Minimum and maximum value will be "
      "set by the user");
    AddParameter(ParameterType_Float , "minmax.man.min" , "Minimum");
    AddParameter(ParameterType_Float , "minmax.man.max" , "Maximum");
    MandatoryOff("minmax.man.min");
    MandatoryOff("minmax.man.max");

    AddParameter(ParameterType_Choice , "mode" , "What to equalized");
    AddChoice( "mode.each" , "Channels" );
    SetParameterDescription( "mode.each" ,
                "Each channel are equalized independently" );
    AddChoice( "mode.lum" , "Luminance" );
    SetParameterDescription( "mode.lum" ,
                "The luminance is equalized and then a gain is applied "
                "on each channels. This gain for each channels will depend on"
                "the weight (coef) of the channel in the luminance." );
    AddParameter(ParameterType_Group , "mode.lum.red" , "Red Channel" );
    AddParameter(ParameterType_Int , "mode.lum.red.ch" , "Red Channel" );
    SetDefaultParameterInt("mode.lum.red.ch", 0 );
    AddParameter(ParameterType_Float , "mode.lum.red.coef" ,
                 "Value for luminance computation" );
    SetDefaultParameterFloat("mode.lum.red.coef", 0.21 );

    AddParameter(ParameterType_Group , "mode.lum.gre" , "Green Channel" );
    AddParameter(ParameterType_Int , "mode.lum.gre.ch" , "Green Channel" );
    SetDefaultParameterInt("mode.lum.gre.ch", 1 );
    AddParameter(ParameterType_Float , "mode.lum.gre.coef" ,
                 "Value for luminance computation" );
    SetDefaultParameterFloat("mode.lum.gre.coef", 0.71 );

    AddParameter(ParameterType_Group , "mode.lum.blu" , "Blue Channel" );
    AddParameter(ParameterType_Int , "mode.lum.blu.ch" , "Blue Channel" );
    SetDefaultParameterInt("mode.lum.blu.ch", 2 );
    AddParameter(ParameterType_Float , "mode.lum.blu.coef" ,
                 "Value for luminance computation" );
    SetDefaultParameterFloat("mode.lum.blu.coef", 0.08 );

    SetMinimumParameterIntValue("mode.lum.red.ch", 0);
    SetMinimumParameterIntValue("mode.lum.gre.ch", 0);
    SetMinimumParameterIntValue("mode.lum.blu.ch", 0);
    SetMinimumParameterIntValue("bin", 2);
    SetMinimumParameterIntValue("thumb.h", 1);
    SetMinimumParameterIntValue("thumb.w", 1);

    AddRAMParameter(); 
  }

  void DoUpdateParameters() override
  {
    if ( HasValue("in") )
      {
      FloatVectorImageType * inImage = GetParameterImage("in");
      FloatVectorImageType::RegionType::SizeType size;
      size = inImage->GetLargestPossibleRegion().GetSize() ;

      if ( !HasUserValue("thumb.w") )
        SetParameterInt( "thumb.w" , size[0] );
        
      if ( !HasUserValue("thumb.h") )
        SetParameterInt( "thumb.h" , size[1] );
      
      if ( HasValue("thumb.h") && HasValue("thumb.w") && HasValue("bin") )
        CheckValidity();
      
      
      if ( !HasUserValue("nodata") && IsParameterEnabled("nodata"))
        SetDefaultValue( inImage , "NODATA" );

      if ( GetParameterString( "mode" ) == "lum" && 
           !HasUserValue("mode.lum.red.ch") &&
           !HasUserValue("mode.lum.gre.ch") &&
           !HasUserValue("mode.lum.blu.ch") )
        SetDefaultValue( inImage , "RGB" );

      if ( HasUserValue("minmax.man.min") && HasUserValue("minmax.man.max") )
        {
        if ( GetParameterFloat( "minmax.man.min" ) > 
             GetParameterFloat( "minmax.man.max" ) )
          {
          float temp = GetParameterFloat( "minmax.man.min" );
          SetParameterFloat( "minmax.man.min" , 
                             GetParameterFloat( "minmax.man.max" ));
          SetParameterFloat( "minmax.man.max" , temp );
          }
        else if ( GetParameterFloat( "minmax.man.min" ) == 
                  GetParameterFloat( "minmax.man.max" ) )
          {
          std::ostringstream oss;
          oss<<"Warning minimum and maximum are equal."<<std::endl;
          otbAppLogINFO( << oss.str() );
          }
        }

      }

    if ( GetParameterString("minmax") == "man" )
      {
      MandatoryOn("minmax.man.min");
      MandatoryOn("minmax.man.max");
      }
    else if ( GetParameterString("minmax") == "auto" )
      {
      MandatoryOff("minmax.man.min");
      MandatoryOff("minmax.man.max");
      }
  }

  void DoExecute() override
  {
    m_ThumbSize[0] = GetParameterInt("thumb.w");
    m_ThumbSize[1] = GetParameterInt("thumb.h");
    FloatVectorImageType * inImage = GetParameterImage("in");
    ForceGlobalOrNot( inImage );
    std::string mode = GetParameterString("mode");
    ImageListType::Pointer outputImageList ( ImageListType::New() ); 
    m_VectorToImageListFilter = VectorToImageListFilterType::New() ;
    m_VectorToImageListFilter->SetInput( inImage );
    m_VectorToImageListFilter->UpdateOutputInformation();
    ImageListType::Pointer inputImageList = 
                  m_VectorToImageListFilter->GetOutput();
    unsigned int nbChannel = inImage->GetVectorLength ();

    if ( mode == "each")
      {
      // Each channel will be equalized
      m_GainLutFilter.resize(nbChannel);
      m_ApplyFilter.resize(nbChannel);
      m_BufferFilter.resize(nbChannel);
      m_StreamingFilter.resize(nbChannel);
      PerBandEqualization( inImage , inputImageList , 
                           nbChannel , outputImageList );
      }
    else if ( mode == "lum")
      {
      std::vector<int> rgb( 3 , 0 );
      rgb[0] = GetParameterInt("mode.lum.red.ch");
      rgb[1] = GetParameterInt("mode.lum.gre.ch");
      rgb[2] = GetParameterInt("mode.lum.blu.ch");
      ComputeLuminance( inImage , rgb );
      LuminanceEqualization( inputImageList , rgb , outputImageList );
      }

    m_ImageListToVectorFilterOut = ImageListToVectorFilterType::New() ;
    m_ImageListToVectorFilterOut->SetInput(outputImageList);
    // m_ImageListToVectorFilterOut->Update();
    // std::cout<<"not you imagelistetovecor"<<std::endl;
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
      SetParameterInt( "mode.lum.gre.ch" , rgb[1] );
      SetParameterInt( "mode.lum.blu.ch" , rgb[2] );
      if( m < rgb[ 0 ] )
        {
        SetParameterFloat ("mode.lum.red.coef" , 0.0 );
        SetParameterInt( "mode.lum.red.ch" , 0 );
        }
      if( m < rgb[ 1 ] )
        {
        SetParameterFloat ("mode.lum.gre.coef" , 0.0 );
        SetParameterInt( "mode.lum.gre.ch" , 0 );
        }
      if( m < rgb[ 2 ] )
        {
        SetParameterFloat ("mode.lum.blu.coef" , 0.0 );
        SetParameterInt( "mode.lum.blu.ch" , 0 );
        }
      }
  }

  // Force global computation if the thumbsize is equal to the largest region
  void ForceGlobalOrNot( FloatVectorImageType * input)
  {
    auto size = input->GetLargestPossibleRegion().GetSize();
    if ( size[0] == m_ThumbSize[0] && size[1] == m_ThumbSize[1] )
      SetParameterEmpty( "global" , true );
  }

  // Check if the image size is a multiple of the thumbnail size
  void CheckValidity()
  {
    std::ostringstream oss;
    long nbPixel = GetParameterInt("thumb.w") * GetParameterInt("thumb.h");
    if ( nbPixel < 10 * GetParameterInt("bin"))
      {   
      oss<<"Warning in parameters selection the thumbnail size is small "
      "in comparison with the number of bin. Histogram may not have much sens. "
      "For better result enlarge thumbnail size or reduce number of bin."
      <<std::endl;
      }
    otbAppLogINFO( << oss.str() );
  }

  // Compute min max from a vector image
  void ComputeVectorMinMax( const FloatVectorImageType::Pointer inImage ,
                            FloatVectorImageType::PixelType & max ,
                            FloatVectorImageType::PixelType & min )
  {
    if ( GetParameterString("minmax") == "man" )
      {
      min.Fill( GetParameterFloat("minmax.man.min") );
      max.Fill( GetParameterFloat("minmax.man.max") );
      }
    else if ( GetParameterString("minmax") == "auto" )
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
      statFilter->Update();
      min = statFilter->GetMinimum();
      max = statFilter->GetMaximum();
      if ( IsParameterEnabled("minmax.auto.global") )
        {
        float temp(min[0]);
        for ( unsigned int i = 0 ; i < min.GetSize() ; i++ )
          {
          temp = std::min(temp , min[i]);
          }
        min.Fill(temp);
        temp = max[0];
        for ( unsigned int i = 0 ; i < max.GetSize() ; i++ )
          {
          temp = std::max(temp , max[i]);
          }
        max.Fill(temp);
        }
      }
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
    m_StreamingFilter[channel]->SetInput( m_GainLutFilter[channel]->GetOutput() );
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

    if ( IsParameterEnabled("global") )
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
                                 GetParameterInt("bin") ,
                                 thresh );
        m_Histogram[channel] = m_HistoFilter[channel]->GetHistoOutput();
        }
      }

    for ( unsigned int channel = 0 ; channel < nbChannel ; channel++ ) 
      {
      SetUpPipeline( channel , inputImageList->GetNthElement(channel) );

      if ( min[channel] == max[channel] )
        {
          //TODO Warn user through log
          std::cout<<"Channel constant"<<std::endl;
          std::cout<<"min "<<min[channel]<<std::endl;
          std::cout<<"max "<<max[channel]<<std::endl;
          m_BufferFilter[channel]->SetInput( 
                inputImageList->GetNthElement(channel) );
          outputImageList->PushBack( m_BufferFilter[channel]->GetOutput() );
          // outputImageList->PushBack( inputImageList->GetNthElement(channel) );
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
                         std::vector < int > rgb )
  {
    // Retreive coeffs for each channel
    std::vector < float > lumCoef( 3 , 0.0 );
    lumCoef[0] = GetParameterFloat("mode.lum.red.coef");
    lumCoef[1] = GetParameterFloat("mode.lum.gre.coef");
    lumCoef[2] = GetParameterFloat("mode.lum.blu.coef");
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
    // std::cout<<m_LuminanceFunctor->GetOutput()->GetNumberOfComponentsPerPixel()<<std::endl;
  }

  // Equalize the luminance and apply the corresponding gain on each channel
  // used to compute this luminance
  void LuminanceEqualization( const ImageListType::Pointer inputImageList ,
                              const std::vector < int > rgb ,
                              ImageListType::Pointer outputImageList )
  {
    m_GainLutFilter.resize( 1 , GainLutFilterType::New() );
    m_HistoFilter.resize( 1 , HistoFilterType::New() );
    m_StreamingFilter.resize( 1 , StreamingImageFilterType::New() );
    m_ApplyFilter.resize(3);
    m_BufferFilter.resize(3);
    FloatVectorImageType::PixelType min(1) , max(1);
    ComputeVectorMinMax( m_LuminanceFunctor->GetOutput() , max , min );

    if ( IsParameterEnabled("global") )
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
                                 GetParameterInt("bin") ,
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

  void PersistentComputation( const FloatVectorImageType::Pointer inImage ,
                              const unsigned int nbChannel ,
                              const FloatVectorImageType::PixelType & max ,
                              const FloatVectorImageType::PixelType & min)
  { 

    HistoPersistentFilterType::Pointer histoPersistent (
        HistoPersistentFilterType::New());
    unsigned int nbBin ( GetParameterInt( "bin" ) );
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
    histoPersistent->Update();
    HistoPersistentFilterType::HistogramListType * histoList = 
            histoPersistent->GetHistogramList();

    Transfert( histoList );

    if ( HasValue("hfact") )
      {
      Threshold( histoList , nbBin );
      }
  }

  void Threshold( HistoPersistentFilterType::HistogramListType * histoList ,
                  unsigned int nbBin )
  {
    for ( unsigned int j = 0 ; j < m_Histogram.size() ; j++ )
      {
      unsigned int rest(0) , height ( static_cast<unsigned int>( 
          GetParameterFloat( "hfact" ) * 
          ( histoList->GetNthElement(j)->GetTotalFrequency() / nbBin ) ) );

      HistogramType::IndexType zero;
      HistogramType::Pointer & histoToThresh = m_Histogram[j];
      zero.Fill(0);
      for( unsigned int i = 0 ; i < nbBin ; i++ )
        {
        if ( histoToThresh->GetPixel(zero)[i] > height )
          {
          rest += histoToThresh->GetPixel(zero)[i] - height ;
          histoToThresh->GetPixel(zero)[i] = height ;
          }
        }
      height = rest / nbBin;
      rest = rest % nbBin;
      for( unsigned int i = 0 ; i < nbBin ; i++ )
        {
        histoToThresh->GetPixel(zero)[i] += height ;
        if ( i > (nbBin - rest)/2 && i <= (nbBin - rest)/2 + rest )
          {
          ++histoToThresh->GetPixel(zero)[i];
          }
        }
      }
  }

  void Transfert( HistoPersistentFilterType::HistogramListType * histoList )
  {
    unsigned int nbBin( GetParameterInt( "bin" ) );

    HistoPersistentFilterType::HistogramType::Pointer histo;
    FloatImageType::SpacingType inputSpacing ( GetParameterImage("in")->GetSpacing() );
    FloatImageType::PointType inputOrigin ( GetParameterImage("in")->GetOrigin() );

    HistogramType::SpacingType histoSpacing ;
    histoSpacing[0] = inputSpacing[0] * m_ThumbSize[0] ;
    histoSpacing[1] = inputSpacing[1] * m_ThumbSize[1] ;
    HistogramType::PointType histoOrigin ;
    histoOrigin[0] = histoSpacing[0] / 2 +  inputOrigin[0] - inputSpacing[0] / 2 ;
    histoOrigin[1] = histoSpacing[1] / 2 +  inputOrigin[1] - inputSpacing[1] / 2 ;

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
    histoVectorImage->SetSpacing( histoSpacing ) ;
    histoVectorImage->SetOrigin( histoOrigin );
    histoVectorImage->Allocate();
    for (unsigned int j = 0 ; j < nbBin ; j++ )
      {
      histoPixel[j] = histo->GetFrequency( j );
      }
    histoVectorImage->SetPixel( index , histoPixel );
    return histoVectorImage;
  }

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

  void SetGainLutFilterParameter( GainLutFilterType::Pointer gainLutFilter ,
                                  ImagePixelType min ,
                                  ImagePixelType max )
  {
    gainLutFilter->SetMin( min );
    gainLutFilter->SetMax( max );
    gainLutFilter->SetNbPixel( m_ThumbSize[0]*m_ThumbSize[1] );
  }

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