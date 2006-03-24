/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P. Imbo
  Language  :   C++
  Date      :   22 mars 2006
  Version   :   
  Role      :   Evaluate a function of an image over a specific path
  $Id$

=========================================================================*/
#ifndef _otbPathFunction_h
#define _otbPathFunction_h

#include "itkFunctionBase.h"
#include "itkPoint.h"
#include "itkIndex.h"
#include "itkContinuousIndex.h"
#include "itkImageBase.h"

namespace otb
{


/** \class PathFunction
 * \brief Evaluates a function of an image over a specific path.
 *
 * PathFunction is a baseclass for all objects that evaluates
 * a function of an image using a path list.
 * This class is templated over the input image type, the path type 
 * and the function output and the coordinate representation type
 * (e.g. float or double).
 *
 * The input image is set via method SetInputImage().
 * The input path is set via method SetInputPath().
 *
 *
 * \sa Path
 *
 * \ingroup PathFunction
 */
template <
class TInputImage, 
class TInputPath,
class TOutput
>
class ITK_EXPORT PathFunction : 
    public itk::FunctionBase< TInputPath, TOutput > 
{
public:
  /** Dimension underlying input image. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TInputImage::ImageDimension);

  /** Standard class typedefs. */
  typedef PathFunction                                                 Self;
  typedef itk::FunctionBase<  TInputPath,TOutput >                     Superclass;
  typedef itk::SmartPointer<Self>                                      Pointer;
  typedef itk::SmartPointer<const Self>                                ConstPointer;
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(PathFunction, itk::FunctionBase);

  /** InputImageType typedef support. */
  typedef TInputImage InputImageType;

  /** InputPixel typedef support */
  typedef typename InputImageType::PixelType InputPixelType;

  /** InputImagePointer typedef support */ 
  typedef typename InputImageType::ConstPointer InputImageConstPointer;


  /** InputPathType typedef support. */
  typedef typename Superclass::InputType InputPathType;

  /** InputPathPointer typedef support */ 
  typedef typename InputPathType::ConstPointer InputPathConstPointer;


  /** OutputType typedef support. */
  typedef typename Superclass::OutputType OutputType;

  /** CoordRepType typedef support. */
  typedef float     TCoordRep;
  typedef TCoordRep CoordRepType;

  /** Index Type. */
  typedef typename InputImageType::IndexType IndexType;

  /** ContinuousIndex Type. */
  typedef itk::ContinuousIndex<TCoordRep,itkGetStaticConstMacro(ImageDimension)>
          ContinuousIndexType;

  /** Point Type. */
  typedef itk::Point<TCoordRep,itkGetStaticConstMacro(ImageDimension)> PointType;

  /** Set the input image.
   * \warning this method caches BufferedRegion information.
   * If the BufferedRegion has changed, user must call
   * SetInputImage again to update cached values. */
  virtual void SetInputImage( const InputImageType * ptr );

  /** Get the input image. */
  const InputImageType * GetInputImage() const
    { return m_Image.GetPointer(); }

    
  /** Check if an index is inside the image buffer.
   * \warning For efficiency, no validity checking of
   * the input image is done. */
  virtual bool IsInsideBuffer( const IndexType & index ) const
    { 
      for ( unsigned int j = 0; j < ImageDimension; j++ )
        {
        if ( index[j] < m_StartIndex[j] ) { return false; };
        if ( index[j] > m_EndIndex[j] ) { return false; };
        }
      return true;
    }
            
  /** Check if a continuous index is inside the image buffer.
   * \warning For efficiency, no validity checking of
   * the input image is done. */
  virtual bool IsInsideBuffer( const ContinuousIndexType & index ) const
    { 
      for ( unsigned int j = 0; j < ImageDimension; j++ )
        {
        if ( index[j] < m_StartContinuousIndex[j] ) { return false; };
        if ( index[j] > m_EndContinuousIndex[j] ) { return false; };
        }
      return true;
    }

  /** Check if a point is inside the image buffer.
   * \warning For efficiency, no validity checking of
   * the input image pointer is done. */
  virtual bool IsInsideBuffer( const PointType & point ) const
    { 
    ContinuousIndexType index;
    m_Image->TransformPhysicalPointToContinuousIndex( point, index );
    return this->IsInsideBuffer( index );
    }

  /** Convert point to nearest index. */
  void ConvertPointToNearestIndex( const PointType & point,
    IndexType & index ) const
    {
    ContinuousIndexType cindex;
    m_Image->TransformPhysicalPointToContinuousIndex( point, cindex );
    this->ConvertContinuousIndexToNearestIndex( cindex, index );
    }

  /** Convert continuous index to nearest index. */
  void ConvertContinuousIndexToNearestIndex( const ContinuousIndexType & cindex,
    IndexType & index ) const
    {
    typedef typename IndexType::IndexValueType ValueType;
    for ( unsigned int j = 0; j < ImageDimension; j++ )
      { index[j] = static_cast<ValueType>( vnl_math_rnd( cindex[j] ) ); }
    }
  
  itkGetConstReferenceMacro(StartIndex, IndexType);
  itkGetConstReferenceMacro(EndIndex, IndexType);

  itkGetConstReferenceMacro(StartContinuousIndex, ContinuousIndexType);
  itkGetConstReferenceMacro(EndContinuousIndex, ContinuousIndexType);

protected:
  PathFunction();
  ~PathFunction() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** Const pointer to the input image. */
  InputImageConstPointer  m_Image;

  /** Cache some values for testing if indices are inside buffered region. */
  IndexType               m_StartIndex;
  IndexType               m_EndIndex;
  ContinuousIndexType     m_StartContinuousIndex;
  ContinuousIndexType     m_EndContinuousIndex;

private:
  PathFunction(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  
};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPathFunction.txx"
#endif

#endif
