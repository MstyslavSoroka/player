```
nix-shell 
``` 
> or install the packages in the shell.nix file to run
```
clang main.c -o player $(pkg-config --cflags --libs gtk4 glib-2.0) && ./player
```
