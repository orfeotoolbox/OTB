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
#ifndef __otbKmzProductWriter_txx
#define __otbKmzProductWriter_txx

#include <string>

#include "otbKmzProductWriter.h"
#include "itksys/SystemTools.hxx"

#include "otbMetaDataKey.h"
#include "otbVectorDataKeywordlist.h"

namespace otb
{
template <class TInputImage>
KmzProductWriter<TInputImage>
::KmzProductWriter():
  m_UseExtendMode(true),
  m_TileSize(512),
  m_MaxDepth(0),
  m_CurrentDepth(0),
  m_CurIdx(0)
{
  // Modify superclass default values, can be overridden by subclasses
  this->SetNumberOfRequiredInputs(1);
}

template <class TInputImage>
KmzProductWriter<TInputImage>
::~KmzProductWriter()
{
  // Reset the boost::intrusive_ptr<KmzFile> :
  // TODO : when upgrading boost > 1.42 use method release().
  m_KmzFile = NULL;
}

/**
 *
 */
template <class TInputImage>
void
KmzProductWriter<TInputImage>
::SetInput(const InputImageType *input)
{
  // Process object is not const-correct so the const_cast is required here
  this->ProcessObject::SetNthInput(0,
                                   const_cast< InputImageType * >( input ) );
}


template <class TInputImage>
void
KmzProductWriter<TInputImage>
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
const typename KmzProductWriter<TInputImage>::InputImageType *
KmzProductWriter<TInputImage>
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
 *
 */
template <class TInputImage>
const typename KmzProductWriter<TInputImage>::InputImageType *
KmzProductWriter<TInputImage>
::GetInput(unsigned int idx)
{
  return static_cast< const TInputImage * >
    (this->ProcessObject::GetInput(idx));
}

template <class TInputImage>
void
KmzProductWriter<TInputImage>
::Write()
{
  m_VectorImage = const_cast<TInputImage *>(this->GetInput());

  // Do some checks, If no metadata nor projection ref available,
  // input is not usable.
  bool emptyProjRef = m_VectorImage->GetProjectionRef().empty();
  bool emptyKWL     = m_VectorImage->GetImageKeywordlist().GetSize() == 0 ? true : false;

  if(emptyProjRef &&  emptyKWL)
    {
    itkExceptionMacro(<<"The input image have empty keyword list, please use an image with metadata information");
    }

  // Continue processing
  this->Initialize();
  this->AddLogo();
  this->Tiling();
}


template <class TInputImage>
void
KmzProductWriter<TInputImage>
::Initialize()
{

  // Check if the path is not empty
  if (m_Path.empty())
    {
    itkExceptionMacro(<<" Output path is empty !");
    }

  // Check that the right extension is given : expected .kmz */
  if (itksys::SystemTools::GetFilenameLastExtension(m_Path) != ".kmz")
    {
    itkExceptionMacro(<<itksys::SystemTools::GetFilenameLastExtension(m_Path)
                      <<" is a wrong Extension FileName : Expected .kmz");
    }

  // Decompose the filename, the full path
  m_FileName = itksys::SystemTools::GetFilenameWithoutExtension(m_Path);
  m_Path = itksys::SystemTools::GetFilenamePath(m_Path);
  m_CurrentImageName = this->GetCuttenFileName(m_FileName, 0);

  // Create the extension following the user choice
  if (m_UseExtendMode)
    {
    m_KmzExtension = "xt.kmz";
    m_KmlExtension = "xt.kml";
    }
  else
    {
    m_KmzExtension = ".kmz";
    m_KmlExtension = ".kml";
    }

  // Create a kmz file
  m_KmzFileName << m_Path << "/" << m_FileName << m_KmzExtension;
  m_KmzFile = kmlengine::KmzFile::Create(m_KmzFileName.str().c_str());
}

