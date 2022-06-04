export module sml:Matrix;

export import <array>;
export import <algorithm>;
export import <iostream>;
export import <string>;
export import <tuple>;
export import <vector>;

#ifndef sml_export
#define sml_export export
#endif

import :Vector;
#define SML_MODULE_MATRIX
#include "Matrix.hpp"