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
#include "otbMultiChannelExtractROI.h"

#include "itkImage.h"

int main ( int argc, char * argv[] )
{
  std::string linputPixelType;
  std::string loutputPixelType;
  const char * inputFilename(argv[1]);
  //        const char * outputFilename(argv[2]);
  typedef unsigned char  InputPixelType;
  typedef unsigned char  OutputPixelType;

  typedef otb::MultiChannelExtractROI< InputPixelType, 
    OutputPixelType >  ExtractROIFilterType;

  ExtractROIFilterType::Pointer extractROIFilter = ExtractROIFilterType::New();

  typedef otb::ImageFileReader< ExtractROIFilterType::InputImageType >       ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( inputFilename  );
  reader->Update(); //Necessaire pour connaitre le nombre de canaux dans l'image
  std::cout <<" reader->GetOutput() : "<<reader->GetOutput()<<std::endl;

  extractROIFilter->SetInput( reader->GetOutput() );

  extractROIFilter->SetStartX(50);
  extractROIFilter->SetStartY(50);
  extractROIFilter->SetSizeX(100);
  extractROIFilter->SetSizeY(100);
  extractROIFilter->UpdateLargestPossibleRegion();
  // Resume de la ligne de commande
  std::cout << " ROI selectionnee : startX "<<extractROIFilter->GetStartX()<<std::endl;
  std::cout << "                    startY "<<extractROIFilter->GetStartY()<<std::endl;
  std::cout << "                    sizeX  "<<extractROIFilter->GetSizeX()<<std::endl;
  std::cout << "                    sizeY  "<<extractROIFilter->GetSizeY()<<std::endl;
  extractROIFilter->Update();
  std::cout <<" reader->GetOutput()->GetLargestPossibleRegion() : "<<reader->GetOutput()->GetLargestPossibleRegion()<<std::endl;
  std::cout <<" reader->GetOutput()->GetBufferedRegion() : "<<reader->GetOutput()->GetBufferedRegion()<<std::endl;
  std::cout <<" reader->GetOutput()->GetRequestedRegion() : "<<reader->GetOutput()->GetRequestedRegion()<<std::endl;

  std::cout <<" extractROIFilter->GetOutput()->GetLargestPossibleRegion() : "<<extractROIFilter->GetOutput()->GetLargestPossibleRegion()<<std::endl;
  std::cout <<" extractROIFilter->GetOutput()->GetBufferedRegion() : "<<extractROIFilter->GetOutput()->GetBufferedRegion()<<std::endl;
  std::cout <<" extractROIFilter->GetOutput()->GetRequestedRegion() : "<<extractROIFilter->GetOutput()->GetRequestedRegion()<<std::endl;



  std::cout << " Deuxieme extrait" <<std::endl;

  extractROIFilter->SetStartX(150);
  extractROIFilter->SetStartY(150);
  extractROIFilter->SetSizeX(80);
  extractROIFilter->SetSizeY(95);
  // Resume de la ligne de commande
  std::cout << " ROI selectionnee : startX "<<extractROIFilter->GetStartX()<<std::endl;
  std::cout << "                    startY "<<extractROIFilter->GetStartY()<<std::endl;
  std::cout << "                    sizeX  "<<extractROIFilter->GetSizeX()<<std::endl;
  std::cout << "                    sizeY  "<<extractROIFilter->GetSizeY()<<std::endl;
  extractROIFilter->UpdateLargestPossibleRegion();

  extractROIFilter->Update();
  std::cout <<" reader->GetOutput()->GetLargestPossibleRegion() : "<<reader->GetOutput()->GetLargestPossibleRegion()<<std::endl;
  std::cout <<" reader->GetOutput()->GetBufferedRegion() : "<<reader->GetOutput()->GetBufferedRegion()<<std::endl;
  std::cout <<" reader->GetOutput()->GetRequestedRegion() : "<<reader->GetOutput()->GetRequestedRegion()<<std::endl;

  std::cout <<" extractROIFilter->GetOutput()->GetLargestPossibleRegion() : "<<extractROIFilter->GetOutput()->GetLargestPossibleRegion()<<std::endl;
  std::cout <<" extractROIFilter->GetOutput()->GetBufferedRegion() : "<<extractROIFilter->GetOutput()->GetBufferedRegion()<<std::endl;
  std::cout <<" extractROIFilter->GetOutput()->GetRequestedRegion() : "<<extractROIFilter->GetOutput()->GetRequestedRegion()<<std::endl;


  
  return EXIT_SUCCESS;  
}        
        

  





