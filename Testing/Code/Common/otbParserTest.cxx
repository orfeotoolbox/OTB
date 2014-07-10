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
#include "otbMath.h"
#include "otbParser.h"

typedef otb::Parser ParserType;

int otbParserTestNew(int itkNotUsed(argc), char ** itkNotUsed(argv))
{
  // Instantiating object
  ParserType::Pointer parser = ParserType::New();
  std::cout << parser << std::endl;
  return EXIT_SUCCESS;
}

void otbParserTest_ThrowIfNotEqual(double output, double ref, std::string testname, double epsilon = 1.0E-12)
{
  std::cout << "Running test " << testname << std::endl;
  if (vcl_abs(output-ref) > epsilon)
  {
    itkGenericExceptionMacro( << "Got " << output << " while waiting for " << ref );
  }
  std::cout << " -- OK" << std::endl;
}

void otbParserTest_ThrowIfNotEqual(int output, int ref, std::string testname = "")
{
  std::cout << "Running test " << testname << std::endl;
  if (output != ref)
  {
    itkGenericExceptionMacro( << "Got " << output << " while waiting for " << ref );
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
  otbParserTest_ThrowIfNotEqual(parser->Eval(), vcl_cos(1.55), "BuildInFun");
}

void otbParserTest_UserDefinedCst(void)
{
  ParserType::Pointer parser = ParserType::New();
  parser->SetExpr("ln10");
  otbParserTest_ThrowIfNotEqual(parser->Eval(), vcl_log(10.0), "UserDefinedCst");
}

void otbParserTest_UserDefinedFun(void)
{
  ParserType::Pointer parser = ParserType::New();
  parser->SetExpr("ndvi(100, 10)");
  otbParserTest_ThrowIfNotEqual(parser->Eval(), (10.0-100.0)/(10.0+100.0), "UserDefinedFun");
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
  otbParserTest_ThrowIfNotEqual(parser->Eval(), (var1+var2-var3)*var4/var5, "UserDefinedVars");
}

void otbParserTest_Mixed(void)
{
  ParserType::Pointer parser = ParserType::New();
  parser->SetExpr("(7+10)/2+cos(pi/4)*10-10*ln10+ndvi(100, 10)");
  otbParserTest_ThrowIfNotEqual(parser->Eval(),
                                (7.0+10.0)/2.0+vcl_cos(otb::CONST_PI/4)*10.0-10.0*vcl_log(10.0)+(10.0-100.0)/(10.0+100.0),
                                "Mixed");
}

void otbParserTest_LogicalOperator(void)
{
  ParserType::Pointer parser = ParserType::New();
  parser->SetExpr("1 and 0");
  otbParserTest_ThrowIfNotEqual(static_cast<int>(parser->Eval()), 0, "LogicalOperator and");
  parser->SetExpr("1 or 0");
  otbParserTest_ThrowIfNotEqual(static_cast<int>(parser->Eval()), 1, "LogicalOperator or");
}

int otbParserTest(int itkNotUsed(argc), char ** itkNotUsed(argv))
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
