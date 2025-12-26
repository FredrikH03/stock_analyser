{
  description = "Dev shell with GCC, Make and Zig";

  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";

  outputs = { self, nixpkgs }: 
    let
      system = "x86_64-linux";
      pkgs = import nixpkgs { inherit system; };
    in
    {
      devShells.${system}.default = pkgs.mkShell {
        buildInputs = with pkgs; [
          gcc
          gnumake
          zig
          boost
          newman
        ];

        shellHook = ''
          echo "the codening can commence"
          export PS1="\e[0;32m[nix-dev \w]\$ \e[0m"
        '';
      };
    };
}
