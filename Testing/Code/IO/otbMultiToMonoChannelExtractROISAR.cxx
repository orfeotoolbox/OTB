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


#include "itkExceptionObject.h"

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbExtractROI.h"

int otbMultiToMonoChannelExtractROISAR ( int argc, char * argv[] )
{
  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];

  typedef std::complex<float>  	                                InputPixelType;
  typedef std::complex<float>  	                                OutputPixelType;


  typedef otb::ExtractROI< InputPixelType, 
    OutputPixelType >  ExtractROIFilterType;

  ExtractROIFilterType::Pointer extractROIFilter = ExtractROIFilterType::New();

  extractROIFilter->SetStartX( 10 );
  extractROIFilter->SetStartY( 10 );
  extractROIFilter->SetSizeX( 100 );
  extractROIFilter->SetSizeY( 100 );
	
  // Resume de la ligne de commande
  std::cout << " ROI selectionnee : startX "<<extractROIFilter->GetStartX()<<std::endl;
  std::cout << "                    startY "<<extractROIFilter->GetStartY()<<std::endl;
  std::cout << "                    sizeX  "<<extractROIFilter->GetSizeX()<<std::endl;
  std::cout << "                    sizeY  "<<extractROIFilter->GetSizeY()<<std::endl;

  typedef otb::ImageFileReader< ExtractROIFilterType::InputImageType >        ReaderType;
  typedef otb::ImageFileWriter< ExtractROIFilterType::OutputImageType >       WriterType;
  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName( inputFilename  );
  writer->SetFileName( outputFilename );
	
  extractROIFilter->SetInput( reader->GetOutput() );        
  writer->SetInput( extractROIFilter->GetOutput() );
  writer->Update(); 
  std::cout << " Nb canaux dans l'image d'entree : "<< reader->GetOutput()->GetNumberOfComponentsPerPixel()<<std::endl;
  std::cout << " Nb canaux dans l'image de sortie : "<<extractROIFilter->GetOutput()->GetNumberOfComponentsPerPixel() <<std::endl;



  return EXIT_SUCCESS;
}

