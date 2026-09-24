# Proyecto Paralelo — Guía de contexto para Claude

> Este archivo existe para que cualquier sesión de Claude entienda el
> proyecto de inmediato, sin tener que releer todos los `.tex`. Está
> actualizado al 2026-09-09 (se agregó el sistema de prácticas). Si la
> estructura cambia (nuevo capítulo, nueva práctica, nuevo comando,
> etc.), actualiza este archivo también.

## Qué es esto

Dos entregables para la materia *Cómputo Paralelo* (IPN — ESCOM),
ambos en LaTeX, que comparten preámbulo, portada y bibliografía pero
**se compilan y entregan por separado**:

1. **El libro**: 7 capítulos. Cada uno se entrega por separado durante
   el semestre como PDF autocontenido (portada + índices + capítulo +
   referencias IEEE propias), y al final los 7 se compilan juntos en
   un solo libro con portada e índices unificados. Ver "El libro" abajo.
2. **Las prácticas**: entregas sueltas que la materia pide aparte del
   libro. Usan la misma estructura de entrega (portada, índices,
   referencias IEEE) pero **nunca se incluyen en `libro.tex`** — son
   PDF completamente independientes. Ver "Las prácticas" abajo.

Equipo (4 integrantes): Said Carbot Cruz Trejo, Magaly López Castro,
Lino Ehecatl Romero Rosales, Aarón Ugalde Téllez. Profesor: José
Alfredo Jiménez Benítez.

## El libro: una sola fuente de verdad

El texto de cada capítulo vive en **un único archivo**:
`capitulos/capNN/contenido.tex`. Ese mismo archivo lo consumen, vía
`\input`, dos documentos distintos:

- `capNN.tex` (raíz) → entrega individual de ese capítulo.
- `libro.tex` (raíz) → libro completo con los 7 capítulos.

**Nunca hay que copiar ni duplicar contenido.** Si vas a "escribir el
capítulo N" o "cambiar algo del capítulo N", el único archivo que se
edita es `capitulos/capNN/contenido.tex`. No toques `capNN.tex` ni
`libro.tex` para eso: son sólo envoltorios (portada + índices +
`\input` + referencias).

## Las prácticas: entregas independientes del libro

Viven en `practicas/`, con la misma lógica de "una sola fuente de
verdad" que los capítulos pero en su propio espacio, totalmente
separado del libro:

- `practicas/pracNN.tex` → envoltorio de esa práctica (portada +
  índices + `\input` + referencias). **Vive dentro de `practicas/`**,
  no en la raíz (a diferencia de `capNN.tex`), para no llenar la raíz
  a medida que se acumulen prácticas durante el semestre.
- `practicas/pracNN/contenido.tex` → el cuerpo real de esa práctica.
- `practicas/pracNN/figuras/` → sus imágenes, prefijo `pracNN-`.

**Ninguna práctica se referencia jamás desde `libro.tex` ni desde
`capitulos/`.** Son documentos `book` independientes que sólo comparten
`config/metadatos.tex`, `config/preambulo.tex`, `config/portada.tex` y
`bib/referencias.bib` con el libro (por eso las prácticas también
llevan portada IPN/ESCOM, mismos tamaños de letra e IEEE).

Diferencia de rótulo: dentro de cada `pracNN.tex` se hace
`\addto\captionsspanish{\renewcommand{\chaptername}{Práctica}}` antes
de `\begin{document}`, así que el encabezado sale como "Práctica 1."
en vez de "Capítulo 1." (esto es local a ese documento, no afecta a
los capítulos del libro).

⚠️ **Tiene que ir dentro de `\addto\captionsspanish{...}`.** Un
`\renewcommand{\chaptername}{Práctica}` suelto **no funciona**: babel
reasigna `\chaptername` al llegar a `\begin{document}` y lo pisa, y la
práctica sale rotulada "Capítulo 1." sin ningún error ni aviso en el
log. Se detectó y corrigió el 2026-09-11; si creas una práctica nueva
copiando el envoltorio, no lo "simplifiques" de vuelta.
Tampoco llevan `\setcounter{chapter}{...}`: cada práctica es su propio
documento y siempre es "la 1" dentro de sí misma, no necesita imitar
una numeración global como sí lo hacen los capítulos sueltos.

