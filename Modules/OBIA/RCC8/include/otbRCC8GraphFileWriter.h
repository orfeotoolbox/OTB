/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbRCC8GraphFileWriter_h
#define otbRCC8GraphFileWriter_h

#include "itkProcessObject.h"
#include "itkMacro.h"
#include "otbRCC8Graph.h"

namespace otb
{
/** \class RCC8GraphFileWriterException
 * \brief Base exception class for IO problems during writing.
 *
 * \ingroup OTBRCC8
 */
class ITK_EXPORT RCC8GraphFileWriterException
  : public itk::ExceptionObject
{
public:
  /** Run-time information. */
  itkTypeMacro(RCC8GraphFileWriterException, ExceptionObject);

  /** Constructor. */
  RCC8GraphFileWriterException(const char *file, unsigned int line,
                               const char* message = "Error in IO",
                               const char* loc = "Unknown") :
    ExceptionObject(file, line, message, loc) {}
  /** Constructor. */
  RCC8GraphFileWriterException(const std::string & file, unsigned int line,
                               const char* message = "Error in IO",
                               const char* loc = "Unknown") :
    ExceptionObject(file, line, message, loc) {}
};
/**
 * \class RCC8GraphFileWriter
 * \brief This class writes a RCC8 Graph to a dot file (graphviz file format).
 *
 * The writer first loops on the vertices of the graph, getting the property map
 * from each vertex and printing it in a line.
 *
 * It then iterates on the edges of the graphs, printing source index, target index,
 * and RCC8 value in a line for each of them.
 *
 * \sa RCC8GraphFileReader
 * \sa RCC8Graph
 *
 * \ingroup OTBRCC8
 */
template <class TInputGraph>
class ITK_EXPORT RCC8GraphFileWriter
  : public itk::ProcessObject
{
public:
  /** Standards typedefs */
  typedef RCC8GraphFileWriter           Self;
  typedef itk::ProcessObject            Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  /** Method for creation through the object factory */
  itkNewMacro(Self);
  /** Run-time type information (and related methods). */
  itkTypeMacro(RCC8GraphFileWriter, ProcessObject);
  /** Typedefs for the output graph type */
  typedef TInputGraph                                   InputGraphType;
  typedef typename InputGraphType::Pointer              InputGraphPointerType;
  typedef typename InputGraphType::VertexType           VertexType;
  typedef typename VertexType::Pointer                  VertexPointerType;
  typedef typename InputGraphType::VertexDescriptorType VertexDescriptorType;
  typedef typename InputGraphType::RCC8ValueType        RCC8ValueType;
  typedef typename VertexType::AttributesMapType        AttributesMapType;

  /** Set the filename */
  itkSetStringMacro(FileName);
  /** Get the filename */
  itkGetStringMacro(FileName);
  
  using Superclass::SetInput;
  /**
   * Set the input graph.
   * \param inputGraph The graph to write.
   */
  virtual void SetInput(const InputGraphType* inputGraph);
  /**
   * Get the input graph.
   * \return The input graph pointer.
   */
  virtual InputGraphPointerType GetInput();
  /**
   * Update method.
   */
  void Update(void) override;

protected:
  /** Constructor */
  RCC8GraphFileWriter();
  /** Destructor */
  ~RCC8GraphFileWriter() override;
  /**
   * Main computation method.
   */
  void GenerateData(void) override;
  /**
   * Write Method.
   * Performs checkings and invoke GenerateData().
   */
  virtual void Write(void);
  /**
   * Write an edge to file.
   * \param of The output file stream.
   * \param source The index of the source vertex.
   * \param target The index of the target vertex.
   * \param value  The value of the edge.
   */
  void WriteEdge(std::ofstream& of, VertexDescriptorType source,
                 VertexDescriptorType target, RCC8ValueType value);
  /**
   * Write a vertex to file.
   * \param of The output file stream.
   * \param index The index of the edge to write.
   * \param vertex The pointer to the vertex object.
  */
  void WriteVertex(std::ofstream& of, VertexDescriptorType index,
                   VertexPointerType vertex);
  /**
   * PrintSelf method
   */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  /** Filename of the graph file to write */
  std::string m_FileName;
};
} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbRCC8GraphFileWriter.hxx"
#endif

#endif
