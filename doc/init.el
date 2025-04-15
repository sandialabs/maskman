;; Configuration file for Emacs 23+

;; EMACS USUALLY HAS A SPLASH SCREEN ON STARTUP. LET'S GET RID OF THAT AND START WITH A BLANK BUFFER.
(setq inhibit-startup-message t)

;; FIX PARAGRAPH FILLING WIDTH
;; To do this easily, just do M-q
(setq-default fill-column 72)

;; MORE LOGICAL LINE WRAPPING
(setq org-startup-truncated nil)

;; MAKE ORG MODE SPIT OUT SMART QUOTES
(setq org-export-with-smart-quotes t)

;; HANDLE TODO BETTER
(setq org-log-done 'time)
(setq org-log-done 'note)

;; ENABLE EXPORTING
(require 'ox-texinfo)
(require 'ox-man)

;; REMOVE HYPERREF (DON'T FORGET TO MANUALLY ADD IT IN)
(delete '("" "hyperref" nil) org-latex-default-packages-alist)

;; EMACS TERMINAL TAB
(setq evil-want-C-i-jump nil)

