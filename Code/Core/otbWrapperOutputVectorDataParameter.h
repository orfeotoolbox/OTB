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
#ifndef __otbWrapperOutputVectorDataParameter_h
#define __otbWrapperOutputVectorDataParameter_h

#include "otbVectorData.h"
#include "otbWrapperParameter.h"

namespace otb
{
namespace Wrapper
{
/** \class OutputVectorDataParameter
 *  \brief This class represents a OutputVectorData parameter
 */

class ITK_EXPORT OutputVectorDataParameter : public Parameter
{
public:
  /** Standard class typedef */
  typedef OutputVectorDataParameter      Self;
  typedef Parameter                     Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Defining ::New() static method */
  itkNewMacro(Self);

  /** RTTI support */
  itkTypeMacro(OutputVectorDataParameter,Parameter);


  /** Set the value */
  itkSetObjectMacro(VectorData, VectorDataType);

  /** Get the value */
  itkGetObjectMacro(VectorData, VectorDataType);

  /** Return any value */
  void SetValue(VectorDataType* vd)
  {
    m_VectorData = vd;
  }

  /** Return any value */
  VectorDataType* GetValue( void )
  {
    return m_VectorData;
  }

  itkSetStringMacro(FileName);
  itkGetStringMacro(FileName);

protected:
  /** Constructor */
  OutputVectorDataParameter()
  {
    this->SetName("Output Vector Data");
    this->SetKey("outvd");
  }

  /** Destructor */
  virtual ~OutputVectorDataParameter()
  {}

  VectorDataType::Pointer m_VectorData;
  std::string m_FileName;

private:
  OutputVectorDataParameter(const Parameter &); //purposely not implemented
  void operator =(const Parameter&); //purposely not implemented

};

} // End namespace Wrapper
} // End namespace otb

#endif
