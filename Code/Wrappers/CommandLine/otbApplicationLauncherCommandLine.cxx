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
  if (argc < 2)
    {
    std::cerr << "Usage : " << argv[0] << " module_name [MODULEPATH] [arguments]" << std::endl;
    return EXIT_FAILURE;
    }

  // Construct the string expression
  std::string exp;
  for (int i = 1; i < argc; i++)
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

  if (launcher->Load(exp) == false)
    {
    if (launcher->ExecuteAndWriteOutput() == true)
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