 /**
  * Add Logo if any
  *
  */
template <class TInputImage>
void
KmzProductWriter<TInputImage>
::AddLogo()
{
  // Logo
  if(m_Logo.IsNull()) return;

  std::ostringstream     logoFilename;
  logoFilename << m_Path;
  logoFilename << "/logo.jpeg";

  if (!itksys::SystemTools::MakeDirectory(m_Path.c_str()))
    {
    itkExceptionMacro(<< "Error while creating cache directory" << m_Path);
    }

  typename CastFilterType::Pointer castFiler = CastFilterType::New();
  castFiler->SetInput(m_Logo);

  m_VectorWriter = VectorWriterType::New();
  m_VectorWriter->SetFileName(logoFilename.str());
  m_VectorWriter->SetInput(castFiler->GetOutput());
  m_VectorWriter->Update();

  // Add the logo to the kmz
  std::ostringstream logo_root_path_in_kmz;
  logo_root_path_in_kmz << "logo.jpeg";

  std::ostringstream logo_absolut_path;
  logo_absolut_path << logoFilename.str();

  this->AddFileToKMZ(logo_absolut_path, logo_root_path_in_kmz);

  // Remove the logo file with stdio method :remove
  if (remove(logo_absolut_path.str().c_str()) != 0)
    {
    itkExceptionMacro(<< "Error while deleting the file" << logo_absolut_path.str());
    }
}

/**
  * Add legend if any
  */
template <class TInputImage>
void
KmzProductWriter<TInputImage>
::ProcessLegends()
{
  for (unsigned int idx = 0; idx < m_LegendVector.size(); ++idx)
    {
    std::ostringstream legendName;
    legendName << m_Path;
    legendName << "/legend_" << idx <<".jpeg";

    InputImagePointer legend = m_LegendVector[idx].second;
    typename CastFilterType::Pointer castFiler = CastFilterType::New();
    castFiler->SetInput(legend);

    m_VectorWriter = VectorWriterType::New();
    m_VectorWriter->SetFileName(legendName.str().c_str());
    m_VectorWriter->SetInput(castFiler->GetOutput());
    m_VectorWriter->Update();

    // Add the legend to the kmz
    std::ostringstream legend_root_path_in_kmz;
    legend_root_path_in_kmz << "legends/legend_" << idx << ".jpeg";

    std::ostringstream legend_absolut_path;
    legend_absolut_path << legendName.str();

    this->AddFileToKMZ(legend_absolut_path, legend_root_path_in_kmz);

    // Remove the legend file with stdio method :remove
    if (remove(legend_absolut_path.str().c_str()) != 0)
      {
      itkExceptionMacro(<< "Error while deleting the file" << legend_absolut_path.str());
      }
    }
}

template <class TInputImage>
void
KmzProductWriter<TInputImage>
::Tiling()
{
  unsigned int numberOfChannel = m_VectorImage->GetNumberOfComponentsPerPixel();
  unsigned int curIdx = 0;

  /** Image statistics*/
  typename InputImageType::PixelType inMin(numberOfChannel), inMax(numberOfChannel),
    outMin(numberOfChannel), outMax(numberOfChannel);
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
    static_cast<unsigned int>(std::max(vcl_ceil(vcl_log(static_cast<float>(sizeX) / static_cast<float>(m_TileSize)) / vcl_log(2.0)),
                              vcl_ceil(vcl_log(static_cast<float>(sizeY) / static_cast<float>(m_TileSize)) / vcl_log(2.0))));
  m_MaxDepth = maxDepth;
  m_CurIdx = 0;

  // Compute nbTile
  int nbTile = 0;

  for (unsigned int i = 0; i <= maxDepth; ++i)
    {
    unsigned int ratio = static_cast<unsigned int>(vcl_pow(2.,static_cast<int>((static_cast<int>(maxDepth) - i))));
    nbTile += (((sizeX / ratio) / m_TileSize) + 1)  * (((sizeY / ratio) / m_TileSize) + 1);
    }

  // Extract size & index
  SizeType  extractSize;
  IndexType extractIndex;

  for (unsigned int depth = 0; depth <= maxDepth; depth++)
    {
    // update the attribute value Current Depth
    m_CurrentDepth = depth;

    // Resample image to the max Depth
    int sampleRatioValue = (1 << (maxDepth - depth));

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

      // Case of input image with size lower than the tileSize
      // this case is illustrated with depth==0 and
      // sampleRatioValue==1
      if(depth==0 && sampleRatioValue == 1)
        {
        m_VectorRescaleIntensityImageFilter->SetAutomaticInputMinMaxComputation(true);
        }
      else
        {
        m_VectorRescaleIntensityImageFilter->SetInputMinimum(inMin);
        m_VectorRescaleIntensityImageFilter->SetInputMaximum(inMax);
        m_VectorRescaleIntensityImageFilter->SetAutomaticInputMinMaxComputation(false);
        }
      m_ResampleVectorImage = m_VectorRescaleIntensityImageFilter->GetOutput();
      }

    m_ResampleVectorImage->UpdateOutputInformation();

    // Get the image size
    size = m_ResampleVectorImage->GetLargestPossibleRegion().GetSize();

    sizeX = size[0];
    sizeY = size[1];

    // Tiling resample image
    for (unsigned int tx = 0, x = 0; tx < sizeX; tx += m_TileSize, ++x)
      {
      for (unsigned int ty = 0, y = 0; ty < sizeY; ty += m_TileSize, ++y)
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

        // Generate pathname
        if (!itksys::SystemTools::MakeDirectory(m_Path.c_str()))
          {
          itkExceptionMacro(<< "Error while creating cache directory" << m_Path);
          }
        std::ostringstream path;
        path << m_Path;

        // Generate Tile filename
        std::ostringstream ossFileName;
        ossFileName << m_Path;
        ossFileName << "/";
        ossFileName << y;
        ossFileName << ".jpg";

        // Extract ROI
        m_VectorImageExtractROIFilter = VectorImageExtractROIFilterType::New();

        // Set extract roi parameters
        m_VectorImageExtractROIFilter->SetStartX(extractIndex[0]);
        m_VectorImageExtractROIFilter->SetStartY(extractIndex[1]);
        m_VectorImageExtractROIFilter->SetSizeX(extractSize[0]);
        m_VectorImageExtractROIFilter->SetSizeY(extractSize[1]);

        // Set Channel to extract
        if(m_VectorImage->GetNumberOfComponentsPerPixel()>3)
          {
          m_VectorImageExtractROIFilter->SetChannel(1); //m_ProductVector[m_CurrentProduct].m_Composition[0] + 1);
          m_VectorImageExtractROIFilter->SetChannel(2); //m_ProductVector[m_CurrentProduct].m_Composition[1] + 1);
          m_VectorImageExtractROIFilter->SetChannel(3); //m_ProductVector[m_CurrentProduct].m_Composition[2] + 1);
          }

        // Set extract roi input
        m_VectorImageExtractROIFilter->SetInput(m_ResampleVectorImage);

        // Configure writer
        m_VectorWriter = VectorWriterType::New();
        m_VectorWriter->SetFileName(ossFileName.str().c_str());
        m_VectorWriter->SetInput(m_VectorImageExtractROIFilter->GetOutput());
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
        double          sizeTile[2];
        double          halfSizeTile[2];

        sizeTile[0] = static_cast<double>(extractSize[0]);
        sizeTile[1] = static_cast<double>(extractSize[1]);
        halfSizeTile[0] = sizeTile[0] / 2.0;
        halfSizeTile[1] = sizeTile[1] / 2.0;

        itk::ContinuousIndex<double, 2> indexRef(extractIndex);
        indexRef[0] += -0.5;
        indexRef[1] += -0.5;
        itk::ContinuousIndex<double, 2> indexTile(indexRef);

        // Compute North value
        indexTile[0] += halfSizeTile[0];
        m_ResampleVectorImage->TransformContinuousIndexToPhysicalPoint(indexTile, inputPoint);
        outputPoint = m_Transform->TransformPoint(inputPoint);
        double north = outputPoint[1];

        // Compute South value
        indexTile[1] += sizeTile[1];
        m_ResampleVectorImage->TransformContinuousIndexToPhysicalPoint(indexTile, inputPoint);
        outputPoint = m_Transform->TransformPoint(inputPoint);
        double south = outputPoint[1];

        // Compute West value
        indexTile = indexRef;
        indexTile[1] += halfSizeTile[1];
        m_ResampleVectorImage->TransformContinuousIndexToPhysicalPoint(indexTile, inputPoint);
        outputPoint = m_Transform->TransformPoint(inputPoint);
        double west = outputPoint[0];

        // Compute East value
        indexTile[0] += sizeTile[0];
        m_ResampleVectorImage->TransformContinuousIndexToPhysicalPoint(indexTile, inputPoint);
        outputPoint = m_Transform->TransformPoint(inputPoint);
        double east = outputPoint[0];

        // Compute center value (lat / long)
        indexTile = indexRef;
        indexTile[0] += halfSizeTile[0];
        indexTile[1] += halfSizeTile[1];
        m_ResampleVectorImage->TransformContinuousIndexToPhysicalPoint(indexTile, inputPoint);
        outputPoint = m_Transform->TransformPoint(inputPoint);
        double centerLat = outputPoint[1];
        double centerLong = outputPoint[0];

        /** GX LAT LON **/
        // Compute upper left corner
        indexTile = indexRef;
        m_ResampleVectorImage->TransformContinuousIndexToPhysicalPoint(indexTile, inputPoint);
        outputPoint = m_Transform->TransformPoint(inputPoint);
        OutputPointType upperLeftCorner = outputPoint;

        // Compute lower left corner
        indexTile[1] += sizeTile[1];
        m_ResampleVectorImage->TransformContinuousIndexToPhysicalPoint(indexTile, inputPoint);
        outputPoint = m_Transform->TransformPoint(inputPoint);
        OutputPointType lowerLeftCorner = outputPoint;

        // Compute lower right corner
        indexTile[0] += sizeTile[0];
        m_ResampleVectorImage->TransformContinuousIndexToPhysicalPoint(indexTile, inputPoint);
        outputPoint = m_Transform->TransformPoint(inputPoint);
        OutputPointType lowerRightCorner = outputPoint;

        // Compute upper right corner
        indexTile[1] -= sizeTile[1];
        m_ResampleVectorImage->TransformContinuousIndexToPhysicalPoint(indexTile, inputPoint);
        outputPoint = m_Transform->TransformPoint(inputPoint);
        OutputPointType upperRightCorner = outputPoint;
        /** END GX LAT LON */

        // Create KML - Filename - PathName - tile number - North - South - East - West
        if (sampleRatioValue == 1)
          {
          if (!m_UseExtendMode) // Extended format
            {
            this->GenerateKML(path.str(), depth, x, y, north, south, east, west);
            }
          else
            {
            this->GenerateKMLExtended(path.str(), depth,
                                      x, y, lowerLeftCorner,
                                      lowerRightCorner,
                                      upperRightCorner, upperLeftCorner);
            }
          }
        else
          {
          // Search tiles to link
          int tileXStart = extractIndex[0] / (m_TileSize / 2);
          int tileYStart = extractIndex[1] / (m_TileSize / 2);

          // Create KML with link
          if (!m_UseExtendMode)
            {
            this->GenerateKMLWithLink(path.str(), depth, x, y, tileXStart, tileYStart,
                                      north, south, east, west, centerLong, centerLat);
            }
          else
            {
            this->GenerateKMLExtendedWithLink(
              path.str(), depth, x, y, tileXStart, tileYStart,
              lowerLeftCorner, lowerRightCorner, upperRightCorner, upperLeftCorner,
              centerLong, centerLat);
            }
          }

        if (depth == 0)
          {
          // Add the headers and the basic stuffs in the kml only once.
          if (curIdx == 0)
            {
            this->RootKmlProcess(north, south, east, west);
            }

          // Add the bounding box kml
          this->BoundingBoxKmlProcess(north, south, east, west);
          }

        // Add the files to the kmz file
        // Relative path to the root directory  in the kmz file
        std::ostringstream jpg_in_kmz, kml_in_kmz;
        jpg_in_kmz << m_CurrentImageName << "/" << depth << "/" << x << "/" << y << ".jpg";
        kml_in_kmz << m_CurrentImageName << "/" << depth << "/" << x << "/" << y << m_KmlExtension;

        // Absolute path where are stored the files
        std::ostringstream jpg_absolute_path, kml_absolute_path;
        jpg_absolute_path << m_Path << "/" << y << ".jpg";
        kml_absolute_path << m_Path << "/" << y << m_KmlExtension;

        //Add the files to the kmz
        this->AddFileToKMZ(jpg_absolute_path, jpg_in_kmz);
        this->AddFileToKMZ(kml_absolute_path, kml_in_kmz);

        // Remove the unecessary files with stdio method :remove
        if (remove(kml_absolute_path.str().c_str())  ||  remove(jpg_absolute_path.str().c_str()))
          {
          itkExceptionMacro(
            << "Error while deleting the file" << kml_absolute_path.str() << "or file " << jpg_absolute_path.str());
          }
        }
      }
    }
}


