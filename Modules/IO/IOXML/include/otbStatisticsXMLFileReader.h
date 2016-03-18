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
#ifndef __otbStatisticsXMLFileReader_h
#define __otbStatisticsXMLFileReader_h

#include "itkProcessObject.h"

namespace otb {

/** \class StatisticsXMLFileReader
 *  \brief   Read a xml file where are stored several statistics
 *
 * To get a specific statistic Measurement vector, use the method
 * GetStatisticVectorByName(name) which allow you to get the
 * Measurement vector for the statistic set as paramater.
 *
 *
 *
 * \ingroup OTBIOXML
 */
template < class TMeasurementVector>
class  StatisticsXMLFileReader :
    public itk::Object
{
public:
  /** Standard class typedefs */
  typedef StatisticsXMLFileReader          Self;
  typedef itk::Object                      Superclass;
  typedef itk::SmartPointer< Self >        Pointer;
  typedef itk::SmartPointer<const Self>    ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(StatisticsXMLFileReader, itk::Object);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** InputSampleList typedefs */
  typedef TMeasurementVector                             MeasurementVectorType;
  typedef typename MeasurementVectorType::ValueType      InputValueType;

  /** Convenient typedef */
  typedef std::pair<std::string , MeasurementVectorType>  InputDataType;
  typedef std::vector< InputDataType >                   MeasurementVectorContainer;

  typedef std::map<std::string , std::string>           GenericMapType;
  typedef std::map<std::string , GenericMapType>        GenericMapContainer;

  virtual void Modified() const
    {
      m_IsUpdated = false;
    }

  /** Set the output filename */
  itkSetStringMacro(FileName);
  itkGetStringMacro(FileName);

  /** Get the number of Outputs*/
  unsigned int GetNumberOfOutputs();

  /** Get the list of vector statistics names */
  std::vector<std::string> GetStatisticVectorNames();

  /** Get the list of map statistics names */
  std::vector<std::string> GetStatisticMapNames();

  /** Method to get the MeasurementVector by name */
  MeasurementVectorType GetStatisticVectorByName(const char * statisticName);

  /** Method to get a statistics map by name */
  template <typename MapType>
  MapType GetStatisticMapByName(const char * statisticName);

protected:

  /** Read into the file and extract information in vector and map containers */
  virtual void Read();

  StatisticsXMLFileReader();
  virtual ~StatisticsXMLFileReader() {}
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  StatisticsXMLFileReader(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  std::string                 m_FileName;
  MeasurementVectorContainer  m_MeasurementVectorContainer;
  mutable bool                m_IsUpdated;
  GenericMapContainer         m_GenericMapContainer;

}; // end of class StatisticsXMLFileReader

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbStatisticsXMLFileReader.txx"
#endif

#endif
