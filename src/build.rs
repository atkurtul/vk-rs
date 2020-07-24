extern crate cc;

fn main() {   
    let mut files = vec![
        "loader/loader.c",
        "loader/impl.c",
    ];

    #[cfg(feature = "xlib")]
    files.push("loader/impl_xlib.c");

    #[cfg(feature = "xcb")]
    files.push("loader/impl_xcb.c");

    #[cfg(feature = "wayland")]
    files.push("loader/impl_wayland.c");

    #[cfg(feature = "android")]
    files.push("loader/impl_android.c");

    #[cfg(feature = "win32")]
    files.push("loader/impl_win32.c");



    cc::Build::new()
        .files(files.iter())
        .opt_level(3)
        .compile("vk-rs");
}

