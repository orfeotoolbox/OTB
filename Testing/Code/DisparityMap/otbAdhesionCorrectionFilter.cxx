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
#include "otbAdhesionCorrectionFilter.h"
#include "otbImage.h"
#include "otbStreamingImageFileWriter.h"
#include "otbStandardWriterWatcher.h"
#include "otbImageFileReader.h"

 const unsigned int Dimension = 2;
  typedef float                                                         PixelType;
  typedef otb::Image<PixelType, Dimension>                              ImageType;
  typedef float                                                         MaskPixelType;
  typedef otb::Image<MaskPixelType, Dimension>                          MaskType;


  typedef otb::ImageFileReader<ImageType>               ReaderType;
  typedef otb::ImageFileReader<MaskType>                MaskReaderType;

  typedef otb::StreamingImageFileWriter<ImageType>                      WriterType;
  typedef otb::StreamingImageFileWriter<MaskType>                       MaskWriterType;

  typedef otb::AdhesionCorrectionFilter<ImageType, MaskType> AdhesionCorrectionFilterType;

int otbAdhesionCorrectionFilterNew(int argc, char* argv[])
{

  // Instantiation
  AdhesionCorrectionFilterType::Pointer filter = AdhesionCorrectionFilterType::New();

   return EXIT_SUCCESS;
}

int otbAdhesionCorrectionFilter(int argc, char* argv[])
{
  if ((argc != 15))
    {
    std::cerr << "Usage: " << argv[0];
    std::cerr << " imageinput_fname meddispinput_fname medmaskinput_fname edgedispinput_fname  subpixmaskinput_fname"
        << std::endl;
    std::cerr << " ouputmap_filename  outputmask_filename outputriskedges_filename ";
    std::cerr << " radius tolerance discontinuitythres discontinuityhighthres edgethres maxedgegap ";
    return EXIT_FAILURE;
    }
  AdhesionCorrectionFilterType::Pointer filter = AdhesionCorrectionFilterType::New();

  ReaderType::Pointer inReader = ReaderType::New();
  inReader->SetFileName(argv[1]);

  ReaderType::Pointer medianDispReader = ReaderType::New();
    medianDispReader->SetFileName(argv[2]);

  MaskReaderType::Pointer medianMaskReader = MaskReaderType::New();
  medianMaskReader->SetFileName(argv[3]);


  ReaderType::Pointer edgeDispReader = ReaderType::New();
  edgeDispReader->SetFileName(argv[4]);

  MaskReaderType::Pointer subPixelMaskReader = MaskReaderType::New();
  subPixelMaskReader->SetFileName(argv[5]);

  filter->SetInput(inReader->GetOutput());
  filter->SetMedianDisparityInput(medianDispReader->GetOutput());
  filter->SetMedianMaskInput(medianMaskReader->GetOutput());
  filter->SetEdgesDisparityInput(edgeDispReader->GetOutput());
  filter->SetSubPixelMaskInput(subPixelMaskReader->GetOutput());

  filter->SetRadius(atoi(argv[9]));
  filter->SetTolerance(atof(argv[10]));
  filter->SetDiscontinuityThreshold(atof(argv[11]));
  filter->SetDiscontinuityHighThreshold(atof(argv[12]));
  filter->SetEdgeThreshold(atof(argv[13]));
  filter->SetMaxEdgeGap(atof(argv[14]));


WriterType  ::Pointer dispWriter = WriterType::New();
  dispWriter->SetInput(filter->GetOutput());
  dispWriter->SetFileName(argv[6]);
  otb::StandardWriterWatcher watcher1(dispWriter, filter, "Computing disparity ");

  dispWriter->Update();

  WriterType::Pointer maskWriter = WriterType::New();
  maskWriter->SetInput(filter->GetOutputMask());
  maskWriter->SetFileName(argv[7]);

  maskWriter->Update();

  WriterType::Pointer edgeWriter = WriterType::New();
  edgeWriter->SetInput(filter->GetOutputRiskEdges());
  edgeWriter->SetFileName(argv[8]);

  edgeWriter->Update();

  return EXIT_SUCCESS;
}


