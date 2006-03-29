/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P. Imbo
  Language  :   C++
  Date      :   24 mars 2006
  Version   :   
  Role      :   Hu's invariant Class of path 
  $Id:$

=========================================================================*/
#ifndef _otbHuPathFunction_txx
#define _otbHuPathFunction_txx

#include "otbHuPathFunction.h"
#include "otbComplexMomentPathFunction.h"
#include "itkNumericTraits.h"
#include "itkMacro.h"

namespace otb
{

/**
   * Constructor
   */
template < class TInputPath, class TOutput>
HuPathFunction< TInputPath, TOutput >
::HuPathFunction()
{
  m_Number =-1; 
}

/**
   *
   */
template < class TInputPath, class TOutput>
void
HuPathFunction< TInputPath, TOutput >
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os,indent);
  os << indent << " m_Number           : "  << m_Number << std::endl;
}


template < class TInputPath, class TOutput>
typename HuPathFunction<TInputPath, TOutput >::RealType
HuPathFunction<TInputPath, TOutput >
::Evaluate( const PathType& path) const
{
  typedef ComplexMomentPathFunction<PathType>   FunctionType;
  typedef typename FunctionType::ComplexType    ComplexType;
  
  RealType                         HuValue;
  ComplexType                      HuValueComplex;

  typename FunctionType::Pointer function =FunctionType::New();
 
  function->SetStep( this->GetStep() );

  switch(m_Number)
    {
    case 1 : 
        {
	ComplexType C11;
	function->SetP(1);
	function->SetQ(1);
	C11 = function->Evaluate( path );
        HuValue = C11.real() ;
	}
	break;
    case 2:
        {
	ComplexType C20,C02;
	function->SetP(2);
	function->SetQ(0);
	C20 = function->Evaluate( path );
	function->SetP(0);
	function->SetQ(2);
	C02 = function->Evaluate( path );

	HuValue = abs( C20 * C02 ) ;

	}
	break;
    case 3:
        {
	ComplexType C30,C03;
	function->SetP(3);
	function->SetQ(0);
	C30 = function->Evaluate( path );
	function->SetP(0);
	function->SetQ(3);
	C03 = function->Evaluate( path );

	HuValue = abs( C30 * C03 );
	}
	break;
    case 4:
        {
	ComplexType C21,C12;
	function->SetP(2);
	function->SetQ(1);
	C21 = function->Evaluate( path );
	function->SetP(1);
	function->SetQ(2);
	C12 = function->Evaluate( path );

	HuValue = abs( C21 * C12 );
	}	
	break;

    case 5:
        {
	ComplexType C30,C12;
	function->SetP(3);
	function->SetQ(0);
	C30 = function->Evaluate( path );
	function->SetP(1);
	function->SetQ(2);
	C12 = function->Evaluate( path );

	HuValueComplex = C30 * pow(C12,3) ;
	HuValue = HuValueComplex.real();       
	}	
	break;

    case 6:
        {
	ComplexType C20,C12;
	function->SetP(2);
	function->SetQ(0);
	C20 = function->Evaluate( path );
	function->SetP(1);
	function->SetQ(2);
	C12 = function->Evaluate( path );

	HuValueComplex = C20 * pow( C12 ,2 );
	HuValue = HuValueComplex.real();         
	}	
	break;

    case 7:
        {
	ComplexType C30,C12;
	function->SetP(3);
	function->SetQ(0);
	C30 = function->Evaluate( path );
	function->SetP(1);
	function->SetQ(2);
	C12 = function->Evaluate( path );

	HuValueComplex = C30 * pow( C12 , 3);
	HuValue = HuValueComplex.imag();         
	}	
	break;
	
    default:
	itkWarningMacro("Hu's invariant parameters are between 1 and 7");	
    }

  return (static_cast<RealType>(HuValue) );

}


} // namespace otb

#endif
