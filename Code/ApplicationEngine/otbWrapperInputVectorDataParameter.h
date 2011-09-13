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
#ifndef __otbWrapperInputVectorDataParameter_h
#define __otbWrapperInputVectorDataParameter_h

#include "otbVectorDataFileReader.h"
#include "otbWrapperParameter.h"

namespace otb
{
namespace Wrapper
{
/** \class InputVectorDataParameter
 *  \brief This class represents a InputVectorData parameter
 */

class ITK_EXPORT InputVectorDataParameter : public Parameter
{
public:
  /** Standard class typedef */
  typedef InputVectorDataParameter      Self;
  typedef Parameter                     Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Defining ::New() static method */
  itkNewMacro(Self);

  /** RTTI support */
  itkTypeMacro(InputVectorDataParameter,Parameter);

  typedef double CoordinatePrecisionType;
  typedef double ValuePrecisionType;
  typedef otb::VectorData<CoordinatePrecisionType, 2, ValuePrecisionType>  VectorDataType;

  /** Set value from filename */
  void SetFromFileName(const std::string& filename)
  {
    VectorDataFileReaderType::Pointer reader = VectorDataFileReaderType::New();
    reader->SetFileName(filename);
    reader->UpdateOutputInformation();

    // everything went fine, store the object references
    m_Reader = reader;
    m_VectorData = reader->GetOutput();
    SetActive(true);
  }

  std::string GetFileName() const
  {
    if (m_Reader)
      {
      return m_Reader->GetFileName();
      }

    itkExceptionMacro(<< "No value filename value");
  }

  VectorDataType* GetVectorData() const
  {
    return m_VectorData;
  }

  void SetVectorData(VectorDataType* vectorData)
  {
    m_VectorData = vectorData;
    m_Reader = VectorDataFileReaderType::Pointer();
  }

  bool HasValue() const
  {
    return m_VectorData.IsNotNull();
  }

  void ClearValue()
  {
    m_VectorData = VectorDataType::Pointer();
    m_Reader = VectorDataFileReaderType::Pointer();
  }

protected:
  /** Constructor */
  InputVectorDataParameter()
  {
    this->SetName("Input Vector Data");
    this->SetKey("ivd");
  }

  /** Destructor */
  virtual ~InputVectorDataParameter()
  {}

  typedef otb::VectorDataFileReader<VectorDataType> VectorDataFileReaderType;
  VectorDataType::Pointer m_VectorData;
  VectorDataFileReaderType::Pointer m_Reader;

private:
  InputVectorDataParameter(const Parameter &); //purposely not implemented
  void operator =(const Parameter&); //purposely not implemented

};

} // End namespace Wrapper
} // End namespace otb

#endif
