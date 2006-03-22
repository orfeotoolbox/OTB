/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P. Imbo
  Language  :   C++
  Date      :   22 mars 2006
  Version   :   
  Role      :   Real Geometric Moments Class of Path 
  $Id:$

=========================================================================*/
#ifndef _otbRealMomentPathFunction_h
#define _otbRealMomentPathFunction_h

#include "otbGeometricMomentPathFunction.h"


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
           class TOutput = float,
	   class TCoordRep = float >
class ITK_EXPORT RealMomentPathFunction :
  public GeometricMomentPathFunction<TInputImage,TInputPath, TOutput,TCoordRep >
{
public:
  /** Standard class typedefs. */
  typedef RealMomentPathFunction                                      Self;
  typedef GeometricMomentPathFunction< TInputImage, TInputPath,
                                       TOutput,TCoordRep >            Superclass;
  typedef itk::SmartPointer<Self>                                     Pointer;
  typedef itk::SmartPointer<const Self>                               ConstPointer;
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(RealMomentPathFunction, GeometricMomentPathFunction);


  /** InputImageType typedef support. */
  typedef typename Superclass::InputType            InputType;
  typedef typename Superclass::IndexType            IndexType;
  typedef typename Superclass::ContinuousIndexType  ContinuousIndexType;
  typedef typename Superclass::PointType            PointType;
  typedef typename Superclass::InputPathType         InputPathType;
  typedef typename Superclass::InputPathConstPointer InputPathConstPointer;
 
  typedef TOutput                                   OutputType;


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

