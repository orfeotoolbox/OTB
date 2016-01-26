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

#include "otbMultivariateAlterationDetectorImageFilter.h"

namespace otb
{
namespace Wrapper
{

class MultivariateAlterationDetector: public Application
{
public:
  /** Standard class typedefs. */
  typedef MultivariateAlterationDetector          Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(MultivariateAlterationDetector, otb::Wrapper::Application);

private:
  void DoInit()
  {
    SetName("MultivariateAlterationDetector");
    SetDescription("Multivariate Alteration Detector");

    // Documentation
    SetDocName("Multivariate alteration detector");
    SetDocLongDescription("This application detects change between two given images.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" This filter implements the Multivariate Alteration Detector, based "
                  " on the following work: \n"
                  " A. A. Nielsen and K. Conradsen, Multivariate alteration detection"
                  " (mad) in multispectral, bi-temporal image data: a new approach to"
                  " change detection studies, Remote Sens. Environ., vol. 64,"
                  " pp. 1-19, (1998) \n"
                  " \n"
                  " Multivariate Alteration Detector takes two images as inputs and "
                  " produce a set of N change maps as a VectorImage (where N is the "
                  " maximum of number of bands in first and second image) with the "
                  " following properties: \n"
                  " - Change maps are differences of a pair of linear combinations of "
                  " bands from image 1 and bands from image 2 chosen to maximize the "
                  " correlation. \n"
                  " - Each change map is orthogonal to the others. \n"
                  " \n"
                  " This is a statistical method which can handle different modalities "
                  " and even different bands and number of bands between images. \n"
                  " \n"
                  " If numbers of bands in image 1 and 2 are equal, then change maps "
                  " are sorted by increasing correlation. If number of bands is "
                  " different, the change maps are sorted by decreasing correlation. \n"
                  " \n"
                  " The GetV1() and GetV2() methods allow retrieving the linear "
                  " combinations used to generate the Mad change maps as a vnl_matrix of "
                  " double, and the GetRho() method allows retrieving the correlation "
                  " associated to each Mad change maps as a vnl_vector. \n"
                  " \n"
                  " This filter has been implemented from the Matlab code kindly made "
                  " available by the authors here: \n"
                  " http://www2.imm.dtu.dk/~aa/software.html \n"
                  " \n"
                  " Both cases (same and different number of bands) have been validated "
                  " by comparing the output image to the output produced by the Matlab "
                  " code, and the reference images for testing have been generated from "
                  " the Matlab code using Octave." );

    AddDocTag(Tags::FeatureExtraction);

    AddParameter(ParameterType_InputImage,  "in1", "Input Image 1");
    SetParameterDescription("in1","Image which describe initial state of the scene.");
    AddParameter(ParameterType_InputImage,  "in2", "Input Image 2");
    SetParameterDescription("in2","Image which describe scene after perturbations.");
    AddParameter(ParameterType_OutputImage, "out", "Change Map");
    SetParameterDescription("out","Image of detected changes.");

    AddRAMParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue("in1", "Spot5-Gloucester-before.tif");
    SetDocExampleParameterValue("in2", "Spot5-Gloucester-after.tif");
    SetDocExampleParameterValue("out", "detectedChangeImage.tif");
  }

  void DoUpdateParameters()
  {
  }

  void DoExecute()
  {
    typedef otb::MultivariateAlterationDetectorImageFilter<
        FloatVectorImageType,
        FloatVectorImageType> ChangeFilterType;

    ChangeFilterType::Pointer changeFilter = ChangeFilterType::New();

    changeFilter->SetInput1(GetParameterImage("in1"));
    changeFilter->SetInput2(GetParameterImage("in2"));
    changeFilter->GetOutput()->UpdateOutputInformation();

    otbAppLogINFO("Input 1 mean: "<<changeFilter->GetMean1());
    otbAppLogINFO("Input 2 mean: "<<changeFilter->GetMean2());
    otbAppLogINFO("Input 1 transform: "<<changeFilter->GetV1());
    otbAppLogINFO("Input 2 transform: "<<changeFilter->GetV2());
    otbAppLogINFO("Rho: "<<changeFilter->GetRho());

    m_Ref = changeFilter;

    SetParameterOutputImage("out", changeFilter->GetOutput());
  }

  itk::LightObject::Pointer m_Ref;

};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::MultivariateAlterationDetector)
