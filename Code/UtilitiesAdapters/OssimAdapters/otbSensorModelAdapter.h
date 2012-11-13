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
#ifndef __otbSensorModelAdapter_h
#define __otbSensorModelAdapter_h

#include "itkObject.h"
#include "itkObjectFactory.h"
#include "otbDEMHandler.h"

class ossimProjection;
class ossimTieGptSet;

namespace otb
{

class ImageKeywordlist;

/**
 * \class SensorModelAdapter
 * \brief Wrapper class to group all dependencies to ossim for sensor models
 *
 * This class is NOT intented to be used outside of OTB. Use the
 * InverseSensorModel and ForwardSensorModel. If you feel that you need to use
 * it directly, think again!
 *
 * \sa InverseSensorModel
 * \sa ForwardSensorModel
 * \ingroup Projection
 *
 **/

class SensorModelAdapter: public itk::Object
{
public:
  /** Standard class typedefs. */
  typedef SensorModelAdapter          Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef ossimProjection*       InternalMapProjectionPointer;
  typedef const ossimProjection* InternalMapProjectionConstPointer;
  typedef ossimTieGptSet*        InternalTiePointsContainerPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(SensorModelAdapter, itk::Object);

  // Deprecated calls to elevation setters
  otbLegacyElevationMacro();

  /** Create the projection ( m_Model). Called by the SetImageGeometry methods */
  void CreateProjection(const ImageKeywordlist& image_kwl);
  // FIXME check if it should be protected instead

  void ForwardTransformPoint(double x, double y, double z,
                             double& lon, double& lat, double& h) const;

  void InverseTransformPoint(double lon, double lat, double h,
                             double& x, double& y, double& z) const;

  void AddTiePoint(double x, double y, double z, double lon, double lat);

  void ClearTiePoints();

  double Optimize();

  /** Is sensor model valid method. return false if the m_SensorModel is null*/
  bool IsValidSensorModel();

  /** Read geom file and instanciate sensor model */
  bool ReadGeomFile(const std::string & infile);
  
  /** Write geom file corresponding to sensor model */
  bool WriteGeomFile(const std::string& outfile);

protected:
  SensorModelAdapter();
  virtual ~SensorModelAdapter();

private:
  SensorModelAdapter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  InternalMapProjectionPointer m_SensorModel;

  InternalTiePointsContainerPointer m_TiePoints;

  /** Object that read and use DEM */
  DEMHandler::Pointer m_DEMHandler;

  /** Specify if DEM is used in Point Transformation */
  bool m_UseDEM;

  /** Difference between consecutives heights in iterative transform point with DEM */
  double m_Epsilon; //FIXME check if required

  /** Number of iterations in iterative transform point with DEM */
  double m_NbIter; //FIXME check if required

};

} // namespace otb

#endif
