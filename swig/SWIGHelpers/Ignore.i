%ignore gpstk::Triple::operator[](const size_t index);
%ignore gpstk::Triple::operator[](const size_t index) const;
%ignore gpstk::Triple::operator()(const size_t index);
%ignore *::operator*(double, const gpstk::Triple&);
%ignore *::operator*(const gpstk::Triple&, double);
%ignore gpstk::Triple::theArray;
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
// The gpstk::StringUtils generate a bunch of shadowing warnings,
// since SWIG can't tell them apart.
// This may be fixed in SWIG 4.0.
%ignore gpstk::StringUtils::asString;
%ignore gpstk::SatMetaData::asString;
%ignore *::operator CommonTime() const;
%ignore *::operator CommonTime();
%ignore scanTime(TimeTag&, const std::string&, const std::string&);
%ignore scanTime(CommonTime&, const std::string&, const std::string&);
%ignore mixedScanTime(CommonTime&, const std::string&, const std::string&);
//
%ignore gpstk::CivilTime::MonthNames;
%ignore gpstk::CivilTime::MonthAbbrevNames;

%ignore gpstk::VectorBase::operator[] (size_t i) const;
%ignore gpstk::ConstVectorBase::operator[] (size_t i) const;
%ignore gpstk::RefVectorBase::operator[] (size_t i);
%ignore gpstk::Vector::operator[] (size_t i);
%ignore gpstk::Vector::operator[] (size_t i) const;
%ignore gpstk::Vector::operator[] (const std::slice&);
%ignore gpstk::VectorSlice::operator[] (size_t i);
%ignore gpstk::VectorSlice::operator[] (size_t i) const;
%ignore gpstk::VectorSlice::operator[] (size_t i) const;
%ignore gpstk::ConstVectorSlice::operator[] (size_t i) const;

%ignore gpstk::Expression::print;
// why does SWIG complain about methods in private nested classes?
%ignore gpstk::Expression::ExpNode::print;
%ignore gpstk::Expression::ConstNode::print;
%ignore gpstk::Expression::VarNode::print;
%ignore gpstk::Expression::BinOpNode::print;
%ignore gpstk::Expression::FuncOpNode::print;
%ignore gpstk::Expression::Token::print;

// more
%ignore gpstk::DataStatus::operator void*() const;
%ignore gpstk::Position::operator-;
%ignore gpstk::Position::operator+;
%ignore gpstk::Position::operator*;
// NOT ignoring gpstk::range causes a warning, yet it produces code
// that is and has been tested.
//%ignore gpstk::range;
%ignore gpstk::RefVectorBase::operator-() const;
%ignore gpstk::Matrix::operator[](size_t);
%ignore gpstk::Matrix::operator[](size_t) const;
%ignore gpstk::MatrixColSlice::operator[](size_t);
%ignore gpstk::MatrixColSlice::operator[](size_t) const;
%ignore gpstk::ConstMatrixColSlice::operator[](size_t) const;
%ignore gpstk::MatrixRowSlice::operator[](size_t);
%ignore gpstk::MatrixRowSlice::operator[](size_t) const;
%ignore gpstk::ConstMatrixRowSlice::operator[](size_t) const;
%ignore gpstk::BinexData::UBNXI::operator unsigned long() const;
%ignore gpstk::BinexData::MGFZI::operator long long() const;
%ignore gpstk::CodeBuffer::operator[](int);
%ignore gpstk::CodeBuffer::operator[](const int) const;
%ignore gpstk::GPSZcount::operator std::string() const;
%ignore gpstk::FormattedDouble::operator double() const;
%ignore gpstk::FormattedDouble::operator double&();
%ignore gpstk::FormattedDouble::operator std::string();
%ignore gpstk::RinexNavData::operator EngEphemeris() const;
%ignore gpstk::RinexNavData::operator GPSEphemeris() const;
%ignore gpstk::Rinex3NavData::operator EngEphemeris() const;
%ignore gpstk::Rinex3NavData::operator GPSEphemeris() const;
%ignore gpstk::Rinex3NavData::operator GloEphemeris() const;
%ignore gpstk::Rinex3NavData::operator GalEphemeris() const;
%ignore gpstk::Rinex3NavData::operator BDSEphemeris() const;
%ignore gpstk::Rinex3NavData::operator QZSEphemeris() const;
%ignore gpstk::GenXSequence::operator[](int);
%ignore gpstk::SEMData::operator AlmOrbit() const;
%ignore gpstk::SEMData::operator OrbAlmGen() const;
%ignore gpstk::X1Sequence::operator[](int);
%ignore gpstk::X1Sequence::operator[](int) const;
%ignore gpstk::X2Sequence::operator[](long);
%ignore gpstk::YumaData::operator AlmOrbit() const;
%ignore gpstk::YumaData::operator OrbAlmGen() const;
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
