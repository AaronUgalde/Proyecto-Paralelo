# Tutorial del proyecto

Guía para clonar, compilar y extender **Proyecto-Paralelo** (libro y prácticas de Cómputo Paralelo, IPN-ESCOM, todo en LaTeX con XeLaTeX).

| Archivo | Qué explica |
|---|---|
| [01-instalacion.md](01-instalacion.md) | Clonar el repo e instalar las herramientas |
| [02-compilar.md](02-compilar.md) | Generar todos los productos (capítulos, libro, prácticas, código en C) |
| [03-extender.md](03-extender.md) | Escribir un capítulo o crear una práctica nueva |
| [04-git-ramas.md](04-git-ramas.md) | Trabajar en una rama y subirla con `git push` |

## Idea central

Cada capítulo o práctica tiene **un solo archivo de contenido**:

- Capítulo N → `capitulos/capNN/contenido.tex`
- Práctica N → `practicas/pracNN/contenido.tex`

Los archivos `capNN.tex`, `libro.tex` y `practicas/pracNN.tex` son envoltorios (portada, índices, referencias). **No se editan** para cambiar contenido.

## Productos

| Producto | Comando | Salida (en `entregas/`) |
|---|---|---|
| Capítulo N | `make capNN` | `6BV1_Ugalde_Tellez_Capitulo-NN.pdf` |
| Capítulos 1 a N | `make acumNN` | `...Entrega-Capitulos-01-a-NN.pdf` |
| Libro completo | `make libro` | `...Libro-Completo.pdf` |
| Práctica N | `make pracNN` | `...Practica-NN.pdf` |
