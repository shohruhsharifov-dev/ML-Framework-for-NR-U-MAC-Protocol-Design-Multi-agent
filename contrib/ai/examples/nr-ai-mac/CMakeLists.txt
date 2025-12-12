build_lib_example(
  NAME ns3ai_nrmac_gym
        SOURCE_FILES
            use-gym/nr-ai-mac.cc
            use-gym/nr-mac-env.cc
        LIBRARIES_TO_LINK
            ${libai}
            ${libcore}
            ${libpoint-to-point}
            ${libpoint-to-point-layout}
            ${libnetwork}
            ${libapplications}
            ${libmobility}
            ${libcsma}
            ${libinternet}
            ${libspectrum}
            ${libwifi}
            ${libflow-monitor}
            ${libvr-app}
            ${libnr}
            ${libnr-u}
)


add_executable(nr-ai-mac
    use-gym/nr-ai-mac.cc
    use-gym/nr-mac-env.cc
)

target_link_libraries(nr-ai-mac
    PRIVATE
        ${libai}
        ${libcore}
        ${libpoint-to-point}
        ${libpoint-to-point-layout}
        ${libnetwork}
        ${libapplications}
        ${libmobility}
        ${libcsma}
        ${libinternet}
        ${libspectrum}
        ${libwifi}
        ${libflow-monitor}
        ${libvr-app}
        ${libnr}
        ${libnr-u}
)
