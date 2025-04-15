#Anthony M. Agelastos, 2017/11/01
#AMA's Standard GNU Make Makefile, version 1.0
#This Makefile builds documentation
#
#The following targets are available
# all:    (DEFAULT) Performs all tasks to clean, build, and install the documentation
# initpurge:        Deletes all output and intermediate files from previous build
# clean:            Deletes intermediate files from previous build
# spellcheck:       Does spellchecking via `aspell` or `hunspell` if installed
# gedit:            Opens up Emacs, in graphical mode, and loads all relevant files into buffer
# cedit:            Opens up Emacs, in console mode, and loads all relevant files into buffer


### USER SETTINGS ###
TITLE := maskman
INSTALLDIR := share
EMACS_CLI ?= emacs -nw
EMACS_GUI ?= emacs


### COMPILER SETTINGS (ONLY ADMINS BELOW) ###

# define how to build with LaTeX
MAKPDF := latexmk -lualatex -r latexmkrc
CLEAN := latexmk -c -lualatex -r latexmkrc
PURGE := latexmk -C -lualatex -r latexmkrc

# define spell checker
MYHUNSPELL := $(shell command -v hunspell 2> /dev/null)
MYASPELL := $(shell command -v aspell 2> /dev/null)
ifdef MYASPELL
	SPELLCHECKTARGET := spellcheckaspell
endif
ifdef MYHUNSPELL
	SPELLCHECKTARGET := spellcheckhunspell
endif

# define pandoc
PANDOC := $(shell command -v pandoc 2> /dev/null)
ifdef PANDOC
	PANDOCTARGET := $(TITLE).mediawiki
endif


### OTHER VARIABLES ###
LOGAPPEND := $(shell date '+%Y%m%d-%H%M%S')
BASEDIR := $(CURDIR)


### DIRECTORY LAYOUT ###
# vpath %.tex doc


### MAIN TARGETS ###
all: initpurge build-all install-all clean
build-all: $(TITLE).txt $(TITLE).pdf $(TITLE).md $(TITLE).man $(TITLE).info $(TITLE).html $(TITLE).odt $(PANDOCTARGET)
build-nolatex: $(TITLE).txt $(TITLE).md $(TITLE).man $(TITLE).info $(TITLE).html $(TITLE).odt $(PANDOCTARGET)
ifndef PANDOCTARGET
	$(warning "Pandoc was not found so Mediawiki export will not occur.")
endif
install-org:
	mkdir -p $(INSTALLDIR)/markup
	cp $(TITLE).org $(INSTALLDIR)/markup
install-man:
	mkdir -p $(INSTALLDIR)/man/man1
	cp $(TITLE).man $(INSTALLDIR)/man/man1
install-info:
	mkdir -p $(INSTALLDIR)/info
	cp $(TITLE).info $(INSTALLDIR)/info
install-txt:
	mkdir -p $(INSTALLDIR)/txt
	cp -a $(TITLE).txt $(INSTALLDIR)/txt
install-html:
	mkdir -p $(INSTALLDIR)/html
	cp $(TITLE).html $(INSTALLDIR)/html
install-markdown:
	mkdir -p $(INSTALLDIR)/markup
	cp $(TITLE).md $(INSTALLDIR)/markup
install-odt:
	mkdir -p $(INSTALLDIR)/doc
	cp $(TITLE).odt $(INSTALLDIR)/doc
install-pdf:
	mkdir -p $(INSTALLDIR)/markup
	mkdir -p $(INSTALLDIR)/doc
	cp $(TITLE).pdf $(INSTALLDIR)/doc
	cp $(TITLE).tex $(INSTALLDIR)/markup
install-mediawiki:
	mkdir -p $(INSTALLDIR)/markup
	-cp $(TITLE).mediawiki $(INSTALLDIR)/markup
install-all: install-org install-man install-info install-txt install-html install-markdown install-odt install-pdf install-mediawiki
install-nolatex: install-org install-man install-info install-txt install-html install-markdown install-odt install-mediawiki
uninstall:
	-rm -rf $(INSTALLDIR)


### RELATIONAL TREE ###
%.tex: %.org
	$(EMACS_CLI) -Q -l load.el -l init.el $< --batch -f org-latex-export-to-latex --kill
%.pdf: %.tex
	$(MAKPDF) $<
%.txt: %.org
	$(EMACS_CLI) -Q -l load.el -l init.el $< --batch -f org-ascii-export-to-ascii --kill
