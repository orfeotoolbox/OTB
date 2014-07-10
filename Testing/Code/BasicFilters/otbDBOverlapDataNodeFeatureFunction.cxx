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


#include "otbDBOverlapDataNodeFeatureFunction.h"

#include "otbVectorData.h"
#include "otbVectorDataFileReader.h"
#include "otbVectorDataFileWriter.h"
#include "itkPreOrderTreeIterator.h"

int otbDBOverlapDataNodeFeatureFunctionNew(int itkNotUsed(argc), char** itkNotUsed(argv))
{
  typedef double CoordRepType;
  typedef double PrecisionType;
  typedef otb::DBOverlapDataNodeFeatureFunction<CoordRepType, PrecisionType>
                                       DataNodeFeaturefunctionType;

  DataNodeFeaturefunctionType::Pointer DBOverlapFeature = DataNodeFeaturefunctionType::New();

  std::cout << DBOverlapFeature<< std::endl;

  return EXIT_SUCCESS;
}

int otbDBOverlapDataNodeFeatureFunction(int itkNotUsed(argc), char* argv[])
{
  const char * inputVD  = argv[1];
  const char * inputDB  = argv[2];
  const char * outputVD = argv[3];
  int DisplayWarnings   = atoi(argv[4]);

  typedef double                                      CoordRepType;
  typedef double                                      PrecisionType;
  typedef otb::VectorData<CoordRepType, 2, PrecisionType>
                                                      VectorDataType;
  typedef VectorDataType::DataNodeType                DataNodeType;
  typedef otb::VectorDataFileReader<VectorDataType>   VectorDataReaderType;
  typedef otb::VectorDataFileWriter<VectorDataType>   VectorDataWriterType;
  typedef itk::PreOrderTreeIterator<VectorDataType::DataTreeType>
                                                      TreeIteratorType;

  typedef otb::DBOverlapDataNodeFeatureFunction<CoordRepType, PrecisionType>
                                                      DataNodeFunctionType;
  typedef DataNodeFunctionType::OutputType
                                                      FeatureOutputType;

  VectorDataReaderType::Pointer   vdReader = VectorDataReaderType::New();
  VectorDataReaderType::Pointer vdDBReader = VectorDataReaderType::New();
  VectorDataWriterType::Pointer   vdWriter = VectorDataWriterType::New();
  DataNodeFunctionType::Pointer   function = DataNodeFunctionType::New();

  if (!DisplayWarnings)
   {
    vdReader->SetGlobalWarningDisplay(0);
   }

  vdReader->SetFileName(inputVD);
  vdReader->Update();

  vdDBReader->SetFileName(inputDB);
  vdDBReader->Update();

  function->SetInputVectorData(vdDBReader->GetOutput());
  function->SetDistanceThreshold(0.003);

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

  TreeIteratorType itVector(vdReader->GetOutput()->GetDataTree());
  itVector.GoToBegin();
  while (!itVector.IsAtEnd())
    {
    if (itVector.Get()->IsLineFeature() || itVector.Get()->IsPolygonFeature())
         {
          const DataNodeType::Pointer currentGeometry = itVector.Get();
          FeatureOutputType currentResult;
          currentResult = function->Evaluate(*(currentGeometry.GetPointer()));
          currentGeometry->SetFieldAsDouble("DBOVER", (double)(currentResult[0]));
          outVD->GetDataTree()->Add(currentGeometry, folder);
         }
    ++itVector;
    }

  vdWriter->SetInput(outVD);
  vdWriter->SetFileName(outputVD);
  vdWriter->Update();

  return EXIT_SUCCESS;
}
