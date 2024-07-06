#pragma once

#define _MOKA_SIGNAL_CONNECT_BASE(signal, object, callback, num_args) \
signal.Connect(std::bind(&callback, object

#define _MOKA_SIGNAL_CONNECT_0ARGS(signal, object, callback) \
_MOKA_SIGNAL_CONNECT_BASE(signal, object, callback, num_args)))
#define _MOKA_SIGNAL_CONNECT_1ARGS(signal, object, callback) \
_MOKA_SIGNAL_CONNECT_BASE(signal, object, callback, num_args), std::placeholders::_1))
#define _MOKA_SIGNAL_CONNECT_2ARGS(signal, object, callback) \
_MOKA_SIGNAL_CONNECT_BASE(signal, object, callback, num_args), std::placeholders::_1, std::placeholders::_2))
#define _MOKA_SIGNAL_CONNECT_3ARGS(signal, object, callback) \
_MOKA_SIGNAL_CONNECT_BASE(signal, object, callback, num_args), std::placeholders::_1, std::placeholders::_2, std::placeholders::_3))
#define _MOKA_SIGNAL_CONNECT_4ARGS(signal, object, callback) \
_MOKA_SIGNAL_CONNECT_BASE(signal, object, callback, num_args), std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4))

#define MOKA_SIGNAL_CONNECT(signal, object, callback, num_args) \
_MOKA_SIGNAL_CONNECT_##num_args##ARGS(signal, object, callback)


