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
#ifndef __otbMulti3DMapToDEMFilter_h
#define __otbMulti3DMapToDEMFilter_h

#include "itkImageToImageFilter.h"
#include "otbImageToGenericRSOutputParameters.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkImageRegionConstIterator.h"
#include "otbVectorImage.h"
#include "otbImage.h"
#include "itkImageRegionSplitter.h"
#include "otbObjectList.h"

namespace otb
{

namespace CellFusionMode
{
enum CellFusionMode {
  MIN = 0,
  MAX = 1,
  MEAN = 2,
  ACC = 3 //return accumulator for debug purpose
  };
}


/** \class Multi3DMapToDEMFilter
 *  \brief Project N 3D images (long,lat,alti) into a regular DEM in the chosen map projection system.
 *
 *  This filter uses a group of N 3D points images and project then onto a regular DEM grid parametrized by DEMGridStep (in meter).
 *  The 3D coordinates (sorted by band) of input map  are : longitude , latitude (in degree, wrt WGS84) and altitude (in meters)
 *
 * DEM cell fusion strategy available are :
 * - 0 MIN : we keep the minimum altitude
 * - 1 MAX : we keep the maximum altitude
 * - 2 MEAN : mean is computed
 * - 3 ACC : returns cell count (useful to create mask from output)
 *
 *  empty cell are filled with the NoDataValue (-32768 by default)
 *
 *  if 3Map are not in sensor geometry Min and MaxElevation parameters are useless
 *  otherwise Min and MaxElevation are used to reproject DEM long/lat in sensor geometry to extend area search in input 3Map
 *
 *  DEM parameters are automatically deduced by the union of input extent if SetOutputParametersFrom3DMap is called without parameters. If
 *  an index is given, the 3D corresponding Map is used to deduced Output parameters (expect Spacing which is calculated for DEMGridStep (in meter))
 *
 *  DEM output parameters can also be manually filled using Setters for :
 *  Origin, Spacing, Size, StartIndex, ProjectionRef
 *  thus DEMGridStep parameter is ignored in this case (replaced by Spacing)
 *
 *  \sa FineRegistrationImageFilter
 *  \sa MultiDisparityMapTo3DFilter
 *
 *  \ingroup Streamed
 *  \ingroup Threaded
 *
 *
 * \ingroup OTBStereo
 */
template <class T3DImage =  otb::VectorImage<double,2>,
          class TMaskImage = otb::Image<unsigned char>, class TOutputDEMImage = otb::Image<double> >
class ITK_EXPORT Multi3DMapToDEMFilter :
    public itk::ImageToImageFilter<T3DImage,TOutputDEMImage>
{
public:
  /** Standard class typedef */
  typedef Multi3DMapToDEMFilter                       Self;
  typedef itk::ImageToImageFilter<T3DImage,
                                  TOutputDEMImage>             Superclass;
  typedef itk::SmartPointer<Self>                           Pointer;
  typedef itk::SmartPointer<const Self>                     ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(Multi3DMapToDEMFilter, ImageToImageFilter);

  /** Usefull typedefs */
  typedef T3DImage                InputMapType;
  typedef TOutputDEMImage         OutputImageType;
  typedef TMaskImage              MaskImageType;
  typedef otb::Image<unsigned int>                   AccumulatorImageType;

  typedef typename OutputImageType::RegionType         RegionType;
  typedef typename OutputImageType::PixelType          DEMPixelType;
  typedef typename OutputImageType::PointType          OriginType;
  typedef typename OutputImageType::SpacingType        SpacingType;
  typedef typename OutputImageType::SizeType           SizeType;
  typedef typename OutputImageType::IndexType          IndexType;

  typedef typename AccumulatorImageType::PixelType     AccumulatorPixelType;

  typedef typename InputMapType::PixelType             MapPixelType;
  typedef typename InputMapType::InternalPixelType   InputInternalPixelType;
  // 3D RS transform
  // TODO: Allow tuning precision (i.e. double or float)
  typedef double                  PrecisionType;
  typedef otb::GenericRSTransform
    <PrecisionType,3,3>           RSTransformType;

  typedef otb::ImageToGenericRSOutputParameters<OutputImageType> OutputParametersEstimatorType;

  // 3D points
  typedef typename RSTransformType::InputPointType  TDPointType;

  // 2D Transform
  typedef otb::GenericRSTransform<>                 RSTransform2DType;

  typedef otb::ImageKeywordlist                     ImageKeywordListType;

  typedef std::map
    <unsigned int,
     itk::ImageRegionConstIterator<InputMapType> >   MapIteratorList;

  typedef std::map
    <unsigned int,
     itk::ImageRegionConstIterator<MaskImageType> >      MaskIteratorList;


  typedef double ValueType;
  typedef itk::VariableLengthVector<ValueType> MeasurementType;

  typedef itk::ImageRegionSplitter<2>   SplitterType;
  typedef otb::ObjectList<SplitterType>      SplitterListType;

  /** Set the number of 3D images (referred earlier as N) */
  void SetNumberOf3DMaps(unsigned int nb);

  /** Get the number of 3D images (referred earlier as N) */
  unsigned int GetNumberOf3DMaps();

  /** Set 3D map input at  corresponding 'index' */
  void Set3DMapInput(unsigned int index, const T3DImage * hmap);

  /** Set mask associated to 3D maps  'index'
   * (optional, pixels with a null mask value are ignored)
   */
  void SetMaskInput(unsigned int index, const TMaskImage * mask);

  /** Get the inputs */
  const T3DImage * Get3DMapInput(unsigned int index) const;
  const TMaskImage  * GetMaskInput(unsigned int index) const;

  /** Get DEM output*/
   const TOutputDEMImage * GetDEMOutput() const;
   TOutputDEMImage * GetDEMOutput();

   /** Set/Get macro for DEM grid step */
   itkSetMacro(DEMGridStep, double);
   itkGetConstReferenceMacro(DEMGridStep, double);

   /** Set/Get macro for CellFusionMode */
   itkSetMacro(CellFusionMode, int);
   itkGetConstReferenceMacro(CellFusionMode, int);

   /** Set/Get macro for NoDataValue */
   itkSetMacro(NoDataValue, DEMPixelType);
   itkGetConstReferenceMacro(NoDataValue, DEMPixelType);

   /** compute
    *  Set DEM extent using 3DMap 'index' , if index =-1 union of all input Map extent is done
     */
    void SetOutputParametersFrom3DMap(int index=-1)
    {
      if (static_cast<unsigned int>((2 * (index + 1))) > this->GetNumberOfInputs())
          {
           itkExceptionMacro(<< "input at position "<<index<<" is unavailable");
          }
      m_OutputParametersFrom3DMap=index;
    }


    itkSetMacro(OutputOrigin, OriginType);
    itkGetConstReferenceMacro(OutputOrigin, OriginType);

    /** Output Start index */
    itkSetMacro(OutputStartIndex, IndexType);
    itkGetConstReferenceMacro(OutputStartIndex, IndexType);

    /** Output Size */
    itkSetMacro(OutputSize, SizeType);
    itkGetConstReferenceMacro(OutputSize, SizeType);

    /** Output Spacing */
    itkSetMacro(OutputSpacing, SpacingType);
    itkGetConstReferenceMacro(OutputSpacing, SpacingType);

    /** Projection REf */
    itkSetMacro(ProjectionRef, std::string);
    itkGetConstReferenceMacro(ProjectionRef, std::string);

    itkGetConstReferenceMacro(OutputParametersFrom3DMap, int);


    /** Set/Get macro for minimum elevation */
    itkSetMacro(ElevationMin, double);
    itkGetConstReferenceMacro(ElevationMin, double);

    /** Set/Get macro for maximum elevation */
   itkSetMacro(ElevationMax, double);
   itkGetConstReferenceMacro(ElevationMax, double);


     /** margin for input requested region size  */
    itkSetMacro(Margin, SizeType);
    itkGetConstReferenceMacro(Margin, SizeType);


protected:
  /** Constructor */
  Multi3DMapToDEMFilter();

  /** Destructor */
  virtual ~Multi3DMapToDEMFilter();

  /** Generate output information */
  virtual void GenerateOutputInformation();

  /** Generate input requrested region */
  virtual void GenerateInputRequestedRegion();

  /** Before threaded generate data */
  virtual void BeforeThreadedGenerateData();

  /** Threaded generate data */
  virtual void ThreadedGenerateData(const RegionType & outputRegionForThread, itk::ThreadIdType threadId);

  /** After threaded generate data */
  virtual void AfterThreadedGenerateData();

  /** Override VerifyInputInformation() since this filter's inputs do
    * not need to occupy the same physical space.
    *
    * \sa ProcessObject::VerifyInputInformation
    */
  virtual void VerifyInputInformation() {}


private:

  void SetOutputParametersFromImage();

  Multi3DMapToDEMFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** Keywordlist of each map */
 // std::vector<ImageKeywordListType> m_MapKeywordLists;


  /** Reference sensor image transform */
  RSTransformType::Pointer m_ReferenceToGroundTransform;

  /** Moving sensor image transforms */
  std::vector<RSTransformType::Pointer> m_MapToGroundTransform;

  std::vector<MeasurementType> m_MapMinVal;
  std::vector<MeasurementType> m_MapMaxVal;
  /** DEM grid step (in meters) */
  double m_DEMGridStep;

  /** Temporary DEMs for multithreading */
   std::vector<typename OutputImageType::Pointer>      m_TempDEMRegions;
   /** Temporary accumulator for multithreading and mean calculus*/
   std::vector<typename AccumulatorImageType::Pointer> m_TempDEMAccumulatorRegions;


  std::vector<unsigned int> m_NumberOfSplit; // number of split for each map
  // std::vector<int> m_ThreadProcessed; //
  /** Region splitter for input disparity maps */
  SplitterListType::Pointer m_MapSplitterList;

  DEMPixelType              m_NoDataValue;
  int                       m_CellFusionMode;
  std::string               m_ProjectionRef;

  /** Minimum elevation of the DEM in meters */
   double m_ElevationMin;

   /** Maximum elevation of the DEM in meters */
   double m_ElevationMax;


  SizeType      m_OutputSize;
  IndexType     m_OutputStartIndex;
  SpacingType   m_OutputSpacing;
  OriginType    m_OutputOrigin;

  SizeType      m_Margin;

  int           m_OutputParametersFrom3DMap;
  bool          m_IsGeographic;
  
  /** internal transform between WGS84 and user's ProjRef */
  RSTransform2DType::Pointer m_GroundTransform;

};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMulti3DMapToDEMFilter.txx"
#endif

#endif
