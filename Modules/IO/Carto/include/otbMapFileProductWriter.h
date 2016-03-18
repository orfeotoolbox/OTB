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
#ifndef __otbMapFileProductWriter_h
#define __otbMapFileProductWriter_h

#include <fstream>

#include "itkObjectFactory.h"

// Image Tiling
#include "otbMultiChannelExtractROI.h"
#include "otbImageFileWriter.h"
#include "otbVectorRescaleIntensityImageFilter.h"
#include "otbGeoInformationConversion.h"
#include "otbStreamingShrinkImageFilter.h"

// sahpe index necessary includes
#include "otbVectorData.h"
#include "otbVectorDataFileWriter.h"

// projection filter
#include "otbGenericRSResampleImageFilter.h"

namespace otb
{

/** \class MapFileProductWriter
 * \brief This class produces Map file product ie a file .map,
 *        the tiles to draw in a mapserver, and finally
 *        a shapefile wich describe the tiles and where to find them
 *        on the disk.
 *
 * This filter begins by tiling the input image. An accessor
 * SetTileSize allows setting the size of each tile. For each tile
 * generated, an entry is added to the shapefile to store the location
 * where the file is saved on the disk.
 * The product generated are a mapfile wich is the configuration file
 * for mapservers, a tile index and finally the tiles.
 * This class allow the user to specify the cgi-bin used (SetCGIPath)
 * and the directory where to store the index shapefile and the tiles
 * via SetShapeIndexPath method
 *
 * Note that this class reproject the input image in the projection
 * defined by the SRID (via SetSRID() method) set by the user.
 *
 *
 * \ingroup IO
 *
 *
 * \ingroup OTBCarto
 */
template <class TInputImage>
class ITK_EXPORT MapFileProductWriter : public itk::ProcessObject
{
public:
  /** Standard class typedefs. */
  typedef MapFileProductWriter              Self;
  typedef itk::ProcessObject                Superclass;
  typedef itk::SmartPointer<Self>           Pointer;
  typedef itk::SmartPointer<const Self>     ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(MapFileProductWriter, itk::ProcessObject);

  /** Some convenient typedefs. */
  typedef TInputImage                                  InputImageType;
  typedef typename InputImageType::InternalPixelType   InternalPixelType;
  typedef typename InputImageType::SizeType            SizeType;
  typedef typename InputImageType::IndexType           IndexType;
  typedef typename InputImageType::Pointer             InputImagePointer;
  typedef typename InputImageType::RegionType          InputImageRegionType;
  typedef typename InputImageType::PixelType           InputImagePixelType;

  typedef unsigned char                                OutputPixelType;
  typedef VectorImage<OutputPixelType>                 OutputImageType;

  typedef VectorData<double, 2>                         VectorDataType;
  typedef typename VectorDataType::DataNodeType        DataNodeType;
  typedef typename DataNodeType::PolygonType           PolygonType;
  typedef typename PolygonType::VertexType             VertexType;

  typedef VectorDataFileWriter<VectorDataType>         VectorDataFileWriterType;


  /// Multi channels Extract ROI filter
  typedef MultiChannelExtractROI
  <InternalPixelType, OutputPixelType>                 VectorImageExtractROIFilterType;

  // Writer
  typedef ImageFileWriter<OutputImageType>            VectorWriterType;

  // Resampler
  typedef StreamingShrinkImageFilter
  <InputImageType, InputImageType >        StreamingShrinkImageFilterType;

  // Intensity Rescale
  typedef VectorRescaleIntensityImageFilter
  <InputImageType, InputImageType>          VectorRescaleIntensityImageFilterType;

  // Project filter
  typedef GenericRSResampleImageFilter<InputImageType, InputImageType>  GenericRSResamplerType;
  typedef typename GenericRSResamplerType::Pointer                   GenericRSResamplerPointerType;

  // Transformer
  typedef GenericRSTransform<>           TransformType;
  typedef TransformType::InputPointType  InputPointType;
  typedef TransformType::OutputPointType OutputPointType;

  /** Dimension of input image. */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      InputImageType::ImageDimension);

  using Superclass::SetInput;
  virtual void SetInput( const InputImageType *image);
  virtual void SetInput( unsigned int, const TInputImage * image);
  const InputImageType * GetInput(void);
  const InputImageType * GetInput(unsigned int idx);

  /** Method to set the filename of the mapfile generated */
  itkSetStringMacro(FileName);
  itkGetStringMacro(FileName);

  /** Set/Get the size of each tile*/
  itkSetMacro(TileSize, unsigned int);
  itkGetMacro(TileSize, unsigned int);

  /** Set CGI filename path */
  itkSetStringMacro(CGIPath);
  itkGetStringMacro(CGIPath);

  /** Set SahpePATH: the directory where to store the shapefile */
  itkSetStringMacro(ShapeIndexPath);
  itkGetStringMacro(ShapeIndexPath);

  /** Update Method : Call a porotected Write method */
  virtual void Update()
  {
    this->Write();
  }

  /** Macro to set the SRID we want to project Data in*/
  itkSetMacro(SRID, int);

protected:
  MapFileProductWriter();
  virtual ~MapFileProductWriter();
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /**Method for Tiling the input image*/
  virtual void Tiling();

  /** Method for writing */
  virtual void Write();

private:
  MapFileProductWriter(const Self &); //purposely not implemented
  void operator =(const Self&);  //purposely not implemented

   /** Mehtod to initialize the variables*/
   virtual void Initialize();

  /**MapFile  IndexTule build*/
  virtual void AddBBoxToIndexTile(OutputPointType lowerLeftCorner,
          OutputPointType lowerRightCorner,
          OutputPointType upperRightCorner,
          OutputPointType upperLeftCorner,
                                  unsigned int x, unsigned int y);

  /** Method To Generate the mapFile*/
  virtual void GenerateMapFile();

  /** Add a layer to the mapfile*/
  void AddLayer();

  /** Intialize vd*/
  void InitializeVectorData();

  InputImagePointer    m_VectorImage;
  InputImagePointer    m_ResampleVectorImage;

    // Extract ROI
  typename VectorImageExtractROIFilterType::Pointer m_VectorImageExtractROIFilter;

  // Writer
  typename VectorWriterType::Pointer                m_VectorWriter;

  // Resampler
  typename StreamingShrinkImageFilterType::Pointer  m_StreamingShrinkImageFilter;

  // Rescale intensity
  typename VectorRescaleIntensityImageFilterType::Pointer m_VectorRescaleIntensityImageFilter;

  // Transformer
  typename TransformType::Pointer                  m_Transform;

  // VectorData Pointer Type for tileindex shapefile
  typename VectorDataType::Pointer                 m_VectorDataIndexTile;
  typename DataNodeType::Pointer                   m_Polygon;
  typename DataNodeType::Pointer                   m_Folder;

  // Projection Filter
  GenericRSResamplerPointerType                    m_GenericRSResampler;

  // Tile size
  unsigned int                 m_TileSize;
  unsigned int                 m_CurrentDepth;

  // File and path name
  std::string                  m_FileName;
  std::string                  m_IndexShapeFileName;
  std::string                  m_CGIPath;
  std::string                  m_ShapeIndexPath;

  std::ofstream                m_File;
  int                          m_SRID;

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMapFileProductWriter.txx"
#endif

#endif
