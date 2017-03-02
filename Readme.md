dwm - dynamic window manager
============================
dwm is an extremely fast, small, and dynamic window manager for X.

Requirements
------------
In order to build dwm you need the Xlib header files.
Also if you want mpd support you need libmpd installed in your system.

Installation
------------
### Arch
After cloning repository run `makepkg -si` and instal with pacman as usual.
If you want make changes in configuration than dont forget commit them because PKGBUILD uses master branch for building.

### Other GNU/Linux distributions
Edit config.mk to match your local setup (dwm is installed into
the /usr/local namespace by default).
Afterwards enter the following command to build and install dwm (if
necessary as root):

    make clean install

Note than you need specify X11 headers and libs directories like this:

    make X11INC=/usr/include/X11 X11LIB=/usr/lib/X11 clean install


Running dwm
-----------
Add the following line to your .xinitrc to start dwm using startx:

    exec dwm

In order to connect dwm to a specific display, make sure that
the DISPLAY environment variable is set correctly, e.g.:

    DISPLAY=foo.bar:1 exec dwm

(This will start dwm on display :1 of the host foo.bar.)

In order to display status info in the bar, you can do something
like this in your .xinitrc:

    ```shell
    while xsetroot -name "`date` `uptime | sed 's/.*,//'`"
    do
    	sleep 1
    done &
    exec dwm
    ```

Configuration
-------------
The configuration of dwm is done by creating a custom config.h
and (re)compiling the source code.

