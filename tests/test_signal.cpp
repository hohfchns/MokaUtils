#include "moka/event/signal.hpp"
#include "moka/event/macros.h"
#include "moka/macros/assert.h"
#include <gtest/gtest.h>

using namespace moka::event;

static int gs_some_called = 0;
static int gs_another_called = 0;
static int gs_message_called = 0;
static int gs_multi_arg_message_called = 0;

static void some_callback()
{
  std::cout << "Callback!" << std::endl;
  gs_some_called++;
}

static void another_callback()
{
  std::cout << "Another Callback!" << std::endl;
  gs_another_called++;
}

static void on_message(const std::string& message)
{
  std::cout << "Message Callback | " << gs_message_called << " | " << message << std::endl;
  gs_message_called++;
}

static void on_multi_arg_message(const std::string& message, int number)
{
  std::cout << "Message Callback | " << gs_multi_arg_message_called << " | " << message << " | " << number << std::endl;
  gs_multi_arg_message_called++;
}

class SomeClass {
public:
  int num_called = 0;

  void LocalCallback()
  {
    std::cout << "Local Callback!" << std::endl;
    num_called++;
  }

  void LocalCallbackWithArgs(const std::string& message)
  {
    std::cout << "Local Callback with message: " << message << std::endl;
    num_called++;
  }
};


TEST(MokaUtils, Signal) {
  // Signal with no arguments
  Signal callback_signal;

  // Basic non-bound anonymous function calling
  callback_signal.Connect(&some_callback);
  callback_signal.Emit();
  MOKA_ASSERT(gs_some_called == 1, "Callback not called properly.")

  // Basic non-bound ID'd function calling
  callback_signal.Connect(&another_callback, nullptr, "another_callback_id");
  callback_signal.Emit();
  MOKA_ASSERT(gs_another_called == 1, "Callback not called properly.")

  // Basic non-bound disconnection
  callback_signal.Disconnect(nullptr, "another_callback_id");
  callback_signal.Emit();
  MOKA_ASSERT(gs_another_called == 1, "Callback not disconnected properly.")

  SomeClass class_object;

  // Bound method calling with lambdas
  callback_signal.Connect(
    [&class_object]() { class_object.LocalCallback(); },
    &class_object,
    "method_callback_id"
  );
  callback_signal.Emit();
  MOKA_ASSERT(class_object.num_called == 1, "Local Callback not called properly.");

  // Bound method disconnection
  callback_signal.Disconnect(
    &class_object,
    "method_callback_id"
  );
  callback_signal.Emit();
  MOKA_ASSERT(class_object.num_called == 1, "Local Callback not disconnected properly.");

  // Bound method with std::bind
  callback_signal.Connect(
    std::bind(&SomeClass::LocalCallback, &class_object),
    &class_object,
    "method_callback_id"
  );
  callback_signal.Emit();
  MOKA_ASSERT(class_object.num_called == 2, "Local Callback not called properly.");

  callback_signal.Disconnect(
    &class_object,
    "method_callback_id"
  );

  // Single argument signal
  Signal<const std::string&> message_signal;

  message_signal.Connect(&on_message);
  message_signal.Emit("Test Message");
  MOKA_ASSERT(gs_message_called == 1, "Arg Callback not called properly.");

  // Multiple argument signal
  Signal<const std::string&, int> multi_arg_message_signal;

  multi_arg_message_signal.Connect(&on_multi_arg_message);
  multi_arg_message_signal.Emit("Test Multi Arg Message", 5);
  MOKA_ASSERT(gs_multi_arg_message_called == 1, "Multi Arg Callback not called properly.");

  // Bound method with arg
  message_signal.Connect(
    [&class_object](const std::string& arg) { class_object.LocalCallbackWithArgs(arg); },
    &class_object,
    "method_callback_with_args_id"
  );
  message_signal.Emit("Test Message 2");
  MOKA_ASSERT(class_object.num_called == 3, "Local Callback with args not called properly.");

  message_signal.Disconnect(
    &class_object,
    "method_callback_with_args_id"
  );

  // Permanent Bound method with macro (supports up to 4 arguments)
  MOKA_SIGNAL_CONNECT(message_signal, &class_object, SomeClass::LocalCallbackWithArgs, 1);
  message_signal.Emit("Test Message 3");
  MOKA_ASSERT(class_object.num_called == 4, "Local Callback with args not called properly.");

  // ID'd Bound method with macro (supports up to 4 arguments)
  MOKA_SIGNAL_CONNECT_ID(message_signal, "method_callback_with_args_id", &class_object, SomeClass::LocalCallbackWithArgs, 1);
  message_signal.Emit("Test Message 5");
  MOKA_ASSERT(class_object.num_called == 6, "Local Callback with args not called properly.");

  bool exception_thrown = false;

  try {
    MOKA_SIGNAL_CONNECT_ID(message_signal, "method_callback_with_args_id", &class_object, SomeClass::LocalCallbackWithArgs, 1);
  } catch (std::runtime_error& e)
  {
    exception_thrown = true;
  }

  MOKA_ASSERT(exception_thrown, "Duplicate callback id was allowed");

  // Make sure to disconnect the callback and not leave a dangling pointer.
  message_signal.Disconnect(
    &class_object,
    "method_callback_with_args_id"
  );
}
