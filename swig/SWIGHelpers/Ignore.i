%ignore gnsstk::Triple::operator[](const size_t index);
%ignore gnsstk::Triple::operator[](const size_t index) const;
%ignore gnsstk::Triple::operator()(const size_t index);
%ignore *::operator*(double, const gnsstk::Triple&);
%ignore *::operator*(const gnsstk::Triple&, double);
%ignore gnsstk::Triple::theArray;
// Ignores on things we can't wrap
%ignore *::dump;  // takes a stream as a parameter
%ignore operator<<;
%ignore operator>>;
%ignore operator==;
%ignore operator!=;
%ignore operator&;
%ignore operator|;
%ignore operator^;
%ignore *::operator<<;
%ignore *::operator>>;
%ignore *::operator=;
%ignore *::operator++;
%ignore *::operator--;
%ignore *::operator||;
%ignore *::operator&&;
%ignore *::operator();
%ignore *::operator-();
// The gnsstk::StringUtils generate a bunch of shadowing warnings,
// since SWIG can't tell them apart.
// This may be fixed in SWIG 4.0.
%ignore gnsstk::StringUtils::asString;
%ignore gnsstk::SatMetaData::asString;
%ignore *::operator CommonTime() const;
%ignore *::operator CommonTime();
%ignore scanTime(TimeTag&, const std::string&, const std::string&);
%ignore scanTime(CommonTime&, const std::string&, const std::string&);
%ignore mixedScanTime(CommonTime&, const std::string&, const std::string&);
//
%ignore gnsstk::CivilTime::MonthNames;
%ignore gnsstk::CivilTime::MonthAbbrevNames;

%ignore gnsstk::VectorBase::operator[] (size_t i) const;
%ignore gnsstk::ConstVectorBase::operator[] (size_t i) const;
%ignore gnsstk::RefVectorBase::operator[] (size_t i);
%ignore gnsstk::Vector::operator[] (size_t i);
%ignore gnsstk::Vector::operator[] (size_t i) const;
%ignore gnsstk::Vector::operator[] (const std::slice&);
%ignore gnsstk::VectorSlice::operator[] (size_t i);
%ignore gnsstk::VectorSlice::operator[] (size_t i) const;
%ignore gnsstk::VectorSlice::operator[] (size_t i) const;
%ignore gnsstk::ConstVectorSlice::operator[] (size_t i) const;

%ignore gnsstk::Expression::print;
// why does SWIG complain about methods in private nested classes?
%ignore gnsstk::Expression::ExpNode::print;
%ignore gnsstk::Expression::ConstNode::print;
%ignore gnsstk::Expression::VarNode::print;
%ignore gnsstk::Expression::BinOpNode::print;
%ignore gnsstk::Expression::FuncOpNode::print;
%ignore gnsstk::Expression::Token::print;

// more
%ignore gnsstk::DataStatus::operator void*() const;
%ignore gnsstk::Position::operator-;
%ignore gnsstk::Position::operator+;
%ignore gnsstk::Position::operator*;
// NOT ignoring gnsstk::range causes a warning, yet it produces code
// that is and has been tested.
//%ignore gnsstk::range;
%ignore gnsstk::RefVectorBase::operator-() const;
%ignore gnsstk::Matrix::operator[](size_t);
%ignore gnsstk::Matrix::operator[](size_t) const;
%ignore gnsstk::MatrixColSlice::operator[](size_t);
%ignore gnsstk::MatrixColSlice::operator[](size_t) const;
%ignore gnsstk::ConstMatrixColSlice::operator[](size_t) const;
%ignore gnsstk::MatrixRowSlice::operator[](size_t);
%ignore gnsstk::MatrixRowSlice::operator[](size_t) const;
%ignore gnsstk::ConstMatrixRowSlice::operator[](size_t) const;
%ignore gnsstk::BinexData::UBNXI::operator unsigned long() const;
%ignore gnsstk::BinexData::MGFZI::operator long long() const;
%ignore gnsstk::CodeBuffer::operator[](int);
%ignore gnsstk::CodeBuffer::operator[](const int) const;
%ignore gnsstk::GPSZcount::operator std::string() const;
%ignore gnsstk::FormattedDouble::operator double() const;
%ignore gnsstk::FormattedDouble::operator double&();
%ignore gnsstk::FormattedDouble::operator std::string();
%ignore gnsstk::RinexNavData::operator EngEphemeris() const;
%ignore gnsstk::RinexNavData::operator GPSEphemeris() const;
%ignore gnsstk::Rinex3NavData::operator EngEphemeris() const;
%ignore gnsstk::Rinex3NavData::operator GPSEphemeris() const;
%ignore gnsstk::Rinex3NavData::operator GloEphemeris() const;
%ignore gnsstk::Rinex3NavData::operator GalEphemeris() const;
%ignore gnsstk::Rinex3NavData::operator BDSEphemeris() const;
%ignore gnsstk::Rinex3NavData::operator QZSEphemeris() const;
%ignore gnsstk::GenXSequence::operator[](int);
%ignore gnsstk::SEMData::operator AlmOrbit() const;
%ignore gnsstk::SEMData::operator OrbAlmGen() const;
%ignore gnsstk::X1Sequence::operator[](int);
%ignore gnsstk::X1Sequence::operator[](int) const;
%ignore gnsstk::X2Sequence::operator[](long);
%ignore gnsstk::YumaData::operator AlmOrbit() const;
%ignore gnsstk::YumaData::operator OrbAlmGen() const;
// friend functions that need to be specified differently to match in SWIG
%ignore identical(const Namelist&,const Namelist&);
%ignore operator-(const Position&,const Position&);
%ignore operator+(const Position&,const Position&);
%ignore operator*(const double&,const Position&);
%ignore operator*(const Position&,const double&);
%ignore operator*(const int&,const Position&);
%ignore operator*(const Position&,const int&);
%ignore operator*(double,const Triple&);
%ignore operator*(const Triple&,double);
