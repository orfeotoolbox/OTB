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
#ifndef __otbSpatialObjectSource_h
#define __otbSpatialObjectSource_h

#include "otbMacro.h"
#include "itkProcessObject.h"

namespace otb
{
/**
 * \class SpatialObjectSource
 * \brief Base class for filters producing a SpatialObject as output.
 * \ingroup DataSources
 *
 * \ingroup OTBSpatialObjects
 */
template <class TSpatialObject>
class ITK_EXPORT SpatialObjectSource
  : public itk::ProcessObject
{
public:
  /** Standard typedefs */
  typedef SpatialObjectSource           Self;
  typedef itk::ProcessObject            Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Creation through the object factory */
  itkNewMacro(Self);

  /** Runtime information */
  itkTypeMacro(SpatialObjectSource, itk::ProcessObject);

  /** Template parameters typedefs */
  typedef TSpatialObject                      SpatialObjectType;
  typedef typename SpatialObjectType::Pointer SpatialObjectPointerType;

  /** Data object pointer */
  typedef itk::DataObject::Pointer DataObjectPointer;

  /**
   * Get the output spatial object.
   * \return The output spatial object.
   */
  virtual SpatialObjectType * GetOutput(void);

protected:
  /** Constructor */
  SpatialObjectSource();
  /** Destructor */
  virtual ~SpatialObjectSource() {}
  /** PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  SpatialObjectSource(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSpatialObjectSource.txx"
#endif

#endif
