
#ifndef otbBoxImageFilter_txx
#define otbBoxImageFilter_txx



#include "itkProgressReporter.h"
#include "itkImageToImageFilter.h" 
#include "itkObjectFactory.h"





namespace otb
{

template <class TInputImage, class TOutputImage>
class ITK_EXPORT OcclusionsDetectionFilter : 
	public itk::ImageToImageFilter<TInputImage,TOutputImage>
{
public:
	typedef OcclusionsDetectionFilter Self ;
  	typedef itk::ImageToImageFilter<TInputImage, 
                                  TOutputImage>    Superclass; 
  	typedef itk::SmartPointer<Self>                           Pointer;
  	typedef itk::SmartPointer<const Self>                     ConstPointer;	

  	itkNewMacro(Self);
  	itkTypeMacro(OcclusionsDetectionFilter, ImageToImageFilter); 

  	typedef TInputImage                                       InputImageType;
	typedef typename InputImageType::SizeType                 SizeType;
	typedef typename InputImageType::IndexType                IndexType;
	typedef typename InputImageType::RegionType               RegionType;
	typedef typename InputImageType::PixelType                PixelType;	 
	typedef TOutputImage									  OutputImageType;

	// GETERS
	const TInputImage * GetRightDisparity() const;
	const TInputImage * GetLeftDisparity() const ;
	TOutputImage * GetOutputImage();

	//SETERS
	void SetInputImage( const TInputImage * image);

protected:
	OcclusionsDetectionFilter();
	~OcclusionsDetectionFilter();

private:
	BoxImageFilter(const Self&); //purposely not implemented
  	void operator=(const Self&); //purposely not implemeFnted  

};


} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbOcclusionsDetectionFilter.txx"
#endif

#endif
