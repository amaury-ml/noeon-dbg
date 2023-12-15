
sint_neon_types = ["s8", "s16", "s32", "s64"]
uint_neon_types = ["u8", "u16", "u32", "u64"]
float_neon_types = ["f16", "f32", "f64"]

neon_types = sint_neon_types + uint_neon_types + float_neon_types

sint_type2ctype =  {"s8" : "int8", "s16": "int16", "s32": "int32", "s64": "int64" }
sint_type2size  =  {"s8" : 1, "s16": 2, "s32": 4, "s64": 8 }

uint_type2ctype =  {"u8" : "uint8", "u16": "uint16", "u32": "uint32", "u64": "uint64" }
uint_type2size  =  {"u8" : 1, "u16": 2, "u32": 4, "u64": 8 }

float_type2ctype = {"f16": "float16", "f32" : "float32", "f64": "float64"}
float_type2size  = {"f16" : 2, "f32" : 4, "f64": 8}

neon_type2ctype = sint_type2ctype
neon_type2ctype.update(uint_type2ctype)
neon_type2ctype.update(float_type2ctype)

neon_type2size = sint_type2size
neon_type2size.update(uint_type2size)
neon_type2size.update(float_type2size)



sint_ret_types = {x:x for x in sint_neon_types}
uint_ret_types = {x:x for x in uint_neon_types}
float_ret_types = {x:x for x in float_neon_types}
ret_types = sint_ret_types
ret_types .update(uint_ret_types)
ret_types .update(float_ret_types)

sint_long_types = {"s8" : "s16", "s16": "s32", "s32": "s64" }
uint_long_types = {"u8" : "u16", "u16": "u32", "u32": "u64" }
long_types  = sint_long_types
long_types .update(uint_long_types )


sint_narrow_types = {"s16": "s8", "s32": "s16", "s64": "s32" }
uint_narrow_types = {"u16": "u8", "u32": "u16", "u64": "u32" }
narrow_types  = sint_narrow_types
narrow_types .update(uint_narrow_types )

def default_api(abbrev, inputtypes, nargs):
    for size in [8, 16]:
        for inputtype in inputtypes:
            
            fname = f"{abbrev}q_{inputtype}" if size == 16 else f"{abbrev}_{inputtype}"

            bitsize = neon_type2size[inputtype]
            basetype = f"{neon_type2ctype[inputtype]}x{int(size/bitsize) }_t"

            rettype = basetype


            args = [(basetype + " const&", "a")]*n

            if nargs > 1: 
                args = [(args[i][0], args[i][1] + f"{i}") for i in range(nargs)]
            
                
            fn_arglist = ", ".join([f"{at} {an}" for at, an in args])
            #print (fn_arglist)
            arglist = ", ".join([f"{an}" for at, an in args])
            #print (arglist)
            #print (inputtype, rett)
            print(f"inline {rettype} {fname}({fn_arglist}) {{ return neon::{f}({arglist}); }}")

def vaddl_high_api(abbrev, inputtypes, nargs):
    
    for inputtype in inputtypes:
        fname = f"{abbrev}_{inputtype}"
        rtype = long_types[inputtype]
        rettype = f"{neon_type2ctype[rtype]}x{int(16/neon_type2size[rtype]) }_t"

        basetype = f"{neon_type2ctype[inputtype]}x{int(16/neon_type2size[inputtype]) }_t"

        
        args = [(basetype + " const&", f"a{i}") for i in range(nargs) ]

        fn_arglist = ", ".join([f"{at} {an}" for at, an in args])

        arglist = ", ".join([f"{an}" for at, an in args])
        print(f"inline {rettype} {fname}({fn_arglist}) {{ return neon::{f}({arglist}); }}")

