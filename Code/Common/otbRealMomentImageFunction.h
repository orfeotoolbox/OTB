/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P. Imbo
  Language  :   C++
  Date      :   17 mars 2006
  Version   :   
  Role      :   Real Geometric Moments Class of iamges 
  $Id:$

=========================================================================*/
#ifndef _otbRealMomentImageFunction_h
#define _otbRealMomentImageFunction_h

#include "otbGeometricMomentImageFunction.h"


namespace otb
{

/**
 * \class RealMomentImageFunction
 * \brief Virtual class for the Real moments for an image function
 *
 * \ingroup ImageFunctions
 */

template < class TInput, 
           class TOutput = float,
	   class TCoordRep = float >
class ITK_EXPORT RealMomentImageFunction :
  public GeometricMomentImageFunction<TInput, TOutput,TCoordRep >
{
public:
  /** Standard class typedefs. */
  typedef RealMomentImageFunction                                     Self;
  typedef GeometricMomentImageFunction< TInput, TOutput,TCoordRep >   Superclass;
  typedef itk::SmartPointer<Self>                                     Pointer;
  typedef itk::SmartPointer<const Self>                               ConstPointer;
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(RealMomentImageFunction, GeometricMomentImageFunction);


  /** InputImageType typedef support. */
  typedef typename Superclass::InputType            InputType;
  typedef typename Superclass::IndexType            IndexType;
  typedef typename Superclass::ContinuousIndexType  ContinuousIndexType;
  typedef typename Superclass::PointType            PointType;
 
  typedef TOutput                                   OutputType;


protected:
  RealMomentImageFunction() {};
  ~RealMomentImageFunction(){};
  void PrintSelf(std::ostream& os, itk::Indent indent) const 
     {
      Superclass::PrintSelf( os, indent );
     }
     
private:
  RealMomentImageFunction( const Self& ); //purposely not implemented
  void operator=( const Self& );               //purposely not implemented
};

} // namespace otb

#endif

