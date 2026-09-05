# =====================================================================
#  Makefile — Proyecto Paralelo (libro de 7 capítulos)
#
#  Uso:
#     make cap01        Compila la entrega individual del capítulo 1
#     make cap03        ... del capítulo 3, etc.
#     make libro        Compila el libro completo unificado
#     make capitulos    Compila las 7 entregas individuales
#     make all          Compila las 7 entregas + el libro
#     make clean        Borra archivos intermedios (deja los PDF)
#     make distclean    Borra build/ y entregas/ por completo
#     make watch-cap01  Recompila el capítulo 1 al guardar cambios
#     make verificar    Comprueba fuente y tamaños de letra en los PDF
#
#  Los PDF listos para entregar quedan en entregas/
# =====================================================================

BUILD    := build
OUT      := entregas
LATEXMK  := latexmk
# -xelatex: obligatorio para usar la fuente real Times New Roman del sistema.
FLAGS    := -xelatex -interaction=nonstopmode -file-line-error -halt-on-error -outdir=$(BUILD)

CAPS     := cap01 cap02 cap03 cap04 cap05 cap06 cap07

.PHONY: all capitulos libro clean distclean help verificar $(CAPS)

help:
	@echo "Objetivos disponibles:"
	@echo "  make cap01 .. cap07   Entrega individual de un capítulo"
	@echo "  make capitulos        Las 7 entregas individuales"
	@echo "  make libro            Libro completo unificado"
	@echo "  make all              Todo lo anterior"
	@echo "  make clean            Limpia intermedios"
	@echo "  make distclean        Limpia build/ y entregas/"
	@echo "  make watch-cap01      Recompila al guardar"
	@echo "  make verificar        Revisa fuente y tamaños de letra"

all: capitulos libro

capitulos: $(CAPS)

$(CAPS): cap%: cap%.tex
	@mkdir -p $(BUILD) $(OUT)
	$(LATEXMK) $(FLAGS) $<
	@cp $(BUILD)/$@.pdf $(OUT)/Capitulo-$*.pdf
	@echo ">> Listo: $(OUT)/Capitulo-$*.pdf"

libro: libro.tex
	@mkdir -p $(BUILD) $(OUT)
	$(LATEXMK) $(FLAGS) libro.tex
	@cp $(BUILD)/libro.pdf $(OUT)/Libro-Completo.pdf
	@echo ">> Listo: $(OUT)/Libro-Completo.pdf"

# Recompilación continua: make watch-cap01 / make watch-libro
watch-%:
	@mkdir -p $(BUILD)
	$(LATEXMK) $(FLAGS) -pvc $*.tex

# Comprueba en los PDF que la fuente es Times New Roman y los tamaños
# son 11 pt (texto) y 12/14/16 pt (títulos).
verificar:
	@python3 tools/verificar-tipografia.py

clean:
	$(LATEXMK) -c -outdir=$(BUILD) libro.tex $(addsuffix .tex,$(CAPS)) 2>/dev/null || true
	@rm -f $(BUILD)/*.bbl $(BUILD)/*.run.xml $(BUILD)/*.bcf
	@echo ">> Intermedios eliminados (los PDF siguen en $(OUT)/)"

distclean:
	@rm -rf $(BUILD) $(OUT)
	@echo ">> build/ y entregas/ eliminados"
