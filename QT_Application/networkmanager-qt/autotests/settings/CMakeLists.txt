macro(NETWORKMANAGERQT_AUTOTESTS)
    foreach(_testname ${ARGN})
        ecm_add_test(${_testname}.cpp LINK_LIBRARIES Qt${QT_MAJOR_VERSION}::Test NetworkManagerQt_static)
    endforeach(_testname)
endmacro(NETWORKMANAGERQT_AUTOTESTS)

NETWORKMANAGERQT_AUTOTESTS(
    8021xsettingtest
    bridgesettingtest
    bridgeportsettingtest
    connectionsettingtest
    ipv4settingtest
    ipv6settingtest
    matchsettingtest
    macsecsettingtest
    tcsettingtest
    usersettingtest
    wiredsettingtest
    wirelesssettingtest
    wirelesssecuritysettingtest
)