def vaddl_api(abbrev, inputtypes, nargs):
    
    for inputtype in inputtypes:
        fname = f"{abbrev}_{inputtype}"

        rtype = long_types[inputtype]
        rettype = f"{neon_type2ctype[rtype]}x{int(16/neon_type2size[rtype]) }_t"

        basetype = f"{neon_type2ctype[inputtype]}x{int(8/neon_type2size[inputtype]) }_t"

        
        args = [(basetype + " const&", f"a{i}") for i in range(nargs) ]

        fn_arglist = ", ".join([f"{at} {an}" for at, an in args])

        arglist = ", ".join([f"{an}" for at, an in args])
        print(f"inline {rettype} {fname}({fn_arglist}) {{ return neon::{f}({arglist}); }}")

def widening64_op(abbrev, inputtypes, nargs):
    for inputtype in inputtypes:
        fname = f"{abbrev}_{inputtype}"
        
        rtype = long_types[inputtype]

        qtype = f"{neon_type2ctype[rtype]}x{int(16/neon_type2size[rtype]) }_t"
        dtype = f"{neon_type2ctype[inputtype]}x{int(8/neon_type2size[inputtype]) }_t"

        
        args = [(qtype + " const&", "a0"), (dtype + " const&", "a1")]

        fn_arglist = ", ".join([f"{at} {an}" for at, an in args])

        arglist = ", ".join([f"{an}" for at, an in args])
        print(f"inline {qtype} {fname}({fn_arglist}) {{ return neon::{f}({arglist}); }}")

def widening128_op(abbrev, inputtypes, nargs):
    for inputtype in inputtypes:
        fname = f"{abbrev}_{inputtype}"
        
        rtype = long_types[inputtype]

        qtype = f"{neon_type2ctype[rtype]}x{int(16/neon_type2size[rtype]) }_t"
        dtype = f"{neon_type2ctype[inputtype]}x{int(16/neon_type2size[inputtype]) }_t"

        
        args = [(qtype + " const&", "a0"), (dtype + " const&", "a1")]

        fn_arglist = ", ".join([f"{at} {an}" for at, an in args])

        arglist = ", ".join([f"{an}" for at, an in args])
        print(f"inline {qtype} {fname}({fn_arglist}) {{ return neon::{f}({arglist}); }}")

def narrow64_op(abbrev, inputtypes, nargs):
    for inputtype in inputtypes:
        fname = f"{abbrev}_{inputtype}"
        
        rtype = narrow_types[inputtype]

        qtype = f"{neon_type2ctype[inputtype]}x{int(16/neon_type2size[inputtype]) }_t"
        dtype = f"{neon_type2ctype[rtype]}x{int(8/neon_type2size[rtype]) }_t"

        
        args = [(qtype + " const&", "a0"), (qtype + " const&", "a1")]

        fn_arglist = ", ".join([f"{at} {an}" for at, an in args])

        arglist = ", ".join([f"{an}" for at, an in args])
        print(f"inline {dtype} {fname}({fn_arglist}) {{ return neon::{f}({arglist}); }}")

def narrow128_op(abbrev, inputtypes, nargs):
    for inputtype in inputtypes:
        fname = f"{abbrev}_{inputtype}"
        
        qtype = f"{neon_type2ctype[inputtype]}x{int(16/neon_type2size[inputtype]) }_t"

        rtype = narrow_types[inputtype]

        rdtype = f"{neon_type2ctype[rtype]}x{int(8/neon_type2size[rtype]) }_t"
        rqtype = f"{neon_type2ctype[rtype]}x{int(16/neon_type2size[rtype]) }_t"

        
        args = [(rdtype + " const&", "a0"), (qtype + " const&", "a1"), (qtype + " const&", "a2")]

        fn_arglist = ", ".join([f"{at} {an}" for at, an in args])

        arglist = ", ".join([f"{an}" for at, an in args])
        print(f"inline {rqtype} {fname}({fn_arglist}) {{ return neon::{f}({arglist}); }}")

