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
#ifndef __otbKmzProductWriter_h
#define __otbKmzProductWriter_h

#include <fstream>

#include "itkObjectFactory.h"

//kmz creation
#include "kml/engine/kmz_file.h"
#include "kml/base/file.h"

// Image Tiling
#include "otbMultiChannelExtractROI.h"
#include "otbImageFileWriter.h"
#include "otbVectorRescaleIntensityImageFilter.h"
#include "otbGenericRSTransform.h"
#include "otbStreamingShrinkImageFilter.h"
#include "itkCastImageFilter.h"

// Possiblity to includes vectordatas necessary includes
#include "otbVectorData.h"
#include "otbVectorDataFileWriter.h"


namespace otb
{

/** \class KmzProductWriter
 * \brief This class generate a Kmz from one or several input
 * images. The inputs must have a non empty metadata information.
 *
 * This class writes kmz files, supported by Google Earth. The input
 * must have metadata (a non empty keywordlist)
 *
 * The size of the tiles can be set via the method
 * SetTileSize(unsigned int), the user can also specify the name of
 * the output Kmz filename via SetPath().
 *
 *
 *
 * \ingroup IO
 *
 *
 * \ingroup OTBKMZWriter
 */
template <class TInputImage>
class ITK_EXPORT KmzProductWriter : public itk::ProcessObject
{
public:
  /** Standard class typedefs. */
  typedef KmzProductWriter                  Self;
  typedef itk::ProcessObject                Superclass;
  typedef itk::SmartPointer<Self>           Pointer;
  typedef itk::SmartPointer<const Self>     ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(KmzProductWriter, itk::ProcessObject);

  /** Some convenient typedefs. */
  typedef TInputImage                                  InputImageType;
  typedef typename InputImageType::InternalPixelType   InternalPixelType;
  typedef typename InputImageType::SizeType            SizeType;
  typedef typename InputImageType::IndexType           IndexType;
  typedef typename InputImageType::RegionType          RegionType;
  typedef unsigned char                                OutputPixelType;
  typedef typename InputImageType::Pointer             InputImagePointer;
  typedef typename InputImageType::PixelType           InputImagePixelType;

  typedef VectorData<double, 2>                        VectorDataType;
  typedef typename VectorDataType::DataNodeType        DataNodeType;
  typedef typename DataNodeType::PolygonType           PolygonType;
  typedef typename PolygonType::VertexType             VertexType;

  typedef VectorDataFileWriter<VectorDataType>         VectorDataFileWriterType;


  // Multi channels Extract ROI filter
  typedef MultiChannelExtractROI<InternalPixelType,  OutputPixelType> VectorImageExtractROIFilterType;

  // Writer
  typedef ImageFileWriter< VectorImage<OutputPixelType> >             VectorWriterType;

  // Resampler
  typedef StreamingShrinkImageFilter<InputImageType, InputImageType > StreamingShrinkImageFilterType;

  // Intensity Rescale
  typedef VectorRescaleIntensityImageFilter<InputImageType,
              InputImageType>                                         VectorRescaleIntensityImageFilterType;

  // Transformer
  typedef GenericRSTransform<>                                        TransformType;
  typedef TransformType::InputPointType                               InputPointType;
  typedef TransformType::OutputPointType                              OutputPointType;

  // Cast Image Filter
  typedef itk::CastImageFilter<InputImageType,
                               VectorImage<OutputPixelType> >         CastFilterType;

  // std::pair description <-> legend (image)
  typedef std::pair<std::string, InputImagePointer>                   LegendPairType;
  typedef std::vector<LegendPairType>                                 LegendVectorType;

  /** Dimension of input image. */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      InputImageType::ImageDimension);

  using Superclass::SetInput;
  virtual void SetInput( const InputImageType *image);
  virtual void SetInput( unsigned int, const TInputImage * image);
  const InputImageType * GetInput(void);
  const InputImageType * GetInput(unsigned int idx);

  itkSetStringMacro(Path);

  /** Update Method */
  virtual void Update()
  {
    this->Write();
  }

  /* Method to Add Logo */
  itkSetMacro(Logo, InputImagePointer);

  /** Method to store the legend and their description in the
    * legendVector
    */
  void AddLegend(const std::string& description, const InputImagePointer legend)
  {
    LegendPairType   legendPair;
    legendPair.first  = description;
    legendPair.second = legend;
    m_LegendVector.push_back(legendPair);
  }

  /** Method to add legend with empty description */
  void AddLegend(const InputImagePointer legend)
  {
    std::string emptyString ="";
    this->AddLegend(emptyString, legend);
  }

  /** Set the size of the tiles to produce */
  void SetTileSize(unsigned int tileSize);

protected:
  KmzProductWriter();
  virtual ~KmzProductWriter();
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /**Method for Tiling the input image*/
  virtual void Tiling();

