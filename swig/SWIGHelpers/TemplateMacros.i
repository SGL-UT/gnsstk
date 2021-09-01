// THIS FILE IS TO CREATE MACROS ONLY
// DO NOT INSTANTIATE ANY TEMPLATES IN THIS FILE
// THIS FILE SHOULD BE IMPORTED AT THE BEGINNING OF EACH MODULE'S .i FILE

// Creates a Macro to allow for quick instantiation of various vector types in python using templates
// Every gpstk::vector requires a ConstVectorBase as well as a RefVectorBase, therefore,
// we can just create macro to create all of these for us
%define VECTOR_TEMPLATE(TYPE)
 // Template directives for different vector classes
%template(ConstVectorBase_ ## TYPE) gpstk::ConstVectorBase<TYPE, gpstk::Vector<TYPE>>;
%template(RefVectorBase_ ## TYPE) gpstk::RefVectorBase<TYPE, gpstk::Vector<TYPE>>;
%template(Vector_ ## TYPE) gpstk::Vector<TYPE>;
   // Template directives for all single vector operations
%template(Vector_sum_ ## TYPE) gpstk::sum<TYPE, gpstk::Vector<TYPE>>;
%template(Vector_minabs_ ## TYPE) gpstk::minabs<TYPE, gpstk::Vector<TYPE>>;
%template(Vector_min_ ## TYPE) gpstk::min<TYPE, gpstk::Vector<TYPE>>;
/// @todo resolve whatever's causing the build to fail for Vector_maxabs
//%template(Vector_maxabs_ ## TYPE) gpstk::maxabs<TYPE, gpstk::Vector<TYPE>>;
%template(Vector_max_ ## TYPE) gpstk::max<TYPE, gpstk::Vector<TYPE>>;
%template(Vector_dotscalar_ ## TYPE) gpstk::dot<TYPE, gpstk::Vector<TYPE>>;
%template(Vector_norm_ ## TYPE) gpstk::norm<TYPE, gpstk::Vector<TYPE>>;
%template(Vector_normalize_ ## TYPE) gpstk::normalize<TYPE, gpstk::Vector<TYPE>>;
%template(Vector_RSS_ ## TYPE) gpstk::RSS<TYPE, gpstk::Vector<TYPE>>;
%template(Vector_RMS_ ## TYPE) gpstk::RMS<TYPE, gpstk::Vector<TYPE>>;
   // Template directives for VectorSlice
%template(ConstVectorBase_VectorSlice_ ## TYPE) gpstk::ConstVectorBase<TYPE, gpstk::VectorSlice<TYPE>>;
%template(ConstVectorBase_ConstVectorSlice_ ## TYPE) gpstk::ConstVectorBase<TYPE, gpstk::ConstVectorSlice<TYPE>>;
%template(RefVectorBase_ ## TYPE ## _VectorSlice) gpstk::RefVectorBase<TYPE, gpstk::VectorSlice<TYPE>>;
%template(VectorSliceBase_ ## TYPE) gpstk::VectorSliceBase<gpstk::VectorSlice<TYPE>>;
%template(ConstVectorSliceBase_ ## TYPE) gpstk::ConstVectorSliceBase<TYPE, gpstk::VectorSlice<TYPE>>;
%template(RefVectorSliceBase_ ## TYPE) gpstk::RefVectorSliceBase<TYPE, gpstk::VectorSlice<TYPE>>;
%template(VectorSlice_ ## TYPE) gpstk::VectorSlice<TYPE>;
   // Unary Operations
%template(Vector_abs_ ## TYPE) std::abs<TYPE, gpstk::Vector<TYPE>>;
%template(Vector_acos_ ## TYPE) std::acos<TYPE, gpstk::Vector<TYPE>>;
%template(Vector_asin_ ## TYPE) std::asin<TYPE, gpstk::Vector<TYPE>>;
%template(Vector_atan_ ## TYPE) std::atan<TYPE, gpstk::Vector<TYPE>>;
%template(Vector_cos_ ## TYPE) std::cos<TYPE, gpstk::Vector<TYPE>>;
%template(Vector_cosh_ ## TYPE) std::cosh<TYPE, gpstk::Vector<TYPE>>;
%template(Vector_exp_ ## TYPE) std::exp<TYPE, gpstk::Vector<TYPE>>;
%template(Vector_log_ ## TYPE) std::log<TYPE, gpstk::Vector<TYPE>>;
%template(Vector_log10_ ## TYPE) std::log10<TYPE, gpstk::Vector<TYPE>>;
%template(Vector_sinh_ ## TYPE) std::sinh<TYPE, gpstk::Vector<TYPE>>;
%template(Vector_sin_ ## TYPE) std::sin<TYPE, gpstk::Vector<TYPE>>;
%template(Vector_sqrt_ ## TYPE) std::sqrt<TYPE, gpstk::Vector<TYPE>>;
%template(Vector_tan_ ## TYPE) std::tan<TYPE, gpstk::Vector<TYPE>>;
%template(Vector_tanh_ ## TYPE) std::tanh<TYPE, gpstk::Vector<TYPE>>;
   // Template directive for the iterator
%template(VectorIterator_ ## TYPE) VectorIterator<TYPE>;
%enddef

// Binary vector Operation Template Macro to add template directives for all types
// Note this function must belong to the STD namespace
%define TRANSCBINVECTOROP_TEMPLATE(TYPE, FUN)
%template(Vector_ ## FUN ## _ ## TYPE ## _Vector_ ## TYPE) std::FUN<TYPE, gpstk::Vector<TYPE>>;
%template(Vector_ ## FUN ## _Vector_ ## TYPE ## _Vector_ ## TYPE) std::FUN<TYPE, gpstk::Vector<TYPE>, gpstk::Vector<TYPE>>;
%enddef

// Macro for adding binary vector operations
// Note this function must belong to the GPSTK namespace
%define BINVECTOROP_TEMPLATE(TYPE, FUN)
%template(Vector_ ## FUN ## _Vector_ ## TYPE ## _Vector_ ## TYPE) gpstk::FUN<TYPE, gpstk::Vector<TYPE>, gpstk::Vector<TYPE>>;
%enddef

// Create a macro for matrix type creation
%define MATRIX_TEMPLATE(TYPE)
 // Matrix Base templates for Matrix
%template(ConstMatrixBase_ ## TYPE ## _Matrix) gpstk::ConstMatrixBase<TYPE, gpstk::Matrix<TYPE>>;
%template(RefMatrixBase_ ## TYPE ## _Matrix) gpstk::RefMatrixBase<TYPE, gpstk::Matrix<TYPE>>;
   // Matrix Base templates for Matrix Slices
%template(ConstMatrixBase_ ## TYPE ## _ConstMatrixSlice) gpstk::ConstMatrixBase<TYPE, gpstk::ConstMatrixSlice<TYPE>>;
%template(ConstMatrixBase_ ## TYPE ## _MatrixSlice) gpstk::ConstMatrixBase<TYPE, gpstk::MatrixSlice<TYPE>>;
%template(RefMatrixBase_ ## TYPE ## _MatrixSlice ) gpstk::RefMatrixBase<TYPE, gpstk::MatrixSlice<TYPE>>;
%template(ConstMatrixBase_ ## TYPE ## _ConstMatrixColSlice) gpstk::ConstMatrixBase<TYPE, gpstk::ConstMatrixColSlice<TYPE>>;
%template(ConstMatrixBase_ ## TYPE ## _MatrixColSlice) gpstk::ConstMatrixBase<TYPE, gpstk::MatrixColSlice<TYPE>>;
%template(RefMatrixBase_ ## TYPE ## _MatrixColSlice) gpstk::RefMatrixBase<TYPE, gpstk::MatrixColSlice<TYPE>>;
%template(ConstMatrixBase_ ## TYPE ## _ConstMatrixRowSlice) gpstk::ConstMatrixBase<TYPE, gpstk::ConstMatrixRowSlice<TYPE>>;
%template(ConstMatrixBase_ ## TYPE ## _MatrixRowSlice) gpstk::ConstMatrixBase<TYPE, gpstk::MatrixRowSlice<TYPE>>;
%template(RefMatrixBase_ ## TYPE ## _MatrixRowSlice) gpstk::RefMatrixBase<TYPE, gpstk::MatrixRowSlice<TYPE>>;
   // Matrix template
%template(Matrix_ ## TYPE) gpstk::Matrix<TYPE>;
   // Matrix Slice Base templates
%template(MatrixSliceBase_ ## TYPE ## _MatrixSlice) gpstk::MatrixSliceBase<TYPE, gpstk::MatrixSlice<TYPE>>;
%template(MatrixSliceBase_ ## TYPE ## _ConstMatrixSlice) gpstk::MatrixSliceBase<TYPE, gpstk::ConstMatrixSlice<TYPE>>;
%template(MatrixSliceBase_ ## TYPE ## _MatrixColSlice) gpstk::MatrixSliceBase<TYPE, gpstk::MatrixColSlice<TYPE>>;
%template(MatrixSliceBase_ ## TYPE ## _ConstMatrixColSlice) gpstk::MatrixSliceBase<TYPE, gpstk::ConstMatrixColSlice<TYPE>>;
%template(MatrixSliceBase_ ## TYPE ## _MatrixRowSlice) gpstk::MatrixSliceBase<TYPE, gpstk::MatrixRowSlice<TYPE>>;
%template(MatrixSliceBase_ ## TYPE ## _ConstMatrixRowSlice) gpstk::MatrixSliceBase<TYPE, gpstk::ConstMatrixRowSlice<TYPE>>;
   // Ref and Const Matrix Slice Base templates
%template(RefMatrixSliceBase_ ## TYPE ## _MatrixSlice) gpstk::RefMatrixSliceBase<TYPE, gpstk::MatrixSlice<TYPE>>;
%template(ConstMatrixSliceBase_ ## TYPE ## _ConstrMatrixSlice) gpstk::ConstMatrixSliceBase<TYPE, gpstk::ConstMatrixSlice<TYPE>>;
%template(RefMatrixSliceBase_ ## TYPE ## _MatrixColSlice) gpstk::RefMatrixSliceBase<TYPE, gpstk::MatrixColSlice<TYPE>>;
%template(ConstMatrixSliceBase_ ## TYPE ## _ConstMatrixColSlice) gpstk::ConstMatrixSliceBase<TYPE, gpstk::ConstMatrixColSlice<TYPE>>;
%template(RefMatrixSliceBase_ ## TYPE ## _MatrixRowSlice) gpstk::RefMatrixSliceBase<TYPE, gpstk::MatrixRowSlice<TYPE>>;
%template(ConstMatrixSliceBase_ ## TYPE ## _ConstMatrixRowSlice) gpstk::ConstMatrixSliceBase<TYPE, gpstk::ConstMatrixRowSlice<TYPE>>;
   // Matrix Slice Templates
%template(MatrixSlice_ ## TYPE) gpstk::MatrixSlice<TYPE>;
%template(ConstMatrixSlice_ ## TYPE) gpstk::ConstMatrixSlice<TYPE>;
%template(MatrixRowSlice_ ## TYPE) gpstk::MatrixRowSlice<TYPE>;
%template(ConstMatrixRowSlice_ ## TYPE) gpstk::ConstMatrixRowSlice<TYPE>;
%template(MatrixColSlice_ ## TYPE) gpstk::MatrixColSlice<TYPE>;
%template(ConstMatrixColSlice_ ## TYPE) gpstk::ConstMatrixColSlice<TYPE>;
   // MatrixBase Operators
%template(ident_transform_ ## TYPE) gpstk::ident<TYPE, gpstk::Matrix<TYPE>>;
%template(trace_ ## TYPE) gpstk::trace<TYPE, gpstk::Matrix<TYPE>>;
%template(normF_ ## TYPE) gpstk::normF<TYPE, gpstk::Matrix<TYPE>>;
%template(normCol_ ## TYPE) gpstk::normCol<TYPE, gpstk::Matrix<TYPE>>;
%template(slowDet_ ## TYPE) gpstk::slowDet<TYPE, gpstk::Matrix<TYPE>>;

   // Unary Matrix Operators
%template(minorMatrix_ ## TYPE) gpstk::minorMatrix<TYPE, gpstk::Matrix<TYPE>>;
%template(transpose_ ## TYPE) gpstk::transpose<TYPE, gpstk::Matrix<TYPE>>;
%template(det_ ## TYPE) gpstk::det<TYPE, gpstk::Matrix<TYPE>>;
%template(condNum_ ## TYPE) gpstk::condNum<TYPE, gpstk::Matrix<TYPE>>;
%template(ident_ ## TYPE) gpstk::ident<TYPE>;
%template(diag_ ## TYPE) gpstk::diag<TYPE, gpstk::Matrix<TYPE>>;
%template(blkdiag_ ## TYPE) gpstk::blkdiag<TYPE, gpstk::Matrix<TYPE>>;
%template(rotation_ ## TYPE) gpstk::rotation<TYPE>;
%template(inverse_ ## TYPE) gpstk::inverse<TYPE, gpstk::Matrix<TYPE>>;
%template(outer_ ## TYPE) gpstk::outer<TYPE, gpstk::Vector<TYPE>>;
%template(matrixmaxabs_ ## TYPE) gpstk::matrixmaxabs<TYPE, gpstk::Matrix<TYPE>>;
%enddef

%define NONINT_MATRIX_FUNCTION_TEMPLATES(TYPE)
 // Matrix Functors
%template(SVD_ ## TYPE) gpstk::SVD<TYPE>;
%template(LUDecomp_ ## TYPE) gpstk::LUDecomp<TYPE>;
%template(Cholesky_ ## TYPE) gpstk::Cholesky<TYPE>;
%template(CholeskyCrout_ ## TYPE) gpstk::CholeskyCrout<TYPE>;
%template(HouseHolder_ ## TYPE) gpstk::Householder<TYPE>;
   // Matrix Functions
%template(inverseLUD_ ## TYPE) gpstk::inverseLUD<TYPE, gpstk::Matrix<TYPE>>;
%template(inverseSVD_ ## TYPE) gpstk::inverseSVD<TYPE, gpstk::Matrix<TYPE>>;
%template(inverseChol_ ## TYPE) gpstk::inverseChol<TYPE, gpstk::Matrix<TYPE>>;
%enddef

// Create a python conversion of all signed valid C types with this macro
// int, double, float, char, short, long
%define VALIDTYPE_TEMPLATE(TYPE)
%template(ValidType_ ## TYPE) gpstk::ValidType<TYPE>;
%enddef

// Create a python conversion of all valid C types with this macro
// uchar, ushort, uint, ulong
%define UNSIGNED_VALIDTYPE_TEMPLATE(TYPE)
%template(ValidType_ ## u ## TYPE) ValidType<unsigned TYPE>;
%enddef
