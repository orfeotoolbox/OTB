/*********************************************************************************************
 *
 * ossimJpipMessage was modified from the oiginal source found in a JAVA based framework called
 *
 * CADI Software - a JPIP Client/Server framework
 * 
 *********************************************************************************************/
#ifndef ossimJpipMessageHeader_HEADER
#define ossimJpipMessageHeader_HEADER
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimString.h>
#include <sstream>

class OSSIM_DLL ossimJpipMessageHeader : public ossimReferenced
{
public:
    ossimJpipMessageHeader()
    :m_isLastByte(true),
    m_inClassIdentifier(-1),
    m_classIdentifier(-1),
    m_CSn(-1),
    m_msgOffset(-1),
    m_msgLength(-1),
    m_aux(-1),
    m_isEOR(false),
    m_EORCode(-1),
    m_EORReasonMessage("")
    {
        
    }
    void reset()
    {
        m_isLastByte        = true;
        m_inClassIdentifier = -1;
        m_classIdentifier   = -1;
        m_CSn               = -1;
        m_msgOffset         = -1;
        m_msgLength         = -1;
        m_aux               = -1;
        m_isEOR             = false;
        m_EORCode           = -1;
        m_EORReasonMessage  = "";
    }
    ossimString toString() const
    {
        std::ostringstream out;
        
        out << "m_isLastByte: " << ossimString::toString(m_isLastByte) << "\n"
        << "m_inClassIdentifier: " << m_inClassIdentifier << "\n"
        << "m_classIdentifier: " << m_classIdentifier << "\n"
        << "m_CSn: " << m_CSn << "\n"
        << "m_msgOffset: " << m_msgOffset << "\n"
        << "m_msgLength: " << m_msgLength << "\n"
        << "m_aux: "       << m_aux << "\n"
        << "m_isEOR: "            << m_isEOR << "\n"
        << "m_EORCode: "          << m_EORCode << "\n"
        << "m_EORReasonMessage: " << m_EORReasonMessage << "\n";
        
        return ossimString(out.str());
	}	
    
    /**
	 *	Bin-ID = [BinIdIndicator, completeDataBin, InClassIdentifier]
	 * 
	 * Bin-ID format:<BR>
	 *  
	 *  <samp>
	 *  MSB                                                 LSB 
	 *   7 6 5 4 3 2 1 0    7 6 5 4 3 2 1 0    7 6 5 4 3 2 1 0  
	 *  -----------------  -----------------  -----------------
	 *  |a|b b|c|d d d d|  |a|d d d d d d d|  |a|d d d d d d d| .....
	 *  -----------------  -----------------  -----------------
	 *  </samp>
	 * <P>
	 *  Bits 6 and 5 of the first, BinIDIndicator, byte indicate whether the
	 *  Class and CSn VBASs are present in the message header. Next table defines
	 *  the bit values and its meaning
	 *  <P>
	 *  <table border=1 cellspacing=0 cellpadding=2>
	 *  	<tr align=center>
	 *  		<td> <b> Indicator </b> </td>
	 *  		<td> <b> Meaning </b> </td>
	 *  	</tr>
	 *  	<tr>
	 *  		<td> 0 </td>
	 *  		<td> Prohibited </td>
	 *  	</tr>
	 *  	<tr>
	 * 	 	<td> 1 </td>
	 *  		<td> No Class or CSn VBAS is present in message header </td>
	 *  	</tr>    
	 *  	<tr>
	 *  		<td> 2 </td>
	 *  		<td> Class VBAS is present but CSn is no present in message header </td>
	 *  	</tr>  
	 *  	<tr>
	 *  		<td> 3 </td>
	 *  		<td> Class and CSn VBAS are both present in the message header </td>
	 *  	</tr>  
	 *  </table>
	 *  <P>
	 *  Bit 4, completeDataBin, of the first byte of the Bin-ID indicates
	 *  whether or not this message contains the last byte in the asociated
	 *  data-bin: '0' means it is not the last byte in the data-bin; '1'
	 *  inidates tat it is the last byte in the data-bin.
	 *  <P>
	 *  The remaining 4 bits of the first byte and the 7 low order bits of
	 *  any remaining bytes in the Bin-ID VBAS form an "in-class identifier".
	 *  <P>
	 */
    bool m_isLastByte;	// completeDataBin better???
    ossim_int64 m_inClassIdentifier;
    
