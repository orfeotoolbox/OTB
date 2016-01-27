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
// #include "otbMaskedIteratorDecorator.h"

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

  template <typename TInputImage>
  void Add(otb::ogr::Layer::const_iterator& featIt,
           // otb::MaskedIteratorDecorator<itk::ImageRegionIterator<TInputImage> >& pixelIt,
           const std::string &fieldName)
  {
    // TODO : move code to txx

    // TODO : switch case with feature type

    //OGRPolygon* inPolygon = dynamic_cast<OGRPolygon *>(geom);
    //OGRLinearRing* exteriorRing = inPolygon->getExteriorRing();
      //itk::Point<double, 2> point;
      //inputImage->TransformIndexToPhysicalPoint(it.GetIndex(), point);
      // ->Test if the current pixel is in a polygon hole
      // If point is in feature
      //if(exteriorRing->isPointInRing(&pointOGR, TRUE) && isNotInHole)
      //{
      //}
      // Count
      //nbOfPixelsInGeom++;
      //nbPixelsGlobal++;

    //Class name recuperation
    //int className = featIt->ogr().GetFieldAsInteger(GetParameterString("cfield").c_str());

    //Counters update, number of pixel in each classes and in each polygons
    //polygon[featIt->ogr().GetFID()] += nbOfPixelsInGeom;

    //Generation of a random number for the sampling in a polygon where we only need one pixel, it's choosen randomly
    //elmtsInClass[className] = elmtsInClass[className] + nbOfPixelsInGeom;
  }

  /** Reset the accumulator */
  void Reset();

protected:
  /** Constructor */
  PolygonClassStatisticsAccumulator() {}
  /** Destructor */
  virtual ~PolygonClassStatisticsAccumulator() {}

private:
  //Number of pixels in all the polygons
  unsigned long nbPixelsGlobal;
  //Number of pixels in each classes
  std::map<int, int> m_elmtsInClass;
  //Number of pixels in each polygons
  std::map<unsigned long, int> m_polygon;  // check the feature id

  // Not implemented
  PolygonClassStatisticsAccumulator(const Self&);
  void operator=(const Self&);
};

} // end of namespace otb

#endif
