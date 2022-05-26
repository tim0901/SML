module;
export import <array>;
export import <algorithm>;
export import <iostream>;
export import <vector>;

#ifndef sml_export
#define sml_export export
#endif

export module sml:Vector;
#define SML_MODULE_VECTOR
#include "Vector.hpp"