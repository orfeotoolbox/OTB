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


#include "otbSpectralAngleDataNodeFeatureFunction.h"

#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbVectorData.h"
#include "otbVectorDataFileReader.h"
#include "otbVectorDataIntoImageProjectionFilter.h"
#include "otbVectorDataFileWriter.h"
#include "itkPreOrderTreeIterator.h"

int otbSpectralAngleDataNodeFeatureFunctionNew(int itkNotUsed(argc), char** itkNotUsed(argv))
{
  typedef double CoordRepType;
  typedef double PrecisionType;
  typedef otb::VectorImage<double, 2>  ImageType;
  typedef otb::SpectralAngleDataNodeFeatureFunction<ImageType, CoordRepType, PrecisionType>
                                       DataNodeFeaturefunctionType;

  DataNodeFeaturefunctionType::Pointer featureFunction = DataNodeFeaturefunctionType::New();

  std::cout << featureFunction << std::endl;

  return EXIT_SUCCESS;
}

int otbSpectralAngleDataNodeFeatureFunction(int itkNotUsed(argc), char* argv[])
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

  typedef otb::SpectralAngleDataNodeFeatureFunction<ImageType, CoordRepType, PrecisionType>
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
  featureFunction->SetRadius(0);

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
          currentGeometry->SetFieldAsDouble("RADIOM", (double)(currentResult[0]));
          outVD->GetDataTree()->Add(currentGeometry, folder);
         }
    ++itVector;
    }

  vdWriter->SetInput(outVD);
  vdWriter->SetFileName(outputVD);
  vdWriter->Update();

  return EXIT_SUCCESS;
}
