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


#include <fstream>
#include <iostream>

#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbAttributesMapLabelObject.h"
#include "itkLabelImageToLabelMapFilter.h"
#include "otbBandsStatisticsAttributesLabelMapFilter.h"

const unsigned int Dimension = 2;
typedef unsigned short LabelType;
typedef double         PixelType;

typedef otb::AttributesMapLabelObject<LabelType, Dimension, double>                LabelObjectType;
typedef itk::LabelMap<LabelObjectType>                                             LabelMapType;
typedef otb::VectorImage<PixelType, Dimension>                                     VectorImageType;
typedef otb::Image<unsigned int, 2>                                                LabeledImageType;

typedef LabelMapType::Iterator                                                     IteratorType;

typedef otb::ImageFileReader<VectorImageType>                                      ReaderType;
typedef otb::ImageFileReader<LabeledImageType>                                     LabeledReaderType;

typedef itk::LabelImageToLabelMapFilter<LabeledImageType, LabelMapType>             LabelMapFilterType;
typedef otb::BandsStatisticsAttributesLabelMapFilter<LabelMapType, VectorImageType> BandsStatisticsFilterType;


int otbBandsStatisticsAttributesLabelMapFilterNew(int argc, char* argv[])
{
  BandsStatisticsFilterType::Pointer object = BandsStatisticsFilterType::New();
  return EXIT_SUCCESS;
}

int otbBandsStatisticsAttributesLabelMapFilter(int argc, char* argv[])
{
  const char * infname  = argv[1];
  const char * lfname   = argv[2];
  const char * outfname = argv[3];

  // Filters instanciation
  ReaderType::Pointer                reader              = ReaderType::New();
  LabeledReaderType::Pointer         labeledReader       = LabeledReaderType::New();
  LabelMapFilterType::Pointer        filter              = LabelMapFilterType::New();
  BandsStatisticsFilterType::Pointer stats               = BandsStatisticsFilterType::New();

  // Read inputs
  reader->SetFileName(infname);
  labeledReader->SetFileName(lfname);

  // Make a LabelMap out of it
  filter->SetInput(labeledReader->GetOutput());
  filter->SetBackgroundValue(itk::NumericTraits<LabelType>::max());

  //Compute band statistics attributes
  stats->SetInput(filter->GetOutput());
  stats->SetFeatureImage(reader->GetOutput());

  stats->Update();

  LabelMapType::Pointer labelMap = stats->GetOutput();

  // Dump all results in the output file
  std::ofstream outfile(outfname);
  IteratorType it = IteratorType( labelMap );

  while ( !it.IsAtEnd() )
    {
    LabelType label = it.GetLabel();
    LabelObjectType::Pointer labelObject = it.GetLabelObject();

    outfile << "Label " << label << " : " << std::endl;

    std::vector<std::string> attributes = labelObject->GetAvailableAttributes();
    std::vector<std::string>::const_iterator attrIt = attributes.begin();
    std::vector<std::string>::const_iterator attrEnd = attributes.end();
    for (; attrIt != attrEnd; ++attrIt)
      {
      LabelObjectType::AttributesValueType value = labelObject->GetAttribute(attrIt->c_str());
      outfile << "  " << *attrIt << " : "<< std::fixed << std::setprecision(6) << value << std::endl;
      }
    outfile << std::endl;
    ++it;
    }

  return EXIT_SUCCESS;
}

