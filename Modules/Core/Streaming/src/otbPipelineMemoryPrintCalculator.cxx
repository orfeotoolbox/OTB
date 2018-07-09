/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include <complex>
#include <ostream>

#include "otbPipelineMemoryPrintCalculator.h"

#include "otbMacro.h"
#include "otbMath.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "itkFixedArray.h"
#include "otbImageList.h"

namespace otb
{
const double PipelineMemoryPrintCalculator::ByteToMegabyte = 1./std::pow(2.0, 20);
const double PipelineMemoryPrintCalculator::MegabyteToByte = std::pow(2.0, 20);

PipelineMemoryPrintCalculator
::PipelineMemoryPrintCalculator()
  : m_MemoryPrint(0),
    m_DataToWrite(nullptr),
    m_BiasCorrectionFactor(1.),
    m_VisitedProcessObjects()
{}

PipelineMemoryPrintCalculator
::~PipelineMemoryPrintCalculator()
{}

// [static]
unsigned long
PipelineMemoryPrintCalculator
::EstimateOptimalNumberOfStreamDivisions(MemoryPrintType memoryPrint, MemoryPrintType availableMemory)
{
  unsigned long divisions;
  divisions = std::ceil(static_cast<double>(memoryPrint)
                       / availableMemory);
  return divisions;
}

void
PipelineMemoryPrintCalculator
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  // Call superclass implementation
  Superclass::PrintSelf(os, indent);

  // Display parameters
  os<<indent<<"Data to write:                      "<<m_DataToWrite<<std::endl;
  os<<indent<<"Memory print of whole pipeline:     "<<m_MemoryPrint * ByteToMegabyte <<" Mb"<<std::endl;
  os<<indent<<"Bias correction factor applied:     "<<m_BiasCorrectionFactor<<std::endl;
}

void
PipelineMemoryPrintCalculator
::Compute(bool propagate)
{
  // Clear the visited process objects set
  m_VisitedProcessObjects.clear();

  // Dry run of pipeline synchronisation
  if (propagate)
    {
    m_DataToWrite->UpdateOutputInformation();
    m_DataToWrite->SetRequestedRegionToLargestPossibleRegion();
    m_DataToWrite->PropagateRequestedRegion();
    }

  // Get the source process object
  ProcessObjectType * source = m_DataToWrite->GetSource();

  // Check if source exists
  if(source)
    {
    // Call the recursive memory print evaluation
    m_MemoryPrint = EvaluateProcessObjectPrintRecursive(source);
    }
  else
    {
    // Get memory print for this data only
    m_MemoryPrint = EvaluateDataObjectPrint(m_DataToWrite);
    }

  // Apply bias correction factor
  m_MemoryPrint *= m_BiasCorrectionFactor;

}

PipelineMemoryPrintCalculator::MemoryPrintType
PipelineMemoryPrintCalculator
::EvaluateProcessObjectPrintRecursive(ProcessObjectType * process)
{
  otbLogMacro(Debug,<<"Recursive evaluation of memory print for ProcessObject" << process->GetNameOfClass() << " (" << process << ")");
  // This variable will store the final print
  MemoryPrintType print = 0;

  // Check if this process object has already been visited
  if(m_VisitedProcessObjects.count(process))
    {
    return print;
    }
  // Else register it as a visited process object
  else
    {
    m_VisitedProcessObjects.insert(process);
    }

  // Retrieve the array of inputs
  ProcessObjectType::DataObjectPointerArray inputs = process->GetInputs();
  // First, recurse on each input source
  for(unsigned int i = 0; i < process->GetNumberOfInputs(); ++i)
    {
    // Retrieve the data object
    DataObjectType * input = inputs[i];

    if( input )
      {
        // Retrieve possible source
        ProcessObjectType * source = input->GetSource();

        // If data object has a source
        if(source)
          {
            print += this->EvaluateProcessObjectPrintRecursive(source);
          }
        else
          {
            MemoryPrintType localPrint = this->EvaluateDataObjectPrint(input);
            print += localPrint;
          }
      }
    }

  // Retrieve the output array
  ProcessObjectType::DataObjectPointerArray outputs = process->GetOutputs();

  // Now, evaluate the current object print
  for(unsigned int i = 0; i < process->GetNumberOfOutputs(); ++i)
    {
      MemoryPrintType localPrint = this->EvaluateDataObjectPrint(outputs[i]);
      print += localPrint;
    }

  // Finally, return the total print
  return print;
}

