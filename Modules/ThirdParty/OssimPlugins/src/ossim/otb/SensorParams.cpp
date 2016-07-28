//----------------------------------------------------------------------------
//
// "Copyright Centre National d'Etudes Spatiales"
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------
// $Id$

#include "otb/SensorParams.h"
#include "ossimKeyWordListUtilities.h"

namespace ossimplugins
{


static const char PREFIX[]               = "sensor_params.";
static const char PRF_KW[]               = "prf";
static const char SF_KW[]                = "sampling_frequency";
static const char RWL_KW[]               = "radar_wave_length";
static const char COL_DIR_KW[]           = "column_direction";
static const char LIN_DIR_KW[]           = "line_direction";
static const char SIGHT_DIR_KW[]         = "sight_direction";
static const char SEMI_MAJOR_AXIS_KW[]   = "semi_major_axis";
static const char SEMI_MINOR_AXIS_KW[]   = "semi_minor_axis";
static const char NUM_AZIMUTH_LOOKS_KW[] = "number_azimuth_looks";
static const char NUM_RANGE_LOOKS_KW[]   = "number_range_looks";
static const char DOPCEN_KW[]            = "doppler_centroid";
static const char DOPCENLINEAR_KW[]      = "doppler_centroid_linear_term";
static const char RANGETOFIRSTDATA_KW[]  = "range_to_first_data_sample";

SensorParams::SensorParams():
   m_prf(0.0),
   m_sf(0.0),
   m_rwl(0.0),
   m_col_direction(1),
   m_lin_direction(1),
   m_sightDirection(Right),
   m_semiMajorAxis(6378137.0),
   m_semiMinorAxis(6356752.3141),
   m_nAzimuthLook(1),
   m_nRangeLook(1),
   m_dopcen(0.0),
   m_dopcenLinear(0.0)
{
}

bool SensorParams::saveState(ossimKeywordlist& kwl, std::string const& prefix) const
{
   const std::string pfx = prefix + PREFIX;

   add(kwl, pfx, PRF_KW,                        m_prf);
   add(kwl, pfx, SF_KW,                         m_sf);
   add(kwl, pfx, RWL_KW,                        m_rwl);
   add(kwl, pfx, COL_DIR_KW,                    m_col_direction);
   add(kwl, pfx, LIN_DIR_KW,                    m_lin_direction);
   add(kwl, pfx, SIGHT_DIR_KW, static_cast<int>(m_sightDirection));
   add(kwl, pfx, SEMI_MAJOR_AXIS_KW,            m_semiMajorAxis);
   add(kwl, pfx, SEMI_MINOR_AXIS_KW,            m_semiMinorAxis);
   add(kwl, pfx, NUM_AZIMUTH_LOOKS_KW,          m_nAzimuthLook);
   add(kwl, pfx, NUM_RANGE_LOOKS_KW,            m_nRangeLook);
   add(kwl, pfx, DOPCEN_KW,                     m_dopcen);
   add(kwl, pfx, DOPCENLINEAR_KW,               m_dopcenLinear);
   return true;
}

bool SensorParams::loadState(const ossimKeywordlist& kwl, std::string const& prefix)
{
   const std::string pfx = prefix + PREFIX;

   int sightDirection;
   get(kwl, pfx, PRF_KW,               m_prf);
   get(kwl, pfx, SF_KW,                m_sf);
   get(kwl, pfx, RWL_KW,               m_rwl);
   get(kwl, pfx, COL_DIR_KW,           m_col_direction);
   get(kwl, pfx, LIN_DIR_KW,           m_lin_direction);
   get(kwl, pfx, SIGHT_DIR_KW,         sightDirection);
   get(kwl, pfx, SEMI_MAJOR_AXIS_KW,   m_semiMajorAxis);
   get(kwl, pfx, SEMI_MINOR_AXIS_KW,   m_semiMinorAxis);
   get(kwl, pfx, NUM_AZIMUTH_LOOKS_KW, m_nAzimuthLook);
   get(kwl, pfx, NUM_RANGE_LOOKS_KW,   m_nRangeLook);
   get(kwl, pfx, DOPCEN_KW,            m_dopcen);
   get(kwl, pfx, DOPCENLINEAR_KW,      m_dopcenLinear);

   m_sightDirection = static_cast<SightDirection>(sightDirection);
   return true;
}

}
