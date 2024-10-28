#include <boost/preprocessor/seq/enum.hpp>
#include <boost/preprocessor/seq/transform.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/variadic/to_seq.hpp>

#define MY_SEQUENCE (SENSOR, Coordinate::Cartesian2D), (EXPOSURE, Coordinate::Cartesian2D), (VIP1_VCA, Coordinate::Cartesian2D), (MASK, Coordinate::Cartesian2D), (YUV, Coordinate::Cartesian2D), (DEST_OVERLAY, Coordinate::Cartesian2D), (ENCODED, Coordinate::Cartesian2D), (EXT_DEVICE_SPHERE, Coordinate::Cartesian3D), (EXT_DEVICE_3D, Coordinate::Cartesian3D), (EXT_GEOGRAPHIC, Coordinate::Geographic)

// Helper macro to extract the first element of each tuple
#define FIRST_ELEM(s, data, elem) BOOST_PP_TUPLE_ELEM(2, 0, elem)

// Macro to create an enum from a sequence of pairs
#define EXTRACTED_ENUM(seq) BOOST_PP_SEQ_TRANSFORM(FIRST_ELEM, _, seq)

// Example usage with an inline sequence
#define MY_ENUM(seq) enum MyEnum { BOOST_PP_SEQ_ENUM(EXTRACTED_ENUM(seq)) }

#define WRAPPED_SEQUENCE BOOST_PP_VARIADIC_TO_SEQ(WRAP_ELEM MY_SEQUENCE)

// Define your sequence
// #define MY_SEQUENCE ((SENSOR, Coordinate::Cartesian2D))((EXPOSURE, Coordinate::Cartesian2D))

// Use the MY_ENUM macro with the sequence
// MY_ENUM(MY_SEQUENCE);

// EXTRACTED_ENUM((MY_SEQUENCE))

// WRAPPED_SEQUENCE
BOOST_PP_VARIADIC_TO_SEQ(WRAP_ELEM MY_SEQUENCE)

