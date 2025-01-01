#pragma once

#if defined(MOKA_LOGGER) && defined(MOKA_ASSERT_LOGGER)

#include "moka/logger/logger.h"
#include <stdexcept>

#ifndef NDEBUG
#define MOKA_ASSERT(cond, error_message) \
if (!(cond)) { MOKA_LOG_ERROR(MOKA_ASSERT_LOGGER, std::string("Assertion `" #cond "` FAILED | ") + error_message); std::throw_with_nested(std::runtime_error(error_message)); }
#else
#define MOKA_ASSERT(cond, error_message)
#endif

#ifndef NDEBUG
#define MOKA_ASSERTF(cond, error_format, ...) \
if (!(cond)) { MOKA_LOGF_ERROR(MOKA_ASSERT_LOGGER, std::string("Assertion `" #cond "` FAILED | ") + error_format, __VA_ARGS__); std::throw_with_nested(std::runtime_error(error_format)); }
#else
#define MOKA_ASSERTF(cond, error_format, ...)
#endif

#else

#include <stdexcept>
#include <iostream>

#ifndef NDEBUG
#define MOKA_ASSERT(cond, error_message) \
if (!(cond)) { std::cerr << "Assertion `" #cond "` FAILED | " << error_message << std::endl; std::throw_with_nested(std::runtime_error(error_message)); }
#else
#define MOKA_ASSERT(cond, error_message)
#endif

#endif

