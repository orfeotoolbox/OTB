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
#include <stdlib.h>
#include <iostream>

#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimImageHandlerRegistry.h>
#include <ossim/base/ossimFilename.h>

// This test demonstrate that handler->close() does not release the file descriptor for jpeg files.

int otbOssimJpegFileRessourceLeakTest(int argc, char * argv[])
{
  if (argc != 3)
    {
    std::cerr << "Usage:" << std::endl;
    std::cerr << argv[0] << " infname nbReading" << std::endl;
    return 1;
    }

  const char *       infname = argv[1];
  const unsigned int nbReading = atoi(argv[2]);

  for (unsigned int i = 0; i < nbReading; ++i)
    {
    // Create the handler
    ossimImageHandler * handler = ossimImageHandlerRegistry::instance()->open(ossimFilename(infname));
    if (!handler)
      {
      std::cerr << "Failed to open image " << i << " times." << std::endl;
      return 1;
      }
    handler->close();
    }
  return 0;
}
