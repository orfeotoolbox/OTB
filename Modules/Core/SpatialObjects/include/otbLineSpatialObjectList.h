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
#ifndef __otbLineSpatialObjectList_h
#define __otbLineSpatialObjectList_h


#include "itkDataObject.h"
#include "itkLineSpatialObject.h"

#include <list>

namespace otb
{
/** \class LineSpatialObjectList
 * \brief TODO
 *
 * \ingroup OTBSpatialObjects
 */

class ITK_EXPORT LineSpatialObjectList :        public std::list<itk::LineSpatialObject<2>::Pointer>,
  public itk::DataObject

{
public:
  /** Standard class typedefs. */
  typedef LineSpatialObjectList Self;
  typedef itk::DataObject       Superclass;

  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(LineSpatialObjectList, itk::DataObject);

  /** Some convenient typedefs. */
  typedef itk::LineSpatialObject<2> LineType;

protected:
  LineSpatialObjectList() {};
  ~LineSpatialObjectList() ITK_OVERRIDE {}

private:
  LineSpatialObjectList(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};

} // end namespace otb

#endif
