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

#ifndef otbImportGeoInformationImageFilter_h
#define otbImportGeoInformationImageFilter_h

#include "itkUnaryFunctorImageFilter.h"
#include "itkCastImageFilter.h"

namespace otb
{
/** \class ImportGeoInformationImageFilter
 *  \brief This filter is a helper class to import metadata
 *  from an existing image into a non-georeferenced image.
 *
 *  It derives from itk::CastImageFilter since it overwrites
 *  its output if possible. Default behavior is InPlaceOn as
 *  the other filters which only modify metadata information
 *  This class has been written as a workaround
 *  for the bug http://public.kitware.com/Bug/bug.php?op=show&bugid=4625&pos=0.
 *  There should be no needs for this filter in a standard pipeline.
 *
 * \sa InPlaceImageFilter
 * \sa ImageBase
 *
 * \ingroup OTBProjection
 */
template <class TImage, class TSourceImage>
class ITK_EXPORT ImportGeoInformationImageFilter : public itk::CastImageFilter<TImage, TImage>
{
public:
  /** Standard typedefs */
  typedef ImportGeoInformationImageFilter Self;
  typedef itk::CastImageFilter<TImage, TImage> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(ImportGeoInformationImageFilter, CastImageFilter);

  /** Template parameters typedefs */
  typedef TImage                           ImageType;
  typedef typename ImageType::Pointer      ImagePointerType;
  typedef typename ImageType::ConstPointer ImageConstPointerType;
  typedef TSourceImage                     SourceImageType;
  typedef SourceImageType                  SourceImagePointerType;

  /**
   * Set the source for geo information.
   * \param source The source image.
   */
  void SetSource(const TSourceImage* source);
  /**
   * Get the source for geo information.
   * \return The source image.
   */
  const TSourceImage* GetSource(void);

protected:
  /** Constructor */
  ImportGeoInformationImageFilter();
  /** Destructor */
  ~ImportGeoInformationImageFilter() override
  {
  }
  /**PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;
  /** Generate input requested region */
  void GenerateInputRequestedRegion(void) override;
  /** Generate output information */
  void GenerateOutputInformation(void) override;

  /** Override VerifyInputInformation() since this filter's inputs do
   * not need to occupy the same physical space.
   *
   * \sa ProcessObject::VerifyInputInformation
   */
  void VerifyInputInformation() override
  {
  }

private:
  ImportGeoInformationImageFilter(const Self&) = delete;
  void operator=(const Self&) = delete;
};
} // End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImportGeoInformationImageFilter.hxx"
#endif

#endif
