extern crate cc;

fn main() {   
    #[cfg(target_os = "linux")]
    let target_impl = "loader/impl_xlib.c";

    #[cfg(target_os = "windows")]
    let target_impl = "loader/impl_win32.c";

    let files = [
        "loader/loader.c",
        "loader/impl.c",
        target_impl   
    ];

    cc::Build::new()
        .files(files.iter())
        .opt_level(3)
        .compile("vk-rs");
}

