{ pkgs ? import <nixpkgs> {} }:

import ./packaging/nix/release.nix { inherit pkgs; }
