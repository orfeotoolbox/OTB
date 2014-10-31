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
#include "otbMacro.h"
#include "otbParserX.h"
#include "otbParserXPlugins.h"

namespace otb
{

class ITK_EXPORT ParserXImpl : public itk::LightObject
{
public:
  /** Standard class typedefs. */
  typedef ParserXImpl                               Self;
  typedef itk::LightObject                         Superclass;
  typedef itk::SmartPointer<Self>                  Pointer;
  typedef itk::SmartPointer<const Self>            ConstPointer;

  /** New macro for creation of through a Smart Pointer */
  itkNewMacro(Self);

  /** Run-time type information (and related methods) */
  itkTypeMacro(ParserXImpl, itk::LightObject);

  /** Convenient type definitions */
  typedef mup::Value                                ValueType;
  typedef mup::ParserError                          ExceptionType;

  /** Initialize user defined constants */
  virtual void InitConst()
  {
    
    m_MuParserX.DefineConst( "log2e",  CONST_LOG2E );
    m_MuParserX.DefineConst( "log10e", CONST_LOG10E );
    m_MuParserX.DefineConst( "ln2",    CONST_LN2 );
    m_MuParserX.DefineConst( "ln10",   CONST_LN10 );
    m_MuParserX.DefineConst( "euler",  CONST_EULER );

  }

  /** Initialize user defined functions */
  virtual void InitFun()
  {
    m_MuParserX.DefineFun(new ndvi);
    m_MuParserX.DefineFun(new conv);
    m_MuParserX.DefineFun(new bands);
    m_MuParserX.DefineFun(new cat);
    m_MuParserX.DefineFun(new vcos);
    m_MuParserX.DefineFun(new vsin);
    m_MuParserX.DefineOprt(new ElementWiseDivision);
    m_MuParserX.DefineOprt(new ElementWiseMultiplication);
  }

  /** Set the expression to be parsed */
  virtual void SetExpr(const std::string & Expression)
  {
    m_MuParserX.SetExpr(Expression);
  }

  /** Trigger the parsing */
  ValueType Eval()
  {
    //ParserX::ValueType result = 0.0;
    ValueType result = 0.0;
    try
      {
      result = m_MuParserX.Eval();
      }
    catch(ExceptionType &e)
      {
      ExceptionHandler(e);
      }
    return result;
  }


  /** Define a variable */
  void DefineVar(const std::string &sName, ValueType *fVar)
  {
    try
      {
      m_MuParserX.DefineVar(sName, mup::Variable(fVar));
      }
    catch(ExceptionType &e)
      {
      ExceptionHandler(e);
      }
  }

  /** Clear all the defined variables */
  void ClearVar()
  {
    m_MuParserX.ClearVar();
  }

  /** Return the expression to be parsed */
  const std::string& GetExpr() const
  {
    return m_MuParserX.GetExpr();
  }

  /** Return the list of variables */
  const mup::var_maptype& GetVar() const
  {
    return m_MuParserX.GetVar();
  }

  /** Return the list of expression variables (only make sense after having set up an expression) */
  const mup::var_maptype& GetExprVar() const
  {
    return m_MuParserX.GetExprVar();
  }

  /**  Check Expression **/
  bool CheckExpr()
  {
    try
    {
      m_MuParserX.Eval();
    }
   catch(ExceptionType &e)
    {
     ExceptionHandler(e);
     return false;
    }

    return true;
  }

  // Get the map with the functions
  /*ParserX::FunctionMapType GetFunList() const chris
  {
    ParserX::FunctionMapType output;
    const mu::funmap_type& funmap = m_MuParserX.GetFunDef();

    mu::funmap_type::const_iterator funItem;

    for (funItem = funmap.begin(); funItem != funmap.end(); ++funItem)
      {
      output[funItem->first] = funItem->second.GetArgc();
      }
    return output;
  }*/

  /** Convert parser specific exception into itk exception */
  virtual void ExceptionHandler(ExceptionType &e)
  {
    itkExceptionMacro(                                     << std::endl
          << "Message:     "   << e.GetMsg()   << std::endl
          << "Formula:     "   << e.GetExpr()  << std::endl
          << "Token:       "   << e.GetToken() << std::endl
          << "Position:    "   << e.GetPos()   << std::endl
                 << std::endl);
  //        << "Errc:        "   << e.GetCode()  << std::endl);
  }

  /** Convert parser specific exception into itk debug macro */
   virtual void ExceptionHandlerDebug(ExceptionType &e)
   {
     ExceptionHandler(e);
   }

protected:
  ParserXImpl()
  {
    InitFun();
    InitConst();
  }

  virtual ~ParserXImpl()
  {
  }

  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os, indent);
  }


private:
  ParserXImpl(const Self &);             //purposely not implemented
  void operator =(const Self &);    //purposely not implemented

  mup::ParserX m_MuParserX;


}; // end class



void ParserX::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}


ParserX::ParserX()
: m_InternalParserX( ParserXImpl::New() )
{
}

ParserX::~ParserX()
{
}

void ParserX::SetExpr(const std::string & Expression)
{
  m_InternalParserX->SetExpr(Expression);
}

ParserX::ValueType ParserX::Eval()
{
  return m_InternalParserX->Eval();
}

void ParserX::DefineVar(const std::string &sName, ValueType *fVar)
{
  m_InternalParserX->DefineVar(sName, fVar);
}


void ParserX::ClearVar()
{
  m_InternalParserX->ClearVar();
}

bool ParserX::CheckExpr()
{
  return m_InternalParserX->CheckExpr();
}


const std::string& ParserX::GetExpr() const
{
  return m_InternalParserX->GetExpr();
}

// Get the map with the variables
const mup::var_maptype& ParserX::GetVar() const
{
  return m_InternalParserX->GetVar();
}


/** Return the list of expression variables (only make sense after having set up an expression) */
const mup::var_maptype& ParserX::GetExprVar() const
{
   return m_InternalParserX->GetExprVar();
}


// Get the map with the functions
/*ParserX::FunctionMapType ParserX::GetFunList() const chris
{
  return m_InternalParserX->GetFunList();
}*/

}//end namespace otb
