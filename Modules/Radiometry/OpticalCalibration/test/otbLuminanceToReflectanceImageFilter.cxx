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
#include "itkMacro.h"

#include "otbLuminanceToReflectanceImageFilter.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int otbLuminanceToReflectanceImageFilter(int argc, char * argv[])
{
  const char * inputFileName  = argv[1];
  const char * outputFileName = argv[2];
  const double angle = static_cast<double>(atof(argv[3]));
  double       flux = 0.;
  int          day = 1;
  int          month = 1;

  if (argc == 9)
    {
    flux = static_cast<double>(atof(argv[8]));
    }
  else
    {
    day = atoi(argv[8]);
    month = atoi(argv[9]);
    }

  const unsigned int Dimension = 2;
  typedef double                                                                  PixelType;
  typedef otb::VectorImage<PixelType, Dimension>                                  InputImageType;
  typedef otb::VectorImage<PixelType, Dimension>                                  OutputImageType;
  typedef otb::ImageFileReader<InputImageType>                                    ReaderType;
  typedef otb::ImageFileWriter<OutputImageType>                                   WriterType;
  typedef otb::LuminanceToReflectanceImageFilter<InputImageType, OutputImageType> LuminanceToReflectanceImageFilterType;
  typedef LuminanceToReflectanceImageFilterType::VectorType                       VectorType;

  ReaderType::Pointer reader  = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  reader->SetFileName(inputFileName);
  writer->SetFileName(outputFileName);
  reader->UpdateOutputInformation();

  unsigned int nbOfComponent = reader->GetOutput()->GetNumberOfComponentsPerPixel();
  // WARNING : 1 ELEMENT FOR EACH PIXEL IMAGE COMPONENT
  VectorType solarIllumination(nbOfComponent);
  solarIllumination.Fill(0);
  solarIllumination[0] = static_cast<double>(atof(argv[4]));
  solarIllumination[1] = static_cast<double>(atof(argv[5]));
  solarIllumination[2] = static_cast<double>(atof(argv[6]));
  solarIllumination[3] = static_cast<double>(atof(argv[7]));

  // Instantiating object
  LuminanceToReflectanceImageFilterType::Pointer filter = LuminanceToReflectanceImageFilterType::New();

  filter->SetZenithalSolarAngle(angle);
  filter->SetSolarIllumination(solarIllumination);
  filter->SetUseClamp(false);
  if (argc == 9)
    {
    filter->SetFluxNormalizationCoefficient(flux);
    }
  else
    {
    filter->SetDay(day);
    filter->SetMonth(month);
    }

  filter->SetInput(reader->GetOutput());
  writer->SetInput(filter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
