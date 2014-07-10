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


#include "otbNDVIDataNodeFeatureFunction.h"

#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbVectorData.h"
#include "otbVectorDataFileReader.h"
#include "otbVectorDataIntoImageProjectionFilter.h"
#include "otbVectorDataFileWriter.h"
#include "itkPreOrderTreeIterator.h"

int otbNDVIDataNodeFeatureFunctionNew(int itkNotUsed(argc), char** itkNotUsed(argv))
{
  typedef double CoordRepType;
  typedef double PrecisionType;
  typedef otb::VectorImage<double, 2>  ImageType;
  typedef otb::NDVIDataNodeFeatureFunction<ImageType, CoordRepType, PrecisionType>
                                       NDVIDataNodeFeaturefunctionType;

  NDVIDataNodeFeaturefunctionType::Pointer NDVIFeature = NDVIDataNodeFeaturefunctionType::New();

  std::cout << NDVIFeature << std::endl;

  return EXIT_SUCCESS;
}

int otbNDVIDataNodeFeatureFunction(int itkNotUsed(argc), char* argv[])
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

  typedef otb::NDVIDataNodeFeatureFunction<ImageType, CoordRepType, PrecisionType>
                                         NDVIDataNodeFeaturefunctionType;
  typedef NDVIDataNodeFeaturefunctionType::OutputType
                                                      NDVIFeatureOutputType;

  ImageReaderType::Pointer imgReader = ImageReaderType::New();
  VectorDataReaderType::Pointer vdReader = VectorDataReaderType::New();
  VectorDataReProjFilter::Pointer vdReProjFilter = VectorDataReProjFilter::New();
  VectorDataWriterType::Pointer vdWriter = VectorDataWriterType::New();
  NDVIDataNodeFeaturefunctionType::Pointer NDVIFeatureFunction = NDVIDataNodeFeaturefunctionType::New();

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
/*
  std::cout<< "vdReProjFilter->GetOutput()->Size(): "
        << vdReProjFilter->GetOutput()->Size() << std::endl;
*/
  NDVIFeatureFunction->SetREDChannelIndex(3);
  NDVIFeatureFunction->SetNIRChannelIndex(4);
  NDVIFeatureFunction->SetNDVIThreshold(0.047);
  NDVIFeatureFunction->SetInputImage(imgReader->GetOutput());

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
          NDVIFeatureOutputType currentResult;
          currentResult = NDVIFeatureFunction->Evaluate(*(currentGeometry.GetPointer()));
          currentGeometry->SetFieldAsDouble("NDVI", (double)(currentResult[0]));
          outVD->GetDataTree()->Add(currentGeometry, folder);
         }
    ++itVector;
    }

  vdWriter->SetInput(outVD);
  vdWriter->SetFileName(outputVD);
  vdWriter->Update();

  return EXIT_SUCCESS;
}
