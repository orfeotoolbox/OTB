#include "otbImageFileReader.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbStreamingImageFileWriter.h"
#include "otbLocalRxDetectorFilter.h"
#include "itkRescaleIntensityImageFilter.h"

int LocalRXDetectorNewTest(int argc, char * argv[])
{
	typedef double PixelType;
	typedef otb::VectorImage<PixelType, 2> VectorImageType;
	typedef otb::Image<PixelType, 2> ImageType;
	typedef otb::LocalRxDetectorFilter<VectorImageType, ImageType> LocalRxDetectorFilterType;

	LocalRxDetectorFilterType::Pointer rxDetector = LocalRxDetectorFilterType::New();
        return EXIT_SUCCESS;
}

int LocalRXDetectorTest(int argc, char * argv[])
{
	typedef double PixelType;
	typedef otb::VectorImage<PixelType, 2> VectorImageType;
	typedef otb::Image<PixelType, 2> ImageType;
	typedef otb::LocalRxDetectorFilter<VectorImageType, ImageType> LocalRxDetectorFilterType;

	typedef otb::ImageFileReader<VectorImageType> ReaderType;

	typedef unsigned char WritePixelType;
	typedef otb::Image<WritePixelType> WriteImageType;
	typedef otb::StreamingImageFileWriter<WriteImageType> WriterType;
	typedef itk::RescaleIntensityImageFilter<ImageType, WriteImageType> RescalerType;


	/////////// PARAMETERS ///////////


	const char * filename = "cupriteSubHsi.tif";	//argv[1];
	const char * outputFilename = "test.tif";	//argv[2];
	const int externalRadius = 7;					//atoi(argv[3]);
	const int internalRadius = 1;					//atoi(argv[4]);

	//////// RX Filtering /////////

	ReaderType::Pointer reader = ReaderType::New();
	reader->SetFileName(filename);

	LocalRxDetectorFilterType::Pointer rxDetector = LocalRxDetectorFilterType::New();
	rxDetector->SetExternalRadius(externalRadius);
	rxDetector->SetInternalRadius(internalRadius);
	rxDetector->SetInput(reader->GetOutput());

	RescalerType::Pointer rescaler = RescalerType::New();
	rescaler->SetOutputMinimum(0);
	rescaler->SetOutputMaximum(255);
	rescaler->SetInput(rxDetector->GetOutput());


	WriterType::Pointer writer = WriterType::New();
	writer->SetFileName(outputFilename);
	writer->SetInput(rescaler->GetOutput());
	writer->Update();

	return EXIT_SUCCESS;
}


