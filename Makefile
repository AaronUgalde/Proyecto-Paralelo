# =====================================================================
#  Makefile — Proyecto Paralelo (libro de 7 capítulos + prácticas)
#
#  Uso:
#     make cap01        Compila la entrega individual del capítulo 1
#     make cap03        ... del capítulo 3, etc.
#     make acum03       Entrega ACUMULATIVA: capítulos 1 a 3 en un PDF
#     make libro        Compila el libro completo unificado
#     make capitulos    Compila las 7 entregas individuales
#     make all          Compila las 7 entregas + el libro
#     make prac01       Compila la entrega de la práctica 1
#     make practicas    Compila TODAS las prácticas existentes
#     make clean        Borra archivos intermedios (deja los PDF)
#     make distclean    Borra build/ y entregas/ por completo
#     make watch-cap01  Recompila el capítulo 1 al guardar cambios
#     make watch-prac01 Recompila la práctica 1 al guardar cambios
#     make verificar    Comprueba fuente y tamaños de letra en los PDF
#
#  Las prácticas (practicas/pracNN.tex) son ENTREGAS INDEPENDIENTES:
#  nunca se compilan como parte de "make libro" ni de "make all", que
#  siguen siendo sólo el libro y sus 7 capítulos.
#
#  Los PDF listos para entregar quedan en entregas/, ya nombrados con
#  el prefijo que pide la materia (ver PREFIJO más abajo), por ejemplo
#  entregas/6BV1_Ugalde_Tellez_Practica-01.pdf
# =====================================================================

BUILD    := build
OUT      := entregas
# Prefijo con el que la materia pide nombrar los archivos que se suben.
# Si compila otro integrante del equipo, sólo cambia esta línea.
PREFIJO  := 6BV1_Ugalde_Tellez_
LATEXMK  := latexmk
# -xelatex: obligatorio para usar la fuente real Times New Roman del sistema.
FLAGS    := -xelatex -interaction=nonstopmode -file-line-error -halt-on-error -outdir=$(BUILD)

CAPS     := cap01 cap02 cap03 cap04 cap05 cap06 cap07
# Detecta automáticamente las prácticas existentes en practicas/pracNN.tex,
# así que agregar una práctica nueva NO requiere tocar este Makefile.
PRACS    := $(basename $(notdir $(wildcard practicas/prac*.tex)))
# Entregas acumulativas (capítulos 1..N en un solo PDF). Empiezan en la 02
# porque la entrega acumulada del capítulo 1 es simplemente "make cap01".
ACUMS    := acum02 acum03 acum04 acum05 acum06 acum07

.PHONY: all capitulos libro practicas clean distclean help verificar $(CAPS) $(PRACS) $(ACUMS)

help:
	@echo "Objetivos disponibles:"
	@echo "  make cap01 .. cap07   Entrega individual de un capítulo"
	@echo "  make acum02 .. acum07 Entrega acumulativa (capítulos 1 a N)"
	@echo "  make capitulos        Las 7 entregas individuales"
	@echo "  make libro            Libro completo unificado"
	@echo "  make all              Los 7 capítulos + el libro"
	@echo "  make prac01 ...       Entrega individual de una práctica"
	@echo "  make practicas        Todas las prácticas existentes"
	@echo "  make clean            Limpia intermedios"
	@echo "  make distclean        Limpia build/ y entregas/"
	@echo "  make watch-cap01      Recompila un capítulo al guardar"
	@echo "  make watch-prac01     Recompila una práctica al guardar"
	@echo "  make verificar        Revisa fuente y tamaños de letra"

all: capitulos libro

capitulos: $(CAPS)

practicas: $(PRACS)

$(CAPS): cap%: cap%.tex
	@mkdir -p $(BUILD) $(OUT)
	$(LATEXMK) $(FLAGS) $<
	@cp $(BUILD)/$@.pdf $(OUT)/$(PREFIJO)Capitulo-$*.pdf
	@echo ">> Listo: $(OUT)/$(PREFIJO)Capitulo-$*.pdf"

# Las prácticas viven en practicas/pracNN.tex (entregas independientes,
# nunca se incluyen en libro.tex ni cuentan para "make all").
$(PRACS): prac%: practicas/prac%.tex
	@mkdir -p $(BUILD) $(OUT)
	$(LATEXMK) $(FLAGS) $<
	@cp $(BUILD)/$@.pdf $(OUT)/$(PREFIJO)Practica-$*.pdf
	@echo ">> Listo: $(OUT)/$(PREFIJO)Practica-$*.pdf"

# Entrega acumulativa: "make acum03" genera un PDF con los capítulos 1 a 3
# (portada, índices, los capítulos seguidos y una sola lista de referencias).
# El envoltorio se escribe solo en build/; no hace falta un .tex por entrega.
$(ACUMS): acum%:
	@mkdir -p $(BUILD) $(OUT)
	@{ printf '%s\n' \
	     '% Generado por "make acum$*". No lo edites: se reescribe cada vez.' \
	     '\documentclass[12pt, letterpaper, oneside, openany]{book}' \
	     '\input{config/metadatos}' \
	     '\input{config/preambulo}' \
	     '\input{config/portada}' \
	     '\begin{document}' \
	     '\frontmatter'; \
	   printf '\\Portada{\\TituloLibro}{Entrega acumulativa — Capítulos 1 a %d}\n' $*; \
	   printf '%s\n' '\Indices' '\mainmatter'; \
	   for n in $$(seq 1 $*); do printf '\\input{capitulos/cap%02d/contenido}\n' $$n; done; \
	   printf '%s\n' '\Referencias' '\end{document}'; \
	 } > $(BUILD)/acum$*.tex
	$(LATEXMK) $(FLAGS) $(BUILD)/acum$*.tex
	@cp $(BUILD)/acum$*.pdf $(OUT)/$(PREFIJO)Entrega-Capitulos-01-a-$*.pdf
	@echo ">> Listo: $(OUT)/$(PREFIJO)Entrega-Capitulos-01-a-$*.pdf"

libro: libro.tex
	@mkdir -p $(BUILD) $(OUT)
	$(LATEXMK) $(FLAGS) libro.tex
	@cp $(BUILD)/libro.pdf $(OUT)/$(PREFIJO)Libro-Completo.pdf
	@echo ">> Listo: $(OUT)/$(PREFIJO)Libro-Completo.pdf"

# Recompilación continua: make watch-cap01 / make watch-libro
watch-%:
	@mkdir -p $(BUILD)
	$(LATEXMK) $(FLAGS) -pvc $*.tex

# Recompilación continua para prácticas: make watch-prac01
watch-prac%:
	@mkdir -p $(BUILD)
	$(LATEXMK) $(FLAGS) -pvc practicas/prac$*.tex

# Comprueba en los PDF que la fuente es Times New Roman y los tamaños
# son 11 pt (texto) y 12/14/16 pt (títulos).
verificar:
	@python3 tools/verificar-tipografia.py

clean:
	$(LATEXMK) -c -outdir=$(BUILD) libro.tex $(addsuffix .tex,$(CAPS)) $(wildcard practicas/prac*.tex) 2>/dev/null || true
	@rm -f $(BUILD)/*.bbl $(BUILD)/*.run.xml $(BUILD)/*.bcf
	@echo ">> Intermedios eliminados (los PDF siguen en $(OUT)/)"

distclean:
	@rm -rf $(BUILD) $(OUT)
	@echo ">> build/ y entregas/ eliminados"