  /** Method for writing */
  virtual void Write();

private:
  KmzProductWriter(const Self &); //purposely not implemented
  void operator =(const Self&);  //purposely not implemented

   /** Mehtod to initialize the variables*/
   virtual void Initialize();

  /**
     * Close the root kml by adding the last flags and close the
     * std::ofstream
     */
  void  CloseRootKML();

  /** method to  generate the bounding box kml*/
  void BoundingBoxKmlProcess(double north, double south, double east, double west);

   /** Method to generate the root kml*/
   void RootKmlProcess(double north, double south, double east, double west);

  /** Add Roi to kmz*/
//   void RegionOfInterestProcess();

  // Add Legends to the root kml
  void AddLegendToRootKml(double north, double south, double east, double west);

  /** KML root generate */
  void GenerateKMLRoot(const std::string& title, double north, double south, double east, double west, bool extended);

  /** KML generate  Filename - PathName - tile number - North - South - East - West */
  void GenerateKML(const std::string& pathname, int depth, int x, int y, double north, double south, double east, double west);

  void GenerateKMLExtended(const std::string& pathname,
                           int depth,
                           int x,
                           int y,
                           OutputPointType lowerLeft,
                           OutputPointType lowerRight,
                           OutputPointType upperRight,
                           OutputPointType upperLeft);

  /** KML with link generate */
  void GenerateKMLWithLink(const std::string& pathname,
                           int depth, int x, int y, int tileStartX, int tileStartY,
                           double north, double south, double east, double west,
                           double centerLong, double centerLat);
  void GenerateKMLExtendedWithLink(const std::string& pathname,
                                   int depth, int x, int y, int tileStartX, int tileStartY,
                                   OutputPointType lowerLeft, OutputPointType lowerRight,
                                   OutputPointType upperRight, OutputPointType upperLeft,
                                   double centerLong, double centerLat);

  /** Method to create the bounding kml of the "iteration" th product */
  void GenerateBoundingKML(double north, double south,
                           double east,  double west);


  /**
    * Add a networkLink <NetworkLink> ....  </NetworkLink>
    * to root kml (used to handle several image in the same kmz)
    */
  void AddNetworkLinkToRootKML(double north,
                               double south,
                               double east,
                               double west,
                               const std::string& directory,
                               bool addRegion,
                               unsigned int pos);

  /**
   * Add file to KMZ : return the number of error when writing the
   * file in the kmz
   */
  virtual int AddFileToKMZ(const std::ostringstream&  absolutePath, const std::ostringstream&   kmz_in_path);

  /**Cut the image file name to built the directory name*/
  std::string GetCuttenFileName(const std::string& description, unsigned int idx);

  /** Method to add logo if any */
  void AddLogo();

  /** Method to add Legend if any */
  void ProcessLegends();

  std::string          m_Path;
  bool                 m_UseExtendMode;
  InputImagePointer    m_VectorImage;
  InputImagePointer    m_ResampleVectorImage;

    // Extract ROI
  typename VectorImageExtractROIFilterType::Pointer m_VectorImageExtractROIFilter;

  // Writer
  typename VectorWriterType::Pointer                m_VectorWriter;

  // Resampler
  typename StreamingShrinkImageFilterType::Pointer m_StreamingShrinkImageFilter;

  // Rescale intensity
  typename VectorRescaleIntensityImageFilterType::Pointer m_VectorRescaleIntensityImageFilter;

  // Transformer
  typename TransformType::Pointer                  m_Transform;

  // VectorData Pointer Type for tileindex shapefile
  typename VectorDataType::Pointer                 m_VectorDataIndexTile;
  typename DataNodeType::Pointer                   m_Polygon;
  typename DataNodeType::Pointer                   m_MultiPolygon;
  typename DataNodeType::Pointer                   m_Folder;

  // Tile size
  unsigned int          m_TileSize;
  int                    m_MaxDepth;
  int                    m_CurrentDepth;
  unsigned int           m_CurIdx;

  // KMZ file

  kmlengine::KmzFilePtr  m_KmzFile;

  // KMZ file name
  std::ostringstream     m_KmzFileName;

  // the kml root ofstream
  std::ofstream          m_RootKmlFile;
  std::ofstream          m_TempRootKmlFile;

    // File and path name
  std::string            m_FileName;
  std::string            m_CurrentImageName;

  // Convenience string
  std::string            m_KmzExtension;
  std::string            m_KmlExtension;

  // Logo
  InputImagePointer      m_Logo;

  // Vector used to store legend and description
  LegendVectorType       m_LegendVector;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbKmzProductWriter.txx"
#endif

#endif
