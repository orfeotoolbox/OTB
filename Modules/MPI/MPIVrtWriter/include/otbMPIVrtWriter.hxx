/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbMPIVrtWriter_hxx
#define otbMPIVrtWriter_hxx

#include "otbMPIVrtWriter.h"
#include "otbMacro.h"

namespace otb
{

template <typename TImage>
MPIVrtWriter<TImage>::MPIVrtWriter() : m_AvailableRAM(0), m_IORegion(), m_Filename(""), m_WriteVRT(true)
{
}

template <typename TImage>
MPIVrtWriter<TImage>::~MPIVrtWriter()
{
}

template <typename TImage>
void MPIVrtWriter<TImage>::SetInput(const InputImageType* input)
{
  this->ProcessObject::SetNthInput(0, const_cast<InputImageType*>(input));
}

template <typename TImage>
const TImage* MPIVrtWriter<TImage>::GetInput()
{
  if (this->GetNumberOfInputs() < 1)
  {
    return 0;
  }
  return static_cast<const InputImageType*>(this->ProcessObject::GetInput(0));
}

template <typename TImage>
void MPIVrtWriter<TImage>::SetFileName(const std::string& extendedFileName)
{
  if (m_Filename.compare(extendedFileName) != 0)
  {
    m_Filename = extendedFileName;
    this->Modified();
  }
}

template <typename TImage>
const char* MPIVrtWriter<TImage>::GetFileName() const
{
  return m_Filename.c_str();
}

template <typename TImage>
void MPIVrtWriter<TImage>::SetIORegion(const itk::ImageIORegion& region)
{
  if (m_IORegion != region)
  {
    m_IORegion = region;
    this->Modified();
  }
}

template <typename TImage>
void MPIVrtWriter<TImage>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "File Name: " << m_Filename << std::endl;
  os << indent << "Available RAM: " << m_AvailableRAM << std::endl;
  os << indent << "Write VRT: " << m_WriteVRT << std::endl;
}

