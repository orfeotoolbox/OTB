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

 
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#ifdef __BORLANDC__
#define ITK_LEAN_AND_MEAN
#endif

#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbStreamingImageFileWriter.h"
#include "otbVectorRescaleIntensityImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "otbCommandLineArgumentParser.h"
#include "itkCastImageFilter.h"

template<typename OutputPixelType>
int generic_main_convert(otb::CommandLineArgumentParseResult* parseResult) 
{


  typedef otb::VectorImage<double, 2> InputImageType;
  typedef otb::VectorImage<OutputPixelType, 2> OutputImageType;
  typedef otb::ImageFileReader<OutputImageType> ReaderType;
  typedef otb::StreamingImageFileWriter<OutputImageType> WriterType;
//   typedef itk::RescaleIntensityImageFilter<InputImageType, OutputImageType> RescalerType;
//   typedef otb::VectorRescaleIntensityImageFilter<InputImageType, OutputImageType> RescalerType;
//   typedef itk::CastImageFilter<InputImageType, OutputImageType> RescalerType;
  
  typename ReaderType::Pointer reader=ReaderType::New();
  
  typename WriterType::Pointer writer=WriterType::New();
  
  reader->SetFileName(parseResult->GetInputImage().c_str());
  
  
//   OutputImageType::PixelType minimum,maximum;
//   minimum.SetSize(3);
//   maximum.SetSize(3);
//   minimum.Fill(0);
//   maximum.Fill(255);
//   typename RescalerType::Pointer rescaler=RescalerType::New();
//   rescaler->SetOutputMinimum(0);
//   rescaler->SetOutputMaximum(255);
// //   rescaler->SetOutputMinimum(minimum);
// //   rescaler->SetOutputMaximum(maximum);
//   rescaler->SetInput(reader->GetOutput());
  
  writer->SetFileName(parseResult->GetOutputImage().c_str());
  
//   writer->SetInput(rescaler->GetOutput());
  writer->SetInput(reader->GetOutput());
  
  writer->Update();
  
  return 0; 
}

int main(int argc, char ** argv)
{
  try
  { 
    // Parse command line parameters
    typedef otb::CommandLineArgumentParser ParserType;	
    ParserType::Pointer parser = ParserType::New();
    
    parser->AddInputImage();
    parser->AddOutputImage();
    parser->AddOption("--OutputPixelType","OutputPixelType: unsigned char (1), short int (2), int (3), float (4), double (5), unsigned short int (12), unsigned int (13)","-t", 1, false);

    typedef otb::CommandLineArgumentParseResult ParserResultType;
    ParserResultType::Pointer  parseResult = ParserResultType::New();
				
    try
    {
      parser->ParseCommandLine(argc,argv,parseResult);
    }
    catch( itk::ExceptionObject & err ) 
    { 
      std::string descriptionException = err.GetDescription();   
      if(descriptionException.find("ParseCommandLine(): Help Parser") != std::string::npos) 
      {
        std::cout << "WARNING : output file pixels are converted in 'unsigned char'" << std::endl;
        return EXIT_SUCCESS;
      }
      if(descriptionException.find("ParseCommandLine(): Version Parser") != std::string::npos) 
      {
        return EXIT_SUCCESS;
      }
      return EXIT_FAILURE;
    }	
    
    
    unsigned int type=parseResult->GetParameterUInt("--OutputPixelType");
    
    switch(type)
    {
      case 1:
        generic_main_convert<unsigned char>(parseResult);
        break;
      case 2:
        generic_main_convert<short int>(parseResult);
        break;
      case 3:
        generic_main_convert<int>(parseResult);
        break;
      case 4:
        generic_main_convert<float>(parseResult);
        break;
      case 5:
        generic_main_convert<double>(parseResult);
        break;
      case 12:
        generic_main_convert<unsigned short int>(parseResult);
        break;
      case 13:
        generic_main_convert<unsigned int>(parseResult);
        break;
      default:
        generic_main_convert<unsigned char>(parseResult);
        break;
    }
    
    
  }
  catch( itk::ExceptionObject & err ) 
  { 
    std::cout << "Exception itk::ExceptionObject levee !" << std::endl; 
    std::cout << err << std::endl; 
    return EXIT_FAILURE;
  } 
  catch( std::bad_alloc & err ) 
  { 
    std::cout << "Exception bad_alloc : "<<(char*)err.what()<< std::endl; 
    return EXIT_FAILURE;
  } 
  catch( ... ) 
  { 
    std::cout << "Exception levee inconnue !" << std::endl; 
    return EXIT_FAILURE;
  } 
  return EXIT_SUCCESS;
}
