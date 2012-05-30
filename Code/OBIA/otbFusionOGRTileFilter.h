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
#ifndef __otbFusionOGRTileFilter_h
#define __otbFusionOGRTileFilter_h

#include "otbOGRDataSourceWrapper.h"
#include "otbMacro.h"

//#if(GDAL_VERSION_NUM < 1800)
//#error FusionOGRTileFilter requires GDAL version >= 1.8.0
//#endif

#include "itkProcessObject.h"

#include <algorithm>

namespace otb
{

/** \class FusionOGRTileFilter
 *  \brief This filter fusion the geometries in a layer (ogr) along streaming lines.
 *  The SetStreamSize() method allows to retrieve the number of streams in row and column,
 *  and their pixel coordinates.
 *  The input image is used to transform pixel coordinates of the streaming lines into
 *  coordinate system of the image, which must be the same as the one in the OGR input file.
 *  The input OGR file is updated with the fusionned polygons.
 *
 *
 *  \ingroup OBIA
 *
 *
 */
template <class TInputImage>
class ITK_EXPORT FusionOGRTileFilter :
    public itk::ProcessObject
{
public:

   /** typedef for the classes standards. */
  typedef FusionOGRTileFilter                 Self;
  typedef itk::ProcessObject                              Superclass;
  typedef itk::SmartPointer<Self>                         Pointer;
  typedef itk::SmartPointer<const Self>                   ConstPointer;
  
  /** Definition of the input image */
  typedef TInputImage                           InputImageType;
  typedef typename InputImageType::PixelType    InputPixelType;
  typedef typename InputImageType::IndexType    InputIndexType;
  typedef typename InputImageType::SizeType     SizeType;
  typedef typename InputImageType::RegionType   RegionType;
  typedef typename InputImageType::SpacingType  SpacingType;
  typedef typename InputImageType::PointType    OriginType;
  typedef typename InputImageType::IndexType    IndexType;
  
  typedef ogr::DataSource                            OGRDataSourceType;
  typedef typename OGRDataSourceType::Pointer        OGRDataSourcePointerType;
  typedef ogr::Layer                                 OGRLayerType;
  typedef ogr::Feature                               OGRFeatureType;

  
  /** Set/Get the input image of this process object.  */
  virtual void SetInput(const InputImageType *input);
  virtual const InputImageType * GetInput(void);
  
  /** Method for management of the object factory. */
  itkNewMacro(Self);

  /** Return the name of the class. */
  itkTypeMacro(FusionOGRTileFilter, ProcessObject);
  
  /** Set/Get the input OGRDataSource */
  void SetOGRDataSource( OGRDataSourcePointerType ogrDS );
  OGRDataSourceType * GetOGRDataSource( void );
  
  /** Set/Get the size of the stream */
  itkSetMacro(StreamSize, SizeType);
  itkGetMacro(StreamSize, SizeType);
  
  /** Set/Get the name of the output layer to write in the input ogrDataSource. */
  itkSetStringMacro(LayerName);
  itkGetStringMacro(LayerName);
  
  /** Generate Data method*/
  virtual void GenerateData();
  
protected:
  FusionOGRTileFilter();
  virtual ~FusionOGRTileFilter() {}
  
  struct FusionStruct
  {
     unsigned int indStream1;
     unsigned int indStream2;
     double overlap;
  };
  struct FeatureStruct
  {
     FeatureStruct(OGRFeatureDefn & defn) : feat(defn), fusioned(false)
     {
     }
     OGRFeatureType feat;
     bool fusioned;
  };
  struct SortFeatureStruct
  {
     bool operator() (FusionStruct f1, FusionStruct f2) { return (f1.overlap > f2.overlap); }
  } SortFeature;
  
  void ProcessStreamingLine(bool line);
  /** get length in case of  OGRGeometryCollection.
   * This function recodes the get_lenght method available since gdal 1.8.0
   * in the case of OGRGeometryCollection. The aim is allow to access polygon stiching
   * functionalities with gdal 1.6.
   */
  double GetLengthOGRGeometryCollection(OGRGeometryCollection * intersection);

private:
  FusionOGRTileFilter(const Self &);  //purposely not implemented
  void operator =(const Self&);      //purposely not implemented
  
  SizeType m_StreamSize;
  unsigned int m_Radius;
  std::string m_LayerName;


};


} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbFusionOGRTileFilter.txx"
#endif

#endif
