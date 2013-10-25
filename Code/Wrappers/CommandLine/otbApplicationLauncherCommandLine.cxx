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

#include "otbWrapperCommandLineLauncher.h"

int main(int argc, char* argv[])
{

  std::string exp;
  int start = 1;

  if (argc < 2)
    {
    std::cerr << "Usage 1: " << argv[0] << " module_name [MODULEPATH] [arguments]" << std::endl;
    std::cerr << "Usage 2: " << argv[0] << " [MODULEPATH] -inxml otbApplicationOutput.xml" << std::endl;
    return EXIT_FAILURE;
    }
    // Construct the string expression
    for (int i = start; i < argc; i++)
      {
      if (i != argc - 1)
        {
        exp.append(argv[i]);
        exp.append(" ");
        }
      else
        {
        exp.append(argv[i]);
        }
      }

  typedef otb::Wrapper::CommandLineLauncher LauncherType;
  LauncherType::Pointer launcher = LauncherType::New();

  if (launcher->Load(exp) == true)
    {
    if (launcher->ExecuteAndWriteOutput() == false)
      {
      return EXIT_FAILURE;
      }
    }
  else
    {
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
