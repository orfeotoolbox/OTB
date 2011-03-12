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
#include "itkMacro.h"
#include "otbParser.h"

namespace otb
{

void Parser::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}


//----------  User Defined Function Definitions  ----------//BEGIN
Parser::ValueType Parser::NDVI(Parser::ValueType r, Parser::ValueType niri)
{
  if ( vcl_abs(r + niri) < 1E-6 )
    {
    return 0.;
    }
  return (niri-r)/(niri+r);
}
//----------  User Defined Function Definitions  ----------//END


Parser::Parser()
{
  InitFun();
  InitConst();
}

Parser::~Parser()
{

}

void Parser::InitConst()
{
 m_InternalParser.DefineConst( "e",      CONST_E );
 m_InternalParser.DefineConst( "log2e",  CONST_LOG2E );
 m_InternalParser.DefineConst( "log10e", CONST_LOG10E );
 m_InternalParser.DefineConst( "ln2",    CONST_LN2 );
 m_InternalParser.DefineConst( "ln10",   CONST_LN10 );
 m_InternalParser.DefineConst( "pi",     CONST_PI );
 m_InternalParser.DefineConst( "euler",  CONST_EULER );
}

void Parser::InitFun()
{
  m_InternalParser.DefineFun("ndvi", NDVI);
  m_InternalParser.DefineFun("NDVI", NDVI);
}

void Parser::SetExpr(const std::string & Expression)
{
  m_InternalParser.SetExpr(Expression);
}

Parser::ValueType Parser::Eval()
{
  Parser::ValueType result = 0.0;
  try
    {
    result = m_InternalParser.Eval();
    }
  catch(ParserType::ExceptionType &e)
    {
    ExceptionHandler(e);
    }
  return result;
}

void Parser::DefineVar(const std::string &sName, Parser::ValueType *fVar)
{
  try
    {
    m_InternalParser.DefineVar(sName, fVar);
    }
  catch(ParserType::ExceptionType &e)
    {
    ExceptionHandler(e);
    }
}

void Parser::ClearVar()
{
  m_InternalParser.ClearVar();
}

const std::string& Parser::GetExpr() const
{
  return m_InternalParser.GetExpr();
}

void Parser::ExceptionHandler(ParserType::ExceptionType &e)
{
  itkExceptionMacro(                                     << std::endl
        << "Message:     "   << e.GetMsg()   << std::endl
        << "Formula:     "   << e.GetExpr()  << std::endl
        << "Token:       "   << e.GetToken() << std::endl
        << "Position:    "   << e.GetPos()   << std::endl
               << std::endl);
//        << "Errc:        "   << e.GetCode()  << std::endl);
}


}//end namespace otb
