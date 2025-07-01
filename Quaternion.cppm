export module sml:Quaternion;

#ifndef sml_export
#define sml_export export
#endif

#ifdef SML_NO_IMPORT_STD

import <array>;
import <algorithm>;
import <iostream>;
import <unordered_map>;
import <vector>;

#else
import std;
#endif // SML_NO_IMPORT_STD

import :Vector;
import :Matrix;
#define SML_MODULE_VECTOR
#include "Quaternion.hpp"