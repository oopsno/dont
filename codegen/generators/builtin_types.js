const generator = require('../generator');

function genStructureDefine(dtype, ctype) {
    return `typedef struct DOBJ_STRUCTURE(${dtype}) {
  DontObject_HEADER;
  ${ctype} value;
} ${dtype};`
}

function genDeclare(dtype, ctype) {
    return `${dtype} *DOBJ_CTOR(${dtype})(${ctype} value);`
}

function genDebugDeclare(dtype, ctype) {
    return `extern DontTypeObject DOBJ_TYPEOBJ(${dtype});
extern DontTypeBasicMethods DOBJ_METHODS_STT(${dtype});
`;
}

function genTypeObject(dtype, ctype) {
    return `DONT_PRIVATE DontTypeObject DOBJ_TYPEOBJ(${dtype}) = {
  .name = "${dtype}",
  .methods = &DOBJ_METHODS_STT(${dtype})
};`;
}

function genMethods(dtype, ctype) {
    const methodFields = [['add', '+'], ['sub', '-'], ['mul', '*'], ['div', '/']];
    
    function genMethod(n, op) {
        return `${dtype} *DOBJ_METHOD(${dtype}, ${n})(${dtype} *lhs, ${dtype} *rhs) {
  return $new(${dtype}, $$(lhs) ${op} $$(rhs));
}`
    }
    
    function genField(n) {
        return `.${n} = (void *) DOBJ_METHOD(${dtype}, ${n})`
    }

     return `${dtype} *DOBJ_CTOR(${dtype})(${ctype} value) {
  ${dtype} *self = (${dtype} *) malloc(sizeof(${dtype})); 
  $type(self) = &_dont_${dtype}_type_obj;
  $size(self) = sizeof(${dtype});
  $$(self) = value;
  return self;
};

${methodFields.map(xs => genMethod(...xs)).join('\n')}
 
bool DOBJ_METHOD(${dtype}, to_bool)(${dtype} *self) {
  return $$(self) != (${ctype}) 0;
}

DONT_PRIVATE DontTypeBasicMethods DOBJ_METHODS_STT(${dtype}) = {
  ${methodFields.map(([n]) => genField(n)).join(',\n  ')},
  .to_bool = DOBJ_METHOD(${dtype}, to_bool)
};`;
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

TEST(BuiltinTypesArithmeticMethods, ${dtype}) {
  ${dtype} *lhs = $new(${dtype}, 40);
  ${dtype} *rhs = $new(${dtype},  2);
  ${dtype} *sum = (${dtype} *) $call(lhs, add, rhs);
  ${dtype} *dif = (${dtype} *) $call(lhs, sub, rhs);
  ${dtype} *prd = (${dtype} *) $call(lhs, mul, rhs);
  ${dtype} *rem = (${dtype} *) $call(lhs, div, rhs);
  EXPECT_EQ($$(sum), 42); 
  EXPECT_EQ($$(dif), 38); 
  EXPECT_EQ($$(prd), 80); 
  EXPECT_EQ($$(rem), 20); 
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
    const cIncludes = ['builtin_types.h', 'stddef.h', 'stdint.h', 'stdbool.h', 'stdlib.h'];
    const tIncludes = ['gtest/gtest.h', 'limits', 'builtin_types.h'];
    const h = generator.genHeader('', hIncludes,
        generateOnTypes(types, [genStructureDefine, genDeclare]),
        generateOnTypes(types, [genDebugDeclare]));
    const c = generator.genSource('', cIncludes,
        generateOnTypes(types, [genMethods, genTypeObject]));
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
