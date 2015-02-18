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
#ifndef __otbParserX_h
#define __otbParserX_h

#include "itkLightObject.h"
#include "itkObjectFactory.h"
#include "mpParser.h"

namespace otb
{

class ParserXImpl;


/** \class ParserX
 * \brief  Definition of the standard floating point parser.
 * Standard implementation of the mathematical expressions parser.
 *
 * \sa BandMathImageFilter
 *
 */
class ITK_EXPORT ParserX : public itk::LightObject
{
public:
  /** Standard class typedefs. */
  typedef ParserX                                   Self;
  typedef itk::LightObject                         Superclass;
  typedef itk::SmartPointer<Self>                  Pointer;
  typedef itk::SmartPointer<const Self>            ConstPointer;

  /** New macro for creation of through a Smart Pointer */
  itkNewMacro(Self);

  /** Run-time type information (and related methods) */
  itkTypeMacro(ParserX, itk::LightObject);

  /** Convenient type definitions */
  typedef ParserX                                   ParserXType;
  typedef mup::Value                                ValueType;

  /** Set the expression to be parsed */
  virtual void SetExpr(const std::string & Expression);

  /** Trigger the parsing */
  ValueType Eval();

  /** Define a variable */
  void DefineVar(const std::string &sName, ValueType *fVar);

  /** Define a constant */
  //void DefineConst(const std::string &sName, ValueType *fVar);

  /** Clear all the defined variables */
  void ClearVar();

  /** Return the expression to be parsed */
  const std::string& GetExpr() const;

  /** Return the list of variables */
  const mup::var_maptype& GetVar() const;
 
  /** Return the list of expression variables (only make sense after having set up an expression) */
  const mup::var_maptype& GetExprVar() const;

  /**  Check Expression **/
  bool CheckExpr();

protected:
  ParserX();
  virtual ~ParserX();
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;


private:
  ParserX(const Self &);             //purposely not implemented
  void operator =(const Self &);    //purposely not implemented

  typedef itk::SmartPointer<ParserXImpl> ParserXImplPtr;
  ParserXImplPtr m_InternalParserX;
}; // end class

}//end namespace otb

#endif
