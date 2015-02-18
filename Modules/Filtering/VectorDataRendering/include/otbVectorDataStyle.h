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

#ifndef __otbVectorDataStyle_h
#define __otbVectorDataStyle_h

#include "itkLightObject.h"
#include "itkObjectFactory.h"
#include <mapnik/map.hpp>

namespace otb
{
/** \class VectorDataStyle
 *   \brief Load style into a mapnik::Map
 *
 * \sa VectorDataToMapFilter
 */

class ITK_EXPORT VectorDataStyle : public itk::LightObject
{
public:
  /** Standard class typedefs. */
  typedef VectorDataStyle               Self;
  typedef itk::LightObject              Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(VectorDataStyle, LightObject);

  void LoadOSMStyle(mapnik::Map& mapnikMap) const;

  void LoadBinaryRasterizationStyle(mapnik::Map& mapnikMap) const;

  /** Get/Set the size of the scale factor. */
  double GetScaleFactor() const
  {
    return m_ScaleFactor;
  }
  void SetScaleFactor(double scale)
  {
    m_ScaleFactor = scale;
  }

protected:
  /** Constructor */
  VectorDataStyle() :
    m_ScaleFactor(1.0){};
  /** Destructor */
  virtual ~VectorDataStyle() {}
  /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  VectorDataStyle(const Self &);  //purposely not implemented
  void operator =(const Self&);  //purposely not implemented

  //this parameter is used only in the case of sensor geometry
  //to adjust the scale
  double m_ScaleFactor;

}; // end class
} // end namespace otb

#endif