Actualmente sólo existe `practicas/prac01/` como plantilla vacía
("Contenido pendiente"), lista para recibir la primera práctica real.

### Cómo agregar una práctica nueva (ej. la 2)

1. Copia `practicas/prac01/` → `practicas/prac02/` (contenido.tex +
   figuras/).
2. Copia `practicas/prac01.tex` → `practicas/prac02.tex` y cambia
   `\TituloPracUno`→`\TituloPracDos`, `prac01`→`prac02` y
   `{Práctica 1}`→`{Práctica 2}` en el `\Portada{...}`.
3. Agrega `\TituloPracDos` en `config/metadatos.tex` (junto a
   `\TituloPracUno`).
4. Agrega `{practicas/prac02/figuras/}` al `\graphicspath` de
   `config/preambulo.tex`.
5. Ya se puede usar `make prac02` sin tocar el `Makefile`: los
   objetivos de prácticas se detectan solos vía
   `wildcard practicas/prac*.tex`.

## Estructura del repositorio

```
.
├── libro.tex                 # libro completo (entrega final)
├── cap01.tex … cap07.tex     # envoltorio de cada entrega individual
├── Makefile                  # make cap01 / make libro / make all / ...
├── .latexmkrc                # motor XeLaTeX, salida a build/
├── config/
│   ├── metadatos.tex         # institución, profesor, autores, TÍTULOS de capítulo
│   ├── preambulo.tex         # fuentes, tamaños, colores, estilos de código, biblatex
│   └── portada.tex           # macros \Portada{titulo}{subtitulo} e \Indices
├── bib/referencias.bib       # bibliografía ÚNICA de todo el libro (IEEE)
├── tools/verificar-tipografia.py  # audita fuente/tamaños sobre el PDF ya generado
├── capitulos/
│   ├── cap01/contenido.tex + figuras/   # ← contenido real, ver "Estado" abajo
│   ├── cap02/contenido.tex + figuras/   # plantilla vacía
│   ├── ...
│   └── cap07/contenido.tex + figuras/   # plantilla vacía
├── img/                       # logos institucionales (IPN, ESCOM)
├── practicas/                 # entregas independientes, NO forman parte del libro
│   ├── prac01.tex             # envoltorio de la práctica 1
│   └── prac01/contenido.tex + figuras/
├── entregas/                  # PDF finales (Capitulo-01.pdf, Practica-01.pdf, Libro-Completo.pdf)
└── build/                     # intermedios de compilación (ignorado por git)
```

## Cómo compilar (siempre desde la raíz del repo)

```bash
make cap01        # entrega individual del capítulo 1 → entregas/<PREFIJO>Capitulo-01.pdf
make acum03       # entrega ACUMULATIVA capítulos 1–3 → entregas/<PREFIJO>Entrega-Capitulos-01-a-03.pdf
make libro        # libro completo → entregas/<PREFIJO>Libro-Completo.pdf
make all          # los 7 capítulos + el libro (NO incluye prácticas)
make capitulos    # sólo los 7 capítulos sueltos
make prac01       # entrega de la práctica 1 → entregas/Practica-01.pdf
make practicas    # todas las prácticas existentes (detectadas solas)
make verificar    # audita fuente/tamaños del PDF ya generado (ver más abajo)
make clean        # borra intermedios, deja los PDF
make distclean    # borra build/ y entregas/ completos
make watch-cap01  # recompila automáticamente al guardar
make watch-prac01 # ídem, para una práctica
```

`make all` y `make libro` siguen siendo exclusivamente el libro; las
prácticas nunca se cuelan ahí ni se mezclan con `libro.tex`.

### Nombre de los archivos entregables (`PREFIJO`)

Todos los PDF de `entregas/` se nombran con el prefijo que pide la
materia, definido en **una sola línea** al inicio del `Makefile`:

```make
PREFIJO := 6BV1_Ugalde_Tellez_
```

