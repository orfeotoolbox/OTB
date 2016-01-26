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
#include "itksys/FundamentalType.h"
#include <set>

namespace otb
{
/** \class PipelineMemoryPrintCalculator
 *  \brief Estimate pipeline memory usage and optimal stream divisions
 *
 *  This class allows estimating the memory usage of a given pipeline
 *  by tracing back pipeline from a given data (in general, this
 *  data should be set to the data to write) and
 *  examining each filter to determine its memory footprint. To do so,
 *  it performs a dry run of the requested region pipeline
 *  negotiation.
 *
 *  The SetDataToWrite() method allows setting the data candidate for
 *  writing, and for which memory usage estimation should be
 *  performed.
 *
 *  Additionally, this class allows computing the optimal number of
 *  stream division to write the data. To do so, the available memory
 *  can be set via the SetAvailableMemory() method, and an optional
 *  bias correction factor can be applied to weight the estimate
 *  memory usage in case a bias occurs between estimated and real
 *  memory usage. The optimal number of stream divisions can be
 *  retrieved using the GetOptimalNumberOfStreamDivisions().
 *
 *  Please note that for now this calculator suffers from the
 *  following limitations:
 *  - DataObject taken into account for memory usage estimation are
 *  only Image, VectorImage and ImageList instantiation,
 *  - The estimator is non-intrusive regarding the pipeline high-level
 *  class, but it is not able to estimate memory usage of minipipeline
 *  within composite filter (because there is no way to trace back to
 *  these internal filters). Therefore, memory usage can be highly
 *  biased depending on the filters in the upstream pipeline. The bias
 *  correction factor parameters allows compensating this bias to the first
 *  order.
 *
 * \ingroup OTBStreaming
 */
class ITK_EXPORT PipelineMemoryPrintCalculator :
  public itk::Object
{
public:
  /** Standard class typedefs */
  typedef PipelineMemoryPrintCalculator       Self;
  typedef itk::Object                         Superclass;
  typedef itk::SmartPointer<Self>             Pointer;
  typedef itk::SmartPointer<const Self>       ConstPointer;

  /** Useful typedefs */
  typedef itk::ProcessObject                  ProcessObjectType;
  typedef ProcessObjectType::Pointer          ProcessObjectPointerType;
  typedef itk::DataObject                     DataObjectType;
  typedef DataObjectType::Pointer             DataObjectPointerType;
  typedef ::itksysFundamentalType_UInt64      MemoryPrintType;
  typedef std::set<const ProcessObjectType *> ProcessObjectPointerSetType;

  /** Run-time type information (and related methods). */
  itkTypeMacro(PipelineMemoryPrintCalculator, itk::Object);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Get the total memory print (in bytes) */
  itkGetMacro(MemoryPrint, MemoryPrintType);

  /** Set/Get the bias correction factor which will weight the
   * estimated memory print (allows compensating bias between
   * estimated and real memory print, default is 1., i.e. no correction) */
  itkSetMacro(BiasCorrectionFactor, double);
  itkGetMacro(BiasCorrectionFactor, double);

  /** Get the optimal number of stream division */
  static unsigned long EstimateOptimalNumberOfStreamDivisions(
      MemoryPrintType memoryPrint, MemoryPrintType availableMemory);

  /** Set last pipeline filter */
  itkSetObjectMacro(DataToWrite, DataObjectType);

  /** Compute pipeline memory print */
  void Compute();

  /** Const conversion factor */
  static const double ByteToMegabyte;
  static const double MegabyteToByte;

  /** Evaluate the print (in bytes) of a single data object */
  MemoryPrintType EvaluateDataObjectPrint(DataObjectType * data) const;

protected:
  /** Constructor */
  PipelineMemoryPrintCalculator();

  /** Destructor */
  virtual ~PipelineMemoryPrintCalculator();

  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** Recursive method to evaluate memory print in bytes */
  MemoryPrintType EvaluateProcessObjectPrintRecursive(ProcessObjectType * process);

private:
  PipelineMemoryPrintCalculator(const Self &); //purposely not implemented
  void operator =(const Self&);                //purposely not implemented

  /** The total memory print of the pipeline */
  MemoryPrintType       m_MemoryPrint;

  /** Pointer to the last pipeline filter */
  DataObjectPointerType m_DataToWrite;

  /** Bias correction factor */
  double m_BiasCorrectionFactor;

  /** Visited ProcessObject set */
  ProcessObjectPointerSetType m_VisitedProcessObjects;

};
} // end of namespace otb

#endif
