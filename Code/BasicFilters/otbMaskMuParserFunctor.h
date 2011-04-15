/*=========================================================================

 Program:   ORFEO Toolbox
 Language:  C++
 Date:      $Date$
 Version:   $Revision$


 Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
 See OTBCopyright.txt for details.

 Some parts of this code are derived from ITK. See ITKCopyright.txt
 for details.


 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE.  See the above copyright notices for more information.

 =========================================================================*/

#ifndef __otbMaskMuParserFunctor_h
#define __otbMaskMuParserFunctor_h

#include "otbParser.h"
#include "otbMacro.h"
#include "itkMacro.h"


#include "itkLightObject.h"
#include "itkObjectFactory.h"


namespace otb
{
/** \class MaskMuParserFunctor
 * \brief functor used to create binary mask for input of connected component segmentation module.
 *
 * This functor is based on the mathematical parser library muParser.
 * The built in functions and operators list is available at:
 * http://muparser.sourceforge.net/mup_features.html#idDef2
 *
 * OTB additional functions:
 * ndvi(r, niri)
 *
 * OTB additional constants:
 * e - log2e - log10e - ln2 - ln10 - pi - euler
 *
 * \sa Parser
 *
 */

/** \class MaskMuParserFunctor
 *  \brief This functor use MuParser to evaluate and process mathematical expression
 */
namespace Functor
{

template<class TInput>
class ITK_EXPORT MaskMuParserFunctor : public itk::LightObject
{
public:
typedef Parser ParserType;
typedef MaskMuParserFunctor Self;
typedef itk::LightObject                         Superclass;
typedef itk::SmartPointer<Self>                  Pointer;
typedef itk::SmartPointer<const Self>            ConstPointer;
/** Method for creation through the object factory. */
itkNewMacro(Self);

/** Run-time type information (and related methods). */
itkTypeMacro(MaskMuParserFunctor, itk::LightObject);

inline bool operator()(const TInput &p)
{

  double value;

  if(p.Size() !=m_NbOfBands)
    {
    this->SetNumberOfBands(p.GetSize());
    }

  // we fill the buffer
  for(unsigned int i=0; i<m_NbOfBands; i++)
    {
    m_AImage[i]= static_cast<double> (p[i]);
    }

  // user defined variables
  m_Intensity = 0.0;
  for(unsigned int i = 0; i<m_NbOfBands; ++i)
    {
    m_Intensity +=p[i];
    }
  m_Intensity  = m_Intensity/(static_cast<double> (m_NbOfBands));

  try
  {
    value = m_Parser->Eval();
  }
  catch(itk::ExceptionObject& err)
  {
    itkExceptionMacro(<< err);
  }
  return static_cast<bool> (value);

}

void SetExpression(const std::string expression)
{
  m_Expression=expression;
  m_Parser->SetExpr(m_Expression);
}

/** Return the expression to be parsed */
std::string GetExpression() const
{
  return m_Expression;
}

//void SetVarName(unsigned int idx, const std::string varName)
//{//
// m_VarName[idx] = varName;
// m_Parser->DefineVar(m_VarName[idx], &(m_AImage[idx]));
//}

//std::string GetVarName(unsigned int idx)
//{
//  return m_VarName[idx];
//}

void SetNumberOfBands(unsigned int NbOfBands)
{

  m_NbOfBands=NbOfBands;
  std::ostringstream varName;

  m_AImage.resize(NbOfBands, 0.0);

  for(unsigned int i=0; i<NbOfBands; i++)
    {
    varName << "b" << i+1;
    m_Parser->DefineVar(varName.str(), &(m_AImage[i]));
    varName.str("");
    }
  // customized data
  //m_NbVar++;
  //this->SetDataSize(m_NbVar);
  m_Parser->DefineVar("intensity", &m_Intensity);
}



/** Check the expression */
bool CheckExpression()
{

  for(unsigned int i=0; i<m_NbOfBands; i++)
    {
    m_AImage[i]=0;
    }

  m_Intensity=0.0;

  try
  {
    m_Parser->Eval();

  }
  catch(itk::ExceptionObject& err)
  {
    itkExceptionMacro(<< err);
    return false;
  }

  return true;
}

protected:
MaskMuParserFunctor()
{
m_Parser = ParserType::New();
m_NbOfBands=0;
};

~MaskMuParserFunctor()
{};


private:

MaskMuParserFunctor(const Self &); //purposely not implemented
void operator =(const Self &);    //purposely not implemented

std::string m_Expression;
ParserType::Pointer m_Parser;
std::vector<double> m_AImage;
//std::vector<std::string > m_VarName;
unsigned int m_NbOfBands;
//unsigned int m_NbVar;
double m_ParserResult;

//user defined variables
double m_Intensity;

};
} // end of Functor namespace


}//end namespace otb


#endif
