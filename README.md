# pkgwrapp - a wrapper for multiple package managers
pkgwrapp is a wrapper for multiple package managers. This means you can use `pkg install <package name here>` and it will go over the package managers you configured to it and will try untill it finds one that you can install the package from, and `pkg upgrade` will upgrade packages from all package managers configured.

## configuration
using the configuration file in `/etc/pkgwrapp` you may configure it to use different package managers. an example config can be seen in `exampleconfig.txt`.

the configuration looks like: 
```
PKG
name: apt
update: apt update && apt upgrade
install: apt install
remove: apt remove
root: true

PKG
name: flatpak
update: flatpak update
install: flatpak install
remove: flatpak uninstall
root: false
```
- PKG: a seperator for the config parser - have it every time you start a new package manager entry. 
- name: name of package manager
- update: command to upgrade all packages
- install: command to install a package
- remove: command to remove package
- root: whether the package manager needs root acess or not. will use `su -c` if needed, in order to not break compatibility with systems using `doas`.

## how to install:
you can clone the repo <repo url here> and run the BUILD.sh script as root, which will build and install `pkgwrapp` with the defualt config example file (apt & flatpak) into `/etc/pkgwrapp` and the binary itself will be in `/usr/bin/pkg`.