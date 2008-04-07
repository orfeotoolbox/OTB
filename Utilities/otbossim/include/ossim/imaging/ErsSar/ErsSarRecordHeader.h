#ifndef ErsSarRecordHeader_h
#define ErsSarRecordHeader_h

#include<iostream>


/**
 * @ingroup ErsSar
 * @brief This class is able to read a record header
 * @author Magellium, Pacome Dentraygues
 * @version 1.0
 * @date 23-11-07
 */
class ErsSarRecordHeader
{
public:
	/**
	 * @brief Constructor
	 */
	ErsSarRecordHeader();

	/**
	 * @brief Destructor
	 */
	~ErsSarRecordHeader();

	/**
	 * @brief Copy constructor
	 */
	ErsSarRecordHeader(const ErsSarRecordHeader& rhs);

	/**
	 * @brief Copy operator
	 */
	ErsSarRecordHeader& operator=(const ErsSarRecordHeader& rhs);
	/**
	 * @brief This function write the RecordHeader in a stream
	 */
	friend std::ostream& operator<<(std::ostream& os, const ErsSarRecordHeader& data);

	/**
	 * @brief This function read a RecordHeader from a stream
	 */
	friend std::istream& operator>>(std::istream& is, ErsSarRecordHeader& data);

	/**
	 * @brief Record sequence number
	 */
	int  get_rec_seq()
	{
		return _rec_seq;
	};

	/**
	 * @brief First record sub-type code
	 */
    unsigned char   get_rec_sub1()
	{
		return _rec_sub1;
	};

	/**
	 * @brief Record type code
	 */
    unsigned char   get_rec_type()
	{
		return _rec_type;
	};

	/**
	 * @brief Second record sub-type code
	 */
    unsigned char   get_rec_sub2()
	{
		return _rec_sub2;
	};

	/** 
	 * @brief Third record sub-type code
	 */
    unsigned char   get_rec_sub3()
	{
		return _rec_sub3;
	};

	/**
	 * @brief Length of this record (in bytes)
	 */
    int  get_length()
	{
		return _length;
	};
protected:

	/**
	 * @brief This function switch the LSB value and the MSB value of the parameter
	 */
	void SwitchEndian(int& value);

	/**
	 * @brief Record sequence number
	 */
	int  _rec_seq;
	/**
	 * @brief First record sub-type code
	 */
    unsigned char   _rec_sub1;
	/**
	 * @brief Record type code
	 */
    unsigned char   _rec_type;
	/**
	 * @brief Second record sub-type code
	 */
    unsigned char   _rec_sub2;
	/** 
	 * @brief Third record sub-type code
	 */
    unsigned char   _rec_sub3;
	/**
	 * @brief Length of this record (in bytes)
	 */
    int  _length;
private:


};

#endif