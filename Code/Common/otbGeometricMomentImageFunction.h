/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P. Imbo
  Language  :   C++
  Date      :   17 mars 2006
  Version   :   
  Role      :   Geometric Moments Class of iamges 
  $Id:$

=========================================================================*/
#ifndef _otbGeometricMomentImageFunction_h
#define _otbGeometricMomentImageFunction_h

#include "itkImageFunction.h"


namespace otb
{

/**
 * \class GeometricMomentImageFunction
 * \brief Virtual class for the Gometric moments for an image function
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

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** InputImageType typedef support. */
  typedef typename Superclass::InputType            InputType;
  typedef typename Superclass::IndexType            IndexType;
  typedef typename Superclass::ContinuousIndexType  ContinuousIndexType;
  typedef typename Superclass::PointType            PointType;
 
  typedef TOutput                                   OutputType;

  /** Evaluate the function at specified Point position.
   * Subclasses must provide this method. */
  virtual TOutput Evaluate( const PointType& point ) const = 0;

  /** Evaluate the function at specified Index position.
   * Subclasses must provide this method. */
  virtual TOutput EvaluateAtIndex( const IndexType & index ) const = 0;

  /** Evaluate the function at specified ContinousIndex position.
   * Subclasses must provide this method. */
  virtual TOutput EvaluateAtContinuousIndex( 
    const ContinuousIndexType & index ) const = 0;

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

