/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P. IMBO
  Language  :   C++
  Date      :   28 mars 2006
  Version   :   
  Role      :   
  $Id$

=========================================================================*/
#ifndef __otbMultiplyByScalarImageFilter_h
#define __otbMultiplyByScalarImageFilter_h

#include "itkUnaryFunctorImageFilter.h"

namespace otb
{
  
/** \class MultiplyByScalarImageFilter
 * 
 */

namespace Functor {  
  
template< class TInput, class TOutput >
class MultiplyByScalar
{
public:
  MultiplyByScalar() {m_Coef = 1.0;}
  ~MultiplyByScalar() {};
  inline TOutput operator()( const TInput & value )
  {
    TOutput result;
       
    result = static_cast<TOutput>( m_Coef * value );
    
    return result;
  }

  void SetCoef(double Coef)
  {
    m_Coef = Coef;
  }
  double GetCoef(void)const
  {
  	return (  m_Coef ); 
  }

private:
  double  m_Coef;  
}; 
}

template <class TInputImage, class TOutputImage>
class ITK_EXPORT MultiplyByScalarImageFilter :
    public itk::UnaryFunctorImageFilter<
            TInputImage,TOutputImage,
            Functor::MultiplyByScalar<
                   ITK_TYPENAME TInputImage::PixelType, 
		   ITK_TYPENAME TOutputImage::PixelType>   >
{
public:
  /** Standard class typedefs. */
  typedef MultiplyByScalarImageFilter                                      Self;
  typedef typename itk::UnaryFunctorImageFilter<
                             TInputImage,
		             TOutputImage, 
                             Functor::MultiplyByScalar< 
				       ITK_TYPENAME TInputImage::PixelType, 
		                       ITK_TYPENAME TOutputImage::PixelType> > Superclass;
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
  void SetCoef(double Coef)
  {
	this->GetFunctor().SetCoef( Coef );
  }  
  double GetCoef(void)const
  {
	return( this->GetFunctor().GetCoef() );
  }  
protected:
  MultiplyByScalarImageFilter() {}
  virtual ~MultiplyByScalarImageFilter() {}

private:
  MultiplyByScalarImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  
};

} // end namespace otb


#endif
