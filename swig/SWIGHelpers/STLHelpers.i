// SHared information:
%pythoncode %{
_namefixes = {'str': 'string', 'float' : 'double'}
%}



// std::map <-> dict conversions
%pythoncode %{
def dictToMap(dict, outtype=''):
    """Translates a python dict type to a std::map backed type.
    This uses the first element to get the type. If there are 0 elements,
    None is returned.

    This raises a TypeError if it uses a type that is not templated or if the
    dict does not use the same key,value type for all its elements.

    Note that this recopies the contents of the dict and is a linear time operation.

    You may specify the what the name (string name) of the C++ backed type is. If it is blank (the default),
    the function will attempt to discover what the appropriate type is. For example, you
    might use outType=\'map_string_int\'.
    """
    import sys
    if sys.version_info[0] < 3:
        import __builtin__ as builtins
    else:
        import builtins
    if len(dict) == 0:
        return None
    first_key = builtins.list(dict.keys())[0]
    t_key = first_key.__class__.__name__
    t_value = dict[first_key].__class__.__name__

    # some python names dont map to the vector wrap names perfectly, so they get fixed:
    if t_key in _namefixes:
        t_key = _namefixes[t_key]
    if t_value in _namefixes:
        t_value = _namefixes[t_value]

    if outtype == '':
        new_name = 'std_map_' + t_key + '_' + t_value
    else:
        new_name = outtype

    try:
        m = globals()[new_name]()  # constructs an object of class t
    except:
        if outtype == '':
            raise TypeError('There is no map wrapper for ' + t_key + ' => ' + t_value)
        else:
            raise TypeError('The type ' + outtype + ' does not exist')

    t_key = type(first_key)
    t_value = type(dict[first_key])
    for key, value in dict.items():
        if type(key) != t_key or type(value) != t_value:
            raise TypeError('Type mismatch in dict: ({0}, {1}) vs. ({2}, {3})'
                            .format(t_key, t_value, type(key), type(value)))
        m[key] = value
    return m

def mapToDict(map):
    """Translates a std::map backed type (from gnsstk.cpp) to a python dict.
    Note that this recopies the contents of the map and is a linear time operation.
    """
    dict = {}
    for key, value in map.items():
        dict[key] = value
    return dict
%}


// std::vector <-> list conversions

%pythoncode %{
def seqToVector(seq, outtype=''):
    """Translates a python iterable type to a std::vector backed type.
    This uses the first element to get the type. If there are 0 elements,
    None is returned, unless the outtype is specified.

    This raises a TypeError if it uses a type that is not templated or if the
    sequence does not use the same type for all its elements.

    Note that this recopies the contents of the sequence and is a linear time operation.

    You may specify the what the name (string name) of the C++ backed type is. If it is blank (the default),
    the function will attempt to discover what the appropriate type is. For example, you
    might use outtype=\'std_vector_double\'.
    """
    import sys
    if sys.version_info[0] < 3:
        import __builtin__ as builtins
    else:
        import builtins

    if outtype == '':
        if len(seq) == 0:
            return None
        t = seq[0].__class__.__name__
        if t in _namefixes:
            t = _namefixes[t]
        new_name = 'std_vector_' + t
    else:
        new_name = outtype
    try:
        v = globals()[new_name](len(seq))  # constructs an object of class t
    except:
        if outtype == '':
            raise TypeError('There is no vector wrapper for ' + t)
        else:
            raise TypeError('The type ' + outtype + ' does not exist')

    first_type = type(seq[0])
    for i in builtins.range(len(seq)):
        if type(seq[i]) != first_type:
            raise TypeError('Type mismatch in sequence: {0} vs. {1}'
                            .format(first_type, type(seq[i])))
        v[i] = seq[i]
    return v

def vectorToSeq(vector):
    """Translates a std::vector backed type (from gnsstk.cpp) to a python list.

    Note that this recopies the contents of the vector and is a linear time operation.
    """
    list = [None] * vector.size()  # pre-allocates size to help efficiency
    import sys
    if sys.version_info[0] < 3:
        import __builtin__ as builtins
    else:
        import builtins
    for i in builtins.range(vector.size()):
        list[i] = vector[i]
    return list
%}