/**
*
*/
template <class TInputImage>
int
KmzProductWriter<TInputImage>
::AddFileToKMZ(const std::ostringstream& absolutePath, const std::ostringstream& kmz_in_path)
{
  std::string absolute = absolutePath.str();
  std::string relative = kmz_in_path.str();
  std::string file_data;
  int         errs = 0;
  if (!kmlbase::File::ReadFileToString(absolute, &file_data))
    {
    itkExceptionMacro(<< "Error while reading file " << absolute);
    }
  else
    {
    m_KmzFile->AddFile(file_data, relative);
    }

  return errs;
}


/**
 * Actually the root kml is not fully generated :
 * It generates only the part till the network link
 * In case of multiple inputs, other network liks are
 * needed. See the method Add NetworkLinkToRootKML.
 */
template <class TInputImage>
void
KmzProductWriter<TInputImage>
::GenerateKMLRoot(const std::string& title,
      double north,
      double south,
      double east,
      double west,
      bool itkNotUsed(extended))
{
  // Give a name to the root file
  std::ostringstream kmlname;
  kmlname << m_Path;
  kmlname << "/";
  kmlname << m_FileName;
  kmlname << m_KmlExtension;
  m_RootKmlFile.open(kmlname.str().c_str());
  m_RootKmlFile << std::fixed << std::setprecision(6);

  m_RootKmlFile << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << std::endl;
  m_RootKmlFile << "<kml xmlns=\"http://www.opengis.net/kml/2.2\"" << std::endl;
  m_RootKmlFile << " xmlns:gx=\"http://www.google.com/kml/ext/2.2\">" << std::endl;
  m_RootKmlFile << "\t<Document>" << std::endl;
  m_RootKmlFile << "\t\t<name>" << title << "</name>" << std::endl;
  m_RootKmlFile << "\t\t<description></description>" << std::endl;
  m_RootKmlFile << "\t\t<LookAt>" << std::endl;
  m_RootKmlFile << "\t\t\t<longitude>" << (east + west) / 2. << "</longitude>" << std::endl;
  m_RootKmlFile << "\t\t\t<latitude>" << (south + north) / 2. << "</latitude>" << std::endl;
  m_RootKmlFile << "\t\t\t<altitude>35000</altitude>" << std::endl;
  m_RootKmlFile << "\t\t\t<range>35000</range>" << std::endl;
  m_RootKmlFile << "\t\t</LookAt>" << std::endl;
}

/**
 */
template <class TInputImage>
void
KmzProductWriter<TInputImage>
::RootKmlProcess(double north, double south, double east, double west)
{
  bool extended = m_UseExtendMode;
  this->GenerateKMLRoot(m_FileName, north, south, east, west, extended);

  // Add the legend for this product if any
  this->ProcessLegends();

  // Add the flag netwotk link for each input image
  this->AddNetworkLinkToRootKML(north, south, east, west, m_CurrentImageName, true, 0);

  // Last thing to do is to close the root kml
  this->CloseRootKML();

  // Add the root kml in the kmz
  std::ostringstream root_in_kmz;
  root_in_kmz << m_FileName << m_KmlExtension;

  std::ostringstream root_absolute_path;
  root_absolute_path << m_Path << "/" << root_in_kmz.str();

  // Add the root file in the kmz
  this->AddFileToKMZ(root_absolute_path, root_in_kmz);

  // Remove the root files with stdio method :remove
  if (remove(root_absolute_path.str().c_str()) != 0)
    {
    itkExceptionMacro(<< "Error while deleting the file" << root_absolute_path.str());
    }
}

/**
 * Close the root kml
 */
template <class TInputImage>
void
KmzProductWriter<TInputImage>
::CloseRootKML()
{
  if (!m_Logo.IsNull())
    {
    RegionType logoReg = m_Logo->GetLargestPossibleRegion();
    SizeType   logoSize = logoReg.GetSize();
    double     lx = static_cast<double>(logoSize[0]);
    double     ly = static_cast<double>(logoSize[1]);
    int        sizey = 150/4;
    int        sizex = static_cast<int>((lx / ly * sizey));

    /** LOGO **/
    m_RootKmlFile << "\t\t<ScreenOverlay>" << std::endl;
    m_RootKmlFile << "\t\t\t<Icon>" << std::endl;
    m_RootKmlFile << "\t\t\t\t<href>logo.jpeg</href>" << std::endl;
    m_RootKmlFile << "\t\t\t</Icon>" << std::endl;
    m_RootKmlFile << "\t\t\t<name>Logo</name>" << std::endl;
    m_RootKmlFile << "\t\t\t<overlayXY x=\"1\" y=\"1\" xunits=\"fraction\" yunits=\"fraction\"/>" << std::endl;
    m_RootKmlFile << "\t\t\t<screenXY x=\"1\" y=\"1\" xunits=\"fraction\" yunits=\"fraction\"/>" << std::endl;
    m_RootKmlFile << "\t\t\t<size x=\"" << sizex << "\" y=\"" << sizey <<
    "\" xunits=\"pixels\" yunits=\"pixels\"/> " << std::endl;
    m_RootKmlFile << "\t\t</ScreenOverlay>" << std::endl;
    /** LOGO **/
    }

  m_RootKmlFile << "\t</Document>" << std::endl;
  m_RootKmlFile << "</kml>" << std::endl;
  m_RootKmlFile.close();
}


/**
 * Add A networkLink to the root kml
 *
 */
