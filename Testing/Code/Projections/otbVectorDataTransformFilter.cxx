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

#include "otbVectorDataTransformFilter.h"
#include "itkAffineTransform.h"

#include "otbVectorImage.h"
#include "otbVectorData.h"
#include "otbImageFileReader.h"
#include "otbVectorDataFileReader.h"
#include "otbVectorDataFileWriter.h"

#include "otbVectorDataProjectionFilter.h"

// common typedefs
typedef otb::VectorData<>                      VectorDataType;
typedef otb::VectorDataTransformFilter
        <VectorDataType, VectorDataType>       VectorDataTransformType;

int otbVectorDataTransformFilterNew (int argc, char * argv[])
{
  VectorDataTransformType::Pointer transformFilter = VectorDataTransformType::New();
  return EXIT_SUCCESS;
}

int otbVectorDataTransformFilter (int argc, char * argv[])
{
  typedef otb::VectorImage<double, 2>             ImageType;
  typedef otb::VectorData<>                      VectorDataType;

  typedef otb::ImageFileReader<ImageType>             ReaderType;
  typedef otb::VectorDataFileReader<VectorDataType>   VectorDataFileReaderType;
  typedef otb::VectorDataFileWriter<VectorDataType>   VectorDataFileWriterType;

  typedef otb::VectorDataProjectionFilter<VectorDataType,
    VectorDataType>                                   VDProjectionFilterType;

  // Instanciate the image reader
  ReaderType::Pointer      reader = ReaderType::New();
  reader->SetFileName(argv[1]);
  reader->UpdateOutputInformation();

  VectorDataFileReaderType::Pointer  vdreader = VectorDataFileReaderType::New();
  vdreader->SetFileName(argv[2]);
  vdreader->Update();

  // Reproject the VectorData In the image coordinate system
  VDProjectionFilterType::Pointer  vdproj = VDProjectionFilterType::New();
  vdproj->SetInput(vdreader->GetOutput());
  vdproj->SetInputProjectionRef(vdreader->GetOutput()->GetProjectionRef());
  vdproj->SetOutputKeywordList(reader->GetOutput()->GetImageKeywordlist());
  vdproj->SetOutputProjectionRef(reader->GetOutput()->GetProjectionRef());

  // Test the translation using the ApplyTransformTo
  typedef itk::AffineTransform<double, 2>           TransformType;
  typedef TransformType::OutputVectorType          TranslationParamType;

  // Set up the transform (Apply a translation of 8 pixels in the y direction)
  TransformType::Pointer transform = TransformType::New();
  TranslationParamType   tranlationParam;
  tranlationParam[0] = 0;
  tranlationParam[1] = 8. * reader->GetOutput()->GetSpacing()[1];
  transform->SetTranslation(tranlationParam);

  VectorDataTransformType::Pointer transformFilter = VectorDataTransformType::New();
  transformFilter->SetInput(vdproj->GetOutput());
  transformFilter->SetTransform(transform);

  // retransform int the input vector projection
  VDProjectionFilterType::Pointer  reverseVdProj = VDProjectionFilterType::New();
  reverseVdProj->SetInput(transformFilter->GetOutput());
  reverseVdProj->SetOutputProjectionRef(vdreader->GetOutput()->GetProjectionRef());
  reverseVdProj->SetInputKeywordList(reader->GetOutput()->GetImageKeywordlist());
  reverseVdProj->SetInputProjectionRef(reader->GetOutput()->GetProjectionRef());

  //Write the vectordata
  VectorDataFileWriterType::Pointer vdwriter = VectorDataFileWriterType::New();
  vdwriter->SetInput(reverseVdProj->GetOutput());
  vdwriter->SetFileName(argv[3]);
  vdwriter->Update();

  return EXIT_SUCCESS;
}
