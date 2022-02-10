%include "Triple.hpp"

// C++ extensions:
%extend gnsstk::Triple {

   // Implements indexing for Triples
   double __getitem__(unsigned int i) {
      // element [] access
      return $self->theArray[i];
   }

   // Implements setting for Triple
   void __setitem__(unsigned int i, double value) {
       (*($self))[i] = value;
   }

   gnsstk::Triple scale(double scalar) {
      // since operator* can't be wrapped
      return gnsstk::Triple(scalar * $self->theArray[0],
                           scalar * $self->theArray[1],
                           scalar * $self->theArray[2]);
   }

   // Implements __len__ function for Triple
   int __len__() {
      return 3;
   }

   // Implements __mul__ function for Triple with a scalar
   Triple __mul__(double right) {
       return (*($self)) * right;
   }
}



// Python extensions:
%pythoncode %{
# tuple -> triple translation:
def makeTriple(tuple):
    """Generates a GNSSTk-style Triple from a Python tuple."""
    # TODO: add error checking
    return Triple(tuple[0], tuple[1], tuple[2])

# triple -> tuple translation:
def makeTuple(self):
    """Given a GNSSTk Triple, returns a tuple representing it."""
    return (self[0], self[1], self[2])
Triple.makeTuple = makeTuple

Triple.__str__ = lambda self: makeTuple(self).__str__()
Triple.__repr__ = lambda self: makeTuple(self).__str__()
%}
