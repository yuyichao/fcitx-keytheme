include_directories(${FCITX4_FCITX_INCLUDE_DIRS})
include_directories(${FCITX4_FCITX_CONFIG_INCLUDE_DIRS})
include_directories(${FCITX4_FCITX_UTILS_INCLUDE_DIRS})
include_directories(${PROJECT_BINARY_DIR})

set(fcitx_keytheme_sources
  eim.c
  config.c
  redirect.c
  shortcut.c
  )
fcitx_add_addon_full(keytheme
  SOURCES ${fcitx_keytheme_sources}
  DESC_SRC ../data/fcitx-keytheme.desc
  CONF_SRC ../data/fcitx-keytheme.conf)
