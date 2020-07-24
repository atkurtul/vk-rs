extern crate cc;

fn main() {    
    let files = [
        "loader/loader.c",
        "loader/impl.c",
        "loader/impl_win32.c",   
    ];

    cc::Build::new()
        .files(files.iter())
        .opt_level(3)
        .compile("hello");
    println!("cargo:rerun-if-changed=src/main.c");
}

