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
#ifndef __otbOGRLayerStreamStitchingFilter_h
#define __otbOGRLayerStreamStitchingFilter_h

#include "otbOGRDataSourceWrapper.h"
#include "otbMacro.h"

//#if(GDAL_VERSION_NUM < 1800)
//#error OGRLayerStreamStitchingFilter requires GDAL version >= 1.8.0
//#endif

#include "itkProgressReporter.h"

#include <algorithm>

namespace otb
{

/** \class OGRLayerStreamStitchingFilter
 *  \brief This filter fusion the geometries in a layer (\c OGRLayer) along streaming lines.
 *  It is a in-line filter which means that the result of the fusion overwrites the input layer.
 *  The strategy for merging polygons is quite simple. A polygon P1 is merge with a polygon P2 if:
 *  - P1 and P2 are on different side of the streaming line
 *  - P1 and P2 intersect each other.
 *  - P2 has the largest intersection with P1 among all other polygons Pi intersecting P1.
 *  The \c SetStreamSize() method allows retrieving the number of streams in row and column,
 *  and their pixel coordinates.
 *  The input image is used to transform pixel coordinates of the streaming lines into
 *  coordinate system of the image, which must be the same as the one in the OGR input file.
 *  This filter is intended to be used after \c StreamingVectorizedSegmentationOGR.
 *  @see Example/StreamingMeanShiftSegmentation.cxx
 *
 *  \ingroup OBIA
 *
 *
 *
 * \ingroup OTBOGRProcessing
 */
template <class TInputImage>
class ITK_EXPORT OGRLayerStreamStitchingFilter :
    public itk::ProcessObject
{
public:

   /** typedef for the classes standards. */
  typedef OGRLayerStreamStitchingFilter        Self;
  typedef itk::ProcessObject                   Superclass;
  typedef itk::SmartPointer<Self>              Pointer;
  typedef itk::SmartPointer<const Self>        ConstPointer;

  /** Definition of the input image */
  typedef TInputImage                          InputImageType;
  typedef typename InputImageType::PixelType   InputPixelType;
  typedef typename InputImageType::IndexType   InputIndexType;
  typedef typename InputImageType::SizeType    SizeType;
  typedef typename InputImageType::RegionType  RegionType;
  typedef typename InputImageType::SpacingType SpacingType;
  typedef typename InputImageType::PointType   OriginType;
  typedef typename InputImageType::IndexType   IndexType;

  typedef ogr::Layer                           OGRLayerType;
  typedef ogr::Feature                         OGRFeatureType;

  /** Set the input image of this process object.  */
  using Superclass::SetInput;
  virtual void SetInput(const InputImageType *input);
  /** Get the input image. */
  virtual const InputImageType * GetInput(void);

  /** Method for management of the object factory. */
  itkNewMacro(Self);

  /** Return the name of the class. */
  itkTypeMacro(OGRLayerStreamStitchingFilter, ProcessObject);

  /** Set the input OGRLayer */
  void SetOGRLayer( const OGRLayerType & ogrLayer );
  /** Get the input OGRLayer */
  const OGRLayerType & GetOGRLayer( void ) const;

  /** Set the stream size.
   * As this filter is intended to be used right after the \c StreamingVectorizedSegmentation,
   * use the \c GetStreamSize() method on it to get the correct stream size.
   */
  itkSetMacro(StreamSize, SizeType);
  /** Get stream size*/
  itkGetMacro(StreamSize, SizeType);

  /** Generate Data method. This method must be called explicitly (not through the \c Update method). */
  virtual void GenerateData();

protected:
  OGRLayerStreamStitchingFilter();
  virtual ~OGRLayerStreamStitchingFilter() {}

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

  /**
   Main computation method. if line is true process row part, else process column part.
   */
  void ProcessStreamingLine(bool line, itk::ProgressReporter &progress);
  /** get length in case of  OGRGeometryCollection.
   * This function recodes the get_lenght method available since gdal 1.8.0
   * in the case of OGRGeometryCollection. The aim is to allow accessing polygon stiching
   * functionalities with gdal 1.6.
   */
  double GetLengthOGRGeometryCollection(OGRGeometryCollection * intersection);

private:
  OGRLayerStreamStitchingFilter(const Self &);  //purposely not implemented
  void operator =(const Self&);      //purposely not implemented

  SizeType m_StreamSize;
  unsigned int m_Radius;
  OGRLayerType m_OGRLayer;


};


} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbOGRLayerStreamStitchingFilter.txx"
#endif

#endif