%.md: %.org
	$(EMACS_CLI) -Q -l load.el -l init.el $< --batch -f org-md-export-to-markdown --kill
%.html: %.org
	$(EMACS_CLI) -Q -l load.el -l init.el $< --batch -f org-html-export-to-html --kill
%.man: %.org
	$(EMACS_CLI) -Q -l load.el -l init.el $< --batch -f org-man-export-to-man --kill
%.info: %.org
	$(EMACS_CLI) -Q -l load.el -l init.el $< --batch -f org-texinfo-export-to-info --kill
%.mediawiki: %.org
	$(PANDOC) -f org -t mediawiki -o $@ $<
%.odt: %.org
	$(EMACS_CLI) -Q -l load.el -l init.el $< --batch -f org-odt-export-to-odt --kill


### ADMINISTRATIVE TARGETS ###
.PHONY: gedit
gedit:
	-( $(EMACS_GUI) -l $(BASEDIR)/init.el $(BASEDIR)/Makefile $(BASEDIR)/init.el $(BASEDIR)/preamble.tex $(BASEDIR)/doc_req.org $(BASEDIR)/$(TITLE).org & )

.PHONY: cedit
cedit:
	-$(EMACS_CLI) -l $(BASEDIR)/init.el $(BASEDIR)/Makefile $(BASEDIR)/init.el $(BASEDIR)/preamble.tex $(BASEDIR)/doc_req.org $(BASEDIR)/$(TITLE).org

.PHONY: clean
clean:
	-$(EMACS_CLI) -Q -l load.el -l init.el $(TITLE).org --batch -f org-latex-export-to-latex --kill
	$(CLEAN) $(TITLE).tex
	-rm -f $(TITLE).tex
	-rm -f $(TITLE).texi
	-find . -name "*.nlo" -type f -exec rm -f '{}' \;
	-find . -name "*.nls" -type f -exec rm -f '{}' \;
	-find . -name "*.bbl" -type f -exec rm -f '{}' \;
	-find . -name "*~" -type f -exec rm -f '{}' \;
	-find . -name "*.swp" -type f -exec rm -f '{}' \;
	-find . -name ".DS_Store" -type f -exec rm -f '{}' \;
	-find . -name "*.dSYM" -type d -exec rm -rf '{}' \;

.PHONY: initpurge
initpurge: uninstall
	-rm -f $(TITLE).txt
	-$(EMACS_CLI) -Q -l load.el -l init.el $(TITLE).org --batch -f org-latex-export-to-latex --kill
	-$(PURGE) $(TITLE).tex
	-rm -f $(TITLE).tex
	-rm -f $(TITLE).md
	-rm -f $(TITLE).html
	-rm -f $(TITLE).man
	-rm -f $(TITLE).info
	-rm -f $(TITLE).texi
	-rm -f $(TITLE).mediawiki
	-rm -f $(TITLE).odt
	-find . -name "*.nlo" -type f -exec rm -f '{}' \;
	-find . -name "*.nls" -type f -exec rm -f '{}' \;
	-find . -name "*.bbl" -type f -exec rm -f '{}' \;
	-find . -name "*~" -type f -exec rm -f '{}' \;
	-find . -name "*.swp" -type f -exec rm -f '{}' \;
	-find . -name ".DS_Store" -type f -exec rm -f '{}' \;
	-find . -name "*.dSYM" -type d -exec rm -rf '{}' \;

.PHONY: distclean
distclean: initpurge

.PHONY: spellcheckaspell
spellcheckaspell:
	-$(EMACS_CLI) -Q -l load.el -l init.el $(TITLE).org --batch -f org-latex-export-to-latex --kill
	-find . -type f -name $(TITLE).tex -exec aspell -c '{}' --lang=en_US --mode=tex \;
	-rm -f $(TITLE).tex

.PHONY: spellcheckhunspell
spellcheckhunspell:
	-$(EMACS_CLI) -Q -l load.el -l init.el $(TITLE).org --batch -f org-latex-export-to-latex --kill
	-find . -type f -name $(TITLE).tex -exec hunspell -t -d en_US '{}' \;
	-rm -f $(TITLE).tex

.PHONY: spellcheck
spellcheck: $(SPELLCHECKTARGET)
ifndef SPELLCHECKTARGET
	$(error "No spell checking application was found.")
endif
