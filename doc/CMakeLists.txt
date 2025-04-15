
cmake_minimum_required(VERSION 3.21)

# Find LaTeX
find_package(LATEX REQUIRED)

# Set Metadata
set(MaskmanDocBasename maskman)
set(MaskmanDocSource ${MaskmanDocBasename}.org)

configure_file(${MaskmanDocSource} ${MaskmanDocSource} COPYONLY)
configure_file(index.html index.html COPYONLY)
configure_file(init.el init.el COPYONLY)
configure_file(latexmkrc latexmkrc COPYONLY)
configure_file(load.el load.el COPYONLY)
configure_file(preamble.tex preamble.tex COPYONLY)

# TARGET: ASCII Text
set(MaskmanDocCurrent ${MaskmanDocBasename}.txt)
add_custom_command(
  OUTPUT ${MaskmanDocCurrent}
  COMMAND emacs -Q -l load.el -l init.el ${MaskmanDocSource} --batch -f org-ascii-export-to-ascii --kill
  DEPENDS ${MaskmanDocSource}
)
add_custom_target(maskman_doc_txt ALL DEPENDS ${MaskmanDocCurrent})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${MaskmanDocCurrent} DESTINATION share/txt)

# TARGET: HTML
set(MaskmanDocCurrent ${MaskmanDocBasename}.html)
add_custom_command(
  OUTPUT ${MaskmanDocCurrent}
  COMMAND emacs -Q -l load.el -l init.el ${MaskmanDocSource} --batch -f org-html-export-to-html --kill
  DEPENDS ${MaskmanDocSource}
)
add_custom_target(maskman_doc_html ALL DEPENDS ${MaskmanDocCurrent})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${MaskmanDocCurrent} DESTINATION share/html)

# TARGET: Unix Manual
set(MaskmanDocCurrent ${MaskmanDocBasename}.1)
add_custom_command(
  OUTPUT ${MaskmanDocCurrent}
  COMMAND emacs -Q -l load.el -l init.el ${MaskmanDocSource} --batch -f org-man-export-to-man --kill
  COMMAND ${CMAKE_COMMAND} -E copy ${MaskmanDocBasename}.man ${MaskmanDocCurrent}
  DEPENDS ${MaskmanDocSource}
)
add_custom_target(maskman_doc_man ALL DEPENDS ${MaskmanDocCurrent})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${MaskmanDocCurrent} DESTINATION share/man/man1)

# TARGET: Unix Info
set(MaskmanDocCurrent ${MaskmanDocBasename}.info)
add_custom_command(
  OUTPUT ${MaskmanDocCurrent}
  COMMAND emacs -Q -l load.el -l init.el ${MaskmanDocSource} --batch -f org-texinfo-export-to-info --kill
  DEPENDS ${MaskmanDocSource}
)
add_custom_target(maskman_doc_info ALL DEPENDS ${MaskmanDocCurrent})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${MaskmanDocCurrent} DESTINATION share/info)

# TARGET: LaTeX Source
set(MaskmanDocCurrent ${MaskmanDocBasename}.tex)
add_custom_command(
  OUTPUT ${MaskmanDocCurrent}
  COMMAND emacs -Q -l load.el -l init.el ${MaskmanDocSource} --batch -f org-latex-export-to-latex --kill
  DEPENDS ${MaskmanDocSource}
)
add_custom_target(maskman_doc_tex ALL DEPENDS ${MaskmanDocCurrent})
set(TEX_FILES "${CMAKE_CURRENT_BINARY_DIR}/${MaskmanDocCurrent};${CMAKE_CURRENT_BINARY_DIR}/preamble.tex")
install(FILES ${TEX_FILES} DESTINATION share/markup)

# TARGET: PDF from LaTeX Source
set(MaskmanDocCurrent ${MaskmanDocBasename}.pdf)
add_custom_command(
  OUTPUT ${MaskmanDocCurrent}
  COMMAND emacs -Q -l load.el -l init.el ${MaskmanDocSource} --batch -f org-latex-export-to-latex --kill
  COMMAND latexmk -lualatex -r latexmkrc ${MaskmanDocBasename}.tex
  DEPENDS ${MaskmanDocSource}
)
set(PDF_DEPENDS "maskman_doc_tex;${MaskmanDocCurrent}")
add_custom_target(maskman_doc_pdf ALL DEPENDS ${PDF_DEPENDS})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${MaskmanDocCurrent} DESTINATION share/doc)

# TARGET: Markdown
set(MaskmanDocCurrent ${MaskmanDocBasename}.md)
add_custom_command(
  OUTPUT ${MaskmanDocCurrent}
  COMMAND emacs -Q -l load.el -l init.el ${MaskmanDocSource} --batch -f org-md-export-to-markdown --kill
  DEPENDS ${MaskmanDocSource}
)
add_custom_target(maskman_doc_md ALL DEPENDS ${MaskmanDocCurrent})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${MaskmanDocCurrent} DESTINATION share/markup)

# TARGET: ODT
set(MaskmanDocCurrent ${MaskmanDocBasename}.odt)
add_custom_command(
  OUTPUT ${MaskmanDocCurrent}
  COMMAND emacs -Q -l load.el -l init.el ${MaskmanDocSource} --batch -f org-odt-export-to-odt --kill
  DEPENDS ${MaskmanDocSource}
)
add_custom_target(maskman_doc_odt ALL DEPENDS ${MaskmanDocCurrent})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${MaskmanDocCurrent} DESTINATION share/doc)
