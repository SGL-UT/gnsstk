// Geomatics Files in order of dependency
%include "msecHandler.hpp"
%include "Namelist.hpp"

namespace gnsstk{
    %extend Namelist{
        //This checks if two name lists are equal (Overloads the == operator in C++ and Python)
        bool __eq__(const Namelist& n) {
            return *($self) == n;
        }

        //This checks if two name lists are not equal (Overloads the != operator in C++ and Python)
        bool __ne__(const Namelist& n) {
            return *($self) != n;
        }

        //This checks if two name lists are equal (Overloads the & operator in C++ and Python)
        Namelist __and__(const Namelist& n) {
            return *($self) & n;
        }

        //This checks if two name lists are equal (Overloads the | operator in C++ and Python)
        Namelist __or__(const Namelist& n) {
            return *($self) | n;
        }

        //This checks if two name lists are equal (Overloads the ^ operator in C++ and Python)
        Namelist __xor__(const Namelist& n) {
            return *($self) ^ n;
        }

        //This checks if two name lists are equal (Overloads the identical operator in C++ and Python)
        bool identical(const Namelist& n){
            return identical(*($self),n);
        }
    }
};
