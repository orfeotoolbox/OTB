#ifndef __dummy_filter_txx
#define __dummy_filter_txx
 
#include "dummy_filter.h"
#include <fstream>


#include "itkObjectFactory.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "otbVectorImage.h"

/*
#include <shark/Data/Csv.h>
#include <shark/Data/Pgm.h> //for exporting the learned filters
#include <shark/Data/SparseData.h>//for reading in the images as sparseData/Libsvm format
#include <shark/Models/Autoencoder.h>//normal autoencoder model
#include <shark/Models/TiedAutoencoder.h>//autoencoder with tied weights
#include <shark/Models/Normalizer.h>
#include <shark/Algorithms/Trainers/NormalizeComponentsUnitVariance.h>
*/
 
//using namespace shark;

template< class TImage>
DummyFilter<TImage>::DummyFilter()
{
  this->SetNumberOfRequiredInputs(1);
}



template< class TImage>
typename TImage::ConstPointer DummyFilter<TImage>::GetInputImage()
{
  return static_cast< const TImage * >
         ( this->itk::ProcessObject::GetInput(0) );
}





template< class TImage>
void DummyFilter<TImage>::BeforeThreadedGenerateData()
{
#ifdef _OPENMP
// OpenMP will take care of threading
this->SetNumberOfThreads(1);
#endif
}


/*template< class TImage, class AutoencoderModel, class NormalizerModel>
void EncodeFilter<TImage, AutoencoderModel, NormalizerModel>::GenerateData()*/
template< class TImage>
void DummyFilter<TImage>::ThreadedGenerateData(const typename TImage::RegionType &outputRegionForThread, unsigned int threadId)
{
	//Data_with_info info;
	typename TImage::ConstPointer input = this->GetInput();
	typename TImage::Pointer output = this->GetOutput();
 	
	// Image to vector
	const unsigned int img_bands = input->GetNumberOfComponentsPerPixel();
	
	itk::ImageRegionConstIterator<TImage> inputIterator(input,outputRegionForThread);
	itk::ImageRegionIterator<TImage> imageIteratorOut(output,outputRegionForThread);
	
	typename TImage::PixelType pixelValue;
 	while(!inputIterator.IsAtEnd()){
		imageIteratorOut.Set(inputIterator.Get());
		++inputIterator;
		++imageIteratorOut;
	}
	
}
	 

#endif
