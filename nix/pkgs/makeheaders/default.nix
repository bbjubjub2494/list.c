{ stdenv }:
stdenv.mkDerivation {
  name = "makeheaders";
  src = ./makeheaders.tar.xz;
  buildPhase = ''
    make makeheaders
    '';
  installPhase = ''
    install -D -t $out/bin/ makeheaders
    '';
}
