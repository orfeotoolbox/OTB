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


#include "otbMacro.h"

#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbVectorData.h"
#include "otbVectorDataFileReader.h"
#include "otbVectorDataFileWriter.h"

#include "otbVectorDataIntoImageProjectionFilter.h"

#include "otbVectorDataExtractROI.h"
#include "otbVectorDataProjectionFilter.h"

/*
* Tests prTvVectorDataIntoImageProjectionFilterTest_2 and
* prTvVectorDataIntoImageProjectionFilterTest_4 failed because of the bug http://bugs.orfeo-toolbox.org/view.php?id=333.
*/

int otbVectorDataIntoImageProjectionFilterTest(int itkNotUsed(argc), char * argv[])
{
  typedef float                                           PixelType;
  typedef otb::VectorImage<PixelType, 2>                  VectorImageType;
  typedef otb::ImageFileReader<VectorImageType>           ReaderType;

  // Vector Data
  typedef otb::VectorData<>                               VectorDataType;
  typedef otb::VectorDataFileReader<VectorDataType>       VectorDataReaderType;

  typedef otb::VectorDataIntoImageProjectionFilter
                 <VectorDataType, VectorImageType>        VectorDataIntoImageProjectionFilterType;

  std::string imageInputFilename = argv[1];
  std::string vectorDataInputFilename = argv[2];
  std::string demDirectory = argv[3];
  int expectedFeatures = atoi(argv[4]);

  otb::DEMHandler::Instance()->OpenDEMDirectory(demDirectory);

  // Read the image
  ReaderType::Pointer    reader  = ReaderType::New();
  reader->SetFileName(imageInputFilename);
  reader->UpdateOutputInformation();

  // Read the Vector Data
  VectorDataReaderType::Pointer vdReader = VectorDataReaderType::New();
  vdReader->SetFileName(vectorDataInputFilename);
  vdReader->Update();

  VectorDataIntoImageProjectionFilterType::Pointer vdReProjFilter
    = VectorDataIntoImageProjectionFilterType::New();
  vdReProjFilter->SetInputImage(reader->GetOutput());
  vdReProjFilter->SetInputVectorData(vdReader->GetOutput());
  vdReProjFilter->Update();

  int nbElem = 0;
  typedef itk::PreOrderTreeIterator<VectorDataType::DataTreeType> TreeIteratorType;
  TreeIteratorType itVector(vdReProjFilter->GetOutput()->GetDataTree());
  itVector.GoToBegin();
  while (!itVector.IsAtEnd())
    {
    if (   !itVector.Get()->IsRoot()
        && !itVector.Get()->IsFolder()
        && !itVector.Get()->IsDocument())
      {
      nbElem++;
      }
    ++itVector;
    }

  if (nbElem != expectedFeatures)
    {
    std::cerr << "Kept " << nbElem << " features, but expected " << expectedFeatures << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}



int otbVectorDataIntoImageProjectionFilterCompareImplTest(int itkNotUsed(argc), char * argv[])
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

  typedef otb::VectorDataProjectionFilter
  <VectorDataType, VectorDataType>                        VectorDataProjectionFilterType;
  typedef otb::VectorDataExtractROI<VectorDataType>       VectorDataExtractROIType;
  typedef VectorDataExtractROIType::RegionType            RemoteSensingRegionType;

  std::string imageInputFilename = argv[1];
  std::string vectorDataInputFilename = argv[2];
  std::string demDirectory = argv[3];
  std::string vectorDataOutputFilename = argv[4];
  std::string vectorDataOutputFilename2 = argv[5];

  std::cout << imageInputFilename << "\n"
            << vectorDataInputFilename << "\n"
            << demDirectory << "\n"
            << vectorDataOutputFilename << "\n"
            << vectorDataOutputFilename2 << std::endl;

  if (!demDirectory.empty())
    {
    otb::DEMHandler::Instance()->OpenDEMDirectory(demDirectory);
    }

  // Read the image
  ReaderType::Pointer    reader  = ReaderType::New();
  reader->SetFileName(imageInputFilename);
  reader->UpdateOutputInformation();

  // read the Vector Data
  VectorDataReaderType::Pointer vdReader = VectorDataReaderType::New();
  vdReader->SetFileName(vectorDataInputFilename);
  vdReader->Update();

  VectorDataReProjFilter::Pointer vdReProjFilter = VectorDataReProjFilter::New();
  vdReProjFilter->SetInputImage(reader->GetOutput());
  vdReProjFilter->SetInputVectorData(vdReader->GetOutput());

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

  //----------
  // WRITE
  //vdReProjFilter->Update();

  VectorDataWriterType::Pointer vdwriter = VectorDataWriterType::New();
  vdwriter->SetFileName(vectorDataOutputFilename);
  vdwriter->SetInput(vdReProjFilter->GetOutput());
  vdwriter->Update();

  //-------
  // do the same with old code

  VectorDataProjectionFilterType::Pointer vproj;
  VectorDataExtractROIType::Pointer vdextract;

      // Extract The part of the VectorData that actually overlaps with
  // the image extent
  vdextract = VectorDataExtractROIType::New();
  vdextract->SetInput(vdReader->GetOutput());

  // Find the geographic region of interest

  // Ge the index of the corner of the image
  ImageType::IndexType ul, ur, ll, lr;
  ImageType::PointType pul, pur, pll, plr;
  ul = reader->GetOutput()->GetLargestPossibleRegion().GetIndex();
  ur = ul;
  ll = ul;
  lr = ul;
  ur[0] += reader->GetOutput()->GetLargestPossibleRegion().GetSize()[0];
  lr[0] += reader->GetOutput()->GetLargestPossibleRegion().GetSize()[0];
  lr[1] += reader->GetOutput()->GetLargestPossibleRegion().GetSize()[1];
  ll[1] += reader->GetOutput()->GetLargestPossibleRegion().GetSize()[1];

  // Transform to physical point
  reader->GetOutput()->TransformIndexToPhysicalPoint(ul, pul);
  reader->GetOutput()->TransformIndexToPhysicalPoint(ur, pur);
  reader->GetOutput()->TransformIndexToPhysicalPoint(ll, pll);
  reader->GetOutput()->TransformIndexToPhysicalPoint(lr, plr);

  // Build the cartographic region
  RemoteSensingRegionType rsRegion;
  RemoteSensingRegionType::IndexType rsOrigin;
  RemoteSensingRegionType::SizeType rsSize;
  rsOrigin[0] = std::min(pul[0], plr[0]);
  rsOrigin[1] = std::min(pul[1], plr[1]);
  rsSize[0] = vcl_abs(pul[0] - plr[0]);
  rsSize[1] = vcl_abs(pul[1] - plr[1]);

  rsRegion.SetOrigin(rsOrigin);
  rsRegion.SetSize(rsSize);
  rsRegion.SetRegionProjection(reader->GetOutput()->GetProjectionRef());
  rsRegion.SetKeywordList(reader->GetOutput()->GetImageKeywordlist());

  // Set the cartographic region to the extract roi filter
  vdextract->SetRegion(rsRegion);

  // Reproject VectorData in image projection
  vproj = VectorDataProjectionFilterType::New();
  vproj->SetInput(vdextract->GetOutput());
  vproj->SetInputProjectionRef(vdReader->GetOutput()->GetProjectionRef());
  vproj->SetOutputKeywordList(reader->GetOutput()->GetImageKeywordlist());
  vproj->SetOutputProjectionRef(reader->GetOutput()->GetProjectionRef());
  vproj->SetOutputOrigin(reader->GetOutput()->GetOrigin());
  vproj->SetOutputSpacing(reader->GetOutput()->GetSpacing());

  //----------
  // WRITE
  VectorDataWriterType::Pointer vdwriter2 = VectorDataWriterType::New();
  vdwriter2->SetFileName(vectorDataOutputFilename2);
  vdwriter2->SetInput(vproj->GetOutput());
  vdwriter2->Update();


  return EXIT_SUCCESS;
}
