%include "MatrixFunctors.hpp"
%include "MatrixOperators.hpp"
%include "TemplateMacros.i"

%inline %{
   namespace gnsstk
   {
         //Matrix version of maxabs
         //This is necessary since the vector version has the same name and SWIG
         //automatically tries to wrap the vector version with matrix arguments
      template <class T, class BaseClass>
      T matrixmaxabs(const gnsstk::ConstMatrixBase<T, BaseClass>& a)
      {
         return maxabs(a);
      }
   }
   %}

%extend gnsstk::Matrix
{
      // Creates a function to call matrix version of assignFrom because the
      // template is not wrapping correctly
   gnsstk::Matrix& assignFromMatrix(const gnsstk::Matrix& mat)
   {
      return $self->operator=(mat);
   }

      // Creates a function to call vector version of assignFrom because the
      // template is not wrapping correctly
   gnsstk::Matrix& assignFromVector(const gnsstk::Vector<T>& x)
   {
      return $self->assignFrom(x);
   }

      // Gets a value from the matrix given row and column index
   T get_value(int i, int j)
   {
         // Throw an exception if the index is out of the bounds
      CHECK_INDEX(i, 0, $self->rows());
      CHECK_INDEX(j, 0, $self->cols());

      return (*($self))(i, j);
   }

      // Python Special Functions
      // Implements Python __getitem__ to get a row slice at a given index
   gnsstk::MatrixRowSlice<T> __getitem__(size_t i)
   {
         // Throw an exception if the index is out of the bounds
      CHECK_INDEX(i, 0, $self->rows());

      return (*($self))[i];
   }

      // Implements Python __add__ for + operation with another matrix
   gnsstk::Matrix<T> __add__(const gnsstk::Matrix& mat)
   {
      return *($self) + mat;
   }

      // Implements Python __add__ for + operation with a value
   gnsstk::Matrix<T> __add__(const T d)
   {
      return *($self) + d;
   }

      // Implements Python __iadd__ for += operation with another matrix
   gnsstk::Matrix<T> __iadd__(const gnsstk::Matrix& mat)
   {
      return *($self) += mat;
   }

      // Implements Python __iadd__ for += operation with a vector
   gnsstk::Matrix<T> __iadd__(const gnsstk::Vector<T>& vec)
   {
      return *($self) += vec;
   }

      // Implements Python __iadd__ for += operation with pointer array
   gnsstk::Matrix<T> __iadd__(const T* x)
   {
      return *($self) += x;
   }

      // Implements Python __iadd__ for += operation with value
   gnsstk::Matrix<T> __iadd__(T x)
   {
      return *($self) += x;
   }

      // Implements Python __sub__ for - operation with a matrix
   gnsstk::Matrix<T> __sub__(const gnsstk::Matrix& mat)
   {
      return *($self) - mat;
   }

      // Implements Python __sub__ for - operation with a value
   gnsstk::Matrix<T> __sub__(const T d)
   {
      return *($self) - d;
   }

      // Implements Python __isub__ for -= operation with another matrix
   gnsstk::Matrix<T> __isub__(const gnsstk::Matrix& mat)
   {
      return *($self) -= mat;
   }

      // Implements Python __isub__ for -= operation with a vector
   gnsstk::Matrix<T> __isub__(const gnsstk::Vector<T>& vec)
   {
      return *($self) -= vec;
   }

      // Implements Python __isub__ for -= operation with pointer array
   gnsstk::Matrix<T> __isub__(const T* x)
   {
      return *($self) -= x;
   }

      // Implements Python __isub__ for -= operation with value
   gnsstk::Matrix<T> __isub__(T x)
   {
      return *($self) -= x;
   }

      // Implements Python __mul__ for elementwise matrix multiplication by a value
   gnsstk::Matrix<T> __mul__(const T d)
   {
      return *($self) * d;
   }

      // Implements Python __mul__ for matrix-matrix multiplication
   gnsstk::Matrix<T> __mul__(const gnsstk::Matrix& mat)
   {
      return *($self) * mat;
   }

      // Implements Python __mul__ for matrix-vector multiplication
   gnsstk::Vector<T> __mul__(const gnsstk::Vector<T>& vec)
   {
      return *($self) * vec;
   }

      // Implements Python __imul__ for *= operation with value
   gnsstk::Matrix<T> __imul__(const T x)
   {
      return *($self) *= x;
   }

      // Implements Python __itruediv__ for /= operation with value
   gnsstk::Matrix<T> __itruediv__(const T x)
   {
      return *($self) /= x;
   }

//        // Implements Python __truediv__ for elementwise / operation by a value
   gnsstk::Matrix<T> __truediv__(const T d)
   {
      return *($self) / d;
   }

      // Implements top bottom concatenation with another matrix
   gnsstk::Matrix<T> stack(const gnsstk::ConstMatrixBase<T, gnsstk::Matrix<T>>& b)

   {
      return *($self) && b;
   }

      // Implements top bottom concatenation with another vector
   gnsstk::Matrix<T> stack(const gnsstk::ConstVectorBase<T, gnsstk::Vector<T>>& b)

   {
      return *($self) && b;
   }

      // Implements left to right concatenation with another matrix
   gnsstk::Matrix<T> concatenate(const gnsstk::ConstMatrixBase<T, gnsstk::Matrix<T>>& r)

   {
      return *($self) || r;
   }

      // Implements left to right concatenation with another vector
   gnsstk::Matrix<T> concatenate(const gnsstk::ConstVectorBase<T, gnsstk::Vector<T>>& r)

   {
      return *($self) || r;
   }
}

   // Add functions to implement vector functions that use matrices or return matrices
