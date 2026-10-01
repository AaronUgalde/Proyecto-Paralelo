#!/usr/bin/env python3
"""Verificación serial de la Serie de Fourier de f(x) = -5/6 x^2 + x/3 - 2/7.

Lee el CSV que produce fourier_aaron.c y lo compara con:
  * f(x)   : la función original (Capítulo 1),
  * F10(x) : la suma de 10 armónicos de la hoja de cálculo (Capítulo 2),
  * F100(x): la suma de 100 armónicos evaluada en forma serial (sin procesos).
Uso:  python3 verificacion_serie.py fourier_aaron_64puntos.csv
"""
import csv
import math
import sys

PI = math.pi


def f(x):
    return -5 / 6 * x * x + x / 3 - 2 / 7


def serie(x, armonicos):
    """a0/2 + suma de los primeros 'armonicos' términos (mismas fórmulas que el C)."""
    s = -5 * PI ** 2 / 18 - 2 / 7
    for n in range(1, armonicos + 1):
        signo = 1.0 if (n + 1) % 2 == 0 else -1.0
        s += 10 * signo / (3 * n * n) * math.cos(n * x)
        s += 2 * signo / (3 * n) * math.sin(n * x)
    return s


def mse(a, b, idx):
    return sum((a[i] - b[i]) ** 2 for i in idx) / len(idx)


ruta = sys.argv[1] if len(sys.argv) > 1 else "fourier_aaron_64puntos.csv"
filas = list(csv.DictReader(open(ruta)))
xs = [float(r["x"]) for r in filas]
f_c = [float(r["f_fourier"]) for r in filas]          # resultado del programa en C
f_ref = [f(x) for x in xs]
f_10 = [serie(x, 10) for x in xs]
f_100 = [serie(x, 100) for x in xs]

todos = list(range(len(xs)))
interior = [i for i in todos if abs(xs[i]) <= 2.5]      # lejos de la discontinuidad
dentro = [i for i in todos if abs(xs[i]) <= PI]         # nodos dentro de [-pi, pi]

print("C vs serie serial (100 armónicos): MSE = %.3e, |dif| máx = %.3e"
      % (mse(f_c, f_100, todos), max(abs(a - b) for a, b in zip(f_c, f_100))))
for nombre, serie_k in (("F10  (Excel)", f_10), ("F100 (C)    ", f_c)):
    print("%s vs f: MSE |x|<=2.5 = %.3e | MSE |x|<=pi = %.3e | MSE 64 nodos = %.3e"
          % (nombre, mse(serie_k, f_ref, interior), mse(serie_k, f_ref, dentro),
             mse(serie_k, f_ref, todos)))

print("\n  x        f(x)       F10        F100(C)")
for k in (0, 1, 2, 10, 21, 31, 32, 41, 51, 61, 62, 63):
    print("%8.4f %10.6f %10.6f %10.6f" % (xs[k], f_ref[k], f_10[k], f_c[k]))
