%include "TemplateMacros.i"

// Iterator class to wrap for Vector
%inline %{
      // VectorIterator class to wrap to Python iterator
   template<class T>
      class VectorIterator
   {
   public:
      typename gpstk::Vector<T>::iterator cur;
      typename gpstk::Vector<T>::iterator end;
         
      VectorIterator(typename gpstk::Vector<T>::iterator cur,
                     typename gpstk::Vector<T>::iterator end)
         : cur(cur), end(end)
      {}

      ~VectorIterator()
      {}
   };
%}

//Implement Python __next__ function for the vector iterator
%extend VectorIterator
{
   T __next__()
   {
      if ($self->cur != $self->end)
      {
         return *($self->cur++);
      }
      throw gpstk::StopIterator();
   }

   T next()
   {
      if ($self->cur != $self->end)
      {
         return *($self->cur++);
      }
      throw gpstk::StopIterator();
   }
}

namespace gpstk{
    %extend Vector{
        //Implements Python __getitem__ to get an element at an index
        T __getitem__(size_t i) {
            // Throw an exception if the index is out of the bounds of the vector
            CHECK_INDEX(i, 0, $self->size())

            return (*($self))[i];
        }

        //Implements Python __getitem__ to set an element at an index
        void __setitem__(size_t i, T value) {
            // Throw an exception if the index is out of the bounds of the vector
            CHECK_INDEX(i, 0, $self->size())

            (*($self))[i] = value;
        }

        //Implements Python __len__ to define the length of the vector
        size_t __len__() {
            return $self->size();
        }

        //Implements Python __iter__ to define an iterator function
        VectorIterator<T> __iter__()
        {
           return VectorIterator<T>($self->begin(), $self->end());
        }

        //Wraps the overloaded operation to append a scalar
        Vector<T> append(T value) {
            return *($self) && value;
        }

        //Wraps the overloaded operation to extend a vector by another vector
        Vector<T> extend(Vector<T> other_vector) {
            return *($self) && other_vector;
        }

        //Implements elementwise addition for Python with a vector
        Vector<T> __add__(const ConstVectorBase<T, Vector<T>>& r) {
            return *($self) + r;
        }

        // Implements elementwise addition for Python with a value
        Vector<T> __add__(const T r) {
            return *($self) + r;
        }

        //Implements elementwise += for python with a vector
        Vector<T> __iadd__(const ConstVectorBase<T, Vector<T>>& r) {
            return *($self) += r;
        }

        //Implements elementwise += for python with a value
        Vector<T> __iadd__(const T r) {
            return *($self) += r;
        }

        //Implements elementwise subtraction for Python with a vector
        Vector<T> __sub__(const ConstVectorBase<T, Vector<T>>& r) {
            return *($self) - r;
        }

        //Implements elementwise subtraction for Python with a value
        Vector<T> __sub__(const T r) {
            return *($self) - r;
        }

        //Implements elementwise -= for python with a vector
        Vector<T> __isub__(const ConstVectorBase<T, Vector<T>>& r) {
            return *($self) -= r;
        }

        //Implements elementwise -= for python with a value
        Vector<T> __isub__(const T r) {
            return *($self) -= r;
        }

        //Implements elementwise multiplication for Python with a vector
        Vector<T> __mul__(const ConstVectorBase<T, Vector<T>>& r) {
            return *($self) * r;
        }

        //Implements elementwise multiplication for Python with a value
        Vector<T> __mul__(const T r) {
            return *($self) * r;
        }

        //Implements elementwise *= for Python with a vector
        Vector<T> __imul__(const ConstVectorBase<T, Vector<T>>& r) {
            return *($self) *= r;
        }

        //Implements elementwise *= for Python with a value
        Vector<T> __imul__(const T r) {
            return *($self) *= r;
        }

        //Implements elementwise pow for python with vector
        Vector<T> __pow__(const ConstVectorBase<T, Vector<T>>& r) {
            return std::pow(*($self), r);
        }

        // Implements elementwise pow for python with value
        Vector<T> __pow__(const T r) {
            return std::pow(*($self), r);
        }

        //Implements elementwise division for Python with a vector
        //This will automatically perform floor division for integers
        Vector<T> __truediv__(const ConstVectorBase<T, Vector<T>>& r) {
            return *($self) / r;
        }

        //Implements elementwise division for Python with a value
        //This will automatically perform floor division for integers
        Vector<T> __truediv__(const T r) {
            return *($self) / r;
        }

        //Implements elementwise /= for Python with a vector
        Vector<T> __itruediv__(const ConstVectorBase<T, Vector<T>>& r) {
            return *($self) /= r;
        }

        //Implements elementwise /= for Python with a value
        Vector<T> __itruediv__(const T r) {
            return *($self) /= r;
        }

        //Implements elementwise "less than" for Python with a vector
        Vector<bool> __lt__(const ConstVectorBase<T, Vector<T>>& r) {
            return *($self) < r;
        }

        bool lt(const ConstVectorBase<T, Vector<T>>& r) {
            return lt((*($self)), r);
        }

        //Implements elementwise "less than" for Python with a value
        Vector<bool> __lt__(const T r) {
            return *($self) < r;
        }

        bool lt(const T r) {
            return lt((*($self)), r);
        }

        //Implements elementwise "less than or equal to" for Python with a vector
        Vector<bool> __le__(const ConstVectorBase<T, Vector<T>>& r) {
            return *($self) <= r;
        }

        bool le(const ConstVectorBase<T, Vector<T>>& r) {
            return le((*($self)), r);
        }

        //Implements elementwise "less than or equal to" for Python with a value
        Vector<bool> __le__(const T r) {
            return *($self) <= r;
        }

        bool le(const T r) {
            return le((*($self)), r);
        }

        //Implements elementwise "equal to" for Python with a vector
        Vector<bool> __eq__(const ConstVectorBase<T, Vector<T>>& r) {
            return *($self) == r;
        }

        bool eq(const ConstVectorBase<T, Vector<T>>& r) {
            return eq((*($self)), r);
        }

        //Implements elementwise "equal to" for Python with a value
        Vector<bool> __eq__(const T r) {
            return *($self) == r;
        }

        bool eq(const T r) {
            return eq((*($self)), r);
        }

        //Implements elementwise "not equal to" for Python with a vector
        Vector<bool> __ne__(const ConstVectorBase<T, Vector<T>>& r) {
            return *($self) != r;
        }

        bool ne(const ConstVectorBase<T, Vector<T>>& r) {
            return ne((*($self)), r);
        }

        //Implements elementwise "not equal to" for Python with a value
        Vector<bool> __ne__(const T r) {
            return *($self) != r;
        }

        bool ne(const T r) {
            return ne((*($self)), r);
        }

        //Implements elementwise "greater than" for Python with a vector
        Vector<bool> __gt__(const ConstVectorBase<T, Vector<T>>& r) {
            return *($self) > r;
        }

        bool gt(const ConstVectorBase<T, Vector<T>>& r) {
            return gt((*($self)), r);
        }

        //Implements elementwise "greater than" for Python with a value
        Vector<bool> __gt__(const T r) {
            return *($self) > r;
        }

        bool gt(const T r) {
            return gt((*($self)), r);
        }

        //Implements elementwise "greater than or equal to" for Python with a vector
        Vector<bool> __ge__(const ConstVectorBase<T, Vector<T>>& r) {
            return *($self) >= r;
        }

        bool ge(const ConstVectorBase<T, Vector<T>>& r) {
            return ge((*($self)), r);
        }

        //Implements elementwise "greater than or equal to" for Python with a value
        Vector<bool> __ge__(const T r) {
            return *($self) >= r;
        }

        bool ge(const T r) {
            return ge((*($self)), r);
        }
    }

