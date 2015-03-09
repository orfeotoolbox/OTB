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

#include "otbReflectanceToImageImageFilter.h"

//#include "otbReflectanceToLuminanceImageFilter.h"
//#include "otbLuminanceToImageImageFilter.h"

#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkVariableLengthVector.h"

int otbReflectanceToImageImageFilter(int argc, char * argv[])
{
  const char * inputFileName  = argv[1];
  const char * outputFileName = argv[2];
  const double angle = static_cast<double>(atof(argv[3]));
  double       flux = 0.;
  int          day = 1;
  int          month = 1;

  if (argc == 17)
    {
    flux = static_cast<double>(atof(argv[16]));
    }
  else
    {
    day = atoi(argv[16]);
    month = atoi(argv[17]);
    }

  const unsigned int Dimension = 2;
  typedef double                                                              PixelType;
  typedef otb::VectorImage<PixelType, Dimension>                              InputImageType;
  typedef otb::VectorImage<PixelType, Dimension>                              OutputImageType;
  typedef otb::ImageFileReader<InputImageType>                                ReaderType;
  typedef otb::ImageFileWriter<OutputImageType>                               WriterType;
  typedef otb::ReflectanceToImageImageFilter<InputImageType, OutputImageType> ReflectanceToImageImageFilterType;
  typedef ReflectanceToImageImageFilterType::VectorType                       VectorType;

  ReaderType::Pointer reader  = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  reader->SetFileName(inputFileName);
  writer->SetFileName(outputFileName);
  reader->UpdateOutputInformation();

  unsigned int nbOfComponent = reader->GetOutput()->GetNumberOfComponentsPerPixel();

  VectorType alpha(nbOfComponent);
  VectorType beta(nbOfComponent);
  VectorType solarIllumination(nbOfComponent);
  alpha.Fill(0);
  beta.Fill(0);
  solarIllumination.Fill(0);

  for (unsigned int i = 0; i < nbOfComponent; ++i)
    {
    alpha[i] = static_cast<double>(atof(argv[i + 4]));
    beta[i] = static_cast<double>(atof(argv[i + 8]));
    solarIllumination[i] = static_cast<double>(atof(argv[i + 12]));
    }

  // Instantiating object
  ReflectanceToImageImageFilterType::Pointer filter = ReflectanceToImageImageFilterType::New();

  filter->SetAlpha(alpha);
  filter->SetBeta(beta);
  filter->SetZenithalSolarAngle(angle);
  filter->SetSolarIllumination(solarIllumination);

  if (argc == 17)
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


  /*const char * inputFileName  = argv[1];
  const char * outputFileName = argv[2];
  const double angle = static_cast<double>(atof(argv[3]));
  double       flux = 0.;
  int          day = 1;
  int          month = 1;

  if (argc == 17)
    {
    flux = static_cast<double>(atof(argv[16]));
    }
  else
    {
    day = atoi(argv[16]);
    month = atoi(argv[17]);
    }

  const unsigned int Dimension = 2;
  typedef double                                                              PixelType;
  typedef otb::VectorImage<PixelType, Dimension>                              InputImageType;
  typedef otb::VectorImage<PixelType, Dimension>                              OutputImageType;
  typedef otb::ImageFileReader<InputImageType>                                ReaderType;
  typedef otb::ImageFileWriter<OutputImageType>                               WriterType;

typedef otb::ReflectanceToLuminanceImageFilter<InputImageType, InputImageType> ReflectanceToLuminanceImageFilterType;
typedef otb::LuminanceToImageImageFilter<InputImageType, OutputImageType> LuminanceToImageImageFilterType;

  typedef ReflectanceToLuminanceImageFilterType::VectorType                       VectorType;

  ReaderType::Pointer reader  = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  reader->SetFileName(inputFileName);
  writer->SetFileName(outputFileName);
  reader->UpdateOutputInformation();

  unsigned int nbOfComponent = reader->GetOutput()->GetNumberOfComponentsPerPixel();

  VectorType alpha(nbOfComponent);
  VectorType beta(nbOfComponent);
  VectorType solarIllumination(nbOfComponent);
  alpha.Fill(0);
  beta.Fill(0);
  solarIllumination.Fill(0);

  for (unsigned int i = 0; i < nbOfComponent; ++i)
    {
    alpha[i] = static_cast<double>(atof(argv[i + 4]));
    beta[i] = static_cast<double>(atof(argv[i + 8]));
    solarIllumination[i] = static_cast<double>(atof(argv[i + 12]));
    }

  // Instantiating object

ReflectanceToLuminanceImageFilterType::Pointer filter = ReflectanceToLuminanceImageFilterType::New();
LuminanceToImageImageFilterType::Pointer filter2 = LuminanceToImageImageFilterType::New();

  filter2->SetAlpha(alpha);
  filter2->SetBeta(beta);
  filter->SetZenithalSolarAngle(angle);
  filter->SetSolarIllumination(solarIllumination);

  if (argc == 17)
    {
    filter->SetFluxNormalizationCoefficient(flux);
    }
  else
    {
    filter->SetDay(day);
    filter->SetMonth(month);
    }

  filter->SetInput(reader->GetOutput());
  filter2->SetInput(filter->GetOutput());
  writer->SetInput(filter2->GetOutput());
  writer->Update();

  return EXIT_SUCCESS; */

}
