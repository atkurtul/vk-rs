clang -c loader.c impl.c impl_xlib.c
llvm-ar -rc ../vk.lib loader.o impl.o impl_xlib.o