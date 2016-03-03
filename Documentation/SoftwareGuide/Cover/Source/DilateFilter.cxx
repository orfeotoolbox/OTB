/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: DilateFilter.cxx,v $
  Language:  C++
  Date:      $Date: 2003/07/25 14:12:35 $
  Version:   $Revision: 1.1 $

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/



#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkBinaryDilateImageFilter.h"
#include "itkBinaryBallStructuringElement.h" 
#include "itkImage.h"


int main( int argc, char ** argv )
{

  // Verify the number of parameters in the command line
  if( argc < 4 )
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " inputImageFile  outputImageFile " << std::endl;
    std::cerr << " radius " << std::endl;
    return -1;
    }


  typedef unsigned char        PixelType;

  const   unsigned int        Dimension = 3;

  typedef itk::Image< PixelType,  Dimension >    ImageType;



  typedef itk::ImageFileReader< ImageType >  ReaderType;
  typedef itk::ImageFileWriter< ImageType >  WriterType;


  typedef itk::BinaryBallStructuringElement< 
                                        PixelType,
                                        Dimension  >             
                                             StructuringElementType;
                      
  typedef itk::BinaryDilateImageFilter< 
                                  ImageType, 
                                  ImageType, 
                                  StructuringElementType > FilterType;

  FilterType::Pointer filter = FilterType::New();


  unsigned int radius = atoi( argv[3] );

  StructuringElementType  structuringElement;

  structuringElement.SetRadius( radius );  

  structuringElement.CreateStructuringElement();

  filter->SetKernel(  structuringElement );


  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];

  reader->SetFileName( inputFilename  );
  writer->SetFileName( outputFilename );


  filter->SetInput( reader->GetOutput() );

  writer->SetInput( filter->GetOutput() );


  try 
    { 
    writer->Update(); 
    } 
  catch( itk::ExceptionObject & err ) 
    { 
    std::cout << "ExceptionObject caught !" << std::endl; 
    std::cout << err << std::endl; 
    return -1;
    } 



  return 0;


}



