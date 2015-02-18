/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbPersistentImageToOGRLayerFilter_h
#define __otbPersistentImageToOGRLayerFilter_h

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

  void AllocateOutputs();
  virtual void Reset(void);
  virtual void Synthetize(void);

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
  virtual ~PersistentImageToOGRLayerFilter();

  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  virtual void GenerateData();


private:
  PersistentImageToOGRLayerFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  virtual OGRDataSourcePointerType ProcessTile() = 0;

  // The layer where to dump geometries
  OGRLayerType m_OGRLayer;

  SizeType m_StreamSize;
}; // end of class
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPersistentImageToOGRLayerFilter.txx"
#endif

#endif
