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
#ifndef __otbMPIConfig_txx
#define __otbMPIConfig_txx

#include "otbMPIConfig.h"
#include "otbImageFileWriter.h"
#include "itkRegionOfInterestImageFilter.h"
#include "otbNumberOfDivisionsTiledStreamingManager.h"
#include <vector>
#include <iostream>
#include <sstream>

namespace otb {

namespace mpi {

template <typename TImage> void MPIConfig::UpdateMPI (TImage *img, const std::string &prefix, bool useStreaming=true, const std::string xtArgs = "") {
   typedef otb::ImageFileWriter<TImage> WriterType;
   typedef otb::NumberOfDivisionsTiledStreamingManager<TImage>
StreamingManagerType;
   typedef itk::RegionOfInterestImageFilter<TImage, TImage> ExtractFilterType;

   // First, update from image to write
   img->UpdateOutputInformation();

   // Configure streaming manager
   typename StreamingManagerType::Pointer streamingManager = StreamingManagerType::New();
   streamingManager->SetNumberOfDivisions(m_NbProcs);
streamingManager->PrepareStreaming(img,img->GetLargestPossibleRegion());
   unsigned int numberOfSplits = streamingManager->GetNumberOfSplits();

   // This vector will hold all regions to write for current rank
   std::vector<typename TImage::RegionType> regionsToWrite;

   // Handle both cases when there are much more (resp. less) region to
   // write than NbProcs
   if (m_MyRank < numberOfSplits)
   {
     unsigned int splitIdx = m_MyRank;
     while(splitIdx < numberOfSplits)
     {
       typename TImage::RegionType currentRegion = 
streamingManager->GetSplit(splitIdx);
       regionsToWrite.push_back(currentRegion);
       splitIdx+=m_NbProcs;
     }
   }

   // Now write all the regions
   for (typename std::vector<typename
TImage::RegionType>::const_iterator it =
regionsToWrite.begin();it!=regionsToWrite.end();++it)
   {
     typename ExtractFilterType::Pointer extractFilter = ExtractFilterType::New();
     extractFilter->SetInput(img);
     extractFilter->SetRegionOfInterest(*it);
     // Writer
     std::stringstream ss;
ss<<prefix<<"_"<<it->GetIndex()[0]<<"_"<<it->GetIndex()[1]<<"_"<<it->GetSize()[0]<<"_"<<it->GetSize()[1]<<".tif"<<xtArgs;
     typename WriterType::Pointer writer = WriterType::New();
     writer->SetFileName(ss.str());
     writer->SetInput(extractFilter->GetOutput());

     if (!useStreaming)
     {
       writer->SetNumberOfDivisionsTiledStreaming(0);

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
       this->logError(message.str());
       this->abort(EXIT_FAILURE);
     }
   }
}

} // End namespece mpi

} // End namespace otb
#endif //__otbMPIConfig_txx