Resultado: `6BV1_Ugalde_Tellez_Capitulo-01.pdf`,
`6BV1_Ugalde_Tellez_Practica-01.pdf`,
`6BV1_Ugalde_Tellez_Entrega-Capitulos-01-a-03.pdf`,
`6BV1_Ugalde_Tellez_Libro-Completo.pdf`.

Si compila otro integrante del equipo para subirlo a su cuenta, lo
único que cambia es esa línea (`grupo_apellidos_`). No hay nombres de
archivo escritos a mano en ninguna otra parte del `Makefile`.

### Entregas acumulativas (`make acumNN`)

La materia pide las entregas **acumuladas**: el capítulo 2 se entrega
junto con el 1, el 3 junto con 1–2, y así. `make acumNN` (de `acum02`
a `acum07`) produce ese PDF: portada, índices unificados, los
capítulos 1…N seguidos y **una sola** lista de referencias con lo
citado en todos ellos.

No hay un `.tex` por entrega acumulativa: el objetivo escribe el
envoltorio al vuelo en `build/acumNN.tex` (por eso ese archivo no se
edita, se reescribe en cada compilación) y lo compila. Los capítulos
se numeran solos 1…N porque van seguidos en un mismo documento; no
lleva `\setcounter{chapter}` como sí lo llevan los `capNN.tex`
sueltos. Salida: `entregas/Entrega-Capitulos-01-a-NN.pdf`.

Empieza en `acum02` a propósito: la entrega acumulada del capítulo 1
es simplemente `make cap01`. Y `make acum07` no es lo mismo que
`make libro` — el libro usa su propia portada (`\TituloLibro` con
`\SubtituloLibro`) y es el entregable final del semestre.

**Motor obligatorio: XeLaTeX, nunca pdflatex.** Es lo único que puede
usar la fuente real *Times New Roman* del sistema (macOS/Windows la
traen). Si no existe en la máquina, `config/preambulo.tex` cae solo en
*TeX Gyre Termes* (clon métrico libre) y compila igual. El `Makefile`
y el `.latexmkrc` ya fuerzan `-xelatex`; no hace falta pasar flags a
mano, incluso compilando con `latexmk cap01.tex` directo.

## Requisito tipográfico y cómo se cumple (importante si tocas `preambulo.tex`)

| Elemento | Tamaño | Mecanismo |
|---|---|---|
| Texto corrido, pies, encabezados, folio | **11 pt** | `\usepackage[fontsize=11bp]{fontsize}` |
| Título de capítulo / índices / referencias | **16 pt** negritas | `titlesec` + `\TamCapitulo` |
| `\section` | **14 pt** negritas | `\TamSeccion` |
| `\subsection` | **12 pt** negritas | `\TamSubseccion` |
| `\subsubsection` | **12 pt** negritas cursiva | `\TamSubseccion` + itshape |

Excepciones deliberadas (no son errores): código en Courier New a 10 pt
(a 11 pt no cabe en el ancho de página), números de línea de código a
8 pt, superíndices ordinales de bibliografía en español (`2.ª ed.`).

Detalles no obvios que importan si algo se ve "casi bien pero no":

- Los tamaños se declaran en **`bp`** (1/72 pulgada, el punto de PDF/
  Word), no en `pt` (1/72.27 pulgada, el punto nativo de TeX). Pedir
  `11pt` da 10.96 pt reales en el PDF; `11bp` da 11.00 exactos.
- `es-lcroman` en las opciones de `babel` evita que los números
  romanos del índice se compongan en versalitas falsas (Times New
  Roman no trae versalitas reales; XeTeX las simularía encogiendo la
  letra a ~8 pt, rompiendo el tamaño).
- `make verificar` (o `python3 tools/verificar-tipografia.py`) lee los
  operadores reales del PDF ya compilado —no el código fuente— y
  reporta qué fuentes y tamaños quedaron embebidos. Úsalo después de
  tocar cualquier cosa de tipografía para confirmar que el requisito
  se sigue cumpliendo en el PDF real, no sólo en el `.tex`.

## Bibliografía (IEEE, compartida, biblatex + biber)

