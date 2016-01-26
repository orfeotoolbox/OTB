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
#ifndef __otbSensorModelBase_h
#define __otbSensorModelBase_h

#include <iostream>

#include "otbMacro.h"
#include "otbImageKeywordlist.h"
#include "otbSensorModelAdapter.h"

#include "otbTransform.h"
#include "itkSmartPointer.h"

namespace otb
{
/** \class SensorModelBase
 *  \brief Base class for the sensor model projection classes.
 *
 *  This class allows transforming a geographic point in (lat, long) to a point
 *  in the sensor geometry.  (lat, lon) -> (i, j) ou (lat, lon, h) -> (i, j)
 *
 * \ingroup Projection
 *
 *
 * \ingroup OTBTransform
 */
template <class TScalarType,
    unsigned int NInputDimensions = 3,
    unsigned int NOutputDimensions = 2>
class ITK_EXPORT SensorModelBase : public Transform<TScalarType,
      NInputDimensions,
      NOutputDimensions>
{

public:

  /** Standard class typedefs. */
  typedef SensorModelBase               Self;
  typedef Transform<TScalarType,
      NInputDimensions,
      NOutputDimensions>                Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef itk::Point<TScalarType, NInputDimensions>  InputPointType;
  typedef itk::Point<TScalarType, NOutputDimensions> OutputPointType;

  typedef TScalarType                                      PixelType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(SensorModelBase, Transform);

  itkStaticConstMacro(InputSpaceDimension,  unsigned int, NInputDimensions);
  itkStaticConstMacro(OutputSpaceDimension, unsigned int, NOutputDimensions);

  /* Get the ImageKeywordlist */
  ImageKeywordlist GetImageGeometryKeywordlist(void) const;

  /*
   * Set the Imagekeywordlist and affect the ossim projection ( m_Model)
   * Return false if not model found.
   */
  virtual void SetImageGeometry(const ImageKeywordlist& image_kwl);

  /** Is sensor model valid method. return false if the sensor model is null */
  bool IsValidSensorModel()
  {
    return m_Model->IsValidSensorModel();
  }

protected:
  SensorModelBase();
  virtual ~SensorModelBase();

  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** ImageKeywordlist */
  ImageKeywordlist m_ImageKeywordlist;
  /** Pointer on an ossim projection (created with the keywordlist) */
  SensorModelAdapter::Pointer m_Model;

private:
  SensorModelBase(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSensorModelBase.txx"
#endif

#endif
