# TinyLang

All of this is subject to change as the language evolves. Some important features are missing.

Primitive Types:
```
u64 i64 u32 i32 f32 f64

// Literal suffixes:
// Any of the primitive types can be appended to the end of a literal to change its type
f64 variable_1 = 0.2312f64;
f32 variable_2 = 0.231f32;
i64 variable_3 = 1231i64;

// Underscores '_' are ignored in numerical literals
u32 variable_4 = 131_332_u32;

```

Functions (return type is optional):
```
fn test(<params>) -> <return_type> {

    return <value>;
}
```

Declaring Variables:
```
<type> variable_name;
<type> variable_name = <initial_value>;
```

Comments:
```
// Comment
```