Una sola base: `bib/referencias.bib`. Se cita con `\cite{clave}` desde
cualquier capítulo. `biber` sólo imprime en cada PDF las entradas
realmente citadas en *ese* documento, así que cada entrega individual
sale con su propia lista de referencias y el libro sale con la lista
completa — sin mantener archivos separados. Convención de claves:
`autor:tema:año` (ej. `flynn:taxonomia:1972`, `fourier:teoria-calor:1822`).

Un capítulo que aún no cita nada mostrará su sección "Referencias"
vacía; es esperado, no un error.

## Metadatos editables (`config/metadatos.tex`)

Si piden cambiar institución, profesor, autores, grupo, semestre o
**título de un capítulo**, este es el único archivo a tocar; se
propaga solo a portada, encabezados, índice y libro:

```latex
\Institucion, \Escuela, \Materia, \Profesor, \Grupo, \Semestre, \Lugar
\TituloLibro, \SubtituloLibro
\Autores                 % una línea por integrante
\TituloCapUno … \TituloCapSiete   % título de cada capítulo
```

## Estado actual de los capítulos (2026-09-09)

| Cap. | `\TituloCapXXX` en metadatos.tex | Contenido real | Estado |
|---|---|---|---|
| 01 | "Introducción al cómputo paralelo" | **La Serie de Fourier** (marco teórico + 4 soluciones a mano en PDF + conclusiones individuales) | ⚠️ Escrito completo, pero ver aviso abajo |
| 02 | "Gráficas con Excel" | Gráficas de la Serie de Fourier en hoja de cálculo (2.1 originales, 2.2 serie, 2.3 comparaciones, 2.4 conclusiones; una subsección por integrante) | ✅ Escrito (23 pp.), sin figuras pendientes |
| 03 | "Modelos de programación y métricas de desempeño" | Plantilla | Sin escribir |
| 04 | "Programación con memoria compartida: hilos y OpenMP" | Plantilla | Sin escribir |
| 05 | "Programación con memoria distribuida: MPI" | Plantilla | Sin escribir |
| 06 | "Cómputo en GPU: CUDA" | Plantilla | Sin escribir |
| 07 | "Diseño, análisis y caso de estudio integrador" | Plantilla | Sin escribir |

**⚠️ Aviso conocido, decisión tomada — no tocar sin pedirlo:** el
título en `config/metadatos.tex` sigue diciendo "Introducción al
cómputo paralelo", pero el contenido de `cap01/contenido.tex` es sobre
la Serie de Fourier. Ya se habló con el equipo (2026-09-09): **se deja
tal cual, no se mueve ni se retitula por ahora.** El sistema de
prácticas (sección de arriba) se creó aparte, sin tocar nada del
libro; no asumas que hay que mover ese contenido a una práctica salvo
que el usuario lo pida explícitamente.

Los capítulos 3–7 son la plantilla estándar que se genera para
capítulos nuevos (ver sección siguiente): `\chapter`, `\section`
Introducción/Desarrollo/Conclusiones, con ejemplos comentados de
figura y tabla listos para descomentar.

### Capítulo 2 (2026-09-18)

Mismo patrón que el cap. 1: fusiona las cuatro entregas individuales,
pero organizado por el guion de la práctica (2.1 gráficas originales,
2.2 gráficas con la serie, 2.3 comparaciones, 2.4 conclusiones) y
dentro de cada sección una subsección por integrante. Figuras
renombradas a `cap02-<integrante>-<qué es>.<ext>`. Los libros de Excel
de cada quien están en `capitulos/cap02/exeles/`.

Título: `\TituloCapDos` = "Gráficas con Excel", que es el nombre real
que la materia le da al capítulo (confirmado por el usuario el
2026-09-18). El rótulo "Capítulo 2." lo antepone el documento solo, no
va dentro del macro. A diferencia del cap. 1, aquí título y contenido
sí corresponden.

Los cuatro integrantes están completos contra la rúbrica: figuras y
archivo de Excel (2026-09-18).

