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

#if 0
#if defined(BOOST_TEST_DYN_LINK)
#error BOOST_TEST_DYN_LINK defined
#else
#error BOOST_TEST_DYN_LINK not defined
#endif

#if defined(BOOST_TEST_MAIN)
#error BOOST_TEST_MAIN defined
#else
#error BOOST_TEST_MAIN not defined
#endif

#if defined(BOOST_TEST_NO_MAIN)
#error BOOST_TEST_NO_MAIN defined
#else
#error BOOST_TEST_NO_MAIN not defined
#endif
#endif

/*===========================================================================*/
/*==============================[ other stuff ]==============================*/
/*===========================================================================*/

BOOST_AUTO_TEST_CASE(OGRDataSource_new)
{
  ogr::DataSource::Pointer ds = ogr::DataSource::New();
  BOOST_ASSERT(ds);
  BOOST_CHECK_EQUAL(ds->GetLayersCount(), 0);
  BOOST_CHECK_EQUAL(ds->Size(false), 0);
  BOOST_CHECK_EQUAL(ds->Size(true), 0);
  BOOST_CHECK_THROW(ds->GetLayerChecked(0), itk::ExceptionObject);
}
