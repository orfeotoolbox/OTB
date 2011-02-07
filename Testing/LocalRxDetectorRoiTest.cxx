#include "otbImageFileReader.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbStreamingImageFileWriter.h"
#include "otbLocalRxDetectorNonThreadFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "otbExtractROI.h"


int LocalRXDetectorROITest(int argc, char * argv[])
{
	typedef double PixelType;
	typedef otb::VectorImage<PixelType, 2> VectorImageType;
	typedef otb::Image<PixelType, 2> ImageType;
	typedef otb::LocalRxDetectorNonThreadFilter<VectorImageType, ImageType> LocalRxDetectorFilterType;
	typedef otb::ImageFileReader<VectorImageType> ReaderType;
	typedef unsigned char WritePixelType;
	typedef otb::Image<WritePixelType> WriteImageType;
	typedef otb::StreamingImageFileWriter<ImageType> WriterType;
	typedef itk::RescaleIntensityImageFilter<ImageType, WriteImageType> RescalerType;
	typedef otb::ExtractROI<ImageType::PixelType, ImageType::PixelType> ExtractRoiType;



	//---------------    Parameters   ------------------//
	const char * filename = argv[1];
	const char * outputFilename = argv[2];
	const int externalRadius = atoi(argv[3]);
	const int internalRadius = atoi(argv[4]);

	const int nbCol = atoi(argv[5]);
	const int nbRow = atoi(argv[6]);

	//---------------   Read hsi file   -----------------//

	ReaderType::Pointer reader = ReaderType::New();
	reader->SetFileName(filename);
	reader->UpdateOutputInformation();

	//-------------   Anomaly detection   ---------------//

	LocalRxDetectorFilterType::Pointer rxDetector = LocalRxDetectorFilterType::New();
	rxDetector->SetExternalRadius(externalRadius);
	rxDetector->SetInternalRadius(internalRadius);
	rxDetector->SetInput(reader->GetOutput());

	//---------------   Define the ROI   ----------------//

	ImageType::SizeType size;
	size[0] = nbCol;
	size[1] = nbRow;
        
	ImageType::IndexType index;
	index[0] = 0;
	index[1] = 0;

	ImageType::RegionType region;
	region.SetSize(size);
	region.SetIndex(index);

	//------------   Build extractRoiFilter   -----------//

	ExtractRoiType::Pointer roiExtractor = ExtractRoiType::New();
	//roiExtractor->SetExtractionRegion(region);
	roiExtractor->SetStartX(0);
        roiExtractor->SetStartY(0);
        roiExtractor->SetSizeX(nbCol);
        roiExtractor->SetSizeY(nbRow);
        roiExtractor->SetInput(rxDetector->GetOutput());

	//------------   rescaling and writing   ------------//

	RescalerType::Pointer rescaler = RescalerType::New();
	rescaler->SetOutputMinimum(0);
	rescaler->SetOutputMaximum(255);
	rescaler->SetInput(roiExtractor->GetOutput());

	WriterType::Pointer writer = WriterType::New();
	writer->SetFileName(outputFilename);
	writer->SetInput(roiExtractor->GetOutput());

	//----------------   Launching pipeline   ------------//

	writer->Update();

	return EXIT_SUCCESS;
}


