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
/**
 * \class TestHelper
 * \brief Helper class to perform the baseline comparisons during the tests
 */
class /*ITK_EXPORT*/ TestHelper
{
public:
  TestHelper() :
    m_EpsilonBoundaryChecking(1.0e-30),
    m_ReportErrors(false),
    m_IgnoreLineOrder(false)
  {}

  ~TestHelper(){}

  std::map<std::string, int> RegressionTestbaselines(char *baselineFilename) const;

  int RegressionTestImage(int cpt, const char *testImageFilename, const char *baselineImageFilename,
                          const double toleranceDiffPixelImage) const;

  int RegressionTestOgrFile(const char *testOgrFilename, const char *baselineOgrFilename,
                            const double toleranceDiffValue) const;

  int RegressionTestBinaryFile(const char * testBinaryFileName, const char * baselineBinaryFileName) const;

  int RegressionTestAsciiFile(const char * testAsciiFileName, const char * baselineAsciiFileName,
                              const double epsilon, std::vector<std::string> ignoredLines) const;

  int RegressionTestMetaData(const char *testImageFilename, const char *baselineImageFilename,
                             const double toleranceDiffPixelImage) const;

  void ReportErrorsOn()
  {
    m_ReportErrors = true;
  }
  void ReportErrorsOff()
  {
    m_ReportErrors = false;
  }

  void IgnoreLineOrderOn()
  {
    m_IgnoreLineOrder = true;
  }
  void IgnoreLineOrderOff()
  {
    m_IgnoreLineOrder = false;
  }

  void SetEpsilonBoundaryChecking(double epsilonBoundary)
  {
    m_EpsilonBoundaryChecking = epsilonBoundary;
  }

private:
  bool isNumber(int i) const;
  bool isHexaNumber(int i) const;
  bool isPoint(int i) const;
  bool isMinusSign(int i) const;
  bool isNumeric(std::string str) const;
  bool isScientificNumeric(std::string str) const;
  bool isHexaPointerAddress(std::string str) const;
  bool isStringFound(std::string str) const;
  std::string VectorToString(otb::MetaDataKey::VectorType vector) const;
  //FIXME parameters have to be cleaned up later (this is the first step of refactoring)
  bool CompareLines(std::string strfileref,
                    std::string strfiletest,
                    int& nbdiff,
                    std::ofstream& fluxfilediff,
                    int& numLine,
                    std::vector<std::string>& listStrDiffLineFileRef,
                    std::vector<std::string>& listStrDiffLineFileTest,
                    double epsilon) const;

  void
  ogrReportOnLayer(OGRLayer * ref_poLayer, const char *ref_pszWHERE, OGRGeometry *ref_poSpatialFilter,
                   OGRLayer * test_poLayer, const char *test_pszWHERE, OGRGeometry *test_poSpatialFilter,
                   int& nbdiff) const;

  static void DumpOGRFeature(FILE* fileid, OGRFeature* feature, char** papszOptions = NULL);
  static void DumpOGRGeometry(FILE* fileid, OGRGeometry* geometry, const char * pszPrefix, char** papszOptions = NULL);

  double m_EpsilonBoundaryChecking;
  bool   m_ReportErrors;
  bool   m_IgnoreLineOrder;

};
}

#endif