template <class TInputImage>
void
KmzProductWriter<TInputImage>
::AddNetworkLinkToRootKML(double north,
        double south,
        double east,
        double west,
        const std::string& directory,
        bool addRegion,
        unsigned int pos)
{
  m_RootKmlFile << "\t\t<Document>" << std::endl;
  m_RootKmlFile << "\t\t\t<name>" << m_FileName<< "</name>" << std::endl;
  //m_RootKmlFile << "\t\t\t<Description>" << m_ProductVector[pos].m_Description << "</Description>" << std::endl;
  m_RootKmlFile << "\t\t\t<LookAt>" << std::endl;
  m_RootKmlFile << "\t\t\t\t<longitude>" << (east + west) / 2. << "</longitude>" << std::endl;
  m_RootKmlFile << "\t\t\t\t<latitude>" << (south + north) / 2. << "</latitude>" << std::endl;
  m_RootKmlFile << "\t\t\t\t<altitude>35000</altitude>" << std::endl;
  m_RootKmlFile << "\t\t\t\t<range>35000</range>" << std::endl;
  m_RootKmlFile << "\t\t\t</LookAt>" << std::endl;

  m_RootKmlFile << "\t\t\t<NetworkLink>" << std::endl;
  m_RootKmlFile << "\t\t\t\t<name>" << m_FileName<< "</name>" <<  std::endl;
  m_RootKmlFile << "\t\t\t\t<open>1</open>" << std::endl;
  m_RootKmlFile << "\t\t\t<Style>" << std::endl;
  m_RootKmlFile << "\t\t\t\t<ListStyle id=\"hideChildren\">" << std::endl;
  m_RootKmlFile << "\t\t\t\t\t<listItemType>checkHideChildren</listItemType>" << std::endl;
  m_RootKmlFile << "\t\t\t\t</ListStyle>" << std::endl;
  m_RootKmlFile << "\t\t\t</Style>" << std::endl;

  if (addRegion)
    {
    m_RootKmlFile << "\t\t\t\t<Region>" << std::endl;
    m_RootKmlFile << "\t\t\t\t\t<Lod>" << std::endl;
    m_RootKmlFile << "\t\t\t\t\t\t<minLodPixels>" << m_TileSize / 2 << "</minLodPixels>" << std::endl;
    /** If the last depth, put level of detail LOD to infinite (ie -1)*/
    if (m_CurrentDepth == m_MaxDepth)
      m_RootKmlFile << "\t\t\t\t\t\t<maxLodPixels>-1</maxLodPixels>" << std::endl;
    else
      m_RootKmlFile << "\t\t\t\t\t\t<maxLodPixels>" << m_TileSize * 4 << "</maxLodPixels>" << std::endl;
    m_RootKmlFile << "\t\t\t\t\t</Lod>" << std::endl;
    m_RootKmlFile << "\t\t\t\t\t<LatLonAltBox>" << std::endl;
    m_RootKmlFile << "\t\t\t\t\t\t<north>" << north << "</north>" << std::endl;
    m_RootKmlFile << "\t\t\t\t\t\t<south>" << south << "</south>" << std::endl;
    m_RootKmlFile << "\t\t\t\t\t\t<east>" << east << "</east>" << std::endl;
    m_RootKmlFile << "\t\t\t\t\t\t<west>" << west << "</west>" << std::endl;
    m_RootKmlFile << "\t\t\t\t\t</LatLonAltBox>" << std::endl;
    m_RootKmlFile << "\t\t\t\t</Region>" << std::endl;
    }

  m_RootKmlFile << "\t\t\t\t<Link>" << std::endl;
  m_RootKmlFile << "\t\t\t\t\t<href>" << directory << "/0/0/0" << m_KmlExtension << "</href>" << std::endl;
  m_RootKmlFile << "\t\t\t\t\t<viewRefreshMode>onRegion</viewRefreshMode>" << std::endl;
  m_RootKmlFile << "\t\t\t\t</Link>" << std::endl;
  m_RootKmlFile << "\t\t\t</NetworkLink>" << std::endl;

  m_RootKmlFile << "\t\t\t<Folder>" << std::endl;
  m_RootKmlFile << "\t\t\t\t<name>The bounding box </name>" << std::endl;
  m_RootKmlFile << "\t\t\t\t<Style>" << std::endl;
  m_RootKmlFile << "\t\t\t\t\t<ListStyle id=\"hideChildren\">" << std::endl;
  m_RootKmlFile << "\t\t\t\t\t\t<listItemType>checkHideChildren</listItemType>" << std::endl;
  m_RootKmlFile << "\t\t\t\t\t</ListStyle>" << std::endl;
  m_RootKmlFile << "\t\t\t\t</Style>" << std::endl;
  m_RootKmlFile << "\t\t\t\t<NetworkLink>" << std::endl;
  m_RootKmlFile << "\t\t\t\t<Region>" << std::endl;
  m_RootKmlFile << "\t\t\t\t\t<Lod>" << std::endl;
  m_RootKmlFile << "\t\t\t\t\t\t<minLodPixels>" << m_TileSize / 2 << "</minLodPixels>" << std::endl;
  m_RootKmlFile << "\t\t\t\t\t\t<maxLodPixels>-1</maxLodPixels>" << std::endl;
  m_RootKmlFile << "\t\t\t\t\t</Lod>" << std::endl;
  m_RootKmlFile << "\t\t\t\t</Region>" << std::endl;
  m_RootKmlFile << "\t\t\t\t<Link>" << std::endl;
  m_RootKmlFile << "\t\t\t\t\t<href>bounds/bound_" << pos << m_KmlExtension << "</href>" << std::endl;
  m_RootKmlFile << "\t\t\t\t\t<viewRefreshMode>onRegion</viewRefreshMode>" << std::endl;
  m_RootKmlFile << "\t\t\t\t</Link>" << std::endl;
  m_RootKmlFile << "\t\t\t\t</NetworkLink>" << std::endl;
  m_RootKmlFile << "\t\t\t</Folder>" << std::endl;

  // Add a placemark with the images used as legend
  // If any
  this->AddLegendToRootKml(north, south, east, west);

  m_RootKmlFile << "\t\t</Document>" << std::endl;
}

/**
  * Add the bounding box kml
  */

template <class TInputImage>
void
KmzProductWriter<TInputImage>
::BoundingBoxKmlProcess(double north, double south, double east, double west)
{
  // Create the bounding kml
  this->GenerateBoundingKML(north, south,  east, west);

  // Add the root kml in the kmz
  std::ostringstream bound_in_kmz;
  bound_in_kmz << "bounds/bound_0"<< m_KmlExtension;
  std::ostringstream bound_absolute_path;
  bound_absolute_path << m_Path << "/bound_0"<< m_KmlExtension;

  // Add the root file in the kmz
  this->AddFileToKMZ(bound_absolute_path, bound_in_kmz);

  // Remove the bounding files with stdio method :remove
  if (remove(bound_absolute_path.str().c_str()) != 0)
    {
    itkExceptionMacro(<< "Error while deleting the file" << bound_absolute_path.str());
    }
}

/**
 * Add the legends available
 *
 */
template <class TInputImage>
void
KmzProductWriter<TInputImage>
::AddLegendToRootKml(double north, double south, double east, double west)
{
  double lat = (north + south) / 2.;
  double lon = (east + west) / 2.;

  if (m_LegendVector.size() > 0)
    {
    m_RootKmlFile << "\t\t\t\t<Placemark>" << std::endl;
    m_RootKmlFile << "\t\t\t\t\t<name>Legend</name>" << std::endl;
    m_RootKmlFile << "\t\t\t\t\t<description>" << std::endl;
    m_RootKmlFile << "\t\t\t\t\t\t<![CDATA[ Legend of the product " << m_FileName << std::endl;

    for(unsigned int idx = 0; idx < m_LegendVector.size(); ++idx)
      m_RootKmlFile << "\t\t\t\t\t\t<img src=\"legends/legend_"<< idx <<".jpeg\" width=\"215\" height=\"175\"  >";

    m_RootKmlFile << "\t\t\t\t\t\t ]]>" << std::endl;
    m_RootKmlFile << "\t\t\t\t\t</description>" << std::endl;
    m_RootKmlFile << "\t\t\t\t\t<Point>" << std::endl;
    m_RootKmlFile << "\t\t\t\t\t\t<coordinates>" << lon << "," << lat << "</coordinates>" << std::endl;
    m_RootKmlFile << "\t\t\t\t\t</Point>" << std::endl;
    m_RootKmlFile << "\t\t\t\t</Placemark>" << std::endl;
    }
}


