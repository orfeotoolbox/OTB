/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbTestHelper_h
#define __otbTestHelper_h

#include <map>
#include <string>
#include <vector>
#include "otbMetaDataKey.h"

// OGR Headers files
#include "ogrsf_frmts.h"

namespace otb
{
  class /*ITK_EXPORT*/ TestHelper
  {
    public:
      TestHelper() : m_EpsilonBoundaryChecking(1.0e-30){};
      ~TestHelper(){};

      std::map<std::string,int> RegressionTestbaselines (char *baselineFilename) const;
      int RegressionTestImage (int, const char *, const char *, int, const double) const;
      int RegressionTestOgrFile (const char *, const char *, int, const double) const;
      int RegressionTestBinaryFile(const char *, const char *, int) const;
      int RegressionTestAsciiFile(const char *, const char *, int, const double, std::vector<std::string> ignoredLines) const;
      int RegressionTestListFile(const char *, const char *, int, const double, std::vector<std::string> ignoredLines) const;
      int RegressionTestMetaData(const char *, const char *,int,const double) const;

    private:
      bool isNumber(int i) const;
      bool isHexaNumber(int i) const;
      bool isPoint(int i) const;
      bool isMinusSign(int i) const;
      bool isNumeric(std::string str) const;
      bool isScientificNumeric(std::string str) const;
      bool isHexaPointerAddress(std::string str) const;
      std::string VectorToString(otb::MetaDataKey::VectorType vector) const;

      void ogrReportOnLayer( OGRLayer *, const char *, OGRGeometry * ) const;
      void ogrReportOnLayer( 
        OGRLayer * ref_poLayer,   const char *ref_pszWHERE,   OGRGeometry *ref_poSpatialFilter,
        OGRLayer * test_poLayer,  const char *test_pszWHERE,  OGRGeometry *test_poSpatialFilter,
                           int & nbdiff, int bVerbose ) const;

      double m_EpsilonBoundaryChecking;

  };
}

#endif
