/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#include "otbRadiometryHomogenousWithNeighborhoodDataNodeFeatureFunction.h"

#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbVectorData.h"
#include "otbVectorDataFileReader.h"
#include "otbVectorDataIntoImageProjectionFilter.h"
#include "otbVectorDataFileWriter.h"
#include "itkPreOrderTreeIterator.h"


int otbRadiometryHomogenousWithNeighborhoodDataNodeFeatureFunction(int itkNotUsed(argc), char* argv[])
{
  const char * inputVD  = argv[1];
  const char * inputImg = argv[2];
  const char * DEMDir   = argv[3];
  const char * outputVD = argv[4];
  int DisplayWarnings   = atoi(argv[5]);

  typedef double                                      CoordRepType;
  typedef double                                      PrecisionType;
  typedef otb::VectorImage<PrecisionType>             ImageType;
  typedef otb::ImageFileReader<ImageType>             ImageReaderType;
  typedef otb::VectorData<CoordRepType, 2, PrecisionType>
                                                      VectorDataType;
  typedef VectorDataType::DataNodeType                DataNodeType;
  typedef otb::VectorDataFileReader<VectorDataType>   VectorDataReaderType;
  typedef otb::VectorDataIntoImageProjectionFilter<VectorDataType, ImageType>
                                                      VectorDataReProjFilter;
  typedef otb::VectorDataFileWriter<VectorDataType>   VectorDataWriterType;
  typedef itk::PreOrderTreeIterator<VectorDataType::DataTreeType>
                                                      TreeIteratorType;

  typedef otb::RadiometryHomogenousWithNeighborhoodDataNodeFeatureFunction<ImageType, CoordRepType, PrecisionType>
                                                      FeaturefunctionType;
  typedef FeaturefunctionType::OutputType
                                                      FeatureOutputType;

  ImageReaderType::Pointer imgReader = ImageReaderType::New();
  VectorDataReaderType::Pointer vdReader = VectorDataReaderType::New();
  VectorDataReProjFilter::Pointer vdReProjFilter = VectorDataReProjFilter::New();
  VectorDataWriterType::Pointer vdWriter = VectorDataWriterType::New();
  FeaturefunctionType::Pointer featureFunction = FeaturefunctionType::New();

  if (!DisplayWarnings)
   {
    imgReader->SetGlobalWarningDisplay(0);
   }

  otb::DEMHandler::Instance()->OpenDEMDirectory(DEMDir);

  vdReader->SetFileName(inputVD);
  vdReader->Update();

  imgReader->SetFileName(inputImg);
  imgReader->UpdateOutputInformation();
  imgReader->Update(); //Needed to set m_EndIndex, m_StartIndex in otbDataNodeImageFunction

  vdReProjFilter->SetInputImage(imgReader->GetOutput());
  vdReProjFilter->SetInputVectorData(vdReader->GetOutput());
  vdReProjFilter->SetUseOutputSpacingAndOriginFromImage(true);
  vdReProjFilter->Update();

  std::cout<< "vdReProjFilter->GetOutput()->Size(): "
        << vdReProjFilter->GetOutput()->Size() << std::endl;

  featureFunction->SetInputImage(imgReader->GetOutput());

  // Output
  VectorDataType::Pointer outVD = VectorDataType::New();
  // Retrieving root node
  DataNodeType::Pointer root = outVD->GetDataTree()->GetRoot()->Get();
  // Create the document node
  DataNodeType::Pointer document = DataNodeType::New();
  document->SetNodeType(otb::DOCUMENT);
  // Adding the layer to the data tree
  outVD->GetDataTree()->Add(document, root);
  // Create the folder node
  DataNodeType::Pointer folder = DataNodeType::New();
  folder->SetNodeType(otb::FOLDER);
  // Adding the layer to the data tree
  outVD->GetDataTree()->Add(folder, document);

  TreeIteratorType itVector(vdReProjFilter->GetOutput()->GetDataTree());
  itVector.GoToBegin();
  while (!itVector.IsAtEnd())
    {
    if (itVector.Get()->IsLineFeature() || itVector.Get()->IsPolygonFeature())
         {
          const DataNodeType::Pointer currentGeometry = itVector.Get();
          FeatureOutputType currentResult;
          currentResult = featureFunction->Evaluate(*(currentGeometry.GetPointer()));
          currentGeometry->SetFieldAsDouble("RADIOM_HOM", (double)(currentResult[0]));
          outVD->GetDataTree()->Add(currentGeometry, folder);
         }
    ++itVector;
    }

  vdWriter->SetInput(outVD);
  vdWriter->SetFileName(outputVD);
  vdWriter->Update();

  return EXIT_SUCCESS;
}
