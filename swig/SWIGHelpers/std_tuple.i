// Modified from stackoverflow: https://stackoverflow.com/questions/72816953/support-for-stdtuple-in-swig
// The main use case for this wrapper is to allow tuple unpacking of C++ functions
// that return std::tuple.
%{
#include <tuple>
#include <utility>
%}

#define make_getter(pos, type) const type& get##pos() const { return std::get<pos>(*$self); }
#define make_setter(pos, type) void set##pos(const type& val) { std::get<pos>(*$self) = val; }
#define make_ctorargN(pos, type) , type v##pos
#define make_ctorarg(first, ...) const first& v0 FOR_EACH(make_ctorargN, __VA_ARGS__)

#define FE_0(...)
#define FE_1(action,a1) action(0,a1)
#define FE_2(action,a1,a2) action(0,a1) action(1,a2)
#define FE_3(action,a1,a2,a3) action(0,a1) action(1,a2) action(2,a3)
#define FE_4(action,a1,a2,a3,a4) action(0,a1) action(1,a2) action(2,a3) action(3,a4)
#define FE_5(action,a1,a2,a3,a4,a5) action(0,a1) action(1,a2) action(2,a3) action(3,a4) action(4,a5)

#define GET_MACRO(_1,_2,_3,_4,_5,NAME,...) NAME
%define FOR_EACH(action,...)
  GET_MACRO(__VA_ARGS__, FE_5, FE_4, FE_3, FE_2, FE_1, FE_0)(action,__VA_ARGS__)
%enddef

%define %std_tuple(Name, ...)
%rename(Name) std::tuple<__VA_ARGS__>;
namespace std {
    struct tuple<__VA_ARGS__> {
        tuple(make_ctorarg(__VA_ARGS__));
        %extend {
            FOR_EACH(make_getter, __VA_ARGS__)
            FOR_EACH(make_setter, __VA_ARGS__)
            size_t __len__() const { return std::tuple_size<std::decay<decltype(*$self)>::type>{}; }
            %pythoncode %{
                def __getitem__(self, n):
                    if n < 0:
                        n = len(self) + n
                    if n < 0 or n >= len(self):
                        raise IndexError()
                    return getattr(self, 'get%d' % n)()
                def __setitem__(self, n, val):
                    if n < 0:
                        n = len(self) + n
                    if n < 0 or n >= len(self):
                        raise IndexError()
                    getattr(self, 'set%d' % n)(val)
            %}
        }
    };
}
%enddef
