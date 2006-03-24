/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P. Imbo
  Language  :   C++
  Date      :   22 mars 2006
  Version   :   
  Role      :   Real Geometric Moments Class of Path 
  $Id$

=========================================================================*/
#ifndef _otbRealMomentPathFunction_h
#define _otbRealMomentPathFunction_h

#include "otbGeometricMomentPathFunction.h"
#include "itkVectorContainer.h"


namespace otb
{

/**
 * \class RealMomentPathFunction
 * \brief Virtual class for the Real moments for a path
 *
 * \ingroup PathFunctions
 */

template < class TInputImage, 
           class TInputPath,    
           class TOutput      = double>
class ITK_EXPORT RealMomentPathFunction :
  public GeometricMomentPathFunction<TInputImage,TInputPath, TOutput>
{
public:
  /** Standard class typedefs. */
  typedef RealMomentPathFunction                                      Self;
  typedef GeometricMomentPathFunction< TInputImage, TInputPath,
                                       TOutput>                       Superclass;
  typedef itk::SmartPointer<Self>                                     Pointer;
  typedef itk::SmartPointer<const Self>                               ConstPointer;
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(RealMomentPathFunction, GeometricMomentPathFunction);


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
 
  typedef typename Superclass::OutputType               RealType;


protected:
  RealMomentPathFunction() {};
  ~RealMomentPathFunction(){};
  void PrintSelf(std::ostream& os, itk::Indent indent) const 
     {
      Superclass::PrintSelf( os, indent );
     }
     
private:
  RealMomentPathFunction( const Self& ); //purposely not implemented
  void operator=( const Self& );               //purposely not implemented
};

} // namespace otb

#endif