template <class TInputImage>
void
KmzProductWriter<TInputImage>
::GenerateKMLExtended(const std::string& pathname, int depth, int itkNotUsed(x), int y,
          OutputPointType lowerLeft, OutputPointType lowerRight,
          OutputPointType upperRight, OutputPointType upperLeft)
{
  std::ostringstream kmlname;
  kmlname << pathname;
  kmlname << "/";
  kmlname << y << "xt.kml";
  std::ofstream fileTest(kmlname.str().c_str());

  fileTest << std::fixed << std::setprecision(6);

  fileTest << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << std::endl;
  fileTest << "<kml xmlns=\"http://www.opengis.net/kml/2.2\"" << std::endl;
  fileTest << " xmlns:gx=\"http://www.google.com/kml/ext/2.2\">" << std::endl;
  fileTest << "\t<Document>" << std::endl;
  fileTest << "\t\t<name>" << y << "xt.kml</name>" << std::endl;
  fileTest << "\t\t<Region>" << std::endl;
  fileTest << "\t\t\t<Lod>" << std::endl;
  fileTest << "\t\t\t\t<minLodPixels>" << m_TileSize / 2 << "</minLodPixels>" << std::endl;
  fileTest << "\t\t\t\t<maxLodPixels>-1</maxLodPixels>" << std::endl;
  fileTest << "\t\t\t</Lod>" << std::endl;
  fileTest << "\t\t\t<gx:LatLonQuad>" << std::endl;
  fileTest << "\t\t\t\t<coordinates>" << std::endl;
  fileTest << "\t\t\t\t\t" << lowerLeft[0]  << "," << lowerLeft[1];
  fileTest << " " << lowerRight[0] << "," << lowerRight[1];
  fileTest << " " << upperRight[0] << "," << upperRight[1];
  fileTest << " " << upperLeft[0]  << "," << upperLeft[1]  << std::endl;
  fileTest << "\t\t\t\t</coordinates>" << std::endl;
  fileTest << "\t\t\t</gx:LatLonQuad>" << std::endl;
  fileTest << "\t\t</Region>" << std::endl;
  fileTest << "\t\t<GroundOverlay>" << std::endl;
  fileTest << "\t\t\t<drawOrder>" << depth + (m_CurIdx * m_MaxDepth) << "</drawOrder>" << std::endl;
  fileTest << "\t\t\t<Icon>" << std::endl;
  fileTest << "\t\t\t\t<href>" << y << ".jpg" << "</href>" << std::endl;
  fileTest << "\t\t\t</Icon>" << std::endl;
  fileTest << "\t\t\t<gx:LatLonQuad>" << std::endl;
  fileTest << "\t\t\t\t<coordinates>" << std::endl;
  fileTest << "\t\t\t\t\t" << lowerLeft[0]  << "," << lowerLeft[1];
  fileTest << " " << lowerRight[0] << "," << lowerRight[1];
  fileTest << " " << upperRight[0] << "," << upperRight[1];
  fileTest << " " << upperLeft[0]  << "," << upperLeft[1]  << std::endl;
  fileTest << "\t\t\t\t</coordinates>" << std::endl;
  fileTest << "\t\t\t</gx:LatLonQuad>" << std::endl;
  fileTest << "\t\t</GroundOverlay>" << std::endl;
  fileTest << "\t</Document>" << std::endl;
  fileTest << "</kml>" << std::endl;

  fileTest.close();
}


template <class TInputImage>
void
KmzProductWriter<TInputImage>
::GenerateKML(const std::string& pathname, int depth,
              int itkNotUsed(x), int y, double north, double south,
              double east, double west)
{
  std::ostringstream kmlname;
  kmlname << pathname;
  kmlname << "/";
  kmlname << y << ".kml";
  std::ofstream fileTest(kmlname.str().c_str());

  fileTest << std::fixed << std::setprecision(6);

  fileTest << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << std::endl;
  fileTest << "<kml xmlns=\"http://www.opengis.net/kml/2.2\"" << std::endl;
  fileTest << " xmlns:gx=\"http://www.google.com/kml/ext/2.2\">" << std::endl;
  fileTest << "\t<Document>" << std::endl;
  fileTest << "\t\t<name>" << y << ".kml</name>" << std::endl;
  fileTest << "\t\t<Region>" << std::endl;
  fileTest << "\t\t\t<Lod>" << std::endl;
  fileTest << "\t\t\t\t<minLodPixels>" << m_TileSize / 2 << "</minLodPixels>" << std::endl;
  if (m_CurrentDepth == m_MaxDepth) fileTest << "\t\t\t\t<maxLodPixels>-1</maxLodPixels>" << std::endl;
  else fileTest << "\t\t\t\t<maxLodPixels>" << m_TileSize * 4 << "</maxLodPixels>" << std::endl;
  fileTest << "\t\t\t</Lod>" << std::endl;
  fileTest << "\t\t\t<LatLonAltBox>" << std::endl;
  fileTest << "\t\t\t\t<north>" << north << "</north>" << std::endl;
  fileTest << "\t\t\t\t<south>" << south << "</south>" << std::endl;
  fileTest << "\t\t\t\t<east>" << east << "</east>" << std::endl;
  fileTest << "\t\t\t\t<west>" << west << "</west>" << std::endl;
  fileTest << "\t\t\t</LatLonAltBox>" << std::endl;
  fileTest << "\t\t</Region>" << std::endl;
  fileTest << "\t\t<GroundOverlay>" << std::endl;
  fileTest << "\t\t\t<drawOrder>" << depth + (m_CurIdx * m_MaxDepth) << "</drawOrder>" << std::endl;
  fileTest << "\t\t\t<Icon>" << std::endl;
  fileTest << "\t\t\t\t<href>" << y << ".jpg" << "</href>" << std::endl;
  fileTest << "\t\t\t</Icon>" << std::endl;
  fileTest << "\t\t\t<LatLonAltBox>" << std::endl;
  fileTest << "\t\t\t\t<north>" << north << "</north>" << std::endl;
  fileTest << "\t\t\t\t<south>" << south << "</south>" << std::endl;
  fileTest << "\t\t\t\t<east>" << east << "</east>" << std::endl;
  fileTest << "\t\t\t\t<west>" << west << "</west>" << std::endl;
  fileTest << "\t\t\t</LatLonAltBox>" << std::endl;
  fileTest << "\t\t</GroundOverlay>" << std::endl;
  fileTest << "\t</Document>" << std::endl;
  fileTest << "</kml>" << std::endl;

  fileTest.close();
}

