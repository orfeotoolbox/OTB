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
#ifndef __otbDisparityMapToDEMFilter_h
#define __otbDisparityMapToDEMFilter_h

#include "itkImageToImageFilter.h"
#include "otbGenericRSTransform.h"
#include "itkImageRegionSplitter.h"
#include "otbVectorImage.h"
#include "otbImage.h"

namespace otb
{

/** \class DisparityMapToDEMFilter
 *  \brief Project an input disparity map into a regular DEM
 *
 *  This filter uses an input disparity map (horizontal and vertical) to produce a DEM with a regular sampling
 *  in the chosen map projection system. The elevation is computed from the triangulation of the "left-right" pairs
 *  of pixels matched. When several elevations are possible on a DEM cell, the highest is kept. Note : disparity maps
 *  and DEM are expected to be of scalar image type.
 *
 *  The inputs are:
 *    - the horizontal and vertical disparity maps (SetHorizontalDisparityMapInput, SetVerticalDisparityMapInput)
 *    - the original image pair (in sensor geometry) (SetLeftInput, SetRightInput)
 *    - the deformation grids of the epipolar geometry (SetLeftEpipolarGridInput, SetRightEpipolarGridInput)
 *    - the mask associated to the input disparity maps (SetDisparityMaskInput)
 *    [- the DEM map projection system]
 *
 *  The outputs are:
 *    - the DEM (GetDEMOutput)
 *
 *  \sa FineRegistrationImageFilter
 *  \sa StereorectificationDisplacementFieldSource
 *  \sa SubPixelDisparityImageFilter
 *  \sa PixelWiseBlockMatchingImageFilter
 *
 *  \ingroup Streamed
 *  \ingroup Threaded
 *
 *
 * \ingroup OTBDisparityMap
 */
template <class TDisparityImage, class TInputImage, class TOutputDEMImage = TDisparityImage,
          class TEpipolarGridImage = otb::VectorImage<float,2> , class TMaskImage = otb::Image<unsigned char> >
class ITK_EXPORT DisparityMapToDEMFilter :
    public itk::ImageToImageFilter<TDisparityImage,TOutputDEMImage>
{
public:
  /** Standard class typedef */
  typedef DisparityMapToDEMFilter                           Self;
  typedef itk::ImageToImageFilter<TDisparityImage,
                                  TOutputDEMImage>          Superclass;
  typedef itk::SmartPointer<Self>                           Pointer;
  typedef itk::SmartPointer<const Self>                     ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(DisparityMapToDEMFilter, ImageToImageFilter);

  /** Usefull typedefs */
  typedef TDisparityImage         DisparityMapType;
  typedef TInputImage             SensorImageType;
  typedef TOutputDEMImage         DEMImageType;
  typedef TEpipolarGridImage      GridImageType;
  typedef TMaskImage              MaskImageType;

  typedef typename DEMImageType::RegionType         RegionType;
  typedef typename DEMImageType::PixelType          DEMPixelType;

  typedef itk::ImageRegionSplitter<2>   SplitterType;

  // 3D RS transform
  // TODO: Allow tuning precision (i.e. double or float)
  typedef otb::GenericRSTransform<double,3,3>       RSTransformType;
  //typedef typename RSTransformType::Pointer         RSTransformPointerType;

  // 3D points
  typedef typename RSTransformType::InputPointType  TDPointType;

  /** Set horizontal disparity map input */
  void SetHorizontalDisparityMapInput( const TDisparityImage * hmap);

  /** Set vertical disparity map input */
  void SetVerticalDisparityMapInput( const TDisparityImage * vmap);

  /** Set left input */
  void SetLeftInput( const TInputImage * image);

  /** Set right input */
  void SetRightInput( const TInputImage * image);

  /** Set left epipolar grid (deformation grid from sensor image to epipolar space, regular in epipolar space)*/
  void SetLeftEpipolarGridInput( const TEpipolarGridImage * grid);

  /** Set right epipolar grid (deformation grid from sensor image to epipolar space, regular in epipolar space)*/
  void SetRightEpipolarGridInput( const TEpipolarGridImage * grid);

  /** Set mask associated to disparity maps (optional, pixels with a null mask value are ignored) */
  void SetDisparityMaskInput( const TMaskImage * mask);

  /** Get the inputs */
  const TDisparityImage * GetHorizontalDisparityMapInput() const;
  const TDisparityImage * GetVerticalDisparityMapInput() const;
  const TInputImage * GetLeftInput() const;
  const TInputImage * GetRightInput() const;
  const TEpipolarGridImage * GetLeftEpipolarGridInput() const;
  const TEpipolarGridImage * GetRightEpipolarGridInput() const;
  const TMaskImage  * GetDisparityMaskInput() const;

  /** Get DEM output*/
  const TOutputDEMImage * GetDEMOutput() const;
  TOutputDEMImage * GetDEMOutput();

  /** Set/Get macro for minimum elevation */
  itkSetMacro(ElevationMin, double);
  itkGetConstReferenceMacro(ElevationMin, double);

  /** Set/Get macro for maximum elevation */
  itkSetMacro(ElevationMax, double);
  itkGetConstReferenceMacro(ElevationMax, double);

  /** Set/Get macro for DEM grid step */
  itkSetMacro(DEMGridStep, double);
  itkGetConstReferenceMacro(DEMGridStep, double);

protected:
  /** Constructor */
  DisparityMapToDEMFilter();

  /** Destructor */
  virtual ~DisparityMapToDEMFilter();

  /** Generate output information */
  virtual void GenerateOutputInformation();

  /** Generate input requrested region */
  virtual void GenerateInputRequestedRegion();

  /** Before threaded generate data */
  virtual void BeforeThreadedGenerateData();

  /** Threaded generate data */
  virtual void ThreadedGenerateData(const RegionType & outputRegionForThread, itk::ThreadIdType threadId);

  /** After threaded generate data : sum up temporary DEMs */
  virtual void AfterThreadedGenerateData();

  /** Override VerifyInputInformation() since this filter's inputs do
    * not need to occupy the same physical space.
    *
    * \sa ProcessObject::VerifyInputInformation
    */
  virtual void VerifyInputInformation() {}


private:
  DisparityMapToDEMFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** Minimum elevation of the DEM in meters */
  double m_ElevationMin;

  /** Maximum elevation of the DEM in meters */
  double m_ElevationMax;

  /** DEM grid step (in meters) */
  double m_DEMGridStep;

  /** Region splitter for input disparity maps */
  SplitterType::Pointer m_InputSplitter;

  /** Number of splits used for input multithreading */
  unsigned int m_UsedInputSplits;

  /** Temporary DEMs for mutlithreading */
  std::vector<typename DEMImageType::Pointer> m_TempDEMRegions;
  
  /** Left sensor image transform */
  RSTransformType::Pointer m_LeftToGroundTransform;
  
  /** Right sensor image transform */
  RSTransformType::Pointer m_RightToGroundTransform;
  
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbDisparityMapToDEMFilter.txx"
#endif

#endif
