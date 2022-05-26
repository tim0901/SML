module;
export import <array>;
export import <algorithm>;
export import <iostream>;
export import <string>;
export import <tuple>;
export import <vector>;

#ifndef sml_export
#define sml_export export
#endif

export module sml:Matrix;
import :Vector;
#define SML_MODULE_MATRIX
#include "Matrix.hpp"