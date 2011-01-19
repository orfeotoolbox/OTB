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
#ifndef __otbPipelineMemoryPrintCalculator_h
#define __otbPipelineMemoryPrintCalculator_h

#include "itkProcessObject.h"
#include "itkDataObject.h"
#include "otbMath.h"
#include "itkImage.h"
#include "itkVectorImage.h"
#include "itkFixedArray.h"

namespace otb
{
/** \class PipelineMemoryPrintCalculator
 *
 */
class ITK_EXPORT PipelineMemoryPrintCalculator :
  public itk::Object
{
public:
  /** Standard class typedefs */
  typedef PipelineMemoryPrintCalculator Self;
  typedef itk::Object            Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Useful typedefs */
  typedef itk::ProcessObject            ProcessObjectType;
  typedef ProcessObjectType::Pointer    ProcessObjectPointerType;
  typedef itk::DataObject               DataObjectType;
  typedef DataObjectType::Pointer       DataObjectPointerType;

  /** Run-time type information (and related methods). */
  itkTypeMacro(PipelineMemoryPrintCalculator, itk::Object);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
  /** Get the total memory print (in Mo) */
  itkGetMacro(MemoryPrint,double);
  
  /** Set/Get the available memory for pipeline execution (in Mo) */
  itkSetMacro(AvailableMemory,double);
  itkGetMacro(AvailableMemory,double);

  /** Get the optimal number of stream division */
  itkGetMacro(OptimalNumberOfStreamDivisions,unsigned long);

  /** Set last pipeline filter */
  itkSetObjectMacro(DataToWrite,DataObjectType);

  /** Compute pipeline memory print */
  void Compute()
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
      std::cout<<"DataToWrite has no source."<<std::endl;
      m_MemoryPrint = EvaluateDataObjectPrint(m_DataToWrite);
      }

    // TODO: Remove this !
    m_MemoryPrint*=2;

    // Compute the optimal number of stream division
    m_OptimalNumberOfStreamDivisions = vcl_ceil(m_MemoryPrint
                                                /m_AvailableMemory);
  }

protected:
  /** Constructor */
  PipelineMemoryPrintCalculator()
    : m_MemoryPrint(0),
    m_AvailableMemory(256),
    m_OptimalNumberOfStreamDivisions(0),
    m_DataToWrite(NULL)
    {}

  /** Destructor */
  virtual ~PipelineMemoryPrintCalculator() {}

  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    // Call superclass implementation
    Superclass::PrintSelf(os,indent);
  }
  
  /** Internal recursive method to evaluate memory print in bytes */
  double EvaluateMemoryPrint(ProcessObjectType * process)
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
        std::cout<<process->GetNameOfClass()<<"::Input"<<i<<" (sourceless) "<<localPrint<<" Mo"<<std::endl;
        print += localPrint;
        }
      }
    // Retrieve the output array
    ProcessObjectType::DataObjectPointerArray outputs = process->GetOutputs();

    // Now, evaluate the current object print
    for(unsigned int i =0; i < process->GetNumberOfOutputs();++i)
      {
      double localPrint = this->EvaluateDataObjectPrint(outputs[0]);
      std::cout<<process->GetNameOfClass()<<"::Output"<<i<<" "<<localPrint<<" Mo"<<std::endl;
      print += localPrint;
      }

    // Finally, return the total print
    return print;
  }

  /** Internal method to evaluate the print (in Mo) of a single data object */
  double EvaluateDataObjectPrint(DataObjectType * data)
  {

#define OTB_IMAGE_SIZE_BLOCK(type) \
    if(dynamic_cast<itk::Image<type,2> *>(data) != NULL) \
      { \
      itk::Image<type,2> * image = dynamic_cast<itk::Image<type,2> *>(data);\
      std::cout<<"Image of "<< image->GetRequestedRegion().GetNumberOfPixels() <<" pixels of type " # type ": "<<sizeof(type)<<" octets, ("<<image->GetRequestedRegion()<<")"<<std::endl; \
      return image->GetRequestedRegion().GetNumberOfPixels() * image->GetNumberOfComponentsPerPixel() * sizeof(type) * OctetToMegaOctet; \
      } \
    if(dynamic_cast<itk::VectorImage<type,2> * >(data) != NULL)     \
      { \
      itk::VectorImage<type,2> * image = dynamic_cast<itk::VectorImage<type,2> *>(data);\
      std::cout<<"VectorImage of "<< image->GetRequestedRegion().GetNumberOfPixels() <<" pixels of "<<image->GetNumberOfComponentsPerPixel()<<" components of type " # type ": "<<sizeof(type)<<" octets, ("<<image->GetRequestedRegion()<<")"<<std::endl;\
      return image->GetRequestedRegion().GetNumberOfPixels() * image->GetNumberOfComponentsPerPixel() * sizeof(type) * OctetToMegaOctet; \
      } \
  

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
      
      return 0;
  }

private:
  PipelineMemoryPrintCalculator(const Self &); //purposely not implemented
  void operator =(const Self&);                //purposely not implemented

  /** The total memory print of the pipeline */
  double       m_MemoryPrint;
  /** The available memory for pipeline execution */
  double       m_AvailableMemory;
  /** The optimal number of stream division */
  unsigned long m_OptimalNumberOfStreamDivisions;

  /** Pointer to the last pipeline filter */
  DataObjectPointerType m_DataToWrite;

  static const double OctetToMegaOctet;
};

const double PipelineMemoryPrintCalculator::OctetToMegaOctet = 1./vcl_pow(2,20);

} // end of namespace otb

#endif
