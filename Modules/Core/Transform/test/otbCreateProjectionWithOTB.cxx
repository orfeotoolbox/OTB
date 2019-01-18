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



/*!
 *
 * PURPOSE:
 *
 * Application to rproject an image region into gepgraphical coordinates
 * usinf un Interpolator+regionextractor and an Iterator.
 *
 */

// iostream is used for general output
#include <iostream>
#include <iterator>
#include <stdlib.h>

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImageFileWriter.h"

#include "itkExtractImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkLinearInterpolateImageFunction.h"

#include "otbInverseSensorModel.h"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Woverloaded-virtual"
#pragma GCC diagnostic ignored "-Wshadow"
#include "ossim/base/ossimKeywordlist.h"
#include "ossim/base/ossimGpt.h"
#include "ossim/projection/ossimProjectionFactoryRegistry.h"
#include "ossim/imaging/ossimImageHandlerRegistry.h"
#pragma GCC diagnostic pop
#else
#include "ossim/base/ossimKeywordlist.h"
#include "ossim/base/ossimGpt.h"
#include "ossim/projection/ossimProjectionFactoryRegistry.h"
#include "ossim/imaging/ossimImageHandlerRegistry.h"
#endif

int otbCreateProjectionWithOTB(int argc, char* argv[])
{
  if (argc != 2)
    {
    std::cout << argv[0] << " <input filename> " << std::endl;
    return EXIT_FAILURE;
    }

  typedef otb::Image<unsigned int, 2>     ImageType;
  typedef otb::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);

  //Read meta data (ossimKeywordlist)
  reader->GenerateOutputInformation();

  otbGenericMsgDebugMacro(<< "Read ossim Keywordlist...");
  otb::ImageKeywordlist otb_image_keywordlist = reader->GetOutput()->GetImageKeywordlist();

  ossimKeywordlist geom;
  otb_image_keywordlist.convertToOSSIMKeywordlist(geom);

  otbGenericMsgDebugMacro(<< "ossim Keywordlist:" << geom);

  ossimGpt          ossimGPoint(0, 0);
  ossimDpt          ossimDPoint;
  ossimProjection * model = nullptr;
  otbGenericMsgDebugMacro(<< "Creating projection...");
  model = ossimProjectionFactoryRegistry::instance()->createProjection(geom);
  if (model == nullptr)
    {
    itkGenericExceptionMacro(<< "Invalid Model * == NULL !");
    }

  otbGenericMsgDebugMacro(<< "Creating RefPtr of projection...");
  ossimRefPtr<ossimProjection> ptrmodel = model;
  if (ptrmodel.valid() == false)
    {
    itkGenericExceptionMacro(<< "Invalid Model pointer .valid() == false !");
    }

  return EXIT_SUCCESS;

}
