// THIS FILE IS TO CREATE MACROS ONLY
// DO NOT INSTANTIATE ANY TEMPLATES IN THIS FILE
// THIS FILE SHOULD BE IMPORTED AT THE BEGINNING OF EACH MODULE'S .i FILE

// Creates a Macro to allow for quick instantiation of various vector types in python using templates
// Every gnsstk::vector requires a ConstVectorBase as well as a RefVectorBase, therefore,
// we can just create macro to create all of these for us
%define VECTOR_TEMPLATE(TYPE)
 // Template directives for different vector classes
%template(ConstVectorBase_ ## TYPE) gnsstk::ConstVectorBase<TYPE, gnsstk::Vector<TYPE>>;
%template(RefVectorBase_ ## TYPE) gnsstk::RefVectorBase<TYPE, gnsstk::Vector<TYPE>>;
%template(Vector_ ## TYPE) gnsstk::Vector<TYPE>;
   // Template directives for all single vector operations
%template(Vector_sum_ ## TYPE) gnsstk::sum<TYPE, gnsstk::Vector<TYPE>>;
%template(Vector_minabs_ ## TYPE) gnsstk::minabs<TYPE, gnsstk::Vector<TYPE>>;
%template(Vector_min_ ## TYPE) gnsstk::min<TYPE, gnsstk::Vector<TYPE>>;
/// @todo resolve whatever's causing the build to fail for Vector_maxabs
//%template(Vector_maxabs_ ## TYPE) gnsstk::maxabs<TYPE, gnsstk::Vector<TYPE>>;
%template(Vector_max_ ## TYPE) gnsstk::max<TYPE, gnsstk::Vector<TYPE>>;
%template(Vector_dotscalar_ ## TYPE) gnsstk::dot<TYPE, gnsstk::Vector<TYPE>>;
%template(Vector_norm_ ## TYPE) gnsstk::norm<TYPE, gnsstk::Vector<TYPE>>;
%template(Vector_normalize_ ## TYPE) gnsstk::normalize<TYPE, gnsstk::Vector<TYPE>>;
%template(Vector_RSS_ ## TYPE) gnsstk::RSS<TYPE, gnsstk::Vector<TYPE>>;
%template(Vector_RMS_ ## TYPE) gnsstk::RMS<TYPE, gnsstk::Vector<TYPE>>;
   // Template directives for VectorSlice
%template(ConstVectorBase_VectorSlice_ ## TYPE) gnsstk::ConstVectorBase<TYPE, gnsstk::VectorSlice<TYPE>>;
%template(ConstVectorBase_ConstVectorSlice_ ## TYPE) gnsstk::ConstVectorBase<TYPE, gnsstk::ConstVectorSlice<TYPE>>;
%template(RefVectorBase_ ## TYPE ## _VectorSlice) gnsstk::RefVectorBase<TYPE, gnsstk::VectorSlice<TYPE>>;
%template(VectorSliceBase_ ## TYPE) gnsstk::VectorSliceBase<gnsstk::VectorSlice<TYPE>>;
%template(ConstVectorSliceBase_ ## TYPE) gnsstk::ConstVectorSliceBase<TYPE, gnsstk::VectorSlice<TYPE>>;
%template(RefVectorSliceBase_ ## TYPE) gnsstk::RefVectorSliceBase<TYPE, gnsstk::VectorSlice<TYPE>>;
%template(VectorSlice_ ## TYPE) gnsstk::VectorSlice<TYPE>;
   // Unary Operations
%template(Vector_abs_ ## TYPE) std::abs<TYPE, gnsstk::Vector<TYPE>>;
%template(Vector_acos_ ## TYPE) std::acos<TYPE, gnsstk::Vector<TYPE>>;
%template(Vector_asin_ ## TYPE) std::asin<TYPE, gnsstk::Vector<TYPE>>;
%template(Vector_atan_ ## TYPE) std::atan<TYPE, gnsstk::Vector<TYPE>>;
%template(Vector_cos_ ## TYPE) std::cos<TYPE, gnsstk::Vector<TYPE>>;
%template(Vector_cosh_ ## TYPE) std::cosh<TYPE, gnsstk::Vector<TYPE>>;
%template(Vector_exp_ ## TYPE) std::exp<TYPE, gnsstk::Vector<TYPE>>;
%template(Vector_log_ ## TYPE) std::log<TYPE, gnsstk::Vector<TYPE>>;
%template(Vector_log10_ ## TYPE) std::log10<TYPE, gnsstk::Vector<TYPE>>;
%template(Vector_sinh_ ## TYPE) std::sinh<TYPE, gnsstk::Vector<TYPE>>;
%template(Vector_sin_ ## TYPE) std::sin<TYPE, gnsstk::Vector<TYPE>>;
%template(Vector_sqrt_ ## TYPE) std::sqrt<TYPE, gnsstk::Vector<TYPE>>;
%template(Vector_tan_ ## TYPE) std::tan<TYPE, gnsstk::Vector<TYPE>>;
%template(Vector_tanh_ ## TYPE) std::tanh<TYPE, gnsstk::Vector<TYPE>>;
   // Template directive for the iterator
%template(VectorIterator_ ## TYPE) VectorIterator<TYPE>;
%enddef

// Binary vector Operation Template Macro to add template directives for all types
// Note this function must belong to the STD namespace
%define TRANSCBINVECTOROP_TEMPLATE(TYPE, FUN)
%template(Vector_ ## FUN ## _ ## TYPE ## _Vector_ ## TYPE) std::FUN<TYPE, gnsstk::Vector<TYPE>>;
%template(Vector_ ## FUN ## _Vector_ ## TYPE ## _Vector_ ## TYPE) std::FUN<TYPE, gnsstk::Vector<TYPE>, gnsstk::Vector<TYPE>>;
%enddef

// Macro for adding binary vector operations
// Note this function must belong to the GNSSTK namespace
%define BINVECTOROP_TEMPLATE(TYPE, FUN)
%template(Vector_ ## FUN ## _Vector_ ## TYPE ## _Vector_ ## TYPE) gnsstk::FUN<TYPE, gnsstk::Vector<TYPE>, gnsstk::Vector<TYPE>>;
%enddef

// Create a macro for matrix type creation
%define MATRIX_TEMPLATE(TYPE)
 // Matrix Base templates for Matrix
%template(ConstMatrixBase_ ## TYPE ## _Matrix) gnsstk::ConstMatrixBase<TYPE, gnsstk::Matrix<TYPE>>;
%template(RefMatrixBase_ ## TYPE ## _Matrix) gnsstk::RefMatrixBase<TYPE, gnsstk::Matrix<TYPE>>;
   // Matrix Base templates for Matrix Slices
%template(ConstMatrixBase_ ## TYPE ## _ConstMatrixSlice) gnsstk::ConstMatrixBase<TYPE, gnsstk::ConstMatrixSlice<TYPE>>;
%template(ConstMatrixBase_ ## TYPE ## _MatrixSlice) gnsstk::ConstMatrixBase<TYPE, gnsstk::MatrixSlice<TYPE>>;
%template(RefMatrixBase_ ## TYPE ## _MatrixSlice ) gnsstk::RefMatrixBase<TYPE, gnsstk::MatrixSlice<TYPE>>;
%template(ConstMatrixBase_ ## TYPE ## _ConstMatrixColSlice) gnsstk::ConstMatrixBase<TYPE, gnsstk::ConstMatrixColSlice<TYPE>>;
%template(ConstMatrixBase_ ## TYPE ## _MatrixColSlice) gnsstk::ConstMatrixBase<TYPE, gnsstk::MatrixColSlice<TYPE>>;
%template(RefMatrixBase_ ## TYPE ## _MatrixColSlice) gnsstk::RefMatrixBase<TYPE, gnsstk::MatrixColSlice<TYPE>>;
%template(ConstMatrixBase_ ## TYPE ## _ConstMatrixRowSlice) gnsstk::ConstMatrixBase<TYPE, gnsstk::ConstMatrixRowSlice<TYPE>>;
%template(ConstMatrixBase_ ## TYPE ## _MatrixRowSlice) gnsstk::ConstMatrixBase<TYPE, gnsstk::MatrixRowSlice<TYPE>>;
%template(RefMatrixBase_ ## TYPE ## _MatrixRowSlice) gnsstk::RefMatrixBase<TYPE, gnsstk::MatrixRowSlice<TYPE>>;
   // Matrix template
%template(Matrix_ ## TYPE) gnsstk::Matrix<TYPE>;
   // Matrix Slice Base templates
%template(MatrixSliceBase_ ## TYPE ## _MatrixSlice) gnsstk::MatrixSliceBase<TYPE, gnsstk::MatrixSlice<TYPE>>;
%template(MatrixSliceBase_ ## TYPE ## _ConstMatrixSlice) gnsstk::MatrixSliceBase<TYPE, gnsstk::ConstMatrixSlice<TYPE>>;
%template(MatrixSliceBase_ ## TYPE ## _MatrixColSlice) gnsstk::MatrixSliceBase<TYPE, gnsstk::MatrixColSlice<TYPE>>;
%template(MatrixSliceBase_ ## TYPE ## _ConstMatrixColSlice) gnsstk::MatrixSliceBase<TYPE, gnsstk::ConstMatrixColSlice<TYPE>>;
%template(MatrixSliceBase_ ## TYPE ## _MatrixRowSlice) gnsstk::MatrixSliceBase<TYPE, gnsstk::MatrixRowSlice<TYPE>>;
%template(MatrixSliceBase_ ## TYPE ## _ConstMatrixRowSlice) gnsstk::MatrixSliceBase<TYPE, gnsstk::ConstMatrixRowSlice<TYPE>>;
   // Ref and Const Matrix Slice Base templates
%template(RefMatrixSliceBase_ ## TYPE ## _MatrixSlice) gnsstk::RefMatrixSliceBase<TYPE, gnsstk::MatrixSlice<TYPE>>;
%template(ConstMatrixSliceBase_ ## TYPE ## _ConstrMatrixSlice) gnsstk::ConstMatrixSliceBase<TYPE, gnsstk::ConstMatrixSlice<TYPE>>;
%template(RefMatrixSliceBase_ ## TYPE ## _MatrixColSlice) gnsstk::RefMatrixSliceBase<TYPE, gnsstk::MatrixColSlice<TYPE>>;
%template(ConstMatrixSliceBase_ ## TYPE ## _ConstMatrixColSlice) gnsstk::ConstMatrixSliceBase<TYPE, gnsstk::ConstMatrixColSlice<TYPE>>;
%template(RefMatrixSliceBase_ ## TYPE ## _MatrixRowSlice) gnsstk::RefMatrixSliceBase<TYPE, gnsstk::MatrixRowSlice<TYPE>>;
%template(ConstMatrixSliceBase_ ## TYPE ## _ConstMatrixRowSlice) gnsstk::ConstMatrixSliceBase<TYPE, gnsstk::ConstMatrixRowSlice<TYPE>>;
   // Matrix Slice Templates
%template(MatrixSlice_ ## TYPE) gnsstk::MatrixSlice<TYPE>;
%template(ConstMatrixSlice_ ## TYPE) gnsstk::ConstMatrixSlice<TYPE>;
%template(MatrixRowSlice_ ## TYPE) gnsstk::MatrixRowSlice<TYPE>;
%template(ConstMatrixRowSlice_ ## TYPE) gnsstk::ConstMatrixRowSlice<TYPE>;
%template(MatrixColSlice_ ## TYPE) gnsstk::MatrixColSlice<TYPE>;
%template(ConstMatrixColSlice_ ## TYPE) gnsstk::ConstMatrixColSlice<TYPE>;
   // MatrixBase Operators
%template(ident_transform_ ## TYPE) gnsstk::ident<TYPE, gnsstk::Matrix<TYPE>>;
%template(trace_ ## TYPE) gnsstk::trace<TYPE, gnsstk::Matrix<TYPE>>;
%template(normF_ ## TYPE) gnsstk::normF<TYPE, gnsstk::Matrix<TYPE>>;
%template(normCol_ ## TYPE) gnsstk::normCol<TYPE, gnsstk::Matrix<TYPE>>;
%template(slowDet_ ## TYPE) gnsstk::slowDet<TYPE, gnsstk::Matrix<TYPE>>;

   // Unary Matrix Operators
%template(minorMatrix_ ## TYPE) gnsstk::minorMatrix<TYPE, gnsstk::Matrix<TYPE>>;
%template(transpose_ ## TYPE) gnsstk::transpose<TYPE, gnsstk::Matrix<TYPE>>;
%template(det_ ## TYPE) gnsstk::det<TYPE, gnsstk::Matrix<TYPE>>;
%template(condNum_ ## TYPE) gnsstk::condNum<TYPE, gnsstk::Matrix<TYPE>>;
%template(ident_ ## TYPE) gnsstk::ident<TYPE>;
%template(diag_ ## TYPE) gnsstk::diag<TYPE, gnsstk::Matrix<TYPE>>;
%template(blkdiag_ ## TYPE) gnsstk::blkdiag<TYPE, gnsstk::Matrix<TYPE>>;
%template(rotation_ ## TYPE) gnsstk::rotation<TYPE>;
%template(inverse_ ## TYPE) gnsstk::inverse<TYPE, gnsstk::Matrix<TYPE>>;
%template(outer_ ## TYPE) gnsstk::outer<TYPE, gnsstk::Vector<TYPE>>;
%template(matrixmaxabs_ ## TYPE) gnsstk::matrixmaxabs<TYPE, gnsstk::Matrix<TYPE>>;
%enddef

%define NONINT_MATRIX_FUNCTION_TEMPLATES(TYPE)
 // Matrix Functors
%template(SVD_ ## TYPE) gnsstk::SVD<TYPE>;
%template(LUDecomp_ ## TYPE) gnsstk::LUDecomp<TYPE>;
%template(Cholesky_ ## TYPE) gnsstk::Cholesky<TYPE>;
%template(CholeskyCrout_ ## TYPE) gnsstk::CholeskyCrout<TYPE>;
%template(HouseHolder_ ## TYPE) gnsstk::Householder<TYPE>;
   // Matrix Functions
%template(inverseLUD_ ## TYPE) gnsstk::inverseLUD<TYPE, gnsstk::Matrix<TYPE>>;
%template(inverseSVD_ ## TYPE) gnsstk::inverseSVD<TYPE, gnsstk::Matrix<TYPE>>;
%template(inverseChol_ ## TYPE) gnsstk::inverseChol<TYPE, gnsstk::Matrix<TYPE>>;
%enddef

// Create a python conversion of all signed valid C types with this macro
// int, double, float, char, short, long
%define VALIDTYPE_TEMPLATE(TYPE)
%template(ValidType_ ## TYPE) gnsstk::ValidType<TYPE>;
%enddef

// Create a python conversion of all valid C types with this macro
// uchar, ushort, uint, ulong
%define UNSIGNED_VALIDTYPE_TEMPLATE(TYPE)
%template(ValidType_ ## u ## TYPE) ValidType<unsigned TYPE>;
%enddef
