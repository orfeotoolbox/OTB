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
#include "otbDisparityMapToDEMFilter.h"
#include "otbImageFileWriter.h"
#include "otbStandardWriterWatcher.h"
#include "otbImageFileReader.h"

#include "otbImageList.h"
#include "otbVectorImageToImageListFilter.h"




  const unsigned int Dimension = 2;
  typedef float                                                        PixelType;
  typedef otb::Image<PixelType, Dimension>                             FloatImageType;
  typedef unsigned char                                                MaskPixelType;
  typedef otb::Image<MaskPixelType, Dimension>                         MaskImageType;
  typedef otb::VectorImage<PixelType, Dimension>                       FloatVectorImageType;

  typedef otb::ImageList<FloatImageType>                  ImageListType;
  typedef otb::VectorImageToImageListFilter
    <FloatVectorImageType, ImageListType>                 VectorImageToListFilterType;


  typedef otb::ImageFileReader<FloatVectorImageType>            ReaderType;
  typedef otb::ImageFileReader<MaskImageType>                   MaskReaderType;
  typedef otb::ImageFileWriter  <FloatImageType>       WriterType;

  typedef otb::DisparityMapToDEMFilter <FloatImageType, FloatVectorImageType>      DisparityToElevationFilterType;

int otbDisparityMapToDEMFilterNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{

  // Instantiation
  DisparityToElevationFilterType::Pointer filter = DisparityToElevationFilterType::New();

   return EXIT_SUCCESS;
}

int otbDisparityMapToDEMFilter(int argc, char* argv[])
{
  if ((argc != 11) && (argc != 12))
    {
    std::cerr << "Usage: " << argv[0];
    std::cerr
        << " dispinput_fname sensorleftinput_fname sensorrighttinput_fname gridleftinput_fname gridrightinput_fname"
        << std::endl;
    std::cerr << " outputDEM_filename " << std::endl;
    std::cerr << " elevmin elevmax avgelev DEMgridstep (maskinput_fname) " << std::endl;
    return EXIT_FAILURE;
    }

  DisparityToElevationFilterType::Pointer filter = DisparityToElevationFilterType::New();
  VectorImageToListFilterType::Pointer vectorToList = VectorImageToListFilterType::New();

  ReaderType::Pointer dispReader = ReaderType::New();
  dispReader->SetFileName(argv[1]);
  vectorToList->SetInput(dispReader->GetOutput());
  vectorToList->UpdateOutputInformation();

  ReaderType::Pointer sensorLeftReader = ReaderType::New();
  sensorLeftReader->SetFileName(argv[2]);
  ReaderType::Pointer sensorRightReader = ReaderType::New();
  sensorRightReader->SetFileName(argv[3]);
  ReaderType::Pointer gridLeftReader = ReaderType::New();
  gridLeftReader->SetFileName(argv[4]);
  ReaderType::Pointer gridRightReader = ReaderType::New();
  gridRightReader->SetFileName(argv[5]);

  filter->SetHorizontalDisparityMapInput(vectorToList->GetOutput()->GetNthElement(0));
  filter->SetVerticalDisparityMapInput(vectorToList->GetOutput()->GetNthElement(1));
  filter->SetLeftInput(sensorLeftReader->GetOutput());
  filter->SetRightInput(sensorRightReader->GetOutput());
  filter->SetLeftEpipolarGridInput(gridLeftReader->GetOutput());
  filter->SetRightEpipolarGridInput(gridRightReader->GetOutput());

  filter->SetElevationMin(atof(argv[7]));
  filter->SetElevationMax(atof(argv[8]));
  filter->SetDEMGridStep(atof(argv[10]));

  otb::DEMHandler::Instance()->SetDefaultHeightAboveEllipsoid(atof(argv[9]));

  MaskReaderType::Pointer maskReader;
  if (argc == 12)
    {
    maskReader = MaskReaderType::New();
    maskReader->SetFileName(argv[11]);
    filter->SetDisparityMaskInput(maskReader->GetOutput());
    }

  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(filter->GetOutput());
  writer->SetFileName(argv[6]);
  otb::StandardWriterWatcher watcher(writer, filter, "disparity to DEM ");

  writer->Update();

  return EXIT_SUCCESS;
}
