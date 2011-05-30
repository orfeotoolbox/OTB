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
#include "otbDSFuzzyModelEstimation.h"

#include <iostream>
#include "otbCommandLineArgumentParser.h"

#include "otbVectorImage.h"
#include "otbImageFileReader.h"

#include "otbVectorData.h"
#include "otbVectorDataFileReader.h"
#include "otbVectorDataFileWriter.h"
#include "otbImageToEnvelopeVectorDataFilter.h"
#include "otbVectorDataToRandomLineGenerator.h"

#include "otbVectorDataProjectionFilter.h"  //Envelope TO WGS84 (OSM friendly)
#include "otbVectorDataIntoImageProjectionFilter.h" //WGS84 to index

#include "otbVectorDataToRoadDescriptionFilter.h"
#include "otbVectorDataToDSValidatedVectorDataFilter.h"

#include "itkAmoebaOptimizer.h"
#include "otbStandardDSCostFunction.h"

#include "otbFuzzyDescriptorsModelManager.h"

#include "otbVectorDataProjectionFilter.h"
#include "otbVectorDataTransformFilter.h"
#include "itkAffineTransform.h"

//  The following piece of code implements an observer
//  that will monitor the evolution of the registration process.
#include "itkCommand.h"
class CommandIterationUpdate : public itk::Command
{
public:
typedef  CommandIterationUpdate   Self;
typedef  itk::Command             Superclass;
typedef itk::SmartPointer<Self>   Pointer;
itkNewMacro( Self );
protected:
CommandIterationUpdate() {};
public:
typedef itk::AmoebaOptimizer         OptimizerType;
typedef   const OptimizerType   *    OptimizerPointer;

void Execute(itk::Object *caller, const itk::EventObject & event)
{
  Execute( (const itk::Object *)caller, event);
}

void Execute(const itk::Object * object, const itk::EventObject & event)
{
  OptimizerPointer optimizer =
      dynamic_cast< OptimizerPointer >( object );
  if( ! itk::IterationEvent().CheckEvent( &event ) )
    {
    return;
    }
  std::cout << optimizer->GetCachedValue() << "   ";
  std::cout << optimizer->GetCachedCurrentPosition() << std::endl;
}
};


//namespace otb
//{

int otb::DSFuzzyModelEstimation::Describe(ApplicationDescriptor* descriptor)
{
  descriptor->SetName("DSFuzzyModelEstimation");
  descriptor->SetDescription("Estimate feature fuzzy model parameters using an image and an VectorData as support");
  descriptor->AddOption("InputImage", "Image Support to estimate the models on",
                        "img", 1,  true, ApplicationDescriptor::InputImage);
  descriptor->AddOption("BuildingsDB", "Building DataBase Support to estimate the models on",
                        "db", 1,   true, ApplicationDescriptor::FileName);
  descriptor->AddOption("InputVectorData", "Ground Truth Vector Data",
                        "vdin", 1, true, ApplicationDescriptor::FileName);
  descriptor->AddOption("InputNegativeVectorData", "Negative samples Vector Data",
                        "nsin", 1, false, ApplicationDescriptor::FileName);
  descriptor->AddOption("Output", "Output Model File Name",
                        "out", 1,  true, ApplicationDescriptor::FileName);
  descriptor->AddOption("Hypothesis", "Dempster Shafer study hypothesis",
                        "hyp", 3, false, ApplicationDescriptor::StringList);
  descriptor->AddOption("Criterion", "Dempster Shafer Criterion (by default (Belief+Plausibility)/2)",
                        "cri", 1, false, ApplicationDescriptor::String);
  descriptor->AddOption("weighting", "Coefficient between 0 and 1 to promote undetection or false detections (default 0.5)",
                        "wgt", 1, false, ApplicationDescriptor::Real);
  descriptor->AddOption("MaximumNumberOfIterations", "Maximum Number os Optimizer Iteration",
                        "MaxNbIt", 1, false, ApplicationDescriptor::Integer);
  descriptor->AddOption("DEMDirectory", "DEM directory",
                        "dem", 1, false, ApplicationDescriptor::DirectoryName);
  descriptor->AddOption("OptimizerObserver", "Activate or not the optimizer observer",
                        "OptObs", 0, false, ApplicationDescriptor::Boolean);
  descriptor->AddOption("GenerateShp", "Activate the output of intermediate vector data (only work with cartographic image)",
                        "grtShp", 0, false, ApplicationDescriptor::Boolean);

  return EXIT_SUCCESS;
}


