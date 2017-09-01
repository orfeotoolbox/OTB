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
#include "itkUnaryFunctorImageFilter.h"
#include "otbStreamingImageVirtualWriter.h"
#include "otbStreamingImageRAMWriter.h"

#include "otbComputeHistoFilter.h"
#include "otbComputeLutFilter.h"
#include "otbComputeGainFilter.h"

namespace otb
{

namespace Wrapper
{

namespace Functor
{

class LuminanceOperator
{
typedef FloatImageType::PixelType OutPixel;
typedef FloatVectorImageType::PixelType InPixel;
public:
  LuminanceOperator()
    {
    m_Rgb = new int [3] ;
    m_LumCoef = new float [3] ;
    }
  virtual ~LuminanceOperator() { }

 OutPixel operator() (  InPixel  input )
  {  
  OutPixel out;  
  out = (m_LumCoef[0]*input[m_Rgb[0]] + \
         m_LumCoef[1]*input[m_Rgb[1]] + \
         m_LumCoef[2]*input[m_Rgb[2]] ) ;
  return out;
  } // end operator ()


  void SetRgb( int rgb[])
    {
    m_Rgb = rgb;
    }

  void SetLumCoef(float lumCoef[])
    {
    m_LumCoef = lumCoef;
    }
private:
  int * m_Rgb;
  float * m_LumCoef;

}; // end of functor class  MultiplyOperator

}  // end of fonctor 

class ContrastEnhancement : public Application
{
public:
  /** Standard class typedefs. */
  typedef ContrastEnhancement	              Self;
  typedef Application	                      Superclass;
  typedef itk::SmartPointer < Self >	      Pointer;
  typedef itk::SmartPointer < const Self >	ConstPointer;

  typedef otb::ComputeHistoFilter < FloatImageType , FloatVectorImageType > 
          FilterHistoType;
  typedef otb::ComputeLutFilter < FloatVectorImageType , FloatVectorImageType > 
          FilterLutType;
  typedef otb::ComputeGainFilter < FloatImageType , FloatVectorImageType , FloatImageType > 
          FilterGainType;
  typedef otb :: ImageList< FloatImageType > ImageListType;
  typedef otb::VectorImageToImageListFilter< FloatVectorImageType, ImageListType > 
          VectorToImageListFilterType;
  typedef otb::ImageListToVectorImageFilter< ImageListType, FloatVectorImageType > 
          ImageListToVectorFilterType;
  typedef otb::StreamingStatisticsVectorImageFilter< FloatVectorImageType >
          VectorStatsFilterType;
  typedef otb::StreamingStatisticsImageFilter< FloatImageType >
          StatsFilterType;
  typedef itk::UnaryFunctorImageFilter < FloatVectorImageType ,
          FloatImageType , Functor::LuminanceOperator > LuminanceFilter;
  typedef otb::StreamingImageVirtualWriter < FloatVectorImageType > 
          VirtualWriter;
  typedef otb::StreamingImageRAMWriter < FloatVectorImageType > RAMWriter;

  /** Standard macro */
  itkNewMacro( Self );
 
  itkTypeMacro( ContrastEnhancement , otb::Application );

private:

	void DoInit() ITK_OVERRIDE
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
    SetParameterString("in", "/home/antoine/dev/my_data/DATA_PHR/PHR1B_MS_SENSOR_20170521_103758/IMG_PHR1B_MS_201705211037588_SEN_2323292101-004_R1C1.JP2");
    AddParameter(ParameterType_OutputImage, "out",  "Output Image");
    SetParameterDescription("out", "Output image.");
    SetParameterString("out", "/home/antoine/dev/my_data/bigbigtest.tif");


    AddParameter(ParameterType_Int,"bin","Bin number");
    SetDefaultParameterInt("bin", 256);
    SetParameterDescription("bin","Number of bin in the histogram");

    AddParameter(ParameterType_Float,"hfact","contrast Limitation");  
    SetParameterDescription("hfact","This parameter will set the maximum "
      "height accepted in a bin on the histogram of the input image. "
      "the height will be computated as hfact*eqHeight where eqHeight "
      "is the height of the theorical flat histogram");
    MandatoryOff("hfact");

