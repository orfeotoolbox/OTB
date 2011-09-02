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
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "otbWrapperCommandLineParser.h"


int otbWrapperCommandLineParserNew(int argc, char* argv[])
{
  typedef otb::Wrapper::CommandLineParser ParserType;
  ParserType::Pointer parser = ParserType::New();

  return EXIT_SUCCESS;
}

int otbWrapperCommandLineParserTest(int argc, char* argv[])
{
  typedef otb::Wrapper::CommandLineParser ParserType;
  ParserType::Pointer parser = ParserType::New();


  ParserType::ParseResultType res = parser->CheckExpression( argv[1] );
  /*if( res == ParserType::OK)
    {
      return EXIT_SUCCESS;
    }
    else */if( res == ParserType::EMPTYEXPRESSION )
    {
      std::cout<<"Empty expression."<<std::endl;
      return EXIT_FAILURE;
    }
  else if( res == ParserType::NOMODULENAME )
    {
      std::cout<<"No module name detected."<<std::endl;
      return EXIT_FAILURE;
    }
else if( res == ParserType::MULTIPLEMODULENAME )
    {
      std::cout<<"Multiple module name detected. Only one possible."<<std::endl;
      return EXIT_FAILURE;
    }
  else if( res == ParserType::WRONGMODULENAME )
    {
      std::cout<<"Invalid module name."<<std::endl;
      return EXIT_FAILURE;
    }
 else if( res == ParserType::INVALIDMODULENAME )
    {
      std::cout<<"Invalid module name. Must only contain alphanumerical characters."<<std::endl;
      return EXIT_FAILURE;
    }
 else if( res == ParserType::INVALIDMODULEPATH )
    {
      std::cout<<"Invalid module path. At least one path doesn't exists."<<std::endl;
      return EXIT_FAILURE;
    }
 else if( res == ParserType::NOMODULEPATH )
    {
      std::cout<<"No module path specified."<<std::endl;
      return EXIT_FAILURE;
    }


  ParserType::ParseResultType res2 = parser->ParseApplicationArgument( argv[1] );
  
  if( res2 == ParserType::MISSINGMANDATORYPARAMETER )
    {
      std::cout<<"Missing mandatory parameter."<<std::endl;
      return EXIT_FAILURE;
    }
  else if( res2 == ParserType::MISSINGPARAMETERVALUE )
    {
      std::cout<<"Missing parameter value."<<std::endl;
      return EXIT_FAILURE;
    }
  else if( res2 == ParserType::INVALIDNUMBEROFVALUE )
    {
      std::cout<<"Invalid number of parameter values."<<std::endl;
      return EXIT_FAILURE;
    }
    
  return EXIT_SUCCESS;
  
}


