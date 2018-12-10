# Introduction

A library containing C++ primitives that I use.  Some things that
might be of use to others include but are not limited to:

- [ndgpp::bounded_integer](include/libndgpp/bounded_integer.hpp)
- [ndgpp::strto](include/libndgpp/strto.hpp)
- [ndgpp::safe_op](include/libndgpp/safe_operators.hpp)

#### ndgpp::bounded_integer

An integer type that allows for a compile time specifications of the
allowed value range.  This class faciliates proper construction from
an arbitraty integer type and has safe signed / unsigned comparisions.
It also facilitates integer construction from "string" types.

#### ndgpp::strto

A function overload set that facilitates converting strings to numeric
types.  These functions are convenient because they allow for compile
time specification of the allowed value range as well as a requiring a
template parameter to specify the target integer type.  The target
integer type template parameter facilitates string to integer
conversions in template functions because the template function does
not have to use a different function name for each target integer type.

#### ndgpp::safe_op

A namespace that contains several functions that perform safe
comparisons on integer types that differ in signedness.
