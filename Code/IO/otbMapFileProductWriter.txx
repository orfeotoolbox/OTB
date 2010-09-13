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
::MapFileProductWriter():  m_UseExtendMode(true), m_TileSize(256)
{
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

  // Initialize the filename, the vectordatas
  this->Initialize();
  
  // Do the tiling
  this->Tiling();

  // Generate the mapFile
  this->GenerateMapFile();

  // Write the IndexTile
  typename VectorDataFileWriterType::Pointer writer = VectorDataFileWriterType::New();
  writer->SetFileName(m_IndexShapeFileName);
  writer->SetInput(m_VectorDataIndexTile);
  writer->Update();
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
    itkExceptionMacro(<<itksys::SystemTools::GetFilenameLastExtension(m_Path)
                      <<" is a wrong Extension FileName : Expected .map");
    }
  
  // the path to the directory where the mapfile will be written
  m_Path = itksys::SystemTools::GetFilenamePath(m_FileName);

  // the index shapefile filename
  std::ostringstream tempIndexShapeName;
  tempIndexShapeName << m_Path<<"/";
  tempIndexShapeName << itksys::SystemTools::GetFilenameWithoutExtension(m_FileName);
  tempIndexShapeName << "_index.shp";
  m_IndexShapeFileName = tempIndexShapeName.str();

  // Intitialize the vectordata to build the indexTile
  m_VectorDataIndexTile  = VectorDataType::New();
  m_VectorDataIndexTile->SetProjectionRef(m_VectorImage->GetProjectionRef());
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
  typename InputImageType::PixelType inMin(numberOfChannel), inMax(numberOfChannel), outMin(numberOfChannel), outMax(
    numberOfChannel);
  outMin.Fill(0);
  outMax.Fill(255);

  // Build wgs ref to compute long/lat
  OGRSpatialReference oSRS;
  oSRS.SetWellKnownGeogCS("WGS84");
  char * wgsRef = NULL;
  oSRS.exportToWkt(&wgsRef);

  // Update image base information
  m_VectorImage->UpdateOutputInformation();

  // Get the image size
  SizeType size;
  size = m_VectorImage->GetLargestPossibleRegion().GetSize();

  unsigned int sizeX = size[0];
  unsigned int sizeY = size[1];

  // Compute max depth
  unsigned int maxDepth =
    static_cast<unsigned int>(max(
                                vcl_ceil(vcl_log(static_cast<float>(sizeX) / static_cast<float>(m_TileSize)) / vcl_log(2.0)),
                                vcl_ceil(vcl_log(static_cast<float>(sizeY) / static_cast<float>(m_TileSize)) / vcl_log(2.0))
                                );
  
//   // Compute nbTile : Keep this for progress Bar ?
//   int nbTile = 0;

//   for (int i = 0; i <= maxDepth; i++)
//     {
//     int ratio = static_cast<int>(vcl_pow(2., (maxDepth - i)));
//     nbTile += (((sizeX / ratio) / m_TileSize) + 1)  * (((sizeY / ratio) / m_TileSize) + 1);
//     }

  // Extract size & index
  SizeType  extractSize;
  IndexType extractIndex;

  std::cout << "maxDepth " << maxDepth<< std::endl;
 
  for (unsigned int depth = 0; depth <= maxDepth; depth++)
    {

    // update the attribute value Current Depth
    m_CurrentDepth = depth;

    // Resample image to the max Depth
//    int sampleRatioValue = static_cast<int>(vcl_pow(2., (maxDepth - depth)));
    int sampleRatioValue = 1 << (maxDepth - depth); // 2^(maxDepth - depth)

    if (sampleRatioValue > 1)
      {
      m_StreamingShrinkImageFilter = StreamingShrinkImageFilterType::New();

      m_StreamingShrinkImageFilter->SetShrinkFactor(sampleRatioValue);
      m_StreamingShrinkImageFilter->SetInput(m_VectorImage);

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

        // Create directory where to store generated tiles
        std::ostringstream path;
        path << m_Path<<"/tiles";
        
        ossimFilename cachingDir(path.str());
        cachingDir.createDirectory();
        
        // Generate Tile filename
        std::ostringstream ossFileName;
        ossFileName << m_Path<<"/";
        ossFileName << "tiles/tile_";
        ossFileName << m_CurrentDepth<<"_";
        ossFileName << x<<"_";
        ossFileName << y;
        ossFileName << ".tif";

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
        m_VectorWriter->WriteGeomFileOn();
        m_VectorWriter->Update();

        /** TODO : Generate KML for this tile */
        // Search Lat/Lon box
        m_Transform = TransformType::New();
        m_Transform->SetInputKeywordList(m_ResampleVectorImage->GetImageKeywordlist());
        m_Transform->SetInputProjectionRef(m_VectorImage->GetProjectionRef());
        m_Transform->SetOutputProjectionRef(wgsRef);
        m_Transform->InstanciateTransform();

        InputPointType  inputPoint;
        OutputPointType outputPoint;
        IndexType       indexTile;
        SizeType        sizeTile;
        
        sizeTile = extractSize;
 
        /** GX LAT LON **/
        // Compute lower left corner
        indexTile[0] = extractIndex[0];
        indexTile[1] = extractIndex[1] + sizeTile[1];
        m_ResampleVectorImage->TransformIndexToPhysicalPoint(indexTile, inputPoint);
        outputPoint = m_Transform->TransformPoint(inputPoint);
        OutputPointType lowerLeftCorner = outputPoint;
        
        // Compute lower right corner
        indexTile[0] = extractIndex[0] + sizeTile[0];
        indexTile[1] = extractIndex[1] + sizeTile[1];
        m_ResampleVectorImage->TransformIndexToPhysicalPoint(indexTile, inputPoint);
        outputPoint = m_Transform->TransformPoint(inputPoint);
        OutputPointType lowerRightCorner = outputPoint;

        // Compute upper right corner
        indexTile[0] = extractIndex[0] + sizeTile[0];
        indexTile[1] = extractIndex[1];
        m_ResampleVectorImage->TransformIndexToPhysicalPoint(indexTile, inputPoint);
        outputPoint = m_Transform->TransformPoint(inputPoint);
        OutputPointType upperRightCorner = outputPoint;

        // Compute upper left corner
        indexTile[0] = extractIndex[0];
        indexTile[1] = extractIndex[1];
        m_ResampleVectorImage->TransformIndexToPhysicalPoint(indexTile, inputPoint);
        outputPoint = m_Transform->TransformPoint(inputPoint);
        OutputPointType upperLeftCorner = outputPoint;
	
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
    }
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
  VertexType pLL,pLR,pUR,pUL;

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
  
  itk::OStringStream oss;
  oss << "tiles/tile_";
  oss <<m_CurrentDepth <<"_"<< x <<"_"<< y <<".tif";
  
  // Add the field "LOCATION" used in mapserver clients 
  // to get the path to tiles
  m_Polygon->SetFieldAsString("LOCATION",oss.str());

  // Add the to vectordata
  m_VectorDataIndexTile->GetDataTree()->Add(m_Polygon,m_Folder);
}

/**
 * Write the mapFile on the disk
 */
template <class TInputImage>
void
MapFileProductWriter<TInputImage>
::GenerateMapFile()
{
  std::ofstream file(m_FileName.c_str());
  file << fixed << setprecision(6);
  
  file <<"MAP" << std::endl;
  file <<"\tNAME Level0" << std::endl;
  file <<"\t# Map image size" << std::endl;
  file <<"\tSIZE "<< m_TileSize <<" "<< m_TileSize << std::endl;
  file <<"\tUNITS dd" << std::endl<<std::endl;
  file <<"\tEXTENT -180 -90 180 90" << std::endl;
  file <<"\tPROJECTION" << std::endl;
  file <<"\t \"init=epsg:4326\"" << std::endl;
  file <<"\tEND" << std::endl;

  file <<"\t# Background color for the map canvas -- change as desired" << std::endl;
  file <<"\tIMAGECOLOR 192 192 192" << std::endl;
  file <<"\tIMAGEQUALITY 95" << std::endl;
  file <<"\tIMAGETYPE PNG" << std::endl;
  file <<"\tOUTPUTFORMAT" << std::endl;
  file <<"\t\tNAME PNG" << std::endl;
  file <<"\t\tDRIVER 'GD/PNG'" << std::endl;
  file <<"\t\tMIMETYPE 'image/png'" << std::endl;
  file <<"\t\tIMAGEMODE RGB" << std::endl;
  file <<"\t\tFORMATOPTION INTERLACE=OFF" << std::endl;
  file <<"\t\tEXTENSION 'png'" << std::endl;
  file <<"\tEND" << std::endl;


  file <<"\t# Web interface definition. Only the template parameter" << std::endl;
  file <<"\t# is required to display a map. See MapServer documentation" << std::endl;
  file <<"\tWEB" << std::endl;
  file <<"\t\t# Set IMAGEPATH to the path where MapServer should" << std::endl;
  file <<"\t\t# write its output." << std::endl;
  //file <<"#IMAGEPATH \'D:\OSGeo4W_bis2/tmp/ms_tmp/\'" << std::endl;

  file <<"\t\t# Set IMAGEURL to the url that points to IMAGEPATH" << std::endl;
  file <<"\t\t# as defined in your web server configuration" << std::endl;
  file <<"\t\t#IMAGEURL '/ms_tmp/'" << std::endl;

  file <<"\t\t# WMS server settings" << std::endl;
  file <<"\t\t# NOTE : the user must change the path to the mapserver excecutable in the "<<std::endl;
  file <<"\t\t  wms_onlineresource field"<<std::endl;
  file <<"\t\tMETADATA" << std::endl;
  file <<"\t\t 'wms_title'           'Level0'" << std::endl;
  file <<"\t\t \'wms_onlineresource\'  \'http://127.0.0.1/cgi-bin/mapserv.exe?map="<<m_FileName<<"&\'" << std::endl;
  file <<"\t\t \'wms_srs\'             \'EPSG:4326\'" << std::endl;
  file <<"\t\tEND" << std::endl;
  file <<"\tEND" << std::endl;
  
  // Get the name of the layer
  std::ostringstream tempIndexShapeName;
  tempIndexShapeName << itksys::SystemTools::GetFilenameWithoutExtension(m_FileName);

  file <<"\tLAYER" << std::endl;
  file <<"\t\tNAME '"<<tempIndexShapeName.str()<<"'"<< std::endl;
  file <<"\t\t\t#GROUP 'earthsat'"<< std::endl;	
  file <<"\t\t\tTYPE RASTER" << std::endl;
  file <<"\t\t\tTILEITEM 'LOCATION'" << std::endl;
  file <<"\t\t\tTILEINDEX \'"<<m_IndexShapeFileName<<"\'" << std::endl;
  file <<"\t\t\tMETADATA" << std::endl;
  file <<"\t\t\t 'wms_title' 'earthsat'" << std::endl;
  file <<"\t\t\t 'wms_name' 'earthsat'" << std::endl;
  file <<"\t\t\tEND" << std::endl;
  file <<"\t\t\tPROCESSING \"RESAMPLE=AVERAGE\"" << std::endl;
  file <<"\t\t\tSTATUS OFF" << std::endl;
  file <<"\t\t\tTRANSPARENCY 100" << std::endl;
  file <<"\t\t\tPROJECTION" << std::endl;
  file <<"\t\t\t \"init=epsg:4326\"" << std::endl;
  file <<"\t\t\tEND" << std::endl;
  file <<"\t\t#MINSCALE 250000" << std::endl;
  file <<"\tEND" << std::endl;
  file <<"END" << std::endl;

  file.close();
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
