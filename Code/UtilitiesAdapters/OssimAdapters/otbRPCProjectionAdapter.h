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
#ifndef __otbRPCProjectionAdapter_h
#define __otbRPCProjectionAdapter_h

#include <vector>

#include "itkObject.h"
#include "itkObjectFactory.h"

#include "itkPoint.h"
#include "ossim/base/ossimDpt.h"
#include "ossim/base/ossimGpt.h"


#include "ossim/projection/ossimRpcProjection.h"
class ossimRpcProjection;

namespace otb {

class ImageKeywordlist;

/** \class RPCProjectionAdapter
 *  \brief Wrapper class for the ossim Rpc projection
 *
 * This class is used inside the GCPsToRPCSensorModelImageFilter and should not
 * be used directly outside.
 *
 * \sa GCPsToRPCSensorModelImageFilter
 **/

class RPCProjectionAdapter: public itk::Object
{
public:
  /** Standard class typedefs. */
  typedef RPCProjectionAdapter          Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef itk::Point<double, 2>               Point2DType;
  typedef itk::Point<double, 3>               Point3DType;
  typedef std::pair<Point2DType, Point3DType> GCPType;
  typedef std::vector<GCPType>                GCPsContainerType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(RPCProjectionAdapter, itk::Object);

  void Solve(const GCPsContainerType& gcpContainer,
             double& rmsError, ImageKeywordlist& otb_kwl);
  void TransformPoint(double x, double y, double z,
                      double& lon, double& lat, double& h) const;

  void LineSampleToWorld(const ossimDpt & imagePoint, ossimGpt& worldPoint) const
  {
    m_RpcProjection->lineSampleToWorld(imagePoint, worldPoint);
  }

protected:
  RPCProjectionAdapter();
  virtual ~RPCProjectionAdapter();

private:
  RPCProjectionAdapter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  /** RPC Projection */
  ossimRpcProjection* m_RpcProjection;
};

} // namespace otb

#endif
