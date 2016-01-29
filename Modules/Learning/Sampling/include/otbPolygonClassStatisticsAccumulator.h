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
#ifndef __otbPolygonClassStatisticsAccumulator_h
#define __otbPolygonClassStatisticsAccumulator_h

#include "otbOGRDataSourceWrapper.h"

namespace otb
{

/**
 * \class PolygonClassStatisticsAccumulator
 *
 * \brief This class stores statistics of OGR data for each thread
 *
 */
class PolygonClassStatisticsAccumulator : public itk::Object
{
public:
  typedef PolygonClassStatisticsAccumulator  Self;
  typedef itk::SmartPointer<Self>            Pointer;
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(PolygonClassStatisticsAccumulator, itk::Object);

  template <typename TPointType>
  void Add(otb::ogr::Layer::const_iterator& featIt,
           const TPointType& point);

  /** Reset the accumulator */
  void Reset();

  itkSetMacro(FieldName, std::string);
  itkGetMacro(FieldName, std::string);

protected:
  /** Constructor */
  PolygonClassStatisticsAccumulator() {}
  /** Destructor */
  virtual ~PolygonClassStatisticsAccumulator() {}

private:
  //Number of pixels in all the polygons
  unsigned long m_NbPixelsGlobal;
  //Number of pixels in each classes
  std::map<int, int> m_elmtsInClass;
  //Number of pixels in each polygons
  std::map<unsigned long, int> m_polygon;  // check the feature id
  
  std::string m_FieldName;

  // Not implemented
  PolygonClassStatisticsAccumulator(const Self&);
  void operator=(const Self&);
};

} // end of namespace otb

#endif
