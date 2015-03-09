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
#ifndef __otbRCC8GraphFileWriter_txx
#define __otbRCC8GraphFileWriter_txx

#include "otbRCC8GraphFileWriter.h"
#include "otbRCC8VertexIterator.h"
#include "otbRCC8EdgeIterator.h"
#include "otbMacro.h"
#include <fstream>

namespace otb
{
/**
 * Constructor
 */
template <class TInputGraph>
RCC8GraphFileWriter<TInputGraph>
::RCC8GraphFileWriter()
{
  this->SetNumberOfRequiredInputs(1);
  m_FileName = "";
}
/**
 * Destructor
 */
template <class TInputGraph>
RCC8GraphFileWriter<TInputGraph>
::~RCC8GraphFileWriter()
{
}
/**
 * Set the input graph.
 * \param graph The graph to write.
 */
template <class TInputGraph>
void
RCC8GraphFileWriter<TInputGraph>
::SetInput(const InputGraphType * graph)
{
  this->itk::ProcessObject::SetNthInput(0, const_cast<TInputGraph *>(graph));
}
/**
 * Get the input graph.
 * \return The input graph pointer.
 */
template <class TInputGraph>
typename RCC8GraphFileWriter<TInputGraph>
::InputGraphPointerType
RCC8GraphFileWriter<TInputGraph>
::GetInput(void)
{
  return static_cast<TInputGraph*>(this->itk::ProcessObject::GetInput(0));
}
/**
 * Update method.
 * (Call the Write() method).
 */
template <class TInputGraph>
void
RCC8GraphFileWriter<TInputGraph>
::Update(void)
{
  this->Write();
}
/**
 * Write Method.
 * Performs checkings and invoke GenerateData().
 */
template <class TInputGraph>
void
RCC8GraphFileWriter<TInputGraph>
::Write(void)
{
  InputGraphType * input = this->GetInput();

  itkDebugMacro(<< "Writing a RCC8Graph file");

  // Make sure input is available
  if (input == 0)
    {
    itkExceptionMacro(<< "No input to writer!");
    }

  // Make sure that we can write the file given the name
  //
  if (m_FileName == "")
    {
    itkExceptionMacro(<< "No filename was specified");
    }

  // Pipeline updating sequence
  input->UpdateOutputInformation();
  input->PropagateRequestedRegion();
  input->UpdateOutputData();
  // GenerateData (actually write file)
  this->GenerateData();
}
/**
 * Main computation method.
 */
template <class TInputGraph>
void
RCC8GraphFileWriter<TInputGraph>
::GenerateData()
{
  otbMsgDevMacro(<< "RCC8GraphFileWriter: GenerateData call");
  // input graph pointer
  InputGraphPointerType input = this->GetInput();

  // iterators typedefs
  typedef otb::RCC8VertexIterator<InputGraphType> VertexIteratorType;
  typedef otb::RCC8EdgeIterator<InputGraphType>   EdgeIteratorType;

  // Output file stream
  std::ofstream out;

  // open the outputfile
  out.open(m_FileName.c_str(), std::ios::out);

  // Test if the file has been opened correctly
  if (!out)
    {
    RCC8GraphFileWriterException e(__FILE__, __LINE__);
    std::ostringstream msg;
    msg << " Could not create IO object for file ";
    msg << m_FileName << "." << std::endl;
    e.SetDescription(msg.str().c_str());
    throw e;
    return;
    }

  // Start writing the graph to file
  out << "digraph G {" << std::endl;

  // For each vertex in the graph
  VertexIteratorType vIt(input);
  for (vIt.GoToBegin(); !vIt.IsAtEnd(); ++vIt)
    {
    this->WriteVertex(out, vIt.GetIndex(), vIt.Get());
    }

  // For each edge in the graph
  EdgeIteratorType eIt(input);
  for (eIt.GoToBegin(); !eIt.IsAtEnd(); ++eIt)
    {
    this->WriteEdge(out, eIt.GetSourceIndex(),
                    eIt.GetTargetIndex(),
                    eIt.GetValue());
    }

  // Ends the graph writing
  out << "}" << std::endl;

  // Close the file
  out.close();
}
/**
 * Write an edge to file.
 * \param of The output file stream.
 * \param source The index of the source vertex.
 * \param target The index of the target vertex.
 * \param value  The value of the edge.
 */
template <class TInputGraph>
void
RCC8GraphFileWriter<TInputGraph>
::WriteEdge(std::ofstream& of, VertexDescriptorType source,
            VertexDescriptorType target, RCC8ValueType value)
{
  otbMsgDevMacro(<< "RCC8GraphFileWriter: WriteEdge call: " << source << " " << target << " " << value);
  of << source << " -> " << target << " ";
  of << "[Value=\"" << value << "\"];";
  of << std::endl;
}
/**
 * Write a vertex to file.
 * \param of The output file stream.
 * \param index The index of the edge to write.
 * \param vertex The pointer to the vertex object.
 */
template <class TInputGraph>
void
RCC8GraphFileWriter<TInputGraph>
::WriteVertex(std::ofstream& of, VertexDescriptorType index,
              VertexPointerType vertex)
{
  typedef typename VertexType::AttributesMapType AttributesMapType;
  typedef typename AttributesMapType::iterator   IteratorType;
  AttributesMapType attr = vertex->GetAttributesMap();
  otbMsgDevMacro(<< "RCC8GraphFileWriter: WriteVertex call: " << index);
  of << index << " [";
  IteratorType it = attr.begin();
  while (it != attr.end())
    {
    of << (*it).first << "=\"";
    of << (*it).second << "\"";
    ++it;
    if (it == attr.end())
      {
      of << "];" << std::endl;
      }
    else
      {
      of << ",";
      }
    }
}
/**
 * PrintSelf method
 */
template <class TInputGraph>
void
RCC8GraphFileWriter<TInputGraph>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // namespace otb
#endif
