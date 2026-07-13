{
  pkgs ? import <nixpkgs> { },
}:

pkgs.mkShell {
  packages = with pkgs; [
    clang
    gnumake
    pkg-config
    gtk4
    glib
    sqlite
  ];

  shellHook = ''
    export PKG_CONFIG_PATH=${pkgs.gtk4.dev}/lib/pkgconfig:${pkgs.glib.dev}/lib/pkgconfig:$PKG_CONFIG_PATH
  '';
}