    AddParameter(ParameterType_Float,"nodata","nodata value");
    MandatoryOff("nodata");

    AddParameter(ParameterType_Group, "thumb", "Thumbnail size");

    AddParameter(ParameterType_Int,"thumb.h","Thumbnail height in pixel");
    AddParameter(ParameterType_Int,"thumb.w","Thumbnail width in pixel");

    AddParameter(ParameterType_Choice , "minmax" , "Minimum and maximum definition");
    AddChoice( "minmax.auto" , "Automatique" );
    AddChoice( "minmax.man" , "Manuel" );
    AddParameter(ParameterType_Float , "min" , "Minimum");
    AddParameter(ParameterType_Float , "max" , "Maximum");
    // SetDefaultParameterFloat("min", 0.0 );
    // SetDefaultParameterFloat("max", 0.0 );
    MandatoryOff("min");
    MandatoryOff("max");

    AddParameter(ParameterType_Choice , "mode" , "What to equalized");
    AddChoice( "mode.each" , "Chanels" );
    SetParameterDescription( "mode.each" ,
                "Each chanel are equalized independently" );
    AddChoice( "mode.lum" , "Luminance" );
    SetParameterDescription( "mode.lum" ,
                "The luminance is equalized and then a gain is applied "
                "on the chanels." );
    AddParameter(ParameterType_Group , "mode.lum.red" , "Red Chanel" );
    AddParameter(ParameterType_Int , "mode.lum.red.ch" , "Red Chanel" );
    SetDefaultParameterInt("mode.lum.red.ch", 0 );
    AddParameter(ParameterType_Float , "mode.lum.red.coef" ,
                 "Value for luminance computation" );
    SetDefaultParameterFloat("mode.lum.red.coef", 0.21 );

    AddParameter(ParameterType_Group , "mode.lum.gre" , "Green Chanel" );
    AddParameter(ParameterType_Int , "mode.lum.gre.ch" , "Green Chanel" );
    SetDefaultParameterInt("mode.lum.gre.ch", 1 );
    AddParameter(ParameterType_Float , "mode.lum.gre.coef" ,
                 "Value for luminance computation" );
    SetDefaultParameterFloat("mode.lum.gre.coef", 0.71 );

    AddParameter(ParameterType_Group , "mode.lum.blu" , "Blue Chanel" );
    AddParameter(ParameterType_Int , "mode.lum.blu.ch" , "Blue Chanel" );
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

  void DoUpdateParameters() ITK_OVERRIDE
  {
    if (HasValue("in") )
      {
      FloatVectorImageType * inImage = GetParameterImage("in");
      FloatVectorImageType::RegionType::SizeType size;
      size = inImage->GetLargestPossibleRegion().GetSize() ;

      if ( !HasUserValue("thumb.w") )
        SetParameterInt( "thumb.w" , size[0] );
        
      if ( !HasUserValue("thumb.h") )
        SetParameterInt( "thumb.h" , size[1] );
      
      if ( HasUserValue("thumb.h") || HasUserValue("thumb.w") )
        CheckValidity( size );
      
      
      if ( !HasUserValue("nodata") )
        SetDefaultValue( inImage , "NODATA" );

      if ( GetParameterString( "mode" ) == "lum" && 
           !HasUserValue("mode.lum.red.ch") &&
           !HasUserValue("mode.lum.gre.ch") &&
           !HasUserValue("mode.lum.blu.ch") )
        SetDefaultValue( inImage , "RGB" );

      }

    if ( GetParameterString("minmax") == "man" )
      {
      MandatoryOn("min");
      MandatoryOn("max");
      }
    else if ( GetParameterString("minmax") == "auto" )
      {
      MandatoryOff("min");
      MandatoryOff("max");
      }
  }

