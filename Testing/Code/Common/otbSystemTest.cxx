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

#include <iostream>
#include <cstdlib>

#include "itkMacro.h"
#include "itksys/SystemTools.hxx"
#include "otbSystem.h"

int otbSystemTest(int itkNotUsed(argc), char * argv[])
{
  const char *       inputFileName  = argv[1];
  const char *       inputDirName   = argv[2];
  std::ostringstream msg;

  // Check instanciation test
  otb::System * var(NULL);
  var = new otb::System();
  delete var;
  var = NULL;

  if (itksys::SystemTools::FileExists(inputFileName, true) == false)
    {
    itkGenericExceptionMacro(<< "itksys::SystemTools::FileExists() error : the filename " << inputFileName << " is not detected.");
    }
  if (itksys::SystemTools::FileIsDirectory(inputFileName) == true)
    {
    itkGenericExceptionMacro(<< "itksys::SystemTools::FileIsDirectory() error : the filename " << inputFileName << " is detected.");
    }
  if (itksys::SystemTools::FileExists(inputDirName, true) == true)
    {
    itkGenericExceptionMacro(<< "itksys::SystemTools::FileExists() error : the dirname " << inputDirName << " is detected!!");
    }
  if (itksys::SystemTools::FileIsDirectory(inputDirName) == false)
    {
    itkGenericExceptionMacro(<< "itksys::SystemTools::FileIsDirectory() error : the dirname " << inputDirName << " is not detected!!");
    }

  return EXIT_SUCCESS;
}


int otbParseHdfSubsetName(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  std::string input("");
  std::string key("");
  std::string name("");

  input = "SUBDATASET_7_NAME=HDF4_EOS:EOS_GRID:\"file/MOD13Q1.A2010001.h17v05.005.2010028003734.hdf\":MODIS_Grid_16DAY";
  if ( !otb::System::ParseHdfSubsetName(input, key, name) ) return EXIT_FAILURE;
  if (key.compare("SUBDATASET_7_NAME") != 0) return EXIT_FAILURE;
  if (name.compare("HDF4_EOS:EOS_GRID:\"file/MOD13Q1.A2010001.h17v05.005.2010028003734.hdf\":MODIS_Grid_16DAY") != 0)
    return EXIT_FAILURE;

  input = "invalid";
  if ( otb::System::ParseHdfSubsetName(input, key, name) ) return EXIT_FAILURE;

  return EXIT_SUCCESS;
}

int otbParseHdfFileName(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  std::string id("");
  std::string file("");
  unsigned int num(0);

  // Standard cases
  id = "/some/file.hdf:6";
  if ( !otb::System::ParseHdfFileName(id, file, num) ) return EXIT_FAILURE;
  if (file.compare("/some/file.hdf") != 0) return EXIT_FAILURE;
  if ( num != 6 ) return EXIT_FAILURE;

  id = "/some/file.hdf:0";
  if ( !otb::System::ParseHdfFileName(id, file, num) ) return EXIT_FAILURE;
  if (file.compare("/some/file.hdf") != 0) return EXIT_FAILURE;
  if ( num != 0 ) return EXIT_FAILURE;

  // non specified
  id = "/some/file.hdf";
  if ( otb::System::ParseHdfFileName(id, file, num) ) return EXIT_FAILURE;

  // playing with windows format
  id = "C:\\some\\file.hdf";
  if ( otb::System::ParseHdfFileName(id, file, num) ) return EXIT_FAILURE;
  if (num != 0) return EXIT_FAILURE;

  id = "C:\\some\\file.hdf:6";
  if ( !otb::System::ParseHdfFileName(id, file, num) ) return EXIT_FAILURE;
  if (file.compare("C:\\some\\file.hdf") != 0) return EXIT_FAILURE;
  if ( num != 6 ) return EXIT_FAILURE;

  return EXIT_SUCCESS;
}
