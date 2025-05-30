; ModuleID = 'gehu'
source_filename = "gehu"

@0 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@1 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@2 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@3 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

declare i32 @printf(ptr, ...)

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 10, ptr %a, align 4
  %b = alloca i32, align 4
  store i32 6, ptr %b, align 4
  %0 = load i32, ptr %a, align 4
  %1 = load i32, ptr %b, align 4
  %2 = add i32 %0, %1
  %sum = alloca i32, align 4
  store i32 %2, ptr %sum, align 4
  %3 = load i32, ptr %a, align 4
  %4 = load i32, ptr %b, align 4
  %5 = sub i32 %3, %4
  %diff = alloca i32, align 4
  store i32 %5, ptr %diff, align 4
  %6 = load i32, ptr %a, align 4
  %7 = load i32, ptr %b, align 4
  %8 = mul i32 %6, %7
  %product = alloca i32, align 4
  store i32 %8, ptr %product, align 4
  %9 = load i32, ptr %a, align 4
  %10 = load i32, ptr %b, align 4
  %11 = sdiv i32 %9, %10
  %quotient = alloca i32, align 4
  store i32 %11, ptr %quotient, align 4
  %12 = load i32, ptr %sum, align 4
  %13 = call i32 (ptr, ...) @printf(ptr @0, i32 %12)
  %14 = load i32, ptr %diff, align 4
  %15 = call i32 (ptr, ...) @printf(ptr @1, i32 %14)
  %16 = load i32, ptr %product, align 4
  %17 = call i32 (ptr, ...) @printf(ptr @2, i32 %16)
  %18 = load i32, ptr %quotient, align 4
  %19 = call i32 (ptr, ...) @printf(ptr @3, i32 %18)
  ret i32 0
}
