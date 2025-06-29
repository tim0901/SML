export module sml:Matrix;

#ifdef SML_NO_IMPORT_STD

export import <array>;
export import <algorithm>;
export import <iostream>;
export import <string>;
export import <tuple>;
export import <vector>;

#else
import std;
#endif // SML_NO_IMPORT_STD


#ifndef sml_export
#define sml_export export
#endif

import :Vector;
#define SML_MODULE_MATRIX
#include "Matrix.hpp"