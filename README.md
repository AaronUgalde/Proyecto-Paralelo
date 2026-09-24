# Proyecto Paralelo — Libro de Cómputo Paralelo (IPN / ESCOM)

Libro de **7 capítulos** para la materia de Cómputo Paralelo. Cada capítulo se
entrega por separado a lo largo del semestre como un PDF autocontenido, y al
final los 7 se compilan en un solo libro con material preliminar unificado.

**Una sola fuente de verdad:** el texto de cada capítulo vive en un único archivo
(`capitulos/capNN/contenido.tex`) que consumen tanto la entrega individual como
el libro final. No hay que copiar ni sincronizar nada.

---

## Requisitos de entrega y dónde se resuelven

| Requisito | Entrega individual (`capNN.tex`) | Libro final (`libro.tex`) |
|---|---|---|
| Portada | `\Portada` con el título del capítulo | `\Portada` con el título del libro (una sola) |
| Índice de contenido | `\Indices` → `\tableofcontents` | `\Indices` (uno solo, con los 7 capítulos) |
| Índice de figuras | `\Indices` → `\listoffigures` | `\Indices` (unificado) |
| Índice de tablas | `\Indices` → `\listoftables` | `\Indices` (unificado) |
| Referencias IEEE | `\Referencias` (sólo las citadas en ese capítulo) | `\Referencias` (todas, lista unificada) |

El estilo IEEE lo aporta `biblatex` con `style=ieee` + `biber`: las citas salen
como `[1]`, `[2]`–`[4]` y la lista final sigue el formato IEEE.

---

## Tipografía

Requisito: Times New Roman, 11 pt el texto y 12 / 14 / 16 pt los títulos.

| Elemento | Tamaño | Cómo se fija |
|---|---|---|
| Texto corrido, pies de figura/tabla, encabezados, folio | **11 pt** | `\usepackage[fontsize=11bp]{fontsize}` |
| Título de capítulo (y de los índices y de las referencias) | **16 pt** negritas | `titlesec` + `\TamCapitulo` |
| Sección `\section` | **14 pt** negritas | `titlesec` + `\TamSeccion` |
| Subsección `\subsection` | **12 pt** negritas | `titlesec` + `\TamSubseccion` |
| Sub-subsección `\subsubsection` | **12 pt** negritas cursiva | `titlesec` + `\TamSubseccion` |

Tres detalles que hacen que esto se cumpla *de verdad* y no solo en el código:

1. **Motor XeLaTeX, no pdflatex.** Es el único que puede usar el archivo real
   `Times New Roman.ttf` del sistema. En el PDF quedan embebidas
   `TimesNewRomanPSMT`, `-BoldMT` y `-ItalicMT`. Si la máquina no tuviera la
   fuente, `config/preambulo.tex` cae automáticamente en **TeX Gyre Termes**,
   clon métricamente idéntico, y el documento sigue compilando igual.
2. **Los tamaños van en `bp`, no en `pt`.** El punto de TeX mide 1/72.27 de
   pulgada y el de Word (y el que reportan los visores de PDF) mide 1/72. Pedir
   `11pt` en LaTeX produce 10.96 pt medidos en el PDF; pedir `11bp` produce
   11.00 exactos, que es lo que se va a revisar.
3. **Sin versalitas falsas.** Times New Roman no trae versalitas reales y
   babel-español compone los números romanos en versalitas; XeTeX las simularía
   encogiendo la letra a ~8 pt. La opción `es-lcroman` lo evita.

Excepciones deliberadas, fuera del texto redactado:

- Bloques de código en Courier New a **10 pt** (a 11 pt las líneas largas no
  caben en el ancho de página) con números de línea a 8 pt.
- Superíndices ordinales de la bibliografía en español (`2.ª ed.`), que por
  definición se componen más pequeños.

Todo esto es auditable sobre el PDF ya generado:

```bash
make verificar
```

Lee los operadores del PDF y reporta qué fuentes quedaron embebidas y con qué
tamaños se compuso cada cosa, marcando lo que se salga de la especificación.

---

## Cómo compilar

Siempre **desde la raíz del repositorio**. El motor es **XeLaTeX** (`make` ya lo
usa; si compilas a mano o desde el editor, el `.latexmkrc` también lo impone).

```bash
make cap01          # entrega individual del capítulo 1
```

```bash
make libro          # libro completo unificado
```

```bash
make all            # los 7 capítulos + el libro
```

Otros objetivos: `make capitulos`, `make verificar`, `make clean`,
`make distclean`, `make watch-cap01` (recompila al guardar), `make help`.

Los PDF listos quedan en **`entregas/`**: `Capitulo-01.pdf` … `Capitulo-07.pdf`
y `Libro-Completo.pdf`. Los intermedios van a `build/` (ignorado por git).

Sin `make` también funciona: `latexmk cap01.tex` (el `.latexmkrc` ya fija
XeLaTeX y deja la salida en `build/`).

---

## Estructura

