## Fcitx-KeyTheme

Fcitx-KeyTheme is a plugin for Fcitx input method framework that provide some
extra shortcut as well as the ability to redefine some built-in function key.

NOTE: The redirection feature might not work for some input method
(e.g. fcitx-sunpinyin)

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
