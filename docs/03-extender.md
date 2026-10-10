# 3. Extender el proyecto

## Escribir un capítulo (ejemplo: el 4)

1. **Contenido.** Edita `capitulos/cap04/contenido.tex`. Solo el cuerpo, desde `\chapter{...}`. Nunca pongas `\documentclass`, `\begin{document}`, portada, índices ni `\printbibliography`.

2. **Título.** Vive en `config/metadatos.tex`, no en el capítulo:
   ```latex
   \newcommand{\TituloCapCuatro}{Nuevo título}
   ```

3. **Imágenes.** Guárdalas en `capitulos/cap04/figuras/` con prefijo `cap04-` y sin espacios. Se llaman solo por nombre:
   ```latex
   \Figura{cap04-esquema.png}{0.7}{Pie de figura.}{cap04:esquema}
   ```
   Argumentos: archivo, ancho (0.7 = 70 % del texto), pie, etiqueta (sin `fig:`, la macro lo agrega). Referencia: `Figura~\ref{fig:cap04:esquema}`.

4. **Bibliografía.** Agrega la fuente a `bib/referencias.bib` con clave `autor:tema:año` y cítala con `\cite{clave}`. La bibliografía es una sola para todo el libro.

5. **Etiquetas con prefijo de capítulo**, para que no choquen en el libro:

   | Tipo | Ejemplo |
   |---|---|
   | Figura | `fig:cap04:esquema` |
   | Tabla | `tab:cap04:tiempos` |
   | Sección | `sec:cap04:intro` |
   | Código | `lst:cap04:suma` |

6. **Compilar.** `make cap04`. Para recompilar al guardar: `make watch-cap04`. Comprueba también que `make libro` siga funcionando.

Tabla y código, si se necesitan:

```latex
\begin{table}[H]
    \centering
    \caption{Tiempos.}
    \label{tab:cap04:tiempos}
    \begin{tabular}{rr}
        \toprule
        \textbf{Hilos} & \textbf{Tiempo (s)} \\
        \midrule
        1 & 4.20 \\
        \bottomrule
    \end{tabular}
\end{table}

\lstinputlisting[style=C, caption={Suma.}, label=lst:cap04:suma]{capitulos/cap04/codigo/suma.c}
```

> La plantilla trae las etiquetas `fig:capjemplo` y `sec:cap04onclusiones` mal formadas (sin `:`). Corrígelas al usarlas.

## Crear una práctica nueva (ejemplo: la 4)

1. **Envoltorio.** Copia y adapta `prac03.tex`:
   ```bash
   cp practicas/prac03.tex practicas/prac04.tex
   ```
   Dentro de `prac04.tex` cambia `prac03` → `prac04`, `\TituloPracTres` → `\TituloPracCuatro` y `Práctica 3` → `Práctica 4`.

   **No quites** el bloque `\addto\captionsspanish{\renewcommand{\chaptername}{Práctica}}`: es lo que rotula «Práctica». Un `\renewcommand` suelto no funciona.

2. **Carpeta y contenido.**
   ```bash
   mkdir -p practicas/prac04/figuras practicas/prac04/codigo
   ```
   Crea `practicas/prac04/contenido.tex` (solo el cuerpo, empezando con `\chapter{\TituloPracCuatro}`). Figuras con prefijo `prac04-` y etiquetas como `fig:prac04:ejecucion`.

3. **Título.** Agrega en `config/metadatos.tex`:
   ```latex
   \newcommand{\TituloPracCuatro}{Título de la práctica 4}
   ```

4. **Ruta de figuras.** En `config/preambulo.tex`, dentro de `\graphicspath{...}`, agrega:
   ```latex
   {practicas/prac04/figuras/}%
   ```

5. **Compilar.** `make prac04`. No hay que tocar el `Makefile`: detecta solo cualquier `practicas/prac*.tex`.

### Si la práctica lleva código en C

Copia `practicas/prac03/codigo/Makefile` y ajusta el nombre del programa:

```makefile
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pthread   # -pthread solo si hay hilos
TARGET = programa

all: $(TARGET)
$(TARGET): programa.c
	$(CC) $(CFLAGS) -o $(TARGET) programa.c
run: $(TARGET)
	./$(TARGET)
clean:
	rm -f $(TARGET) a.out
.PHONY: all run clean
```

- Las líneas de comandos van con **TAB**, no con espacios.
- Debe compilar sin errores **ni advertencias**: `make clean && make && ./programa`.
- Las salidas que muestres en el reporte deben venir de una corrida real.
