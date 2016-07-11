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
#ifndef __otbMPIVrtWriter_h
#define __otbMPIVrtWriter_h

#include "otbMPIConfig.h"
#include "otbImageFileWriter.h"
#include "itkRegionOfInterestImageFilter.h"
#include "otbNumberOfDivisionsTiledStreamingManager.h"
#include <vector>
#include <iostream>
#include <sstream>

#include <itksys/SystemTools.hxx>

#include <gdal.h>
#include <gdal_priv.h>
#if defined(__GNUC__) || defined(__clang__)
# pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wshadow"
#include <vrtdataset.h>
# pragma GCC diagnostic pop
#else
#include <vrtdataset.h>
#endif

#include <ogr_spatialref.h>

namespace otb {

/**
 *\brief Write image data to multiple files with MPI processus and add a VRT file.
 *
 * The image is divided into several pieces.
 * Each pieces is distributed to a MPI processus.
 * Each MPI processus write their pieces into a separate
 * file.
 * The master processus writes a VRT file (optional).
 *
 *\param img Image
 *\param output Output Filename
 *\param availableRAM Available memory for streaming
 *\param writeVRTFile Activate the VRT file writing
 */
template <typename TImage> void WriteMPI(TImage *img, const std::string &output, unsigned int availableRAM = 0, bool writeVRTFile=true)
{
  typename otb::MPIConfig::Pointer mpiConfig = otb::MPIConfig::Instance();

  unsigned int myRank = mpiConfig->GetMyRank();
  unsigned int nbProcs = mpiConfig->GetNbProcs();

  typedef otb::ImageFileWriter<TImage>                                           WriterType;
  typedef otb::NumberOfDivisionsTiledStreamingManager<TImage>                    StreamingManagerType;
  typedef itk::RegionOfInterestImageFilter<TImage, TImage>                       ExtractFilterType;

  // First, update infomration from image to write
  img->UpdateOutputInformation();

  // Configure streaming manager
  typename StreamingManagerType::Pointer streamingManager = StreamingManagerType::New();
  streamingManager->SetNumberOfDivisions(nbProcs);
  streamingManager->PrepareStreaming(img,img->GetLargestPossibleRegion());
  unsigned int numberOfSplits = streamingManager->GetNumberOfSplits();

  // This vector will hold all regions to write for current rank
  std::vector<typename TImage::RegionType> regionsToWrite;

  // Handle both cases when there are much more (resp. less) region to
  // write than NbProcs
  if(myRank < numberOfSplits)
  {
    unsigned int splitIdx = myRank;
    while(splitIdx < numberOfSplits)
    {
      typename TImage::RegionType currentRegion = streamingManager->GetSplit(splitIdx);
      regionsToWrite.push_back(currentRegion);
      splitIdx+=nbProcs;
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
  joins.push_back(itksys::SystemTools::GetFilenamePath(output).append("/"));
  joins.push_back(itksys::SystemTools::GetFilenameWithoutExtension(output));
  std::string prefix = itksys::SystemTools::JoinPath(joins);


  // Data type
  std::string dataTypeStr = "Float32";
  GDALImageIO::Pointer gdalImageIO;

  // Now write all the regions
  for(typename std::vector<typename TImage::RegionType>::const_iterator it = regionsToWrite.begin();
      it!=regionsToWrite.end();++it)
  {
    typename ExtractFilterType::Pointer extractFilter = ExtractFilterType::New();
    extractFilter->SetInput(img);
    extractFilter->SetRegionOfInterest(*it);
    // Writer
	  // Output Filename
    std::stringstream ss;
    ss<<prefix<<"_"<<it->GetIndex()[0]<<"_"<<it->GetIndex()[1]<<"_"<<it->GetSize()[0]<<"_"<<it->GetSize()[1]<<".tif";
    typename WriterType::Pointer writer = WriterType::New();
    writer->SetFileName(ss.str());
    writer->SetInput(extractFilter->GetOutput());
    // Datatype
    gdalImageIO = dynamic_cast<GDALImageIO *>(writer->GetImageIO());
    if(gdalImageIO.IsNotNull())
    {
      dataTypeStr = gdalImageIO->GetGdalPixelTypeAsString();
    }

    if (!availableRAM)
      {
      writer->SetNumberOfDivisionsTiledStreaming(0);
      }
    else
      {
      writer->SetAutomaticAdaptativeStreaming(availableRAM);
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
    if (writeVRTFile && (myRank == 0))
    {
      // VRT Filename
      std::stringstream vrtfOut;
      vrtfOut<< prefix << ".vrt";

      // Data type
      GDALDataType dataType;
      dataType = GDALGetDataTypeByName(dataTypeStr.c_str());

      int imageSizeY = img->GetLargestPossibleRegion().GetSize()[1];
      int imageSizeX = img->GetLargestPossibleRegion().GetSize()[0];
      const unsigned int nbBands = img->GetNumberOfComponentsPerPixel();

      // Get VRT driver
      GDALAllRegister();
      GDALDriver *driver = GetGDALDriverManager()->GetDriverByName("VRT");
      if (driver == NULL) {
         mpiConfig->logError("Error opening VRT driver.");
         mpiConfig->abort(EXIT_FAILURE);
      }

      // Create output raster
      GDALDataset *VRTOutput = driver->Create(vrtfOut.str().c_str(),
            imageSizeX,
            imageSizeY,
            0,
            dataType,
            NULL); // No options
      if (VRTOutput == NULL) {
        mpiConfig->logError("driver->Create call failed.\n");
        mpiConfig->abort(EXIT_FAILURE);
      }

      // Set GeoTransform
      double gt[6];
      gt[0] = img->GetOrigin()[0] - 0.5*img->GetSpacing()[0];
      gt[1] = img->GetSpacing()[0];
      gt[2] = 0.0;
      gt[3] = img->GetOrigin()[1] - 0.5*img->GetSpacing()[1];
      gt[4] = 0.0;
      gt[5] = img->GetSpacing()[1];
      VRTOutput->SetGeoTransform(gt);

      // Set projection
      OGRSpatialReference out_sr;
      char *wkt = NULL;
      out_sr.SetFromUserInput(img->GetProjectionRef().c_str());
      out_sr.exportToWkt(&wkt);
      VRTOutput->SetProjection(wkt);

      for(unsigned int band = 1; band<=nbBands;++band)
      {
        VRTOutput->AddBand(dataType, NULL);

        typename TImage::RegionType currentRegion;
        for(unsigned int id=0; id < numberOfSplits; ++id)
        {
          currentRegion = streamingManager->GetSplit(id);
          int tileSizeX = currentRegion.GetSize()[0];
          int tileSizeY = currentRegion.GetSize()[1];
          int tileIndexX = currentRegion.GetIndex()[0];
          int tileIndexY = currentRegion.GetIndex()[1];
          std::stringstream tileFileName;
          tileFileName <<prefix<<"_"<<tileIndexX<<"_"<<tileIndexY<<"_"<<tileSizeX<<"_"<<tileSizeY<<".tif";
          std::cout<<tileFileName.str()<<std::endl;

          GDALDataset *dataset = (GDALDataset*) GDALOpen(tileFileName.str().c_str(), GA_ReadOnly);

          VRTSourcedRasterBand *VRTBand = dynamic_cast<VRTSourcedRasterBand*> (VRTOutput->GetRasterBand(band));
          VRTBand->AddSimpleSource(dataset->GetRasterBand(band),
                                      0, //xOffSrc
                                      0, //yOffSrc
                                      tileSizeX, //xSizeSrc
                                      tileSizeY, //ySizeSrc
                                      tileIndexX, //xOffDest
                                      tileIndexY, //yOffDest
                                      tileSizeX, //xSizeDest
                                      tileSizeY, //ySizeDest
                                      "near",
                                      VRT_NODATA_UNSET);
        }

      }

      // Close
      OGRFree(wkt);
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

} // End namespace otb
#endif //__otbMPIVrtWriter_h
