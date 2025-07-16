export module sml:Utility;

#ifndef sml_export
#define sml_export export
#endif

#ifdef SML_NO_IMPORT_STD
import <numbers>
#else
import std;
#endif // SML_NO_IMPORT_STD

#include "Utility.hpp"