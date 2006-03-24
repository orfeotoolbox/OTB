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
#include "itkMacro.h"
#include <complex>

namespace otb
{

/**
   * Constructor
   */
template < class TInputImage, class TInputPath, class TOutput>
FlusserPathFunction<TInputImage,TInputPath, TOutput >
::FlusserPathFunction()
{
  m_Number =-1; 
}

/**
   *
   */
template < class TInputImage, class TInputPath, class TOutput>
void
FlusserPathFunction<TInputImage,TInputPath, TOutput >
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os,indent);
  os << indent << " m_Number           : "  << m_Number << std::endl;
}


template < class TInputImage, class TInputPath, class TOutput>
typename FlusserPathFunction<TInputImage,TInputPath, TOutput>::RealType
FlusserPathFunction<TInputImage,TInputPath, TOutput >
::Evaluate( const PathType& path) const
{
  typename InputType::SizeType        ImageSize;
  RealType                            FlusserValue;
  ComplexType                         FlusserValueComplex;

  typedef otb::ComplexMomentImageFunction<InputType,ComplexType>   CMType;
  typename CMType::Pointer function =CMType::New();

  if( !this->GetInputImage() )
    {
    return ( itk::NumericTraits<RealType>::max() );
    }
  
  assert(m_Number > 0);
  assert(m_Number < 12);
	
   function->SetInputImage( this->GetInputImage() );


  switch(m_Number)
    {
    case 1 : 
        {
	ComplexType C11;
	function->SetP(1);
	function->SetQ(1);
	C11 = function->Evaluate( path );
        FlusserValue = C11.real() ;
	}
	break;
    case 2:
        {
	ComplexType C21,C12;
	function->SetP(2);
	function->SetQ(1);
	C21 = function->Evaluate( path );
	function->SetP(1);
	function->SetQ(2);
	C12 = function->Evaluate( path );

	FlusserValue = abs( C21 * C12 ) ;
	}
	break;
    case 3:
        {
	ComplexType C20,C12;
	function->SetP(2);
	function->SetQ(0);
	C20 = function->Evaluate( path );
	function->SetP(1);
	function->SetQ(2);
	C12 = function->Evaluate( path );
	FlusserValueComplex = C20 * pow(C12,2);
	FlusserValue = FlusserValueComplex.real();
	}
	break;
    case 4:
        {
	ComplexType C20,C12;
	function->SetP(2);
	function->SetQ(0);
	C20 = function->Evaluate( path );
	function->SetP(1);
	function->SetQ(2);
	C12 = function->Evaluate( path );
	FlusserValueComplex = C20 * pow(C12,2);
	FlusserValue = FlusserValueComplex.imag();
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

	FlusserValueComplex = C30 * pow(C12,3) ;
	FlusserValue = FlusserValueComplex.real();       
	}	
	break;
    case 6:
        {
	ComplexType C30,C12;
	function->SetP(3);
	function->SetQ(0);
	C30 = function->Evaluate( path );
	function->SetP(1);
	function->SetQ(2);
	C12 = function->Evaluate( path );

	FlusserValueComplex = C30 * pow(C12,3) ;
	FlusserValue = FlusserValueComplex.real();       
	}	
	break;
    case 7 : 
        {
	ComplexType C22;
	function->SetP(2);
	function->SetQ(2);
	C22 = function->Evaluate( path );
        FlusserValue = C22.real() ;
	}
	break;
    case 8:
        {
	ComplexType C31,C12;
	function->SetP(3);
	function->SetQ(1);
	C31 = function->Evaluate( path );
	function->SetP(1);
	function->SetQ(2);
	C12 = function->Evaluate( path );
	FlusserValueComplex = C31 * pow(C12,2);
	FlusserValue = FlusserValueComplex.real();
	}
	break;
    case 9:
        {
	ComplexType C31,C12;
	function->SetP(3);
	function->SetQ(1);
	C31 = function->Evaluate( path );
	function->SetP(1);
	function->SetQ(2);
	C12 = function->Evaluate( path );
	FlusserValueComplex = C31 * pow(C12,2);
	FlusserValue = FlusserValueComplex.imag();
	}
	break;
    case 10:
        {
	ComplexType C40,C12;
	function->SetP(4);
	function->SetQ(0);
	C40 = function->Evaluate( path );
	function->SetP(1);
	function->SetQ(2);
	C12 = function->Evaluate( path );
	FlusserValueComplex = C40 * pow(C12,4);
	FlusserValue = FlusserValueComplex.real();
	}
	break;
    case 11:
        {
	ComplexType C40,C12;
	function->SetP(4);
	function->SetQ(0);
	C40 = function->Evaluate( path );
	function->SetP(1);
	function->SetQ(2);
	C12 = function->Evaluate( path );
	FlusserValueComplex = C40 * pow(C12,4);
	FlusserValue = FlusserValueComplex.imag();
	}
	break;
	
    default:
	itkWarningMacro("Hu's invariant parameters are between 1 and 7");	
    }


  return (static_cast<RealType>(FlusserValue) );

}


} // namespace otb

#endif
