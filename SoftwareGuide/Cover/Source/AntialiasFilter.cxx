/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: AntialiasFilter.cxx,v $
  Language:  C++
  Date:      $Date: 2003/07/25 14:42:04 $
  Version:   $Revision: 1.2 $

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/



#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkAntiAliasBinaryImageFilter.h"
#include "itkImage.h"


int main( int argc, char ** argv )
{

  // Verify the number of parameters in the command line
  if( argc < 5 )
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " inputImageFile  outputImageFile " << std::endl;
    std::cerr << " maximumRMSError maximumIterations " << std::endl;
    return -1;
    }


  typedef unsigned char        InputPixelType;
  typedef float                OutputPixelType;

  const   unsigned int        Dimension = 3;

  typedef itk::Image< InputPixelType,   Dimension >    InputImageType;
  typedef itk::Image< OutputPixelType,  Dimension >    OutputImageType;

  typedef itk::ImageFileReader< InputImageType >  ReaderType;
  typedef itk::ImageFileWriter< OutputImageType >  WriterType;


  typedef itk::AntiAliasBinaryImageFilter< 
                                  InputImageType, 
                                  OutputImageType > FilterType;

  FilterType::Pointer filter = FilterType::New();


  const double            maximumRMSError     =  atof( argv[3] );
  const unsigned int long numberOfIterations  =  atol( argv[4] );

  filter->SetMaximumRMSError( maximumRMSError );
  filter->SetMaximumIterations( numberOfIterations );

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



