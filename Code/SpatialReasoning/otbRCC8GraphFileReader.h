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
#include "itkExceptionObject.h"

#include "otbImageFileReader.h"

namespace otb
{
/** \class RCC8GraphFileReaderException
 * \brief Base exception class for IO problems during reading. 
 */ 
class ITK_EXPORT RCC8GraphFileReaderException 
  : public itk::ExceptionObject 
{
public:
  /** Run-time information. */
  itkTypeMacro( RCC8GraphFileReaderException, ExceptionObject );

  /** Constructor. */
  RCC8GraphFileReaderException(const char *file, unsigned int line, 
                           const char* message = "Error in IO",
                           const char* loc = "Unknown" ) : 
    ExceptionObject(file, line, message, loc)
  {}
  /** Constructor. */
  RCC8GraphFileReaderException(const std::string &file, unsigned int line, 
                           const char* message = "Error in IO",
                           const char* loc = "Unknown" ) :
    ExceptionObject(file, line, message, loc)
  {}
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
 * The flag ReadSegmentationImages enable or disable the reading of the segmentation images.
 * If this is enabled, the RCC8GraphFileReader will look for image files with the same name
 * than the graph filename, concatenated with the segmentation image index, and the image extension.
 *
 * For instance, if the graph file is "graph.dot", and the image extension "tif", and if the graph comes
 * from 6 segmentations image (information extracted from the Vertices properties), the reader will look for
 * image files named graph1.tif, graph2.tif ..., graph6.tif. If one of the segmentation image is missing, none of
 * them are loaded into the graph object output.
 *
 * \sa RCC8GraphFileWriter
 * \sa RCC8Graph
 */
template <class TOutputGraph>
class ITK_EXPORT RCC8GraphFileReader : public RCC8GraphSource<TOutputGraph>
{
public:
  /** Standards typedef */
  typedef RCC8GraphFileReader Self;
  typedef RCC8GraphSource<TOutputGraph> Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  /** Creation through the object factory */
  itkNewMacro(Self);
  /** Runtime type informations */
  itkTypeMacro(RCC8GraphFileReader,RCC8GraphSource);
  /** Output related typedefs */
  typedef TOutputGraph OutputGraphType;
  typedef typename Superclass::OutputGraphPointerType OutputGraphPointerType;
  typedef typename OutputGraphType::VertexType VertexType;
  typedef typename VertexType::Pointer VertexPointerType;
  typedef typename OutputGraphType::RCC8ValueType RCC8ValueType;
  typedef typename OutputGraphType::SegmentationImageType SegmentationImageType;
  typedef typename OutputGraphType::SegmentationImagePointerType SegmentationImagePointerType;
  typedef typename OutputGraphType::SegmentationImageListType SegmentationImageListType;
  typedef typename OutputGraphType::SegmentationImageListPointerType SegmentationImageListPointerType;
  typedef ImageFileReader<SegmentationImageType> SegmentationImageReaderType;
  typedef typename SegmentationImageReaderType::Pointer SegmentationImageReaderPointerType;

  /** Set the filename  */
  itkSetStringMacro(FileName);
  /** Get the filename */
  itkGetStringMacro(FileName);
  /** Flag to enable/Disable the reading of segmentation images */
  itkSetMacro(ReadSegmentationImages,bool);
  itkGetMacro(ReadSegmentationImages,bool);
  itkBooleanMacro(ReadSegmentationImages);
  /** Set the segmentation image extension */
  itkSetStringMacro(ImageExtension);
  /** Get the segmentation image extension */
  itkGetStringMacro(ImageExtension);

protected:
  /** Constructor */
  RCC8GraphFileReader();
  /** Destructor */
  ~RCC8GraphFileReader();
  /** Main computation method */
  virtual void GenerateData();
  /**
   * Parse edge informations from a given line.
   * \param line The line to parse.
   */
  void ParseEdge(std::string line);
  /**
   * Parse vertex informations from a given line.
   * \param line The line to parse.
   */
  void  ParseVertex(std::string line);
  /** PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

 private:
  /** File name */
  std::string m_FileName;
  std::string m_ImageExtension;
  bool m_ReadSegmentationImages;
};
}
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbRCC8GraphFileReader.txx"
#endif

#endif 
