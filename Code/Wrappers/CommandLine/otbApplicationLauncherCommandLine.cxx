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

std::string PrepareExpressionFromXML(std::string filename);

int main(int argc, char* argv[])
{
  if (argc < 2)
    {
    std::cerr << "Usage : " << argv[0] << " module_name [MODULEPATH] [arguments]" << std::endl;
    return EXIT_FAILURE;
    }

  std::string exp;
  if( strcmp(argv[1], "-inxml") == 0 )
  {
     PrepareExpressionFromXML(argv[2]);
  }

  // Construct the string expression

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

  std::cerr << exp << "\n";

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

std::string PrepareExpressionFromXML(std::string filename)
{

  std::string expression;

//XMLLoader -in ./output.xml
  return expression;

}