Las cuatro partes son **simétricas a propósito**: cinco figuras cada
una (original, coeficientes o equivalente, matriz, suma y comparativa)
y ningún `table` ni `lstlisting` en todo el capítulo. La parte de
Aarón llegó a tener una tabla de coeficientes compuesta en LaTeX y un
listado con la fórmula de Excel; se quitaron el 2026-09-18 justamente
para que no desentonara. No los vuelvas a agregar sin pedirlo: los
valores ya se leen en su captura de la matriz, y las expresiones
cerradas de $a_0/2$, $a_n$ y $b_n$ quedan como ecuaciones numeradas.

⚠️ **El texto no debe mencionar los archivos `.xlsx` ni presentarlos
como adjuntos** (decisión del usuario, 2026-09-18). Se quitaron las
seis menciones que traían las entregas originales. Los libros de Excel
siguen en `capitulos/cap02/exeles/` como respaldo interno del equipo,
pero el documento no los cita y no hay que volver a agregarlos al
redactar. Ojo: la rúbrica oficial sí pide adjuntar el xlsx en 2.1 y
2.2; es una decisión consciente del usuario, no un descuido.

Los datos de la parte de Aarón se verificaron cruzándolos con sus
capturas: el $a_0/2 = -3.0272711$ de su hoja coincide con la fórmula
cerrada, y la suma de los diez armónicos de la fila en $x \approx -\pi$
da $-8.1931146$ contra el $-8.1931145$ de su columna $F(x)$. Si esos
números cambian, rehacer la comprobación.

Otros pendientes menores del capítulo 2, contra la rúbrica oficial:

- **2.5 sólo tiene una referencia.** Únicamente Magaly cita
  (`walkenbach:excel-bible:2015`). Said y Lino no citan nada.
  `kreyszig:matematicas-avanzadas:2011` y
  `oppenheim:senales-sistemas:1997` ya están en el `.bib` y encajan.
- Los xlsx de `exeles/` ya siguen el formato que pide la materia,
  `Grupo_ApellidoPaterno_ApellidoMaterno_Fase2.xlsx` (renombrados el
  2026-09-18): `6BV1_Cruz_Trejo_Fase2.xlsx`,
  `6BV1_López_Castro_Fase2.xlsx`, `6BV1_Romero_Rosales_Fase2.xlsx` y
  `6BV1_Ugalde_Téllez_Fase2.xlsx`. Están los cuatro integrantes.
  Recuerda que el documento no los menciona (ver aviso de arriba).
- La conclusión de Magaly es la más corta (~342 palabras); la rúbrica
  pide una página por integrante.
- La gráfica comparativa de Magaly conserva el título por defecto
  "Título del gráfico" de la hoja de cálculo.

Los coeficientes de la función de Aarón que aparecen en la tabla
`tab:cap02:aaron:coeficientes` ($a_0/2 = -5\pi^2/18 - 2/7$,
$a_n = 10(-1)^{n+1}/3n^2$, $b_n = 2(-1)^{n+1}/3n$) se verificaron
contra integración numérica el 2026-09-18. Si alguien los cambia, hay
que rehacer esa verificación: van en un documento calificado.

### Estado actual de las prácticas (2026-09-09)

| Práctica | `\TituloPracXXX` | Contenido real | Estado |
|---|---|---|---|
| 01 | "Plataformas y herramientas para el cómputo paralelo" | Resumen + Introducción + Marco teórico (13 temas) + instalación (VM y partición física) + 40 comandos probados + instrucciones finales + Conclusiones (una por integrante) | ✅ Escrita (56 pp.), sin figuras pendientes |

La práctica 1 fusiona en `practicas/prac01/contenido.tex` las cuatro
entregas individuales del equipo (Aarón y Said sobre macOS; Magaly y
Lino sobre Ubuntu). La autoría de cada bloque está marcada con
comentarios en el `.tex` y con subsecciones "Comandos asignados a …".

Detalle sobre sus figuras: conviven tres convenciones de nombre porque
las capturas llegaron así de cada integrante (`captura_*.png`,
`1.png`…`15.png`/`a.png`, `clear.png`/`pwd.png`/…). Las que traían
espacios en el nombre se renombraron a `lino-*.jpeg` y `magaly-*.png`
porque `\includegraphics` no las localiza con espacios. Cada figura va
envuelta en `\IfFileExists` con una caja de reemplazo, así que el
documento compila aunque falte una captura.

