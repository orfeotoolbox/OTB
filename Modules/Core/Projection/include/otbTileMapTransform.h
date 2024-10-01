/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbTileMapTransform_h
#define otbTileMapTransform_h


// Only for the enum definition
#include "otbGenericMapProjection.h"

namespace otb
{

/** \class TileMapTransform
 *  \brief to do
 *
 * \ingroup OTBProjection
 **/

template <TransformDirection TTransformDirection, class TScalarType = double, unsigned int NInputDimensions = 2,
          unsigned int NOutputDimensions = 2>
class ITK_EXPORT       TileMapTransform : public Transform<TScalarType, // Data type for scalars
                                                     NInputDimensions,  // Number of dimensions in the input space
                                                     NOutputDimensions> // Number of dimensions in the output space
{
public:
  /** Standard class typedefs. */
  typedef TileMapTransform Self;
  typedef Transform<TScalarType, NInputDimensions, NOutputDimensions> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef typename Superclass::ScalarType ScalarType;
  typedef itk::Point<ScalarType, NInputDimensions>  InputPointType;
  typedef itk::Point<ScalarType, NOutputDimensions> OutputPointType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(TileMapTransform, Transform);

  static const TransformDirection DirectionOfMapping = TTransformDirection;

  itkStaticConstMacro(InputSpaceDimension, unsigned int, NInputDimensions);
  itkStaticConstMacro(OutputSpaceDimension, unsigned int, NOutputDimensions);
  itkStaticConstMacro(SpaceDimension, unsigned int, NInputDimensions);
  itkStaticConstMacro(ParametersDimension, unsigned int, NInputDimensions*(NInputDimensions + 1));

  itkGetConstMacro(Depth, int);
  itkSetMacro(Depth, int);

  void SetLevel(unsigned int level);
  unsigned int GetLevel() const;

  OutputPointType TransformPoint(const InputPointType& point) const override;

  virtual void PrintMap() const;

  /** FIXME with a Mercator projection definition*/
  virtual std::string GetWkt() const
  {
    return "TileMapTransform";
  }

protected:
  TileMapTransform();
  ~TileMapTransform() override;

private:
  TileMapTransform(const Self&) = delete;
  void operator=(const Self&) = delete;

  int m_Depth;
};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbTileMapTransform.hxx"
#endif

#endif
