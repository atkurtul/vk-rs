clang -c loader.c impl.c impl_wayland.c
llvm-ar -rc ../vk.lib loader.o impl.o impl_wayland.o