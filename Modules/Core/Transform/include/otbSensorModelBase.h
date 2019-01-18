/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbSensorModelBase_h
#define otbSensorModelBase_h


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
 *  in the sensor geometry.  (lat, lon) -> (i, j) or (lat, lon, h) -> (i, j)
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
  ~SensorModelBase() override;

  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  /** ImageKeywordlist */
  ImageKeywordlist m_ImageKeywordlist;
  /** Pointer on an ossim projection (created with the keywordlist) */
  SensorModelAdapter::Pointer m_Model;

private:
  SensorModelBase(const Self &) = delete;
  void operator =(const Self&) = delete;

};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSensorModelBase.hxx"
#endif

#endif
