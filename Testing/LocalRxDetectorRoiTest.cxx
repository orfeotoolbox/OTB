#include "otbImageFileReader.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbStreamingImageFileWriter.h"
#include "otbLocalRxDetectorFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "otbExtractROI.h"


int LocalRXDetectorROITest(int argc, char * argv[])
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
	typedef otb::ExtractROI<ImageType::PixelType, ImageType::PixelType> ExtractRoiType;



	//---------------    Parameters   ------------------//
	const char * filename = argv[1];
	const char * outputFilename = argv[2];
	const int externalRadius = atoi(argv[3]);
	const int internalRadius = atoi(argv[4]);

	const int nbCol = atoi(argv[5]);
	const int nbRow = atoi(argv[6]);

	const int idxCol = 0;
	const int idxRow = 0;



	//---------------   Read hsi file   -----------------//

	ReaderType::Pointer reader = ReaderType::New();
	reader->SetFileName(filename);
	reader->UpdateOutputInformation();

	std::cout << "File read : OK" << std::endl;



	//-------------   Anomaly detection   ---------------//

	LocalRxDetectorFilterType::Pointer rxDetector = LocalRxDetectorFilterType::New();
	rxDetector->SetExternalRadius(externalRadius);
	rxDetector->SetInternalRadius(internalRadius);
	rxDetector->SetInput(reader->GetOutput());

	std::cout << "Anomaly detection ok" << std::endl;



	//---------------   Define the ROI   ----------------//

	ImageType::SizeType size;
	size[0] = nbCol;
	size[1] = nbRow;

	ImageType::IndexType index;
	index[0] = idxCol;
	index[1] = idxRow;

	ImageType::RegionType region;
	region.SetSize(size);
	region.SetIndex(index);

	std::cout << "ROI defined OK" << std::endl;



	//------------   Build extractRoiFilter   -----------//

	ExtractRoiType::Pointer roiExtractor = ExtractRoiType::New();
	roiExtractor->SetExtractionRegion(region);
	roiExtractor->SetInput(rxDetector->GetOutput());

	std::cout << "build extractor ok" << std::endl;



	//------------   rescaling and writing   ------------//

	RescalerType::Pointer rescaler = RescalerType::New();
	rescaler->SetOutputMinimum(0);
	rescaler->SetOutputMaximum(255);
	rescaler->SetInput(roiExtractor->GetOutput());

	WriterType::Pointer writer = WriterType::New();
	writer->SetFileName(outputFilename);
	writer->SetInput(rescaler->GetOutput());

	std::cout << "Rescaling and writing ok" << std::endl;

	//----------------   Launching pipeline   ------------//

	writer->Update();

	return EXIT_SUCCESS;
}