template <typename TImage>
void MPIVrtWriter<TImage>::Update()
{
  typename otb::MPIConfig::Pointer mpiConfig = otb::MPIConfig::Instance();

  unsigned int myRank  = mpiConfig->GetMyRank();
  unsigned int nbProcs = mpiConfig->GetNbProcs();

  typedef otb::ImageFileWriter<TImage>                        WriterType;
  typedef otb::NumberOfDivisionsTiledStreamingManager<TImage> StreamingManagerType;
  typedef itk::RegionOfInterestImageFilter<TImage, TImage> ExtractFilterType;

  // First, update infomration from image to write
  UpdateOutputInformation();
  InputImageType* img    = const_cast<InputImageType*>(GetInput());
  std::string     output = GetFileName();

  // Configure streaming manager
  typename StreamingManagerType::Pointer streamingManager = StreamingManagerType::New();
  streamingManager->SetNumberOfDivisions(nbProcs);
  streamingManager->PrepareStreaming(img, img->GetLargestPossibleRegion());
  unsigned int numberOfSplits = streamingManager->GetNumberOfSplits();

  // This vector will hold all regions to write for current rank
  std::vector<typename TImage::RegionType> regionsToWrite;

  // Handle both cases when there are much more (resp. less) region to
  // write than NbProcs
  if (myRank < numberOfSplits)
  {
    unsigned int splitIdx = myRank;
    while (splitIdx < numberOfSplits)
    {
      typename TImage::RegionType currentRegion = streamingManager->GetSplit(splitIdx);
      regionsToWrite.push_back(currentRegion);
      splitIdx += nbProcs;
    }
  }

  // Output prefix
  std::string extension = itksys::SystemTools::GetFilenameExtension(output);
  if (extension != ".vrt")
  {

    // TODO: Maybe remove this
    if (extension == "")
    {
      // Missing extension
      mpiConfig->logInfo("Filename has no extension. Adding <.vrt> extension.");
    }
    else
    {
      // Bad extension
      mpiConfig->logError("Filename must have .vrt extension!");
      mpiConfig->abort(EXIT_FAILURE);
    }
  }
  std::vector<std::string> joins;
  itksys::SystemTools::SplitPath(output, joins);
  joins.back()       = itksys::SystemTools::GetFilenameWithoutExtension(output);
  std::string prefix = itksys::SystemTools::JoinPath(joins);

  // Data type
  std::string          dataTypeStr = "Float32";
  GDALImageIO::Pointer gdalImageIO;

  // Now write all the regions
  for (typename std::vector<typename TImage::RegionType>::const_iterator it = regionsToWrite.begin(); it != regionsToWrite.end(); ++it)
  {
    typename ExtractFilterType::Pointer extractFilter = ExtractFilterType::New();
    extractFilter->SetInput(img);
    extractFilter->SetRegionOfInterest(*it);
    // Writer
    // Output Filename
    std::stringstream ss;
    ss << prefix << "_" << it->GetIndex()[0] << "_" << it->GetIndex()[1] << "_" << it->GetSize()[0] << "_" << it->GetSize()[1] << ".tif";
    typename WriterType::Pointer writer = WriterType::New();
    writer->SetFileName(ss.str());
    writer->SetInput(extractFilter->GetOutput());
    // Datatype
    gdalImageIO = dynamic_cast<GDALImageIO*>(writer->GetImageIO());
    if (gdalImageIO.IsNotNull())
    {
      dataTypeStr = gdalImageIO->GetGdalPixelTypeAsString();
    }

    if (!m_AvailableRAM)
    {
      writer->SetNumberOfDivisionsTiledStreaming(0);
    }
    else
    {
      writer->SetAutomaticAdaptativeStreaming(m_AvailableRAM);
    }

    // Pipeline execution
    try
    {
      writer->Update();
    }
    catch (itk::ExceptionObject& err)
    {
      std::stringstream message;
      message << "ExceptionObject caught: " << err << std::endl;
      mpiConfig->logError(message.str());
      mpiConfig->abort(EXIT_FAILURE);
    }
  }

  // MPI process synchronization
  mpiConfig->barrier();

  // Write VRT file
  try
  {
    if (m_WriteVRT && (myRank == 0))
    {
      // VRT Filename
      std::stringstream vrtfOut;
      vrtfOut << prefix << ".vrt";

      // Data type
      GDALDataType dataType;
      dataType = GDALGetDataTypeByName(dataTypeStr.c_str());

      int                imageSizeY = img->GetLargestPossibleRegion().GetSize()[1];
      int                imageSizeX = img->GetLargestPossibleRegion().GetSize()[0];
      const unsigned int nbBands    = img->GetNumberOfComponentsPerPixel();

      // Get VRT driver
      GDALAllRegister();
      GDALDriver* driver = GetGDALDriverManager()->GetDriverByName("VRT");
      if (driver == NULL)
      {
        mpiConfig->logError("Error opening VRT driver.");
        mpiConfig->abort(EXIT_FAILURE);
      }

      // Create output raster
      GDALDataset* VRTOutput = driver->Create(vrtfOut.str().c_str(), imageSizeX, imageSizeY, 0, dataType, NULL); // No options
      if (VRTOutput == NULL)
      {
        mpiConfig->logError("driver->Create call failed.\n");
        mpiConfig->abort(EXIT_FAILURE);
      }

      // Set GeoTransform
      double gt[6];
      gt[0] = img->GetOrigin()[0] - 0.5 * img->GetSignedSpacing()[0];
      gt[1] = img->GetSignedSpacing()[0];
      gt[2] = 0.0;
      gt[3] = img->GetOrigin()[1] - 0.5 * img->GetSignedSpacing()[1];
      gt[4] = 0.0;
      gt[5] = img->GetSignedSpacing()[1];
      VRTOutput->SetGeoTransform(gt);

      // Set projection
      OGRSpatialReference out_sr;
      char*               wkt = NULL;
      out_sr.SetFromUserInput(img->GetProjectionRef().c_str());
      out_sr.exportToWkt(&wkt);
      VRTOutput->SetProjection(wkt);

      for (unsigned int band = 1; band <= nbBands; ++band)
      {
        VRTOutput->AddBand(dataType, NULL);

        typename TImage::RegionType currentRegion;
        for (unsigned int id = 0; id < numberOfSplits; ++id)
        {
          currentRegion                = streamingManager->GetSplit(id);
          int               tileSizeX  = currentRegion.GetSize()[0];
          int               tileSizeY  = currentRegion.GetSize()[1];
          int               tileIndexX = currentRegion.GetIndex()[0];
          int               tileIndexY = currentRegion.GetIndex()[1];
          std::stringstream tileFileName;
          tileFileName << prefix << "_" << tileIndexX << "_" << tileIndexY << "_" << tileSizeX << "_" << tileSizeY << ".tif";
          otbDebugMacro(<< tileFileName.str());

          GDALDataset* dataset = (GDALDataset*)GDALOpen(tileFileName.str().c_str(), GA_ReadOnly);

          VRTSourcedRasterBand* VRTBand = dynamic_cast<VRTSourcedRasterBand*>(VRTOutput->GetRasterBand(band));
          VRTBand->AddSimpleSource(dataset->GetRasterBand(band),
                                   0,          // xOffSrc
                                   0,          // yOffSrc
                                   tileSizeX,  // xSizeSrc
                                   tileSizeY,  // ySizeSrc
                                   tileIndexX, // xOffDest
                                   tileIndexY, // yOffDest
                                   tileSizeX,  // xSizeDest
                                   tileSizeY,  // ySizeDest
                                   "near", VRT_NODATA_UNSET);
        }
      }

      // Close
      CPLFree(wkt);
      GDALClose(VRTOutput);
    }
  }
  catch (itk::ExceptionObject& err)
  {
    std::stringstream message;
    message << "ExceptionObject caught: " << err << std::endl;
    mpiConfig->logError(message.str());
    mpiConfig->abort(EXIT_FAILURE);
  }
}


} // end of namespace otb

#endif
