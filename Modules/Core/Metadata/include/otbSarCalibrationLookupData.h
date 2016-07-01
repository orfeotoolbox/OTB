#ifndef SarCalibrationLookupData_H
#define SarCalibrationLookupData_H 1
#include <string>
#include <itkLightObject.h>
#include <itkNumericTraits.h>
#include <itkObjectFactory.h>

namespace otb {

class ITK_EXPORT SarCalibrationLookupData : public itk::LightObject {

  public:

    /** Standard typedefs */
  typedef SarCalibrationLookupData          Self;
  typedef itk::LightObject                 Superclass;
  typedef itk::SmartPointer<Self>          Pointer;
  typedef itk::SmartPointer<const Self>    ConstPointer;

  /** Creation through the object factory */
  itkNewMacro(Self);

  /** RTTI */
  itkTypeMacro(SarCalibrationLookupData, itk::LightObject);

  typedef itk::IndexValueType IndexValueType;

  enum {
    SIGMA = 0,
    BETA,
    GAMMA,
    DN
  };

  SarCalibrationLookupData()
    :m_Type(0)
  {
  }

  ~SarCalibrationLookupData() ITK_OVERRIDE
  {
  }

  virtual double GetValue(const IndexValueType itkNotUsed(x), const IndexValueType itkNotUsed(y)) const
  {
    return 1.0;
  }

  void SetType(short t)
  {
    m_Type = t;
  }

  itkGetMacro(Type, short);

  void PrintSelf(std::ostream & os, itk::Indent indent) const ITK_OVERRIDE
  {
    os << indent << " lookup table type:'" << m_Type << "'" << std::endl;
    Superclass::PrintSelf(os, indent);
  }

private:

  SarCalibrationLookupData(const Self&); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
  short m_Type;

};

}
#endif
