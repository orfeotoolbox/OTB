/*
 * Copyright (C) 2018 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part a Remote Module for Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"
#include "otbImage.h"
#include "otbMultiToMonoChannelExtractROI.h"
#include "otbOGRDataSourceWrapper.h"
#include "otbOGRFeatureWrapper.h"

#include "otbSLICScheduler.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
// #include "otbSegmentCharacteristicsFilter.h"
#include "otbPersistentFilterStreamingDecorator.h"
#include "itkLabelToRGBImageFilter.h"

namespace otb
{
  namespace Wrapper
  {
    class SLIC : public Application
    {
    public:
      typedef SLIC Self;
      typedef Application Superclass;
      typedef itk::SmartPointer<Self> Pointer;
      typedef itk::SmartPointer<const Self> ConstPointer;
      typedef otb::VectorImage<float>                                              VectorImageType;
      typedef unsigned int                                                         LabelType;
      typedef otb::Image<LabelType>                                                LabelImageType;
      typedef otb::SLICScheduler<VectorImageType, LabelImageType>                  SLICSchedulerType;
      typedef otb::ImageFileReader<VectorImageType>                                ReaderType;
      typedef otb::ImageFileReader<LabelImageType>                                 LabelReaderType;
      typedef otb::ImageFileWriter<LabelImageType>                                 WriterType;
      typedef typename otb::VectorImage<double>::PixelType                         SampleType;

      typedef otb::VectorImage<unsigned char>                                      RGBImageType;
      typedef RGBImageType::PixelType                                              RGBPixelType;
      typedef otb::ImageFileWriter<RGBImageType>                                   RGBWriterType;

      // Segmentation contrast maximisation LUT
      typedef itk::LabelToRGBImageFilter <LabelImageType, RGBImageType>            LabelToRGBFilterType;
    
      itkNewMacro(Self);
      itkTypeMacro(Merging, otb::Application);

    private:


      //Tiling modes
      enum TilingMode
	{
	  AUTO,
	  MANUAL
	};
      
      void DoInit()
      {
	SetName("SLIC");
	SetDescription("");

	SetDocLongDescription("");
	SetDocLimitations("");
	SetDocAuthors("");
	SetDocSeeAlso("");

	AddParameter(ParameterType_InputImage,  "in",    "Input image");
	SetParameterDescription( "in", "The input image." );
   
	AddParameter(ParameterType_OutputImage, "out", "Output raster");
	SetParameterDescription( "out", "Output raster containing the segment label IDs" );
	SetDefaultOutputPixelType("out",ImagePixelType_uint32);

	AddParameter(ParameterType_Float,"spw","Spatial Width");
	SetParameterDescription("spw","Initial width of segment grid, in pixels. Is approximately the average width of final segments");
	SetDefaultParameterFloat("spw",100);
	SetMinimumParameterFloatValue("spw",2);

	AddParameter(ParameterType_Float,"dw","Distance weight");
	SetParameterDescription("dw","Importance of spatial distance w.r.t. spectral distance");
	SetDefaultParameterFloat("dw",1);
	SetMinimumParameterFloatValue("dw",0);

	AddParameter(ParameterType_Int,"maxit","Maximum number of iterations");
	SetParameterDescription("maxit","Maximum number of iterations of the SLIC algorithm.");
	SetDefaultParameterInt("maxit",100);
	SetMinimumParameterIntValue("maxit",1);

	AddParameter(ParameterType_Float,"thresh","Stopping threshold");
	SetParameterDescription("thresh","Minimum residial for stopping the algorithm.");
	SetDefaultParameterFloat("thresh",0.001);
	SetMinimumParameterFloatValue("thresh",0);
    
	AddParameter(ParameterType_Float,"margin","Margin for stabilizing");
	SetParameterDescription("margin","Margin in superpixel widths for stabilizing the scaled algorithm");
	SetDefaultParameterFloat("margin",3);
	SetMinimumParameterFloatValue("margin",0);

	AddParameter(ParameterType_Directory,  "tmpdir",    "Temporary directory");
	SetParameterDescription( "tmpdir", "Temporary directory for storing parts of the output." );

	AddParameter(ParameterType_Choice,"tiling","Tiling mode (auto or manual)");
	AddChoice("tiling.auto","Automatic tiling mode");
	AddChoice("tiling.manual","Manual tiling mode");
    
	AddParameter(ParameterType_Int, "tiling.auto.ram", "Available ram (Mb)");
	SetParameterDescription("tiling.auto.ram", "Available ram to be used per MPI process in Mb");
	SetDefaultParameterInt("tiling.auto.ram", 128);
	SetMinimumParameterIntValue("tiling.auto.ram", 0);

	AddParameter(ParameterType_Int, "tiling.manual.nx", "Number of tiles (X-axis)");
	SetParameterDescription("tiling.manual.nx", "Number of tiles along the X-axis.");
	SetMinimumParameterIntValue("tiling.manual.nx", 1);

	AddParameter(ParameterType_Int, "tiling.manual.ny", "Number of tiles (Y-axis)");
	SetParameterDescription("tiling.manual.ny", "Number of tiles along the Y-axis.");
	SetMinimumParameterIntValue("tiling.manual.ny", 1);

	GDALSetCacheMax(0);
      }

      void DoUpdateParameters()
      {
      }

      void DoExecute()
      {

	
	typename otb::MPIConfig::Pointer mpiConfig = otb::MPIConfig::Instance();
	const unsigned int myRank = mpiConfig->GetMyRank();

	VectorImageType::Pointer imageIn = GetParameterImage("in");
	const std::string inputName = GetParameterString("in");
	imageIn->UpdateOutputInformation();
	const std::string outputName = GetParameterString("out");
	const unsigned int spatialWidth = GetParameterFloat("spw");
	const double distanceWeight = GetParameterFloat("dw");
	const unsigned int maxIterations = GetParameterInt("maxit");
	const double threshold = GetParameterFloat("thresh");
	unsigned int margin = GetParameterInt("margin");
	unsigned int nbTilesX = 1;
	unsigned int nbTilesY = 1;

	std::stringstream s_prefix;
	s_prefix << GetParameterString("tmpdir") << "/" << "temp" ;
	std::string prefix = s_prefix.str();
	s_prefix << ".vrt";
	std::string outputVrt = s_prefix.str();

	switch(GetParameterInt("tiling"))
	  {
	  case AUTO :
	    {
	      const unsigned int maxMemory = GetParameterInt("tiling.auto.ram");
	      //Auto Tiling mode
	      const unsigned int X = imageIn->GetLargestPossibleRegion().GetSize()[0];
	      const unsigned int Y = imageIn->GetLargestPossibleRegion().GetSize()[1];
	      const unsigned int nbComps = imageIn->GetNumberOfComponentsPerPixel();
	      const unsigned int pin = 2*nbComps; //because reader casts to unsigned short
	      const unsigned int pout = 4; //unsigned int label
	      const unsigned int pc = (nbComps+2)*8+4; //centroid size

	      //Reading config
	      double alpha = 2*pin;
	      double beta = (2*pin + pout)*2*spatialWidth*margin;
	      const unsigned int readingNx = vcl_ceil(X*alpha/(vcl_sqrt(4*beta*beta + alpha*maxMemory*1024*1024) - beta));
	      const unsigned int readingNy = vcl_ceil(Y*alpha/(vcl_sqrt(4*beta*beta + alpha*maxMemory*1024*1024) - beta));
    
	      //Algo config
	      alpha = pin+pout+(double)2*pc/spatialWidth/spatialWidth;
	      beta = (pin + pout)*2*spatialWidth*margin;
	      const unsigned int algoNx = vcl_ceil(X*alpha/(vcl_sqrt(4*beta*beta + alpha*maxMemory*1024*1024) - beta));
	      const unsigned int algoNy = vcl_ceil(Y*alpha/(vcl_sqrt(4*beta*beta + alpha*maxMemory*1024*1024) - beta));
    
	      nbTilesX = readingNx > algoNx ? readingNx : algoNx;  
	      nbTilesY = readingNy > algoNy ? readingNy : algoNy;

	      std::cout << "Auto tiling mode selected, available RAM = " << maxMemory << "MB" << std::endl;
	      std::cout << "Number of x tiles = "<<  nbTilesX << std::endl;
	      std::cout << "Number of y tiles = "<<  nbTilesY << std::endl;
	      break;
	    };
	  case MANUAL :
	    {
	      nbTilesX = GetParameterInt("tiling.manual.nx");
	      nbTilesY = GetParameterInt("tiling.manual.ny");
	    };
	  }
	//SlicScheduler will write the tiled images and a .vrt in the tmpdir
	std::cout << "Starting segmentation" << "\n";
	SLICSchedulerType::Pointer slicScheduler = SLICSchedulerType::New();
	slicScheduler->SetInputImage(imageIn);
	slicScheduler->SetInputName(inputName); 
	slicScheduler->SetPrefix(prefix);
	slicScheduler->SetSpatialWidth(spatialWidth);
	slicScheduler->SetSpatialDistanceWeight(distanceWeight);
	slicScheduler->SetMaxIterationNumber(maxIterations);
	slicScheduler->SetThreshold(threshold);
	slicScheduler->SetNbTilesX(nbTilesX);
	slicScheduler->SetNbTilesY(nbTilesY);
	slicScheduler->SetMargin(margin);
	slicScheduler->Run();

	std::cout << "Segmentation finished" << "\n";
	
	//Rank 0 thread must have time to write the .vrt
	mpiConfig->barrier();

	//Convert .vrt into .tif using MPI : requires SPTW
	LabelReaderType::Pointer labelReader = LabelReaderType::New();
	labelReader->SetFileName(outputVrt);
	labelReader->Update();
	SetParameterOutputImage<UInt32ImageType> ("out",dynamic_cast<UInt32ImageType*>(labelReader->GetOutput()));

      }
    };
  }
}

OTB_APPLICATION_EXPORT(otb::Wrapper::SLIC)