template <class TInputImage>
void
KmzProductWriter<TInputImage>
::GenerateKMLExtendedWithLink(const std::string& pathname,
                              int depth, int itkNotUsed(x), int y, int tileStartX, int tileStartY,
                              OutputPointType lowerLeft, OutputPointType lowerRight,
                              OutputPointType upperRight, OutputPointType upperLeft,
                              double centerLong, double centerLat)
{
  std::ostringstream kmlname;
  kmlname << pathname;
  kmlname << "/";
  kmlname << y << "xt.kml";
  std::ofstream fileTest(kmlname.str().c_str());

  fileTest << std::fixed << std::setprecision(6);

  fileTest << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << std::endl;
  fileTest << "<kml xmlns=\"http://www.opengis.net/kml/2.2\"" << std::endl;
  fileTest << " xmlns:gx=\"http://www.google.com/kml/ext/2.2\">" << std::endl;
  fileTest << "\t<Document>" << std::endl;
  fileTest << "\t\t<name>" << y << "xt.kml</name>" << std::endl;
  fileTest << "\t\t<Region>" << std::endl;
  fileTest << "\t\t\t<Lod>" << std::endl;
  fileTest << "\t\t\t\t<minLodPixels>" << m_TileSize / 2 << "</minLodPixels>" << std::endl;
  fileTest << "\t\t\t\t<maxLodPixels>-1</maxLodPixels>" << std::endl;
  fileTest << "\t\t\t</Lod>" << std::endl;
  fileTest << "\t\t\t<gx:LatLonQuad>" << std::endl;
  fileTest << "\t\t\t\t<coordinates>" << std::endl;
  fileTest << "\t\t\t\t\t" << lowerLeft[0]  << "," << lowerLeft[1];
  fileTest << " " << lowerRight[0] << "," << lowerRight[1];
  fileTest << " " << upperRight[0] << "," << upperRight[1];
  fileTest << " " << upperLeft[0]  << "," << upperLeft[1]  << std::endl;
  fileTest << "\t\t\t\t</coordinates>" << std::endl;
  fileTest << "\t\t\t</gx:LatLonQuad>" << std::endl;
  fileTest << "\t\t</Region>" << std::endl;
  fileTest << "\t\t<GroundOverlay>" << std::endl;
  fileTest << "\t\t\t<drawOrder>" << depth + (m_CurIdx * m_MaxDepth) << "</drawOrder>" << std::endl;
  fileTest << "\t\t\t<Icon>" << std::endl;
  fileTest << "\t\t\t\t<href>" << y << ".jpg" << "</href>" << std::endl;
  fileTest << "\t\t\t</Icon>" << std::endl;
  fileTest << "\t\t\t<gx:LatLonQuad>" << std::endl;
  fileTest << "\t\t\t\t<coordinates>" << std::endl;
  fileTest << "\t\t\t\t\t" << lowerLeft[0]  << "," << lowerLeft[1];
  fileTest << " " << lowerRight[0] << "," << lowerRight[1];
  fileTest << " " << upperRight[0] << "," << upperRight[1];
  fileTest << " " << upperLeft[0]  << "," << upperLeft[1]  << std::endl;
  fileTest << "\t\t\t\t</coordinates>" << std::endl;
  fileTest << "\t\t\t</gx:LatLonQuad>" << std::endl;
  fileTest << "\t\t</GroundOverlay>" << std::endl;

  /* Sous tuile 1 */
  std::ostringstream fileTile1;
  fileTile1 << "../../";
  fileTile1 << depth + 1;
  fileTile1 << "/";
  fileTile1 << tileStartX;
  fileTile1 << "/";
  fileTile1 << tileStartY;

  fileTest << "\t\t<NetworkLink>" << std::endl;
  fileTest << "\t\t\t<name>" << fileTile1.str() << ".jpg</name>" << std::endl;
  fileTest << "\t\t\t<Region>" << std::endl;
  fileTest << "\t\t\t\t<Lod>" << std::endl;
  fileTest << "\t\t\t\t\t<minLodPixels>" << m_TileSize / 2 << "</minLodPixels>" << std::endl;
  fileTest << "\t\t\t\t\t<maxLodPixels>-1</maxLodPixels>" << std::endl;
  fileTest << "\t\t\t\t</Lod>" << std::endl;
  fileTest << "\t\t\t\t<gx:LatLonQuad>" << std::endl;
  fileTest << "\t\t\t\t\t<coordinates>" << std::endl;
  fileTest << "\t\t\t\t\t\t" << lowerLeft[0]  << "," << centerLat;
  fileTest << " " << centerLong << "," << centerLat;
  fileTest << " " << centerLong << "," << upperLeft[1];
  fileTest << " " << upperLeft[0]  << "," << upperLeft[1]  << std::endl;
  fileTest << "\t\t\t\t\t</coordinates>" << std::endl;
  fileTest << "\t\t\t\t</gx:LatLonQuad>" << std::endl;
  fileTest << "\t\t\t</Region>" << std::endl;
  fileTest << "\t\t\t<Link>" << std::endl;
  fileTest << "\t\t\t\t<href>" << fileTile1.str() << "xt.kml</href>" << std::endl;
  fileTest << "\t\t\t\t<viewRefreshMode>onRegion</viewRefreshMode>" << std::endl;
  fileTest << "\t\t\t\t<viewFormat/>" << std::endl;
  fileTest << "\t\t\t</Link>" << std::endl;
  fileTest << "\t\t</NetworkLink>" << std::endl;

  /* Sous tuile 2 */
  std::ostringstream fileTile2;
  fileTile2 << "../../";
  fileTile2 << depth + 1;
  fileTile2 << "/";
  fileTile2 << tileStartX + 1;
  fileTile2 << "/";
  fileTile2 << tileStartY;

  fileTest << "\t\t<NetworkLink>" << std::endl;
  fileTest << "\t\t\t<name>" << fileTile2.str() << ".jpg</name>" << std::endl;
  fileTest << "\t\t\t<Region>" << std::endl;
  fileTest << "\t\t\t<Lod>" << std::endl;
  fileTest << "\t\t\t\t<minLodPixels>" << m_TileSize / 2 << "</minLodPixels>" << std::endl;
  fileTest << "\t\t\t\t<maxLodPixels>-1</maxLodPixels>" << std::endl;
  fileTest << "\t\t\t</Lod>" << std::endl;
  fileTest << "\t\t\t\t<gx:LatLonQuad>" << std::endl;
  fileTest << "\t\t\t\t\t<coordinates>" << std::endl;
  fileTest << "\t\t\t\t\t\t" << centerLong  << "," << centerLat;
  fileTest << " " << lowerRight[0] << "," << centerLat;
  fileTest << " " << upperRight[0] << "," << upperRight[1];
  fileTest << " " << centerLong << "," << upperLeft[1]  << std::endl;
  fileTest << "\t\t\t\t\t</coordinates>" << std::endl;
  fileTest << "\t\t\t\t</gx:LatLonQuad>" << std::endl;
  fileTest << "\t\t\t</Region>" << std::endl;
  fileTest << "\t\t\t<Link>" << std::endl;
  fileTest << "\t\t\t\t<href>" << fileTile2.str() << "xt.kml</href>" << std::endl;
  fileTest << "\t\t\t\t<viewRefreshMode>onRegion</viewRefreshMode>" << std::endl;
  fileTest << "\t\t\t\t<viewFormat/>" << std::endl;
  fileTest << "\t\t\t</Link>" << std::endl;
  fileTest << "\t\t</NetworkLink>" << std::endl;

  /* Sous tuile 3 */
  std::ostringstream fileTile3;
  fileTile3 << "../../";
  fileTile3 << depth + 1;
  fileTile3 << "/";
  fileTile3 << tileStartX + 1;
  fileTile3 << "/";
  fileTile3 << tileStartY + 1;

  fileTest << "\t\t<NetworkLink>" << std::endl;
  fileTest << "\t\t\t<name>" << fileTile3.str() << ".jpg</name>" << std::endl;
  fileTest << "\t\t\t<Region>" << std::endl;
  fileTest << "\t\t\t\t<Lod>" << std::endl;
  fileTest << "\t\t\t\t\t<minLodPixels>" << m_TileSize / 2 << "</minLodPixels>" << std::endl;
  fileTest << "\t\t\t\t\t<maxLodPixels>-1</maxLodPixels>" << std::endl;
  fileTest << "\t\t\t\t</Lod>" << std::endl;
  fileTest << "\t\t\t\t<gx:LatLonQuad>" << std::endl;
  fileTest << "\t\t\t\t\t<coordinates>" << std::endl;
  fileTest << "\t\t\t\t\t\t" << centerLong  << "," << lowerLeft[1];
  fileTest << " " << lowerRight[0] << "," << lowerRight[1];
  fileTest << " " << upperRight[0] << "," << centerLat;
  fileTest << " " << centerLong  << "," << centerLat  << std::endl;
  fileTest << "\t\t\t\t\t</coordinates>" << std::endl;
  fileTest << "\t\t\t\t</gx:LatLonQuad>" << std::endl;
  fileTest << "\t\t\t</Region>" << std::endl;
  fileTest << "\t\t\t<Link>" << std::endl;
  fileTest << "\t\t\t\t<href>" << fileTile3.str() << "xt.kml</href>" << std::endl;
  fileTest << "\t\t\t\t<viewRefreshMode>onRegion</viewRefreshMode>" << std::endl;
  fileTest << "\t\t\t\t<viewFormat/>" << std::endl;
  fileTest << "\t\t\t</Link>" << std::endl;
  fileTest << "\t\t</NetworkLink>" << std::endl;

  /* Sous tuile 4 */
  std::ostringstream fileTile4;
  fileTile4 << "../../";
  fileTile4 << depth + 1;
  fileTile4 << "/";
  fileTile4 << tileStartX;
  fileTile4 << "/";
  fileTile4 << tileStartY + 1;

  fileTest << "\t\t<NetworkLink>" << std::endl;
  fileTest << "\t\t\t<name>" << fileTile4.str() << ".jpg</name>" << std::endl;
  fileTest << "\t\t\t<Region>" << std::endl;
  fileTest << "\t\t\t\t<Lod>" << std::endl;
  fileTest << "\t\t\t\t\t<minLodPixels>" << m_TileSize / 2 << "</minLodPixels>" << std::endl;
  fileTest << "\t\t\t\t\t<maxLodPixels>-1</maxLodPixels>" << std::endl;
  fileTest << "\t\t\t\t</Lod>" << std::endl;
  fileTest << "\t\t\t\t<gx:LatLonQuad>" << std::endl;
  fileTest << "\t\t\t\t\t<coordinates>" << std::endl;
  fileTest << "\t\t\t\t\t\t" << lowerLeft[0]  << "," << lowerLeft[1];
  fileTest << "\t\t\t\t\t\t" << centerLong << "," << lowerRight[1];
  fileTest << "\t\t\t\t\t\t" << centerLong << "," << centerLat;
  fileTest << "\t\t\t\t\t\t" << upperLeft[0] << "," << centerLat  << std::endl;
  fileTest << "\t\t\t\t\t</coordinates>" << std::endl;
  fileTest << "\t\t\t\t</gx:LatLonQuad>" << std::endl;
  fileTest << "\t\t\t</Region>" << std::endl;
  fileTest << "\t\t\t<Link>" << std::endl;
  fileTest << "\t\t\t\t<href>" << fileTile4.str() << "xt.kml</href>" << std::endl;
  fileTest << "\t\t\t\t<viewRefreshMode>onRegion</viewRefreshMode>" << std::endl;
  fileTest << "\t\t\t\t<viewFormat/>" << std::endl;
  fileTest << "\t\t\t</Link>" << std::endl;
  fileTest << "\t\t</NetworkLink>" << std::endl;

  fileTest << "\t</Document>" << std::endl;
  fileTest << "</kml>" << std::endl;
  fileTest.close();

}

