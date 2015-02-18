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
#ifndef __otbParser_h
#define __otbParser_h

#include "itkLightObject.h"
#include "itkObjectFactory.h"

namespace otb
{

class ParserImpl;


/** \class Parser
 * \brief  Definition of the standard floating point parser.
 * Standard implementation of the mathematical expressions parser.
 *
 * \sa BandMathImageFilter
 *
 */
class ITK_EXPORT Parser : public itk::LightObject
{
public:
  /** Standard class typedefs. */
  typedef Parser                                   Self;
  typedef itk::LightObject                         Superclass;
  typedef itk::SmartPointer<Self>                  Pointer;
  typedef itk::SmartPointer<const Self>            ConstPointer;

  /** New macro for creation of through a Smart Pointer */
  itkNewMacro(Self);

  /** Run-time type information (and related methods) */
  itkTypeMacro(Parser, itk::LightObject);

  /** Convenient type definitions */
  typedef Parser                                   ParserType;
  typedef double                                   ValueType;

  /** Type for function/number of arguments map */
  typedef std::map<std::string, int>               FunctionMapType;

  /** Set the expression to be parsed */
  virtual void SetExpr(const std::string & Expression);

  /** Trigger the parsing */
  ValueType Eval();

  /** Define a variable */
  void DefineVar(const std::string &sName, ValueType *fVar);

  /** Clear all the defined variables */
  void ClearVar();

  /** Return the expression to be parsed */
  const std::string& GetExpr() const;

  /** Return the list of variables */
  const std::map<std::string, Parser::ValueType*>& GetVar() const;

  /** Return a map of function names and associated number of arguments */
  FunctionMapType GetFunList() const;

  /**  Check Expression **/
  bool CheckExpr();

protected:
  Parser();
  virtual ~Parser();
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;


private:
  Parser(const Self &);             //purposely not implemented
  void operator =(const Self &);    //purposely not implemented

  typedef itk::SmartPointer<ParserImpl> ParserImplPtr;
  ParserImplPtr m_InternalParser;
}; // end class

}//end namespace otb

#endif
