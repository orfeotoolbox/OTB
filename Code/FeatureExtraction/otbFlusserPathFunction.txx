/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P. Imbo
  Language  :   C++
  Date      :   24 mars 2006
  Version   :   
  Role      :   Flusser's invariant Class of path 
  $Id:$

=========================================================================*/
#ifndef _otbFlusserPathFunction_txx
#define _otbFlusserPathFunction_txx

#include "otbFlusserPathFunction.h"
#include "otbComplexMomentPathFunction.h"
#include "itkNumericTraits.h"

namespace otb
{

/**
   * Constructor
   */
template < class TInputPath, class TOutput>
FlusserPathFunction<TInputPath, TOutput >
::FlusserPathFunction()
{
  //OTB-FA-00024-CS
  m_MomentNumber =-1; 
}

/**
   *
   */
template < class TInputPath, class TOutput>
void
FlusserPathFunction< TInputPath, TOutput >
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os,indent);
  //OTB-FA-00024-CS
  os << indent << " m_MomentNumber           : "  << m_MomentNumber << std::endl;
}


template < class TInputPath, class TOutput>
typename FlusserPathFunction<TInputPath, TOutput>::RealType
FlusserPathFunction<TInputPath, TOutput >
::Evaluate( const PathType& path) const
{
  typedef ComplexMomentPathFunction<PathType>     FunctionType;
  typedef typename FunctionType::ComplexType      ComplexType;

  RealType                            FlusserValue;
  ComplexType                         FlusserValueComplex;

  typename FunctionType::Pointer function =FunctionType::New();
 
  function->SetStep( this->GetStep() );
  //OTB-FA-00023-CS
  function->SetInputPath( this->GetInputPath() );
  
  //OTB-FA-00024-CS
  switch(m_MomentNumber)
    {
    case 1 : 
        {
	ComplexType C11;
	function->SetP(1);
	function->SetQ(1);
	C11 = function->Evaluate( );
        FlusserValue = C11.real() ;
	}
	break;
    case 2:
        {
	ComplexType C21,C12;
	function->SetP(2);
	function->SetQ(1);
	C21 = function->Evaluate( );
	function->SetP(1);
	function->SetQ(2);
	C12 = function->Evaluate(  );

	FlusserValue = abs( C21 * C12 ) ;
	}
	break;
    case 3:
        {
	ComplexType C20,C12;
	function->SetP(2);
	function->SetQ(0);
	C20 = function->Evaluate( );
	function->SetP(1);
	function->SetQ(2);
	C12 = function->Evaluate( );
	FlusserValueComplex = C20 * pow(C12,2);
	FlusserValue = FlusserValueComplex.real();
	}
	break;
    case 4:
        {
	ComplexType C20,C12;
	function->SetP(2);
	function->SetQ(0);
	C20 = function->Evaluate( );
	function->SetP(1);
	function->SetQ(2);
	C12 = function->Evaluate( );
	FlusserValueComplex = C20 * pow(C12,2);
	FlusserValue = FlusserValueComplex.imag();
	}
	break;
    case 5:
        {
	ComplexType C30,C12;
	function->SetP(3);
	function->SetQ(0);
	C30 = function->Evaluate( );
	function->SetP(1);
	function->SetQ(2);
	C12 = function->Evaluate( );

	FlusserValueComplex = C30 * pow(C12,3) ;
	FlusserValue = FlusserValueComplex.real();       
	}	
	break;
    case 6:
        {
	ComplexType C30,C12;
	function->SetP(3);
	function->SetQ(0);
	C30 = function->Evaluate( );
	function->SetP(1);
	function->SetQ(2);
	C12 = function->Evaluate( );

	FlusserValueComplex = C30 * pow(C12,3) ;
	FlusserValue = FlusserValueComplex.real();       
	}	
	break;
    case 7 : 
        {
	ComplexType C22;
	function->SetP(2);
	function->SetQ(2);
	C22 = function->Evaluate( );
        FlusserValue = C22.real() ;
	}
	break;
    case 8:
        {
	ComplexType C31,C12;
	function->SetP(3);
	function->SetQ(1);
	C31 = function->Evaluate( );
	function->SetP(1);
	function->SetQ(2);
	C12 = function->Evaluate( );
	FlusserValueComplex = C31 * pow(C12,2);
	FlusserValue = FlusserValueComplex.real();
	}
	break;
    case 9:
        {
	ComplexType C31,C12;
	function->SetP(3);
	function->SetQ(1);
	C31 = function->Evaluate( );
	function->SetP(1);
	function->SetQ(2);
	C12 = function->Evaluate( );
	FlusserValueComplex = C31 * pow(C12,2);
	FlusserValue = FlusserValueComplex.imag();
	}
	break;
    case 10:
        {
	ComplexType C40,C12;
	function->SetP(4);
	function->SetQ(0);
	C40 = function->Evaluate( );
	function->SetP(1);
	function->SetQ(2);
	C12 = function->Evaluate( );
	FlusserValueComplex = C40 * pow(C12,4);
	FlusserValue = FlusserValueComplex.real();
	}
	break;
    case 11:
        {
	ComplexType C40,C12;
	function->SetP(4);
	function->SetQ(0);
	C40 = function->Evaluate( );
	function->SetP(1);
	function->SetQ(2);
	C12 = function->Evaluate( );
	FlusserValueComplex = C40 * pow(C12,4);
	FlusserValue = FlusserValueComplex.imag();
	}
	break;
	
    default:
	itkWarningMacro("Flusser's invariant parameters are between 1 and 11");	
    }


  return (static_cast<RealType>(FlusserValue) );

}

template < class TInputPath, class TOutput>
typename FlusserPathFunction<TInputPath, TOutput>::RealType
FlusserPathFunction<TInputPath, TOutput >
::Evaluate() const
{
  //OTB-FA-00022-CS
  if( !this->GetInputPath() )
    {
    std::cout << "Pb with GetInputPath" << std::endl;
    return static_cast<RealType>( itk::NumericTraits<float>::max());
    }

  RealType Result =  Evaluate( *(this->GetInputPath()) );
  
  return Result;

}


} // namespace otb

#endif
