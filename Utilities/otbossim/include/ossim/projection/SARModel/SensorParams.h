#ifndef SensorParams_h
#define SensorParams_h

/**
 * @ingroup SARModel
 * @brief This class handle the sensor parameters
 * @author Magellium, Pacome Dentraygues
 * @version 1.0
 * @date 15-01-08
 */
 
 class SensorParams
 {
	enum SightDirection
	{
		Left,
		Right
	};
	
 public:
	/**
	 * @brief Constructor
	 */
	SensorParams();
	/**
	 * @brief Destructor
	 */
	~SensorParams();
	
	/** 
	 *@brief Copy constructor
	 */
	SensorParams(const SensorParams& rhs);
	
	/**
	 * @brief Affectation operator
	 */
	SensorParams& operator=(const SensorParams& rhs);
	
	double get_prf()
	{
		return _prf;
	};
	
	double get_sf()
	{
		return _sf;
	};
	
	double get_rwl()
	{
		return _rwl;
	};
	
	void set_prf(double prf)
	{
		_prf = prf;
	};
	
	void set_sf(double sf)
	{
		_sf = sf;
	};
	
	void set_rwl(double rwl)
	{
		_rwl = rwl;
	};

	SightDirection get_sightDirection()
	{
		return _sightDirection;
	};

	SightDirection set_sightDirection(SightDirection sight)
	{
		_sightDirection = sight;
	};
	
	virtual SensorParams* Clone()
	{
		return new SensorParams(*this);
	};

	int get_col_direction()
	{
		return _col_direction;
	};

	int get_lin_direction()
	{
		return _lin_direction;
	};

	void set_col_direction(int dir)
	{
		_col_direction = dir;
	};

	void set_lin_direction(int dir)
	{
		_lin_direction = dir;
	};
 protected:
	/**
	 * @brief Nominal PRF, Hz
	 */
	double _prf;
	
	/**
	 * @brief Sampling frequency
	 */
	double _sf;
	
	/**
	 * @brief Radar wave length
	 */
	double _rwl;

	int _col_direction;
	int _lin_direction;
	SightDirection _sightDirection;
 private:
 };
 
#endif