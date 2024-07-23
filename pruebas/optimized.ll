; ModuleID = 'ejemploC.ll'
source_filename = "ejemploC.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

module asm ".globl _ZSt21ios_base_library_initv"

%"class.std::basic_ostream" = type { ptr, %"class.std::basic_ios" }
%"class.std::basic_ios" = type { %"class.std::ios_base", ptr, i8, i8, ptr, ptr, ptr, ptr }
%"class.std::ios_base" = type { ptr, i64, i64, i32, i32, i32, ptr, %"struct.std::ios_base::_Words", [8 x %"struct.std::ios_base::_Words"], i32, ptr, %"class.std::locale" }
%"struct.std::ios_base::_Words" = type { ptr, i64 }
%"class.std::locale" = type { ptr }

@_ZSt4cout = external global %"class.std::basic_ostream", align 8
@.str = private unnamed_addr constant [14 x i8] c"In functionA\0A\00", align 1
@.str.1 = private unnamed_addr constant [14 x i8] c"In functionB\0A\00", align 1
@.str.2 = private unnamed_addr constant [14 x i8] c"Factorial of \00", align 1
@.str.3 = private unnamed_addr constant [5 x i8] c" is \00", align 1
@.str.4 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1

; Function Attrs: mustprogress noinline optnone uwtable
define dso_local noundef i32 @_Z9factoriali(i32 noundef %0) #0 {
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

; Function Attrs: mustprogress noinline optnone uwtable
define dso_local void @_Z9functionAv() #0 {
  %1 = call noundef nonnull align 8 dereferenceable(8) ptr <badref>(ptr noundef nonnull align 8 dereferenceable(8) @_ZSt4cout, ptr noundef @.str)
  call void @_Z9functionBv()
  ret void
}

; Function Attrs: mustprogress noinline optnone uwtable
define dso_local void @_Z9functionBv() #0 {
  %1 = call noundef nonnull align 8 dereferenceable(8) ptr <badref>(ptr noundef nonnull align 8 dereferenceable(8) @_ZSt4cout, ptr noundef @.str.1)
  call void @_Z9functionAv()
  ret void
}

attributes #0 = { mustprogress noinline optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"Ubuntu clang version 18.1.3 (1)"}
