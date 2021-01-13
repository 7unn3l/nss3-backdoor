workspace "nss3"
    configurations { "Debug", "Release" }


project "nss3"
    kind "SharedLib"
    architecture "x86_64"
    language "c++"
    targetdir "bin/%{cfg.buildcfg}"


filter "files:**"
    flags {"NoPCH"}

staticruntime "off"
