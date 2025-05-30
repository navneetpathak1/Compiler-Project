; ModuleID = 'complex'
source_filename = "complex"

@.str = private unnamed_addr constant [21 x i8] c"Sum of %d and %d: %d\00", align 1

declare i32 @printf(ptr, ...)

define i32 @add(i32 %a, i32 %b) {
entry:
  %result = add i32 %a, %b
  ret i32 %result
}

define i32 @main() {
entry:
  %a = alloca i32
  %b = alloca i32
  store i32 5, ptr %a
  store i32 3, ptr %b
  
  %a_val = load i32, ptr %a
  %b_val = load i32, ptr %b
  
  %sum = call i32 @add(i32 %a_val, i32 %b_val)
  
  %call = call i32 (ptr, ...) @printf(ptr @.str, i32 %a_val, i32 %b_val, i32 %sum)
  ret i32 0
} 