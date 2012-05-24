/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"

/*
// otb basic
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbStreamingImageFileWriter.h"
#include "otbChangeLabelImageFilter.h"
#include "otbStandardWriterWatcher.h"

// itk
#include "itkVariableLengthVector.h"


// Statistic XML Reader
#include "otbStatisticsXMLFileReader.h"

// Shift Scale Vector Image Filter
#include "otbShiftScaleVectorImageFilter.h"

// Classification filter
#include "otbSVMImageClassificationFilter.h"

#include "itkTimeProbe.h"
#include "otbStandardFilterWatcher.h"
*/




#include "otbImageFileReader.h"
#include "otbStreamingImageFileWriter.h"
#include "otbImageFileWriter.h"
#include "otbStandardWriterWatcher.h"

#include "otbVectorImage.h"
#include "otbImage.h"

#include "otbImageList.h"
#include "otbVectorImageToImageListFilter.h"
#include "otbVectorImageToAmplitudeImageFilter.h"
#include "otbMultiChannelExtractROI.h"

#include "otbStreamingShrinkImageFilter.h"
#include "itkListSample.h"
#include "otbListSampleToHistogramListGenerator.h"
#include "itkImageRegionConstIterator.h"
#include "otbVectorRescaleIntensityImageFilter.h"
#include "otbLocalHistogramImageFunction.h"
#include "otbFlusserMomentsImageFunction.h"
#include "otbHaralickTexturesImageFunction.h"
#include "otbFourierMellinDescriptorsImageFunction.h"
#include "otbHistogramOfOrientedGradientCovariantImageFunction.h"

#include "itkGradientImageFilter.h"
#include "itkMeanImageFilter.h"

#include "otbVariableLengthVectorConverter.h"
#include "itkTimeProbe.h"

#include "itkNumericTraits.h"

namespace otb
{
	namespace Wrapper
	{

		class ComputeImageFeatures : public Application
		{
		public:
			/** Standard class typedefs. */
			typedef ComputeImageFeatures          Self;
			typedef Application                   Superclass;
			typedef itk::SmartPointer<Self>       Pointer;
			typedef itk::SmartPointer<const Self> ConstPointer;
			
			typedef FloatVectorImageType::InternalPixelType PixelType;
			typedef otb::VectorImage<PixelType> InputImageType;
			typedef otb::VectorImage<double, 2> OutImageType;
			
			typedef otb::Image<PixelType> SingleInputImageType;
			typedef otb::VectorImage<unsigned char> OutputImageType;
			typedef otb::ImageFileReader<InputImageType> InputImageReaderType;
			typedef otb::ImageFileReader<OutputImageType> OutputImageReaderType;
			typedef otb::StreamingImageFileWriter<OutputImageType> OutputWriterType;
			typedef otb::ImageList<SingleInputImageType> ImageListType;
			typedef otb::VectorImageToImageListFilter<InputImageType, ImageListType> VI2ILFilterType;
			typedef otb::VectorImageToAmplitudeImageFilter<InputImageType,SingleInputImageType> AmplitudeFilterType;
			typedef otb::VectorRescaleIntensityImageFilter<InputImageType,OutputImageType> RescaleFilterType;
			typedef otb::StreamingShrinkImageFilter<InputImageType, InputImageType> ShrinkFilterType;
			typedef itk::Statistics::ListSample<InputImageType::PixelType> ListSampleType;
			typedef itk::Statistics::DenseFrequencyContainer DFContainerType;
			typedef otb::ListSampleToHistogramListGenerator<ListSampleType, InputImageType::InternalPixelType, DFContainerType> HistogramsGeneratorType;
			typedef otb::MultiChannelExtractROI<unsigned char, unsigned char> UCharExtractFilterType;
			typedef otb::MultiChannelExtractROI<PixelType, PixelType> UShortExtractFilterType;

			// Features
			typedef otb::LocalHistogramImageFunction<SingleInputImageType,double> HistogramFunctionType;
			typedef otb::VariableLengthVectorConverter<HistogramFunctionType::OutputType, double> HistogramConverterType;

			typedef otb::FlusserMomentsImageFunction<SingleInputImageType> FlusserMomentsFunctionType;
			typedef otb::VariableLengthVectorConverter<FlusserMomentsFunctionType::OutputType, double> FlusserMomentsConverterType;

