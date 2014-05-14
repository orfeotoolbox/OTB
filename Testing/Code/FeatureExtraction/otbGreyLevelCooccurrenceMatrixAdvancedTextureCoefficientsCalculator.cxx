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
#include "itkMacro.h"

#include "itkHistogram.h"
#include "vnl/vnl_math.h"

#include "otbGreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculator.h"

int otbGreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculator(int argc, char * argv[])
{

  try   // the rest of the function is in the try block...

    { //Data definitions
    const unsigned int HISTOGRAM_AXIS_LEN =  25;

    //------------------------------------------------------
    // Create a simple test histogram. The histogram must be
    // symmetric and normalized.
    //------------------------------------------------------
    const unsigned int Dimension = 2;
    typedef float                                                  MeasurementType;
    typedef itk::Statistics::Histogram<MeasurementType> HistogramType;
    HistogramType::Pointer  histogram = HistogramType::New();
    HistogramType::SizeType size(Dimension);
    size.Fill(HISTOGRAM_AXIS_LEN);
    HistogramType::MeasurementVectorType lowerBound(Dimension);
    HistogramType::MeasurementVectorType upperBound(Dimension);
    lowerBound[0] = 0;
    lowerBound[1] = 0;
    upperBound[0] = HISTOGRAM_AXIS_LEN + 1;
    upperBound[1] = HISTOGRAM_AXIS_LEN + 1;
    histogram->SetMeasurementVectorSize(Dimension);
    histogram->Initialize(size, lowerBound, upperBound);
    histogram->SetToZero();

    HistogramType::IndexType index(Dimension);
    index[0] = 0;
    index[1] = 0;
    histogram->SetFrequencyOfIndex(index, 10);
    index[0] = 3;
    index[1] = 3;
    histogram->SetFrequencyOfIndex(index, 50);
    index[0] = 2;
    index[1] = 1;
    histogram->SetFrequencyOfIndex(index, 5);
    index[0] = 1;
    index[1] = 2;
    histogram->SetFrequencyOfIndex(index, 5);
    index[0] = 7;
    index[1] = 6;
    histogram->SetFrequencyOfIndex(index, 10);
    index[0] = 6;
    index[1] = 7;
    histogram->SetFrequencyOfIndex(index, 10);
    index[0] = 10;
    index[1] = 10;
    histogram->SetFrequencyOfIndex(index, 10);

    typedef otb::GreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculator<
        HistogramType>  GLCMCalcType;
    GLCMCalcType::Pointer glcmCalc = GLCMCalcType::New();

    glcmCalc->SetHistogram(histogram);
    glcmCalc->Compute();

    double trueVariance = 7.6475;
    double trueMean = 3.95;
    double trueSumAverage = 0;
    double trueSumVariance = 0;
    double trueSumEntropy = 0;
    double trueDifferenceEntropy = 0.770746;
    double trueDifferenceVariance = 0.1275;
    double trueIC1 = 0.557305;
    double trueIC2 = 0.908663;

    double variance = glcmCalc->GetVariance();
    double mean = glcmCalc->GetMean();
    double sumAverage = glcmCalc->GetSumAverage();
    double sumVariance = glcmCalc->GetSumVariance();
    double sumEntropy = glcmCalc->GetSumEntropy();
    double differenceEntropy = glcmCalc->GetDifferenceEntropy();
    double differenceVariance = glcmCalc->GetDifferenceVariance();
    double ic1 = glcmCalc->GetIC1();
    double ic2 = glcmCalc->GetIC2();

    bool passed = true;

    if (vnl_math_abs(variance - trueVariance) > 0.001)
      {
      std::cerr << "Error:" << std::endl;
      std::cerr << "Variance calculated wrong. Expected: " << trueVariance << ", got: "
                << variance << std::endl;
      passed = false;
      }

    if (vnl_math_abs(mean - trueMean) > 0.001)
      {
      std::cerr << "Error:" << std::endl;
      std::cerr << "Mean calculated wrong. Expected: " << trueMean << ", got: "
                << mean << std::endl;
      passed = false;
      }

    if (vnl_math_abs(sumAverage - trueSumAverage) > 0.001)
      {
      std::cerr << "Error:" << std::endl;
      std::cerr << "Sum average calculated wrong. Expected: " << trueSumAverage << ", got: "
                << sumAverage << std::endl;
      passed = false;
      }

    if (vnl_math_abs(sumVariance - trueSumVariance) > 0.001)
      {
      std::cerr << "Error:" << std::endl;
      std::cerr << "Sum variance calculated wrong. Expected: " << trueSumVariance <<
      ", got: "  << sumVariance << std::endl;
      passed = false;
      }

    if (vnl_math_abs(sumEntropy - trueSumEntropy) > 0.001)
      {
      std::cerr << "Error:" << std::endl;
      std::cerr << "Sum entropy calculated wrong. Expected: " <<
      trueSumEntropy <<  ", got: "  << sumEntropy << std::endl;
      passed = false;
      }

    if (vnl_math_abs(differenceEntropy - trueDifferenceEntropy) > 0.001)
      {
      std::cerr << "Error:" << std::endl;
      std::cerr << "Difference entropy calculated wrong. Expected: " << trueDifferenceEntropy << ", got: "
                << differenceEntropy << std::endl;
      passed = false;
      }

    if (vnl_math_abs(differenceVariance - trueDifferenceVariance) > 0.001)
      {
      std::cerr << "Error:" << std::endl;
      std::cerr << "Difference variance calculated wrong. Expected: " << trueDifferenceVariance <<
      ", got: "  << differenceVariance << std::endl;
      passed = false;
      }

    if (vnl_math_abs(ic1 - trueIC1) > 0.001)
      {
      std::cerr << "Error:" << std::endl;
      std::cerr << "IC1 calculated wrong. Expected: "
                << trueIC1 << ", got: "  << ic1 << std::endl;
      passed = false;
      }

    if (vnl_math_abs(ic2 - trueIC2) > 0.001)
      {
      std::cerr << "Error:" << std::endl;
      std::cerr << "IC2 calculated wrong. Expected: "
                << trueIC2 << ", got: "  << ic2 << std::endl;
      passed = false;
      }

    if (!passed)
      {
      std::cerr << "Test failed" << std::endl;
      return EXIT_FAILURE;
      }
    else
      {
      std::cerr << "Test succeeded" << std::endl;
      return EXIT_SUCCESS;
      }

    }
  catch (itk::ExceptionObject& err)
    {
    std::cerr << "ExceptionObject caught !" << std::endl;
    std::cerr << err << std::endl;
    std::cerr << "Test failed" << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
