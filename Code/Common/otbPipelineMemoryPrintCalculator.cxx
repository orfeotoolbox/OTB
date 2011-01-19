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
#include "otbPipelineMemoryPrintCalculator.h"

namespace otb
{
const double PipelineMemoryPrintCalculator::OctetToMegaOctet = 1./vcl_pow(2,20);

PipelineMemoryPrintCalculator
::PipelineMemoryPrintCalculator()
  : m_MemoryPrint(0),
    m_AvailableMemory(256),
    m_OptimalNumberOfStreamDivisions(1),
    m_DataToWrite(NULL),
    m_BiasCorrectionFactor(1.)
{}

PipelineMemoryPrintCalculator
::~PipelineMemoryPrintCalculator()
{}

void 
PipelineMemoryPrintCalculator
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  // Call superclass implementation
  Superclass::PrintSelf(os,indent);

  // Display parameters
  os<<indent<<"Data to write:                      "<<m_DataToWrite<<std::endl;
  os<<indent<<"Memory print of whole pipeline:     "<<m_MemoryPrint<<" Mo"<<std::endl;
  os<<indent<<"Available memory:                   "<<m_AvailableMemory<<" Mo"<<std::endl;
  os<<indent<<"Optimal number of stream divisions: "<<m_OptimalNumberOfStreamDivisions<<std::endl;
  os<<indent<<"Bias correction factor applied:     "<<m_BiasCorrectionFactor<<std::endl;
}

void 
PipelineMemoryPrintCalculator
::Compute()
{
  // Dry run of pipeline synchronisation
  m_DataToWrite->UpdateOutputInformation();
  m_DataToWrite->SetRequestedRegionToLargestPossibleRegion();
  m_DataToWrite->PropagateRequestedRegion();

  // Get the source process object
  ProcessObjectType * source = m_DataToWrite->GetSource();

  // Check if source exists
  if(source)
    {
    // Call the recursive memory print evaluation
    m_MemoryPrint = EvaluateMemoryPrint(source);
    }
  else
    {
    // Get memory print for this data only
    m_MemoryPrint = EvaluateDataObjectPrint(m_DataToWrite);
    }

  // Apply bias correction factor
  m_MemoryPrint*=m_BiasCorrectionFactor;

  // Compute the optimal number of stream division
  m_OptimalNumberOfStreamDivisions = vcl_ceil(m_MemoryPrint
                                              /m_AvailableMemory);
}

double 
PipelineMemoryPrintCalculator
::EvaluateMemoryPrint(ProcessObjectType * process)
{
  // This variable will store the final print
  double print = 0;

  // Retrieve the array of inputs
  ProcessObjectType::DataObjectPointerArray inputs = process->GetInputs();
  // First, recurse on each input source
  for(unsigned int i =0; i < process->GetNumberOfInputs();++i)
    {
    // Retrieve the data object
    DataObjectType * input = inputs[i];
      
    // Retrieve possible source
    ProcessObjectType * source = input->GetSource();

    // If data object has a source
    if(source)
      {
      print += this->EvaluateMemoryPrint(source);
      }
    else
      {
      double localPrint = this->EvaluateDataObjectPrint(input);
      print += localPrint;
      }
    }
  // Retrieve the output array
  ProcessObjectType::DataObjectPointerArray outputs = process->GetOutputs();

  // Now, evaluate the current object print
  for(unsigned int i =0; i < process->GetNumberOfOutputs();++i)
    {
    double localPrint = this->EvaluateDataObjectPrint(outputs[0]);
    print += localPrint;
    }

  // Finally, return the total print
  return print;
}

double 
PipelineMemoryPrintCalculator
::EvaluateDataObjectPrint(DataObjectType * data)
{

#define OTB_IMAGE_SIZE_BLOCK(type)                                      \
  if(dynamic_cast<itk::Image<type,2> *>(data) != NULL)                  \
    {                                                                   \
    itk::Image<type,2> * image = dynamic_cast<itk::Image<type,2> *>(data); \
    return image->GetRequestedRegion().GetNumberOfPixels()              \
      * image->GetNumberOfComponentsPerPixel() * sizeof(type) * OctetToMegaOctet; \
    }                                                                   \
  if(dynamic_cast<itk::VectorImage<type,2> * >(data) != NULL)           \
    {                                                                   \
    itk::VectorImage<type,2> * image = dynamic_cast<itk::VectorImage<type,2> *>(data); \
    return image->GetRequestedRegion().GetNumberOfPixels()              \
      * image->GetNumberOfComponentsPerPixel() * sizeof(type) * OctetToMegaOctet; \
    }                                                                   \
  
  // Call the macro for each pixel type
  OTB_IMAGE_SIZE_BLOCK(unsigned char)
    OTB_IMAGE_SIZE_BLOCK(char)
    OTB_IMAGE_SIZE_BLOCK(unsigned short)
    OTB_IMAGE_SIZE_BLOCK( short)
    OTB_IMAGE_SIZE_BLOCK(unsigned int)
    OTB_IMAGE_SIZE_BLOCK( int)
    OTB_IMAGE_SIZE_BLOCK(unsigned long)
    OTB_IMAGE_SIZE_BLOCK( long)
    OTB_IMAGE_SIZE_BLOCK(float)
    OTB_IMAGE_SIZE_BLOCK( double)
    OTB_IMAGE_SIZE_BLOCK(std::complex<float>)
    OTB_IMAGE_SIZE_BLOCK(std::complex<double>)
    typedef itk::FixedArray<float,2> FloatFixedArray2Type;
  typedef itk::FixedArray<float,2> DoubleFixedArray2Type;
  OTB_IMAGE_SIZE_BLOCK(FloatFixedArray2Type)
    OTB_IMAGE_SIZE_BLOCK(DoubleFixedArray2Type)
      
    // If we are still here, none of the macro call succeed
    return 0;
}

} // End namespace otb
