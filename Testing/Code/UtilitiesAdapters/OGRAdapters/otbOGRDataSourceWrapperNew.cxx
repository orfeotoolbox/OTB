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

/*===========================================================================*/
/*===============================[ Includes ]================================*/
/*===========================================================================*/
#define BOOST_TEST_MODULE "otb::org::DataSource creation unit testing"
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include "otbOGRDataSourceWrapper.h"


using namespace otb;

/**\ingroup Geometry
 * \file    otbOGRDataSourceWrapperNew.cxx
 *
 *\todo test: CopyLayer from one datasource to another
 *\todo test: read datasource
 *\todo test: write datasource
 */

/*===========================================================================*/
/*==============================[ other stuff ]==============================*/
/*===========================================================================*/

BOOST_AUTO_TEST_CASE(OGRDataSource_new_empty)
{
  ogr::DataSource::Pointer ds = ogr::DataSource::New();
  BOOST_ASSERT(ds);
  BOOST_CHECK_EQUAL(ds->GetLayersCount(), 0);
  BOOST_CHECK_EQUAL(ds->Size(false), 0);
  BOOST_CHECK_EQUAL(ds->Size(true), 0);
  BOOST_CHECK_THROW(ds->GetLayerChecked(0), itk::ExceptionObject);
}

const std::string k_one = "one";
const std::string k_two = "two";

BOOST_AUTO_TEST_CASE(OGRDataSource_mem_add_n_del_layer)
{
  ogr::DataSource::Pointer ds = ogr::DataSource::New();
  BOOST_ASSERT(ds);
  ogr::Layer l = ds -> CreateLayer(k_one);
  BOOST_CHECK_EQUAL(ds->GetLayersCount(), 1);
  BOOST_CHECK_EQUAL(l.GetFeatureCount(false), 0);
  BOOST_CHECK_EQUAL(l.GetFeatureCount(true), 0);
  ogr::Layer l0 = ds->GetLayerChecked(0);
  BOOST_CHECK_THROW(ds->GetLayerChecked(1), itk::ExceptionObject);
  BOOST_CHECK_EQUAL(l.GetName(), k_one);
  BOOST_CHECK_EQUAL(l0.GetName(), k_one);

  ogr::Layer l2 = ds -> CreateLayer(k_two);
  BOOST_CHECK_EQUAL(ds->GetLayersCount(), 2);
  BOOST_CHECK_EQUAL(ds->Size(false), 0);
  BOOST_CHECK_EQUAL(ds->Size(true), 0);
  BOOST_CHECK_EQUAL(ds->GetLayer(0).GetName(), k_one);
  BOOST_CHECK_EQUAL(ds->GetLayer(1).GetName(), k_two);
  BOOST_CHECK_THROW(ds->GetLayerChecked(2), itk::ExceptionObject);

  ds->DeleteLayer(0);
  BOOST_CHECK_EQUAL(ds->GetLayersCount(), 1);
  BOOST_CHECK_EQUAL(ds->GetLayer(0).GetName(), k_two);
  BOOST_CHECK_THROW(ds->GetLayerChecked(1), itk::ExceptionObject);
  BOOST_CHECK_EQUAL(ds->Size(false), 0);
  BOOST_CHECK_EQUAL(ds->Size(true), 0);

  std::cout << "End of scope" << std::endl;
}
