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
#ifndef __otbSpatialObjectDXFReader_h
#define __otbSpatialObjectDXFReader_h

#include "itkGroupSpatialObject.h"
#include "otbSpatialObjectSource.h"

namespace otb
{
  /** \class SpatialObjectDXFReader
   *  \brief This class reads the DXF file format to SpatialObjects.
   *
   *  \ingroup ObjectSource
   *
   * \example IO/DXFReaderExample.cxx
   *
   */
template <class TSpatialObject>
class ITK_EXPORT SpatialObjectDXFReader
  : public SpatialObjectSource< TSpatialObject >
{
 public:
  typedef SpatialObjectDXFReader Self;
  typedef SpatialObjectSource<TSpatialObject> Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(SpatialObjectDXFReader, SpatialObjectSource);

 /** Superclass Typedefs */
 typedef TSpatialObject GroupSpatialObjectType;
 typedef typename GroupSpatialObjectType::Pointer GroupSpatialObjectPointerType;

  /** Set the filename  */
  void SetFileName(char * filename);

  /** Get the filename */
  itkGetStringMacro(FileName);

 /** Set The layer name */
 itkSetStringMacro(LayerName);

 /** Get The layer name */
 itkGetStringMacro(LayerName);

 /** Workaround while waiting for the SpatialObject bug to be fixed */
 virtual void Update(void);

 protected:
 /** Constructor */
  SpatialObjectDXFReader();
 /** Destructor */
  ~SpatialObjectDXFReader();
  /** Main computation method */
  virtual void GenerateData();

 private:
  SpatialObjectDXFReader(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** Test whether the given filename exist and it is readable,
      this is intended to be called before attempting to use
      ImageIO classes for actually reading the file. If the file
      doesn't exist or it is not readable, and exception with an
      approriate message will be thrown. */
  void TestFileExistanceAndReadability();

  std::string m_FileName;
  std::string  m_LayerName;
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSpatialObjectDXFReader.txx"
#endif

#endif
