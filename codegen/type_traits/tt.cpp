#include <string>
#include <cstdlib>
#include <map>
#include <node/node.h>
#include "tables.hpp"

namespace tt {

using V8Arg = v8::FunctionCallbackInfo<v8::Value>;

template<int ArgLimit>
bool is_arg_length_legal(const V8Arg &args, v8::Isolate *isolate) {
  const int length = args.Length();
  const bool is_legal = length == ArgLimit;
  if (!is_legal) {
    char *message;
    asprintf(&message, "Wrong number of arguments: needs %d, got %d", ArgLimit, length);
    isolate->ThrowException(v8::Exception::TypeError(v8::String::NewFromUtf8(isolate, message)));
  }
  return is_legal;
}

static std::string StringToU8(v8::String *str) {
  char *buffer = new char[str->Utf8Length() + 1];
  str->WriteUtf8(buffer);
  std::string s(buffer);
  delete[] (buffer);
  return s;
}

template<typename Map>
static bool has_key(Map &m, std::string key, v8::Isolate *isolate) {
  const auto result = is_floating_point_table.find(key);
  if (result == is_floating_point_table.cend()) {
    char *message;
    asprintf(&message, "Unknown type \"%s\"", key.c_str());
    isolate->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(isolate, message)));
    return false;
  } else {
    return true;
  }
}

template<typename ReturnType, typename Table>
void LookupTable(const V8Arg &args, Table &table) {
  v8::Isolate *isolate = args.GetIsolate();
  if (!is_arg_length_legal<1>(args, isolate)) {
    return;
  }
  std::string name = StringToU8(*(args[0]->ToString()));
  if (!has_key(is_floating_point_table, name, isolate)) {
    return;
  }
  args.GetReturnValue().Set(ReturnType::New(isolate, table.at(name)));
}

void IsFloatingPoint(const V8Arg &args) {
  LookupTable<v8::Boolean>(args, is_floating_point_table);
}

void SizeOfType(const V8Arg &args) {
  LookupTable<v8::Number>(args, type_size_table);
}

void init(v8::Local<v8::Object> exports) {
  NODE_SET_METHOD(exports, "is_floating_point", IsFloatingPoint);
  NODE_SET_METHOD(exports, "sizeof", SizeOfType);
}

NODE_MODULE(addon, init)

}