def horizontal_op(abbrev, inputtypes, nargs):
    for size in [8, 16]:
        for inputtype in inputtypes:
            if inputtype == 'f16': 
                # Not sure why f16 (vaddv case?)
                continue
            if size == 8 and inputtype in ['f16', 'f64']:
                # We are in a float64x1 as input case, so it's a noop!
                continue
            fname = f"{abbrev}q_{inputtype}" if size == 16 else f"{abbrev}_{inputtype}"

            rtype = f"{neon_type2ctype[inputtype]}_t"
            itype = f"{neon_type2ctype[inputtype]}x{int(size/neon_type2size[inputtype]) }_t"

        
            args = [(itype + " const&", "a")]

            fn_arglist = ", ".join([f"{at} {an}" for at, an in args])

            arglist = ", ".join([f"{an}" for at, an in args])
            print(f"inline {rtype} {fname}({fn_arglist}) {{ return neon::{f}({arglist}); }}")

def bsl_api(abbrev, inputtypes, nargs):
    rep_type = {"f16":"u16", "f32": "u32", "f64": "u64"}
    for size in [8, 16]:
        for inputtype in inputtypes:
            if inputtype in float_neon_types:
                a_arg = f"{neon_type2ctype[rep_type[inputtype]]}x{int(size/neon_type2size[inputtype]) }_t"
            elif inputtype in sint_neon_types:
                a_arg = f"u{neon_type2ctype[inputtype]}x{int(size/neon_type2size[inputtype]) }_t"
            else:
                a_arg = f"{neon_type2ctype[inputtype]}x{int(size/neon_type2size[inputtype]) }_t"
            bc_args = f"{neon_type2ctype[inputtype]}x{int(size/neon_type2size[inputtype]) }_t"

            fname = f"{abbrev}q_{inputtype}" if size == 16 else f"{abbrev}_{inputtype}"

            args = [(a_arg + " const&", "a0"), (bc_args + " const&", "a1"), (bc_args + " const&", "a2")]

            fn_arglist = ", ".join([f"{at} {an}" for at, an in args])
            templatetype = f"{neon_type2ctype[inputtype]}_t"
            arglist = ", ".join([f"{an}" for at, an in args])
            print(f"inline {bc_args} {fname}({fn_arglist}) {{ return neon::{f}<{templatetype}>({arglist}); }}")

def compare_api(abbrev, inputtypes, nargs):
    rep_type = {"f16":"u16", "f32": "u32", "f64": "u64"}

    for size in [8, 16]:
        for inputtype in inputtypes:
            if inputtype in float_neon_types:
                r_type = f"{neon_type2ctype[rep_type[inputtype]]}x{int(size/neon_type2size[inputtype]) }_t"
            elif inputtype in sint_neon_types:
                r_type = f"u{neon_type2ctype[inputtype]}x{int(size/neon_type2size[inputtype]) }_t"
            else:
                r_type = f"{neon_type2ctype[inputtype]}x{int(size/neon_type2size[inputtype]) }_t"

            arg_type = f"{neon_type2ctype[inputtype]}x{int(size/neon_type2size[inputtype]) }_t"
            fname = f"{abbrev}q_{inputtype}" if size == 16 else f"{abbrev}_{inputtype}"
            if nargs > 1:
                args = [(arg_type + " const&", f"a{i}") for i in range(nargs) ]
            else:
                args = [(arg_type + " const&", "a")]
            fn_arglist = ", ".join([f"{at} {an}" for at, an in args])
            templatetype = f"{neon_type2ctype[inputtype]}_t"
            arglist = ", ".join([f"{an}" for at, an in args])
            print(f"inline {r_type} {fname}({fn_arglist}) {{ return neon::{f}({arglist}); }}")

def cvt_floats(abbrev, inputtypes, nargs):
    rep_type = {"f16":["s16", "u16"], "f32": ["s32", "u32"], "f64": ["s64", "u64"]}
    for size in [8, 16]:
        for inputtype in inputtypes:
            for rtype in rep_type[inputtype]:
                r_type = f"{neon_type2ctype[rtype]}x{int(size/neon_type2size[rtype]) }_t"
                arg_type = f"{neon_type2ctype[inputtype]}x{int(size/neon_type2size[inputtype]) }_t"

                fname = f"{abbrev}_{rtype}_{inputtype}" if size == 8 else f"{abbrev}q_{rtype}_{inputtype}"
            
                args = [(arg_type + " const&", "a")]
                fn_arglist = ", ".join([f"{at} {an}" for at, an in args])
                templatetype = f"{neon_type2ctype[rtype]}_t"
                arglist = ", ".join([f"{an}" for at, an in args])
                print(f"inline {r_type} {fname}({fn_arglist}) {{ return neon::{f}<{templatetype}>({arglist}); }}")