PipelineMemoryPrintCalculator::MemoryPrintType
PipelineMemoryPrintCalculator
::EvaluateDataObjectPrint(DataObjectType * data)
{
    
  otbLogMacro(Debug,<<"Evaluation of memory print for DataObject " << data->GetNameOfClass() << " (" << data << ")");
    
#define OTB_IMAGE_SIZE_BLOCK(type)                                      \
  if(dynamic_cast<itk::Image<type, 2> *>(data) != NULL)                  \
    {                                                                   \
    itk::Image<type, 2> * image = dynamic_cast<itk::Image<type, 2> *>(data); \
    return image->GetRequestedRegion().GetNumberOfPixels()              \
      * image->GetNumberOfComponentsPerPixel() * sizeof(type); \
    }                                                                   \
  if(dynamic_cast<itk::VectorImage<type, 2> * >(data) != NULL)           \
    {                                                                   \
    itk::VectorImage<type, 2> * image = dynamic_cast<itk::VectorImage<type, 2> *>(data); \
    return image->GetRequestedRegion().GetNumberOfPixels()              \
      * image->GetNumberOfComponentsPerPixel() * sizeof(type); \
    }                                                                   \
  if(dynamic_cast<ImageList<Image<type, 2> > *>(data) != NULL)   \
    {                                                                   \
    ImageList<Image<type, 2> > * imageList = dynamic_cast<otb::ImageList<otb::Image<type, 2> > *>(data); \
    MemoryPrintType print(0);                                         \
    for(ImageList<Image<type, 2> >::Iterator it = imageList->Begin(); \
       it != imageList->End(); ++it)                                    \
       {                                                             \
       if(it.Get()->GetSource())                                        \
         print += this->EvaluateProcessObjectPrintRecursive(it.Get()->GetSource());\
       else                                                             \
         print += this->EvaluateDataObjectPrint(it.Get());              \
       }                                                           \
    return print;                                                  \
    }                                                              \
  if(dynamic_cast<ImageList<VectorImage<type, 2> > *>(data) != NULL)   \
    {                                                                   \
    ImageList<VectorImage<type, 2> > * imageList = dynamic_cast<otb::ImageList<otb::VectorImage<type, 2> > *>(data); \
    MemoryPrintType print(0);                                         \
    for(ImageList<VectorImage<type, 2> >::ConstIterator it = imageList->Begin(); \
       it != imageList->End(); ++it)                                    \
       {                                                             \
       if(it.Get()->GetSource())                                        \
         print += this->EvaluateProcessObjectPrintRecursive(it.Get()->GetSource());\
       else                                                             \
         print += this->EvaluateDataObjectPrint(it.Get());              \
       }                                                           \
    return print;                                                  \
    }                                                              \


  // Call the macro for each pixel type
  OTB_IMAGE_SIZE_BLOCK(unsigned char)
  OTB_IMAGE_SIZE_BLOCK(char)
  OTB_IMAGE_SIZE_BLOCK(unsigned short)
  OTB_IMAGE_SIZE_BLOCK(short)
  OTB_IMAGE_SIZE_BLOCK(unsigned int)
  OTB_IMAGE_SIZE_BLOCK(int)
  OTB_IMAGE_SIZE_BLOCK(unsigned long)
  OTB_IMAGE_SIZE_BLOCK(long)
  OTB_IMAGE_SIZE_BLOCK(float)
  OTB_IMAGE_SIZE_BLOCK(double)
  OTB_IMAGE_SIZE_BLOCK(std::complex<float>)
  OTB_IMAGE_SIZE_BLOCK(std::complex<double>)
  typedef itk::FixedArray<float, 2> FloatFixedArray2Type;
  typedef itk::FixedArray<float, 2> DoubleFixedArray2Type;
  OTB_IMAGE_SIZE_BLOCK(FloatFixedArray2Type)
  OTB_IMAGE_SIZE_BLOCK(DoubleFixedArray2Type)

  // If we are still here, none of the macro call succeed
  return 0;
}

} // End namespace otb
