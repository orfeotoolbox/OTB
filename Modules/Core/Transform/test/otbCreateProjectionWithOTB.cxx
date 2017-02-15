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

#include "itkUnaryFunctorImageFilter.h"
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
  ossimProjection * model = ITK_NULLPTR;
  otbGenericMsgDebugMacro(<< "Creating projection...");
  model = ossimProjectionFactoryRegistry::instance()->createProjection(geom);
  if (model == ITK_NULLPTR)
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
