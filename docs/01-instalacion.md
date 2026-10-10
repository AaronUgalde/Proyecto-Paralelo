# 1. Clonar e instalar

## Clonar

```bash
git clone https://github.com/AaronUgalde/Proyecto-Paralelo.git
cd Proyecto-Paralelo
```

La rama principal es `master`.

## Herramientas necesarias

Se necesita un motor LaTeX (XeLaTeX, `biber`, `latexmk`), `make`, `gcc` y `python3`.

### macOS

```bash
xcode-select --install                 # make, git, clang
brew install --cask mactex-no-gui      # TeX Live completo
brew install gcc                       # opcional
```

Reabre la terminal para que `xelatex` quede en el `PATH`. macOS ya trae Times New Roman y Courier New.

### Ubuntu / Debian / WSL2

```bash
sudo apt update
sudo apt install -y git make build-essential python3 \
    texlive-xetex texlive-latex-extra texlive-science texlive-lang-spanish \
    texlive-bibtex-extra texlive-fonts-extra tex-gyre latexmk biber
```

Si falta algún paquete de LaTeX: `sudo apt install texlive-full`.
Sin Times New Roman, el proyecto usa automáticamente TeX Gyre Termes (equivalente).

### Windows

Usar WSL2 con Ubuntu y seguir los pasos de Linux.

## Verificar

```bash
xelatex --version && biber --version && latexmk -v | head -2
make --version | head -1 && gcc --version | head -1
make help
```

## Reglas al compilar

1. Siempre desde la **raíz** del repositorio.
2. Siempre **XeLaTeX**, nunca `pdflatex` (el `Makefile` y `.latexmkrc` ya lo fuerzan).
