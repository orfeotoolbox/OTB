#ifndef _otbCommandLineArgumentParser_txx
#define _otbCommandLineArgumentParser_txx

#include "otbCommandLineArgumentParser.h"

#include "itkMacro.h"

namespace otb
{


template< typename TypeValeur >
TypeValeur
CommandLineArgumentParseResult
::GetParameter(const char *option, unsigned int number)const
{
  std::string parametre = this->GetStringParameter(option, number);
  TypeValeur lValeur;
  ::itk::OStringStream flux;
//  flux << parametre;
//  flux >> lValeur;
  return lValeur;
}


} // end namespace otb

#endif
