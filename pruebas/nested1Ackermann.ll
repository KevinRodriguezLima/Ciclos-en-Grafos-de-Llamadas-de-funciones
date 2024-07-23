; ModuleID = 'nested1Ackermann.c'
source_filename = "nested1Ackermann.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @ackermann(i32 noundef %0, i32 noundef %1) #0 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  store i32 %0, ptr %4, align 4
  store i32 %1, ptr %5, align 4
  %6 = load i32, ptr %4, align 4
  %7 = icmp eq i32 %6, 0
  br i1 %7, label %8, label %11

8:                                                ; preds = %2
  %9 = load i32, ptr %5, align 4
  %10 = add nsw i32 %9, 1
  store i32 %10, ptr %3, align 4
  br label %29

11:                                               ; preds = %2
  %12 = load i32, ptr %4, align 4
  %13 = icmp sgt i32 %12, 0
  br i1 %13, label %14, label %21

14:                                               ; preds = %11
  %15 = load i32, ptr %5, align 4
  %16 = icmp eq i32 %15, 0
  br i1 %16, label %17, label %21

17:                                               ; preds = %14
  %18 = load i32, ptr %4, align 4
  %19 = sub nsw i32 %18, 1
  %20 = call i32 @ackermann(i32 noundef %19, i32 noundef 1)
  store i32 %20, ptr %3, align 4
  br label %29

21:                                               ; preds = %14, %11
  %22 = load i32, ptr %4, align 4
  %23 = sub nsw i32 %22, 1
  %24 = load i32, ptr %4, align 4
  %25 = load i32, ptr %5, align 4
  %26 = sub nsw i32 %25, 1
  %27 = call i32 @ackermann(i32 noundef %24, i32 noundef %26)
  %28 = call i32 @ackermann(i32 noundef %23, i32 noundef %27)
  store i32 %28, ptr %3, align 4
  br label %29

29:                                               ; preds = %21, %17, %8
  %30 = load i32, ptr %3, align 4
  ret i32 %30
}

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"Ubuntu clang version 18.1.3 (1)"}