```
.
├── libro.tex                 # libro completo (entrega final)
├── cap01.tex … cap07.tex     # envoltorios de cada entrega individual
├── Makefile
├── config/
│   ├── metadatos.tex         # ← autores, profesor, materia, títulos de capítulo
│   ├── preambulo.tex         # fuentes y tamaños, estilos, bibliografía IEEE
│   └── portada.tex           # macros \Portada e \Indices
├── tools/
│   └── verificar-tipografia.py
├── bib/
│   └── referencias.bib       # bibliografía ÚNICA de todo el libro
├── capitulos/
│   ├── cap01/
│   │   ├── contenido.tex     # ← aquí se escribe el capítulo 1
│   │   └── figuras/          # imágenes del capítulo 1 (prefijo cap01-)
│   └── … cap07/
├── img/                      # logos institucionales y figuras compartidas
├── practicas/                # prácticas sueltas — NO forman parte del libro
│   ├── prac01.tex            # ver sección "Prácticas" más abajo
│   └── prac01/{contenido.tex, figuras/}
├── entregas/                 # PDF finales
└── build/                    # intermedios (git-ignored)
```

---

## Flujo de trabajo por capítulo

1. Si cambia el título, edítalo en `config/metadatos.tex` (`\TituloCapDos`, …).
   Se actualiza solo en la portada, el índice y el libro.
2. Escribe el capítulo en `capitulos/capNN/contenido.tex`.
3. Guarda las imágenes en `capitulos/capNN/figuras/` con el prefijo `capNN-`
   y llámalas **sólo por su nombre**: `\includegraphics{cap03-esquema.png}`.
   El `\graphicspath` de `config/preambulo.tex` ya cubre las siete carpetas, así
   que la misma línea funciona en la entrega suelta y en el libro.
4. Agrega las fuentes nuevas a `bib/referencias.bib` y cítalas con `\cite{clave}`.
5. `make capNN` y revisa `entregas/Capitulo-NN.pdf`.

### Convenciones

- **Etiquetas** con prefijo de capítulo para que no choquen al unificar el libro:
  `fig:cap03:speedup`, `tab:cap03:tiempos`, `sec:cap03:metodologia`,
  `lst:cap03:kernel`.
- **Claves bib**: `autor:tema:año` (p. ej. `flynn:taxonomia:1972`).
- Toda figura o tabla que lleve `\caption` entra automáticamente en su índice;
  si no lleva `\caption`, no aparece.
- Los capítulos individuales conservan su numeración del libro (el capítulo 5
  suelto se numera «5», y sus figuras «5.1», «5.2», …) gracias al
  `\setcounter{chapter}` de `capNN.tex`.

### Ayudas disponibles en el preámbulo

- `\Figura{archivo}{ancho}{pie}{etiqueta}` — figura centrada en un solo comando.
- Entornos `nota` y `definicion` — cajas de color para resaltar ideas.
- `lstlisting` con estilos `C` y `CUDA` para código de OpenMP, MPI y CUDA.
- `algorithm2e` para pseudocódigo, `booktabs` para tablas, `siunitx` para unidades.

---

## Prácticas (entregas independientes, fuera del libro)

Además del libro de 7 capítulos, la materia pide **prácticas** sueltas
durante el semestre. Usan exactamente la misma estructura de entrega
(portada, índices, referencias IEEE) pero **nunca se incluyen en
`libro.tex`**: son PDF completamente independientes.

```
practicas/
├── prac01.tex             # envoltorio de la práctica 1 (portada+índices+referencias)
├── prac01/
│   ├── contenido.tex      # ← aquí se escribe la práctica 1
│   └── figuras/           # imágenes de la práctica 1 (prefijo prac01-)
└── prac02.tex, prac02/...  # se agregan igual conforme se asignen
```

Compilación:

```bash
make prac01        # entrega de la práctica 1 → entregas/Practica-01.pdf
make practicas      # todas las prácticas existentes
```

Para agregar una práctica nueva (por ejemplo la 2):

1. Crea `practicas/prac02/contenido.tex` y `practicas/prac02/figuras/`
   copiando la estructura de `practicas/prac01/`.
2. Crea `practicas/prac02.tex` copiando `practicas/prac01.tex` y
   cambiando `\TituloPracUno` → `\TituloPracDos` y las rutas `prac01` → `prac02`.
3. Agrega `\TituloPracDos` en `config/metadatos.tex`.
4. Agrega `{practicas/prac02/figuras/}` al `\graphicspath` en
   `config/preambulo.tex`.
5. No hay que tocar el `Makefile`: `make prac02` funciona solo porque
   detecta automáticamente cualquier `practicas/pracNN.tex` que exista.

Las prácticas comparten la misma bibliografía (`bib/referencias.bib`)
y las mismas ayudas del preámbulo (`\Figura`, `nota`, `definicion`,
estilos de código, etc.) que los capítulos del libro.

## Notas

- Un capítulo que todavía no cita nada mostrará la sección **Referencias** vacía.
  Es esperado: se llena en cuanto aparezca el primer `\cite`.
- La bibliografía es compartida a propósito. `biber` sólo imprime las entradas
  realmente citadas en el documento que se está compilando, así que cada entrega
  individual sale con su propia lista y el libro con la lista completa.
- Toolchain verificada: TeX Live 2026 (`xelatex` + `biber` + `latexmk`).
  **No compiles con `pdflatex`**: fallaría al no encontrar `fontspec`/Times New
  Roman. Si usas VS Code + LaTeX Workshop, configura la receta a `latexmk`
  (tomará XeLaTeX del `.latexmkrc`).
