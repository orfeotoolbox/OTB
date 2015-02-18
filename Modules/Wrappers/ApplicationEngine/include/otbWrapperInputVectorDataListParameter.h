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
#ifndef __otbWrapperInputVectorDataListParameter_h
#define __otbWrapperInputVectorDataListParameter_h

#include "otbVectorDataFileReader.h"

#include "otbWrapperParameter.h"
#include "otbObjectList.h"

namespace otb
{
namespace Wrapper
{
/** \class InputVectorDataListParameter
 *  \brief This class represents a list of VectorData parameter
 */

class ITK_ABI_EXPORT InputVectorDataListParameter : public Parameter
{
public:
  /** Standard class typedef */
  typedef InputVectorDataListParameter           Self;
  typedef Parameter                     Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef otb::VectorDataFileReader<VectorDataType>  VectorDataFileReaderType;
  typedef otb::ObjectList<VectorDataFileReaderType>  VectorDataFileReaderListType;

  /** Defining ::New() static method */
  itkNewMacro(Self);

  /** RTTI support */
  itkTypeMacro(InputVectorDataListParameter, Parameter);

  /** Set image form a list of filename */
  bool SetListFromFileName(const std::vector<std::string> & filenames);

  /** Add null element to lists. */
  void AddNullElement();

  /** Add an image from a filename */
  bool AddFromFileName(const std::string & filename);

  /** Set one specific stored image filename. */
  bool SetNthFileName( const unsigned int id, const std::string & filename );


  /** Get the stored image filename list */
  std::vector<std::string> GetFileNameList() const;

 /** Get one specific stored image filename. */
  std::string GetNthFileName( unsigned int i ) const;

  /** Get one list of the stored image. */
  VectorDataListType* GetVectorDataList() const;

  /** Get one specific stored image. */
  VectorDataType* GetNthVectorData(unsigned int i) const;

  /** Set the list of image. */
  void SetVectorDataList(VectorDataListType* vdList);

  /** Add an image to the list. */
  void AddVectorData(VectorDataType* image);

  bool HasValue() const;


  /** Erase one element of the list. */
  void Erase( unsigned int id );

 /** Clear all the list. */
  virtual void ClearValue();


protected:
  /** Constructor */
  InputVectorDataListParameter();

  /** Destructor */
  virtual ~InputVectorDataListParameter();

  VectorDataListType::Pointer m_VectorDataList;
  VectorDataFileReaderListType::Pointer  m_ReaderList;

private:
  InputVectorDataListParameter(const Parameter &); //purposely not implemented
  void operator =(const Parameter&); //purposely not implemented

}; // End class InputVectorDataList Parameter

} // End namespace Wrapper
} // End namespace otb

#endif
