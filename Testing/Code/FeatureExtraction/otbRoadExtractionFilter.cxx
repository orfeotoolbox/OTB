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

#include "itkPolyLineParametricPath.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbRoadExtractionFilter.h"
#include "otbDrawPathListFilter.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int otbRoadExtractionFilter(int argc, char * argv[])
{
  try
    {
      const unsigned int Dimension = 2;
      typedef otb::VectorImage<double,Dimension> InputImageType;
      typedef otb::Image<unsigned char,Dimension> OutputImageType;
      typedef itk::PolyLineParametricPath<Dimension> PathType;
      
      typedef otb::ImageFileReader<InputImageType> ReaderType;
      typedef otb::ImageFileWriter<OutputImageType> WriterType;
      typedef otb::RoadExtractionFilter<InputImageType,PathType> RoadExtractionFilterType;
      typedef RoadExtractionFilterType::OutputPathListType OutputPathListType;
      typedef RoadExtractionFilterType::InputPixelType InputPixelType;
      typedef otb::DrawPathListFilter<OutputImageType, PathType, OutputImageType> DrawPathFilterType;
	  int cpt = 0;
      while ( argv[cpt] != NULL)
      {
   			std::cout << cpt <<": "<<argv[cpt]<<std::endl;
   			cpt++; 	
      }
	  //Parameters
	  const char * inputFileName(argv[1]);
	  InputPixelType ReferencePixel;
	  ReferencePixel.Reserve(2);
	  ReferencePixel.SetElement(0,::atof(argv[2]));
	  ReferencePixel.SetElement(1,::atof(argv[3]));
      const double Sigma = ::atof(argv[4]);
      const double AmplitudeThreshold = ::atof(argv[5]);
      const double Tolerance = ::atof(argv[6]);
      const double MaxAngle = ((vcl_acos(-1.)*::atof(argv[7]))/180.);
      const double MeanDistanceThreshold  = ::atof(argv[8]);
	  const char * outputFileName(argv[9]);
	  
      // Instantiating object
      ReaderType::Pointer reader = ReaderType::New();
      RoadExtractionFilterType::Pointer roadExtraction = RoadExtractionFilterType::New();
      DrawPathFilterType::Pointer draw = DrawPathFilterType::New();
      WriterType::Pointer writer = WriterType::New();

      //Initialisation parameters
      reader->SetFileName(inputFileName);
      
      roadExtraction->SetInput(reader->GetOutput());
      roadExtraction->SetReferencePixel(ReferencePixel);
      roadExtraction->SetSigma(Sigma);
      roadExtraction->SetAmplitudeThreshold(AmplitudeThreshold);
      roadExtraction->SetTolerance(Tolerance);
      roadExtraction->SetMaxAngle(MaxAngle);
      roadExtraction->SetMeanDistanceThreshold(MeanDistanceThreshold);

	  std::cout << " RoadExtraction instance value: " << roadExtraction <<std::endl; 
      //Provisoire pour debug.....
//      roadExtraction->Update();
//	  std::cout << " RoadExtraction after update instance value: " << roadExtraction <<std::endl; 

      reader->GenerateOutputInformation();
      std::cout << reader->GetOutput()->GetLargestPossibleRegion();

/*      OutputImageType::SizeType size;
      size[0] = sizex;
      size[1] = sizey;
      OutputImageType::IndexType index;
      index.Fill(0);
      OutputImageType::RegionType region;
      region.SetSize(size);
      region.SetIndex(index);*/
      OutputImageType::Pointer image = OutputImageType::New();
      image->SetRegions(reader->GetOutput()->GetLargestPossibleRegion());
      image->Allocate();
      image->FillBuffer(0);

	  OutputPathListType * listPath = roadExtraction->GetOutput();
	  std::cout << "LIstPath : Size() = "<<listPath->Size()<<std::endl;

      draw->SetInput(image);
      draw->SetPathValue(255);
      draw->SetInputPath(roadExtraction->GetOutput());

      writer->SetFileName(outputFileName);
      writer->SetInput(draw->GetOutput());
      writer->Update();
      
    }

  catch( itk::ExceptionObject & err ) 
    { 
    std::cout << "Exception itk::ExceptionObject thrown !" << std::endl; 
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
    std::cout << "Unknown exception thrown !" << std::endl; 
    return EXIT_FAILURE;
    } 
  return EXIT_SUCCESS;
}
