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

#include "otbVectorImage.h"
#include "otbImage.h"
#include "otbImageFileWriter.h"
#include "otbVectorDataFileReader.h"

#include "otbVectorDataToLabelMapWithAttributesFilter.h"
#include "otbSailModel.h"
#include "otbImageSimulationMethod.h"
#include "otbAttributesMapLabelObject.h"
#include "otbImageFileReader.h"

int otbImageSimulationMethodWithVectorDataTest(int itkNotUsed(argc), char * argv[])
{
   const char * infilename  = argv[1];
   const char * satRSRFilename = argv[2];
   unsigned int nbBands = static_cast<unsigned int>(atoi(argv[3]));
   const char * rootPath = argv[4];
   unsigned int radius = atoi(argv[5]);
   const char * outfilename = argv[6];
   const char * outLabelfilename = argv[7];


   typedef unsigned short LabelType;
   const unsigned int Dimension = 2;
   typedef otb::Image<LabelType, Dimension>                                           LabelImageType;
   typedef otb::VectorImage<double, Dimension>                                        OutputImageType;
   typedef otb::ImageFileWriter<OutputImageType>                                      ImageWriterType;
   typedef otb::ImageFileWriter<LabelImageType>                                       LabelImageWriterType;
   typedef otb::VectorData<double, Dimension>                                         VectorDataType;
   typedef otb::AttributesMapLabelObject<LabelType, Dimension, std::string>           LabelObjectType;
   typedef itk::LabelMap<LabelObjectType>                                             LabelMapType;
   typedef otb::VectorDataFileReader<VectorDataType>                                  VectorDataFileReaderType;

   typedef otb::VectorDataToLabelMapWithAttributesFilter<VectorDataType, LabelMapType> SpatialisationFilterType;
   typedef otb::ProspectModel                                                         SimulationStep1Type;
   typedef otb::SailModel                                                             SimulationStep2Type;
   typedef otb::ProlateInterpolateImageFunction<LabelImageType>                       FTMType;
   typedef otb::ImageSimulationMethod<VectorDataType, SpatialisationFilterType,
    SimulationStep1Type, SimulationStep2Type, FTMType , OutputImageType>               ImageSimulationMethodType;


   /** Instantiation of pointer objects*/
   VectorDataFileReaderType::Pointer reader = VectorDataFileReaderType::New();
   ImageWriterType::Pointer writer = ImageWriterType::New();
   LabelImageWriterType::Pointer labelWriter = LabelImageWriterType::New();
   ImageSimulationMethodType::Pointer imageSimulation = ImageSimulationMethodType::New();
   SpatialisationFilterType::Pointer spatialisationFilter = SpatialisationFilterType::New();


   reader->SetFileName(infilename);
   reader->Update();


   spatialisationFilter->SetInput(reader->GetOutput());
   spatialisationFilter->SetInitialLabel(1);
   spatialisationFilter->SetBackgroundValue(0);

   imageSimulation->SetSpatialisation(spatialisationFilter);
   imageSimulation->SetNumberOfComponentsPerPixel(nbBands);
   imageSimulation->SetSatRSRFilename(satRSRFilename);
   imageSimulation->SetPathRoot(rootPath);
   imageSimulation->SetRadius(radius);
//    imageSimulation->SetMean();
//    imageSimulation->SetVariance();
   imageSimulation->UpdateData();


   //Write the result to an image file
   writer->SetFileName(outfilename);
   writer->SetInput(imageSimulation->GetOutputReflectanceImage());
   writer->Update();

   labelWriter->SetFileName(outLabelfilename);
   labelWriter->SetInput(imageSimulation->GetOutputLabelImage());
   labelWriter->Update();


   return EXIT_SUCCESS;
}
