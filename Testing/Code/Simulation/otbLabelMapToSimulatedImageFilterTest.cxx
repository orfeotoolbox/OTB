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
#include "otbVectorData.h"
#include "otbVectorDataFileReader.h"
#include "otbVectorDataToLabelMapWithAttributesFilter.h"
#include "otbAttributesMapLabelObject.h"
#include "otbVectorImage.h"
#include "otbImageFileWriter.h"
#include "otbLabelMapToSimulatedImageFilter.h"
#include "otbProspectModel.h"
#include "otbSailModel.h"

int otbLabelMapToSimulatedImageFilterTest(int argc, char * argv[])
{
   const char * infilename  = argv[1];
   const char * satRSRFilename = argv[2];
   unsigned int nbBands = static_cast<unsigned int>(atoi(argv[3]));
   const char * rootPath = argv[4];
   const char * outfilename = argv[5];

   typedef unsigned short LabelType;
   const unsigned int Dimension = 2;
   typedef otb::VectorImage<double, Dimension>                                         LabeledImageType;
   typedef otb::ImageFileWriter<LabeledImageType>                                         WriterType;
   typedef otb::VectorData<double, Dimension>                                             VectorDataType;
   typedef otb::AttributesMapLabelObject<LabelType, Dimension, std::string>               LabelObjectType;
   typedef itk::LabelMap<LabelObjectType>                                                 LabelMapType;
   typedef otb::VectorDataFileReader<VectorDataType>                                      VectorDataFileReaderType;
   typedef otb::VectorDataToLabelMapWithAttributesFilter<VectorDataType, LabelMapType>    VectorDataToLabelMapFilterType;

   typedef otb::ProspectModel   ProspectType;
   typedef otb::SailModel         SailType;
   typedef otb::LabelMapToSimulatedImageFilter<LabelMapType, ProspectType, SailType, LabeledImageType>            LabelMapToSimulatedImageFilterType;


   /** Instantiation of pointer objects*/
   VectorDataFileReaderType::Pointer reader = VectorDataFileReaderType::New();
   VectorDataToLabelMapFilterType::Pointer vectorDataToLabelMap = VectorDataToLabelMapFilterType::New();
   LabelMapToSimulatedImageFilterType::Pointer labelMapToImageFilter = LabelMapToSimulatedImageFilterType::New();
   WriterType::Pointer writer = WriterType::New();


   reader->SetFileName(infilename);
   reader->Update();

   vectorDataToLabelMap->SetInput(reader->GetOutput());
   vectorDataToLabelMap->SetInitialLabel(1);
   vectorDataToLabelMap->SetBackgroundValue(0);

   labelMapToImageFilter->SetInput(vectorDataToLabelMap->GetOutput());
   labelMapToImageFilter->SetNumberOfComponentsPerPixel(nbBands);
   labelMapToImageFilter->SetSatRSRFilename(satRSRFilename);
   labelMapToImageFilter->SetNumberOfThreads(1); // must be set to 1
   labelMapToImageFilter->SetPathRoot(rootPath);

   //Write the result to an image file
   writer->SetFileName(outfilename);
   writer->SetInput(labelMapToImageFilter->GetOutput());
   writer->Update();

   return EXIT_SUCCESS;
}
