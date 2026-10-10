# Proyecto Paralelo — Guía para Claude

Materia *Cómputo Paralelo* (IPN — ESCOM), todo en LaTeX (XeLaTeX). Dos
entregables que comparten preámbulo, portada y bibliografía pero se
compilan y entregan **por separado**:

1. **El libro**: 7 capítulos (`capitulos/capNN/contenido.tex`).
2. **Las prácticas**: entregas sueltas (`practicas/pracNN/`). **Nunca** se
   incluyen en `libro.tex` ni en `capitulos/`.

Equipo: Said Carbot Cruz Trejo, Magaly López Castro, Lino Ehecatl Romero
Rosales, Aarón Ugalde Téllez. Profesor: José Alfredo Jiménez Benítez.
Grupo 6BV1.

## Regla central: una sola fuente de verdad

- Capítulo N → editar **sólo** `capitulos/capNN/contenido.tex`.
- Práctica N → editar **sólo** `practicas/pracNN/contenido.tex`.
- `capNN.tex`, `libro.tex` y `practicas/pracNN.tex` son envoltorios
  (portada + índices + `\input` + referencias): no se tocan para cambiar
  contenido.
- `contenido.tex` lleva sólo el cuerpo (`\chapter{...}` en adelante):
  nunca preámbulo, `\begin{document}`, portada, índices ni
  `\printbibliography`.
- No mover contenido entre `capitulos/` y `practicas/` sin que el
  usuario lo pida.

## Estructura

```
libro.tex, cap01.tex … cap07.tex   # envoltorios del libro (raíz)
Makefile, .latexmkrc               # motor XeLaTeX, salida a build/
config/metadatos.tex               # institución, autores, \TituloCapXXX, \TituloPracXXX
config/preambulo.tex               # fuentes, tamaños, estilos de código, \graphicspath
config/portada.tex                 # \Portada{titulo}{subtitulo}, \Indices
bib/referencias.bib                # bibliografía ÚNICA (IEEE, biblatex+biber)
capitulos/capNN/{contenido.tex, figuras/}
practicas/pracNN.tex               # envoltorio (vive dentro de practicas/)
practicas/pracNN/{contenido.tex, figuras/, codigo o .c+Makefile, partes/}
entregas/                          # PDF finales (se versionan)
build/                             # intermedios (no se versionan)
tools/verificar-tipografia.py
```

## Compilar (siempre desde la raíz)

```bash
make capNN | make acumNN (02..07) | make libro | make all   # libro (all NO incluye prácticas)
make pracNN | make practicas
make verificar      # audita fuentes/tamaños del PDF ya generado
make clean | make distclean | make watch-capNN | make watch-pracNN
```

- **Siempre XeLaTeX**, nunca `pdflatex` (el Makefile/.latexmkrc ya lo fuerzan).
- Los PDF se nombran con `PREFIJO := 6BV1_Ugalde_Tellez_` (primera línea
  del `Makefile`). La materia pide `Grupo_ApellidoPaterno_ApellidoMaterno_Actividad.pdf`;
  quien suba el archivo renombra/ajusta el prefijo.
- `acumNN` = entrega acumulada capítulos 1…N (envoltorio generado al vuelo
  en `build/acumNN.tex`; no se edita).

## Requisito tipográfico (si tocas `preambulo.tex`)

| Elemento | Tamaño |
|---|---|
| Texto, pies, encabezados | 11 pt (`11bp`, no `pt`) |
| Capítulo / índices / referencias | 16 pt negritas |
| `\section` / `\subsection` / `\subsubsection` | 14 / 12 / 12 pt |

Excepciones válidas: código Courier New 10 pt, números de línea 8 pt,
superíndices ordinales (`2.ª`) ~8.14 pt. Tras tocar tipografía correr
`make verificar` y confirmar que no hay tamaños fuera de {11,12,14,16}
salvo esas excepciones. Nota: `make verificar` revisa **todos** los PDF
de `entregas/`; un fallo puede venir de otro documento (p. ej. el cap. 3
tiene un 6.05 pt conocido).

