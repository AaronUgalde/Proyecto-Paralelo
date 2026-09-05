#!/usr/bin/env python3
"""
Verifica que un PDF generado cumple el requisito tipográfico del proyecto:
fuente Times New Roman y tamaños de 11 pt (texto) y 12 / 14 / 16 pt (títulos).

Uso:
    python3 tools/verificar-tipografia.py entregas/*.pdf
    make verificar

Lee directamente los operadores del PDF, así que comprueba lo que realmente
quedó en el archivo, no lo que dice el código fuente LaTeX.

Excepciones permitidas (declaradas a propósito, ver README):
  10 pt  bloques de código monoespaciado
   8 pt  números de línea del código
   ~8.1  superíndices ordinales (2.ª ed.) de la bibliografía en español
"""

import collections
import glob
import re
import sys
import zlib

TAMANOS_OK = {11.0, 12.0, 14.0, 16.0}
EXCEPCIONES = {10.0: "código monoespaciado", 8.0: "números de línea del código"}
FUENTES_OK = ("TimesNewRoman", "Termes", "CourierNew", "Cursor")


def flujos(datos):
    for m in re.finditer(rb"stream\r?\n", datos):
        ini = m.end()
        fin = datos.find(b"endstream", ini)
        try:
            yield zlib.decompress(datos[ini:fin])
        except zlib.error:
            continue


def analizar(ruta):
    datos = open(ruta, "rb").read()
    tamanos = collections.Counter()
    fuentes = set()
    for bruto in flujos(datos):
        for _, sz in re.findall(rb"/(F\d+)\s+([0-9.]+)\s+Tf", bruto):
            tamanos[round(float(sz), 2)] += 1
        for n in re.findall(rb"/BaseFont\s*/([A-Za-z0-9+\-#,.]+)", bruto):
            fuentes.add(n.decode().split("+")[-1])
    return tamanos, fuentes


def main(rutas):
    fallos = 0
    for ruta in rutas:
        tamanos, fuentes = analizar(ruta)
        print(f"\n=== {ruta} ===")

        malas = [f for f in fuentes if not any(ok in f for ok in FUENTES_OK)]
        print("  Fuentes: " + (", ".join(sorted(fuentes)) or "(ninguna)"))
        if malas:
            print("  !! Fuentes ajenas a Times/Courier: " + ", ".join(malas))
            fallos += 1

        print("  Tamaños:")
        for pt in sorted(tamanos, reverse=True):
            veces = tamanos[pt]
            if pt in TAMANOS_OK:
                marca = "ok"
            elif pt in EXCEPCIONES:
                marca = f"excepción: {EXCEPCIONES[pt]}"
            elif 7.5 <= pt <= 8.5:
                marca = "excepción: superíndice ordinal"
            else:
                marca = "!! FUERA DE ESPECIFICACIÓN"
                fallos += 1
            print(f"    {pt:6.2f} pt  x{veces:<4} {marca}")

    print()
    if fallos:
        print(f"RESULTADO: {fallos} problema(s) tipográfico(s).")
        return 1
    print("RESULTADO: todo conforme (Times New Roman; 11 pt texto, 12/14/16 pt títulos).")
    return 0


if __name__ == "__main__":
    args = sys.argv[1:] or sorted(glob.glob("entregas/*.pdf"))
    if not args:
        print("No hay PDF que revisar. Ejecuta `make all` primero.")
        sys.exit(1)
    sys.exit(main(args))