**Resumen, Introducción y Conclusiones agregados el 2026-09-24**, a
petición explícita del usuario tras confirmar que no existía ninguna
de las tres. Estructura acordada con el usuario (vía pregunta directa,
no asumida):

- `\section{Resumen}` y `\section{Introducción}` van **al inicio**,
  antes de "Marco teórico", y son de **equipo** (una sola voz, sin
  atribuir a nadie), igual que el resto del guion de la práctica que
  ya está unificado.
- `\section{Conclusiones}` va **al final**, después de "Instrucciones
  finales" y antes de `\Referencias`, con **una subsección por
  integrante** (`\subsection{Conclusión — Nombre}`), siguiendo el
  mismo patrón que ya usa el capítulo 1 del libro.

Si se reescribe cualquiera de las tres, respeta esa ubicación y esa
autoría (general vs. por integrante); no es una preferencia estética,
se decidió explícitamente con el usuario para no tener que rehacerlo.

**Sección 1.2 (instalación), reorganizada el 2026-09-24:** ya no son
dos subsecciones genéricas ("Entorno de trabajo en macOS" /
"Instalación de GNU/Linux (Ubuntu)") sino **cuatro, una por
integrante**, cada una con su propio párrafo introductorio explicando
por qué: Aarón y Said tienen equipos Apple Silicon con macOS y no
instalaron GNU/Linux (documentan su entorno de trabajo); Magaly instaló
GNU/Linux en una máquina virtual sobre Windows; Lino lo instaló en una
partición física del disco duro, también sobre Windows. Si se agrega
un quinto integrante o cambia quién instaló qué, hay que ajustar tanto
los títulos de subsección como el párrafo introductorio de la sección
1.2 que explica la división. También se agregaron párrafos de
introducción breves al inicio de "Marco teórico", de cada
"Comandos asignados a …" (recordando en qué sistema se ejecutaron) y
de "Instrucciones finales".

**Pendientes conocidos de la práctica 1** (2026-09-11), contra el
enunciado oficial:

1. Ejercicio de alta y baja de usuario (`sec:prac01:usuario`): **no
   hay figuras pendientes**, el documento está cerrado. Pero conviene
   saber que las capturas entregadas (`usuario-1-crear` …
   `usuario-5-borrar`) muestran `logout` **antes** del `userdel`, es
   decir documentan la baja normal, no el rechazo de `userdel` con la
   sesión abierta. Por eso el paso 4 presenta la secuencia de dos
   terminales y explica el rechazo como comportamiento documentado
   del comando (citando su manual), **sin afirmar en ningún punto que
   se haya observado ni remitir a una figura que lo pruebe**. No
   cambies esa redacción a primera persona del pasado ni le cuelgues
   una de las capturas existentes: ninguna muestra el rechazo.
   Si algún día se toma la captura real, va como
   `usuario-6-sesion-abierta.jpeg` en el paso 4.
2. La Tabla 1 pide `kill -l` (listar señales); lo entregado es
   `kill -1` (SIGHUP) y `kill -9`.
3. Dos pasos de la instalación en partición física (preparación de la
   USB y selección del dispositivo de arranque) quedaron **sólo como
   texto explicativo, sin figura**, porque esas capturas no existen.
   Fue decisión del usuario, no un descuido: no se pusieron cajas de
   reemplazo ahí.
4. Aarón y Said trabajaron sobre macOS con equivalentes documentados
   (`brew` por `apt`, TextEdit por gedit) en vez de instalar GNU/Linux.
   Es una decisión del equipo, no un descuido.

## Flujo de trabajo para escribir o cambiar un capítulo

1. Si cambia el título del capítulo, edítalo en `config/metadatos.tex`
   (no en `contenido.tex` ni en `capNN.tex`).
2. Escribe/edita el cuerpo en `capitulos/capNN/contenido.tex`. Este
   archivo **sólo** lleva el cuerpo: `\chapter{...}` en adelante, nunca
   preámbulo, `\begin{document}`, portada, índices ni
   `\printbibliography` (eso lo ponen `capNN.tex` y `libro.tex`).
