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


// Software Guide : BeginLatex
//
// This example illustrates the details of the seam carving operation.
// References to this method can be found in \cite{Avidan07}. This example
// details the use of \doxygen{otb}{ImageToCarvingPathFilter} and 
// \doxygen{otb}{RemoveCarvingPathFilter}.
//
// Software Guide : EndLatex 

//  Software Guide : BeginCommandLineArgs
//    INPUTS: {QB_Suburb.png}
//    OUTPUTS: {SeamCarvingExampleOutput.png}
//    100
//  Software Guide : EndCommandLineArgs


#include "otbImage.h"
#include "itkPolyLineParametricPath.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkGradientMagnitudeImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"

#include "otbDrawPathFilter.h"
#include "otbImageToCarvingPathFilter.h"
#include "otbRemoveCarvingPathFilter.h"

#include "itkImageDuplicator.h"


int main(int argc, char ** argv)
{

  typedef float InputPixelType;
  typedef unsigned char OutputPixelType;
  const unsigned int Dimension = 2;
  
  typedef otb::Image< InputPixelType, Dimension > ImageType;
  typedef otb::Image< OutputPixelType, Dimension > OutputImageType;
  typedef itk::PolyLineParametricPath<Dimension> PathType;

  typedef otb::ImageFileReader< ImageType > ReaderType;
  typedef otb::ImageFileWriter< OutputImageType > WriterType;
  typedef itk::RescaleIntensityImageFilter<ImageType, OutputImageType> RescalerType;
  
  ReaderType::Pointer reader = ReaderType::New();  
  WriterType::Pointer writer = WriterType::New();
  RescalerType::Pointer rescaler = RescalerType::New();
  
  const char * filenamereader = argv[1];
  reader->SetFileName( filenamereader );

  const char * filenamewriter = argv[2];
  writer->SetFileName( filenamewriter );
  
  int iteration = atoi(argv[3]);
  
  //  Software Guide : BeginLatex
  //
  // Energy is computed according to the gradient of the image, thus an
  // \doxygen{itk}{GradientMagnitudeImageFilter} is instanciated
  //
  //  Software Guide : EndLatex
  
  // Software Guide : BeginCodeSnippet
  typedef itk::GradientMagnitudeImageFilter< ImageType, ImageType> GradientType;
  GradientType::Pointer gradient = GradientType::New();
  // Software Guide : EndCodeSnippet
  
  //  Software Guide : BeginLatex
  //
  // The \doxygen{otb}{ImageToCarvingPathFilter} compute the path of minimum energy
  // according to lines or columns of the image. Later, as we will choose the best option
  // between the two, we need two of these filters.
  //
  //  Software Guide : EndLatex
  
  // Software Guide : BeginCodeSnippet
  typedef otb::ImageToCarvingPathFilter<ImageType, PathType> CarvingFilterType;
  CarvingFilterType::Pointer carvingFilterVert = CarvingFilterType::New();
  CarvingFilterType::Pointer carvingFilterHor = CarvingFilterType::New();
  // Software Guide : EndCodeSnippet
  
    //  Software Guide : BeginLatex
  //
  // The \doxygen{otb}{RemoveCarvingPathFilter} will really resize the image deleting
  // the path.
  //
  //  Software Guide : EndLatex
  
  // Software Guide : BeginCodeSnippet
  typedef otb::RemoveCarvingPathFilter
      <ImageType, PathType, ImageType> RemoveCarvingPathFilterType;
  RemoveCarvingPathFilterType::Pointer removeCarvingPath = RemoveCarvingPathFilterType::New();
  // Software Guide : EndCodeSnippet
  
  
  typedef itk::ImageDuplicator< ImageType > duplicatorType;
  duplicatorType::Pointer duplicator = duplicatorType::New();
  reader->Update();
  duplicator->SetInputImage(reader->GetOutput());
  duplicator->Update();
  
  double energyVert, energyHor;
  
  for (int i=0; i<iteration; i++)
  {
    std::cout << "Iterations: " << i << std::endl;
    
    
    gradient->SetInput( duplicator->GetOutput() );
    
    carvingFilterVert->SetInput( gradient->GetOutput() );
    carvingFilterVert->SetDirection(0);
    carvingFilterVert->UpdateLargestPossibleRegion();
    energyVert = carvingFilterVert->GetEnergyPerPix();
    
    carvingFilterHor->SetInput( gradient->GetOutput() );
    carvingFilterHor->SetDirection(1);
    carvingFilterHor->UpdateLargestPossibleRegion();
    energyHor = carvingFilterHor->GetEnergyPerPix();
    
    if (energyVert < energyHor)
    {
      removeCarvingPath->SetInput( duplicator->GetOutput() );
      removeCarvingPath->SetPathInput( carvingFilterVert->GetOutput() );
      removeCarvingPath->SetDirection(0);
      removeCarvingPath->UpdateLargestPossibleRegion();
    }
    else
    {
      removeCarvingPath->SetInput( duplicator->GetOutput() );
      removeCarvingPath->SetPathInput( carvingFilterHor->GetOutput() );
      removeCarvingPath->SetDirection(1);
      removeCarvingPath->UpdateLargestPossibleRegion();
    }
    
    duplicator->SetInputImage(removeCarvingPath->GetOutput());
    duplicator->Update();

  }
  
  
  rescaler->SetInput( duplicator->GetOutput() );
  writer->SetInput( rescaler->GetOutput() );
  writer->Update();
  

  return EXIT_SUCCESS;
}

