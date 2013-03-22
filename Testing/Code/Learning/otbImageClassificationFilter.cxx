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
#include "otbImageClassificationFilter.h"
#include "otbVectorImage.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbMachineLearningModelFactory.h"

const unsigned int Dimension = 2;
typedef double PixelType;
typedef unsigned short LabeledPixelType;

typedef otb::VectorImage<PixelType, Dimension> ImageType;
typedef otb::Image<LabeledPixelType, Dimension> LabeledImageType;
typedef otb::ImageClassificationFilter<ImageType, LabeledImageType> ClassificationFilterType;
typedef ClassificationFilterType::ModelType ModelType;
typedef ClassificationFilterType::ValueType ValueType;
typedef ClassificationFilterType::LabelType LabelType;
typedef otb::MachineLearningModelFactory<ValueType, LabelType> MachineLearningModelFactoryType;
typedef otb::ImageFileReader<ImageType> ReaderType;
typedef otb::ImageFileWriter<LabeledImageType> WriterType;

int otbImageClassificationFilterNew(int argc, char * argv[])
{
  ClassificationFilterType::Pointer filter = ClassificationFilterType::New();
  return EXIT_SUCCESS;
}

int otbImageClassificationFilter(int argc, char * argv[])
{
  const char * infname = argv[1];
  const char * modelfname = argv[2];
  const char * outfname = argv[3];

  // Instantiating object
  ClassificationFilterType::Pointer filter = ClassificationFilterType::New();

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);

  ModelType::Pointer model;

  model = MachineLearningModelFactoryType::CreateMachineLearningModel(modelfname,
                                                                      MachineLearningModelFactoryType::ReadMode);

  model->Load(modelfname);

  filter->SetModel(model);
  filter->SetInput(reader->GetOutput());

  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(filter->GetOutput());
  writer->SetFileName(outfname);
  writer->Update();

  return EXIT_SUCCESS;
}
