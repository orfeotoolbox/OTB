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

#ifndef __otbVectorDataFileReader_txx
#define __otbVectorDataFileReader_txx

#include <fstream>

#include "otbMacro.h"
#include "otbSystem.h"
#include "otbVectorDataIOFactory.h"
#include "otbVectorDataFileReader.h"
#include "itksys/SystemTools.hxx"
#include "otbVectorDataAdapter.h"
#include "otbVectorData.h"

namespace otb
{
/**
 * Constructor
 */
template <class TOutputVectorData>
VectorDataFileReader<TOutputVectorData>
::VectorDataFileReader() :
  m_VectorDataIO(NULL),
  m_UserSpecifiedVectorDataIO(false),
  m_FileName("")
{
}

/**
 * Destructor
 */
template <class TOutputVectorData>
VectorDataFileReader<TOutputVectorData>
::~VectorDataFileReader()
{
}

/** Test whether the given filename exist and it is readable,
     this is intended to be called before attempting to use
     VectorDataIO classes for actually reading the file. If the file
     doesn't exist or it is not readable, and exception with an
     approriate message will be thrown. */
template <class TOutputVectorData>
void
VectorDataFileReader<TOutputVectorData>
::TestFileExistanceAndReadability()
{
  // Test if the file exists.
  if (!itksys::SystemTools::FileExists(this->m_FileName.c_str()))
    {
    VectorDataFileReaderException e(__FILE__, __LINE__);
    std::ostringstream msg;
    msg << "The file doesn't exist. "
        << std::endl << "Filename = " << this->m_FileName
        << std::endl;
    e.SetDescription(msg.str().c_str());
    throw e;
    return;
    }

  // Test if the file can be open for reading access.
  //Only if m_FileName speciy a filename (not a dirname)
  // FIXME shapefile should be able to open a directory and load the
  // individual shapefiles as layers
  if (itksys::SystemTools::FileExists(this->m_FileName.c_str(), true) == true)
    {
    std::ifstream readTester;
    readTester.open(this->m_FileName.c_str());
    if (readTester.fail())
      {
      readTester.close();
      std::ostringstream msg;
      msg << "The file couldn't be opened for reading. "
          << std::endl << "Filename: " << this->m_FileName
          << std::endl;
      VectorDataFileReaderException e(__FILE__, __LINE__, msg.str().c_str(), ITK_LOCATION);
      throw e;
      return;

      }
    readTester.close();
    }
}

template <class TOutputVectorData>
void
VectorDataFileReader<TOutputVectorData>
::SetVectorDataIO(VectorDataIOBaseType * vectorDataIO)
{
  itkDebugMacro("setting VectorDataIO to " << vectorDataIO);
  if (this->m_VectorDataIO != vectorDataIO)
    {
    this->m_VectorDataIO = vectorDataIO;
    this->Modified();
    }
  m_UserSpecifiedVectorDataIO = true;
}

template <class TOutputVectorData>
void
VectorDataFileReader<TOutputVectorData>
::GenerateOutputInformation(void)
{
  typename TOutputVectorData::Pointer output = this->GetOutput();

  itkDebugMacro(<< "Reading file for GenerateOutputInformation()" << m_FileName);

  // Check to see if we can read the file given the name or prefix
  //
  if (m_FileName == "")
    {
    throw VectorDataFileReaderException(__FILE__, __LINE__, "FileName must be specified", ITK_LOCATION);
    }

  // Test if the file exist and if it can be open.
  // and exception will be thrown otherwise.
  //
  try
    {
    m_ExceptionMessage = "";
    this->TestFileExistanceAndReadability();
    }
  catch (itk::ExceptionObject & err)
    {
    m_ExceptionMessage = err.GetDescription();
    }

  if (m_UserSpecifiedVectorDataIO == false)   //try creating via factory
    {
    m_VectorDataIO = VectorDataIOFactory::CreateVectorDataIO(
      m_FileName.c_str(), VectorDataIOFactory::ReadMode);
    }

  if (m_VectorDataIO.IsNull())
    {
    std::ostringstream msg;
    msg << " Could not create IO object for file "
        << m_FileName.c_str() << std::endl;
    if (m_ExceptionMessage.size())
      {
      msg << m_ExceptionMessage;
      }
    else
      {
      msg << "  Tried to create one of the following:" << std::endl;
      std::list<itk::LightObject::Pointer> allobjects =
        itk::ObjectFactoryBase::CreateAllInstance("otbVectorDataIOBase");
      for (std::list<itk::LightObject::Pointer>::iterator i = allobjects.begin();
           i != allobjects.end(); ++i)
        {
        VectorDataIOBase* io = dynamic_cast<VectorDataIOBase*>(i->GetPointer());
        msg << "    " << io->GetNameOfClass() << std::endl;
        }
      msg << "  You probably failed to set a file suffix, or" << std::endl;
      msg << "    set the suffix to an unsupported type." << std::endl;
      }
    VectorDataFileReaderException e(__FILE__, __LINE__, msg.str().c_str(), ITK_LOCATION);
    throw e;
    return;
    }

  m_VectorDataIO->SetFileName(m_FileName.c_str());

  //Copy MetaDataDictionary from instantiated reader to output VectorData.
  output->SetMetaDataDictionary(m_VectorDataIO->GetMetaDataDictionary());
  this->SetMetaDataDictionary(m_VectorDataIO->GetMetaDataDictionary());
}

template <class TOutputVectorData>
void
VectorDataFileReader<TOutputVectorData>
::GenerateData()
{

  typename TOutputVectorData::Pointer output = this->GetOutput();

  itkDebugMacro (<< "VectorDataFileReader::GenerateData() \n");

  // Test if the file exist and if it can be open.
  // and exception will be thrown otherwise.
  try
    {
    m_ExceptionMessage = "";
    this->TestFileExistanceAndReadability();
    }
  catch (itk::ExceptionObject & err)
    {
    m_ExceptionMessage = err.GetDescription();
    }

  m_VectorDataIO->SetFileName(m_FileName.c_str());

  // Tell the VectorDataIO to read the file
  //

  typedef VectorDataAdapter<VectorData<double, 2>, TOutputVectorData> AdapterType;
  typename AdapterType::InputVectorDataType::Pointer input = AdapterType::InputVectorDataType::New();

  m_VectorDataIO->Read(input);

  typename AdapterType::Pointer adapter = AdapterType::New();
  adapter->SetInput(input);
  adapter->GraftOutput(output);
  adapter->Update();
  this->GraftOutput(adapter->GetOutput());

  return;
}

template <class TOutputVectorData>
void
VectorDataFileReader<TOutputVectorData>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  if (m_VectorDataIO)
    {
    os << indent << "VectorDataIO: \n";
    m_VectorDataIO->Print(os, indent.GetNextIndent());
    }
  else
    {
    os << indent << "m_VectorDataIO: (null)" << "\n";
    }

  os << indent << "UserSpecifiedVectorDataIO flag: " << m_UserSpecifiedVectorDataIO << "\n";
  os << indent << "m_FileName: " << m_FileName << "\n";
}

} //namespace otb

#endif
