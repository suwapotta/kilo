{
  description = "C (Devshell)";

  inputs = {
    flake-parts.url = "github:hercules-ci/flake-parts";
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    devshell.url = "github:numtide/devshell";
  };

  outputs =
    inputs@{ flake-parts, ... }:
    flake-parts.lib.mkFlake { inherit inputs; } {
      imports = [ inputs.devshell.flakeModule ];

      systems = [ "x86_64-linux" ];

      perSystem =
        { pkgs, ... }:

        {
          devshells.default = {
            name = "C";

            packages = with pkgs; [
              gcc
              clang-tools
              lldb
              # bear
            ];

            devshell.motd = ''
               {45}Welcome to C.{reset}
              Enter 'menu' for general commands.
            '';
          };
        };
    };
}
