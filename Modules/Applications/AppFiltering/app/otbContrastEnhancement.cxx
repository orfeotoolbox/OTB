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
#include "otbStreamingStatisticsImageFilter.h"
#include "itkUnaryFunctorImageFilter.h"

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
  typedef otb::StreamingStatisticsImageFilter< FloatImageType >
          StatsFilterType;

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
    SetParameterString("in", "/home/antoine/dev/my_data/lena/lena.jpg");
    AddParameter(ParameterType_OutputImage, "out",  "Output Image");
    SetParameterDescription("out", "Output image.");
    SetParameterString("out", "/home/antoine/dev/my_data/lenaapp.jpg");


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
    AddChoice( "mode.each" , "Channels" );
    SetParameterDescription( "mode.each" ,
                "Each channel are equalized independently" );
    AddChoice( "mode.lum" , "Luminance" );
    SetParameterDescription( "mode.lum" ,
                "The luminance is equalized and then a gain is applied "
                "on the channels." );
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
    SetMinimumParameterIntValue("thumb.h", 0);
    SetMinimumParameterIntValue("thumb.w", 0);

    AddRAMParameter(); 
  }

  void DoUpdateParameters() ITK_OVERRIDE
  {
    if (HasValue("in") )
      {
      FloatVectorImageType * inImage = GetParameterImage("in");
      if ( !HasUserValue("thumb.w") )
        {
        SetParameterInt( "thumb.w" , 
                      inImage->GetLargestPossibleRegion().GetSize()[0] );
        }
      if ( !HasUserValue("thumb.h") )
        {
        SetParameterInt( "thumb.h" , 
                      inImage->GetLargestPossibleRegion().GetSize()[1] );
        }
      
      {
      std::ostringstream oss;
      if ( HasUserValue("thumb.h") && 
        GetParameterImage("in")->GetLargestPossibleRegion().GetSize()[1]%GetParameterInt("thumb.h") != 0 )
        {
        oss <<"error : hThumbnail = "<<GetParameterInt("thumb.h")<<
              " is not a divider of the input's height"<<std::endl;
        oss<<"Image Height = "<<GetParameterImage("in")->GetLargestPossibleRegion().GetSize()[1]<<std::endl;
        // std::cout<<"error : hThumbnail = "<<GetParameterInt("thumb.h")<<" is not a divider of the input's height"<<std::endl;
        // std::cout<<"Image Height = "<<GetParameterImage("in")->GetLargestPossibleRegion().GetSize()[1]<<std::endl;
        }
      if ( HasUserValue("thumb.w") && 
        GetParameterImage("in")->GetLargestPossibleRegion().GetSize()[0]%GetParameterInt("thumb.w") != 0 )
        {   
        oss <<"error : wThumbnail = "<<GetParameterInt("thumb.w")<<
              " is not a divider of the input's width"<<std::endl;
        oss<<"Image Width = "<<GetParameterImage("in")->GetLargestPossibleRegion().GetSize()[1]<<std::endl;
        // std::cout<<"error : wThumbnail = "<<GetParameterInt("thumb.w")<<"is not a divider of the input's width"<<std::endl;
        // std::cout<<"Image Width = "<<GetParameterImage("in")->GetLargestPossibleRegion().GetSize()[0]<<std::endl;
        }
      otbAppLogINFO( << oss.str() );
      }
      if ( !HasUserValue("nodata") )
        {
        typedef ImageMetadataInterfaceBase ImageMetadataInterfaceType;
        ImageMetadataInterfaceType::Pointer metadataInterface = 
            ImageMetadataInterfaceFactory::CreateIMI(inImage->GetMetaDataDictionary());
        std::vector<double> values;
        std::vector<bool> flags;

        bool ret = metadataInterface->GetNoDataFlags(flags,values);

        if(ret && !values.empty() && !flags.empty() && flags[0])
          {
          SetParameterFloat( "nodata" , static_cast<float>( values[0] ) );
          }
        if ( GetParameterString( "mode" ) == "lum" && 
             !HasUserValue("mode.lum.red.ch") &&
             !HasUserValue("mode.lum.gre.ch") &&
             !HasUserValue("mode.lum.blu.ch") )
          {
          std::vector<uint> rgb = metadataInterface->GetDefaultDisplay() ;
          uint m = inImage->GetVectorLength ();
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
      }

      if ( GetParameterString("minmax") == "man" )
        {
        MandatoryOn("min");
        MandatoryOn("max");
        }
      if ( GetParameterString("minmax") == "auto" )
        {
        MandatoryOff("min");
        MandatoryOff("max");
        }
  }

  void SetUpPipeline( const FilterHistoType::Pointer filterHisto ,
                      const FilterLutType::Pointer filterLut ,
                      const FloatImageType::Pointer input )
  {
    float min = 0.0;
    float max = 0.0;
    if ( GetParameterString("minmax") == "man" )
      {
      min = GetParameterFloat("min");
      max = GetParameterFloat("max");
      }
    if ( GetParameterString("minmax") == "auto" )
      {
      StatsFilterType::Pointer statFilter ( StatsFilterType::New() );
      statFilter->SetIgnoreInfiniteValues(true);
      if( HasUserValue("nodata") )
        {
        statFilter->SetIgnoreUserDefinedValue(true);
        statFilter->SetUserIgnoredValue( GetParameterFloat("nodata") );
        }
      statFilter->SetInput( input );
      statFilter->Update();
      min = statFilter->GetMinimum();
      max = statFilter->GetMaximum();
      }
    filterHisto->SetInput( input ) ;
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
    filterLut->SetInput(filterHisto->GetOutput());
    filterLut->Update();
  }

  void DoExecute() ITK_OVERRIDE
  {
    ImageListToVectorFilterType::Pointer 
          imageListToVectorFilterOut( ImageListToVectorFilterType::New() );
    FloatVectorImageType * inImage = GetParameterImage("in");
    ImageListType::Pointer outputImageList ( ImageListType::New() );
    VectorToImageListFilterType::Pointer 
          vectorToImageListFilter ( VectorToImageListFilterType::New() );
    vectorToImageListFilter->SetInput( inImage );
    vectorToImageListFilter->UpdateOutputInformation();

    ImageListType::Pointer inputImageList = vectorToImageListFilter->GetOutput();

    std::vector < FilterHistoType::Pointer > filterHisto;
    std::vector < FilterLutType::Pointer > filterLut;
    std::vector < FilterGainType::Pointer > filterGain;
    if ( GetParameterString("mode") == "each")
      {
      // Each channel will be equalized
      int m = inImage->GetVectorLength ();
      filterHisto.resize(m);
      filterLut.resize(m);
      filterGain.resize(m);
      for (int chanel = 0 ; chanel<m ; chanel++ ) 
        {
        filterHisto[chanel] = FilterHistoType::New();
        filterLut[chanel] = FilterLutType::New();
        filterGain[chanel] = FilterGainType::New();
        // std::cout<<"channel m ="<<m<<std::endl;
        SetUpPipeline ( filterHisto[chanel] , filterLut[chanel] ,
                        inputImageList->GetNthElement(chanel) );
        
        if( HasUserValue("nodata") )
          {
          filterGain[chanel]->SetNoData( GetParameterFloat("nodata") ); 
          }
        filterGain[chanel]->SetMin( filterLut[chanel]->GetMin() );
        filterGain[chanel]->SetMax( filterLut[chanel]->GetMax() );
        filterGain[chanel]->SetInputLut( filterLut[chanel]->GetOutput() );
        filterGain[chanel]->SetInputImage( vectorToImageListFilter->GetOutput()->GetNthElement(chanel) );
        outputImageList->PushBack( filterGain[chanel]->GetOutput() );
        }
      }

    if ( GetParameterString("mode") == "lum")
      {
      filterHisto.resize(1);
      filterLut.resize(1);
      filterGain.resize(3);
      filterHisto[0] = FilterHistoType::New();
      filterLut[0] = FilterLutType::New();
      // Retreive order of the RGB channels
      int rgb[3];
      rgb[0] = GetParameterInt("mode.lum.red.ch");
      rgb[1] = GetParameterInt("mode.lum.gre.ch");
      rgb[2] = GetParameterInt("mode.lum.blu.ch");
      // Retreive the coeff for each channel
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
      typedef itk::UnaryFunctorImageFilter < FloatVectorImageType ,
              FloatImageType , Functor::LuminanceOperator > LuminanceFilter;
      LuminanceFilter::Pointer luminanceFilter ( LuminanceFilter::New() );
      luminanceFilter->GetFunctor().SetRgb(rgb);
      luminanceFilter->GetFunctor().SetLumCoef(lumCoef);
      luminanceFilter->SetInput( inImage );
      
      SetUpPipeline ( filterHisto[0] , filterLut[0] ,
                      luminanceFilter->GetOutput() );
      for ( int chanel : rgb ) 
        {
        filterGain[chanel] = FilterGainType::New();
        filterGain[chanel]->SetInputLut( filterLut[0]->GetOutput() );
        if( HasUserValue("nodata") )
          {
          filterGain[chanel]->SetNoData( GetParameterFloat("nodata") ); 
          }
        filterGain[chanel]->SetMin( filterLut[0]->GetMin() );
        filterGain[chanel]->SetMax( filterLut[0]->GetMax() );
        filterGain[chanel]->SetInputImage( inputImageList->GetNthElement(chanel) );

        outputImageList->PushBack( filterGain[chanel]->GetOutput() );
        }
      }
    imageListToVectorFilterOut->SetInput(outputImageList);
    imageListToVectorFilterOut->UpdateOutputInformation();
    imageListToVectorFilterOut->Update();
    SetParameterOutputImage( "out" , imageListToVectorFilterOut->GetOutput() );
  }
};

} //End namespace Wrapper
} //End namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::ContrastEnhancement)