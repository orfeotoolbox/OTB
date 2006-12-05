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
  m_FileName = "";
  otbMsgDebugMacro(<<"RCC8GraphFileWriter: Call to constructor");
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
 * \param inputGraph The graph to write.
 */
template <class TInputGraph>
void 
RCC8GraphFileWriter<TInputGraph>
::SetInput(const InputGraphType * graph)
{
  this->itk::ProcessObject::SetNthInput(0,const_cast<TInputGraph *>(graph));
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
//   if(this->GetNumberOfInputs()<1)
//     {
//       return 0;
//     }
  return static_cast<TInputGraph*>(this->itk::ProcessObject::GetInput(0));
}
/**
 * Update method
 */
template <class TInputGraph>
void
RCC8GraphFileWriter<TInputGraph>
::Update(void)
{
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
  otbMsgDebugMacro(<<"RCC8GraphFileWriter: GenerateData call");
  // input graph pointer
  InputGraphPointerType input = this->GetInput();
  
  // iterators typedefs
  typedef otb::RCC8VertexIterator<InputGraphType> VertexIteratorType;
  typedef otb::RCC8EdgeIterator<InputGraphType> EdgeIteratorType;

  // Output file stream
  std::ofstream out;
  // open the outputfile
  out.open(m_FileName.c_str(), std::ios::out);
  
  // Start writing the graph to file
  out<<"digraph G {"<<std::endl;

  // For each vertex in the graph
  VertexIteratorType vIt(input);
  for(vIt.GoToBegin();!vIt.IsAtEnd();++vIt)
    {
      this->WriteVertex(out,vIt.GetIndex(),vIt.Get());
    }

  // For each edge in the graph
  EdgeIteratorType eIt(input);
  for(eIt.GoToBegin();!eIt.IsAtEnd();++eIt)
    {
      this->WriteEdge(out, eIt.GetSourceIndex(),
		      eIt.GetTargetIndex(),
		      eIt.GetValue());
    }

  // Ends the graph writing
  out<<"}"<<std::endl;

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
::WriteEdge(std::ofstream& of,VertexDescriptorType source,
		 VertexDescriptorType target, RCC8ValueType value)
{
  otbMsgDebugMacro(<<"RCC8GraphFileWriter: WriteEdge call: "<<source<<" "<<target<<" "<<value);
  of<<source<<" -> "<<target<<" ";
  of<<"[Value=\""<<value<<"\"];";
  of<<std::endl;
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
  typedef typename VertexType::AttributesVectorType AttributesVectorType;
  typedef typename AttributesVectorType::iterator IteratorType;
  AttributesVectorType attr = vertex->GetAttributesVector();
  if(attr.size()%2!=0)
    {
      // We may throw an exception here
      return;
    }
  else
    {
      otbMsgDebugMacro(<<"RCC8GraphFileWriter: WriteVertex call: "<<index);
      of<<index<<" [";
      IteratorType it = attr.begin();
      while(it<attr.end())
	{
	  of<<(*it)<<"=\"";
	  ++it;
	  of<<(*it)<<"\"";
	  ++it;
	  if(it==attr.end())
	    {
	     of<<"];"<<std::endl;
	    }
	  else
	    {
	      of<<",";
	    } 
	}
    }
}
/**
 * PrintSelf method
 */
template <class TInputGraph>
void
RCC8GraphFileWriter<TInputGraph>
::PrintSelf( std::ostream& os,itk::Indent indent ) const
{
  Superclass::PrintSelf(os,indent);
}
} // namespace otb
#endif
