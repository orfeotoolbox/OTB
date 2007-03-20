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
   *  \brief
   *  \ingroups
   */
template <unsigned int VDimension = 2>
class ITK_EXPORT SpatialObjectDXFReader 
  : public SpatialObjectSource< itk::GroupSpatialObject<VDimension> >
{
 public:
  typedef SpatialObjectDXFReader Self;
  typedef SpatialObjectSource< itk::GroupSpatialObject<VDimension> > Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
 		
  /** Method for creation through the object factory */
  itkNewMacro(Self);
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(SpatialObjectDXFReader, SpatialObjectSource);
  
 /** Superclass Typedefs */
 typedef typename Superclass::SpatialObjectType GroupSpatialObjectType;
 typedef typename GroupSpatialObjectType::Pointer GroupSpatialObjectPointerType;
 
  /** Set the filename  */
  itkSetStringMacro(FileName);
  
  /** Get the filename */
  itkGetStringMacro(FileName);
  
 /** Set The layer name */
 itkSetStringMacro(LayerName);
 
 /** Get The layer name */
 itkGetStringMacro(LayerName);
  

  /** Main computation method */
 void GenerateData(void);

 protected: 
 /** Constructor */
  SpatialObjectDXFReader();
 /** Destructor */
  virtual ~SpatialObjectDXFReader();


  private:
  SpatialObjectDXFReader(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  
  std::string m_FileName;
  std::string  m_LayerName;	
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSpatialObjectDXFReader.txx"
#endif

#endif
