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
#ifndef __otbTileDimensionTiledStreamingManager_h
#define __otbTileDimensionTiledStreamingManager_h

#include "otbStreamingManager.h"

namespace otb
{

/** \class TileDimensionTiledStreamingManager
 *  \brief This class computes the divisions needed to stream an image in square tiles,
 *  driven by a user-defined tile dimension
 *
 * You can use SetTileDimension to ask for a specific tile size (number of pixels
 * for each dimension of the tile)
 *
 * The number of tiles will be computed to fit this requirement as close as possible.
 *
 * The tile dimension is always aligned on a multiple of 16 (as in TIFF spec)
 *
 * \sa ImageFileWriter
 * \sa StreamingImageVirtualFileWriter
 */
template<class TImage>
class ITK_EXPORT TileDimensionTiledStreamingManager : public StreamingManager<TImage>
{
public:
  /** Standard class typedefs. */
  typedef TileDimensionTiledStreamingManager Self;
  typedef StreamingManager<TImage>           Superclass;
  typedef itk::SmartPointer<Self>            Pointer;
  typedef itk::SmartPointer<const Self>      ConstPointer;

  typedef TImage                          ImageType;
  typedef typename Superclass::RegionType RegionType;

  /** Creation through object factory macro */
  itkNewMacro(Self);

  /** Type macro */
  itkTypeMacro(TileDimensionTiledStreamingManager, StreamingManager);

  /** Dimension of input image. */
  itkStaticConstMacro(ImageDimension, unsigned int, ImageType::ImageDimension);

  /** The desired tile dimension */
  itkSetMacro(TileDimension, unsigned int);

  /** The desired tile dimension */
  itkGetMacro(TileDimension, unsigned int);

  /** Actually computes the stream divisions, according to the specified streaming mode,
   * eventually using the input parameter to estimate memory consumption */
  virtual void PrepareStreaming(itk::DataObject * input, const RegionType &region);

protected:
  TileDimensionTiledStreamingManager();
  virtual ~TileDimensionTiledStreamingManager();

  /** The number of lines per strip desired by the user.
   *  This may be different than the one computed by the Splitter */
  unsigned int m_TileDimension;

private:
  TileDimensionTiledStreamingManager(const TileDimensionTiledStreamingManager &);
  void operator =(const TileDimensionTiledStreamingManager&);
};

} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbTileDimensionTiledStreamingManager.txx"
#endif

#endif

