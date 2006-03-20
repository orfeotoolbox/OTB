/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P. Imbo
  Language  :   C++
  Date      :   20 mars 2006
  Version   :   
  Role      :   Flusser's invariant Class of iamges 
  $Id:$

=========================================================================*/
#ifndef _otbFlusserImageFunction_h
#define _otbFlusserImageFunction_h

#include "otbRealMomentImageFunction.h"

#include <complex>


namespace otb
{

/**
 * \class FlusserImageFunction
 * \brief Calculate the Flusser's invariant parameters.
 *
 * Calculate the Flusser's invariant over an image defined as:
 *
 * - \f$ \psi_{1} = c_{11} \f$
 * - \f$ \psi_{2} = c_{21} c_{12} \f$
 * - \f$ \psi_{3} = Re (c_{20} c_{12}^{2} )\f$
 * - \f$ \psi_{4} = Im (c_{20} c_{12}^{2} )\f$
 * - \f$ \psi_{5} = Re (c_{30} c_{12}^{3} )\f$
 * - \f$ \psi_{6} = Im (c_{30} c_{12}^{3} )\f$
 * - \f$ \psi_{7} = c_{22} \f$
 * - \f$ \psi_{8} = Re (c_{31} c_{12}^{2} )\f$
 * - \f$ \psi_{9} = Im (c_{31} c_{12}^{2} )\f$
 * - \f$ \psi_{10} = Re (c_{40} c_{12}^{4} )\f$
 * - \f$ \psi_{11} = Im (c_{40} c_{12}^{4} )\f$
 *  
 * With :
 *
 *  \f[  c_{p,q}=\int_{-\infty}^{\infty} \int_{-\infty}^{\infty} (x+iy)^{p} \cdot (x-iy)^{q} \cdot f(x,y) \cdot
 dx \cdot dy \f]
 *
 * And:
 *  - \f$(x,y)\f$ pixel localization;
 *  - \f$ f(x,y)\f$ the pixel value over the \f$(x,y)\f$ coordinate.
 *
 * This class is templated over the input image type and the
 * coordinate representation type (e.g. float or double).
 * 
 * \ingroup ImageFunctions
 */

template < class TInput, 
           class TOutput   = double,
	   class TCoordRep = float >
class ITK_EXPORT FlusserImageFunction :
  public RealMomentImageFunction< TInput, TOutput,TCoordRep >
{
public:
  /** Standard class typedefs. */
  typedef FlusserImageFunction                                      Self;
  typedef RealMomentImageFunction< TInput, TOutput,TCoordRep >      Superclass;
  typedef itk::SmartPointer<Self>                                   Pointer;
  typedef itk::SmartPointer<const Self>                             ConstPointer;
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(FlusserImageFunction, RealMomentImageFunction);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** InputImageType typedef support. */
  typedef TInput                                    InputType;
  typedef typename Superclass::IndexType            IndexType;
  typedef typename Superclass::ContinuousIndexType  ContinuousIndexType;
  typedef typename Superclass::PointType            PointType;

  typedef TOutput                                   RealType;
  typedef typename std::complex<RealType>           ComplexType;
   
  /** Dimension of the underlying image. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      InputType::ImageDimension);
  			 

  /** Evalulate the function at specified index */
  virtual RealType EvaluateAtIndex( const IndexType& index ) const;
  
  /** Evaluate the function at non-integer positions */
  virtual RealType Evaluate( const PointType& point ) const
    { 
      IndexType index;
      this->ConvertPointToNearestIndex( point, index );
      return this->EvaluateAtIndex( index ); 
    }
  virtual RealType EvaluateAtContinuousIndex( 
    const ContinuousIndexType& cindex ) const
    { 
      IndexType index;
      this->ConvertContinuousIndexToNearestIndex( cindex, index );
      return this->EvaluateAtIndex( index ) ; 
    }

  /** Get/Set the radius of the neighborhood over which the
      statistics are evaluated */  
  itkSetClampMacro(Number,short,1,11);
  itkGetConstReferenceMacro( Number, short );

protected:
  FlusserImageFunction();
  ~FlusserImageFunction(){};
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  FlusserImageFunction( const Self& ); //purposely not implemented
  void operator=( const Self& ); //purposely not implemented

  short m_Number;  
};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbFlusserImageFunction.txx"
#endif

#endif