3. Imágenes en `capitulos/capNN/figuras/` con prefijo `capNN-`,
   referenciadas sólo por nombre de archivo:
   `\includegraphics{cap03-esquema.png}` — el `\graphicspath` de
   `preambulo.tex` ya cubre las 7 carpetas, así que la misma línea
   sirve tanto en la entrega suelta como en el libro.
4. Fuentes nuevas van a `bib/referencias.bib` (formato IEEE) y se citan
   con `\cite{clave}`.
5. Compila con `make capNN` y revisa `entregas/Capitulo-NN.pdf`.

### Convenciones a seguir siempre

- **Etiquetas** con prefijo de capítulo para no chocar al unificar en
  el libro: `fig:capNN:algo`, `tab:capNN:algo`, `sec:capNN:algo`,
  `lst:capNN:algo`.
- **Claves bib**: `autor:tema:año`.
- Toda figura/tabla con `\caption` entra automáticamente a su índice;
  sin `\caption` no aparece.
- Los capítulos sueltos conservan la numeración del libro (cap. 5
  suelto se numera "5") gracias a `\setcounter{chapter}` en cada
  `capNN.tex` — no lo quites si tocas ese archivo.

### Ayudas ya definidas en `config/preambulo.tex`

- `\Figura{archivo}{ancho 0–1}{pie}{etiqueta}` — figura centrada en un
  solo comando (genera `\label{fig:etiqueta}`).
- Entornos `nota` y `definicion` — cajas de color para resaltar ideas.
- `lstlisting` con estilos `C` y `CUDA` (para OpenMP, MPI, CUDA), 10 pt.
- `algorithm2e` para pseudocódigo, `booktabs` para tablas, `siunitx`
  para unidades.
- `\includepdf[pages=-, scale=0.8]{...}` — usado en cap01 para incrustar
  las soluciones manuscritas escaneadas de la Serie de Fourier.

## Archivos que casi nunca hace falta tocar

- `.latexmkrc` y el `Makefile` — sólo si se agrega un capítulo 8+ o se
  cambia el flujo de compilación.
- `config/preambulo.tex` — sólo si cambia un requisito tipográfico o de
  estilo; cualquier cambio aquí debe validarse con `make verificar`.
- `config/portada.tex` — sólo si cambia el diseño de la portada o del
  bloque de índices.

## Notas rápidas para Claude al recibir pedidos típicos

- "Cambia el título del capítulo N" → editar sólo `\TituloCapXXX` en
  `config/metadatos.tex`.
- "Escribe/continúa el capítulo N" → editar sólo
  `capitulos/capNN/contenido.tex`, siguiendo la estructura de secciones
  ya presente (Introducción/Desarrollo/Conclusiones, o la que tenga
  cap01 si el capítulo sigue ese patrón de práctica con conclusiones
  individuales).
- "Escribe/continúa la práctica N" → editar sólo
  `practicas/pracNN/contenido.tex`. **Nunca** confundir esto con
  `capitulos/capNN/contenido.tex`: son sistemas paralelos e
  independientes (libro vs. entregas sueltas).
- "Crea/agrega una práctica nueva" → seguir los 5 pasos de "Cómo
  agregar una práctica nueva" más arriba; no requiere tocar el
  `Makefile`.
- "Compila y revisa" → `make capNN` / `make pracNN` (o `make libro`) y
  luego `make verificar`.
- Nunca sugerir `pdflatex`; siempre `xelatex` vía `make` o `latexmk`.
- `entregas/*.pdf` sí se versionan en git (no están en `.gitignore`);
  `build/` no se versiona.
- Antes de dar por bueno un cambio de tipografía, correr
  `make verificar` y confirmar que no aparecen tamaños fuera de
  `{11, 12, 14, 16}` pt (con las excepciones de código a 10/8 pt).
- No mover contenido entre `capitulos/` y `practicas/` sin que el
  usuario lo pida explícitamente (ver el aviso del capítulo 1 arriba).
