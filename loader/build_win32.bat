clang -c loader.c impl.c impl_win32.c
llvm-ar -rc ../vk.lib loader.o impl.o impl_win32.o