api = { "vabd": (default_api, neon_types, 2),  
        "vabs": (default_api, sint_neon_types + float_neon_types, 1),
        "vadd": (default_api, neon_types, 2),  
        "vaddl": (vaddl_api, ["s8", "s16", "s32", "u8", "u16", "u32"], 2),  
        "vaddl_high": (vaddl_high_api, ["s8", "s16", "s32", "u8", "u16", "u32"], 2),  
        "vaddw": (widening64_op,  ["s8", "s16", "s32", "u8", "u16", "u32"], 2),  
        "vaddw_high": (widening128_op,  ["s8", "s16", "s32", "u8", "u16", "u32"], 2),  
        "vaddhn": (narrow64_op,  narrow_types.keys(), 2),  
        "vaddhn_high": (narrow128_op,  narrow_types.keys(), 2),  
        "vaddv": (horizontal_op, neon_types, 1),

        "vbic": (default_api, sint_neon_types + uint_neon_types , 2),
        "vbsl": (bsl_api, neon_types , 3),
        #sha3 "vbcax": (default_api, sint_neon_types + uint_neon_types , 3),
        #armv8.6-a+bf16 "vbfdot"
        #armv8.6-a+bf16 "vbfdot_lane"
        #armv8.6-a+bf16 "vbfmmlatq"
        #armv8.6-a+bf16 "vbfmlalbq"
        #armv8.6-a+bf16 "vbfmlatw"
        #armv8.6-a+bf16 "vbfmlalt_lane"

        "vceq":  (compare_api, neon_types, 2),
        "vceqz":  (compare_api, neon_types, 1),
        "vcge":  (compare_api, neon_types, 2),
        "vcgez":  (compare_api, neon_types, 1),
        "vcle":  (compare_api, neon_types, 2),
        "vclez":  (compare_api, neon_types, 1),

        "vcgt":  (compare_api, neon_types, 2),
        "vcgtz":  (compare_api, neon_types, 1),
        "vclt":  (compare_api, neon_types, 2),
        "vcltz":  (compare_api, neon_types, 1),

        "vcage":  (compare_api, float_neon_types, 2),
        "vcale":  (compare_api, float_neon_types, 2),
        "vcagt":  (compare_api, float_neon_types, 2),
        "vcalt":  (compare_api, float_neon_types, 2),

        "vcvt": (cvt_floats, float_neon_types, 1),

}


print("#include \"neon.h\"")
for f in api:
    fn, t, n = api[f]

    #for size in simd:
    #    for ftype in t:
    #        fname =  f"{f}_{ftype}"
    #        if size == 16 and len(simd) > 1: fname = f"{f}q_{ftype}"
    #
    #        bitsize = neon_type2size[ftype]
    #        basetype = f"{neon_type2ctype[ftype]}x{int(size/bitsize) }_t"
    #
    #        rtype = rett[ftype]
    #        rettype = f"{neon_type2ctype[rtype]}x{int(size/neon_type2size[rtype]) }_t"
    #
    #
    #        args = [(basetype + " const&", "a")]*n
    #
    #        if n > 1: 
    #            args = [(args[i][0], args[i][1] + f"{i}") for i in range(n)]
    #        
    #            
    #        fn_arglist = ", ".join([f"{at} {an}" for at, an in args])
    #        #print (fn_arglist)
    #        arglist = ", ".join([f"{an}" for at, an in args])
    #        #print (arglist)
    #        #print (ftype, rett)
    #        print(f"inline {rettype} {fname}({fn_arglist}) {{ return neon::{f}({arglist}); }}")
    fn(f, t, n)


         