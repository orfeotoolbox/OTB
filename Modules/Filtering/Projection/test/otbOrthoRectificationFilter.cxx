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


// iostream is used for general output
#include <iostream>
#include <stdlib.h>
#include <complex>

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbMapProjections.h"
#include "itkUnaryFunctorImageFilter.h"
//#include "itkComplexToModulusImageFilter.h"
#include "otbDEMHandler.h"
#include "otbUnaryImageFunctorWithVectorImageFilter.h"
#include "otbOrthoRectificationFilter.h"
#include "otbMapProjections.h"
#include "otbComplexToIntensityImageFilter.h"
#include "otbPerBandVectorImageFilter.h"

int otbOrthoRectificationFilter(int argc, char* argv[])
{
  if (argc != 15)
    {
    std::cout << argv[0] <<
    " <input filename> <output filename> <origin easting> <origin northing>"
    " <x size> <y size> <x spacing> <y spacing> <UTM zone> <UTM hemisphere>"
    " <grid_spacing> <mode> <mode.info> <is_complex>"
              << std::endl;

    return EXIT_FAILURE;
    }

  typedef std::complex<double>                                                      ComplexPixelType;
  typedef otb::VectorImage<ComplexPixelType,2>                                     ComplexVectorImageType;
  typedef otb::VectorImage<double, 2>                                               VectorImageType;
  typedef otb::ImageFileReader<VectorImageType>                                     ReaderType;
  typedef otb::ImageFileReader<ComplexVectorImageType>                              ComplexReaderType;
  typedef otb::ImageFileWriter<VectorImageType>                                     WriterType;
  typedef otb::UtmInverseProjection                                                 UtmMapProjectionType;

  // Handling of complex images
  typedef otb::Image<ComplexPixelType> ComplexImageType;
  typedef otb::Image<double>           ImageType;
  typedef otb::ComplexToIntensityImageFilter<ComplexImageType, ImageType> IntensityFilterType;
  typedef otb::PerBandVectorImageFilter<ComplexVectorImageType,VectorImageType,IntensityFilterType> PerBandIntensityFilterType;
  
  typedef otb::OrthoRectificationFilter<VectorImageType, VectorImageType, UtmMapProjectionType> OrthoRectifFilterType;

  //Allocate pointer
  ReaderType::Pointer reader = ReaderType::New();
  ComplexReaderType::Pointer cReader = ComplexReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  PerBandIntensityFilterType::Pointer intensityFilter = PerBandIntensityFilterType::New();
  
  OrthoRectifFilterType::Pointer orthoRectifFilter = OrthoRectifFilterType::New();
  UtmMapProjectionType::Pointer  utmMapProjection = UtmMapProjectionType::New();

  writer->SetFileName(argv[2]);
  
  bool isComplex = atoi(argv[14]);

  if(isComplex)
    {
      cReader->SetFileName(argv[1]);
      cReader->GenerateOutputInformation();
      intensityFilter->SetInput(cReader->GetOutput());
      VectorImageType::PixelType no_data(cReader->GetOutput()->GetNumberOfComponentsPerPixel());
      no_data.Fill(0);
      orthoRectifFilter->SetEdgePaddingValue(no_data);
      orthoRectifFilter->SetInput(intensityFilter->GetOutput());
    }
  else
    {
      reader->SetFileName(argv[1]);
      reader->GenerateOutputInformation();
      VectorImageType::PixelType no_data(reader->GetOutput()->GetNumberOfComponentsPerPixel());
      no_data.Fill(0);
      orthoRectifFilter->SetEdgePaddingValue(no_data);
      orthoRectifFilter->SetInput(reader->GetOutput());
    }

  VectorImageType::IndexType start;
  start[0] = 0;
  start[1] = 0;
  orthoRectifFilter->SetOutputStartIndex(start);

  VectorImageType::SizeType size;
  size[0] = atoi(argv[5]);      // X size
  size[1] = atoi(argv[6]);            //Y size
  orthoRectifFilter->SetOutputSize(size);

  VectorImageType::SpacingType spacing;
  spacing[0] = atof(argv[7]);
  spacing[1] = atof(argv[8]);
  orthoRectifFilter->SetOutputSpacing(spacing);

  VectorImageType::PointType origin;
  origin[0] = strtod(argv[3], ITK_NULLPTR);         //Origin easting
  origin[1] = strtod(argv[4], ITK_NULLPTR);         //Origin northing
  orthoRectifFilter->SetOutputOrigin(origin);

  utmMapProjection->SetZone(atoi(argv[9]));
  utmMapProjection->SetHemisphere(argv[10][0]);
  orthoRectifFilter->SetMapProjection(utmMapProjection);

  // Displacement Field spacing
  VectorImageType::SpacingType  gridSpacing;
  gridSpacing[0] = atof(argv[11]);
  gridSpacing[1] = -atof(argv[11]);
  orthoRectifFilter->SetDisplacementFieldSpacing(gridSpacing);
  
  // manage demHandler
  if (atoi(argv[12])==1) //mode = no DEM
  {
	  otb::DEMHandler::Instance()->SetDefaultHeightAboveEllipsoid(135.8);
  }
  else if ( (atoi(argv[12])==2) || (atoi(argv[12])==3) ) //mode = DEM SRTM || DEM GTIFF
  {
	  otb::DEMHandler::Instance()->OpenDEMDirectory(argv[13]);
  }

  writer->SetInput(orthoRectifFilter->GetOutput());
  writer->SetNumberOfDivisionsTiledStreaming(4);
  writer->Update();

  return EXIT_SUCCESS;
}