template <class TInputImage>
void
KmzProductWriter<TInputImage>
::GenerateKMLWithLink(const std::string& pathname,
                      int depth, int itkNotUsed(x), int y, int tileStartX, int tileStartY,
                      double north, double south, double east, double west, double centerLong, double centerLat)
{
  std::ostringstream kmlname;
  kmlname << pathname;
  kmlname << "/";
  kmlname << y << ".kml";
  std::ofstream fileTest(kmlname.str().c_str());

  fileTest << std::fixed << std::setprecision(6);

  fileTest << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << std::endl;
  fileTest << "<kml xmlns=\"http://www.opengis.net/kml/2.2\"" << std::endl;
  fileTest << " xmlns:gx=\"http://www.google.com/kml/ext/2.2\">" << std::endl;
  fileTest << "\t<Document>" << std::endl;
  fileTest << "\t\t<name>" << y << ".kml</name>" << std::endl;
  fileTest << "\t\t<Region>" << std::endl;
  fileTest << "\t\t\t<Lod>" << std::endl;
  fileTest << "\t\t\t\t<minLodPixels>" << m_TileSize / 2 << "</minLodPixels>" << std::endl;
  if (m_CurrentDepth == m_MaxDepth)
    fileTest << "\t\t\t\t<maxLodPixels>-1</maxLodPixels>" << std::endl;
  else
    fileTest << "\t\t\t\t<maxLodPixels>" << m_TileSize * 2 << "</maxLodPixels>" << std::endl;
  fileTest << "\t\t\t</Lod>" << std::endl;
  fileTest << "\t\t\t<LatLonAltBox>" << std::endl;
  fileTest << "\t\t\t\t<north>" << north << "</north>" << std::endl;
  fileTest << "\t\t\t\t<south>" << south << "</south>" << std::endl;
  fileTest << "\t\t\t\t<east>" << east << "</east>" << std::endl;
  fileTest << "\t\t\t\t<west>" << west << "</west>" << std::endl;
  fileTest << "\t\t\t</LatLonAltBox>" << std::endl;
  fileTest << "\t\t</Region>" << std::endl;
  fileTest << "\t\t<GroundOverlay>" << std::endl;
  fileTest << "\t\t\t<drawOrder>" << depth + (m_CurIdx * m_MaxDepth) << "</drawOrder>" << std::endl;
  fileTest << "\t\t\t<Icon>" << std::endl;
  fileTest << "\t\t\t\t<href>" << y << ".jpg" << "</href>" << std::endl;
  fileTest << "\t\t\t</Icon>" << std::endl;
  fileTest << "\t\t\t<LatLonAltBox>" << std::endl;
  fileTest << "\t\t\t\t<north>" << north << "</north>" << std::endl;
  fileTest << "\t\t\t\t<south>" << south << "</south>" << std::endl;
  fileTest << "\t\t\t\t<east>" << east << "</east>" << std::endl;
  fileTest << "\t\t\t\t<west>" << west << "</west>" << std::endl;
  fileTest << "\t\t\t</LatLonAltBox>" << std::endl;
  fileTest << "\t\t</GroundOverlay>" << std::endl;

  /* Sous tuile 1 */
  std::ostringstream fileTile1;
  fileTile1 << "../../";
  fileTile1 << depth + 1;
  fileTile1 << "/";
  fileTile1 << tileStartX;
  fileTile1 << "/";
  fileTile1 << tileStartY;

  fileTest << "\t\t<NetworkLink>" << std::endl;
  fileTest << "\t\t\t<name>" << fileTile1.str() << ".jpg</name>" << std::endl;
  fileTest << "\t\t\t<Region>" << std::endl;
  fileTest << "\t\t\t\t<Lod>" << std::endl;
  fileTest << "\t\t\t\t\t<minLodPixels>" << m_TileSize / 2 << "</minLodPixels>" << std::endl;
  if (m_CurrentDepth == m_MaxDepth) fileTest << "\t\t\t\t<maxLodPixels>-1</maxLodPixels>" << std::endl;
  else fileTest << "\t\t\t\t<maxLodPixels>" << m_TileSize * 2 << "</maxLodPixels>" << std::endl;
  fileTest << "\t\t\t\t</Lod>" << std::endl;
  fileTest << "\t\t\t\t<LatLonAltBox>" << std::endl;
  fileTest << "\t\t\t\t\t<north>" << north << "</north>" << std::endl;
  fileTest << "\t\t\t\t\t<south>" << centerLat << "</south>" << std::endl;
  fileTest << "\t\t\t\t\t<east>" << centerLong << "</east>" << std::endl;
  fileTest << "\t\t\t\t\t<west>" << west << "</west>" << std::endl;
  fileTest << "\t\t\t\t</LatLonAltBox>" << std::endl;
  fileTest << "\t\t\t</Region>" << std::endl;
  fileTest << "\t\t\t<Link>" << std::endl;
  fileTest << "\t\t\t\t<href>" << fileTile1.str() << ".kml</href>" << std::endl;
  fileTest << "\t\t\t\t<viewRefreshMode>onRegion</viewRefreshMode>" << std::endl;
  fileTest << "\t\t\t\t<viewFormat/>" << std::endl;
  fileTest << "\t\t\t</Link>" << std::endl;
  fileTest << "\t\t</NetworkLink>" << std::endl;

  /* Sous tuile 2 */
  std::ostringstream fileTile2;
  fileTile2 << "../../";
  fileTile2 << depth + 1;
  fileTile2 << "/";
  fileTile2 << tileStartX + 1;
  fileTile2 << "/";
  fileTile2 << tileStartY;

  fileTest << "\t\t<NetworkLink>" << std::endl;
  fileTest << "\t\t\t<name>" << fileTile2.str() << ".jpg</name>" << std::endl;
  fileTest << "\t\t\t<Region>" << std::endl;
  fileTest << "\t\t\t\t<Lod>" << std::endl;
  fileTest << "\t\t\t\t\t<minLodPixels>" << m_TileSize / 2 << "</minLodPixels>" << std::endl;
  if (m_CurrentDepth == m_MaxDepth) fileTest << "\t\t\t\t<maxLodPixels>-1</maxLodPixels>" << std::endl;
  else fileTest << "\t\t\t\t<maxLodPixels>" << m_TileSize * 4 << "</maxLodPixels>" << std::endl;
  fileTest << "\t\t\t\t</Lod>" << std::endl;
  fileTest << "\t\t\t\t<LatLonAltBox>" << std::endl;
  fileTest << "\t\t\t\t\t<north>" << north << "</north>" << std::endl;
  fileTest << "\t\t\t\t\t<south>" << centerLat << "</south>" << std::endl;
  fileTest << "\t\t\t\t\t<east>" << east << "</east>" << std::endl;
  fileTest << "\t\t\t\t\t<west>" << centerLong << "</west>" << std::endl;
  fileTest << "\t\t\t\t</LatLonAltBox>" << std::endl;
  fileTest << "\t\t\t</Region>" << std::endl;
  fileTest << "\t\t\t<Link>" << std::endl;
  fileTest << "\t\t\t\t<href>" << fileTile2.str() << ".kml</href>" << std::endl;
  fileTest << "\t\t\t\t<viewRefreshMode>onRegion</viewRefreshMode>" << std::endl;
  fileTest << "\t\t\t\t<viewFormat/>" << std::endl;
  fileTest << "\t\t\t</Link>" << std::endl;
  fileTest << "\t\t</NetworkLink>" << std::endl;

  /* Sous tuile 3 */
  std::ostringstream fileTile3;
  fileTile3 << "../../";
  fileTile3 << depth + 1;
  fileTile3 << "/";
  fileTile3 << tileStartX + 1;
  fileTile3 << "/";
  fileTile3 << tileStartY + 1;

  fileTest << "\t\t<NetworkLink>" << std::endl;
  fileTest << "\t\t\t<name>" << fileTile3.str() << ".jpg</name>" << std::endl;
  fileTest << "\t\t\t<Region>" << std::endl;
  fileTest << "\t\t\t\t<Lod>" << std::endl;
  fileTest << "\t\t\t\t\t<minLodPixels>" << m_TileSize / 2 << "</minLodPixels>" << std::endl;
  if (m_CurrentDepth == m_MaxDepth) fileTest << "\t\t\t\t<maxLodPixels>-1</maxLodPixels>" << std::endl;
  else fileTest << "\t\t\t\t<maxLodPixels>" << m_TileSize * 4 << "</maxLodPixels>" << std::endl;
  fileTest << "\t\t\t\t</Lod>" << std::endl;
  fileTest << "\t\t\t\t<LatLonAltBox>" << std::endl;
  fileTest << "\t\t\t\t\t<north>" << centerLat << "</north>" << std::endl;
  fileTest << "\t\t\t\t\t<south>" << south << "</south>" << std::endl;
  fileTest << "\t\t\t\t\t<east>" << east << "</east>" << std::endl;
  fileTest << "\t\t\t\t\t<west>" << centerLong << "</west>" << std::endl;
  fileTest << "\t\t\t\t</LatLonAltBox>" << std::endl;
  fileTest << "\t\t\t</Region>" << std::endl;
  fileTest << "\t\t\t<Link>" << std::endl;
  fileTest << "\t\t\t\t<href>" << fileTile3.str() << ".kml</href>" << std::endl;
  fileTest << "\t\t\t\t<viewRefreshMode>onRegion</viewRefreshMode>" << std::endl;
  fileTest << "\t\t\t\t<viewFormat/>" << std::endl;
  fileTest << "\t\t\t</Link>" << std::endl;
  fileTest << "\t\t</NetworkLink>" << std::endl;

  /* Sous tuile 4 */
  std::ostringstream fileTile4;
  fileTile4 << "../../";
  fileTile4 << depth + 1;
  fileTile4 << "/";
  fileTile4 << tileStartX;
  fileTile4 << "/";
  fileTile4 << tileStartY + 1;

  fileTest << "\t\t<NetworkLink>" << std::endl;
  fileTest << "\t\t\t<name>" << fileTile4.str() << ".jpg</name>" << std::endl;
  fileTest << "\t\t\t<Region>" << std::endl;
  fileTest << "\t\t\t\t<Lod>" << std::endl;
  fileTest << "\t\t\t\t\t<minLodPixels>" << m_TileSize / 2 << "</minLodPixels>" << std::endl;
  if (m_CurrentDepth == m_MaxDepth) fileTest << "\t\t\t\t<maxLodPixels>-1</maxLodPixels>" << std::endl;
  else fileTest << "\t\t\t\t<maxLodPixels>" << m_TileSize * 4 << "</maxLodPixels>" << std::endl;

  fileTest << "\t\t\t\t</Lod>" << std::endl;
  fileTest << "\t\t\t\t<LatLonAltBox>" << std::endl;
  fileTest << "\t\t\t\t\t<north>" << centerLat << "</north>" << std::endl;
  fileTest << "\t\t\t\t\t<south>" << south << "</south>" << std::endl;
  fileTest << "\t\t\t\t\t<east>" << centerLong << "</east>" << std::endl;
  fileTest << "\t\t\t\t\t<west>" << west << "</west>" << std::endl;
  fileTest << "\t\t\t\t</LatLonAltBox>" << std::endl;
  fileTest << "\t\t\t</Region>" << std::endl;
  fileTest << "\t\t\t<Link>" << std::endl;
  fileTest << "\t\t\t\t<href>" << fileTile4.str() << ".kml</href>" << std::endl;
  fileTest << "\t\t\t\t<viewRefreshMode>onRegion</viewRefreshMode>" << std::endl;
  fileTest << "\t\t\t\t<viewFormat/>" << std::endl;
  fileTest << "\t\t\t</Link>" << std::endl;
  fileTest << "\t\t</NetworkLink>" << std::endl;

  fileTest << "\t</Document>" << std::endl;
  fileTest << "</kml>" << std::endl;
  fileTest.close();

}


