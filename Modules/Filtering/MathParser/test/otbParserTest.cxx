/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "otbMath.h"
#include "otbParser.h"

typedef otb::Parser ParserType;


void otbParserTest_ThrowIfNotEqual(double output, double ref, std::string testname, double epsilon = 1.0E-12)
{
  std::cout << "Running test " << testname << std::endl;
  if (std::abs(output - ref) > epsilon)
  {
    itkGenericExceptionMacro(<< "Got " << output << " while waiting for " << ref);
  }
  std::cout << " -- OK" << std::endl;
}

void otbParserTest_ThrowIfNotEqual(int output, int ref, std::string testname = "")
{
  std::cout << "Running test " << testname << std::endl;
  if (output != ref)
  {
    itkGenericExceptionMacro(<< "Got " << output << " while waiting for " << ref);
  }
  std::cout << " -- OK" << std::endl;
}

void otbParserTest_Numerical(void)
{
  ParserType::Pointer parser = ParserType::New();
  parser->SetExpr("10.0+3");
  otbParserTest_ThrowIfNotEqual(parser->Eval(), 13.0, "Numerical");
}

void otbParserTest_BuildInFun(void)
{
  ParserType::Pointer parser = ParserType::New();
  parser->SetExpr("cos(1.55)");
  otbParserTest_ThrowIfNotEqual(parser->Eval(), std::cos(1.55), "BuildInFun");
}

void otbParserTest_UserDefinedCst(void)
{
  ParserType::Pointer parser = ParserType::New();
  parser->SetExpr("ln10");
  otbParserTest_ThrowIfNotEqual(parser->Eval(), std::log(10.0), "UserDefinedCst");
}

void otbParserTest_UserDefinedFun(void)
{
  ParserType::Pointer parser = ParserType::New();
  parser->SetExpr("ndvi(100, 10)");
  otbParserTest_ThrowIfNotEqual(parser->Eval(), (10.0 - 100.0) / (10.0 + 100.0), "UserDefinedFun");
}

void otbParserTest_UserDefinedVars(void)
{
  double var1 = 10.0;
  double var2 = 2.0;
  double var3 = 7.0;
  double var4 = 100.0;
  double var5 = 200.0;

  ParserType::Pointer parser = ParserType::New();
  parser->DefineVar("var1", &var1);
  parser->DefineVar("var2", &var2);
  parser->DefineVar("var3", &var3);
  parser->DefineVar("var4", &var4);
  parser->DefineVar("var5", &var5);
  parser->SetExpr("(var1+var2-var3)*var4/var5");
  otbParserTest_ThrowIfNotEqual(parser->Eval(), (var1 + var2 - var3) * var4 / var5, "UserDefinedVars");
}

void otbParserTest_Mixed(void)
{
  ParserType::Pointer parser = ParserType::New();
  parser->SetExpr("(7+10)/2+cos(pi/4)*10-10*ln10+ndvi(100, 10)");
  otbParserTest_ThrowIfNotEqual(parser->Eval(),
                                (7.0 + 10.0) / 2.0 + std::cos(otb::CONST_PI / 4) * 10.0 - 10.0 * std::log(10.0) + (10.0 - 100.0) / (10.0 + 100.0), "Mixed");
}

void otbParserTest_LogicalOperator(void)
{
  ParserType::Pointer parser = ParserType::New();
  parser->SetExpr("1 and 0");
  otbParserTest_ThrowIfNotEqual(static_cast<int>(parser->Eval()), 0, "LogicalOperator and");
  parser->SetExpr("1 or 0");
  otbParserTest_ThrowIfNotEqual(static_cast<int>(parser->Eval()), 1, "LogicalOperator or");
}

int otbParserTest(int itkNotUsed(argc), char* itkNotUsed(argv)[])
{
  otbParserTest_Numerical();
  otbParserTest_BuildInFun();
  otbParserTest_UserDefinedCst();
  otbParserTest_UserDefinedFun();
  otbParserTest_UserDefinedVars();
  otbParserTest_Mixed();
  otbParserTest_LogicalOperator();
  return EXIT_SUCCESS;
}
