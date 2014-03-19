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

#include "otbLuminanceToImageImageFilter.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkVariableLengthVector.h"

int otbLuminanceToImageImageFilter(int argc, char * argv[])
{
  const char * inputFileName  = argv[1];
  const char * outputFileName = argv[2];

  const unsigned int Dimension = 2;
  typedef double                                                            PixelType;
  typedef otb::VectorImage<PixelType, Dimension>                            InputImageType;
  typedef otb::VectorImage<PixelType, Dimension>                            OutputImageType;
  typedef otb::ImageFileReader<InputImageType>                              ReaderType;
  typedef otb::ImageFileWriter<OutputImageType>                             WriterType;
  typedef otb::LuminanceToImageImageFilter<InputImageType, OutputImageType> LuminanceToImageImageFilterType;
  typedef LuminanceToImageImageFilterType::VectorType                       VectorType;

  ReaderType::Pointer reader  = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  reader->SetFileName(inputFileName);
  writer->SetFileName(outputFileName);
  reader->UpdateOutputInformation();

  unsigned int nbOfComponent = reader->GetOutput()->GetNumberOfComponentsPerPixel();

  VectorType alpha(nbOfComponent);
  VectorType beta(nbOfComponent);
  alpha.Fill(0);
  beta.Fill(0);

  for (unsigned int i = 0; i < nbOfComponent; ++i)
    {
    alpha[i] = static_cast<double>(atof(argv[i + 3]));
    beta[i] = static_cast<double>(atof(argv[i + 7]));
    }

  // Instantiating object
  LuminanceToImageImageFilterType::Pointer filter = LuminanceToImageImageFilterType::New();
  filter->SetAlpha(alpha);
  filter->SetBeta(beta);
  filter->SetInput(reader->GetOutput());
  writer->SetInput(filter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
