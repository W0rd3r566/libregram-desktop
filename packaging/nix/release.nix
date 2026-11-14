{ pkgs }:

pkgs.callPackage ./package.nix {
  version = "0.1.0-beta.1";
  archive = pkgs.fetchurl {
    url = "https://github.com/W0rd3r566/libregram-desktop/releases/download/v0.1.0-beta.1/LibreGram-0.1.0-beta.1-linux-x86_64.tar.xz";
    sha256 = "f1ac18cbef4cade712e32813617c22e3ee3a0c835294f240cdb3522462bd5d8b";
  };
}
