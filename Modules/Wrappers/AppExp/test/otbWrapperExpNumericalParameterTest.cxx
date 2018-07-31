/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#include "otbWrapperExpNumericalParameter.h"
#include <complex>

using namespace otb::WrapperExp;

int NumericalInstantiationTest( int  , char **  )
{
  NumericalParameter< int >::Pointer intParam = NumericalParameter< int >::New();
  NumericalParameter< double >::Pointer doubleParam = NumericalParameter< double >::New();
  // NumericalParameter< std::complex<int> >::Pointer complexParam = NumericalParameter< std::complex<int> >::New();
  return EXIT_SUCCESS;
}

template < typename T >
void PrintMinMaxDef( const typename NumericalParameter<T>::Pointer param, 
                     std::ostringstream & oss)
{
  T min = param->GetMinimumTypeValue();
  oss<<"min : "<<min<<std::endl;
  T max = param->GetMaximumTypeValue();
  oss<<"max : "<<max<<std::endl;
  T def = param->GetDefaultTypeValue();
  oss<<"def : "<<def<<std::endl;
}

#define NumericalManipulation( type ) \
{\
  NumericalParameter< type >::Pointer param = NumericalParameter<type>::New(); \
  std::ostringstream oss; \
  oss<<"First round :"<<std::endl;\
  PrintMinMaxDef<type>( param , oss );\
  type val = param->GetIntegerValue(); \
  oss<<"val : "<<val<<std::endl; \
  float fval = param->GetFloattingPointValue(); \
  oss<<"fval : "<<fval<<std::endl; \
  std::string sval = param->GetValue(); \
  oss<<"sval : "<<sval<<std::endl; \
  oss<<"has value : "<<param->HasValue()<<std::endl; \
  oss<<"Manipulation"<<std::endl; \
  param->SetMinimumValue( -1.5 ); \
  PrintMinMaxDef<type>( param , oss );\
  param->SetMaximumValue( -6 ); \
  PrintMinMaxDef<type>( param , oss );\
  param->SetDefaultValue( -150 ); \
  PrintMinMaxDef<type>( param , oss );\
  param->SetMinimumValue( 5 ); \
  PrintMinMaxDef<type>( param , oss );\
  param->SetMinimumValue( -100); \
  PrintMinMaxDef<type>( param , oss );\
  param->SetMaximumValue( 100 ); \
  PrintMinMaxDef<type>( param , oss );\
  param->SetDefaultValue( 0 ); \
  PrintMinMaxDef<type>( param , oss );\
  oss<<"has value : "<<param->HasValue()<<std::endl; \
  std::cout<<oss.str();\
}\

int NumericalManipulationTest( int , char ** )
{
  NumericalParameter< int >::Pointer param = NumericalParameter<int>::New();
  int min = param->GetMinimumTypeValue();
  int max = param->GetMaximumTypeValue();
  int def = param->GetDefaultTypeValue();
  int val = param->GetIntegerValue();
  float fval = param->GetFloattingPointValue();
  std::string sval = param->GetValue();
  bool has_val = param->HasValue();
  param->SetMinimumValue( -1.5 );
  min = param->GetMinimumTypeValue();
  param->SetMaximumValue( -6 );
  max = param->GetMaximumTypeValue();
  param->SetMinimumValue( -100);
  param->SetMaximumValue( 100 );
  param->SetDefaultValue( -150 );
  def = param->GetDefaultTypeValue();
  NumericalManipulation( int )
  NumericalManipulation( double )

  return EXIT_SUCCESS;
}
