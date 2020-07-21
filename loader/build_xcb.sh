clang -c loader.c impl.c impl_xcb.c
llvm-ar -rc ../vk.lib loader.o impl.o impl_xcb.o