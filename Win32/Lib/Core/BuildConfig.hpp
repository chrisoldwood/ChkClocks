////////////////////////////////////////////////////////////////////////////////
//! \file   BuildConfig.hpp
//! \brief  Definitions that configure the build.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef CORE_BUILDCONFIG_HPP
#define CORE_BUILDCONFIG_HPP

#if _MSC_VER > 1000
#pragma once
#endif

////////////////////////////////////////////////////////////////////////////////
// Check build flags.

#ifndef _MT
#error Multi-threaded runtime required
#endif

#if !defined(_WIN32) && !defined(_WIN64)
#error Build platform not specified - _WIN32 or _WIN64
#endif

#if !defined(_MBCS) && !defined(_UNICODE)
#error Character set must be Multi-Byte or Unicode
#endif

#if !defined(_NATIVE_WCHAR_T_DEFINED)
#error wchar_t is not enabled as a built-in type
#endif

#if !defined(_CPPRTTI)
#error RTTI is not enabled
#endif

////////////////////////////////////////////////////////////////////////////////
// Disable VC++ 8.0 warnings about potentially unsafe CRT and STL functions.

#define _CRT_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_WARNINGS

////////////////////////////////////////////////////////////////////////////////
// These additional definitions control other aspects of <windows.h>

#define STRICT						//! Ensures handles are distinct types.
#define VC_EXTRALEAN				//! Trim uncommon portions of <windows.h>

#endif // CORE_BUILDCONFIG_HPP
