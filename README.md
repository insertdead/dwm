# dwm

## Patches
Patches are in the patches folder if you are interested in applying them to your
own build. The commit version used is 6.2.

Quick guide to patching your own build:
```
git clone https://git.suckless.org/dwm
cd dwm
git checkout 6.2 -c new
patch < NAME_OF_PATCH
```

## Installation
If an error appears please open an issue on the github page.
```
git clone https://github.com/insertdead/dwm.git
cd dwm
make
sudo make install
```

### Dependencies
* `libxft-bgra` from the aur
  
  Please install `libxft-bgra` from the aur! This enables usage of color emoji
  in the dwm statusbar, which would otherwise crash it due to a bug in libxft.
