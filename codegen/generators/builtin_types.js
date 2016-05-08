const generator = require('../generator');

function genStructureDefine(dtype, ctype) {
    return `typedef struct _dont_${dtype}_struct {
  DontObject_HEADER;
  ${ctype} value;
} ${dtype};`
}

function genDeclare(dtype, ctype) {
    return `void _dont_${dtype}_ctor(${dtype} *object, va_list value);`
}

function genDebugDeclare(dtype, ctype) {
    return `extern DontTypeObject _dont_${dtype}_type_obj;
extern DontTypeObject _dont_${dtype}_type_obj;
extern DontTypeArithmeticMethods _dont_${dtype}_arithmetic_methods;
`;
}

function genTypeObject(dtype, ctype) {
    return `DONT_PRIVATE DontTypeObject _dont_${dtype}_type_obj = { 
  .name = "${ctype}",
  .arithmetic_methods = &_dont_${dtype}_arithmetic_methods,
  .logic_methods = &_dont_${dtype}_logic_methods
};

void _dont_${dtype}_ctor(${dtype} *object, va_list args) {
  object->object_header.type = &_dont_${dtype}_type_obj;
  object->object_header.size = sizeof(${dtype});
  object->value = va_arg(args, ${ctype});
};`;
}

function genLogicMethods(dtype, ctype) {
    return `bool _dont_${dtype}_logic_methods_to_bool(${dtype} *self) {
  return $$(self) != (${ctype}) 0;
}

DONT_PRIVATE DontTypeLogicMethods _dont_${dtype}_logic_methods = {
  .to_bool = (intptr_t) _dont_${dtype}_logic_methods_to_bool
};
`
}

function genArithmeticMethods(dtype, ctype) {
    return `${dtype}* _dont_${dtype}_arithmetic_methods_add(${dtype} *lhs, ${dtype} *rhs) {
  return $new(${dtype}, lhs->value + rhs->value);
}

${dtype}* _dont_${dtype}_arithmetic_methods_sub(${dtype} *lhs, ${dtype} *rhs) {
  return $new(${dtype}, lhs->value - rhs->value);
}
 
DONT_PRIVATE DontTypeArithmeticMethods _dont_${dtype}_arithmetic_methods = {
  .add = (intptr_t) _dont_${dtype}_arithmetic_methods_add,
  .sub = (intptr_t) _dont_${dtype}_arithmetic_methods_sub
};`
}

function genGTest(dtype, ctype) {
    return `TEST(BuiltinTypesLayout, ${dtype}) {
  EXPECT_EQ(0, offsetof(${dtype}, object_header)) << "${dtype} don't have a DontObject_HEADER";
  EXPECT_EQ(sizeof(DontObject), offsetof(${dtype}, value)) << "${dtype}::value don't begins at correct position";
}

TEST(BuiltinTypesConstructor, ${dtype}) {
  ${dtype} *value = $new(${dtype}, ${ctype}(0));
  EXPECT_EQ(&_dont_${dtype}_type_obj, value->object_header.type) << "Constructor did not initialize field 'type'";
  EXPECT_EQ(sizeof(${dtype}), value->object_header.size) << "Constructor did not initialize field 'size'";
}

TEST(BuiltinTypesBound, ${dtype}) {
  using limit = std::numeric_limits<${ctype}>;
  const ${ctype} c_max = limit::max();
  const ${ctype} c_min = limit::lowest();
  const ${ctype} c_zro = ${ctype}(0);
  const ${ctype} c_eps = limit::epsilon();
  ${dtype}* max = $new(${dtype}, c_max);
  ${dtype}* min = $new(${dtype}, c_min);
  ${dtype}* zro = $new(${dtype}, c_zro);
  if (std::is_floating_point<${ctype}>::value) {
    EXPECT_NEAR(c_max, $$(max), c_eps) << "${dtype} cannot fit ${ctype}'s max value";
    EXPECT_NEAR(c_min, $$(min), c_eps) << "${dtype} cannot fit ${ctype}'s min value";
  } else {
    EXPECT_EQ(c_max, $$(max)) << "${dtype} cannot fit ${ctype}'s max value";
    EXPECT_EQ(c_min, $$(min)) << "${dtype} cannot fit ${ctype}'s min value";
  }
  EXPECT_EQ(c_zro, $$(zro)) << "${dtype} cannot fit zero";
}

TEST(BuiltinTypesLogicMethods, ${dtype}) {
  EXPECT_TRUE($bool($new(${dtype}, 1))); 
  EXPECT_FALSE($bool($new(${dtype}, 0))); 
}
`;
}

function generateOnTypes(types, functions) {
    return functions.map(fn => types.map(t => fn(...t)).join('\n\n')).join('\n\n');
}

const int_bits = [8, 16, 32, 64, 'max'];
const int_tpls = [
    b => [`i${b}_t`, `int${b}_t`],
    b => [`ui${b}_t`, `uint${b}_t`]
];
const types = int_tpls.reduce((xs, tpl) => xs.concat(int_bits.map(tpl)), []).concat([
    ['f32_t', 'float'],
    ['f64_t', 'double']
]);

function generate() {
    const hIncludes = ['common.h', 'object.h', 'stddef.h', 'stdint.h', 'stdbool.h'];
    const cIncludes = ['builtin_types.h', 'stddef.h', 'stdint.h', 'stdbool.h'];
    const tIncludes = ['gtest/gtest.h', 'limits', 'builtin_types.h'];
    const h = generator.genHeader('', hIncludes,
        generateOnTypes(types, [genStructureDefine, genDeclare]),
        generateOnTypes(types, [genDebugDeclare]));
    const c = generator.genSource('', cIncludes,
        generateOnTypes(types, [genArithmeticMethods, genLogicMethods, genTypeObject]));
    const t = generator.genSource('', tIncludes,
        generateOnTypes(types, [genGTest]));
    return new Map([
        ['include/builtin_types.h', h],
        ['src/builtin_types.c', c],
        ['test/builtin_types.cpp', t]
    ]);
}

exports.generate = generate;
exports.types = types;
