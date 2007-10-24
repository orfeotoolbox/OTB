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

//  Software Guide : BeginCommandLineArgs
//  DEM_srtm
//  OUTPUTS: {DEMToImageGenerator.tif}
//  44.5 6.5 500 500 0.002
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// \index{otb::DEMToImageGenerator}
// \index{otb::DEMHandler}
//
//
// The following example illustrates the use of the otb::DEMToImageGenerator class.
// The aim of this class is to generate an image from the srtm data (precising the start extraction 
// latitude and longitude point). Each pixel is a geographic point and its intensity is 
// the altitude of the point.
// If srtm doesn't have altitude information for a point, the altitude value is set at -32768 (value of the srtm norm).
//
// Let's look at the minimal code required to use this algorithm. First, the following header 
// defining the \doxygen{otb}{DEMToImageGenerator} class must be included.
// Software Guide : EndLatex 

// Software Guide : BeginCodeSnippet
#include "otbDEMToImageGenerator.h"
// Software Guide : EndCodeSnippet


#include "itkExceptionObject.h"
#include "otbImageFileWriter.h"
#include "otbImage.h"

int main(int argc, char * argv[])
{
  if(argc<7)
    {
      std::cout << argv[0] <<" folder path , output filename , X Output Orign point , Y Output Origin point , X Output Size, Y Output size , Spacing"  << std::endl;
      return EXIT_FAILURE;
    }
  //  Software Guide : BeginLatex
  //  
  // The image type is now defined using pixel type and 
  // dimension. The output image is defined as an \doxygen{otb}{Image}. 
  //
  // Software Guide : EndLatex 

  char * folderPath = argv[1];
  char * outputName = argv[2];
   // Software Guide : BeginCodeSnippet
  const unsigned int Dimension = 2;
  typedef otb::Image<double , Dimension>           ImageType;
  // Software Guide : EndCodeSnippet

  // The writer is defined 
  typedef otb::ImageFileWriter<ImageType>          WriterType;

  //  Software Guide : BeginLatex
  //  
  // The DEMToImageGenerator is defined using the image pixel 
  // type as a template parameter. After that, the object can be instancied.
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  typedef otb::DEMToImageGenerator<ImageType>      DEMToImageGeneratorType;

  DEMToImageGeneratorType::Pointer object = DEMToImageGeneratorType::New();

  // Software Guide : EndCodeSnippet


  //  Software Guide : BeginLatex
  //  
  // Input parameter types are defined to set the value in the DEMToImageGenerator.
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  typedef DEMToImageGeneratorType::SizeType        SizeType;
  typedef DEMToImageGeneratorType::SpacingType     SpacingType;
  typedef DEMToImageGeneratorType::DEMHandlerType  DEMHandlerType;
  typedef DEMHandlerType::PointType                PointType;
  // Software Guide : EndCodeSnippet
  
  // Instantiating writer
  WriterType::Pointer              writer = WriterType::New();
  

  // Software Guide : BeginLatex
  //  
  // The path to the DEM folder is given to the filter.
  //
  // Software Guide : EndLatex 
  // Software Guide : BeginCodeSnippet
  object->SetDEMDirectoryPath(folderPath);
 // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //  
  // The origin (Longitude/Latitude) of the output image in the DEM is given to the filter.
  //
  // Software Guide : EndLatex 
  // Software Guide : BeginCodeSnippet
  PointType origin;
  origin[0] = ::atof(argv[3]);
  origin[1] = ::atof(argv[4]);

  object->SetOutputOrigin(origin);
  // Software Guide : EndCodeSnippet


  //  Software Guide : BeginLatex
  //  
  // The size (in Pixel) of the output image is given to the filter.
  //
  // Software Guide : EndLatex 
  // Software Guide : BeginCodeSnippet
  SizeType size;
  size[0] = ::atoi(argv[5]);
  size[1] = ::atoi(argv[6]);
  object->SetOutputSize(size);
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //  
  // The spacing (step between to consecutive pixel) is given to the filter.
  // By default, this spacing is set at 0.001.
  //
  //  Software Guide : EndLatex 
  // Software Guide : BeginCodeSnippet
  SpacingType spacing(::atof(argv[7]));

  object->SetOutputSpacing(spacing);
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //  
  // The output image name is given to the writer and 
  // the filter output is linked to the writer input.
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  writer->SetFileName( outputName );

  writer->SetInput( object->GetOutput() );
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //  
  //  The invocation of the \code{Update()} method on the writer triggers the
  //  execution of the pipeline.  It is recommended to place update calls in a
  //  \code{try/catch} block in case errors occur and exceptions are thrown.
  //
  //  Software Guide : EndLatex 
  
  // Software Guide : BeginCodeSnippet
   try
    {
      writer->Update();
    }
  
  catch( itk::ExceptionObject & err ) 
    { 
      std::cout << "Exception itk::ExceptionObject thrown !" << std::endl; 
      std::cout << err << std::endl; 
    return EXIT_FAILURE;
    } 
   // Software Guide : EndCodeSnippet
  catch( ... ) 
    { 
      std::cout << "Unknown exception thrown !" << std::endl; 
      return EXIT_FAILURE;
    } 
  return EXIT_SUCCESS;
}
