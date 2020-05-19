/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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


#ifndef otbVectorDataFileWriter_hxx
#define otbVectorDataFileWriter_hxx

#include "otbMacro.h"
#include "otbVectorDataFileWriter.h"
#include "otbVectorDataIOFactory.h"
#include "otbVectorDataAdapter.h"
#include "otbVectorData.h"

namespace otb
{
/**
 * Constructor
 */
template <class TInputVectorData>
VectorDataFileWriter<TInputVectorData>::VectorDataFileWriter()
  : m_FileName(""), m_VectorDataIO(nullptr), m_UserSpecifiedVectorDataIO(false), m_FactorySpecifiedVectorDataIO(false)
{
}
/**
 * Destructor
 */
template <class TInputVectorData>
VectorDataFileWriter<TInputVectorData>::~VectorDataFileWriter()
{
}

//---------------------------------------------------------
template <class TInputVectorData>
void VectorDataFileWriter<TInputVectorData>::SetInput(const InputVectorDataType* input)
{
  // ProcessObject is not const_correct so this cast is required here.
  this->ProcessObject::SetNthInput(0, const_cast<TInputVectorData*>(input));
}

//---------------------------------------------------------
template <class TInputVectorData>
const typename VectorDataFileWriter<TInputVectorData>::InputVectorDataType* VectorDataFileWriter<TInputVectorData>::GetInput(void)
{
  if (this->GetNumberOfInputs() < 1)
  {
    return nullptr;
  }

  return static_cast<TInputVectorData*>(this->ProcessObject::GetInput(0));
}

//---------------------------------------------------------
template <class TInputVectorData>
const typename VectorDataFileWriter<TInputVectorData>::InputVectorDataType* VectorDataFileWriter<TInputVectorData>::GetInput(unsigned int idx)
{
  return static_cast<TInputVectorData*>(this->ProcessObject::GetInput(idx));
}

//---------------------------------------------------------
template <class TInputVectorData>
void VectorDataFileWriter<TInputVectorData>::Write()
{
  const InputVectorDataType* input = this->GetInput();

  itkDebugMacro(<< "Writing a vector data file");

  // Make sure input is available
  if (input == nullptr)
  {
    itkExceptionMacro(<< "No input to writer!");
  }

  // Make sure that we can write the file given the name
  //
  if (m_FileName == "")
  {
    itkExceptionMacro(<< "No filename was specified");
  }

  if (m_VectorDataIO.IsNull()) // try creating via factory
  {
    itkDebugMacro(<< "Attempting factory creation of VectorDataIO for file: " << m_FileName);
    m_VectorDataIO                 = VectorDataIOFactory::CreateVectorDataIO(m_FileName.c_str(), VectorDataIOFactory::WriteMode);
    m_FactorySpecifiedVectorDataIO = true;
  }
  else
  {
    if (m_FactorySpecifiedVectorDataIO && !m_VectorDataIO->CanWriteFile(m_FileName.c_str()))
    {
      itkDebugMacro(<< "VectorDataIO exists but doesn't know how to write file:" << m_FileName);
      itkDebugMacro(<< "Attempting creation of VectorDataIO with a factory for file:" << m_FileName);
      m_VectorDataIO                 = VectorDataIOFactory::CreateVectorDataIO(m_FileName.c_str(), VectorDataIOFactory::WriteMode);
      m_FactorySpecifiedVectorDataIO = true;
    }
  }

  if (m_VectorDataIO.IsNull())
  {
    VectorDataFileWriterException e(__FILE__, __LINE__);
    std::ostringstream            msg;
    msg << " Could not create IO object for file " << m_FileName << std::endl;
    msg << "  Tried to create one of the following:" << std::endl;
    std::list<itk::LightObject::Pointer> allobjects = itk::ObjectFactoryBase::CreateAllInstance("otbVectorDataIOBase");
    for (std::list<LightObject::Pointer>::iterator i = allobjects.begin(); i != allobjects.end(); ++i)
    {
      VectorDataIOBase* io = dynamic_cast<VectorDataIOBase*>(i->GetPointer());
      if (io)
        msg << "    " << io->GetNameOfClass() << std::endl;
    }
    msg << "  You probably failed to set a file suffix, or" << std::endl;
    msg << "    set the suffix to an unsupported type." << std::endl;
    e.SetDescription(msg.str());
    e.SetLocation(ITK_LOCATION);
    throw e;
  }

  // NOTE: this const_cast<> is due to the lack of const-correctness
  // of the ProcessObject.
  InputVectorDataType* nonConstVectorData = const_cast<InputVectorDataType*>(input);

  // Make sure the data is up-to-date.
  if (nonConstVectorData->GetSource())
  {
    nonConstVectorData->GetSource()->Update();
  }

  // Notify start event observers
  this->InvokeEvent(itk::StartEvent());

  // Actually do something
  this->GenerateData();

  // Notify end event observers
  this->InvokeEvent(itk::EndEvent());

  // Release upstream data if requested
  if (input->ShouldIReleaseData())
  {
    nonConstVectorData->ReleaseData();
  }
}

//---------------------------------------------------------
template <class TInputVectorData>
void VectorDataFileWriter<TInputVectorData>::GenerateData(void)
{
  const InputVectorDataType* input = this->GetInput();

  itkDebugMacro(<< "Writing file: " << m_FileName);

  // Setup the vector data IO for writing.
  //
  m_VectorDataIO->SetFileName(m_FileName);

  typedef VectorDataAdapter<TInputVectorData, VectorData<double, 2>> AdapterType;
  typename AdapterType::Pointer adapter = AdapterType::New();
  adapter->SetInput(input);
  adapter->Update();
  m_VectorDataIO->Write(adapter->GetOutput());
}

template <class TInputVectorData>
void VectorDataFileWriter<TInputVectorData>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "VectorDataFileWriter" << std::endl;
}

} // namespace otb

#endif
