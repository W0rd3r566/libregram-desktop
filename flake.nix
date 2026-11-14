{
  description = "Wrap the portable LibreGram Desktop Linux build for NixOS";
  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";

  outputs = { self, nixpkgs }: {
    packages.x86_64-linux = rec {
      libregram-desktop = import ./default.nix {
        pkgs = import nixpkgs { system = "x86_64-linux"; };
      };
      default = libregram-desktop;
    };
    lib.mkLibreGram = { archive, version, system ? "x86_64-linux" }:
      let pkgs = import nixpkgs { inherit system; };
      in pkgs.callPackage ./packaging/nix/package.nix { inherit archive version; };
  };
}
