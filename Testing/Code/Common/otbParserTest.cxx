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
#include "itkMacro.h"
#include <cstdlib>
#include <cmath>

#include "otbParser.h"

int otbParserTestNew(int argc, char * argv[])
{
  typedef otb::Parser                                 ParserType;

  // Instantiating object
  ParserType::Pointer parser = ParserType::New();

  std::cout << parser << std::endl;

  return EXIT_SUCCESS;
}

int otbParserTest(int argc, char * argv[])
{
  double result;
  double Var1 = 10.0;
  double Var2 = 2.0;
  double Var3 = 7.0;
  double Var4 = 100.0;
  double Var5 = 200.0;
  double error = 0.0;

  std::string Expression_Numerical       = "10.0+3";
  std::string Expression_BuildInFun      = "cos(1.55)";
  std::string Expression_UserDefinedCst  = "ln10";
  std::string Expression_UserDefinedFun  = "ndvi(100, 10)";
  std::string Expression_UserDefinedVars = "(var1+var2-var3)*var4/var5";
  std::string Expression_Mixed           = "(7+10)/2+cos(pi/4)*10-10*ln10+ndvi(100, 10)";
  std::string Expression_StressTest      = "(7+10)/2+cos(pi/4)*10-10*ln10+ndvi(100, 10)*40+rint(83.144)*0.6588";

  typedef otb::Parser                                 ParserType;

  // Instantiating object
  ParserType::Pointer parser = ParserType::New();

  std::cout << std::endl;

  parser->SetExpr(Expression_Numerical);
  result = parser->Eval();
  std::cout << "Expression Parsing Result : "                    << std::endl;
  std::cout << Expression_Numerical << " = " << result           << std::endl; // 13.0
  std::cout << "Expected Result : " << 13.0 << std::endl         << std::endl;
  error += sqrt((result - 13.0)*(result - 13.0));

  parser->SetExpr(Expression_BuildInFun);
  result = parser->Eval();
  std::cout << "Expression Parsing Result : "                    << std::endl;
  std::cout << Expression_BuildInFun << " = " << result          << std::endl; // 0.0207948278
  std::cout << "Expected Result : " << 0.0207948278 << std::endl << std::endl;
  error += sqrt((result - 0.0207948278)*(result - 0.0207948278));

  parser->SetExpr(Expression_UserDefinedCst);
  result = parser->Eval();
  std::cout << "Expression Parsing Result : "                    << std::endl;
  std::cout << Expression_UserDefinedCst << " = " << result      << std::endl; // 2.302585092
  std::cout << "Expected Result : " << 2.302585092 << std::endl  << std::endl;
  error += sqrt((result - 2.302585092)*(result - 2.302585092));

  parser->SetExpr(Expression_UserDefinedFun);
  result = parser->Eval();
  std::cout << "Expression Parsing Result : "                    << std::endl;
  std::cout << Expression_UserDefinedFun << " = " << result      << std::endl; // -0.8181818182
  std::cout << "Expected Result : " << -0.81818182 << std::endl  << std::endl;
  error += sqrt((result - (-0.8181818182))*(result - (-0.8181818182)));

  parser->SetExpr(Expression_UserDefinedVars);
  parser->DefineVar("var1", &Var1);
  parser->DefineVar("var2", &Var2);
  parser->DefineVar("var3", &Var3);
  parser->DefineVar("var4", &Var4);
  parser->DefineVar("var5", &Var5);
  result = parser->Eval();
  parser->ClearVar();
  std::cout << "Expression Parsing Result : "                    << std::endl;
  std::cout << Expression_UserDefinedVars << " = " << result     << std::endl; // 2.5
  std::cout << "Expected Result : " << 2.5         << std::endl  << std::endl;
  error += sqrt((result - 2.5)*(result - 2.5));

  parser->SetExpr(Expression_Mixed);
  result = parser->Eval();
  std::cout << "Expression Parsing Result : "                    << std::endl;
  std::cout << Expression_Mixed << " = " << result               << std::endl; // -8.27296
  std::cout << "Expected Result : " << -8.27296 << std::endl     << std::endl;
  error += sqrt((result - (-8.27296))*(result - (-8.27296)));

  parser->SetExpr(Expression_StressTest);
  result = parser->Eval();
  std::cout << "Expression Parsing Result : "                    << std::endl;
  std::cout << Expression_StressTest << " = " << result          << std::endl; // 14.4983
  std::cout << "Expected Result : " << 14.4983 << std::endl      << std::endl;
  error += sqrt((result - 14.4983)*(result - 14.4983));


  error /= 7;
  std::cout << "Error : " << error << std::endl;

  if(error > 10E-5)
    {
    itkGenericExceptionMacro(  << "Cumulated Error > 10E-5 -> Test Failled" << std::endl );
    }

  /* Exception Tests */
  //parser->DefineVar("ndvi", &Var1);
  //parser->DefineVar("euler", &Var2);

  return EXIT_SUCCESS;
}