## Convenciones

- **Etiquetas con prefijo**: `fig:capNN:x`, `tab:pracNN:x`, `sec:pracNN:x`,
  `lst:pracNN:x` (evita choques al unificar).
- **Claves bib**: `autor:tema:año` (ej. `marquez:unix-programacion:2015`).
  Fuentes nuevas van en `bib/referencias.bib`; se citan con `\cite{}`.
  Un capítulo/práctica sin citas muestra "Referencias" vacía: es normal.
- **Figuras**: en `.../figuras/` con prefijo `capNN-` / `pracNN-`, sólo por
  nombre de archivo (`\graphicspath` ya las cubre; sin espacios en el nombre).
  Sin `\caption` no entra al índice de figuras.
- **Ayudas del preámbulo**: `\Figura{archivo}{ancho}{pie}{etiqueta}`,
  entornos `nota` y `definicion`, `lstlisting` con estilos `C` y `CUDA`,
  `algorithm2e`, `booktabs`, `siunitx`.
- Overfull hbox: un `\texttt{}` largo sin espacios no parte línea; separar
  en varios `\texttt` o reescribir la frase.
- Cambiar título de capítulo/práctica → sólo `config/metadatos.tex`.

## Metadatos (`config/metadatos.tex`)

`\Institucion, \Escuela, \Materia, \Profesor, \Grupo, \Semestre, \Lugar,
\TituloLibro, \SubtituloLibro, \Autores, \TituloCapUno…\TituloCapSiete,
\TituloPracUno, \TituloPracDos, \TituloPracTres…`

## Cómo crear una práctica nueva (ej. la 4)

1. Copiar `practicas/prac03.tex` → `prac04.tex`; cambiar `prac03`→`prac04`,
   `\TituloPracTres`→`\TituloPracCuatro`, `{Práctica 3}`→`{Práctica 4}`.
2. Crear `practicas/prac04/{contenido.tex, figuras/}` (+ código si aplica).
3. Agregar `\TituloPracCuatro` en `config/metadatos.tex`.
4. Agregar `{practicas/prac04/figuras/}%` al `\graphicspath` de
   `config/preambulo.tex`.
5. `make prac04` (el Makefile detecta `practicas/prac*.tex` solo).

⚠️ En el envoltorio, el rótulo "Práctica" va **dentro de**
`\addto\captionsspanish{\renewcommand{\chaptername}{Práctica}}`. Un
`\renewcommand` suelto no funciona (babel lo pisa sin avisar). No lo
"simplifiques".

## Prácticas con código en C (patrón de prac02 y prac03)

Reglas de negocio de la materia: compilar con `Makefile`; compilación sin
errores **ni advertencias**; mostrar compilación y ejecución en el
reporte; si no se entregan códigos y Makefile o no funciona, no se
califica; **sin sockets, sin gestores de BD**; validar toda entrada del
usuario; sólo lenguaje C; subir los `.c`/`.h` sin comprimir; conclusiones
**individuales** de al menos una cuartilla por persona; códigos en
anexos. Una práctica aporta 4% de la calificación.

Flujo:
1. Código y `Makefile` en `practicas/pracNN/` (o `pracNN/codigo/`), con
   `CFLAGS = -Wall -Wextra -std=c11` (`-pthread` si hay hilos) y objetivos
   `all`, `run`, `clean`.
2. Compilar y ejecutar de verdad (`make clean && make && ./programa`);
   comprobar que no hay warnings ni errores.
3. En `contenido.tex`, estructura usada: Resumen → Introducción → Marco
   teórico → Desarrollo del código (diseño, Makefile, validaciones) →
   Evidencias (salidas reales de `make` y de la ejecución en
   `lstlisting[numbers=none, language={}]`) → Conclusiones (una subsección
   por integrante, `\clearpage` antes de cada una) → Anexos
   (`\lstinputlisting[style=C]{practicas/pracNN/.../programa.c}` y el
   Makefile con `language=make`).
4. Las salidas del reporte deben venir de una corrida real; si cambia el
   código, regenerarlas.
