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

#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbVectorRescaleIntensityImageFilter.h"
#include "itkCastImageFilter.h"

#include "otbCommandLineArgumentParser.h"

template<typename OutputPixelType>
int generic_main_convert(otb::CommandLineArgumentParseResult* parseResult)
{
  try 
  { 
    typedef long int InputPixelType;
    typedef otb::VectorImage<InputPixelType, 2> InputImageType;
    typedef otb::VectorImage<OutputPixelType, 2> OutputImageType;
    typedef otb::ImageFileReader<InputImageType> ReaderType;
    typedef otb::ImageFileWriter<OutputImageType> WriterType;
    typedef otb::VectorRescaleIntensityImageFilter<InputImageType, OutputImageType> RescalerType;
  //   typedef itk::CastImageFilter<InputImageType, OutputImageType> RescalerType;
    
    ReaderType::Pointer reader=ReaderType::New();
    WriterType::Pointer writer=WriterType::New();
    
    reader->SetFileName(argv[1]);
    
    
    OutputImageType::PixelType minimum,maximum;
    minimum.SetSize(3);
    maximum.SetSize(3);
    minimum.Fill(0);
    maximum.Fill(255);
    RescalerType::Pointer rescaler=RescalerType::New();
    rescaler->SetOutputMinimum(minimum);
    rescaler->SetOutputMaximum(maximum);
    rescaler->SetInput(reader->GetOutput());
    
    writer->SetFileName(argv[2]);
    //writer->SetInput(rescaler->GetOutput());
    writer->SetInput(reader->GetOutput());
  
  
    writer->Update();
    
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


int main(int argc, char ** argv)
{
  try
  { 
    // Parse command line parameters
    typedef otb::CommandLineArgumentParser ParserType;	
    ParserType::Pointer parser = ParserType::New();

    parser->AddOption("--XCarto","X cartographic value of desired point","-x");
    parser->AddOption("--YCarto","Y cartographic value of desired point","-y");
    parser->AddOptionNParams("--MapProjectionType","Type (UTM/LAMBERT/LAMBERT2/LAMBERT93/SINUS/ECKERT4/TRANSMERCATOR/MOLLWEID) and parameters of map projection used","-mapProj");				

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
    // Code
    
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
