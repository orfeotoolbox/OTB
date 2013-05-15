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

#include "otbStatisticsXMLFileWriter.h"
#include "otbStreamingStatisticsVectorImageFilter.h"

namespace otb
{
namespace Wrapper
{

class ComputeImagesStatistics: public Application
{
public:
  /** Standard class typedefs. */
  typedef ComputeImagesStatistics Self;
  typedef Application Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(ComputeImagesStatistics, otb::Application);

private:
  void DoInit()
  {
    SetName("ComputeImagesStatistics");
    SetDescription("Computes global mean and standard deviation for each band from a set of images and optionally saves the results in an XML file.");
    SetDocName("Compute Images second order statistics");
    SetDocLongDescription("This application computes a global mean and standard deviation for each band of a set of images and optionally saves the results in an XML file. The output XML is intended to be used an input for the TrainImagesClassifier application to normalize samples before learning.");
    SetDocLimitations("Each image of the set must contain the same bands as the others (i.e. same types, in the same order).");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("Documentation of the TrainImagesClassifier application.");
 
    AddDocTag(Tags::Learning);
    AddDocTag(Tags::Analysis);

    AddParameter(ParameterType_InputImageList, "il", "Input images");
    SetParameterDescription( "il", "List of input images filenames." );

    AddParameter(ParameterType_OutputFilename, "out", "Output XML file");
    SetParameterDescription( "out", "XML filename where the statistics are saved for future reuse." );
    MandatoryOff("out");

   // Doc example parameter settings
   SetDocExampleParameterValue("il", "QB_1_ortho.tif");
   SetDocExampleParameterValue("out", "EstimateImageStatisticsQB1.xml");
  }

  void DoUpdateParameters()
  {
    // Nothing to do here : all parameters are independent
  }

  void DoExecute()
  {
    //Statistics estimator
    typedef otb::StreamingStatisticsVectorImageFilter<FloatVectorImageType> StreamingStatisticsVImageFilterType;

    // Samples
    typedef double ValueType;
    typedef itk::VariableLengthVector<ValueType> MeasurementType;

    unsigned int nbSamples = 0;
    unsigned int nbBands = 0;

    // Build a Measurement Vector of mean
    MeasurementType mean;

    // Build a MeasurementVector of variance
    MeasurementType variance;

    FloatVectorImageListType* imageList = GetParameterImageList("il");

    //Iterate over all input images
    for (unsigned int imageId = 0; imageId < imageList->Size(); ++imageId)
      {
      FloatVectorImageType* image = imageList->GetNthElement(imageId);

      if (nbBands == 0)
        {
        nbBands = image->GetNumberOfComponentsPerPixel();
        }
      else if (nbBands != image->GetNumberOfComponentsPerPixel())
        {
        itkExceptionMacro(<< "The image #" << imageId << " has " << image->GetNumberOfComponentsPerPixel()
            << " bands, while the first one has " << nbBands );
        }

      FloatVectorImageType::SizeType size = image->GetLargestPossibleRegion().GetSize();

      //Set the measurement vectors size if it's the first iteration
      if (imageId == 0)
        {
        mean.SetSize(nbBands);
        mean.Fill(0.);
        variance.SetSize(nbBands);
        variance.Fill(0.);
        }

      // Compute Statistics of each VectorImage
      StreamingStatisticsVImageFilterType::Pointer statsEstimator = StreamingStatisticsVImageFilterType::New();
      statsEstimator->SetInput(image);
      statsEstimator->Update();
      mean += statsEstimator->GetMean();
      for (unsigned int itBand = 0; itBand < nbBands; itBand++)
        {
        variance[itBand] += (size[0] * size[1] - 1) * (statsEstimator->GetCovariance())(itBand, itBand);
        }
      //Increment nbSamples
      nbSamples += size[0] * size[1] * nbBands;
      }

    //Divide by the number of input images to get the mean over all layers
    mean /= imageList->Size();
    //Apply the pooled variance formula
    variance /= (nbSamples - imageList->Size());

    MeasurementType stddev;
    stddev.SetSize(nbBands);
    stddev.Fill(0.);
    for (unsigned int i = 0; i < variance.GetSize(); ++i)
      {
      stddev[i] = vcl_sqrt(variance[i]);
      }

    if( HasValue( "out" )==true )
      {
      // Write the Statistics via the statistic writer
      typedef otb::StatisticsXMLFileWriter<MeasurementType> StatisticsWriter;
      StatisticsWriter::Pointer writer = StatisticsWriter::New();
      writer->SetFileName(GetParameterString("out"));
      writer->AddInput("mean", mean);
      writer->AddInput("stddev", stddev);
      writer->Update();
      }
    else
      {
      otbAppLogINFO("Mean: "<<mean<<std::endl);
      otbAppLogINFO("Standard Deviation: "<<stddev<<std::endl);
      }
  }

  itk::LightObject::Pointer m_FilterRef;
};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::ComputeImagesStatistics)
