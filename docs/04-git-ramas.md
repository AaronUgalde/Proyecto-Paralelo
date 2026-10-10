# 4. Ramas y Pull Request

Cada tarea (un capítulo, una práctica) va en su propia rama, no en `master`. La rama se sube con `git push` y el Pull Request se crea en la página de GitHub.

Requisito: ser colaborador del repositorio. Al hacer `git push`, GitHub pide iniciar sesión (usa tu cuenta o un Personal Access Token, no contraseña).

## Pasos

```bash
# 1. Partir de master actualizado
git switch master
git pull origin master

# 2. Crear la rama
git switch -c cap04-openmp

# 3. Trabajar, compilar (make cap04) y añadir solo lo necesario
git add capitulos/cap04/ config/metadatos.tex bib/referencias.bib
git commit -m "Cap. 4: redacta introducción de OpenMP"

# 4. Subir la rama
git push -u origin cap04-openmp
```

## Crear el Pull Request en GitHub

1. Abre https://github.com/AaronUgalde/Proyecto-Paralelo
2. Haz clic en **Compare & pull request** (aviso amarillo).
   Si no aparece: pestaña **Pull requests** → **New pull request** → elige tu rama.
3. Verifica: **base** = `master`, **compare** = tu rama.
4. Escribe un título y haz clic en **Create pull request**.
5. Un compañero lo revisa y pulsa **Merge pull request**.

Después, en tu terminal:

```bash
git switch master
git pull origin master
```

## Qué NO subir

- PDF de `entregas/` (son binarios y generan conflictos; los sube una sola persona al cerrar la entrega).
- Ejecutables (`a.out`, `programa`, `programa1`…).
- Usa `git add` con rutas, no `git add .`.

## Si GitHub avisa de conflictos

```bash
git fetch origin
git merge origin/master
```

Abre el archivo marcado, deja el texto correcto y borra las líneas `<<<<<<<`, `=======` y `>>>>>>>`. Luego:

```bash
git add archivo.tex
git commit
git push
```

El Pull Request se actualiza solo.
