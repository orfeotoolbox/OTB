#ifndef SarCalibrationLookupData_H
#define SarCalibrationLookupData_H 1
#include <string>

namespace otb {


class SarCalibrationLookupData {

  public:

  enum {
    SIGMA = 0,
    BETA,
    GAMMA,
    DN
  };

  SarCalibrationLookupData()
    :dtype(0)
  {

  }

  std::string GetNameOfClass()
  {
    return "SarCalibrationLookupData";
  }

  SarCalibrationLookupData(short t)
    :dtype(t)
  {

  }

  virtual ~SarCalibrationLookupData()
  {

  }

  virtual double GetValue(int /*x*/, int /*y*/)
  {
    return 1.0;
  }


private:

short dtype;

};

}
#endif
