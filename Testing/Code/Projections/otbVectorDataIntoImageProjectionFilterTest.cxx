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

#include "itkExceptionObject.h"
#include "otbMacro.h"

#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbVectorData.h"
#include "otbVectorDataFileReader.h"
#include "otbVectorDataFileWriter.h"

#include "otbVectorDataIntoImageProjectionFilter.h"

int otbVectorDataIntoImageProjectionFilterTest(int argc, char * argv[])
{
  typedef float                                           PixelType;
  typedef otb::VectorImage<PixelType, 2>                  VectorImageType;
  typedef otb::Image<PixelType, 2>                        ImageType;
  typedef otb::ImageFileReader<VectorImageType>           ReaderType;

  // Vector Data
  typedef otb::VectorData<>                               VectorDataType;
  typedef otb::VectorDataFileReader<VectorDataType>       VectorDataReaderType;
  typedef otb::VectorDataFileWriter<VectorDataType>       VectorDataWriterType;

  //
  typedef otb::VectorDataIntoImageProjectionFilter
                 <VectorDataType, VectorImageType>        VectorDataReProjFilter;

  typedef itk::PreOrderTreeIterator<VectorDataType::DataTreeType> TreeIteratorType;

  std::string imageInputFilename = argv[1];
  std::string vectorDataInputFilename = argv[2];
  std::string demDirectory = argv[3];
  std::string vectorDataOutputFilename = argv[4];
  std::string txtOutputFilename = argv[5];

  std::cout << imageInputFilename << "\n"
            << vectorDataInputFilename << "\n"
            << demDirectory << "\n"
            << vectorDataOutputFilename << "\n"
            << txtOutputFilename << std::endl;

  // Read the image
  ReaderType::Pointer    reader  = ReaderType::New();
  reader->SetFileName(imageInputFilename);
  reader->UpdateOutputInformation();

  // read the Vector Data
  VectorDataReaderType::Pointer vdReader = VectorDataReaderType::New();
  vdReader->SetFileName(vectorDataInputFilename);
  vdReader->Update();

  std::cout<<"Set input data to read data DONE"<<std::endl;

  std::ofstream file;
  file.open(txtOutputFilename.c_str());

  file << "--- IMAGE INPUT ---" << std::endl;
  file << "Spacing of the input image: "<< reader->GetOutput()->GetSpacing() << std::endl;
  file << "Origin of the input image: "<< reader->GetOutput()->GetOrigin() << std::endl;
  file << "Size of the input image: "<< reader->GetOutput()->GetLargestPossibleRegion() << std::endl;
  file << "ProjRef of the input image: "<< reader->GetOutput()->GetProjectionRef() << std::endl;


  VectorDataReProjFilter::Pointer vdReProjFilter = VectorDataReProjFilter::New();

  //----------
  // Display input
  file << "\n--- VECTOR DATA INPUT ---" << std::endl;
  file << "ProjRef of the input vector data: "<< vdReader->GetOutput()->GetProjectionRef() << std::endl;
  file << "Origin of the input vector data: "<< vdReader->GetOutput()->GetOrigin() << std::endl;
  file << "Spacing of the input vector data: "<< vdReader->GetOutput()->GetSpacing() << std::endl;

  //----------
  // Set input of the filter
  vdReProjFilter->SetInputImage(reader->GetOutput());

  vdReProjFilter->SetInputVectorData(vdReader->GetOutput());

  vdReProjFilter->SetDEMDirectory(demDirectory);

  std::string stateOutput ="";
  if (atoi(argv[6]) == 1)
   {
   stateOutput = "true";
   vdReProjFilter->SetUseOutputSpacingAndOriginFromImage(true);
   }
  else
   {
   stateOutput = "false";
   vdReProjFilter->SetUseOutputSpacingAndOriginFromImage(false);
   }

  std::cout<<"Set input data for the filter DONE (" << stateOutput << ")" <<std::endl;

  //----------
  // WRITE
  //vdReProjFilter->Update();

  VectorDataWriterType::Pointer vdwriter = VectorDataWriterType::New();
  vdwriter->SetFileName(vectorDataOutputFilename);
  vdwriter->SetInput(vdReProjFilter->GetOutput());
  vdwriter->Update();

  std::cout<<"Update"<<std::endl;

  //----------
  // check output
  file << "\n--- VECTRODATA OUTPUT ---" << std::endl;
  file << "ProjRef of the output vector data: "<< vdReProjFilter->GetOutput()->GetProjectionRef() << std::endl;
  file << "Origin of the output vector data: "<< vdReProjFilter->GetOutput()->GetOrigin() << std::endl;
  file << "Spacing of the output vector data: "<< vdReProjFilter->GetOutput()->GetSpacing() << std::endl;

  file.close();

  std::cout<<"END"<<std::endl;


  return EXIT_SUCCESS;
}
