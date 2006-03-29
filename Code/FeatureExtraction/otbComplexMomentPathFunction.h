/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P. Imbo
  Language  :   C++
  Date      :   22 mars 2006
  Version   :   
  Role      :   Complex Geometric Moments Class of paths 
  $Id$

=========================================================================*/
#ifndef _otbComplexMomentPathFunction_h
#define _otbComplexMomentPathFunction_h

#include "otbGeometricMomentPathFunction.h"
#include "itkVectorContainer.h"

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
template < class TInputPath, 
           class TOutput = std::complex<double> >
class ITK_EXPORT ComplexMomentPathFunction :
    public GeometricMomentPathFunction< TInputPath, 
					TOutput >
{
public:
  /** Standard class typedefs. */
  typedef ComplexMomentPathFunction                                  Self;
  typedef GeometricMomentPathFunction<TInputPath, TOutput >          Superclass;
  typedef itk::SmartPointer<Self>                                    Pointer;
  typedef itk::SmartPointer<const Self>                              ConstPointer;
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(ComplexMomentPathFunction, GeometricMomentPathFunction);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);


  /** InputPathType typedef support. */
  typedef typename Superclass::PathType                 PathType;
  typedef typename Superclass::PathConstPointer         PathConstPointer;  
  typedef typename PathType::ContinuousIndexType        VertexType;
  typedef itk::VectorContainer< unsigned,VertexType >   VertexListType;
  typedef typename VertexListType::ConstPointer         VertexListPointer;

  typedef typename Superclass::OutputType               ComplexType;
  typedef float                                         RealType;

  			 
  /** Evalulate the function */
  virtual ComplexType Evaluate(const PathType& path) const;
  
  itkSetMacro(P, unsigned int);
  itkGetConstReferenceMacro(P, unsigned int);
  itkSetMacro(Q, unsigned int);
  itkGetConstReferenceMacro(Q, unsigned int);
  itkSetMacro(Step, RealType);
  itkGetConstReferenceMacro(Step, RealType);

protected:
  ComplexMomentPathFunction();
  ~ComplexMomentPathFunction(){};
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  ComplexMomentPathFunction( const Self& ); //purposely not implemented
  void operator=( const Self& ); //purposely not implemented
  ComplexType EvaluateComplexMomentAtIndex(VertexType index) const;
 
  unsigned int    m_P;
  unsigned int    m_Q;
  RealType        m_Step;
  
};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbComplexMomentPathFunction.txx"
#endif

#endif