			typedef otb::HaralickTexturesImageFunction<SingleInputImageType> HaralickTexturesFunctionType;
			typedef otb::VariableLengthVectorConverter<HaralickTexturesFunctionType::OutputType,double> HaralickConverterType;

			typedef otb::FourierMellinDescriptorsImageFunction<SingleInputImageType> FourierMellinFunctionType;
			typedef otb::VariableLengthVectorConverter<FourierMellinFunctionType::OutputType,double> FourierMellinConverterType;

			typedef itk::GradientImageFilter<SingleInputImageType> GradientFilterType;
			typedef GradientFilterType::OutputImageType            GradientImageType;

			typedef otb::HistogramOfOrientedGradientCovariantImageFunction<GradientImageType> HOGFunctionType;
			typedef otb::VariableLengthVectorConverter<HOGFunctionType::OutputType,double> HOGConverterType;

			/** Standard macro */
			itkNewMacro(Self);

			itkTypeMacro(ComputeImageFeatures, otb::Application);
			
		private:

			void DoInit()
			{
				SetName("ComputeImageFeatures");
				SetDescription("...");
				
				AddParameter(ParameterType_InputImage, "in",  "Input Image");
				SetParameterDescription("in", "The input image to compute the features on.");
				
				AddParameter(ParameterType_Int, "tilesize",  "Tile Size");
				SetParameterDescription("tilesize", "Width of the window over which the features are computed.");
				SetDefaultParameterInt("tilesize", 20);
				
				AddParameter(ParameterType_Int, "tilestep",  "Tile Step");
				SetParameterDescription("tilestep", "Step distance between neighboring windows (windows may overlap)");
				SetDefaultParameterInt("tilestep", 1);
				
				// Add the feature parameters in a group
				AddParameter(ParameterType_Group, "features", "Features computed");
				SetParameterDescription("features","This group of parameters allows to define features computed in the input image.");

				AddParameter(ParameterType_Empty,"outputs.hog","Compute histogram of oriented gradient by default");
				SetParameterDescription("outputs.hog", "hog");
				EnableParameter("outputs.hog");

				AddParameter(ParameterType_Empty,"outputs.flusser","Compute flusser moments by default");
				SetParameterDescription("outputs.flusser", "flusser");
				EnableParameter("outputs.flusser");

				AddParameter(ParameterType_OutputImage, "out",  "Output Image");
				SetParameterDescription("out", "Output image containing the feature vectors.");
				SetParameterOutputImagePixelType("out", ImagePixelType_double);
			}

			void DoUpdateParameters()
			{
				// Nothing to do here : all parameters are independent
			}

