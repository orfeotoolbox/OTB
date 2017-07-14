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

#ifndef otbGeometriesRegion_h
#define otbGeometriesRegion_h

#include "itkRegion.h"

#include "OTBGdalAdaptersExport.h"

namespace otb
{

/**
 * \class GeometriesRegion
 * 
 * Region type applied to geometries, implemented as a tagged-union
 */
class OTBGdalAdapters_EXPORT GeometriesRegion : public itk::Region
{
public:
  /**\name Standard ITK typedefs */
  //@{
  typedef GeometriesRegion              Self;
  typedef itk::Region                   Superclass;
  //@}

  /**\name Standard macros */
  //@{
  /** Run-time type information (and related methods). */
  itkTypeMacro(GeometriesRegion, itk::Region);
  //@}
  
  typedef enum {RANGE, SPATIAL} ModeType;

  /** Default constructor */
  GeometriesRegion();

  /** Copy constructor*/
  GeometriesRegion(const Self &region);

  /** Default destructor */
  ~GeometriesRegion() ITK_OVERRIDE;

  /** Return the region type : unstructured regions. */
  virtual typename Superclass::RegionType GetRegionType() const ITK_OVERRIDE
  { return Superclass::ITK_UNSTRUCTURED_REGION; }

  void SetMode(ModeType m);
  
  const ModeType & GetMode();

  bool IsRange() const;

  bool IsSpatial() const;

  const unsigned long & GetStartId(void) const;

  void SetStartId(const unsigned long & id);

  const unsigned long & GetCount() const;

  void SetCount(const unsigned long & count);

  const double & GetStartX() const;

  void SetStartX(const double& x);

  const double & GetStartY() const;

  void SetStartY(const double& y);

  const double & GetEndX() const;

  void SetEndX(const double& x);

  const double & GetEndY() const;

  void SetEndY(const double& y);

  double* GetStartPoint();

  double* GetEndPoint();

  /** Compare two regions. */
  bool operator==(const Self & region) const;

  /** Compare two regions. */
  bool operator!=(const Self & region) const;

  /** copy */
  void operator=(const Self & region);

protected:

  virtual void PrintSelf(std::ostream & os, itk::Indent indent) const ITK_OVERRIDE;
  
private:

  ModeType m_Mode;
  
  typedef union
    {
    struct {
      unsigned long Start;
      unsigned long Count;
      } Range;
    struct {
      double Start[2];
      double End[2];
      } Spatial;
    } VariantRegion;

  VariantRegion m_R;
};

OTBGdalAdapters_EXPORT
std::ostream & operator<<(std::ostream & os, const GeometriesRegion & region);

} // end of namespace otb

#endif
