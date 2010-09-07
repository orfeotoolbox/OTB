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
#ifndef __otbPhysicalToRPCSensorModelImageFilter_h
#define __otbPhysicalToRPCSensorModelImageFilter_h

#include "itkInPlaceImageFilter.h"
#include "otbGCPsToRPCSensorModelImageFilter.h"
#include "otbGenericRSTransform.h"


namespace otb {

/** \class PhysicalToRPCSensorModelImageFilter
 * \brief This filter estimates a RPC sensor models from a physical model.
 *
 * This filters estimates an RPC sensor model from a list of user
 * defined GCPs. Internally, it uses an ossimRpcSolver, which performs
 * the estimation using the well known least-square method.
 *
 * The UseImageGCPs flag allows to import GCPs from the image
 * metadata, if any.
 *
 * GCPs can be passed to the filter using one of the AddGCP method
 * implementation.
 *
 * The first implementation takes geographic points with elevation (3D
 * points).
 *
 * The second implementation accepts geographic points without the
 * elevation information. In this case, either the mean elevation or
 * an elevation fetch from a SRT directory is used, depending on the
 * value of the UseDEM flag.
 *
 * If UseDEM is set to true, the DEMHandler is used to retrieve the
 * elevation information. The user can either set its own DEMHandler
 * using the appropriate setter, or configure the existing internal
 * DEMHandler using the Getter.
 *
 * The RMS (root mean square) ground error is available through the
 * appropriate getter.
 *
 * Please note that GCPs are infered to be given in physical
 * coordinates. This is seamless in most cases.
 *
 * Please note that at least 16 GCPs are required to estimate a proper
 * RPC sensor model, although no warning will be reported to the user
 * if the number of GCPs is lower than 16.
 *
 * This filter does not modify the image buffer, but only the
 * metadata. Therefore, it is implemented as an InPlaceImageFilter.
 *
 * The output image can be given to the OrthorectificationFilter.
 *
 */
template <class TImage>
class ITK_EXPORT PhysicalToRPCSensorModelImageFilter :
  public itk::InPlaceImageFilter<TImage>
{
public:
  /** Standard class typedefs. */
  typedef PhysicalToRPCSensorModelImageFilter Self;
  typedef itk::InPlaceImageFilter<TImage> Superclass;
  typedef itk::SmartPointer<Self>         Pointer;
  typedef itk::SmartPointer<const Self>   ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(PhysicalToRPCSensorModelImageFilter, InPlaceImageFilter);

  /** template parameters typedef */
  typedef TImage                          ImageType;
  typedef typename ImageType::PointType   PointType;
  
  /** GCPsToSensor model filter */
  typedef GCPsToRPCSensorModelImageFilter<ImageType>          GCPsToSensorModelType;
  typedef typename GCPsToSensorModelType::Pointer             GCPsToSensorModelPointerType;
  typedef typename GCPsToSensorModelType::Point2DType         Point2DType;
  typedef typename GCPsToSensorModelType::Point3DType         Point3DType;
  typedef typename GCPsToSensorModelType::GCPType             GCPType;
  typedef typename GCPsToSensorModelType::GCPsContainerType   GCPsContainerType;
  typedef typename GCPsToSensorModelType::ErrorsContainerType ErrorsContainerType;

  /** Remote Sensing Transform */
  typedef GenericRSTransform<>                                RSTransformType;
  typedef typename RSTransformType::Pointer                   RSTransformPointerType;

  /** launch the process */
  virtual void GenerateOutputInformation();//GenerateData();
  
  /** Set/Get the grid size*/
  itkSetMacro(GridSpacing, unsigned int);
  itkGetMacro(GridSpacing, unsigned int);

  /** Set/Get the DEM directory*/
  itkSetStringMacro(DEMDirectory);
  itkGetStringMacro(DEMDirectory);
  
protected:
  /** Constructor */
  PhysicalToRPCSensorModelImageFilter();
  /** Destructor */
  virtual ~PhysicalToRPCSensorModelImageFilter();

  /** The PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /**  */
  virtual void ThreadedGenerateData(const typename TImage::RegionType&, int) {}
  
private:
  PhysicalToRPCSensorModelImageFilter(const Self &);   // purposely not implemented
  void operator =(const Self&);    // purposely not implemented
  
  /** The rpc model estimator */
  GCPsToSensorModelPointerType       m_GCPsToSensorModelFilter;
  
  std::string                        m_DEMDirectory;
  unsigned int                       m_GridSpacing;
};
} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPhysicalToRPCSensorModelImageFilter.txx"
#endif

#endif
