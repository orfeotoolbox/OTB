//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  Garrett Potts
//
// Description: CSCCGA tag source file
//
//----------------------------------------------------------------------------
// $Id$
#include <ossim/support_data/ossimNitfCsccgaTag.h>
#include <iomanip>
#include <iostream>

ossimNitfCsccgaTag::ossimNitfCsccgaTag()
{
	clearFields();
}
std::string ossimNitfCsccgaTag::getRegisterTagName() const
{
	return "CSCCGA";
}

void ossimNitfCsccgaTag::parseStream(std::istream& in)
{
	in.read(m_ccgSource, CCG_SOURCE_SIZE);
	in.read(m_regSensor, REG_SENSOR_SIZE);
	in.read(m_originLine, ORIGIN_LINE_SIZE);
	in.read(m_originSample, ORIGIN_SAMPLE_SIZE);
	in.read(m_asCellSize, AS_CELL_SIZE_SIZE);
	in.read(m_csCellSize, CS_CELL_SIZE_SIZE);
	in.read(m_ccgMaxLine, CCG_MAX_LINE_SIZE);
	in.read(m_ccgMaxSample, CCG_MAX_SAMPLE_SIZE);
}

void ossimNitfCsccgaTag::writeStream(std::ostream& out)
{
	out.write(m_ccgSource, CCG_SOURCE_SIZE);
	out.write(m_regSensor, REG_SENSOR_SIZE);
	out.write(m_originLine, ORIGIN_LINE_SIZE);
	out.write(m_originSample, ORIGIN_SAMPLE_SIZE);
	out.write(m_asCellSize, AS_CELL_SIZE_SIZE);
	out.write(m_csCellSize, CS_CELL_SIZE_SIZE);
	out.write(m_ccgMaxLine, CCG_MAX_LINE_SIZE);
	out.write(m_ccgMaxSample, CCG_MAX_SAMPLE_SIZE);
}

ossim_uint32 ossimNitfCsccgaTag::getSizeInBytes()const
{
   return (CCG_SOURCE_SIZE+REG_SENSOR_SIZE+ORIGIN_LINE_SIZE+
		     ORIGIN_SAMPLE_SIZE+AS_CELL_SIZE_SIZE+CS_CELL_SIZE_SIZE +
		     CCG_MAX_LINE_SIZE+CCG_MAX_SAMPLE_SIZE);
}

std::ostream& ossimNitfCsccgaTag::print(std::ostream& out,
	                                     const std::string& prefix) const
{
	std::string pfx = prefix;
   pfx += getRegisterTagName();
   pfx += ".";
   out << setiosflags(ios::left)
       << pfx << std::setw(24) << "CETAG:"      << getRegisterTagName() << "\n"
       << pfx << std::setw(24) << "CEL:"        << getSizeInBytes() << "\n"
       << pfx << std::setw(24) << "CCG_SOURCE:" << m_ccgSource << "\n"
       << pfx << std::setw(24) << "CCG_SOURCE:" << m_regSensor << "\n"
       << pfx << std::setw(24) << "ORIGIN_LINE:" << m_originLine << "\n"
       << pfx << std::setw(24) << "ORIGIN_SAMPLE:" << m_originSample << "\n"
       << pfx << std::setw(24) << "AS_CELL_SIZE:" << m_asCellSize << "\n"
       << pfx << std::setw(24) << "CS_CELL_SIZE:" << m_csCellSize << "\n"
       << pfx << std::setw(24) << "CCG_MAX_LINE:" << m_ccgMaxLine << "\n"
       << pfx << std::setw(24) << "CCG_MAX_SAMPLE:" << m_ccgMaxSample << "\n";


	return out;
}

void ossimNitfCsccgaTag::clearFields()
{
	memset(m_ccgSource,' ', CCG_SOURCE_SIZE);
	memset(m_regSensor, ' ', REG_SENSOR_SIZE);
	memset(m_originLine, '0', ORIGIN_LINE_SIZE);
	memset(m_originSample, '0', ORIGIN_SAMPLE_SIZE);
	memset(m_asCellSize, '0', AS_CELL_SIZE_SIZE);
	memset(m_csCellSize, '0', CS_CELL_SIZE_SIZE);
	memset(m_ccgMaxLine, '0', CCG_MAX_LINE_SIZE);
	memset(m_ccgMaxSample, '0', CCG_MAX_SAMPLE_SIZE);

	m_ccgSource[CCG_SOURCE_SIZE] = '\0';
	m_regSensor[REG_SENSOR_SIZE] = '\0';
	m_originLine[ORIGIN_LINE_SIZE] = '\0';
	m_originSample[ORIGIN_SAMPLE_SIZE] = '\0';
	m_asCellSize[AS_CELL_SIZE_SIZE] = '\0';
	m_csCellSize[CS_CELL_SIZE_SIZE] = '\0';
	m_ccgMaxLine[CCG_MAX_LINE_SIZE] = '\0';
	m_ccgMaxSample[CCG_MAX_SAMPLE_SIZE] = '\0';

}
