/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P.Imbo
  Language  :   C++
  Date      :   08 fevrier 2006
  Version   :   
  Role      :   
  $Id: $

=========================================================================*/
#ifndef __otbImageToPathAlignFilter_h
#define __otbImageToPathAlignFilter_h

#include "itkImageSource.h"
#include "itkConceptChecking.h"

namespace otb
{
  
/** \class PathToImageAlignFilter
 * \brief Base class for filters that take a Path as input and produce an image as output.
 * Base class for filters that take a Path as input and produce an image as
 * output. By default, if the user does not specify the size of the output
 * image, the maximum size of the path's bounding box is used.  The default
 * spacing of the image is given by the spacing of the input  path (currently
 * assumed internally to be 1.0).
 */
template <class TInputImage, class TOutputPath>
class PathToImageAlignFilter : public itk::ImageSource<TInputImage>
{
public:
  /** Standard class typedefs. */
  typedef PathToImageAlignFilter  Self;
  typedef itk::ImageSource<TInputImage>  Superclass;
  typedef itk::SmartPointer<Self>  Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;
  
  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(PathToImageAlignFilter,ImageSource);
  
  /** Some convenient typedefs. */
  typedef typename Superclass::InputImageRegionType   InputImageRegionType;    
  typedef          TOutputPath                        OutputPathType;
  typedef typename OutputPathType::Pointer            OutputPathPointer;
  typedef typename OutputPathType::ConstPointer       OutputPathConstPointer;
  typedef          TInputImage                        InputImageType;          
  typedef typename InputImageType::Pointer            InputImagePointer;       
  typedef typename InputImageType::SizeType           SizeType;                 
  typedef typename InputImageType::ValueType          ValueType;                

  /** ImageDimension constants */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);

  /** Spacing (size of a pixel) of the output image. The
   * spacing is the geometric distance between image samples.
   * It is stored internally as double, but may be set from
   * float. \sa GetSpacing() */
  virtual void SetSpacing( const double* spacing);
  virtual void SetSpacing( const float* spacing);
  virtual const double* GetSpacing() const;

  /** Set/Get the value for pixels on and off the path. 
  * By default, this filter will return a "0" image with path pixels set to 1 */
  itkSetMacro(PathValue, ValueType);
  itkGetMacro(PathValue, ValueType);
  itkSetMacro(BackgroundValue, ValueType);
  itkGetMacro(BackgroundValue, ValueType);

  /** The origin of the output image. The origin is the geometric
   * coordinates of the index (0,0,...,0).  It is stored internally
   * as double but may be set from float.
   * \sa GetOrigin() */
  virtual void SetOrigin( const double* origin);
  virtual void SetOrigin( const float* origin);
  virtual const double * GetOrigin() const;

  /** Set/Get Size */
  itkSetMacro(Size,SizeType);
  itkGetMacro(Size,SizeType);

protected:
  PathToImageAlignFilter();
  ~PathToImageAlignFilter();

  virtual void GenerateOutputInformation(){}; // do nothing
  virtual void GenerateData();


  SizeType     m_Size;
  double       m_Spacing[InputImageDimension];
  double       m_Origin[InputImageDimension];
  ValueType    m_PathValue;
  ValueType    m_BackgroundValue;

  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  PathToImageAlignFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageToPathAlignFilter.txx"
#endif

#endif
