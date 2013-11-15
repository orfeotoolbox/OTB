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

#include "otbVectorImage.h"
#include "otbImage.h"
#include "otbImageFileWriter.h"
#include "otbVectorDataFileReader.h"

#include "otbVectorDataToLabelMapWithAttributesFilter.h"
#include "otbSpatialisationFilter.h"
#include "otbProspectModel.h"
#include "otbSailModel.h"
#include "otbImageSimulationMethod.h"
#include "otbAttributesMapLabelObject.h"

#include "otbImageFileReader.h"
#include "itkListSample.h"
#include "itkWeightedCentroidKdTreeGenerator.h"
#include "itkKdTreeBasedKmeansEstimator.h"
#include "otbKMeansImageClassificationFilter.h"
#include "itkImageRegionIterator.h"

int otbImageSimulationMethodKMeansClassif(int argc, char * argv[])
{
   const char * satRSRFilename = argv[1];
   unsigned int nbBands = static_cast<unsigned int>(atoi(argv[2]));
   const char * rootPath = argv[3];
   unsigned int radius = atoi(argv[4]);
   const char * outfilename = argv[5];
   const char * outLabelfilename = argv[6];

   const unsigned int nbClasses = 4;
   double ConvergenceThreshold = 0.0001;
   unsigned int NumberOfIterations = 1000;

   typedef unsigned short LabelType;
   const unsigned int Dimension = 2;
   typedef otb::Image<LabelType, Dimension>                                           LabelImageType;
   typedef otb::VectorImage<double, Dimension>                                        OutputImageType;
   typedef otb::ImageFileWriter<OutputImageType>                                      ImageWriterType;
   typedef otb::ImageFileWriter<LabelImageType>                                       LabelImageWriterType;
   typedef otb::VectorData<double, Dimension>                                         VectorDataType;
   typedef otb::AttributesMapLabelObject<LabelType, Dimension, std::string>           LabelObjectType;
   typedef itk::LabelMap<LabelObjectType>                                             LabelMapType;
   typedef otb::VectorDataFileReader<VectorDataType>                                  VectorDataFileReaderType;

   typedef otb::SpatialisationFilter<LabelMapType>                                    SpatialisationFilterType;
   typedef otb::ProspectModel                                                         SimulationStep1Type;
   typedef otb::SailModel                                                             SimulationStep2Type;
   typedef otb::ProlateInterpolateImageFunction<LabelImageType>                       FTMType;
   typedef otb::ImageSimulationMethod<VectorDataType, SpatialisationFilterType,
    SimulationStep1Type, SimulationStep2Type, FTMType , OutputImageType>               ImageSimulationMethodType;

   typedef OutputImageType::PixelType                               SampleType;
   typedef itk::Statistics::ListSample<SampleType>                          ListSampleType;
   typedef itk::Statistics::WeightedCentroidKdTreeGenerator<ListSampleType> TreeGeneratorType;
   typedef TreeGeneratorType::KdTreeType                                    TreeType;
   typedef itk::Statistics::KdTreeBasedKmeansEstimator<TreeType>            EstimatorType;

   typedef otb::KMeansImageClassificationFilter<OutputImageType, LabelImageType, nbClasses> ClassificationFilterType;
   typedef ClassificationFilterType::KMeansParametersType                    KMeansParametersType;

   typedef itk::ImageRegionIterator<OutputImageType> ImageRegionIteratorType;


   /** Instantiation of pointer objects*/
   ImageWriterType::Pointer writer = ImageWriterType::New();
   LabelImageWriterType::Pointer labelWriter = LabelImageWriterType::New();
   ImageSimulationMethodType::Pointer imageSimulation = ImageSimulationMethodType::New();
   SpatialisationFilterType::Pointer spatialisationFilter = SpatialisationFilterType::New();


   ClassificationFilterType::Pointer classifier = ClassificationFilterType::New();

   SpatialisationFilterType::SizeType objectSize;
   objectSize[0]=300;
   objectSize[1]=300;

   SpatialisationFilterType::SizeType nbOjects;
   nbOjects[0]=2;
   nbOjects[1]=2;

   std::vector<std::string> pathVector(nbClasses);
   pathVector[0]="JHU/becknic/rocks/sedimentary/powder/0_75/txt/greywa1f.txt";
   pathVector[1]="";
   pathVector[2]="JHU/becknic/water/txt/coarse.txt";
   pathVector[3]="JHU/becknic/soils/txt/0015c.txt";

   std::vector<std::string> areaVector(nbClasses);
   areaVector[0]="sedimentaryRock";
   areaVector[1]="prosail";
   areaVector[2]="water";
   areaVector[3]="soils";

   std::vector<LabelType> labels(nbClasses);
   labels[0]=1;
   labels[1]=2;
   labels[2]=3;
   labels[3]=4;

   spatialisationFilter->SetObjectSize(objectSize);
   spatialisationFilter->SetNumberOfObjects(nbOjects);
   spatialisationFilter->SetPathVector(pathVector);
   spatialisationFilter->SetAreaVector(areaVector);
   spatialisationFilter->SetLabels(labels);


   imageSimulation->SetSpatialisation(spatialisationFilter);
   imageSimulation->SetNumberOfComponentsPerPixel(nbBands);
   imageSimulation->SetSatRSRFilename(satRSRFilename);
   imageSimulation->SetPathRoot(rootPath);
   imageSimulation->SetRadius(radius);
//    imageSimulation->SetMean();
//    imageSimulation->SetVariance();
   imageSimulation->UpdateData();

   imageSimulation->GetOutputReflectanceImage()->Update();

   //get all the pixel of the image for KMeans centroid estimation
   OutputImageType::IndexType centroidIndex;
   centroidIndex[0]=objectSize[0]/2;
   centroidIndex[1]=objectSize[1]/2;

   EstimatorType::ParametersType initialCentroids(nbBands * nbClasses);
   ImageRegionIteratorType it(imageSimulation->GetOutputReflectanceImage(), imageSimulation->GetOutputReflectanceImage()->GetLargestPossibleRegion());
   it.GoToBegin();
   ListSampleType::Pointer listSample = ListSampleType::New();
   listSample->SetMeasurementVectorSize(nbBands);
   unsigned int x=0;
   unsigned int classIndex=0;
   while(!it.IsAtEnd())
   {
      if(it.GetIndex()==centroidIndex)
      {
         for(unsigned int j=0; j<nbBands; ++j)
         {
            initialCentroids[j+classIndex*nbBands]=it.Get()[j];
         }
         classIndex++;

         if(x<(nbOjects[0]-1))
         {
            centroidIndex[0]+=objectSize[0];
            x++;
         }
         else
         {
            x=0;
            centroidIndex[0]=objectSize[0]/2;
            centroidIndex[1]+=objectSize[1];
         }
      }
      listSample->PushBack(it.Get());
      ++it;
   }


   TreeGeneratorType::Pointer treeGenerator = TreeGeneratorType::New();
   treeGenerator->SetSample(listSample);
   treeGenerator->SetBucketSize(100 / (10 * nbClasses));
   treeGenerator->Update();

   EstimatorType::Pointer estimator = EstimatorType::New();
   estimator->SetKdTree(treeGenerator->GetOutput());
   estimator->SetParameters(initialCentroids);
   estimator->SetMaximumIteration(NumberOfIterations);
   estimator->SetCentroidPositionChangesThreshold(ConvergenceThreshold);
   estimator->StartOptimization();

   classifier->SetCentroids(estimator->GetParameters());
   classifier->SetInput(imageSimulation->GetOutputReflectanceImage());

   //Write the result to an image file
   writer->SetFileName(outfilename);
   writer->SetInput(imageSimulation->GetOutputReflectanceImage());
   writer->Update();

   labelWriter->SetFileName(outLabelfilename);
//    labelWriter->SetInput(imageSimulation->GetOutputLabelImage());
   labelWriter->SetInput(classifier->GetOutput());
   labelWriter->Update();


   return EXIT_SUCCESS;
}


