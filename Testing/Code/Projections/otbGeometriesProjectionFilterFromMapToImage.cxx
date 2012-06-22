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

/*===========================================================================*/
/*===============================[ Includes ]================================*/
/*===========================================================================*/
#include "otbGeometriesProjectionFilter.h"
#include "otbGeometriesSet.h"
#include "otbImage.h"
#include "otbImageFileReader.h"

/*===========================================================================*/
/*==============================[ other stuff ]==============================*/
/*===========================================================================*/
int otbGeometriesProjectionFilterFromMapToImage(int argc, char * argv[])
{
  if (argc < 4)
    {
    std::cerr << argv[0] << " <input vector filename> <input image filename>"
              << " <output vector filename>\n";
    return EXIT_FAILURE;
    }

  // Input Geometries set
  typedef otb::GeometriesSet                InputGeometriesType;
  typedef otb::GeometriesSet                OutputGeometriesType;
  otb::ogr::DataSource::Pointer input = otb::ogr::DataSource::New(
    argv[1], otb::ogr::DataSource::Modes::read);
  InputGeometriesType::Pointer in_set = InputGeometriesType::New(input);

  // Input Keywordlist (from image)
  typedef otb::Image<unsigned short int, 2> ImageType;
  typedef otb::ImageFileReader<ImageType>   ImageReaderType;
  ImageReaderType::Pointer imageReader = ImageReaderType::New();
  imageReader->SetFileName(argv[2]);
  imageReader->UpdateOutputInformation();

  // Output Geometries Set
  otb::ogr::DataSource::Pointer output = otb::ogr::DataSource::New(
    argv[3], otb::ogr::DataSource::Modes::write);
  OutputGeometriesType::Pointer out_set = OutputGeometriesType::New(output);

  // Filter
  typedef otb::GeometriesProjectionFilter   GeometriesFilterType;
  GeometriesFilterType::Pointer filter = GeometriesFilterType::New();
  filter->SetInput(in_set);
  filter->SetOutput(out_set);
  filter->SetOutputProjectionRef(imageReader->GetOutput()->GetProjectionRef());
  filter->SetOutputOrigin(imageReader->GetOutput()->GetOrigin());
  filter->SetOutputSpacing(imageReader->GetOutput()->GetSpacing());

  filter->Update();

  return EXIT_SUCCESS;
}
