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
 * Application pour projeter une region d'une image en coordonnees geographiques
 * en utilisant un Interpolator+regionextractor et un Iterator.
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

#include "itkUnaryFunctorImageFilter.h"
#include "itkExtractImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkLinearInterpolateImageFunction.h"

#include "otbInverseSensorModel.h"

#include "otb_ossim.h"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Woverloaded-virtual"
#pragma GCC diagnostic ignored "-Wshadow"
#include "ossim/imaging/ossimImageHandler.h"
#include "ossim/base/ossimKeywordlist.h"
#include "ossim/imaging/ossimImageHandlerRegistry.h"
#include "ossim/projection/ossimProjectionFactoryRegistry.h"
#pragma GCC diagnostic pop
#else
#include "ossim/imaging/ossimImageHandler.h"
#include "ossim/base/ossimKeywordlist.h"
#include "ossim/imaging/ossimImageHandlerRegistry.h"
#include "ossim/projection/ossimProjectionFactoryRegistry.h"
#endif

int otbCreateProjectionWithOSSIM(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cout << argv[0] << " <input filename> " << std::endl;
    return EXIT_FAILURE;
  }

  otbGenericMsgDebugMacro(<< "Creating handler...");
  ossimImageHandler* handler = ossimImageHandlerRegistry::instance()->open(ossimFilename(argv[1]));
  if (!handler)
  {
    itkGenericExceptionMacro(<< "Unable to open input image " << argv[1]);
  }

  ossimKeywordlist geom;
  otbGenericMsgDebugMacro(<< "Read ossim Keywordlist...");
  handler->getImageGeometry()->getProjection()->saveState(geom);
  ossimGpt ossimGPoint(0, 0);
  ossimDpt ossimDPoint;
  otbGenericMsgDebugMacro(<< "Creating projection...");
  ossimProjection* model = nullptr;
  model                  = ossimProjectionFactoryRegistry::instance()->createProjection(geom);
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
