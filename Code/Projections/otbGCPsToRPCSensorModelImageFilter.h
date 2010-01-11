/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) Institut Telecom / Telecom Bretagne. All rights reserved.
  See ITCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbGCPsToRPCSensorModelImageFilter_h
#define __otbGCPsToRPCSensorModelImageFilter_h

#include "itkInPlaceImageFilter.h"
#include "otbDEMHandler.h"

namespace otb {

/** \class GCPsToRPCSensorModelImageFilter
 * \brief 
 *
 */
template < class TImage>
class ITK_EXPORT GCPsToRPCSensorModelImageFilter :
  public itk::InPlaceImageFilter< TImage >
{
public:
  /** Standard class typedefs. */
  typedef GCPsToRPCSensorModelImageFilter    Self;
  typedef itk::InPlaceImageFilter< TImage >  Superclass;
  typedef itk::SmartPointer<Self>            Pointer;
  typedef itk::SmartPointer<const Self>      ConstPointer;

  /** GCPs typedefs */
  typedef itk::Point<double,2>               Point2DType;
  typedef itk::Point<double,3>               Point3DType;
  typedef std::pair<Point2DType,Point3DType> GCPType;
  typedef std::vector<GCPType>               GCPsContainerType;

  /** DEM typedef */
  typedef otb::DEMHandler                    DEMHandlerType;
  typedef typename DEMHandlerType::Pointer   DEMHandlerPointerType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(GCPsToRPCSensorModelImageFilter, InPlaceImageFilter);

  /** Extract dimension from input and output image. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TImage::ImageDimension);

  /** Set/Get/toogle the UseImageGCPs flag */
  itkSetMacro(UseImageGCPs,bool);
  itkGetMacro(UseImageGCPs,bool);
  itkBooleanMacro(UseImageGCPs);

  /** Set/Get/toogle the UseDEM flag */
  itkSetMacro(UseDEM,bool);
  itkGetMacro(UseDEM,bool);
  itkBooleanMacro(UseDEM);

  /** Set/Get the mean elevation */
  itkSetMacro(MeanElevation,double);
  itkGetConstReferenceMacro(MeanElevation,double);

  /** Set/Get the DEMHandler */
  itkSetObjectMacro(DEMHandler,DEMHandlerType);
  itkGetObjectMacro(DEMHandler,DEMHandlerType);

  /** Get the residual ground error */
  itkGetConstReferenceMacro(ResidualGroundError,double);
  
  /** Get the GCPsContainer
   * \return The GCPs container */
  GCPsContainerType & GetGCPsContainer() const;

  /** Set the GCP container */
  void SetGCPsContainer(const GCPsContainerType & container);

  /** Add a GCP to the GCPContainer. This version of the AddGCP method
   * accepts a 3D ground point and does not use DEM or MeanElevation
   * to handle the elevation */
  void AddGCP(const Point2DType& sensorPoint, const Point3DType& groundPoint);

  /** Add A GCP to the GCPContainer. This version of the AddGCP method
   * accepts a 3D ground point and use the DEM or MeanElevation to get
   * the corresponding elevation before calling the AddGCP acception 3D
   * ground points.*/
  void AddGCP(const Point2DType& sensorPoint, const Point2DType & groundPoint);

  /** Clear all GCPs */
  void ClearGCPs();

protected:
  /** Constructor */
  GCPsToRPCSensorModelImageFilter();
  /** Destructor */
  virtual ~GCPsToRPCSensorModelImageFilter();
  
  /** The PrintSelf method */
  virtual void PrintSelf( std::ostream & os, itk::Indent indent ) const;

  /** Actual estimation of the sensor model takes place in the
   * GenerateOutputInformation() method */
  virtual void GenerateOutputInformation();

private:
  GCPsToRPCSensorModelImageFilter ( const Self & ); // purposely not implemented
  void operator= ( const Self & ); // purposely not implemented

  /** True to use GCPs from image metadata as well */
  bool   m_UseImageGCPs;

  /** The residual ground error */
  double m_ResidualGroundError;

  /** True if a DEM should be used */
  bool m_UseDEM;

  /** If no DEM is used, a MeanElevation 
   * over the image is used instead */
  double m_MeanElevation;

  /** The DEMHandler */
  DEMHandlerPointerType m_DEMHandler;

  /** Container of GCPs */
  GCPsContainerType m_GCPsContainer;

}; // end of class

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbGCPsToRPCSensorModelImageFilter.txx"
#endif



#endif


