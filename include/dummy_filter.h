#ifndef __dummy_Filter_h
#define __dummy_Filter_h
 
#include "itkImageToImageFilter.h"
#include "itkMacro.h"

 
template< class TImage>
class ITK_EXPORT DummyFilter:public itk::ImageToImageFilter< TImage, TImage >
{
	public:
	  /** Standard class typedefs. */
		typedef DummyFilter                                   	Self;
		typedef itk::ImageToImageFilter< TImage, TImage > 	Superclass;
		typedef itk::SmartPointer< Self >                       Pointer;
	 
	/** Method for creation through the object factory. */
		itkNewMacro(Self);
	 
	/** Run-time type information (and related methods). */
		itkTypeMacro(DummyFilter, ImageToImageFilter);
		
		//void SetInputImage(const TImage* image);
		

	protected:
		DummyFilter();
	  	~DummyFilter(){}

  		typename TImage::ConstPointer GetInputImage();
 
	/** Does the real work. */

		virtual void BeforeThreadedGenerateData();
	  	void ThreadedGenerateData(const typename TImage::RegionType &outputRegionForThread, unsigned int threadId) ITK_OVERRIDE;
	 	
	private:
	  	DummyFilter(const Self &); //purposely not implemented
	  	void operator=(const Self &);  //purposely not implemented
};
 
#ifndef ITK_MANUAL_INSTANTIATION
#include "dummy_filter.txx"
#endif
 
 
#endif // __dummy_Filter_h
