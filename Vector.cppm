export module sml:Vector;

#ifdef SML_NO_IMPORT_STD

import <array>;
import <algorithm>;
import <iostream>;
import <unordered_map>;
import <vector>;

#else
import std;
#endif // SML_NO_IMPORT_STD

import :Utility;

#ifndef sml_export
#define sml_export export
#endif

#define SML_MODULE_VECTOR
#include "Vector.hpp"