5. Si el equipo deja textos en `practicas/pracNN/partes/*.tex`, fusionarlos
   en `contenido.tex` sin alterar lo escrito (convertir `\section*`→
   `\section`, claves `\cite` a las del `.bib`) y marcar la autoría en el
   comentario de cabecera.
6. Si falta la conclusión de algún integrante, se puede redactar sin
   preguntar: primera persona, basada en el código real, de al menos una
   cuartilla, con el estilo de las demás, y se avisa en una línea en la
   respuesta final. Dejar un comentario en `contenido.tex` indicando que
   la redactó Claude.
7. Capturas de compilación y ejecución: el usuario las deja sueltas en
   `practicas/pracNN/` (ej. `image.png`, `image copy.png`). Verlas para
   identificar cuál es cuál, moverlas a `figuras/` como
   `pracNN-compilacion.png` / `pracNN-ejecucion.png` e insertarlas con
   `\Figura{...}` junto al listado correspondiente de "Evidencias".

## Estado actual

### Libro

| Cap. | Título | Estado |
|---|---|---|
| 01 | "Introducción al cómputo paralelo" | Escrito (contenido: Serie de Fourier) |
| 02 | "Gráficas con Excel" | Escrito (23 pp.) |
| 03–07 | Ver `metadatos.tex` | Plantilla vacía |

⚠️ **Cap. 1:** el título dice "Introducción al cómputo paralelo" pero el
contenido es la Serie de Fourier. Decisión del equipo: se deja así; no
mover ni retitular sin que el usuario lo pida.

**Cap. 2 — decisiones del usuario que no se deben deshacer:**
- Cuatro partes simétricas (una subsección por integrante, 5 figuras cada
  una); sin `table` ni `lstlisting` en todo el capítulo.
- El texto **no menciona los `.xlsx`** (siguen en `capitulos/cap02/exeles/`
  sólo como respaldo interno).
- Pendientes menores: 2.5 casi sin referencias (claves útiles ya en el
  `.bib`: `kreyszig:matematicas-avanzadas:2011`,
  `oppenheim:senales-sistemas:1997`); la conclusión de Magaly es la más
  corta; su gráfica comparativa conserva el título por defecto.

### Prácticas

| Práctica | Título | Estado |
|---|---|---|
| 01 | Plataformas y herramientas para el cómputo paralelo | ✅ 56 pp. |
| 02 | Análisis y diseño de programas paralelos utilizando procesos | ✅ (`fork()`, archivos de texto) |
| 03 | Cómputo paralelo utilizando hilos | ✅ (`pthread`, un hilo por fila, apuntadores, `pthread_join`) |

**Prác. 1:** fusiona las cuatro entregas individuales (Aarón y Said en
macOS con equivalentes documentados; Magaly en VM sobre Windows; Lino en
partición física). Estructura acordada: Resumen e Introducción al inicio
(de equipo); Conclusiones al final, una subsección por integrante. Sobre el
ejercicio de usuarios: la redacción presenta el rechazo de `userdel` con
sesión abierta como comportamiento documentado del manual, **sin afirmar
que se observó** (ninguna captura lo muestra); no la cambies a primera
persona del pasado. La Tabla 1 pide `kill -l` pero se entregó `kill -1`/
`-9`. Hay dos pasos de la instalación en partición sin figura por decisión
del usuario.

**Prác. 3:** código en `practicas/prac03/codigo/` (`programa.c` +
`Makefile`). La parte `partes/main (3).tex` no traía nombre y se asignó a
Magaly (confirmar). La conclusión de Aarón la redactó Claude el
2026-10-06 (marcada en `contenido.tex`). Ya incluye capturas de compilación
y ejecución (`figuras/prac03-*.png`). Del resumen de Said sólo se usa el
último párrafo (los otros describen programas que no se entregan).

## Notas rápidas

- `entregas/*.pdf` se versionan; `build/` no.
- Si la estructura cambia (capítulo/práctica/comando nuevo), actualiza este
  archivo.
