/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P. Imbo
  Language  :   C++
  Date      :   24 mars 2006
  Version   :   
  Role      :   Flusser's invariant Class of path 
  $Id$

=========================================================================*/
#ifndef _otbFlusserPathFunction_h
#define _otbFlusserPathFunction_h

#include "otbRealMomentPathFunction.h"

namespace otb
{

/**
 * \class FlusserImageFunction
 * \brief Calculate the Flusser's invariant parameters.
 *
 * Calculate the Flusser's invariant over a path and defined as:
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
 *  \f[  c_{p,q}=\int \int (x+iy)^{p} \cdot (x-iy)^{q} \cdot f(x,y) \cdot
 dx \cdot dy \f]
 *
 * And:
 *  - \f$(x,y)\f$ pixel localization;
 *  - \f$ f(x,y)\f$ the pixel value over the \f$(x,y)\f$ coordinate.
 *
 * This class is templated over the input image type and the
 * coordinate representation type (e.g. float or double).
 * 
 * \ingroup PathFunctions
 */

template < class TInputImage, 
           class TInputPath,    
           class TOutput      = double>
class ITK_EXPORT FlusserPathFunction :
  public RealMomentPathFunction< TInputImage,TInputPath, TOutput >
{
public:
  /** Standard class typedefs. */
  typedef FlusserPathFunction                                       Self;
  typedef RealMomentPathFunction<TInputImage,TInputPath, TOutput>   Superclass;
  typedef itk::SmartPointer<Self>                                   Pointer;
  typedef itk::SmartPointer<const Self>                             ConstPointer;
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(FlusserPathFunction, RealMomentPathFunction);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** InputImageType typedef support. */
  typedef typename Superclass::ImageType            ImageType;
  typedef typename Superclass::IndexType            IndexType;
  typedef typename Superclass::ContinuousIndexType  ContinuousIndexType;
  typedef typename Superclass::PointType            PointType;

  /** InputPathType typedef support. */
  typedef typename Superclass::PathType                 PathType;
  typedef typename Superclass::PathConstPointer         PathConstPointer;  
  typedef typename PathType::ContinuousIndexType        VertexType;
  typedef itk::VectorContainer< unsigned,VertexType >   VertexListType;
  typedef typename VertexListType::ConstPointer         VertexListPointer;

  typedef typename Superclass::RealType                 RealType;
   
  			 

  /** Evaluate the function at non-integer positions */
  virtual RealType Evaluate( const PathType& path) const;
  /** Get/Set the radius of the neighborhood over which the
      statistics are evaluated */  
  itkSetMacro(Number,short);
  itkGetConstReferenceMacro( Number, short );

protected:
  FlusserPathFunction();
  ~FlusserPathFunction(){};
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  FlusserPathFunction( const Self& ); //purposely not implemented
  void operator=( const Self& ); //purposely not implemented

  short m_Number;  
};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbFlusserPathFunction.txx"
#endif

#endif
