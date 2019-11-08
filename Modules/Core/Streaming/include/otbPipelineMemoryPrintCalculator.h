/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbPipelineMemoryPrintCalculator_h
#define otbPipelineMemoryPrintCalculator_h

#include "itkProcessObject.h"
#if ITK_VERSION_MAJOR < 4 || (ITK_VERSION_MAJOR == 4 && ITK_VERSION_MINOR <= 8)
#include "itksys/FundamentalType.h"
#else
#include "itk_kwiml.h"
#endif
#include <set>
#include <iosfwd>

#include "OTBStreamingExport.h"

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
class OTBStreaming_EXPORT PipelineMemoryPrintCalculator : public itk::Object
{
public:
  /** Standard class typedefs */
  typedef PipelineMemoryPrintCalculator Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Useful typedefs */
  typedef itk::ProcessObject         ProcessObjectType;
  typedef ProcessObjectType::Pointer ProcessObjectPointerType;
  typedef itk::DataObject            DataObjectType;
  typedef DataObjectType::Pointer    DataObjectPointerType;
#if ITK_VERSION_MAJOR < 4 || (ITK_VERSION_MAJOR == 4 && ITK_VERSION_MINOR <= 8)
  typedef ::itksysFundamentalType_UInt64 MemoryPrintType;
#else
  typedef KWIML_INT_uint64_t MemoryPrintType;
#endif
  typedef std::set<const ProcessObjectType*> ProcessObjectPointerSetType;

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
  static unsigned long EstimateOptimalNumberOfStreamDivisions(MemoryPrintType memoryPrint, MemoryPrintType availableMemory);

  /** Set last pipeline filter */
  itkSetObjectMacro(DataToWrite, DataObjectType);

  /** Compute pipeline memory print */
  void Compute(bool propagate = true);

  /** Const conversion factor */
  static const double ByteToMegabyte;
  static const double MegabyteToByte;

  /** Evaluate the print (in bytes) of a single data object */
  MemoryPrintType EvaluateDataObjectPrint(DataObjectType* data);

protected:
  /** Constructor */
  PipelineMemoryPrintCalculator();

  /** Destructor */
  ~PipelineMemoryPrintCalculator() override;

  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  /** Recursive method to evaluate memory print in bytes */
  MemoryPrintType EvaluateProcessObjectPrintRecursive(ProcessObjectType* process);

private:
  PipelineMemoryPrintCalculator(const Self&) = delete;
  void operator=(const Self&) = delete;

  /** The total memory print of the pipeline */
  MemoryPrintType m_MemoryPrint;

  /** Pointer to the last pipeline filter */
  DataObjectPointerType m_DataToWrite;

  /** Bias correction factor */
  double m_BiasCorrectionFactor;

  /** Visited ProcessObject set */
  ProcessObjectPointerSetType m_VisitedProcessObjects;
};
} // end of namespace otb

#endif
