# Gehu Language API Documentation

## Types

### Basic Types
- `number`: Double-precision floating-point number
- `string`: UTF-8 encoded string

### Unit Types
- Distance: `km`, `m`, `mi`
- Time: `min`, `s`, `h`
- Speed: `m/s`, `km/h`, `km/min`
- Mass: `kg`
- Energy: `J`
- Force: `N`
- Acceleration: `m/s²`

## Functions

### Built-in Functions

#### Display
```gehu
show(value: number | string): void
```
Displays a value to the console.

#### Concurrency
```gehu
spawn(block: block): number
```
Creates a new task and returns its ID.

```gehu
wait(task_id: number): void
```
Waits for a task to complete.

#### Unit Conversion
```gehu
convert_unit(value: number, from_unit: string, to_unit: string): number
```
Converts a value between compatible units.

### User-Defined Functions
```gehu
func name(param1: type1, param2: type2, ...): return_type {
    // function body
    return value;
}
```

## Statements

### Variable Declaration
```gehu
let name: type = value;
```

### Function Declaration
```gehu
func name(param1: type1, param2: type2, ...): return_type {
    // function body
}
```

### Show Statement
```gehu
show expression;
```

### Block Statement
```gehu
{
    // statements
}
```

### Spawn Statement
```gehu
spawn {
    // statements
}
```

### Wait Statement
```gehu
wait expression;
```

## Expressions

### Literals
```gehu
42          // number
"hello"     // string
5 km        // unit
```

### Binary Operations
```gehu
a + b       // addition
a - b       // subtraction
a * b       // multiplication
a / b       // division
a == b      // equality
a != b      // inequality
```

### Unary Operations
```gehu
-a          // negation
!a          // logical not
```

### Function Call
```gehu
function(arg1, arg2, ...)
```

### Pipeline
```gehu
value |> function(arg1, _, arg2)
```

### Pattern Matching
```gehu
match value {
    pattern1 => result1;
    pattern2 => result2;
    _ => default_result;
}
```

## Error Handling

### Runtime Errors
- Division by zero
- Invalid unit conversion
- Undefined variable
- Type mismatch
- Invalid function call

### Error Recovery
The compiler provides detailed error messages with line and column information to help identify and fix issues.

## Best Practices

1. Always specify types for function parameters and return values
2. Use meaningful variable and function names
3. Handle potential errors in pattern matching
4. Use units consistently throughout calculations
5. Document complex functions with comments
6. Use the pipeline operator for better readability
7. Keep tasks small and focused when using concurrency 