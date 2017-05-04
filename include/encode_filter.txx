#ifndef __encode_filter_txx
#define __encode_filter_txx
 
#include "encode_filter.h"
#include <fstream>


#include "itkObjectFactory.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "otbVectorImage.h"


#include <shark/Data/Csv.h>
#include <shark/Data/Pgm.h> //for exporting the learned filters
#include <shark/Data/SparseData.h>//for reading in the images as sparseData/Libsvm format
#include <shark/Models/Autoencoder.h>//normal autoencoder model
#include <shark/Models/TiedAutoencoder.h>//autoencoder with tied weights
#include <shark/Models/Normalizer.h>
#include <shark/Algorithms/Trainers/NormalizeComponentsUnitVariance.h>

 
//using namespace shark;

template< class TImage, class AutoencoderModel, class NormalizerModel>
EncodeFilter<TImage, AutoencoderModel, NormalizerModel>::EncodeFilter()
{
  this->SetNumberOfRequiredInputs(1);
}


template< class TImage, class AutoencoderModel, class NormalizerModel>
void EncodeFilter<TImage, AutoencoderModel, NormalizerModel>::SetAutoencoderModel(const std::string encoderPath)
{
 	//m_net = net;
	std::ifstream ifs(encoderPath);
	boost::archive::polymorphic_text_iarchive ia(ifs);
	m_net.read(ia);
	ifs.close();
	m_hidden_neuron = m_net.numberOfHiddenNeurons();
}


template< class TImage, class AutoencoderModel, class NormalizerModel>
void EncodeFilter<TImage, AutoencoderModel, NormalizerModel>::SetNormalizerModel(const std::string NormalizerPath)
{
 	//m_normalizer = normalizer;
	std::ifstream ifs(NormalizerPath);
	boost::archive::polymorphic_text_iarchive ia(ifs);
	m_normalizer.read(ia);
	ifs.close();
}


template< class TImage, class AutoencoderModel, class NormalizerModel>
void EncodeFilter<TImage, AutoencoderModel, NormalizerModel>::SetModels(const AutoencoderModel net, const NormalizerModel normalizer)
{
	m_net = net;
 	m_normalizer = normalizer;
}


template< class TImage, class AutoencoderModel, class NormalizerModel>
typename TImage::ConstPointer EncodeFilter<TImage, AutoencoderModel, NormalizerModel>::GetInputImage()
{
  return static_cast< const TImage * >
         ( this->itk::ProcessObject::GetInput(0) );
}



template< class TImage, class AutoencoderModel, class NormalizerModel>
AutoencoderModel EncodeFilter<TImage, AutoencoderModel, NormalizerModel>::GetAutoencoderModel()
{
	return m_net;
}


template< class TImage, class AutoencoderModel, class NormalizerModel>
NormalizerModel EncodeFilter<TImage, AutoencoderModel, NormalizerModel>::GetNormalizerModel()
{
	return m_normalizer;
}



struct Data_with_info {
	shark::Data<shark::RealVector> data; // This file format can be used to do Machine Learning with the Shark ML library
	otb::VectorImage<double, 2>::RegionType region;
	otb::VectorImage<double, 2>::PointType origin; 
	otb::VectorImage<double, 2>::SpacingType spacing; 
};


template< class TImage, class AutoencoderModel, class NormalizerModel>
void EncodeFilter<TImage, AutoencoderModel, NormalizerModel>::BeforeThreadedGenerateData()
{
#ifdef _OPENMP
// OpenMP will take care of threading
this->SetNumberOfThreads(1);
#endif
}



template< class TImage, class AutoencoderModel, class NormalizerModel>
void EncodeFilter<TImage, AutoencoderModel, NormalizerModel>::ThreadedGenerateData(const typename TImage::RegionType &outputRegionForThread, unsigned int threadId)
{
	//Data_with_info info;
	typename TImage::ConstPointer input = this->GetInput();
	typename TImage::Pointer output = this->GetOutput();
	// Image to vector
	const unsigned int img_bands = input->GetNumberOfComponentsPerPixel();
	
	itk::ImageRegionConstIterator<TImage> inputIterator(input,outputRegionForThread);
	
	std::vector<shark::RealVector> image_vect;
	
	typename TImage::PixelType pixelValue;
 	while(!inputIterator.IsAtEnd()){
		shark::RealVector vect;
		pixelValue = inputIterator.Get();
		for(unsigned int a = 0; a < img_bands; ++a){
			vect.push_back(pixelValue[a]);
		}
		image_vect.push_back(vect);
		++inputIterator;
	}
	shark::Data<shark::RealVector> data = shark::createDataFromRange(image_vect);
	image_vect.clear();

	/** Normalize the data   */
	
	data= transform(data, m_normalizer);
	/** Encode the data */
	
	data = m_net.encode(data);

	/** vector to image */

	std::size_t numHidden = data.element(1).size();
	output->SetVectorLength(numHidden);
	itk::ImageRegionIterator<TImage> imageIteratorOut(output,outputRegionForThread);
	auto vect_it = data.elements().begin();
	while(!imageIteratorOut.IsAtEnd() && vect_it!=data.elements().end()){
		pixelValue.SetSize(numHidden);
		shark::RealVector vect_out=(*vect_it);
		for(unsigned int a = 0; a < numHidden; ++a){
			pixelValue[a]=vect_out[a];
		}
		
		imageIteratorOut.Set(pixelValue);
		++imageIteratorOut;
		++vect_it;
	}
	
}
	 

#endif
