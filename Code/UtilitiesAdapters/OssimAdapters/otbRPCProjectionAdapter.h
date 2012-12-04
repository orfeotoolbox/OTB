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

#include "vcl_deprecated_header.h"

#include <vector>

#include "itkObject.h"
#include "itkObjectFactory.h"

#include "itkPoint.h"
#include "itkSize.h"


class ossimRpcProjection;

namespace otb {

class ImageKeywordlist;

/** \class RPCProjectionAdapter
 *  \brief Wrapper class for the ossim Rpc projection
 *
 * This class is used inside the GCPsToRPCSensorModelImageFilter and should not
 * be used directly outside.
 * 
 * \deprecated Please use the RpcSolverAdapter class static methods instead
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

  void AddGroundRect( ImageKeywordlist& otb_kwl,
                      itk::Point<double, 2> orig, itk::Size<2> size);

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