%extend gnsstk::Vector
{
      // Implements top bottom concatenation with another matrix
   gnsstk::Matrix<T> stack(const gnsstk::ConstMatrixBase<T, gnsstk::Matrix<T>>& b)

   {
      return *($self) && b;
   }
      // Implements left to right concatenation with another matrix
   gnsstk::Matrix<T> concatenate(const gnsstk::ConstMatrixBase<T, gnsstk::Matrix<T>>& r)

   {
      return *($self) || r;
   }
      // Implements left to right concatenation with another matrix
   gnsstk::Matrix<T> concatenate(const gnsstk::ConstVectorBase<T, gnsstk::Vector<T>>& r)

   {
      return *($self) || r;
   }

      // Implements Python __mul__ for matrix-vector multiplication
   gnsstk::Vector<T> __mul__(const gnsstk::Matrix<T>& mat)
   {
      return *($self) * mat;
   }
}

%extend gnsstk::MatrixColSlice
{
      //Implements Python __getitem__ to get an element at a given index
   T __getitem__(size_t i)
   {
         // Throw an exception if the index is out of the bounds
      CHECK_INDEX(i, 0, $self->rows())

         return (*($self))[i];
   }

      // Implements Python __setitem__ to set an elementat a given idex
   void __setitem__(size_t i, T value)
   {
         // Throw an exception if the index is out of the bounds
      CHECK_INDEX(i, 0, $self->rows())

         (*($self))[i] = value;
      return;
   }
}

%extend gnsstk::MatrixRowSlice
{
      //Implements Python __getitem__ to get an element at a given index
   T __getitem__(size_t i)
   {
         // Throw an exception if the index is out of the bounds
      CHECK_INDEX(i, 0, $self->cols())

         return (*($self))[i];
   }

      // Implements Python __setitem__ to set an elementat a given idex
   void __setitem__(size_t i, T value)
   {
         // Throw an exception if the index is out of the bounds
      CHECK_INDEX(i, 0, $self->cols())

         (*($self))[i] = value;
      return;
   }
}

   // Add Python implementations of the __call__ and backSub methods for all Matrix Functor classes
%extend gnsstk::SVD
{
   bool __call__(const gnsstk::ConstMatrixBase<T, gnsstk::Matrix<T>>& mat)
   {
      return (*($self))(mat);
   }

   void backSub(gnsstk::RefVectorBase<T, gnsstk::Vector<T>>& b)
   {
      (*($self)).backSub(b);
   }
}

%extend gnsstk::LUDecomp
{
   void __call__(const gnsstk::ConstMatrixBase<T, gnsstk::Matrix<T>>& mat)
   {
      (*($self))(mat);
   }

   void backSub(gnsstk::RefVectorBase<T, gnsstk::Vector<T>>& b)
   {
      (*($self)).backSub(b);
   }
}

%extend gnsstk::Cholesky
{
   void __call__(const gnsstk::ConstMatrixBase<T, gnsstk::Matrix<T>>& mat)
   {
      (*($self))(mat);
   }

   void backSub(gnsstk::RefVectorBase<T, gnsstk::Vector<T>>& b)
   {
      (*($self)).backSub(b);
   }
}

%extend gnsstk::Householder
{
   void __call__(const gnsstk::ConstMatrixBase<T, gnsstk::Matrix<T>>& mat)
   {
      (*($self))(mat);
   }
}

// Create Matrices for different data types
MATRIX_TEMPLATE(int)
MATRIX_TEMPLATE(double)
NONINT_MATRIX_FUNCTION_TEMPLATES(double)
