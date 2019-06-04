let
  # use a pinned version of nixpkgs
  nixpkgs-version = "19.03";
  nixpkgs = (import (builtins.fetchTarball {
    # Descriptive name to make the store path easier to identify
    name = "nixpkgs-${nixpkgs-version}";
    url = "https://github.com/nixos/nixpkgs/archive/${nixpkgs-version}.tar.gz";
    # Hash obtained using `nix-prefetch-url --unpack <url>`
    sha256 = "0q2m2qhyga9yq29yz90ywgjbn9hdahs7i8wwlq7b55rdbyiwa5dy";
  }) {}).extend (self: super: {
    pythonPackages = self.python37Packages;
    llvmPackages = self.llvmPackages_7;
  });
in
with nixpkgs;
mkShell {
  buildInputs = [
    # C Compiler
    llvmPackages.clang-unwrapped

    # Tools
    gcc
    doxygen
    tup
    clang-tools
    include-what-you-use

    # C libraries
    glibc glibc.dev glibc.debug

    # Python packages
    python37Packages.hypothesis
    python37Packages.sphinx
    python37Packages.breathe
    python37Packages.cffi
    ];

    NIX_DEBUG_DIR = "${glibc.debug}/lib/debug";
    # add in ~/.gdbinit :
    # py
    #   import os, gdb
    #   debug_file_directory = os.getenv("NIX_DEBUG_DIR")
    #   if debug_file_directory is not None:
    #     gdb.execute("set debug-file-directory "+debug_file_directory)
    # end
}
