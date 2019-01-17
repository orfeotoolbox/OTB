/*
 * Copyright (C) 1999-2011 Insight Software Consortium
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

#ifndef otbPersistentImageToOGRLayerFilter_h
#define otbPersistentImageToOGRLayerFilter_h

#include "otbPersistentImageFilter.h"

#include "otbLabelImageToOGRDataSourceFilter.h"
#include "otbOGRLayerWrapper.h"

#include "itkMacro.h"
#include <string>

namespace otb
{

/** \class PersistentImageToOGRLayerFilter
 *  \brief Perform vectorization in a persistent way.
 *
 *  This filter is a generic filter. It is the base class of the Large scale segmentation framework.
 *  The \c ProcessTile() method is pure virtual and is implemented in sub class
 *  (@see \c PersistentStreamingLabelImageToOGRDataFilter). It returns a "memory" DataSource.
 *  This filter only copy each feature of the layer in the "memory" DataSource into the
 *  input \c OGRLayer set by \c SetOGRLayer() method.
 *
 * \sa PersistentImageFilter
 *
 *
 * \ingroup OTBConversion
 */
template<class TImage>
class ITK_EXPORT PersistentImageToOGRLayerFilter :
  public PersistentImageFilter<TImage, TImage>
{
public:
  /** Standard Self typedef */
  typedef PersistentImageToOGRLayerFilter                  Self;
  typedef PersistentImageFilter<TImage, TImage>           Superclass;
  typedef itk::SmartPointer<Self>                         Pointer;
  typedef itk::SmartPointer<const Self>                   ConstPointer;

  /** Runtime information support. */
  itkTypeMacro(PersistentImageToOGRLayerFilter, PersistentImageFilter);

  typedef TImage                                     InputImageType;
  typedef typename InputImageType::Pointer           InputImagePointer;
  typedef typename InputImageType::RegionType        RegionType;
  typedef typename InputImageType::SizeType          SizeType;
  typedef typename InputImageType::IndexType         IndexType;
  typedef typename InputImageType::PixelType         PixelType;
  typedef typename InputImageType::InternalPixelType InternalPixelType;

  typedef ogr::DataSource                            OGRDataSourceType;
  typedef typename OGRDataSourceType::Pointer        OGRDataSourcePointerType;
  typedef ogr::Layer                                 OGRLayerType;
  typedef ogr::Feature                               OGRFeatureType;

  void AllocateOutputs() override;
  void Reset(void) override;
  void Synthetize(void) override;

  /** This method creates the output layer in the OGRLayer set by the user.
   * \note This methode must be called before the call of Update .
   */
  virtual void Initialize(void);

  /** Get the size of the tile used for streaming.
   * This is useful if you use for example the \c OGRFusionTileFilter
   * for fusioning streaming tiles.
   */
  itkGetMacro(StreamSize, SizeType);

  /** Set the \c ogr::Layer in which the geometries will be dumped */
  void SetOGRLayer( const OGRLayerType & ogrLayer );
  /** Get the \c ogr::Layer output. */
  const OGRLayerType & GetOGRLayer( void ) const;

protected:
  PersistentImageToOGRLayerFilter();
  ~PersistentImageToOGRLayerFilter() override;

  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  void GenerateData() override;


private:
  PersistentImageToOGRLayerFilter(const Self &) = delete;
  void operator =(const Self&) = delete;

  virtual OGRDataSourcePointerType ProcessTile() = 0;

  // The layer where to dump geometries
  OGRLayerType m_OGRLayer;

  SizeType m_StreamSize;
}; // end of class
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPersistentImageToOGRLayerFilter.hxx"
#endif

#endif
