# Configuración de latexmk para el proyecto.
# Permite compilar con `latexmk cap01.tex` (o desde el editor) obteniendo
# el mismo resultado que con `make cap01`.
#
# IMPORTANTE: el motor es XeLaTeX, no pdflatex. Es lo que permite usar la
# fuente real Times New Roman instalada en el sistema.

$pdf_mode      = 5;          # 5 = xelatex
$bibtex_use    = 2;
$out_dir       = 'build';
$emulate_aux   = 1;
$xelatex       = 'xelatex -interaction=nonstopmode -file-line-error -synctex=1 %O %S';

# biblatex usa biber; latexmk lo detecta solo a partir del archivo .bcf
$clean_ext .= ' bbl run.xml bcf synctex.gz xdv';
