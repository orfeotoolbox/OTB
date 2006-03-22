/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P. Imbo
  Language  :   C++
  Date      :   22 mars 2006
  Version   :   
  Role      :   Complex Geometric Moments Class of paths 
  $Id:$

=========================================================================*/
#ifndef _otbComplexMomentPathFunction_h
#define _otbComplexMomentPathFunction_h

#include "otbGeometricMomentPathFunction.h"

#include <complex>

namespace otb
{

/**
 * \class ComplexMomentPathFunction
 * \brief Calculate the complex moment value over a path list.
 *
 * The implemented equation is:
 *
 *  \f[  c_{p,q}=\int\int_{D} (x+iy)^{p} \cdot (x-iy)^{q} \cdot f(x,y) \cdot
 dx \cdot dy \f]
 *
 * With:
 *
 *   - \f$ (x,y) \f$ pixel localization;
 *   - \f$ f(x,y) \f$  the pixel value over the \f$(x,y) \f$ coordinate.
 *
 * This class is templated over :
 *   - the input image type
 *   - the path type   
 *   - and the coordinate representation type (e.g. float or double).
 * 
 * \ingroup ImageFunctions
 */
template < class TInputImage,
           class TInputPath, 
           class TOutput = std::complex<double >,
	   class TCoordRep = float >
class ITK_EXPORT ComplexMomentPathFunction :
    public GeometricMomentPathFunction<TInputImage, TInputPath, TOutput,TCoordRep>
{
public:
  /** Standard class typedefs. */
  typedef ComplexMomentPathFunction                                  Self;
  typedef GeometricMomentPathFunction<TInputImage,TInputPath, 
                                      TOutput,TCoordRep>             Superclass;
  typedef itk::SmartPointer<Self>                                    Pointer;
  typedef itk::SmartPointer<const Self>                              ConstPointer;
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(ComplexMomentPathFunction, GeometricMomentPathFunction);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** InputImageType typedef support. */
  typedef typename Superclass::InputType            InputType;
  typedef typename Superclass::IndexType            IndexType;
  typedef typename Superclass::ContinuousIndexType  ContinuousIndexType;
  typedef typename Superclass::PointType            PointType;

  /** InputPathType typedef support. */
  typedef typename Superclass::InputPathType         InputPathType;
  typedef typename Superclass::InputPathConstPointer InputPathConstPointer;
 
  typedef TOutput                                   ComplexType;

  /** Dimension of the underlying image. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      InputType::ImageDimension);
  			 
  /** Evalulate the function */
  virtual ComplexType Evaluate( ) const;
  
  itkSetMacro(P, unsigned int);
  itkGetConstReferenceMacro(P, unsigned int);
  itkSetMacro(Q, unsigned int);
  itkGetConstReferenceMacro(Q, unsigned int);

protected:
  ComplexMomentPathFunction();
  ~ComplexMomentPathFunction(){};
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  ComplexMomentPathFunction( const Self& ); //purposely not implemented
  void operator=( const Self& ); //purposely not implemented

  unsigned int m_P;
  unsigned int m_Q;
  
};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbComplexMomentPathFunction.txx"
#endif

#endif