template <class TInputImage>
void
KmzProductWriter<TInputImage>
::GenerateBoundingKML(double north, double south, double east, double west)
{
  std::ostringstream kmlname;
  kmlname << m_Path;
  kmlname << "/";
  kmlname << "bound_0" << m_KmlExtension;
  std::ofstream fileTest(kmlname.str().c_str());

  fileTest << std::fixed << std::setprecision(6);

  fileTest << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << std::endl;
  fileTest << "<kml xmlns=\"http://www.opengis.net/kml/2.2\"" << std::endl;
  fileTest << " xmlns:gx=\"http://www.google.com/kml/ext/2.2\">" << std::endl;

  fileTest << "\t<Document>" << std::endl;
  fileTest << "\t\t<name> Bounding box of the  product " << m_FileName<< "</name>" <<
  std::endl;
  fileTest << "\t\t<open>1</open>" << std::endl;
  fileTest << "\t\t<Placemark>" << std::endl;
  fileTest << "\t\t\t<description>The bounding Box of the image</description>" << std::endl;
  fileTest << "\t\t<LineString>" << std::endl;
  fileTest << "\t\t\t<extrude>0</extrude>" << std::endl;
  fileTest << "\t\t\t<tessellate>1</tessellate>" << std::endl;
  fileTest << "\t\t\t<altitudeMode>clampedToGround</altitudeMode>" << std::endl;
  fileTest << "\t\t\t<coordinates>" << std::endl;

  fileTest << "\t\t\t\t\t" <<  west << "," << north << std::endl;
  fileTest << "\t\t\t\t\t" <<  east << "," << north << std::endl;
  fileTest << "\t\t\t\t\t" <<  east << "," << south << std::endl;
  fileTest << "\t\t\t\t\t" <<  west << "," << south << std::endl;
  fileTest << "\t\t\t\t\t" <<  west << "," << north << std::endl;

  fileTest << "\t\t\t</coordinates>" << std::endl;
  fileTest << "\t\t</LineString>" << std::endl;
  fileTest << "\t\t</Placemark>" << std::endl;

  fileTest << "\t</Document>" << std::endl;
  fileTest << "</kml>" << std::endl;

  fileTest.close();
}


/**
 */
template <class TInputImage>
std::string
KmzProductWriter<TInputImage>
::GetCuttenFileName(const std::string& itkNotUsed(description), unsigned int idx)
{
  std::string currentImageName;
  std::string tempName;

  std::ostringstream oss;
  oss << "tiles_" << idx;
  tempName = oss.str();

  // Replace all the blanks in the string
  unsigned int i = 0;
  while (i < tempName.length())
    {
    if (tempName[i] != ' ') currentImageName += tempName[i];
    ++i;
    }

  return currentImageName;
}

template <class TInputImage>
void
KmzProductWriter<TInputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  // Call the superclass implementation
  Superclass::PrintSelf(os, indent);
}

template <class TInputImage>
void
KmzProductWriter<TInputImage>
:: SetTileSize(unsigned int tileSize)
{
  if (tileSize >1 )
    m_TileSize = tileSize;
  else
    itkExceptionMacro(<< "Tile size is incorrect, it should be superior to 1");
}

}
#endif
