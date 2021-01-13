workspace "nss3"
    configurations { "Debug", "Release" }


project "nss3"
    kind "SharedLib"
    architecture "x86_64"
    language "c++"

configuration "*"
    targetdir "bin/%{cfg.buildcfg}"
    objdir    "bin/obj/"


filter "files:**"
    flags {"NoPCH"}

staticruntime "off"
