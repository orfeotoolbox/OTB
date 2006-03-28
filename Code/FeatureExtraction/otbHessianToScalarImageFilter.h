/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P. IMBO
  Language  :   C++
  Date      :   28 mars 2006
  Version   :   
  Role      :   
  $Id$

=========================================================================*/
#ifndef __otbHessianToScalarImageFilter_h
#define __otbHessianToScalarImageFilter_h

#include "itkUnaryFunctorImageFilter.h"

namespace otb
{
  
/** \class HessianToScalarImageFilter
 * 
 */

namespace Functor {  
  
template< class TInput, class TOutput >
class HessianToScalar
{
public:
  HessianToScalar() {m_Alpha = 1.0};
  ~HessianToScalar() {};
  inline TOutput operator()( const TInput & Hessian )
  {
    
    TOutput det;
    TOutput trace;
    TOutput result;
    
    det   = static_cast<TOutput>( Hessian(0,0) * Hessian(1,1) - Hessian(0,1) * Hessian(1,0) );
    trace = static_cast<TOutput>( Hessian(0,0) + Hessian(1,1) );
    
    result = det - ( static_cast<TOutput>(m_Alpha) * trace * trace );
    
    return result;
  }

  void SetAlpha(double Alpha)
  {
    m_Alpha = Alpha;
  }
  double GetAlpha(void)const
  {
  	return (  m_Alpha ); 
  }

private:
  double  m_Alpha;  
}; 
}

template <class TInputImage, class TOutputImage>
class ITK_EXPORT HessianToScalarImageFilter :
    public itk::UnaryFunctorImageFilter<
            TInputImage,TOutputImage,
            Functor::HessianToScalar<
                   ITK_TYPENAME TInputImage::PixelType, 
		   ITK_TYPENAME TOutputImage::PixelType>   >
{
public:
  /** Standard class typedefs. */
  typedef HessianToScalarImageFilter                                      Self;
  typedef typename itk::UnaryFunctorImageFilter<
                             TInputImage,
		             TOutputImage, 
                             Functor::HessianToScalar< 
				       ITK_TYPENAME TInputImage::PixelType, 
		                       ITK_TYPENAME TOutputImage::PixelType> > Superclass;
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
  void SetAlpha(double Alpha)
  {
	this->Functor().SetAlpha( Alpha );
  }  
  double GetAlpha(void)const
  {
	return( this->Functor().GetAlpha() );
  }  
protected:
  HessianToScalarImageFilter() {}
  virtual ~HessianToScalarImageFilter() {}

private:
  HessianToScalarImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  
};

} // end namespace otb


#endif
