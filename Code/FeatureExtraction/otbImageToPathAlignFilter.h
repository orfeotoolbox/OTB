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
#include "itkImage.h"

namespace otb
{
  
/** \class ImageToPathAlignFilter
 * \brief Base class for filters that take a Path as input and produce an image as output.
 * Base class for filters that take a Path as input and produce an image as
 * output. By default, if the user does not specify the size of the output
 * image, the maximum size of the path's bounding box is used.  The default
 * spacing of the image is given by the spacing of the input  path (currently
 * assumed internally to be 1.0).
 */
template <class TInputImage, class TOutputPath>
class ImageToPathAlignFilter : public itk::ImageSource<TInputImage>
{
public:
  /** Standard class typedefs. */
  typedef ImageToPathAlignFilter  Self;
  typedef itk::ImageSource<TInputImage>  Superclass;
  typedef itk::SmartPointer<Self>  Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;
  
  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageToPathAlignFilter,ImageSource);
  
  /** ImageDimension constants */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);
 
  /** Some convenient typedefs. */
  typedef          TOutputPath                        OutputPathType;
  typedef typename OutputPathType::Pointer            OutputPathPointer;
  typedef typename OutputPathType::ConstPointer       OutputPathConstPointer;
  typedef          TInputImage                        InputImageType;          
  typedef typename Superclass::InputImageRegionType   InputImageRegionType;    
  typedef typename InputImageType::Pointer            InputImagePointer;       
  typedef typename InputImageType::SizeType           SizeType;                 
  typedef typename InputImageType::ValueType          ValueType;  
  typedef typename InputImageType::PixelType          PixelType;
  typedef          double                             RealType;
  typedef typename itk::Image<RealType,InputImageDimension>             RealImageType; 
  typedef typename InputImageType::Pointer            RealImagePointer;       
  
  typedef typename itk::NumericTraits<PixelType>::RealType RealType;
       

 
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
  ImageToPathAlignFilter();
  ~ImageToPathAlignFilter();

  virtual void GenerateOutputInformation(){}; // do nothing
  virtual void GenerateData();


  SizeType     m_Size;
  double       m_Spacing[InputImageDimension];
  double       m_Origin[InputImageDimension];
  ValueType    m_PathValue;
  ValueType    m_BackgroundValue;

  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  ImageToPathAlignFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  bool		m_isMeaningfulSegment;	/// to get all meaningful segments (maximal or not
  int		m_NbGradDirection; 	/// Number of allowed gradient direction, default 16
  int		m_NbLineDirection; 	/// Number of line directions to scan, default 96)
  double	m_MinGradNorm;		/// Minimum gradient norm to define a direction, default 2.
  double	m_Eps;			/// -log10(max. number of false alarms), default 0

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageToPathAlignFilter.txx"
#endif

#endif
