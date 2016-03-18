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
#ifndef __otbRCC8GraphFileReader_h
#define __otbRCC8GraphFileReader_h

#include "otbRCC8GraphSource.h"
#include "itkMacro.h"

namespace otb
{
/** \class RCC8GraphFileReaderException
 * \brief Base exception class for IO problems during reading.
 *
 * \ingroup OTBRCC8
 */
class ITK_EXPORT RCC8GraphFileReaderException
  : public itk::ExceptionObject
{
public:
  /** Run-time information. */
  itkTypeMacro(RCC8GraphFileReaderException, ExceptionObject);

  /** Constructor. */
  RCC8GraphFileReaderException(const char *file, unsigned int line,
                               const char* message = "Error in IO",
                               const char* loc = "Unknown") :
    ExceptionObject(file, line, message, loc) {}
  /** Constructor. */
  RCC8GraphFileReaderException(const std::string & file, unsigned int line,
                               const char* message = "Error in IO",
                               const char* loc = "Unknown") :
    ExceptionObject(file, line, message, loc) {}
};
/**
 * \class RCC8GraphFileReader
 * \brief This class reads a RCC8 graph from a .dot file (graphviz format).
 *
 * The parsing algorithm reads the lines from the file, deciding if the line is
 * a vertex or edge line. It then call the correct parse method between ParseEdge
 * and ParseVertex. The ParseVertex use builds an AttributesMap and pass it to a new
 * vertex.
 *
 * \sa RCC8GraphFileWriter
 * \sa RCC8Graph
 *
 * \ingroup OTBRCC8
 */
template <class TOutputGraph>
class ITK_EXPORT RCC8GraphFileReader : public RCC8GraphSource<TOutputGraph>
{
public:
  /** Standards typedef */
  typedef RCC8GraphFileReader           Self;
  typedef RCC8GraphSource<TOutputGraph> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  /** Creation through the object factory */
  itkNewMacro(Self);
  /** Runtime type information */
  itkTypeMacro(RCC8GraphFileReader, RCC8GraphSource);
  /** Output related typedefs */
  typedef TOutputGraph                                OutputGraphType;
  typedef typename Superclass::OutputGraphPointerType OutputGraphPointerType;
  typedef typename OutputGraphType::VertexType        VertexType;
  typedef typename VertexType::Pointer                VertexPointerType;
  typedef typename OutputGraphType::RCC8ValueType     RCC8ValueType;

  /** Set the filename  */
  itkSetStringMacro(FileName);
  /** Get the filename */
  itkGetStringMacro(FileName);

protected:
  /** Constructor */
  RCC8GraphFileReader();
  /** Destructor */
  virtual ~RCC8GraphFileReader();
  /** Main computation method */
  virtual void GenerateData();
  /**
   * Parse edge information from a given line.
   * \param line The line to parse.
   */
  void ParseEdge(const std::string& line);
  /**
   * Parse vertex information from a given line.
   * \param line The line to parse.
   */
  void  ParseVertex(const std::string& line);

  /** PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  /** File name */
  std::string m_FileName;
};
}
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbRCC8GraphFileReader.txx"
#endif

#endif
