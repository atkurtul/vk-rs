extern crate cc;

fn main() {   
    let mut files = vec![
        "loader/loader.c",
        "loader/impl.c",
        "loader/impl_linux.c",
    ];

    #[cfg(feature = "win32")]
    files.push("loader/impl_win32.c");


    cc::Build::new()
        .files(files.iter())
        .opt_level(3)
        .compile("vk-rs");
}

