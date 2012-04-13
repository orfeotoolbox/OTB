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
#ifndef __otbLayer_h
#define __otbLayer_h

// #include <iosfwd> // std::ostream&
#include <boost/shared_ptr.hpp>
#include "itkIndent.h"
class OGRLayer; // fwd declaration
class OGRDataSource;

namespace otb { namespace ogr
{
/**\ingroup Geometry
 * \brief Layer of geometric objets.
 *
 * It provides an encapsulation of OGR classes. In that particular case, it's an
 * encapsulation of \c OGRLayer.
 *
 * \note this class is a proxy class on top of an \c OGRLayer. 
 * \note It can be copied.
 * \note when created from a \c otb::ogr::DataSource::ExecuteSQL, it will
 * automatically manage the release of the underlying \c OGRLayer.
 * \note the default constructor is disabled on purpose
 * \note the destructor automatically generated does everything that is
 * expected.
 *
 * \todo find a way to tell whether the related \c OGRDataSource was released
 */
class Layer 
// : public itk::DataObject
  {
public:
  /**\name Standard class typedefs */
  //@{
  typedef Layer                         Self;
  // typedef itk::DataObject               Superclass;
  // typedef itk::SmartPointer<Self>       Pointer;
  // typedef itk::SmartPointer<const Self> ConstPointer;
  //@}

  /**\name Standard macros */
  //@{
  // itkNewMacro(Self);
  // itkTypeMacro(Layer, DataObject);
  //@}

  /**\name Construction */
  //@{
  explicit Layer(OGRLayer* layer);
  Layer(OGRLayer* layer, OGRDataSource* sourceInChargeOfLifeTime);
  //@}
  /** Returns the number of elements in the layer.
   * \param[in] doForceCompuation  indicates whether the size shall be computed
   * even so it's expensive to do so.
   *
   * \return the number of features in the layer, -1 if count is unknown
   * \throw None
   * \sa OGRLayer::GetFeatureCount
   */
  int GetFeatureCount(bool doForceComputation) const;

  std::string GetName() const;

  /** Prints self into stream. */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;


  struct boolean{ int i;};
  /** Can the layer be used (ie not null).
   * 
   * Hack to provide a boolean operator that is convertible only to a
   * boolean expression to be used in \c if tests.
   * @see <em>Imperfect C++</em>, Matthew Wilson, Addisson-Welsey, par 24.6
   */
  operator int boolean ::* () const {
    return m_Layer ? &boolean::i : 0;
  }

  /** Access to raw \c OGRLayer.
   * This function provides an abstraction leak in case deeper control on the
   * underlying \c OGRLayer is required.
   * \pre the underlying \c OGRLayer must be valid, i.e.
   * <tt>m_Layer != 0</tt>, an assertion is fired otherwise.
   * \warning you must under no circonstance try to delete the \c OGRLayer
   * obtained this way.
   */
  OGRLayer & ogr();

private:

  /** Data implementation.
   * \internal The actual %layer implementation belongs to the \c otb::Layer object.
   */
  boost::shared_ptr<OGRLayer> m_Layer;
  };

} } // end namespace otb::ogr

#ifndef OTB_MANUAL_INSTANTIATION
// #include "otbLayer.txx"
#endif

#endif // __otbLayer_h
