#ifndef SarCalibrationLookupData_H
#define SarCalibrationLookupData_H 1
#include <string>

namespace otb {

class SarCalibrationLookupData {

  public:

  SarCalibrationLookupData()
    :dtype("sigma")
  {

  }

  std::string GetNameOfClass()
  {
    return "SarCalibrationLookupData";
  }

  SarCalibrationLookupData(std::string t)
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

std::string dtype;

};

}
#endif
