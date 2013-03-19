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
#include "otbStandardImageViewer.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbVectorData.h"
#include "otbVectorDataFileReader.h"
#include "itkTimeProbe.h"
#include "otbSplittedWidgetManager.h"

int otbStandardImageViewer(int argc, char * argv[])
{
  bool run = atoi(argv[2]);

  typedef otb::VectorImage<double, 2>               ImageType;
  typedef otb::ImageFileReader<ImageType>           ReaderType;
  typedef otb::StandardImageViewer<ImageType>       ViewerType;
  typedef otb::VectorData<double>                   VectorDataType;
  typedef otb::VectorDataFileReader<VectorDataType> VectorDataFileReaderType;

  VectorDataFileReaderType::Pointer vreader = VectorDataFileReaderType::New();
  ReaderType::Pointer               reader = ReaderType::New();
  ViewerType::Pointer               viewer = ViewerType::New();
  VectorDataFileReaderType::Pointer vdreader = VectorDataFileReaderType::New();

  reader->SetFileName(argv[1]);
  viewer->SetImage(reader->GetOutput());

  if (argc > 3)
    {
    std::cout << "Adding a vector layer from file " << argv[3] << std::endl;
    itk::TimeProbe chrono;
    chrono.Start();
    vdreader->SetFileName(argv[3]);
    vdreader->Update();
    chrono.Stop();
    std::cout << "VectorData loaded in " << chrono.GetMeanTime() << " s." << std::endl;
    viewer->SetVectorData(vdreader->GetOutput());
    }

  if (argc > 4)
    {
    std::cout << "Reprojecting using DEM " << argv[4] << std::endl;
    otb::DEMHandler::Instance()->OpenDEMDirectory(argv[4]);
    }

  viewer->SetLabel("Testing standard viewer");

  viewer->Update();

  if (run)
    {
    Fl::run();
    }
  else
    {
    Fl::check();
    }

  return EXIT_SUCCESS;
}


int otbStandardImageViewerSplitted(int argc, char * argv[])
{
  bool run = atoi(argv[2]);

  typedef otb::VectorImage<double, 2>                                                 ImageType;
  typedef otb::ImageFileReader<ImageType>                                             ReaderType;
  typedef otb::VectorData<double>                                                     VectorDataType;
  typedef otb::StandardImageViewer<ImageType, VectorDataType, otb::SplittedWidgetManager> ViewerType;
  typedef otb::VectorDataFileReader<VectorDataType>                                   VectorDataFileReaderType;


  VectorDataFileReaderType::Pointer vreader = VectorDataFileReaderType::New();
  ReaderType::Pointer               reader = ReaderType::New();
  ViewerType::Pointer               viewer = ViewerType::New();
  VectorDataFileReaderType::Pointer vdreader = VectorDataFileReaderType::New();

  reader->SetFileName(argv[1]);
  viewer->SetImage(reader->GetOutput());

  if (argc > 3)
    {
    std::cout << "Adding a vector layer from file " << argv[3] << std::endl;
    itk::TimeProbe chrono;
    chrono.Start();
    vdreader->SetFileName(argv[3]);
    vdreader->Update();
    chrono.Stop();
    std::cout << "VectorData loaded in " << chrono.GetMeanTime() << " s." << std::endl;
    viewer->SetVectorData(vdreader->GetOutput());
    }

  if (argc > 4)
    {
    std::cout << "Reprojecting using DEM " << argv[4] << std::endl;
    otb::DEMHandler::Instance()->OpenDEMDirectory(argv[4]);
    }

  viewer->SetLabel("Testing standard viewer");

  viewer->Update();

  if (run)
    {
    Fl::run();
    }
  else
    {
    Fl::check();
    }

  return EXIT_SUCCESS;
}
