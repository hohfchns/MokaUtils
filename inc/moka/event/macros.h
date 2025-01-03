#pragma once
#include <optional>

#define _MOKA_SIGNAL_CONNECT_BASE(signal, object, callback) \
signal.Connect(std::bind(&callback, object

#define _MOKA_SIGNAL_CONNECT_0ARGS(signal, id, object, callback) \
_MOKA_SIGNAL_CONNECT_BASE(signal, object, callback)), object, id)
#define _MOKA_SIGNAL_CONNECT_1ARGS(signal, id, object, callback) \
_MOKA_SIGNAL_CONNECT_BASE(signal, object, callback), std::placeholders::_1), object, id)
#define _MOKA_SIGNAL_CONNECT_2ARGS(signal, id, object, callback) \
_MOKA_SIGNAL_CONNECT_BASE(signal, object, callback), std::placeholders::_1, std::placeholders::_2), object, id)
#define _MOKA_SIGNAL_CONNECT_3ARGS(signal, id, object, callback) \
_MOKA_SIGNAL_CONNECT_BASE(signal, object, callback), std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), object, id)
#define _MOKA_SIGNAL_CONNECT_4ARGS(signal, id, object, callback) \
_MOKA_SIGNAL_CONNECT_BASE(signal, object, callback), std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4), object, id)

#define MOKA_SIGNAL_CONNECT(signal, object, callback, num_args) \
_MOKA_SIGNAL_CONNECT_##num_args##ARGS(signal, std::nullopt, object, callback)

#define MOKA_SIGNAL_CONNECT_ID(signal, id, object, callback, num_args) \
_MOKA_SIGNAL_CONNECT_##num_args##ARGS(signal, id, object, callback)
