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
 *  \brief Estimate pipeline memory usage and optimal stream divisions
 *
 *  This class allows to estimate the memory usage of a given pipeline
 *  by tracing back pipeline from a given data (in general, this
 *  data should be set to the data to write) and
 *  examining each filter to determine its memory footprint. To do so,
 *  it performs a dry run of the requested region pipeline
 *  negociation.
 *
 *  The SetDataToWrite() method allows to set the data candidate for
 *  writing, and for which memory usage estimation should be
 *  performed.
 * 
 *  Additionnaly, this class allows to compute the optimal number of
 *  stream division to write the data. To do so, the available memory
 *  can be set via the SetAvailableMemory() method, and an optionnal
 *  bias correction factor can be applied to wheight the estimate
 *  memory usage in case a bias occurs between estimated and real
 *  memory usage. The optimal number of stream divisions can be
 *  retreived using the GetOptimalNumberOfStreamDivisions().
 *
 *  Please note that for now this calculator suffers from the
 *  following limitations:
 *  - DataObject taken into account for memory usage estimation are
 *  only Image and VectorImage instantiation,
 *  - The estimator is non-intrusive regarding the pipeline high-level
 *  class, but it is not able to estimate memory usage of minipipeline
 *  within composite filter (because there is no way to trace back to
 *  these internal filters). Therefore, memory usage can be highly
 *  biased depending on the filters in the upstream pipeline. The bias
 *  correction factor parameters allows to compensate this bias to the first
 *  order. 
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
  
  /** Set/Get the available memory for pipeline execution (in Mo,
   *  default is 256 Mo) */
  itkSetMacro(AvailableMemory,double);
  itkGetMacro(AvailableMemory,double);

  /** Set/Get the bias correction factor which will weight the
   * estimated memory print (allows to compensate bias between
   * estimated and real memory print, default is 1., i.e. no correction) */
  itkSetMacro(BiasCorrectionFactor,double);
  itkGetMacro(BiasCorrectionFactor,double);

  /** Get the optimal number of stream division */
  itkGetMacro(OptimalNumberOfStreamDivisions,unsigned long);

  /** Set last pipeline filter */
  itkSetObjectMacro(DataToWrite,DataObjectType);

  /** Compute pipeline memory print */
  void Compute();

protected:
  /** Constructor */
  PipelineMemoryPrintCalculator();

  /** Destructor */
  virtual ~PipelineMemoryPrintCalculator();

  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;
  
  /** Internal recursive method to evaluate memory print in bytes */
  double EvaluateMemoryPrint(ProcessObjectType * process);

  /** Internal method to evaluate the print (in Mo) of a single data object */
  double EvaluateDataObjectPrint(DataObjectType * data);

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

  /** Bias correction factor */
  double m_BiasCorrectionFactor;

  /** Const conversion factor */
  static const double OctetToMegaOctet;
};
} // end of namespace otb

#endif