	/**
	 * If present, provides a message class identifier. The message class
	 * identifier is a non-negative integer, formed by concatenating the
	 * least significant 7 bits of each byte of the VBAS in big-endian order.
	 * If no present, the message class identifier is unchanged form that
	 * associated with the previous message. If the Class VBAS is not present
	 * and there is no previous message, the message class identifier is 0.
	 * <P>
	 * <table border=1 cellspacing=0 cellpadding=2>
	 *  	<tr align=center>
	 *  		<td> <b> Class identifier </b> </td>
	 *  		<td> <b> Message class </b> </td>
	 *  		<td> <b> Data-bin class </b> </td>
	 *  		<td> <b> Stream type </b> </td>
	 *  	</tr>
	 *		<tr>
	 *  		<td> 0 </td>
	 *  		<td> Precinct data-bin message </td>
	 *  	 	<td> Precinct data-bin</td>
	 *    	<td> JPP-stream only </td>
	 *  	</tr>
	 *   	<tr>
	 *  		<td> 1 </td>
	 *  		<td> Extended precinct data-bin message </td>
	 *  	 	<td> Precinct data-bin</td>
	 *    	<td> JPP-stream only </td>
	 *  	</tr>	
	 *   	<tr>
	 *  		<td> 2 </td>
	 *  		<td> Tile header data-bin message </td>
	 *  	 	<td> Tile header data-bin</td>
	 *    	<td> JPP-stream only </td>
	 *  	</tr>
	 *  	<tr>
	 *  		<td> 4 </td>
	 *  		<td> Tile data-bin message </td>
	 *  	 	<td> Tile data-bin</td>
	 *    	<td> JPT-stream only </td>
	 *  	</tr>
	 *   	<tr>
	 *  		<td> 5 </td>
	 *  		<td> Extended precinct data-bin message </td>
	 *  	 	<td> Tile data-bin</td>
	 *    	<td> JPT-stream only </td>
	 *  	</tr>
	 *   	<tr>
	 *  		<td> 6 </td>
	 *  		<td> Main header data-bin message </td>
	 *  	 	<td> Main heaer data-bin</td>
	 *    	<td> JPP- and JPT-stream </td>
	 *  	</tr>
	 *  	<tr>
	 *  		<td> 8 </td>
	 *  		<td> Metadata-bin message </td>
	 *  	 	<td> Metadata-bin</td>
	 *    	<td> JPP and JPT-stream </td>
	 *  	</tr>
	 *  </table>
	 *  
	 *  <p>
	 *  Allowed values, see {@link CADI.Common.Network.JPIP.ClassIdentifiers}.
	 */
	ossim_int32 m_classIdentifier;
    
	/**
	 * If present, identifies the index (stating from 0) of the codestream to
	 * which the data-bin belongs. The codestream index is formed by concatenating
	 * the leas significant 7 bits of each byte of the VBAS in big-endian order.
	 * If the CSn VBAS is no present, the codestream index is unchanged from the
	 * previous message. If CSn VBAS is no present and there is no previous
	 * message, the codestream index is 0.
	 */
    ossim_int32 m_CSn;
    
	/**
	 * Identifies the offset of the data in the message from the start of the
	 * data-bin. It is a non-negative integer value, formed by concatenating the
	 * least significant 7 bits of each byte in the VBAS in big-endian order.
	 */
    ossim_int64 m_msgOffset;	
    
	/**
	 * Identifies the total number of bytes in the body of the message. It is a
	 * non-negative integer value, formed by concatenating the least significant
	 * 7 bits of each byte in the VBAS in big-endian order.
	 */
    ossim_int64 m_msgLength;
    
	/**
	 * If present, it represents a non-negative integer value, formed by
	 * concatenating the least significant 7 bits of each byte in the VBAS in
	 * big-endian order.
	 * <p>
	 * Its presence and meaning if present, is determined by the message class
	 * identifier found within the Bin-ID VBAS.
	 */
    ossim_int64 m_aux;
    
	/**
	 * Indicates if this JPIP Message is an End of Response Message.
	 */
	bool m_isEOR;
	
	/**
	 * Definition in {@link CADI.Common.Network.JPIP.EORCodes}.
	 * <p>
	 * Further information, see ISO/IEC 15444-9 sect. D.3
	 */
	int m_EORCode;		// <<<<<<<<<< NOTICE: the inClassIdentifier could be used as a EORCode >>>>>>>>>>
	
	/**
	 * It is and reason message associated with the <code>EORReasonCode</code>.
	 * <p>
	 * It is an optional attribute.
	 */
    ossimString m_EORReasonMessage;
    
};

#endif