  void DoExecute() ITK_OVERRIDE
  {
    std::string mode = GetParameterString("mode");
    FloatVectorImageType * inImage = GetParameterImage("in");
    ImageListType::Pointer outputImageList ( ImageListType::New() ); 
    m_vectorToImageListFilter = VectorToImageListFilterType::New() ;
    m_vectorToImageListFilter->SetInput( inImage );
    m_vectorToImageListFilter->UpdateOutputInformation();
    ImageListType::Pointer inputImageList = 
                  m_vectorToImageListFilter->GetOutput();
    int nbChanel = inImage->GetVectorLength ();

    if ( mode == "each")
      {
      // Each chanel will be equalized
      PerBandEqualization( inImage , inputImageList , 
                           nbChanel , outputImageList );
      }

    else if ( mode == "lum")
      {
      int rgb[3];
      rgb[0] = GetParameterInt("mode.lum.red.ch");
      rgb[1] = GetParameterInt("mode.lum.gre.ch");
      rgb[2] = GetParameterInt("mode.lum.blu.ch");
      ComputeLuminance( inImage , rgb );
      LuminanceEqualization( inputImageList , rgb , outputImageList );
      }

    m_imageListToVectorFilterOut = ImageListToVectorFilterType::New() ;
    m_imageListToVectorFilterOut->SetInput(outputImageList);

    #ifdef DEBUG
    std::cout<<"vectortoimagelist R"<<m_vectorToImageListFilter->GetOutput()->GetNthElement(0)->GetRequestedRegion().GetSize()<<std::endl;
    std::cout<<"vectortoimagelist B"<<m_vectorToImageListFilter->GetOutput()->GetNthElement(0)->GetBufferedRegion().GetSize()<<std::endl;
    std::cout<<"vectortoimagelist L"<<m_vectorToImageListFilter->GetOutput()->GetNthElement(0)->GetLargestPossibleRegion().GetSize()<<std::endl;
    std::cout<<"filterLut R"<<m_filterLut[0]->GetOutput()->GetRequestedRegion().GetSize()<<std::endl;
    std::cout<<"filterLut B"<<m_filterLut[0]->GetOutput()->GetBufferedRegion().GetSize()<<std::endl;
    std::cout<<"filterLut L"<<m_filterLut[0]->GetOutput()->GetLargestPossibleRegion().GetSize()<<std::endl;
    std::cout<<"filterGain R"<<m_filterGain[0]->GetOutput()->GetRequestedRegion().GetSize()<<std::endl;
    std::cout<<"filterGain B"<<m_filterGain[0]->GetOutput()->GetBufferedRegion().GetSize()<<std::endl;
    std::cout<<"filterGain L"<<m_filterGain[0]->GetOutput()->GetLargestPossibleRegion().GetSize()<<std::endl;
    std::cout<<"outputImageList R"<<outputImageList->GetNthElement(0)->GetRequestedRegion().GetSize()<<std::endl;
    std::cout<<"outputImageList B"<<outputImageList->GetNthElement(0)->GetBufferedRegion().GetSize()<<std::endl;
    std::cout<<"outputImageList L"<<outputImageList->GetNthElement(0)->GetLargestPossibleRegion().GetSize()<<std::endl;
    #endif

    SetParameterOutputImage( "out" , m_imageListToVectorFilterOut->GetOutput() );
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

  // Check if the image size is a multiple of the thumbnail size
  void CheckValidity( const FloatVectorImageType::RegionType::SizeType size )
  {
    std::ostringstream oss;
      if ( HasUserValue("thumb.w") && 
           size[0]%GetParameterInt("thumb.w") != 0 )
        {   
        oss <<"error : wThumbnail = "<<GetParameterInt("thumb.w")<<
              " is not a divider of the input's width"<<std::endl;
        oss<<"Image Width = "<<size[0]<<std::endl;
        }
      if ( HasUserValue("thumb.h") && 
           size[1]%GetParameterInt("thumb.h") != 0 )
        {
        oss <<"error : hThumbnail = "<<GetParameterInt("thumb.h")<<
              " is not a divider of the input's height"<<std::endl;
        oss<<"Image Height = "<<size[1]<<std::endl;
        }
      otbAppLogINFO( << oss.str() );
  }

  // Prepare the first half of the pipe that is common to every methode of 
  // equalization
  void SetUpPipeline( const FilterHistoType::Pointer filterHisto ,
                      const FilterLutType::Pointer filterLut ,
                      const RAMWriter::Pointer ramWriter ,
                      const FloatImageType::Pointer input ,
                      float max ,
                      float min)
  {
    if ( HasValue("hfact") )
      {
      filterHisto->SetThreshold( GetParameterInt("hfact") );
      }
    if ( HasUserValue("nodata") )
      {
      filterHisto->SetNoData( GetParameterFloat("nodata") );
      }
    filterHisto->SetMin( min );
    filterHisto->SetMax( max );
    filterLut->SetMin( min );
    filterLut->SetMax( max );
    filterHisto->SetNbBin(GetParameterInt("bin"));
    FloatImageType::SizeType thumbSize;
    thumbSize[0] = GetParameterInt("thumb.w");
    thumbSize[1] = GetParameterInt("thumb.h");
    filterHisto->SetThumbSize( thumbSize );
    filterHisto->SetInput( input ) ;
    ramWriter->SetInput( filterHisto->GetHistoOutput() );
    VirtualWriter::Pointer virtualWriter ( VirtualWriter::New() );
    virtualWriter->SetInput( ramWriter->GetOutput() );
    virtualWriter->Update();
    filterLut->SetInput( ramWriter->GetOutput() );
    filterLut->Update();
  }

  // Compute min max from a vector image
  void ComputeVectorMinMax( const FloatVectorImageType::Pointer inImage ,
                            FloatVectorImageType::PixelType & max ,
                            FloatVectorImageType::PixelType & min )
  {
    if ( GetParameterString("minmax") == "man" )
      {
      min.Fill( GetParameterFloat("min") );
      max.Fill( GetParameterFloat("max") );
      }
    else if ( GetParameterString("minmax") == "auto" )
      {
      VectorStatsFilterType::Pointer statFilter ( VectorStatsFilterType::New() );
      statFilter->SetIgnoreInfiniteValues(true);
      if( HasUserValue("nodata") )
        {
        statFilter->SetIgnoreUserDefinedValue(true);
        statFilter->SetUserIgnoredValue( GetParameterFloat("nodata") );
        }
      statFilter->SetInput( inImage );
      statFilter->Update();
      min = statFilter->GetMinimum();
      max = statFilter->GetMaximum();
      }
  }

  // Compute min miac from an image
  void ComputeFloatMinMax( const FloatImageType::Pointer luminance ,
                           FloatImageType::PixelType & max ,
                           FloatImageType::PixelType & min )
  {
    if ( GetParameterString("minmax") == "man" )
      {
      min = GetParameterFloat("min") ;
      max =  GetParameterFloat("max") ;
      }
    else if ( GetParameterString("minmax") == "auto" )
      {
      StatsFilterType::Pointer statFilter ( StatsFilterType::New() );
      statFilter->SetIgnoreInfiniteValues(true);
      if( HasUserValue("nodata") )
        {
        statFilter->SetIgnoreUserDefinedValue(true);
        statFilter->SetUserIgnoredValue( GetParameterFloat("nodata") );
        }
      statFilter->SetInput( luminance );
      statFilter->Update();
      min = statFilter->GetMinimum();
      max = statFilter->GetMaximum();
      }
  }

  // Function corresponding to the "each" mode
  void PerBandEqualization( FloatVectorImageType::Pointer inImage ,
                            const ImageListType::Pointer inputImageList ,
                            const int nbChanel,
                            ImageListType::Pointer outputImageList )
  {
    FloatVectorImageType::PixelType min(nbChanel) , max(nbChanel);
    min.Fill(0);
    max.Fill(0);
    ComputeVectorMinMax( inImage , max , min );

    m_filterHisto.resize(nbChanel);
    m_filterLut.resize(nbChanel);
    m_filterGain.resize(nbChanel);
    m_RAMWriter.resize(nbChanel);

    for (int chanel = 0 ; chanel<nbChanel ; chanel++ ) 
      {
      m_filterHisto[chanel] = FilterHistoType::New();
      m_filterLut[chanel] = FilterLutType::New();
      m_filterGain[chanel] = FilterGainType::New();
      m_RAMWriter[chanel] = RAMWriter::New();

      if ( min[chanel] == max[chanel] )
        {
          //TODO Warn user through log
          std::cout<<"Chanel constant"<<std::endl;
          std::cout<<"min "<<min[chanel]<<std::endl;
          std::cout<<"max "<<max[chanel]<<std::endl;
          outputImageList->PushBack( inputImageList->GetNthElement(chanel) );
          continue;
        }
        
      SetUpPipeline ( m_filterHisto[chanel] , m_filterLut[chanel] ,
                      m_RAMWriter[chanel] ,
                      inputImageList->GetNthElement(chanel) ,
                      max[chanel] , min[chanel] );

      if( HasUserValue("nodata") )
        {
        m_filterGain[chanel]->SetNoData( GetParameterFloat("nodata") ); 
        }
      m_filterGain[chanel]->SetMin( m_filterLut[chanel]->GetMin() );
      m_filterGain[chanel]->SetMax( m_filterLut[chanel]->GetMax() );
      m_filterGain[chanel]->SetInputLut( m_filterLut[chanel]->GetOutput() );
      m_filterGain[chanel]->SetInputImage( 
      m_vectorToImageListFilter->GetOutput()->GetNthElement(chanel) );
      outputImageList->PushBack( m_filterGain[chanel]->GetOutput() );
      }
  }

  // Compute the luminance with user parameters
  void ComputeLuminance( const FloatVectorImageType::Pointer inImage ,
                         int rgb[] )
  {
    // Retreive the coeff for each chanel
    float lumCoef[3];
    lumCoef[0] = GetParameterFloat("mode.lum.red.coef");
    lumCoef[1] = GetParameterFloat("mode.lum.gre.coef");
    lumCoef[2] = GetParameterFloat("mode.lum.blu.coef");
    // Normalize those coeff
    float sum = 0.0;
    for (float f : lumCoef)
      {
      sum +=f;
      }
    assert(sum>0);
    for (int i = 0 ; i<3 ; i++ )
      {
      lumCoef[i] /= sum;
      }
    m_luminanceFilter =  LuminanceFilter::New() ;
    m_luminanceFilter->GetFunctor().SetRgb(rgb);
    m_luminanceFilter->GetFunctor().SetLumCoef(lumCoef);
    m_luminanceFilter->SetInput( inImage );
  }

  // Equalize the lumiance and apply the corresponding gain on each chanel
  // used to compute this luminance
  void LuminanceEqualization( const ImageListType::Pointer inputImageList ,
                              const int rgb[] ,
                              ImageListType::Pointer outputImageList )
  {
    m_filterHisto.resize(1);
    m_filterLut.resize(1);
    m_RAMWriter.resize(1);
    m_filterGain.resize(3);
    m_filterHisto[0] = FilterHistoType::New();
    m_filterLut[0] = FilterLutType::New();
    m_RAMWriter[0] = RAMWriter::New();
    // Retreive order of the RGB chanels
    FloatImageType::PixelType min(0) , max(0);
    ComputeFloatMinMax( m_luminanceFilter->GetOutput() , max , min );

    SetUpPipeline ( m_filterHisto[0] , m_filterLut[0] ,
                    m_RAMWriter[0] ,
                    m_luminanceFilter->GetOutput() ,
                    max , min);

    for ( int chanel = 0 ; chanel < 3 ; chanel++ ) 
      {
      m_filterGain[chanel] = FilterGainType::New();
      m_filterGain[chanel]->SetInputLut( m_filterLut[0]->GetOutput() );
      if( HasUserValue("nodata") )
        {
        m_filterGain[chanel]->SetNoData( GetParameterFloat("nodata") ); 
        }
      m_filterGain[chanel]->SetMin( min );
      m_filterGain[chanel]->SetMax( max );
      m_filterGain[chanel]->SetInputImage( 
                    inputImageList->GetNthElement(rgb[chanel]) );
      outputImageList->PushBack( m_filterGain[chanel]->GetOutput() );
      }
  }

  ImageListToVectorFilterType::Pointer m_imageListToVectorFilterOut;
  LuminanceFilter::Pointer m_luminanceFilter;
  VectorToImageListFilterType::Pointer m_vectorToImageListFilter;
  std::vector < FilterHistoType::Pointer > m_filterHisto;
  std::vector < FilterLutType::Pointer > m_filterLut;
  std::vector < FilterGainType::Pointer > m_filterGain;
  std::vector < RAMWriter::Pointer > m_RAMWriter;

};


} //End namespace Wrapper
} //End namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::ContrastEnhancement)