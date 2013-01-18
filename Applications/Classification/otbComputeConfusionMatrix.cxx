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

#include "otbOGRDataSourceToLabelImageFilter.h"
#include "itkImageRegionConstIterator.h"
#include "otbOGRDataSourceWrapper.h"

#include "itkImageRegionSplitter.h"
#include "otbStreamingTraits.h"

namespace otb
{
namespace Wrapper
{

class ComputeConfusionMatrix : public Application
{
public:
  /** Standard class typedefs. */
  typedef ComputeConfusionMatrix Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(ComputeConfusionMatrix, otb::Application);

  typedef itk::ImageRegionConstIterator<Int32ImageType> ImageIteratorType;
  
  typedef otb::OGRDataSourceToLabelImageFilter
    <Int32ImageType>                                    RasterizeFilterType;
  
  typedef otb::StreamingTraits<Int32ImageType>        StreamingTraitsType;
  
  typedef itk::ImageRegionSplitter<2>                 SplitterType;
  
  typedef Int32ImageType::RegionType                  RegionType;

private:
  void DoInit()
  {
  SetName("ComputeConfusionMatrix");
  SetDescription("Computes the confusion matrix of a classification");

  // Documentation
  SetDocName("Confusion matrix Computation");
  SetDocLongDescription("This application computes the confusion matrix of a classification  relatively to a ground truth. This ground truth can be given as a raster or a vector data.");
  SetDocLimitations("None");
  SetDocAuthors("OTB-Team");
  SetDocSeeAlso(" ");

  AddDocTag(Tags::Learning);

  AddParameter(ParameterType_InputImage, "in", "Input Image");
  SetParameterDescription( "in", "The input classification image." );

  AddParameter(ParameterType_OutputFilename, "out", "Matrix output");
  SetParameterDescription("out", "Filename to store the output matrix (csv format)");
  
  AddParameter(ParameterType_Choice,"ref","Ground truth");
  SetParameterDescription("ref","Choice of ground truth format");
  
  AddChoice("ref.raster","Ground truth as a raster image");
  
  AddChoice("ref.vector","Ground truth as a vector data file");
  
  AddParameter(ParameterType_InputImage,"ref.raster.in","Input reference image");
  SetParameterDescription("ref.raster.in","Input image containing the ground truth labels");
  
  AddParameter(ParameterType_InputFilename,"ref.vector.in","Input reference vector data");
  SetParameterDescription("ref.vector.in", "Input vector data of the ground truth");
  
  AddParameter(ParameterType_String,"ref.vector.field","Field name");
  SetParameterDescription("ref.vector.field","Field name containing the label values");
  SetParameterString("ref.vector.field","dn");
  MandatoryOff("ref.vector.field");
  DisableParameter("ref.vector.field");
  
  AddParameter(ParameterType_Int,"labels","Number of labels");
  SetParameterDescription("labels","Number of labels in the classification. The label values shall be contiguous and start from 1.");
  SetDefaultParameterInt("labels",2);
  MandatoryOff("labels");
  DisableParameter("labels");
  
  AddParameter(ParameterType_Int,"nodata","Value for nodata pixels");
  SetParameterDescription("nodata","This value will be used to discard pixels from the ground truth");
  SetDefaultParameterInt("nodata",0);
  MandatoryOff("nodata");
  DisableParameter("nodata");
  
  AddRAMParameter();

  // Doc example parameter settings
  SetDocExampleParameterValue("in", "clLabeledImageQB1.tif");
  SetDocExampleParameterValue("out", "confusion.txt");
  SetDocExampleParameterValue("ref", "vector");
  SetDocExampleParameterValue("ref.vector.in","VectorData_QB1_bis.shp");
  SetDocExampleParameterValue("ref.vector.field","Class");
  SetDocExampleParameterValue("labels","4");
  }

  void DoUpdateParameters()
  {
    // Nothing to do here : all parameters are independent
  }

  void DoExecute()
  {
    Int32ImageType* input = this->GetParameterImage<Int32ImageType>("in");
    
    std::string field;
    int nodata = this->GetParameterInt("nodata");
    
    //Init Conf Matrix
    unsigned int nbClasses = this->GetParameterInt("labels");
    m_Matrix.resize(nbClasses);
    for(unsigned int i=0; i<nbClasses; i++ )
      {
      m_Matrix[i].assign(nbClasses,0);
      }
    
    Int32ImageType::Pointer reference;
    otb::ogr::DataSource::Pointer ogrRef;
    RasterizeFilterType::Pointer rasterizeReference = RasterizeFilterType::New();
    
    if (GetParameterString("ref") == "raster")
      {
      reference = this->GetParameterImage<Int32ImageType>("ref.raster.in");
      }
    else
      {
      ogrRef = otb::ogr::DataSource::New(GetParameterString("ref.vector.in"), otb::ogr::DataSource::Modes::Read);
      field = this->GetParameterString("ref.vector.field");
      
      rasterizeReference->AddOGRDataSource(ogrRef);
      rasterizeReference->SetOutputParametersFromImage(input);
      rasterizeReference->SetBackgroundValue(nodata);
      rasterizeReference->SetBurnAttribute(field.c_str());
      
      reference = rasterizeReference->GetOutput();
      reference->UpdateOutputInformation();
      }

    // Prepare local streaming
    SplitterType::Pointer splitter = SplitterType::New();
    unsigned int numberOfStreamDivisions = StreamingTraitsType::CalculateNumberOfStreamDivisions(
      input,
      input->GetLargestPossibleRegion(),
      splitter,
      otb::SET_BUFFER_MEMORY_SIZE,
      0, 1048576*GetParameterInt("ram"), 0);
    RegionType streamRegion;
    
    otbAppLogINFO("Number of stream divisions : "<<numberOfStreamDivisions);
    
    for (unsigned int index=0; index<numberOfStreamDivisions; index++)
      {
      streamRegion = splitter->GetSplit(index, numberOfStreamDivisions, input->GetLargestPossibleRegion());
      
      input->SetRequestedRegion(streamRegion);
      input->PropagateRequestedRegion();
      input->UpdateOutputData();
      
      reference->SetRequestedRegion(streamRegion);
      reference->PropagateRequestedRegion();
      reference->UpdateOutputData();
      
      ImageIteratorType itInput(input, streamRegion);
      itInput.GoToBegin();
      
      ImageIteratorType itRef(reference, streamRegion);
      itRef.GoToBegin();
      
      while (!itInput.IsAtEnd())
        {
        if (itRef.Get() != nodata)
          {
          if (itRef.Get()>0 && itRef.Get()<=static_cast<int>(nbClasses) &&
              itInput.Get()>0 && itInput.Get()<=static_cast<int>(nbClasses))
            {
            m_Matrix[itInput.Get()-1][itRef.Get()-1] ++;
            }
          }
        ++ itInput;
        ++ itRef;
        }
      }
    
    std::ofstream outFile;
    outFile.open(this->GetParameterString("out").c_str());
    outFile<<std::fixed;
    outFile.precision(10);
    for(unsigned int j=0; j<nbClasses; j++ )
      {
      for(unsigned int i=0; i<nbClasses; i++ )
        {
        outFile << m_Matrix[i][j];
        if (i<(nbClasses-1))
          {
          outFile<<"\t";
          }
        else
          {
          outFile<<std::endl;
          }
        }
      }
    outFile.close();
    
  }

  std::vector<std::vector<unsigned long> > m_Matrix;

};

}
}
OTB_APPLICATION_EXPORT(otb::Wrapper::ComputeConfusionMatrix)
