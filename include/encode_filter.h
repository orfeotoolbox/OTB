#ifndef __encode_Filter_h
#define __encode_Filter_h
 
#include "itkImageToImageFilter.h"
#include "itkMacro.h"

 
template< class TImage, class AutoencoderModel, class NormalizerModel>
class ITK_EXPORT EncodeFilter:public itk::ImageToImageFilter< TImage, TImage >
{
	public:
	  // Standard class typedefs. 
		typedef EncodeFilter                                   	Self;
		typedef itk::ImageToImageFilter< TImage, TImage > 	Superclass;
		typedef itk::SmartPointer< Self >                       Pointer;
	 
	// Method for creation through the object factory. 
		itkNewMacro(Self);
	 
	// Run-time type information (and related methods). 
		itkTypeMacro(EncodeFilter, ImageToImageFilter);
		
		//void SetInputImage(const TImage* image);
		void SetAutoencoderModel(const std::string encoderPath);
		void SetNormalizerModel(const std::string NormalizerPath);
		void SetModels( const AutoencoderModel net, const NormalizerModel normalizer);
		std::size_t GetDimension(){return m_hidden_neuron;};
	protected:
		EncodeFilter();
	  	~EncodeFilter(){}
	
  		typename TImage::ConstPointer GetInputImage();
		AutoencoderModel GetAutoencoderModel();
		NormalizerModel GetNormalizerModel();
 		
	// Does the real work. 
		virtual void GenerateOutputInformation();
		virtual void BeforeThreadedGenerateData();
	  	void ThreadedGenerateData(const typename TImage::RegionType &outputRegionForThread, unsigned int threadId) ITK_OVERRIDE;
	 
	private:
	  	EncodeFilter(const Self &); //purposely not implemented
	  	void operator=(const Self &);  //purposely not implemented
	        AutoencoderModel m_net;  
		NormalizerModel m_normalizer;	
		std::size_t m_hidden_neuron;
};
 
#ifndef ITK_MANUAL_INSTANTIATION
#include "encode_filter.txx"
#endif
 
 
#endif // __encode_Filter_h
