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

#include "otbImageSampleExtractorFilter.h"

namespace otb
{
namespace Wrapper
{

class SampleExtraction : public Application
{
public:
  /** Standard class typedefs. */
  typedef SampleExtraction              Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(SampleExtraction, otb::Application);

  /** Filters typedef */
  typedef otb::ImageSampleExtractorFilter<FloatVectorImageType> FilterType;

private:
  SampleExtraction()
    {
   
    }

  void DoInit()
  {
    SetName("SampleExtraction");
    SetDescription("Extracts samples values from an image.");

    // Documentation
    SetDocName("Sample Extraction");
    SetDocLongDescription("The application extracts samples values from an"
      "image using positions contained in a vector data file. ");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag(Tags::Learning);

    AddParameter(ParameterType_InputImage,  "in",   "InputImage");
    SetParameterDescription("in", "Support image");

    AddParameter(ParameterType_InputFilename, "vec", "Input sampling positions");
    SetParameterDescription("vec","Vector data file containing sampling"
                                  "positions. (OGR format)");
    
    AddParameter(ParameterType_OutputFilename, "out", "Output samples");
    SetParameterDescription("out","Output vector data file storing sample"
      "values (OGR format). If not given, the input vector data file is updated");
    MandatoryOff("out");

    AddParameter(ParameterType_String, "pre", "Output field prefix");
    SetParameterDescription("pre","Prefix used to form the field names that"
      "will contain the extracted values.");
    MandatoryOff("pre");
    SetParameterString("pre", "value_");

    AddParameter(ParameterType_String, "field", "Field Name");
    SetParameterDescription("field","Name of the field carrying the class"
      "name in the input vectors. This field is copied to output.");
    MandatoryOff("field");
    SetParameterString("field", "class");
    
    AddParameter(ParameterType_Int, "layer", "Layer Index");
    SetParameterDescription("layer", "Layer index to read in the input vector file.");
    MandatoryOff("layer");
    SetDefaultParameterInt("layer",0);
    
    AddRAMParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "support_image.tif");
    SetDocExampleParameterValue("vec", "sample_positions.sqlite");
    SetDocExampleParameterValue("pre","band_");
    SetDocExampleParameterValue("field", "label");
    SetDocExampleParameterValue("out","sample_values.sqlite");
  }

  void DoUpdateParameters()
  {
    // Nothing to do
  }

  void DoExecute()
    {
    
    ogr::DataSource::Pointer vectors;
    ogr::DataSource::Pointer output;
    if (IsParameterEnabled("out") && HasValue("out"))
      {
      vectors = ogr::DataSource::New(this->GetParameterString("vec"));
      output = ogr::DataSource::New(this->GetParameterString("out"),
                                    ogr::DataSource::Modes::Overwrite);
      }
    else
      {
      // Update mode
      vectors = ogr::DataSource::New(this->GetParameterString("vec"),
                                    ogr::DataSource::Modes::Update_LayerUpdate);
      output = vectors;
      }
    
    FilterType::Pointer filter = FilterType::New();
    filter->SetInput(this->GetParameterImage("in"));
    filter->SetLayerIndex(this->GetParameterInt("layer"));
    filter->SetSamplePositions(vectors);
    filter->SetOutputOGRData(output);
    filter->SetClassFieldName(this->GetParameterString("field"));
    filter->SetOutputFieldPrefix(this->GetParameterString("pre"));
    
    AddProcess(filter->GetStreamer(),"Extracting sample values...");
    
    filter->Update();
    }

};

} // end of namespace Wrapper
} // end of namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::SampleExtraction)
