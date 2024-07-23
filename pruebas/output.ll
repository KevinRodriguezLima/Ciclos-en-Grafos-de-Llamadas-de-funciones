; ModuleID = 'ejemploCom.ll'
source_filename = "ejemploCom.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@.str = private unnamed_addr constant [21 x i8] c"Factorial of %d: %d\0A\00", align 1
@.str.1 = private unnamed_addr constant [29 x i8] c"Sum of first %d numbers: %d\0A\00", align 1
@.str.2 = private unnamed_addr constant [26 x i8] c"Product of %d and 10: %d\0A\00", align 1
@.str.3 = private unnamed_addr constant [23 x i8] c"GCD of %d and 100: %d\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @factorial(i32 noundef %0) #0 {
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  store i32 %0, ptr %3, align 4
  %4 = load i32, ptr %3, align 4
  %5 = icmp sle i32 %4, 1
  br i1 %5, label %6, label %7

6:                                                ; preds = %1
  store i32 1, ptr %2, align 4
  br label %12

7:                                                ; preds = %1
  %8 = load i32, ptr %3, align 4
  %9 = load i32, ptr %3, align 4
  %10 = sub nsw i32 %9, 1
  %11 = mul nsw i32 %8, <badref>
  store i32 %11, ptr %2, align 4
  br label %12

12:                                               ; preds = %7, %6
  %13 = load i32, ptr %2, align 4
  ret i32 %13
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @sum(i32 noundef %0) #0 {
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  store i32 %0, ptr %3, align 4
  %4 = load i32, ptr %3, align 4
  %5 = icmp sle i32 %4, 0
  br i1 %5, label %6, label %7

6:                                                ; preds = %1
  store i32 0, ptr %2, align 4
  br label %12

7:                                                ; preds = %1
  %8 = load i32, ptr %3, align 4
  %9 = load i32, ptr %3, align 4
  %10 = sub nsw i32 %9, 1
  %11 = add nsw i32 %8, <badref>
  store i32 %11, ptr %2, align 4
  br label %12

12:                                               ; preds = %7, %6
  %13 = load i32, ptr %2, align 4
  ret i32 %13
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @multiply(i32 noundef %0, i32 noundef %1) #0 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  store i32 %0, ptr %4, align 4
  store i32 %1, ptr %5, align 4
  %6 = load i32, ptr %5, align 4
  %7 = icmp eq i32 %6, 0
  br i1 %7, label %8, label %9

8:                                                ; preds = %2
  store i32 0, ptr %3, align 4
  br label %15

9:                                                ; preds = %2
  %10 = load i32, ptr %4, align 4
  %11 = load i32, ptr %4, align 4
  %12 = load i32, ptr %5, align 4
  %13 = sub nsw i32 %12, 1
  %14 = add nsw i32 %10, <badref>
  store i32 %14, ptr %3, align 4
  br label %15

15:                                               ; preds = %9, %8
  %16 = load i32, ptr %3, align 4
  ret i32 %16
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @gcd(i32 noundef %0, i32 noundef %1) #0 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  store i32 %0, ptr %4, align 4
  store i32 %1, ptr %5, align 4
  %6 = load i32, ptr %5, align 4
  %7 = icmp eq i32 %6, 0
  br i1 %7, label %8, label %10

8:                                                ; preds = %2
  %9 = load i32, ptr %4, align 4
  store i32 %9, ptr %3, align 4
  br label %15

10:                                               ; preds = %2
  %11 = load i32, ptr %5, align 4
  %12 = load i32, ptr %4, align 4
  %13 = load i32, ptr %5, align 4
  %14 = srem i32 %12, %13
  store i32 <badref>, ptr %3, align 4
  br label %15

15:                                               ; preds = %10, %8
  %16 = load i32, ptr %3, align 4
  ret i32 %16
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @complexCalculation(i32 noundef %0) #0 {
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  store i32 %0, ptr %2, align 4
  %7 = load i32, ptr %2, align 4
  %8 = call i32 @factorial(i32 noundef %7)
  store i32 %8, ptr %3, align 4
  %9 = load i32, ptr %2, align 4
  %10 = call i32 @sum(i32 noundef %9)
  store i32 %10, ptr %4, align 4
  %11 = load i32, ptr %2, align 4
  %12 = call i32 @multiply(i32 noundef %11, i32 noundef 10)
  store i32 %12, ptr %5, align 4
  %13 = load i32, ptr %2, align 4
  %14 = call i32 @gcd(i32 noundef %13, i32 noundef 100)
  store i32 %14, ptr %6, align 4
  %15 = load i32, ptr %2, align 4
  %16 = load i32, ptr %3, align 4
  %17 = call i32 (ptr, ...) @printf(ptr noundef @.str, i32 noundef %15, i32 noundef %16)
  %18 = load i32, ptr %2, align 4
  %19 = load i32, ptr %4, align 4
  %20 = call i32 (ptr, ...) @printf(ptr noundef @.str.1, i32 noundef %18, i32 noundef %19)
  %21 = load i32, ptr %2, align 4
  %22 = load i32, ptr %5, align 4
  %23 = call i32 (ptr, ...) @printf(ptr noundef @.str.2, i32 noundef %21, i32 noundef %22)
  %24 = load i32, ptr %2, align 4
  %25 = load i32, ptr %6, align 4
  %26 = call i32 (ptr, ...) @printf(ptr noundef @.str.3, i32 noundef %24, i32 noundef %25)
  ret void
}

declare i32 @printf(ptr noundef, ...) #1

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"Ubuntu clang version 18.1.3 (1)"}
