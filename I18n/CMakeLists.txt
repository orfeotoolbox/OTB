#Note, this is mostly inspired by the KWWidget procedure

# ---------------------------------------------------------------------------
# OTB_GET_PO_FILENAME
# Given a PO directory, a prefix and a locale, return the filename to the
# translation file (po) for that locale.
# 'varname': name of the var the translation filename should be stored into
# 'po_dir': path to the po directory where the PO file are stored
# 'po_prefix': string that is used to prefix each translation file.
# 'locale': a locale (say, "fr")

macro(OTB_GET_PO_FILENAME varname po_dir po_prefix locale)

  set(${varname} "${po_dir}/${po_prefix}${locale}.po")

endmacro(OTB_GET_PO_FILENAME)

# ---------------------------------------------------------------------------
# OTB_GET_MO_FILENAME
# Given a translation domain, a MO build directory, and a locale, return the
# filename to the binary translation file (mo) for that locale and domain.
# 'varname': name of the var the translation filename should be stored into
# 'domain_name': translation domain name (i.e. name of application or library)
# 'mo_build_dir': directory where the binary MO files should be saved to
# 'locale': a locale (say, "fr")

macro(OTB_GET_MO_FILENAME varname domain_name mo_build_dir locale)

  set(${varname} "${mo_build_dir}/${locale}/LC_MESSAGES/${domain_name}.mo")

endmacro(OTB_GET_MO_FILENAME)



set(locale_list "fr;")
set(po_dir "${OTB_SOURCE_DIR}/I18n")
set(po_prefix "otb-")
set(mo_build_dir "${OTB_BINARY_DIR}/I18n")
set(domain_name "otb")

set(mo_files)


foreach(locale ${locale_list})
    otb_get_po_filename(po_file
      "${po_dir}" "${po_prefix}" "${locale}")
    otb_get_mo_filename(mo_file
        "${domain_name}" "${mo_build_dir}" "${locale}")
    get_filename_component(mo_dir "${mo_file}" PATH)
    file(MAKE_DIRECTORY ${mo_dir})
    set(mo_files ${mo_files} ${mo_file})
    add_custom_command(
        OUTPUT "${mo_file}"
        DEPENDS "${po_file}"
        COMMAND ${GETTEXT_MSGFMT_EXECUTABLE}
        ARGS --output-file=${mo_file} -c -v "${po_file}"
        )
endforeach(locale ${locale_list})

add_custom_target(OTBI18N ALL DEPENDS ${mo_files})
