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

#include "otbImageFileReader.h"
#include "otbVectorDataFileWriter.h"
#include "otbVectorData.h"
#include "otbVectorDataProjectionFilter.h"

#include <fstream>
#include <iostream>

#include "otbImage.h"
#include "otbLabelMapToVectorDataFilter.h"
#include "otbAttributesMapLabelObject.h"
#include "itkLabelImageToLabelMapFilter.h"

int otbLabelMapToVectorDataFilter(int argc, char * argv[])
{
  /** Use the labelObjecttopolygon functor (not thread safe) only polygon conversion is available yet*/
  if (argc != 3)
    {
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputImageFile outputVectorfile(shp)" << std::endl;
    return EXIT_FAILURE;
    }
  const char * infname = argv[1];
  const char * outfname = argv[2];

  // Labeled image type
  const unsigned int Dimension                 = 2;
  typedef unsigned short                         LabelType;
  typedef otb::Image<LabelType, Dimension>       LabeledImageType;
  typedef otb::ImageFileReader<LabeledImageType> LabeledReaderType;

  // Label map typedef
  typedef otb::AttributesMapLabelObject<LabelType, Dimension, double>             LabelObjectType;
  typedef itk::LabelMap<LabelObjectType>                                          LabelMapType;
  typedef itk::LabelImageToLabelMapFilter<LabeledImageType, LabelMapType>         LabelMapFilterType;
  typedef otb::VectorData<double, 2>                                              VectorDataType;
  typedef otb::VectorDataFileWriter<VectorDataType>                               WriterType;
  typedef otb::VectorDataProjectionFilter<VectorDataType, VectorDataType>         VectorDataFilterType;

  LabeledReaderType::Pointer lreader = LabeledReaderType::New();
  lreader->SetFileName(infname);

  LabelMapFilterType::Pointer labelMapFilter = LabelMapFilterType::New();
  labelMapFilter->SetInput(lreader->GetOutput());
  labelMapFilter->SetBackgroundValue(itk::NumericTraits<LabelType>::min());
  labelMapFilter->Update();

  WriterType::Pointer writer = WriterType::New();

  typedef otb::LabelMapToVectorDataFilter<LabelMapType, VectorDataType> LabelMapToVectorDataFilterType;

  LabelMapToVectorDataFilterType::Pointer MyFilter = LabelMapToVectorDataFilterType::New();

  MyFilter->SetInput(labelMapFilter->GetOutput());
  MyFilter->Update();

  MyFilter->GetOutput()->SetProjectionRef(lreader->GetOutput()->GetProjectionRef());

  VectorDataFilterType::Pointer vectorDataProjection = VectorDataFilterType::New();
  vectorDataProjection->SetInputOrigin(lreader->GetOutput()->GetOrigin());
  vectorDataProjection->SetInputSpacing(lreader->GetOutput()->GetSpacing());
  vectorDataProjection->SetInput(MyFilter->GetOutput());

  writer->SetFileName(outfname);
  writer->SetInput(vectorDataProjection->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;

}
