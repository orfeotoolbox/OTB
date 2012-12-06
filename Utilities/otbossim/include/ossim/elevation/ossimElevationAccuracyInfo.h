#ifndef ossimElevationAccuracyInfo_HEADER
#define ossimElevationAccuracyInfo_HEADER 1
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimReferenced.h>
#include <ossim/base/ossimString.h>


class OSSIM_DLL ossimElevationAccuracyInfo: public ossimReferenced
{
public:
    ossimElevationAccuracyInfo():
        m_confidenceLevel(0.9),
        m_relativeCE(ossim::nan()),
        m_absoluteCE(ossim::nan()),
        m_relativeLE(ossim::nan()),
        m_absoluteLE(ossim::nan()),
        m_surfaceName("None")
    {}

    void makeNan()
    {
     m_relativeCE = ossim::nan();
     m_absoluteCE = ossim::nan();
     m_relativeLE = ossim::nan();
     m_absoluteLE = ossim::nan();
    }
    double getConfidenceLevel()const{return m_confidenceLevel;}

    bool hasValidRelativeError()const
    { 
        return ((!ossim::isnan(m_relativeCE)) && 
                (!ossim::isnan(m_relativeLE)));
    }
    bool hasValidAbsoluteError()const{
        return ((!ossim::isnan(m_absoluteCE)) && 
                (!ossim::isnan(m_absoluteLE)));
    }
    double getRelativeCE()const{return m_relativeCE;}
    void setRelativeCE(double value){m_relativeCE = value;}

    double getAbsoluteCE()const{return m_absoluteCE;}
    void setAbsoluteCE(double value){m_absoluteCE = value;}

    double getRelativeLE()const{return m_relativeLE;}
    void setRelativeLE(double value){m_relativeLE = value;}

    double getAbsoluteLE()const{return m_absoluteLE;}
    void setAbsoluteLE(double value){m_absoluteLE = value;}
    
    const ossimString& getSurfaceName()const{return m_surfaceName;}
    void setSurfaceName(const ossimString& value){m_surfaceName = value;}

    double m_confidenceLevel;
    double m_relativeCE;
    double m_absoluteCE;
    double m_relativeLE;
    double m_absoluteLE;
    ossimString m_surfaceName;
};

#endif
