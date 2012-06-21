## Fcitx-KeyTheme

Fcitx-KeyTheme is a plugin for Fcitx input method framework that provide some
extra shortcut as well as the ability to redefine some built-in function key.

NOTE: The redirection feature now **SUPPORT** almost all input method engine
(**INCLUDING fcitx-sunpinyin**). However, it might not work for certain
input method that rely on key release events (e.g. some Japaness input method
according to Weng Xuetian). The redirection also only work for internal Fcitx
action (i.e. you cannot redirect a key to work as another key when the
input method is not active).

### License

Fcitx-KeyTheme is a free software released under GPLv2.

### Install (From Source)

    $git clone https://github.com/yuyichao/fcitx-keytheme.git

    $cd fcitx-keytheme
    $mkdir -p build
    $cd build
    $cmake .. -DCMAKE_INSTALL_PREFIX=/usr
    $make
    #make install
