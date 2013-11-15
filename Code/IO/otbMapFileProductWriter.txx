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
#ifndef __otbMapFileProductWriter_txx
#define __otbMapFileProductWriter_txx

#include "otbMapFileProductWriter.h"
#include "itksys/SystemTools.hxx"

namespace otb
{

/**
 * Constructor
 */
template <class TInputImage>
MapFileProductWriter<TInputImage>
::MapFileProductWriter(): m_TileSize(256), m_SRID(26918)
{
  m_GenericRSResampler = GenericRSResamplerType::New();

  // Modify superclass default values, can be overridden by subclasses
  this->SetNumberOfRequiredInputs(1);
}


/**
 * Desctructor
 */
template <class TInputImage>
MapFileProductWriter<TInputImage>
::~MapFileProductWriter()
{
}

/**
 *
 */
template <class TInputImage>
void
MapFileProductWriter<TInputImage>
::SetInput(const InputImageType *input)
{
  // Process object is not const-correct so the const_cast is required here
  this->ProcessObject::SetNthInput(0,
                                   const_cast< InputImageType * >( input ) );
}


template <class TInputImage>
void
MapFileProductWriter<TInputImage>
::SetInput( unsigned int index, const TInputImage * image )
{
  // Process object is not const-correct so the const_cast is required here
  this->ProcessObject::SetNthInput(index,
                                   const_cast< TInputImage *>( image ) );
}

/**
 *
 */
template <class TInputImage>
const typename MapFileProductWriter<TInputImage>::InputImageType *
MapFileProductWriter<TInputImage>
::GetInput(void)
{
  if (this->GetNumberOfInputs() < 1)
    {
    return 0;
    }

  return static_cast<const TInputImage * >
    (this->ProcessObject::GetInput(0) );
}

/**
 *  Get the idx input
 */
template <class TInputImage>
const typename MapFileProductWriter<TInputImage>::InputImageType *
MapFileProductWriter<TInputImage>
::GetInput(unsigned int idx)
{
  return static_cast< const TInputImage * >
    (this->ProcessObject::GetInput(idx));
}


/**
 * Write lauch the tiling and the mapFile generation and write on the
 * disk the indexfile as a shapefile
 */
template <class TInputImage>
void
MapFileProductWriter<TInputImage>
::Write()
{
  // Get the input Image
  m_VectorImage = const_cast<TInputImage *>(this->GetInput());
  m_VectorImage->UpdateOutputInformation();

  if(m_VectorImage->GetProjectionRef().empty()
     && m_VectorImage->GetImageKeywordlist().GetSize() > 0)
    {
    otbMsgDevMacro( "Sensor Model detected : Reprojecting in the targer SRID" );
    m_GenericRSResampler->SetInput(this->GetInput());
    m_GenericRSResampler->SetOutputParametersFromMap(otb::GeoInformationConversion::ToWKT(m_SRID));
    m_VectorImage = m_GenericRSResampler->GetOutput();
    m_VectorImage->UpdateOutputInformation();
    }

  // Initialize the filename, the vectordatas
  this->Initialize();

  // Generate the mapFile
  this->GenerateMapFile();

  // Do the tiling
  this->Tiling();
}

/**
 * Initialize the path, the filename, the vectordata used to store
 * each bounding box of a tile as a feature
 */
template <class TInputImage>
void
MapFileProductWriter<TInputImage>
::Initialize()
{
  // check that the right extension is given : expected .map
  if (itksys::SystemTools::GetFilenameLastExtension(m_FileName) != ".map")
    {
    itkExceptionMacro(<<itksys::SystemTools::GetFilenameLastExtension(m_FileName)
                      <<" is a wrong Extension FileName : Expected .map");
    }

  // Initialize the index vectordata
  this->InitializeVectorData();
}

/**
 * Initialize the vectordata to write
 *
 */
template <class TInputImage>
void
MapFileProductWriter<TInputImage>
::InitializeVectorData()
{
  // Intitialize the vectordata to build the indexTile
  m_VectorDataIndexTile  = VectorDataType::New();
  m_VectorDataIndexTile->SetProjectionRef(otb::GeoInformationConversion::ToWKT(m_SRID)/*m_VectorImage->GetProjectionRef()*/);
  DataNodeType::Pointer root = m_VectorDataIndexTile->GetDataTree()->GetRoot()->Get();
  DataNodeType::Pointer document = DataNodeType::New();
  m_Folder = DataNodeType::New();

  document->SetNodeType(otb::DOCUMENT);
  m_Folder->SetNodeType(otb::FOLDER);

  document->SetNodeId("DOCUMENT");
  m_Folder->SetNodeId("FOLDER");

  m_VectorDataIndexTile->GetDataTree()->Add(document, root);
  m_VectorDataIndexTile->GetDataTree()->Add(m_Folder, document);
}

/**
 *  Do the tiling
 */
template <class TInputImage>
void
MapFileProductWriter<TInputImage>
::Tiling()
{
  unsigned int numberOfChannel = m_VectorImage->GetNumberOfComponentsPerPixel();

  /** Image statistics*/
  typename InputImageType::PixelType inMin(numberOfChannel), inMax(numberOfChannel),
    outMin(numberOfChannel), outMax(numberOfChannel);
  outMin.Fill(0);
  outMax.Fill(255);

  // Update image base information
  m_VectorImage->UpdateOutputInformation();

  // Get the image size
  SizeType size;
  size = m_VectorImage->GetLargestPossibleRegion().GetSize();

  unsigned int sizeX = size[0];
  unsigned int sizeY = size[1];

  // Compute max depth
  unsigned int maxDepth =
    static_cast<unsigned int>(std::max(vcl_ceil(vcl_log(static_cast<float>(sizeX) / static_cast<float>(m_TileSize)) / vcl_log(2.0)),
                                  vcl_ceil(vcl_log(static_cast<float>(sizeY) / static_cast<float>(m_TileSize)) / vcl_log(2.0))));

  // Extract size & index
  SizeType  extractSize;
  IndexType extractIndex;

  for (unsigned int depth = 0; depth <= maxDepth; depth++)
    {
    // update the attribute value Current Depth
    m_CurrentDepth = depth;

    // Compose the name of the vectordata
    // the index shapefile filename
    std::ostringstream tempIndexShapeName;
    tempIndexShapeName << m_ShapeIndexPath<<"/";
    tempIndexShapeName << itksys::SystemTools::GetFilenameWithoutExtension(m_FileName)<<"_"<<m_CurrentDepth;
    tempIndexShapeName << "_index.shp";
    m_IndexShapeFileName = tempIndexShapeName.str();

    // Resample image to the max Depth
    int sampleRatioValue = 1 << (maxDepth - depth); // 2^(maxDepth - depth)

    if (sampleRatioValue > 1)
      {
      m_StreamingShrinkImageFilter = StreamingShrinkImageFilterType::New();

      m_StreamingShrinkImageFilter->SetShrinkFactor(sampleRatioValue);
      m_StreamingShrinkImageFilter->SetInput(m_VectorImage);
      m_StreamingShrinkImageFilter->GetStreamer()->SetAutomaticStrippedStreaming(0);
      m_StreamingShrinkImageFilter->Update();

      m_VectorRescaleIntensityImageFilter = VectorRescaleIntensityImageFilterType::New();
      m_VectorRescaleIntensityImageFilter->SetInput(m_StreamingShrinkImageFilter->GetOutput());
      m_VectorRescaleIntensityImageFilter->SetOutputMinimum(outMin);
      m_VectorRescaleIntensityImageFilter->SetOutputMaximum(outMax);

      if (depth == 0)
        {
        m_VectorRescaleIntensityImageFilter->Update();
        inMin = m_VectorRescaleIntensityImageFilter->GetInputMinimum();
        inMax = m_VectorRescaleIntensityImageFilter->GetInputMaximum();
        }
      else
        {
        m_VectorRescaleIntensityImageFilter->SetInputMinimum(inMin);
        m_VectorRescaleIntensityImageFilter->SetInputMaximum(inMax);
        m_VectorRescaleIntensityImageFilter->SetAutomaticInputMinMaxComputation(false);
        }

      // New resample vector image
      m_ResampleVectorImage = m_VectorRescaleIntensityImageFilter->GetOutput();
      }
    else
      {
      m_VectorRescaleIntensityImageFilter = VectorRescaleIntensityImageFilterType::New();
      m_VectorRescaleIntensityImageFilter->SetInput(m_VectorImage);
      m_VectorRescaleIntensityImageFilter->SetOutputMinimum(outMin);
      m_VectorRescaleIntensityImageFilter->SetOutputMaximum(outMax);

      m_VectorRescaleIntensityImageFilter->SetInputMinimum(inMin);
      m_VectorRescaleIntensityImageFilter->SetInputMaximum(inMax);
      m_VectorRescaleIntensityImageFilter->SetAutomaticInputMinMaxComputation(false);

      m_ResampleVectorImage = m_VectorRescaleIntensityImageFilter->GetOutput();
      }

    m_ResampleVectorImage->UpdateOutputInformation();

    // Get the image size
    size = m_ResampleVectorImage->GetLargestPossibleRegion().GetSize();

    sizeX = size[0];
    sizeY = size[1];

    unsigned int x = 0;
    unsigned int y = 0;

    // Create directory where to store generated tiles
    // Do it once here outside of the loop
    std::ostringstream path;
    path << m_ShapeIndexPath<<"/tiles";

    if (!itksys::SystemTools::MakeDirectory(path.str().c_str()))
      {
      itkExceptionMacro(<< "Error while creating cache directory" << path.str());
      }

    // Tiling resample image
    for (unsigned int tx = 0; tx < sizeX; tx += m_TileSize)
      {
      for (unsigned int ty = 0; ty < sizeY; ty += m_TileSize)
        {
        if ((tx + m_TileSize) >= sizeX)
          {
          extractIndex[0] = tx;
          extractSize[0] = sizeX - tx;
          }
        else
          {
          extractIndex[0] = tx;
          extractSize[0] = m_TileSize;
          }

        if ((ty + m_TileSize) >= sizeY)
          {
          extractIndex[1] = ty;
          extractSize[1] = sizeY - ty;
          }
        else
          {
          extractIndex[1] = ty;
          extractSize[1] = m_TileSize;
          }

        // Generate Tile filename
        std::ostringstream ossFileName;
        ossFileName << path.str()<<"/";
        ossFileName << "tile_"<<m_CurrentDepth<<"_";
        ossFileName << x<<"_"<<y<<".tif";

        // Extract ROI
        m_VectorImageExtractROIFilter = VectorImageExtractROIFilterType::New();

        // Set extract roi parameters
        m_VectorImageExtractROIFilter->SetStartX(extractIndex[0]);
        m_VectorImageExtractROIFilter->SetStartY(extractIndex[1]);
        m_VectorImageExtractROIFilter->SetSizeX(extractSize[0]);
        m_VectorImageExtractROIFilter->SetSizeY(extractSize[1]);

        // Set Channel to extract
        m_VectorImageExtractROIFilter->SetChannel(1);
        m_VectorImageExtractROIFilter->SetChannel(2);
        m_VectorImageExtractROIFilter->SetChannel(3);

        // Set extract roi input
        m_VectorImageExtractROIFilter->SetInput(m_ResampleVectorImage);

        // Configure writer
        m_VectorWriter = VectorWriterType::New();
        m_VectorWriter->SetFileName(ossFileName.str().c_str());
        m_VectorWriter->SetInput(m_VectorImageExtractROIFilter->GetOutput());
        m_VectorWriter->Update();

        /** TODO : Generate KML for this tile */
        // Search Lat/Lon box

        // Initialize the transform to be used
        //typename TransformType::Pointer transform =
        //TransformType::New();

        m_Transform  = TransformType::New();
        m_Transform->SetInputProjectionRef(m_GenericRSResampler->GetOutputProjectionRef());
        m_Transform->SetOutputProjectionRef(otb::GeoInformationConversion::ToWKT(m_SRID));
        m_Transform->InstanciateTransform();

        InputPointType  inputPoint;
        IndexType       indexTile;
        SizeType        sizeTile;
        sizeTile = extractSize;

        /** GX LAT LON **/
        // Compute lower left corner
        indexTile[0] = extractIndex[0];
        indexTile[1] = extractIndex[1] + sizeTile[1];
        m_ResampleVectorImage->TransformIndexToPhysicalPoint(indexTile, inputPoint);
        OutputPointType lowerLeftCorner = m_Transform->TransformPoint(inputPoint);
        //std::cout <<"indexTile "<< indexTile <<" --> input Point "<< inputPoint << " lowerLeftCorner "<<  lowerLeftCorner << std::endl;

        // Compute lower right corner
        indexTile[0] = extractIndex[0] + sizeTile[0];
        indexTile[1] = extractIndex[1] + sizeTile[1];
        m_ResampleVectorImage->TransformIndexToPhysicalPoint(indexTile, inputPoint);
        OutputPointType lowerRightCorner = m_Transform->TransformPoint(inputPoint);
        //std::cout <<"indexTile "<< indexTile <<" --> input Point "<< inputPoint << " lowerRightCorner   "<< lowerRightCorner  << std::endl;

        // Compute upper right corner
        indexTile[0] = extractIndex[0]+ sizeTile[0];
        indexTile[1] = extractIndex[1];
        m_ResampleVectorImage->TransformIndexToPhysicalPoint(indexTile, inputPoint);
        OutputPointType upperRightCorner = m_Transform->TransformPoint(inputPoint);
        //std::cout <<"indexTile "<< indexTile <<" --> input Point "<< inputPoint << " upperRightCorner "<< upperRightCorner  << std::endl;

        // Compute upper left corner
        indexTile[0] = extractIndex[0];
        indexTile[1] = extractIndex[1];
        m_ResampleVectorImage->TransformIndexToPhysicalPoint(indexTile, inputPoint);
        OutputPointType upperLeftCorner = m_Transform->TransformPoint(inputPoint);
        //std::cout <<"indexTile "<< indexTile <<" --> input Point "<< inputPoint << " upperLeftCorner "<< upperLeftCorner  << std::endl;

        // Build The indexTile
        this->AddBBoxToIndexTile(lowerLeftCorner,
                                 lowerRightCorner,
                                 upperRightCorner,
                                 upperLeftCorner, x, y);

        /** END GX LAT LON */
        y++;
        }
      x++;
      y = 0;
      }

    // Add the layer in the mapfile
    this->AddLayer();

    // Write the IndexTile
    typename VectorDataFileWriterType::Pointer writer = VectorDataFileWriterType::New();
    writer->SetFileName(m_IndexShapeFileName);
    writer->SetInput(m_VectorDataIndexTile);
    writer->Update();

    this->InitializeVectorData();
    }

  // Close the file
  m_File <<"END" << std::endl;
  m_File.close();
}

/**
 *  Add the bounding box and the location of the generated tile as
 *  field of the vectordata
 */
template <class TInputImage>
void
MapFileProductWriter<TInputImage>
::AddBBoxToIndexTile(OutputPointType lowerLeftCorner,
                     OutputPointType lowerRightCorner,
                     OutputPointType upperRightCorner,
                     OutputPointType upperLeftCorner,
                     unsigned int x, unsigned int y)
{
  // From PointType to VertexType
  VertexType pLL, pLR, pUR, pUL;

  pLL[0]=lowerLeftCorner[0];
  pLL[1]=lowerLeftCorner[1];

  pLR[0]=lowerRightCorner[0];
  pLR[1]=lowerRightCorner[1];

  pUR[0]=upperRightCorner[0];
  pUR[1]=upperRightCorner[1];

  pUL[0]=upperLeftCorner[0];
  pUL[1]=upperLeftCorner[1];

  // Form a polygon with the vertices
  PolygonType::Pointer poly = PolygonType::New();
  poly->AddVertex(pLL);
  poly->AddVertex(pLR);
  poly->AddVertex(pUR);
  poly->AddVertex(pUL);

  // Add the polygon to the datanode
  m_Polygon = DataNodeType::New();
  m_Polygon->SetNodeId("FEATURE_POLYGON");
  m_Polygon->SetNodeType(otb::FEATURE_POLYGON);
  m_Polygon->SetPolygonExteriorRing(poly);

  std::ostringstream oss;
  oss << "tiles/tile_";
  oss <<m_CurrentDepth <<"_"<< x <<"_"<< y <<".tif";

  // Add the field "LOCATION" used in mapserver clients
  // to get the path to tiles
  m_Polygon->SetFieldAsString("LOCATION", oss.str());

  // Add the to vectordata
  m_VectorDataIndexTile->GetDataTree()->Add(m_Polygon, m_Folder);
}

/**
 * Write the mapFile on the disk
 */
template <class TInputImage>
void
MapFileProductWriter<TInputImage>
::GenerateMapFile()
{
  // create the directory where to store the filemap
  // if it does not exists
  std::ostringstream path;
  path << itksys::SystemTools::GetFilenamePath(m_FileName);

  if (!itksys::SystemTools::MakeDirectory(path.str().c_str()))
    {
    itkExceptionMacro(<< "Error while creating cache directory" << path.str());
    }

  // Create a mapfile
  m_File.open(m_FileName.c_str());
  m_File << std::fixed << std::setprecision(6);

  // Get the name of the layer
  std::ostringstream tempIndexShapeName;
  tempIndexShapeName << itksys::SystemTools::GetFilenameWithoutExtension(m_FileName);

  m_File <<"MAP" << std::endl;
  m_File <<"\tNAME "<<tempIndexShapeName.str()<< std::endl;
  m_File <<"\t# Map image size" << std::endl;
  m_File <<"\tSIZE "<< m_TileSize <<" "<< m_TileSize << std::endl;
  m_File <<"\tUNITS dd" << std::endl;
  m_File <<"\tSHAPEPATH '"<<m_ShapeIndexPath<<"'"<<std::endl;
  m_File <<"\tEXTENT -180 -90 180 90" << std::endl;  //TODO : get the
                                                   //extent from the
                                                   //vector data
  m_File <<"\tPROJECTION" << std::endl;
  m_File <<"\t \"init=epsg:"<<m_SRID<<"\"" << std::endl;
  m_File <<"\tEND" << std::endl;

  m_File <<"\t# Background color for the map canvas -- change as desired" << std::endl;
  m_File <<"\tIMAGECOLOR 192 192 192" << std::endl;
  m_File <<"\tIMAGEQUALITY 95" << std::endl;
  m_File <<"\tIMAGETYPE PNG" << std::endl;
  m_File <<"\tOUTPUTFORMAT" << std::endl;
  m_File <<"\t\tNAME PNG" << std::endl;
  m_File <<"\t\tDRIVER 'GD/PNG'" << std::endl;
  m_File <<"\t\tMIMETYPE 'image/png'" << std::endl;
  m_File <<"\t\tIMAGEMODE RGB" << std::endl;
  m_File <<"\t\tFORMATOPTION INTERLACE=OFF" << std::endl;
  m_File <<"\t\tEXTENSION 'png'" << std::endl;
  m_File <<"\tEND" << std::endl;


  m_File <<"\t# Web interface definition. Only the template parameter" << std::endl;
  m_File <<"\t# is required to display a map. See MapServer documentation" << std::endl;
  m_File <<"\tWEB" << std::endl;
  m_File <<"\t\t# Set IMAGEPATH to the path where MapServer should" << std::endl;
  m_File <<"\t\t# write its output." << std::endl;
  //m_File <<"#IMAGEPATH \'D:\OSGeo4W_bis2/tmp/ms_tmp/\'" << std::endl;

  m_File <<"\t\t# Set IMAGEURL to the url that points to IMAGEPATH" << std::endl;
  m_File <<"\t\t# as defined in your web server configuration" << std::endl;
  m_File <<"\t\t#IMAGEURL '/ms_tmp/'" << std::endl;

  m_File <<"\t\t# WMS server settings" << std::endl;
  m_File <<"\t\t# NOTE : the user must change the path to the mapserver excecutable in the "<<std::endl;
  m_File <<"\t\t#  wms_onlineresource field"<<std::endl;
  m_File <<"\t\tMETADATA" << std::endl;
  m_File <<"\t\t 'wms_title'           'Level0'" << std::endl;
  m_File <<"\t\t \'wms_onlineresource\'  \'"<<m_CGIPath<<"?map="<<m_FileName<<"&\'" << std::endl;
  m_File <<"\t\t \'wms_srs\'             \'EPSG:"<<m_SRID<<" EPSG:900913\'" << std::endl;
  m_File <<"\t\tEND" << std::endl;
  m_File <<"\tEND" << std::endl;


}


template <class TInputImage>
void
MapFileProductWriter<TInputImage>
::AddLayer()
{
  m_File <<"\tLAYER" << std::endl;
  m_File <<"\t\tNAME '"<<itksys::SystemTools::GetFilenameWithoutExtension(m_IndexShapeFileName)/*tempIndexShapeName.str()*/<<"'"<< std::endl;
  m_File <<"\t\t\tOFFSITE  0 0 0"<< std::endl;
  m_File <<"\t\t\tTYPE RASTER" << std::endl;
  m_File <<"\t\t\tTILEITEM 'LOCATION'" << std::endl;
  m_File <<"\t\t\tTILEINDEX \'"<< itksys::SystemTools::GetFilenameName(m_IndexShapeFileName)<<"\'" << std::endl;
  //file <<"\t\t\tMETADATA" << std::endl;
  //file <<"\t\t\t 'wms_title' 'earthsat'" << std::endl;
  //file <<"\t\t\t 'wms_name' 'earthsat'" << std::endl;
  //file <<"\t\t\tEND" << std::endl;
  m_File <<"\t\t\tPROCESSING \"RESAMPLE=AVERAGE\"" << std::endl;
  m_File <<"\t\t\tSTATUS ON" << std::endl;
  m_File <<"\t\t\tTRANSPARENCY 100" << std::endl;
  m_File <<"\t\t\tPROJECTION" << std::endl;
  m_File <<"\t\t\t \"init=epsg:"<<m_SRID<<"\"" << std::endl;
  m_File <<"\t\t\tEND" << std::endl;
  m_File <<"\t\t#MINSCALE 250000" << std::endl; // TODO : problem with
                                                // the MINSCALE AND
                                                // MAXSCALE ..
  m_File <<"\tEND" << std::endl;
}

template <class TInputImage>
void
MapFileProductWriter<TInputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  // Call the superclass implementation
  Superclass::PrintSelf(os, indent);
}

}
#endif
