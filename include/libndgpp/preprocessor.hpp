#ifndef LIBNDGPP_PREPROCESSOR_HPP
#define LIBNDGPP_PREPROCESSOR_HPP

#define NDGPP_STRINGIZE_P(x) #x

/// Wraps the parameter in quotes
#define NDGPP_STRINGIZE(x) NDGPP_STRINGIZE_P(x)

#endif
