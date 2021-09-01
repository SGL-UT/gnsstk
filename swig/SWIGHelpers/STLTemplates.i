// This is to silence warning about not knowing about the fstream base class
namespace std
{
   class fstream {};
   template <typename ArgumentType, typename ResultType> struct unary_function {};
   template <class Arg1, class Arg2, class Result> struct binary_function {};
}


//////////////////////////////////////////////////
//            std::vector wraps
//////////////////////////////////////////////////
%template(std_vector_int)        std::vector<int>;
%template(std_vector_double)     std::vector<double>;
%template(std_vector_string)     std::vector<std::string>;

//////////////////////////////////////////////////
//             std::map wraps
//////////////////////////////////////////////////
/* %template()     std::pair<swig::SwigPtr_PyObject, swig::SwigPtr_PyObject>; */
/* %template(cmap) std::map<swig::SwigPtr_PyObject, swig::SwigPtr_PyObject>; */

%template(std_map_string_double) std::map<std::string, double>;
%template(std_map_string_int)    std::map<std::string, int>;
/* %template(std_map_string_char)   std::map<std::string, char>; */

/* %template(std_map_double_string) std::map<double, std::string>; */
/* %template(std_map_double_int)    std::map<double, int>; */

/* %template(std_map_int_double) std::map<int, double>; */
%template(std_map_int_char)      std::map<int, char>;
%template(std_map_int_string)    std::map<int, std::string>;

/* %template(std_map_char_string) std::map<char, std::string>; */
/* %template(std_map_char_int)    std::map<char, int>; */

//////////////////////////////////////////////////
//             std::set wraps
//////////////////////////////////////////////////
/* %template(std_set_int) std::set<int>; */
/* %template(std_set_double) std::set<double>; */
/* %template(std_set_string) std::set<std::string>; */

//////////////////////////////////////////////////
//             std::list wraps
//////////////////////////////////////////////////
/* %template(std_list_int) std::list<int>; */
/* %template(std_list_double) std::list<double>; */

//////////////////////////////////////////////////
//             std::pair wraps
//////////////////////////////////////////////////
/* %template(std_pair) std::pair<double, double>; */