int otb::DSFuzzyModelEstimation::Execute(otb::ApplicationOptionsResult* parseResult)
{
  typedef otb::VectorData<double>                     VectorDataType;
  typedef VectorDataType::ValuePrecisionType          PrecisionType;
  typedef VectorDataType::PrecisionType               CoordRepType;
  typedef otb::VectorDataFileReader<VectorDataType>   VectorDataReaderType;
  typedef otb::VectorDataFileWriter<VectorDataType>   VectorDataWriterType;
  typedef otb::VectorDataToRandomLineGenerator<VectorDataType>
  RandomGeneratorType;

  typedef otb::VectorImage<PrecisionType , 2>         ImageType;
  typedef otb::ImageFileReader<ImageType>             ImageReaderType;

  typedef otb::ImageToEnvelopeVectorDataFilter<ImageType, VectorDataType>
  EnvelopeFilterType;

  typedef otb::VectorDataProjectionFilter<VectorDataType, ImageType>
  VectorDataProjFilter;

  typedef otb::VectorDataIntoImageProjectionFilter<VectorDataType, ImageType>
  VectorDataReProjFilter;

  typedef otb::VectorDataToRoadDescriptionFilter<VectorDataType, ImageType>
  DescriptionFilterType;

  typedef otb::VectorDataToDSValidatedVectorDataFilter<VectorDataType, PrecisionType>
  ValidationFilterType;

  typedef otb::StandardDSCostFunction<ValidationFilterType> CostFunctionType;
  typedef CostFunctionType::LabelSetType                    LabelSetType;

  typedef itk::AmoebaOptimizer                        OptimizerType;

  //Instantiate
  ImageReaderType::Pointer                   imgReader = ImageReaderType::New();
  VectorDataReaderType::Pointer               dbReader = VectorDataReaderType::New();
  VectorDataReaderType::Pointer               vdReader = VectorDataReaderType::New();
  EnvelopeFilterType::Pointer           envelopeFilter = EnvelopeFilterType::New();
  RandomGeneratorType::Pointer       vdRandomGenerator = RandomGeneratorType::New();
  VectorDataReProjFilter::Pointer     vdReProjFilterGT = VectorDataReProjFilter::New();
  VectorDataReProjFilter::Pointer     vdReProjFilterRL = VectorDataReProjFilter::New();
  VectorDataReProjFilter::Pointer     vdReProjFilterDB = VectorDataReProjFilter::New();
  DescriptionFilterType::Pointer   descriptionFilterGT = DescriptionFilterType::New();
  DescriptionFilterType::Pointer   descriptionFilterNS = DescriptionFilterType::New();
  CostFunctionType::Pointer               costFunction = CostFunctionType::New();
  OptimizerType::Pointer                     optimizer = OptimizerType::New();

  VectorDataWriterType::Pointer               vdWriter = VectorDataWriterType::New();

  //Read the image
  imgReader->SetFileName(parseResult->GetParameterString("InputImage"));
  imgReader->UpdateOutputInformation();

  imgReader->SetGlobalWarningDisplay(0);

  //Read the building database
  dbReader->SetFileName(parseResult->GetParameterString("BuildingsDB"));
  dbReader->Update();

  //Read the vector data
  vdReader->SetFileName(parseResult->GetParameterString("InputVectorData"));
  vdReader->Update();

  //Generate the envelope
  envelopeFilter->SetInput(imgReader->GetOutput()); //->Output in WGS84
  envelopeFilter->Update();

  if (parseResult->IsOptionPresent("InputNegativeVectorData"))
    {
    //Read the negative samples vector data
    VectorDataReaderType::Pointer               nsReader = VectorDataReaderType::New();
    nsReader->SetFileName(parseResult->GetParameterString("InputNegativeVectorData"));
    nsReader->Update();
    vdReProjFilterRL->SetInputVectorData(nsReader->GetOutput());
    }
  else
    {
    //Generate Random lines
    vdRandomGenerator->SetInput(envelopeFilter->GetOutput());
    vdRandomGenerator->SetNumberOfOutputLine(vdReader->GetOutput()->Size());
    vdRandomGenerator->SetMinLineSize(4);
    vdRandomGenerator->SetMaxLineSize(20);
    vdReProjFilterRL->SetInputVectorData(vdRandomGenerator->GetOutput());
    }


  //Reproject into image index coordinates
  vdReProjFilterGT->SetInputImage(imgReader->GetOutput());
  vdReProjFilterGT->SetInputVectorData(vdReader->GetOutput());
  if( parseResult->IsOptionPresent("DEMDirectory") )
    {
    vdReProjFilterGT->SetDEMDirectory(parseResult->GetParameterString("DEMDirectory"));
    }
  vdReProjFilterGT->SetUseOutputSpacingAndOriginFromImage(true);
  vdReProjFilterGT->Update();

  vdReProjFilterRL->SetInputImage(imgReader->GetOutput());

  if( parseResult->IsOptionPresent("DEMDirectory") )
    {
    vdReProjFilterRL->SetDEMDirectory(parseResult->GetParameterString("DEMDirectory"));
    }
  vdReProjFilterRL->SetUseOutputSpacingAndOriginFromImage(true);
  vdReProjFilterRL->Update();

  vdReProjFilterDB->SetInputImage(imgReader->GetOutput());
  vdReProjFilterDB->SetInputVectorData(dbReader->GetOutput());
  if( parseResult->IsOptionPresent("DEMDirectory") )
    {
    vdReProjFilterDB->SetDEMDirectory(parseResult->GetParameterString("DEMDirectory"));
    }
  vdReProjFilterDB->SetUseOutputSpacingAndOriginFromImage(true);
  vdReProjFilterDB->Update();

  //Add Description to VectorDatas
  descriptionFilterGT->SetInput(vdReProjFilterGT->GetOutput());
  descriptionFilterGT->AddOpticalImage(imgReader->GetOutput());
  descriptionFilterGT->AddBuildingsDB(vdReProjFilterDB->GetOutput());
  descriptionFilterGT->Update();
  descriptionFilterNS->SetInput(vdReProjFilterRL->GetOutput());
  descriptionFilterNS->AddOpticalImage(imgReader->GetOutput());
  descriptionFilterNS->AddBuildingsDB(vdReProjFilterDB->GetOutput());
  descriptionFilterNS->Update();


  // Compute statistics of all the descriptors

  typedef VectorDataType::DataTreeType DataTreeType;
  typedef VectorDataType::DataNodeType DataNodeType;
  typedef itk::PreOrderTreeIterator<VectorDataType::DataTreeType>
  TreeIteratorType;

  typedef DescriptionFilterType::DescriptorsListType DescriptorsListType;

  const DescriptorsListType& descriptors = descriptionFilterGT->GetDescriptorsList();

  std::vector<double> accFirstOrderGT, accSecondOrderGT, minGT, maxGT;
  accFirstOrderGT.resize(descriptors.size());
  accSecondOrderGT.resize(descriptors.size());
  std::fill(accFirstOrderGT.begin(), accFirstOrderGT.end(), 0);
  std::fill(accSecondOrderGT.begin(), accSecondOrderGT.end(), 0);
  minGT.resize(descriptors.size());
  maxGT.resize(descriptors.size());
  unsigned int accNbElemGT = 0;

  TreeIteratorType itVectorGT(vdReProjFilterGT->GetOutput()->GetDataTree());
  for( itVectorGT.GoToBegin(); !itVectorGT.IsAtEnd(); ++itVectorGT )
    {
    if (!itVectorGT.Get()->IsRoot()
        && !itVectorGT.Get()->IsDocument()
        && !itVectorGT.Get()->IsFolder())
      {
      DataNodeType::Pointer currentGeometry = itVectorGT.Get();

      for (unsigned int i = 0; i < descriptors.size(); ++i)
        {
        double desc = currentGeometry->GetFieldAsDouble(descriptors[i]);

        accFirstOrderGT[i] += desc;
        accSecondOrderGT[i] += desc * desc;

        if (desc < minGT[i])
          {
          minGT[i] = desc;
          }
        if (desc > maxGT[i])
          {
          maxGT[i] = desc;
          }

        }
      accNbElemGT ++;
      }
    }

  TreeIteratorType itVectorNS(vdReProjFilterRL->GetOutput()->GetDataTree());
  std::vector<double> accFirstOrderNS, accSecondOrderNS, minNS, maxNS;
  minNS.resize(descriptors.size());
  maxNS.resize(descriptors.size());
  accFirstOrderNS.resize(descriptors.size());
  accSecondOrderNS.resize(descriptors.size());
  std::fill(accFirstOrderNS.begin(), accFirstOrderNS.end(), 0);
  std::fill(accSecondOrderNS.begin(), accSecondOrderNS.end(), 0);
  std::fill(minNS.begin(), minNS.end(), 1);
  std::fill(maxNS.begin(), maxNS.end(), 0);
  unsigned int accNbElemNS = 0;

  for( itVectorNS.GoToBegin(); !itVectorNS.IsAtEnd(); ++itVectorNS )
    {
    if (!itVectorNS.Get()->IsRoot()
        && !itVectorNS.Get()->IsDocument()
        && !itVectorNS.Get()->IsFolder())
      {
      DataNodeType::Pointer currentGeometry = itVectorNS.Get();

      for (unsigned int i = 0; i < descriptors.size(); ++i)
        {
        double desc = currentGeometry->GetFieldAsDouble(descriptors[i]);

        accFirstOrderNS[i] += desc;
        accSecondOrderNS[i] += desc * desc;

        if (desc < minNS[i])
          {
          minNS[i] = desc;
          }
        if (desc > maxNS[i])
          {
          maxNS[i] = desc;
          }

        }
      accNbElemNS ++;
      }
    }
  std::cout << "Descriptors Stats : " << std::endl;
  std::cout << "Ground truth" << std::endl;
  for (unsigned int i = 0; i < descriptors.size(); ++i)
    {
    double mean = accFirstOrderGT[i] / accNbElemGT;
    double stddev = vcl_sqrt( accSecondOrderGT[i]/accNbElemGT - mean*mean );
    std::cout << descriptors[i] << "  :  " << mean << " +/- " << stddev
        << "  (min: " << minGT[i] << "  max: " << maxGT[i] << ")"<< std::endl;
    }

  std::cout << "Negative Samples ";

  if (!parseResult->IsOptionPresent("InputNegativeVectorData"))
    {
    std::cout << "(random generation)";
    }

  std::cout << std::endl;

  for (unsigned int i = 0; i < descriptors.size(); ++i)
    {
    double mean = accFirstOrderNS[i] / accNbElemNS;
    double stddev = vcl_sqrt( accSecondOrderNS[i]/accNbElemNS - mean*mean );
    std::cout << descriptors[i] << "  :  " << mean << " +/- " << stddev
        << "  (min: " << minNS[i] << "  max: " << maxNS[i] << ")"<< std::endl;
    }


  //Cost Function
  //Format Hypothesis
  LabelSetType hyp;
  if (parseResult->IsOptionPresent("Hypothesis"))
    {
    int nbSet = parseResult->GetNumberOfParameters("Hypothesis");
    for (int i = 0; i < nbSet; i++)
      {
      std::string str = parseResult->GetParameterString("Hypothesis", i);
      hyp.insert(str);
      }
    }
  else
    {
    hyp.insert("NDVI");
    hyp.insert("RADIOM");
    hyp.insert("DBOVERLAP");
    }
  costFunction->SetHypothesis(hyp);
  if (parseResult->IsOptionPresent("Weighting"))
    {
    costFunction->SetWeight(parseResult->GetParameterDouble("Weighting"));
    }
  if (parseResult->IsOptionPresent("Criterion"))
    {
    costFunction->SetCriterionFormula(parseResult->GetParameterString("Criterion"));
    }
  costFunction->SetGTVectorData(descriptionFilterGT->GetOutput());
  costFunction->SetNSVectorData(descriptionFilterNS->GetOutput());

  //Optimizer
  optimizer->SetCostFunction(costFunction);
  if (parseResult->IsOptionPresent("MaximumNumberOfIterations"))
    {
    optimizer->SetMaximumNumberOfIterations(parseResult->GetParameterInt("MaximumNumberOfIterations"));
    }
  else
    {
    optimizer->SetMaximumNumberOfIterations(200);
    }
  /*
  optimizer->SetParametersConvergenceTolerance( 0.01 );
  optimizer->SetFunctionConvergenceTolerance(0.001);
   */
  OptimizerType::ParametersType
  simplexDelta( costFunction->GetNumberOfParameters() );
  simplexDelta.Fill(0.1);

  optimizer->AutomaticInitialSimplexOff();
  optimizer->SetInitialSimplexDelta( simplexDelta );

  OptimizerType::ParametersType
  initialPosition( costFunction->GetNumberOfParameters() );

  for (unsigned int i = 0; i < costFunction->GetNumberOfParameters(); i += 4)
    {
    initialPosition.SetElement(i,   0.25);
    initialPosition.SetElement(i+1, 0.50);
    initialPosition.SetElement(i+2, 0.75);
    initialPosition.SetElement(i+3, 0.99);
    }

  optimizer->SetInitialPosition(initialPosition);

  // Create the Command observer and register it with the optimizer.
  CommandIterationUpdate::Pointer observer = CommandIterationUpdate::New();
  if (parseResult->IsOptionPresent("OptimizerObserver"))
    {
    optimizer->AddObserver( itk::IterationEvent(), observer );
    }

  try
  {
    // do the optimization
    optimizer->StartOptimization();
  }
  catch( itk::ExceptionObject& err )
  {
    // An error has occurred in the optimization.
    // Update the parameters
    std::cout << "ERROR: Exception Catched!" << std::endl;
    std::cout << err.GetDescription() << std::endl;
    const unsigned int numberOfIterations
    = optimizer->GetOptimizer()->get_num_evaluations();
    std::cout << "numberOfIterations : " << numberOfIterations << std::endl;
    std::cout << "Results : " << optimizer->GetCurrentPosition() << std::endl;
  }
  // get the results
  const unsigned int numberOfIterations
  = optimizer->GetOptimizer()->get_num_evaluations();
  std::cout << "numberOfIterations : " << numberOfIterations << std::endl;
  std::cout << "Results : " << optimizer->GetCurrentPosition() << std::endl;

  otb::FuzzyDescriptorsModelManager::DescriptorsModelType model;
  otb::FuzzyDescriptorsModelManager::ParameterType        ndvi, radiom, overlap;

  for (unsigned int i = 0; i<4; i++)
    {
    ndvi.push_back(optimizer->GetCurrentPosition()[i]);
    }
  for (unsigned int i = 0; i<4; i++)
    {
    radiom.push_back(optimizer->GetCurrentPosition()[i+4]);
    }
  for (unsigned int i = 0; i<4; i++)
    {
    overlap.push_back(optimizer->GetCurrentPosition()[i+8]);
    }
  otb::FuzzyDescriptorsModelManager::AddDescriptor("NDVI", ndvi, model);
  otb::FuzzyDescriptorsModelManager::AddDescriptor("RADIOM", radiom, model);
  otb::FuzzyDescriptorsModelManager::AddDescriptor("DBOVER", overlap, model);
  otb::FuzzyDescriptorsModelManager::Save(parseResult->GetParameterString("Output"),
                                          model);

  if (parseResult->IsOptionPresent("GenerateShp"))
    {
    VectorDataWriterType::Pointer NSWriter = VectorDataWriterType::New();
    VectorDataWriterType::Pointer PSWriter = VectorDataWriterType::New();
    std::string NSFileName, PSFileName;
    NSFileName = parseResult->GetParameterString("Output") + "_NS.shp";
    PSFileName = parseResult->GetParameterString("Output") + "_PS.shp";

    VectorDataType::Pointer vdNS = descriptionFilterNS->GetOutput();
    VectorDataType::Pointer vdPS = descriptionFilterGT->GetOutput();
    VectorDataType::Pointer projectedVDNS, projectedVDPS;


    std::string projRef = imgReader->GetOutput()->GetProjectionRef();

    typedef itk::AffineTransform<VectorDataType::PrecisionType, 2> TransformType;
    typedef otb::VectorDataTransformFilter<VectorDataType, VectorDataType> VDTransformType;

    TransformType::ParametersType params;
    params.SetSize(6);
    params[0] = imgReader->GetOutput()->GetSpacing()[0];
    params[1] = 0;
    params[2] = 0;
    params[3] = imgReader->GetOutput()->GetSpacing()[1];
    params[4] = imgReader->GetOutput()->GetOrigin()[0];
    params[5] = imgReader->GetOutput()->GetOrigin()[1];

    TransformType::Pointer transform = TransformType::New();
    transform->SetParameters(params);

    VDTransformType::Pointer vdTransformNS = VDTransformType::New();
    vdTransformNS->SetTransform(transform);
    vdTransformNS->SetInput(vdNS);
    vdTransformNS->Update();

    VDTransformType::Pointer vdTransformPS = VDTransformType::New();
    vdTransformPS->SetTransform(transform);
    vdTransformPS->SetInput(vdPS);
    vdTransformPS->Update();

    projectedVDNS = vdTransformNS->GetOutput();
    projectedVDNS->SetProjectionRef(projRef);
    projectedVDPS = vdTransformPS->GetOutput();
    projectedVDPS->SetProjectionRef(projRef);

    NSWriter->SetInput(projectedVDNS);
    NSWriter->SetFileName(NSFileName);
    NSWriter->Update();
    PSWriter->SetInput(projectedVDPS);
    PSWriter->SetFileName(PSFileName);
    PSWriter->Update();
    }

  return EXIT_SUCCESS;
}
