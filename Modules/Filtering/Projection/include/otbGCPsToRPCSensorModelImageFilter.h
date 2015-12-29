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
#ifndef __otbGCPsToRPCSensorModelImageFilter_h
#define __otbGCPsToRPCSensorModelImageFilter_h

#include "itkUnaryFunctorImageFilter.h"
#include "itkCastImageFilter.h"
#include "otbDEMHandler.h"

#include "otbImageKeywordlist.h"

namespace otb {

/** \class GCPsToRPCSensorModelImageFilter
 * \brief This filter estimates a RPC sensor models from GCPs.
 *
 * This filters estimates an RPC sensor model from a list of user
 * defined GCPs. Internally, it uses an ossimRpcSolver, which performs
 * the estimation using the well known least-square method.
 *
 * The UseImageGCPs flag allows importing GCPs from the image
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
 * metadata. Therefore, it provides in-place support, which is
 * enabled by default. Call InPlaceOff() to change the default
 * behavior.
 *
 * The output image can be given to the OrthorectificationFilter.
 *
 *
 * \ingroup OTBProjection
 */
template <class TImage>
class ITK_EXPORT GCPsToRPCSensorModelImageFilter :
  public itk::CastImageFilter<TImage, TImage>
{
public:
  /** Standard class typedefs. */
  typedef GCPsToRPCSensorModelImageFilter      Self;
  typedef itk::CastImageFilter<TImage, TImage> Superclass;
  typedef itk::SmartPointer<Self>              Pointer;
  typedef itk::SmartPointer<const Self>        ConstPointer;

  /** GCPs typedefs */
  typedef itk::Point<double, 2>               Point2DType;
  typedef itk::Point<double, 3>               Point3DType;
  typedef std::pair<Point2DType, Point3DType> GCPType;
  typedef std::vector<GCPType>                GCPsContainerType;
  typedef std::vector<double>                 ErrorsContainerType;

  typedef itk::ContinuousIndex<>          ContinuousIndexType;
  typedef itk::ContinuousIndex<double, 3> Continuous3DIndexType;

  /** DEM typedef */
  typedef otb::DEMHandler                  DEMHandlerType;
  typedef typename DEMHandlerType::Pointer DEMHandlerPointerType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(GCPsToRPCSensorModelImageFilter, CastImageFilter);

  /** Extract dimension from input and output image. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TImage::ImageDimension);

  /** Set/Get/toogle the UseImageGCPs flag */
//  itkSetMacro(UseImageGCPs, bool);
// itkGetMacro(UseImageGCPs, bool);
//  itkBooleanMacro(UseImageGCPs);
  void SetUseImageGCPs(bool use);
  void LoadImageGCPs();

  /** Set/Get/toogle the UseDEM flag */
  itkSetMacro(UseDEM, bool);
  itkGetMacro(UseDEM, bool);
  itkBooleanMacro(UseDEM);

  /** Set/Get the mean elevation */
  itkSetMacro(MeanElevation, double);
  itkGetConstReferenceMacro(MeanElevation, double);

  /** Set/Get the DEMHandler */
  itkSetObjectMacro(DEMHandler, DEMHandlerType);
  itkGetObjectMacro(DEMHandler, DEMHandlerType);

  /** Get the residual ground error */
  itkGetConstReferenceMacro(RMSGroundError, double);

  /** Get the Error container */
  ErrorsContainerType& GetErrorsContainer();

  /** Get the mean error */
  itkGetConstReferenceMacro(MeanError, double);

  /** Get the GCPsContainer
   * \return The GCPs container */
  GCPsContainerType& GetGCPsContainer();

  /** Set the GCP container */
  void SetGCPsContainer(const GCPsContainerType& container);

  /** Get Keywordlist */
  itkGetConstReferenceMacro(Keywordlist, ImageKeywordlist);

  /** Add a GCP to the GCPContainer. This version of the AddGCP method
   * accepts a 3D ground point and does not use DEM or MeanElevation
   * to handle the elevation */
  void AddGCP(const Point2DType& sensorPoint, const Point3DType& groundPoint);

  /** Add A GCP to the GCPContainer. This version of the AddGCP method
   * accepts a 3D ground point and use the DEM or MeanElevation to get
   * the corresponding elevation before calling the AddGCP acception 3D
   * ground points.*/
  void AddGCP(const Point2DType& sensorPoint, const Point2DType& groundPoint);

  /** Remove a GCP given by it's coordinates */
  void RemoveGCP(unsigned int id);

  /** Clear all GCPs */
  void ClearGCPs();

protected:
  /** Constructor */
  GCPsToRPCSensorModelImageFilter();
  /** Destructor */
  virtual ~GCPsToRPCSensorModelImageFilter();

  /** The PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** Actual estimation of the sensor model takes place in the
   * GenerateOutputInformation() method */
  virtual void GenerateOutputInformation();

  /** Drive the model up-to-date flag */
  virtual void Modified() const;

private:
  GCPsToRPCSensorModelImageFilter (const Self &);   // purposely not implemented
  void operator =(const Self&);    // purposely not implemented

  /** Transform all GCPs and compute the error and mean error */
  void ComputeErrors();

  /** True to use GCPs from image metadata as well */
  bool m_UseImageGCPs;

  /** The residual ground error */
  double m_RMSGroundError;

  /** The GCP error list */
  ErrorsContainerType m_ErrorsContainer;

  /** The mean error */
  double m_MeanError;

  /** True if a DEM should be used */
  bool m_UseDEM;

  /** If no DEM is used, a MeanElevation
   * over the image is used instead */
  double m_MeanElevation;

  /** The DEMHandler */
  DEMHandlerPointerType m_DEMHandler;

  /** Container of GCPs */
  GCPsContainerType m_GCPsContainer;

  /** Keywordlist */
  ImageKeywordlist m_Keywordlist;

  /** Flag to see if model is up-to-date */
  mutable bool m_ModelUpToDate;

}; // end of class

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbGCPsToRPCSensorModelImageFilter.txx"
#endif

#endif
