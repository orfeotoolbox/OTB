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


#include "itkExtractImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkLinearInterpolateImageFunction.h"

#include "otbInverseSensorModel.h"

#include "ossim/imaging/ossimImageHandler.h"
#include "ossim/base/ossimKeywordlist.h"
#include "ossim/imaging/ossimImageHandlerRegistry.h"
#include "ossim/projection/ossimProjectionFactoryRegistry.h"

int otbCreateProjectionWithOSSIM(int argc, char* argv[])
{
  if (argc != 2)
    {
    std::cout << argv[0] << " <input filename> " << std::endl;
    return EXIT_FAILURE;
    }

  otbGenericMsgDebugMacro(<< "Creating handler...");
  ossimImageHandler *handler = ossimImageHandlerRegistry::instance()->open(ossimFilename(argv[1]));
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
  ossimProjection * model = NULL;
  model = ossimProjectionFactoryRegistry::instance()->createProjection(geom);
  if (model == NULL)
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
