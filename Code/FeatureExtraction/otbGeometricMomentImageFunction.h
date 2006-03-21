/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P. Imbo
  Language  :   C++
  Date      :   17 mars 2006
  Version   :   
  Role      :   Geometric Moments Class of iamges 
  $Id$

=========================================================================*/
#ifndef _otbGeometricMomentImageFunction_h
#define _otbGeometricMomentImageFunction_h

#include "itkImageFunction.h"


namespace otb
{

/**
 * \class GeometricMomentImageFunction
 * \brief Virtual class for the Geometric moments for an image function
 *
 * \ingroup ImageFunctions
 */

template < class TInput, 
           class TOutput = float,
	   class TCoordRep = float >
class ITK_EXPORT GeometricMomentImageFunction :
  public itk::ImageFunction<TInput, TOutput,TCoordRep >
{
public:
  /** Standard class typedefs. */
  typedef GeometricMomentImageFunction                            Self;
  typedef itk::ImageFunction< TInput, TOutput,TCoordRep >         Superclass;
  typedef itk::SmartPointer<Self>                                 Pointer;
  typedef itk::SmartPointer<const Self>                           ConstPointer;
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(GeometricMomentImageFunction, itk::ImageFunction);


  /** InputImageType typedef support. */
  typedef typename Superclass::InputType            InputType;
  typedef typename Superclass::IndexType            IndexType;
  typedef typename Superclass::ContinuousIndexType  ContinuousIndexType;
  typedef typename Superclass::PointType            PointType;
 
  typedef TOutput                                   OutputType;


protected:
  GeometricMomentImageFunction() {};
  ~GeometricMomentImageFunction(){};
  void PrintSelf(std::ostream& os, itk::Indent indent) const 
     {
      Superclass::PrintSelf( os, indent );
     }
     
private:
  GeometricMomentImageFunction( const Self& ); //purposely not implemented
  void operator=( const Self& );               //purposely not implemented
};

} // namespace otb

#endif

