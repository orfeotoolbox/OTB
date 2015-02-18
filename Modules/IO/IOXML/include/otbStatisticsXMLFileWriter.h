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
#ifndef __otbStatisticsXMLFileWriter_h
#define __otbStatisticsXMLFileWriter_h

#include "itkProcessObject.h"
#include <utility>
#include <string>

namespace otb {

/** \class StatisticsXMLFileWriter
 *  \brief Write in a xml file the values stored in a MeasurementVector set as
 *  input
 *
 * The vector can be set as input via AddInput(name, vector) where name
 * is the name of the statistic, and vector the values.
 * Supported vector types are those implementing the method GetElement(idx)
 * and defining the type ValueType.
 *
 */
template < class TMeasurementVector>
class  StatisticsXMLFileWriter :
    public itk::Object
{
public:
  /** Standard class typedefs */
  typedef StatisticsXMLFileWriter          Self;
  typedef itk::Object                      Superclass;
  typedef itk::SmartPointer< Self >        Pointer;
  typedef itk::SmartPointer<const Self>    ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(StatisticsXMLFileWriter, itk::Object);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** InputSampleList typedefs */
  typedef TMeasurementVector                             MeasurementVectorType;
  typedef typename MeasurementVectorType::ValueType      InputValueType;

  /** Convenient typedef */
  typedef std::pair<std::string , MeasurementVectorType>  InputDataType;
  typedef std::vector< InputDataType >                   MeasurementVectorContainer;

  /** Method to set/get the input list sample */
  void AddInput(const char * name,  const MeasurementVectorType& inputVector );

  /** Trigger the processing */
  void Update()
  {
    this->GenerateData();
  }

  /** Set the output filename */
  itkSetStringMacro(FileName);
  itkGetStringMacro(FileName);

protected:

  virtual void GenerateData();

  StatisticsXMLFileWriter();
  virtual ~StatisticsXMLFileWriter() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  StatisticsXMLFileWriter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  std::string                 m_FileName;
  MeasurementVectorContainer  m_MeasurementVectorContainer;


}; // end of class StatisticsXMLFileWriter

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbStatisticsXMLFileWriter.txx"
#endif

#endif
