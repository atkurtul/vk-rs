extern crate cc;

fn main() {   
    let files = vec![
        "loader/loader.c",
        "loader/impl.c",
        "loader/impl_platform.c",
    ];

    cc::Build::new()
        .files(files.iter())
        .opt_level(3)
        .compile("vk-rs");
}