    // Operations specific for vector ints
    %extend Vector<int> {
        //Implements elementwise modulo for Python
        Vector<int> __mod__(const ConstVectorBase<int, Vector<int>>& r) {
            return *($self) % r;
        }

        //Implements elementwise binary AND for Python
        Vector<int> __and__(const ConstVectorBase<int, Vector<int>>& r) {
            return *($self) & r;
        }

        //Implements elementwise binary OR for Python
        Vector<int> __or__(const ConstVectorBase<int, Vector<int>>& r) {
            return *($self) | r;
        }

        //Implements elementwise binary XOR for Python
        Vector<int> __xor__(const ConstVectorBase<int, Vector<int>>& r) {
            return *($self) ^ r;
        }

        // Implements floor division for Python
        Vector<int> __floordiv__(const ConstVectorBase<int, Vector<int>>& r) {
            return *($self) / r;
        }
    }
}

%pythoncode %{
    # Initializes an empty vector from a list
    # The data type of the list elements must match the vector type
    def init_vector_from_list(vector, lst):
        for i in lst:
            vector = vector.append(lst)
        return vector

    # Typechecking function for pow
    def vectorpow(left, right):
        # Check whether the left and right side are scalars
        left_is_int = isinstance(left, int)
        left_is_double = isinstance(left, float)
        right_is_int = isinstance(right, int)
        right_is_double = isinstance(right, float)
        # Int on left, vectorint on right
        if isinstance(left, vector_int) and right_is_int or left_is_int and isinstance(right, vector_int):
           return vectorpow_int_vectorint(left, right)
        # Vector_int on left, vector_int on right
        elif isinstance(left, vector_int) and isinstance(right, vector_int):
            return vectorpow_vectorint_vectorint(left, right)
        # Double on left, vectordouble on right
        elif isinstance(left, vector_double) and right_is_double or left_is_double and isinstance(right, vector_double):
           return vectorpow_double_vectordouble(left, right)
        # Vector_double on left, vector_double on right
        elif isinstance(left, vector_double) and isinstance(right, vector_double):
           return vectorpow_vectordouble_vectordouble(left, right)
        else:
           raise TypeError("Invalid Argument Types for " + str(type(left)) + " and right type " + str(type(right)))

    # Typechecking function for atan2
    def vectoratan2(left, right):
        # Check if the left and right side are scalas
        left_is_double = isinstance(left, float)
        right_is_double = isinstance(right, float)
        # Double on left, vectordouble on right
        if isinstance(left, vector_double) and right_is_double or left_is_double and isinstance(right, vector_double):
            return vectoratan2_double_vectordouble(left, right)
        # Vector_double on left, vector_double on right
        elif isinstance(left, vector_double) and isinstance(right, vector_double):
            return vectoratan2_vectordouble_vectordouble(left, right)
        else:
            raise TypeError("Invalid Argument Types for " + str(type(left)) + " and right type " + str(type(right)))

%}

// Allow vector templates of int and double to be created
VECTOR_TEMPLATE(int)
VECTOR_TEMPLATE(double)
VECTOR_TEMPLATE(bool)
// Transcendental Binary Vector Operation templates
TRANSCBINVECTOROP_TEMPLATE(int, pow)
TRANSCBINVECTOROP_TEMPLATE(double, pow)
TRANSCBINVECTOROP_TEMPLATE(double, atan2)
// Regular Binary Vector Operation templates
BINVECTOROP_TEMPLATE(int, cross)
BINVECTOROP_TEMPLATE(double, cross)
BINVECTOROP_TEMPLATE(int, dot)
BINVECTOROP_TEMPLATE(double, dot)
BINVECTOROP_TEMPLATE(int, Minkowski)
BINVECTOROP_TEMPLATE(double, Minkowski)
BINVECTOROP_TEMPLATE(double, cosVec)
