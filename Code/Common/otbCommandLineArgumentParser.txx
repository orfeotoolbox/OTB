#ifndef _otbCommandLineArgumentParser_txx
#define _otbCommandLineArgumentParser_txx

#include "otbCommandLineArgumentParser.h"

#include "itkMacro.h"

namespace otb
{


template< typename TypeValeur >
TypeValeur
CommandLineArgumentParseResult
::GetOptionParameterNumericValue(const char *option, unsigned int number)const
{
  const char * parametre = this->GetOptionParameter(option, number);
  TypeValeur lValeur;
  ::itk::OStringStream flux;
  flux << parametre;
  flux >> lValeur;
  return lValeur;
}


} // end namespace otb

#endif
