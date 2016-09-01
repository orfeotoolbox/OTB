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
#ifndef otb_GeoInterface_h
#define otb_GeoInterface_h


#include "itkFloatTypes.h"
#include "itkPoint.h"


namespace otb
{

class ImageKeywordlist;


/**
 * @class This class is the abstract interface for actors which need
 * geo-referenced methods.
 *
 * Use multiple inheritance to provide geo-referenced interface to
 * specialized actors.
 *
 * Example: Derive otb::GlMyActor : public otb::GlActor, public
 * otb::GeoInterface {};
 *
 * By default, single-prevision methods are implemented as conversion
 * method from otb::GeoInterface::Point2f to
 * otb::GeoInterface::Point2d. Derived otb::GlActor classes must
 * implement double-prevision methods.
 */
class GeoInterface
{
public:
  /**
   */
  typedef itk::Point< itk::SpacePrecisionType, 2 > Point2;

  /**
   * Double precision point-type.
   */
  typedef itk::Point< double, 2 > Point2d;

  /**
   * Spacing-type based on ITK floating-point prevision.
   */
  typedef itk::Vector< itk::SpacePrecisionType, 2 > Spacing2;

  /**
   * Transform given point from viewport coordinate-system to this
   * actor coordinate-system.
   *
   * By default this method return false. It should be implemented in
   * derived classes if needed.
   *
   * @param out The transformed point (in this actor coordinate-system).
   * @param in The given point (in viewport coordinate-system).
   * @param isPhysical Indicate physical coordates are desired.
   *
   * @return true if the operation has succeeded; false otherwise.
   */
  virtual bool TransformFromViewport( Point2d & out,
                                      const Point2d & in,
                                      bool isPhysical = true ) const;

  /**
   * Transform given point from viewport coordinate-system to this
   * actor coordinate-system.
   *
   * By default this method return false. It should be implemented in
   * derived classes if needed.
   *
   * @param out The transformed point (in this actor coordinate-system).
   * @param in The given point (in viewport coordinate-system).
   * @param isPhysical Indicate physical coordates are desired.
   *
   * @return true if the operation has succeeded; false otherwise.
   */
  virtual bool TransformToViewport( Point2d & out,
                                    const Point2d & in,
                                    bool isPhysical = true ) const;

  /**
   */
  virtual const Spacing2 & GetSpacing() const;

  /**
   */
  virtual std::string GetWkt() const;

  /**
   */
  virtual bool HasKwl() const;

  /**
   */
  virtual bool GetKwl( ImageKeywordlist & ) const;


protected:
  GeoInterface();
  virtual ~GeoInterface();

private:

}; // End class GeoInterface


} // End namespace otb


#endif // otb_GeoInterface_h
