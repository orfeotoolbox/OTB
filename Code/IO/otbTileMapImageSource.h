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
#ifndef __otbTileMapImageSource_h
#define __otbTileMapImageSource_h

#include "itkImageSource.h"
#include "projection/ossimTileMapModel.h"

namespace otb
{
/** \class TileMapImageSource
 *
 *  \brief Base class for all the ImageIO object for reading and writing TileMap images
 *  produce an image.
 *
 * \ingroup IO
 */
template <class TOutputImage>
class ITK_EXPORT TileMapImageSource
  : public itk::ImageSource<TOutputImage>
{
public:
  /** Standard typedefs */
  typedef TileMapImageSource             Self;
  typedef itk::ImageSource<TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Typedef for the output image PixelType. */
  typedef typename TOutputImage::PixelType OutputImagePixelType;

  /** Typedef to describe the output image region type. */
  typedef typename TOutputImage::RegionType OutputImageRegionType;

  /** Spacing typedef support.  Spacing holds the size of a pixel.  The
   * spacing is the geometric distance between image samples. */
  typedef typename TOutputImage::SpacingType SpacingType;

  /** Origin typedef support.  The origin is the geometric coordinates
   * of the index (0,0). */
  typedef typename TOutputImage::PointType PointType;

  /** Direction typedef support.  The direction is the direction
   * cosines of the image. */
  typedef typename TOutputImage::DirectionType DirectionType;

  /** Size type matches that used for images */
  typedef typename TOutputImage::SizeType SizeType;

  /** Dimensionality of the output image */
  itkStaticConstMacro(NDimensions, unsigned int, TOutputImage::ImageDimension);

  /** Run-time type information (and related methods). */
  itkTypeMacro(TileMapImageSource, ImageSource);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Specify the size of the output image. */
  itkSetMacro(Size, SizeType);
  itkGetMacro(Size, SizeType);

  /** Set/Get the spacing of the output image. */
  itkSetMacro(Spacing, SpacingType);
  itkGetConstReferenceMacro(Spacing, SpacingType);

  /** Set/Get the origin of the output image. */
  itkSetMacro(Origin, PointType);
  itkGetConstReferenceMacro(Origin, PointType);

  /** Specify the direction of the output image. */
  itkSetMacro(Direction, DirectionType);
  itkGetConstReferenceMacro(Direction, DirectionType);

  /** Set/Get the server name */
  itkSetStringMacro(ServerName);
  itkGetStringMacro(ServerName);

  /** Set/Get the depth */
  itkSetClampMacro(Depth, unsigned int, m_DepthMin, m_DepthMax);
  itkGetMacro(Depth, unsigned int);

  /** Set/Get latitude */
  itkSetMacro(Latitude, double);
  itkGetMacro(Latitude, double);

  /** Set/Get longitude */
  itkSetMacro(Longitude, double);
  itkGetMacro(Longitude, double);

  /** Set/Get cache directory */
  itkSetStringMacro(CacheDirectory);
  itkGetStringMacro(CacheDirectory);

protected:
  /** Constructor */
  TileMapImageSource();
  /** Destructor */
  ~TileMapImageSource() {}
  /**PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;
  /** Generate Data */
  void GenerateData();
  /** GenerateOutputInformation */
  void GenerateOutputInformation();

private:
  /** Server name */
  std::string m_ServerName;

  /** Depth */
  unsigned int              m_Depth;
  static const unsigned int m_DepthMin = 1;
  static const unsigned int m_DepthMax = 18;

  /** Lat / Lon */
  double m_Latitude;
  double m_Longitude;

  /** Output image information */
  SizeType      m_Size;       // size
  SpacingType   m_Spacing;    // spacing
  PointType     m_Origin;     // origin
  DirectionType m_Direction;  // direction

  /** Tiles management */
  std::string m_CacheDirectory; // cache directory

  ossimRefPtr<ossimTileMapModel> m_TileMapModel;

};
} // End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbTileMapImageSource.txx"
#endif

#endif
