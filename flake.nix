{
  description = "Wrap the portable LibreGram Desktop Linux build for NixOS";
  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";

  outputs = { self, nixpkgs }: {
    lib.mkLibreGram = { archive, version, system ? "x86_64-linux" }:
      let pkgs = import nixpkgs { inherit system; };
      in pkgs.callPackage ./packaging/nix/package.nix { inherit archive version; };
  };
}
