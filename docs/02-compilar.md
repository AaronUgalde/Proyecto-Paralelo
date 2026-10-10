# 2. Compilar los productos

Todos los comandos se ejecutan desde la raíz. Los PDF quedan en `entregas/`; los intermedios, en `build/`.

## Libro

| Comando | Resultado |
|---|---|
| `make cap01` … `make cap07` | PDF de un capítulo |
| `make acum02` … `make acum07` | Capítulos 1 a N en un solo PDF |
| `make capitulos` | Los 7 capítulos |
| `make libro` | Libro completo (una portada, un índice, una bibliografía) |
| `make all` | 7 capítulos + libro (no incluye prácticas) |

Estado actual: los capítulos 1 a 3 están escritos; del 4 al 7 solo tienen la plantilla («Contenido pendiente»).

### Notas por capítulo

- **Cap. 1**: Serie de Fourier a mano. Las soluciones son PDF en `capitulos/cap01/figuras/`.
- **Cap. 2**: gráficas hechas en Excel (`capitulos/cap02/exeles/*.xlsx`). Las imágenes ya están exportadas en `figuras/`.
- **Cap. 3**: sus resultados salen de programas en C (siguiente sección).

## Prácticas

Son entregas independientes: nunca entran al libro.

```bash
make prac01      # también prac02, prac03
make practicas   # todas
```

| Práctica | Tema | Código |
|---|---|---|
| 1 | Plataformas y herramientas (Linux) | no tiene |
| 2 | Procesos con `fork()` | `practicas/prac02/` |
| 3 | Hilos con `pthread` | `practicas/prac03/codigo/` |

### Práctica 2

```bash
cd practicas/prac02
make
./programa1      # crea matriz.txt
./programa2      # 9 procesos hijos; resultados en resultados.txt
make clean
```

### Práctica 3

```bash
cd practicas/prac03/codigo
make run         # compila con -pthread y ejecuta
make clean
```

## Código en C del capítulo 3

Compilar sin `-std=c11` (se usan `M_PI` y System V IPC).

```bash
cd capitulos/cap03/codigo

gcc -Wall -Wextra -o fourier_aaron fourier_aaron.c -lm
./fourier_aaron                      # → fourier_aaron_64puntos.csv
python3 verificacion_serie.py fourier_aaron_64puntos.csv

gcc -Wall -Wextra -o fourier_f3_lino fourier_f3_lino.c -lm
./fourier_f3_lino                    # → fourier_f3_lino.csv

gcc -Wall -Wextra -o fourier_x5 fourier_x5.c -lm -pthread
./fourier_x5                         # → datos_said.csv
```

Si un programa se interrumpe y deja memoria compartida o semáforos huérfanos: `ipcs` para verlos, `ipcrm -m <id>` / `ipcrm -s <id>` para borrarlos.

## Verificar tipografía

```bash
make verificar
```

Comprueba que los PDF usen Times New Roman con tamaños 11 pt (texto) y 12/14/16 pt (títulos). Revisa **todos** los PDF de `entregas/`.

## Limpiar

```bash
make clean       # borra intermedios, conserva los PDF
make distclean   # borra build/ y entregas/
```

## Problemas comunes

| Síntoma | Solución |
|---|---|
| Error de `fontspec` | Se compiló con `pdflatex`; usar `make` |
| `File 'xxx.sty' not found` | Instalar el paquete (`tlmgr install xxx` o `texlive-full`) |
| Citas con `[?]` o bibliografía vacía | `make clean` y volver a compilar |
