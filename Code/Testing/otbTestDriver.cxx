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

// define some itksys* things to make ShareForward.h happy
#define itksys_SHARED_FORWARD_DIR_BUILD ""
#define itksys_SHARED_FORWARD_PATH_BUILD ""
#define itksys_SHARED_FORWARD_PATH_INSTALL ""
#define itksys_SHARED_FORWARD_EXE_BUILD ""
#define itksys_SHARED_FORWARD_EXE_INSTALL ""

#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include "itksys/SystemTools.hxx"

// include SharedForward to avoid duplicating the code which find the library path variable
// name and the path separator
#include "itksys/SharedForward.h"
#include "itksys/Process.h"

// Temporary definition of otbTestMain
int otbTestMain(int arc, char * arv[]);

/** Display usage */
void usage()
{
  std::cerr << "usage: otbTestDriver [global_options] [non_regression_commands] Execute prg [args]" << std::endl;
  std::cerr << std::endl;
  std::cerr <<
  "otbTestDriver alter the environment, run a test program and does regression testing based on capabilities provided by otbTestMain.h"
            << std::endl;
  std::cerr << std::endl;
  std::cerr << "Global options:" << std::endl;
  std::cerr << "  --add-before-libpath PATH" << std::endl;
  std::cerr << "      Add a path to the library path environment. This option take care of" << std::endl;
  std::cerr << "      choosing the right environment variable for your system." << std::endl;
  std::cerr << "      This option can be used several times." << std::endl;
  std::cerr << std::endl;
  std::cerr << "  --add-before-env NAME VALUE" << std::endl;
  std::cerr << "      Add a VALUE to the variable name in the environment." << std::endl;
  std::cerr << "      This option can be used several times." << std::endl;
  std::cerr << std::endl;
  std::cerr << "  --help" << std::endl;
  std::cerr << "      Display this message and exit." << std::endl;
  std::cerr << std::endl;

}

/** This function parses the command line and process everything
 *  related to the --add-before-libpath, --add-before-env and --help.
 *  Every other args are added to the remainingArgs vector */
int parseCommandLine(int ac, char * av[], std::vector<char *>& remainingArgs)
{
  // parse the command line
  int  i = 1;
  bool skip = false;
  while (i < ac)
    {
    if (!skip && strcmp(av[i], "--add-before-libpath") == 0)
      {
      if (i + 1 >= ac)
        {
        usage();
        return 1;
        }
      std::string libpath = KWSYS_SHARED_FORWARD_LDPATH;
      libpath += "=";
      libpath += av[i + 1];
      char * oldenv = getenv(KWSYS_SHARED_FORWARD_LDPATH);
      if (oldenv)
        {
        libpath += KWSYS_SHARED_FORWARD_PATH_SEP;
        libpath += oldenv;
        }
      itksys::SystemTools::PutEnv(libpath.c_str());
      // on some 64 bit systems, LD_LIBRARY_PATH_64 is used before
      // LD_LIBRARY_PATH if it is set. It can lead the test to load
      // the system library instead of the expected one, so this
      // var must also be set
      if (std::string(KWSYS_SHARED_FORWARD_LDPATH) == "LD_LIBRARY_PATH")
        {
        std::string libpath = "LD_LIBRARY_PATH_64";
        libpath += "=";
        libpath += av[i + 1];
        char * oldenv = getenv("LD_LIBRARY_PATH_64");
        if (oldenv)
          {
          libpath += KWSYS_SHARED_FORWARD_PATH_SEP;
          libpath += oldenv;
          }
        itksys::SystemTools::PutEnv(libpath.c_str());
        }
      i += 2;
      }
    else if (!skip && strcmp(av[i], "--add-before-env") == 0)
      {
      if (i + 2 >= ac)
        {
        usage();
        return 1;
        }
      std::string env = av[i + 1];
      env += "=";
      env += av[i + 2];
      char * oldenv = getenv(av[i + 1]);
      if (oldenv)
        {
        env += KWSYS_SHARED_FORWARD_PATH_SEP;
        env += oldenv;
        }
      itksys::SystemTools::PutEnv(env.c_str());
      i += 3;
      }
    else if (!skip && strcmp(av[i], "--help") == 0)
      {
      usage();
      return 0;
      }
    else
      {
      remainingArgs.push_back(av[i]);
      i += 1;
      }
    }
  return 0;
}

int main(int ac, char* av[])
{
  // A vector to store remaining args
  std::vector<char*> remainingArgs;

  // First parse the command line for system wide options
  int ret = parseCommandLine(ac, av, remainingArgs);

  // Check for the return code
  if (ret)
    {
    std::cerr << "Error while parsing arguments, exiting ..." << std::endl;
    return 1;
    }

  // Check if there are remaining args
  if (remainingArgs.empty())
    {
    usage();
    return 1;
    }
  // a NULL is required at the end of the table
  char** argv = new char*[remainingArgs.size() + 2];
  for (int i = 0; i < static_cast<int>(remainingArgs.size()); ++i)
    {
    argv[i + 1] = remainingArgs[i];
    }
  argv[remainingArgs.size() + 1] = NULL;

  /** Call to the otbTestMain */
  return otbTestMain(remainingArgs.size(), argv);

}

// This is a dummy main to be registered as a test for the otbTestMain
int Execute(int argc, char * argv[])
{
  argc -= 1;
  argv += 1;
  // Create the appropriate itk process
  itksysProcess * process = itksysProcess_New();
  itksysProcess_SetCommand(process, argv);
  itksysProcess_SetPipeShared(process, itksysProcess_Pipe_STDOUT, true);
  itksysProcess_SetPipeShared(process, itksysProcess_Pipe_STDERR, true);
  itksysProcess_Execute(process);
  itksysProcess_WaitForExit(process, NULL);
  int retCode = itksysProcess_GetExitValue(process);
  return retCode;
}

// Include otbTestMain and switch otbTestMain with main definition in otbTestMain.h
#undef otbTestMain
#undef main
#define main otbTestMain
#include "otbTestMain.h"

void RegisterTests()
{
  REGISTER_TEST(Execute);
}