			void DoExecute()
			{
				const unsigned int numParameters = 232;
				
				itk::VariableLengthVector<double> features(numParameters);
				
				float intensityMin = 0; // [!] change
				float intensityMax = 1000; // [!] change
				
				unsigned int tilesize = GetParameterInt("tilesize");
				unsigned int tilestep = GetParameterInt("tilestep");
	
				// First, instantiate a reader
				InputImageReaderType::Pointer inreader = InputImageReaderType::New();
				inreader->SetFileName(GetParameterString("in").c_str());
				inreader->UpdateOutputInformation();

				// Retrieve nb of bands
				unsigned int nbComp = inreader->GetOutput()->GetNumberOfComponentsPerPixel();

				// Shrink image to estimate histogram
				ShrinkFilterType::Pointer shrinkFilter = ShrinkFilterType::New();

				// Shrink factor is computed so as to load a quicklook of 1000
				// pixels square at most
				InputImageType::SizeType imageSize = inreader->GetOutput()->GetLargestPossibleRegion().GetSize();
				unsigned int shrinkFactor = 1; //std::max(imageSize[0], imageSize[1])/1000; // [!]
				std::cout<<"Shrink factor: "<<shrinkFactor<<std::endl;  
				shrinkFilter->SetShrinkFactor(shrinkFactor);
				shrinkFilter->SetInput(inreader->GetOutput());

				otb::StandardWriterWatcher shrinkWatcher(shrinkFilter->GetStreamer(), shrinkFilter->GetFilter(),"Estimating image histogram for rescaling");
				shrinkFilter->GetStreamer()->SetAutomaticTiledStreaming(256);
				shrinkFilter->Update();

				AmplitudeFilterType::Pointer shrinkAmplitudeFilter = AmplitudeFilterType::New();
				shrinkAmplitudeFilter->SetInput(shrinkFilter->GetOutput());
				shrinkAmplitudeFilter->Update();

				ListSampleType::Pointer listSample = ListSampleType::New();

				itk::ImageRegionConstIterator<InputImageType> it(shrinkFilter->GetOutput(), shrinkFilter->GetOutput()->GetLargestPossibleRegion());
				itk::ImageRegionConstIterator<SingleInputImageType> ait(shrinkAmplitudeFilter->GetOutput(), shrinkAmplitudeFilter->GetOutput()->GetLargestPossibleRegion());

				// Now we generate the list of samples
				for(it.GoToBegin(),ait.GoToBegin(); !it.IsAtEnd() && !ait.IsAtEnd(); ++it,++ait)
				  {
				    InputImageType::PixelType sample(nbComp+1);
 
				    for(unsigned int i = 0; i < nbComp; ++i)
				      {
					sample[i] = it.Get()[i];
				      }
				    sample[nbComp] = ait.Get();
				    listSample->PushBack(sample);
				  }

				// And then the histogram
				HistogramsGeneratorType::Pointer histogramsGenerator = HistogramsGeneratorType::New();
				histogramsGenerator->SetListSample(listSample);
				histogramsGenerator->SetNumberOfBins(255);
				histogramsGenerator->NoDataFlagOn();
				histogramsGenerator->Update();

				// And extract the 2% lower and upper quantile
				InputImageType::PixelType inputMin(nbComp), inputMax(nbComp);
				OutputImageType::PixelType outputMin(nbComp),outputMax(nbComp);
				outputMin.Fill(0);
				outputMax.Fill(255);


				for(unsigned int i = 0; i < nbComp; ++i)
				{
					inputMin[i] = intensityMin; // histogramsGenerator->GetOutput()->GetNthElement(i)->Quantile(0, 0.02);
					inputMax[i] = intensityMax; //histogramsGenerator->GetOutput()->GetNthElement(i)->Quantile(0, 0.98);
				}

				std::cout<<"Values below "<<inputMin<<" are set to 0"<<std::endl;
				std::cout<<"Values above "<<inputMax<<" are set to 255"<<std::endl;

				// Now, generate the 8 bits RGB scaled view
				RescaleFilterType::Pointer rescaler = RescaleFilterType::New();
				rescaler->SetInput(inreader->GetOutput());
				rescaler->SetInputMinimum(inputMin);
				rescaler->SetInputMaximum(inputMax);
				rescaler->SetOutputMinimum(outputMin);
				rescaler->SetOutputMaximum(outputMax);
				rescaler->AutomaticInputMinMaxComputationOff();

				UCharExtractFilterType::Pointer extractFilter = UCharExtractFilterType::New();
				extractFilter->SetInput(rescaler->GetOutput());
				extractFilter->SetChannel(3);
				extractFilter->SetChannel(2);
				extractFilter->SetChannel(1);

				// Region for patches extraction
				InputImageType::RegionType region;
				region.SetSize(0,tilesize);
				region.SetSize(1,tilesize);
				InputImageType::IndexType index;

				unsigned int count = 0;

				// unsigned int nbPatches = inreader->GetOutput()->GetLargestPossibleRegion().GetNumberOfPixels()/(tilesize*tilesize); [!]
				unsigned int nbPatches = inreader->GetOutput()->GetLargestPossibleRegion().GetNumberOfPixels()/(tilestep*tilestep);
				
				unsigned int radius = tilesize/2 - 1 + tilesize%2;

				unsigned int resX = 0, resY = 0;
				for(index[1]=0;index[1] < static_cast<unsigned int>(imageSize[1]-tilesize-1);index[1]+=tilestep)
				{
					resY++;
				}

				for(index[0]=0;index[0]<static_cast<unsigned int>(imageSize[0]-tilesize-1);index[0]+=tilestep)
				{
					resX++;
				}

				std::cout << "res = (" << resX << ", " << resY << ")" << std::endl;


				OutImageType::Pointer outImage = OutImageType::New();


				OutImageType::IndexType start; 
				start[0] =   0;  // first index on X 
				start[1] =   0;  // first index on Y 

				OutImageType::SizeType size; 
				size[0]  = resX;  // size along X 
				size[1]  = resY;  // size along Y


				OutImageType::RegionType outRegion; 

				outRegion.SetSize(size); 
				outRegion.SetIndex(start); 

				outImage->SetRegions(outRegion); 

				outImage->SetNumberOfComponentsPerPixel(numParameters); 
				outImage->Allocate();

				OutImageType::PixelType pixel;

				pixel.Reserve(numParameters);


				OutputImageType::IndexType outIndex;
				outIndex[0] = 0;
				outIndex[1] = 0;

				itk::TimeProbe chrono;

				for(index[1]=0;static_cast<unsigned int>(index[1]<imageSize[1]-tilesize-1);index[1]+=tilestep)
				{
					unsigned int remtimeOutput;
					for(index[0]=0;static_cast<unsigned int>(index[0]<imageSize[0]-tilesize-1);index[0]+=tilestep)
					{
			
						unsigned int globalIndex = 0;


						chrono.Start();

						region.SetIndex(index);

						double meanTime = chrono.GetMeanTime();
						unsigned int remainingTime = static_cast<unsigned int>(meanTime*(nbPatches-count));

						remtimeOutput = remainingTime;

						int dx,dy;

						int reds=0,yellows=0,greens=0,cyans=0,blues=0,magentas=0,grays=0;
		
						typedef itk::ExtractImageFilter<InputImageType,InputImageType> ExtractFilterType;
						ExtractFilterType::Pointer extractFilter1 = ExtractFilterType::New();
						extractFilter1->SetInput(inreader->GetOutput());
						extractFilter1->SetExtractionRegion(region);
						extractFilter1->Update();

						typedef itk::ImageRegionConstIterator< InputImageType > ConstIteratorType;  
						ConstIteratorType inputIt(extractFilter1->GetOutput(), extractFilter1->GetOutput()->GetLargestPossibleRegion()); 

						for (inputIt.GoToBegin(); !inputIt.IsAtEnd(); ++inputIt) 
						{ 
							InputImageType::PixelType value = inputIt.Get();
							inreader->Update();

							int margin = 3; // amount by which (e.g.) the red channel value must supercede the others to be counted as "red"

							if      (value[0] - margin > value[1] && value[0] > value[2])
								reds++;
							else if (value[0] + margin < value[1] && value[0] < value[2])
								cyans++;
							else if (value[1] - margin > value[0] && value[1] > value[2])
								greens++;
							else if (value[1] + margin < value[0] && value[1] < value[2])
								magentas++;
							else if (value[2] - margin > value[0] && value[2] > value[1])
								blues++;
							else if (value[2] + margin < value[0] && value[2] < value[1])
								yellows++;
							else
								grays++;
						}

						features[globalIndex++] = reds;
						features[globalIndex++] = cyans;
						features[globalIndex++] = greens;
						features[globalIndex++] = magentas;
						features[globalIndex++] = blues;
						features[globalIndex++] = yellows;
						features[globalIndex++] = grays;

						UCharExtractFilterType::Pointer extractFilter = UCharExtractFilterType::New();
						extractFilter->SetInput(rescaler->GetOutput());
						extractFilter->SetChannel(3);
						extractFilter->SetChannel(2);
						extractFilter->SetChannel(1);
						extractFilter->SetExtractionRegion(region);

						UShortExtractFilterType::Pointer ushortExtractFilter = UShortExtractFilterType::New();
						ushortExtractFilter->SetInput(inreader->GetOutput());
						ushortExtractFilter->SetExtractionRegion(region);
						ushortExtractFilter->Update();

						VI2ILFilterType::Pointer vi2ilFilter = VI2ILFilterType::New();
						vi2ilFilter->SetInput(ushortExtractFilter->GetOutput());
						vi2ilFilter->Update();

						AmplitudeFilterType::Pointer amplitudeFilter = AmplitudeFilterType::New();
						amplitudeFilter->SetInput(ushortExtractFilter->GetOutput());
						amplitudeFilter->Update();

						InputImageType::IndexType centerIndex = index;
						centerIndex[0]+=tilesize/2;
						centerIndex[1]+=tilesize/2;

						InputImageType::PointType point;
						inreader->GetOutput()->TransformIndexToPhysicalPoint(centerIndex,point);

						// Computing histograms for each channel
						for(unsigned int comp = 0; comp<nbComp;++comp)
						{
							HistogramFunctionType::Pointer histogramFunction = HistogramFunctionType::New();
							histogramFunction->SetHistogramMin(intensityMin);//histogramsGenerator->GetOutput()->GetNthElement(comp)->Quantile(0, 0.02));
							histogramFunction->SetHistogramMax(intensityMax);//histogramsGenerator->GetOutput()->GetNthElement(comp)->Quantile(0, 0.98));
							histogramFunction->SetNumberOfHistogramBins(25);
							histogramFunction->GaussianSmoothingOn();
							histogramFunction->SetInputImage(vi2ilFilter->GetOutput()->GetNthElement(comp)); // [!] replaced "0" by "comp". better?
							histogramFunction->SetNeighborhoodRadius(radius);

							HistogramConverterType::Pointer histogramConverter = HistogramConverterType::New();
						}      

						// Compute flusser moments on amplitude
						FlusserMomentsFunctionType::Pointer flusserMomentsFunction = FlusserMomentsFunctionType::New();
						flusserMomentsFunction->SetNeighborhoodRadius(radius);
						flusserMomentsFunction->SetInputImage(amplitudeFilter->GetOutput());

						FlusserMomentsConverterType::Pointer flusserMomentsConverter = FlusserMomentsConverterType::New();

						// Compute Haralick textures on amplitude
						HaralickTexturesFunctionType::Pointer haralickFunction = HaralickTexturesFunctionType::New();
						HaralickTexturesFunctionType::OffsetType offset;
						offset.Fill(2);
						// Last histogam is the amplitude one
						haralickFunction->SetInputImageMaximum(histogramsGenerator->GetOutput()->GetNthElement(nbComp)->Quantile(0, 1.0));
						haralickFunction->SetInputImageMinimum(histogramsGenerator->GetOutput()->GetNthElement(nbComp)->Quantile(0,
      0.0));
						haralickFunction->SetNeighborhoodRadius(radius);
						haralickFunction->SetNumberOfBinsPerAxis(25);
						haralickFunction->SetOffset(offset);
						haralickFunction->SetInputImage(amplitudeFilter->GetOutput());

						HaralickConverterType::Pointer haralickConverter = HaralickConverterType::New();

						// Compute Fourier-Mellin descriptors on amplitude
						FourierMellinFunctionType::Pointer fourierMellinFunction = FourierMellinFunctionType::New();
						fourierMellinFunction->SetNeighborhoodRadius(radius);
						fourierMellinFunction->SetInputImage(amplitudeFilter->GetOutput());

						FourierMellinConverterType::Pointer fourierMellinConverter = FourierMellinConverterType::New();

						// HOG: COMMENT FROM HERE

						// Compute Histogram of oriented gradients on gradient of
						// amplitude image
						GradientFilterType::Pointer gradientFilter = GradientFilterType::New();
						gradientFilter->SetInput(amplitudeFilter->GetOutput());
						gradientFilter->Update();

						HOGFunctionType::Pointer hogFunction = HOGFunctionType::New();
						hogFunction->SetNeighborhoodRadius(radius);
						hogFunction->SetInputImage(gradientFilter->GetOutput());

						HOGConverterType::Pointer hogConverter = HOGConverterType::New();

						// HOG: COMMENT TO HERE

						++ count;

						outImage->SetPixel(outIndex, features);
						//std::cout << outImage->GetPixel(outIndex) << std::endl;
						globalIndex = 0;
						chrono.Stop();
						outIndex[0]++;
					}
					outIndex[0] = 0;
					outIndex[1]++;
					std::cout<<"Remaining Time: "<<(remtimeOutput/60)<<" minutes "<<(remtimeOutput%60)<<" seconds"<<std::endl;
				}
				
				SetParameterOutputImage("out", dynamic_cast<OutImageType*>(&(*outImage)));
			}
		};
	}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::ComputeImageFeatures)
