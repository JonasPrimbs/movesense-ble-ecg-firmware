# Map the given VARIANT_NAME to HWCONFIG. The default selection without
# variant name is Amsterdam to keep the backward compatiblity with TC.
# Also HWCONFIG is mapped to variant name for keeping the backwards
# compatibility. Maybe it can be removed later when this change propagates
# to all branhces.
# Generates alse the list of supported HWCONFIGs.
set(SUPPORTED_HWCONFIGS "SS2_Simu" "SS2_NAND_Simu")

if(NOT DEFINED VARIANT)
    set(DEFAULT_HWCONFIG "SS2_Simu")
    if (NOT HWCONFIG)
        set(VARIANT "Avocado")
    elseif(HWCONFIG STREQUAL "SS2_Simu")
        set(VARIANT "Avocado")
    endif()
else()
    # Please note that MATCHES is used instead of STREQUAL.
    # Usually STREQUAL is the prefered way but MATCHES is used
    # here because it also includes the china variants which
    # have the 'C' postfix in their variant name.
    if((VARIANT MATCHES "Avocado"))
        set(DEFAULT_HWCONFIG "SS2_Simu")
    else()
        message(FATAL_ERROR "Unknown variant name: ${VARIANT}")
    endif()
endif()
