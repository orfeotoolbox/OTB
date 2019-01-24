/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include "otbImageFileReader.h"

#include <fstream>
#include <iostream>

#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbAttributesMapLabelObject.h"
#include "otbImageToLabelMapWithAttributesFilter.h"
#include "otbMinMaxAttributesLabelMapFilter.h"
#include "otbNormalizeAttributesLabelMapFilter.h"

const unsigned int Dimension = 2;
typedef unsigned short LabelType;
typedef double         PixelType;

typedef otb::AttributesMapLabelObject<LabelType, Dimension, double>             LabelObjectType;
typedef otb::LabelMapWithAdjacency<LabelObjectType>                             LabelMapType;
typedef otb::VectorImage<PixelType, Dimension>                                  VectorImageType;
typedef otb::Image<unsigned int, 2>                                              LabeledImageType;

typedef otb::ImageFileReader<VectorImageType>                                   ReaderType;
typedef otb::ImageFileReader<LabeledImageType>                                  LabeledReaderType;
typedef otb::LabelImageToLabelMapWithAdjacencyFilter<LabeledImageType, LabelMapType> LabelMapFilterType;
typedef otb::ShapeAttributesLabelMapFilter<LabelMapType>                        ShapeFilterType;
typedef otb::MinMaxAttributesLabelMapFilter<LabelMapType>                       MinMaxAttributesLabelMapFilterType;
typedef otb::NormalizeAttributesLabelMapFilter<LabelMapType>                    NormalizeAttributesLabelMapFilterType;


int otbNormalizeAttributesLabelMapFilter(int itkNotUsed(argc), char * argv[])
{
  const char * infname  = argv[1];
  const char * lfname   = argv[2];
  const char * outfname = argv[3];

  // SmartPointer instantiation
  ReaderType::Pointer         reader = ReaderType::New();
  LabeledReaderType::Pointer  labeledReader = LabeledReaderType::New();
  LabelMapFilterType::Pointer filter = LabelMapFilterType::New();
  ShapeFilterType::Pointer    shapeFilter = ShapeFilterType::New();
  MinMaxAttributesLabelMapFilterType::Pointer minmaxLabelMapFilter = MinMaxAttributesLabelMapFilterType::New();
  NormalizeAttributesLabelMapFilterType::Pointer normalizeLabelMapFilter = NormalizeAttributesLabelMapFilterType::New();
  MinMaxAttributesLabelMapFilterType::Pointer minmaxAfterLabelMapFilter = MinMaxAttributesLabelMapFilterType::New();

  // Inputs
  reader->SetFileName(infname);
  reader->UpdateOutputInformation();
  labeledReader->SetFileName(lfname);
  labeledReader->UpdateOutputInformation();

  // Filter
  filter->SetInput(labeledReader->GetOutput());
  filter->SetBackgroundValue(itk::NumericTraits<LabelType>::max());

  shapeFilter->SetInput(filter->GetOutput());

  minmaxLabelMapFilter->SetInput(shapeFilter->GetOutput());
  minmaxLabelMapFilter->Update();

  std::ofstream outfile(outfname);

  {
  MinMaxAttributesLabelMapFilterType::AttributesMapType minimum = minmaxLabelMapFilter->GetMinimum();
  MinMaxAttributesLabelMapFilterType::AttributesMapType::const_iterator it;
  outfile<< "Minimum before normalization : " << std::endl;
  for (it = minimum.begin(); it != minimum.end(); ++it)
    {
    outfile << "  " << (*it).first << " : " << std::fixed << std::setprecision(6) << (*it).second << std::endl;
    }
  outfile << std::endl;

  MinMaxAttributesLabelMapFilterType::AttributesMapType maximum = minmaxLabelMapFilter->GetMaximum();
  outfile << "Maximum before normalization : " << std::endl;
  for (it = maximum.begin(); it != maximum.end(); ++it)
    {
    outfile << "  " << (*it).first << " : " << std::fixed << std::setprecision(6) << (*it).second << std::endl;
    }
  outfile << std::endl;
  }

  normalizeLabelMapFilter->SetInput(shapeFilter->GetOutput());
  normalizeLabelMapFilter->SetMinAttributesValues(minmaxLabelMapFilter->GetMinimum());
  normalizeLabelMapFilter->SetMaxAttributesValues(minmaxLabelMapFilter->GetMaximum());
  normalizeLabelMapFilter->Update();

  minmaxAfterLabelMapFilter->SetInput(normalizeLabelMapFilter->GetOutput());
  minmaxAfterLabelMapFilter->Update();

  {
  MinMaxAttributesLabelMapFilterType::AttributesMapType minimum = minmaxAfterLabelMapFilter->GetMinimum();
  MinMaxAttributesLabelMapFilterType::AttributesMapType::const_iterator it;
  outfile << "Minimum after normalization : " << std::endl;
  for (it = minimum.begin(); it != minimum.end(); ++it)
    {
    outfile << "  " << (*it).first << " : " << std::fixed << std::setprecision(6) << (*it).second << std::endl;
    }
  outfile << std::endl;

  MinMaxAttributesLabelMapFilterType::AttributesMapType maximum = minmaxAfterLabelMapFilter->GetMaximum();
  outfile << "Maximum after normalization : " << std::endl;
  for (it = maximum.begin(); it != maximum.end(); ++it)
    {
    outfile << "  " << (*it).first << " : " << std::fixed << std::setprecision(6) << (*it).second << std::endl;
    }
  outfile << std::endl;
  }
  outfile.close();

  return EXIT_SUCCESS;
}
