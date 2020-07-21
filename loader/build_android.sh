clang -c loader.c impl.c impl_android.c
llvm-ar -rc ../vk.lib loader.o impl.o impl_android.o