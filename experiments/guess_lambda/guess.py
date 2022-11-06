import json
import re
from itertools import count
from math import ceil, log2

# Estou usando a versão dummy que usa threads em vez de processos
# porque já estou usando um processo pra executar o encoder, então
# isso ainda é efetivamente paralelo apesar do GIL
from multiprocessing.dummy import Pool
from pathlib import Path

import matplotlib.pyplot as plt

from jplm import JPLM


def cache_on_file(path):
    """
    Decorator para usar um arquivo como cache.
    """
    path = Path(path)
    path.parent.mkdir(parents=True, exist_ok=True)

    def decorator(func):
        def wrapper(arg):
            try:
                with open(path, "r") as file:
                    cache = json.load(file)
            except FileNotFoundError:
                cache = dict()

            hash_ = str(arg)

            if hash_ in cache:
                return cache[hash_]

            value = func(arg)
            cache[hash_] = value

            with open(path, "w") as file:
                json.dump(cache, file, indent=4)
            return value

        return wrapper

    return decorator


def lerp(start, end, t):
    return start * (1 - t) + end * t


def bisect(a, b, function, *, error=1e-6):
    """
    Encontra uma raíz de uma função no intervalo [a, b]
    usando o método da bissecção.
    """

    for i in count(1):
        x = (a + b) / 2
        print(f"{i=}, {x=}")

        fx = function(x)

        if abs(fx) <= error:
            break

        if fx < 0:
            if x == a:
                break
            a = x
        else:
            if x == b:
                break
            b = x

    return x


def multisect(a, b, function, *, threads=1, error=1e-6):
    """
    Encontra uma raíz de uma função no intervalo [a, b]
    usando um método parecido com a bissecção, porém com sub intervalos variáveis.
    """

    p = Pool(threads)
    intervals = threads + 1

    print(f"Using {threads} threads")
    print(f"Interpolating between {intervals - 1} values")

    for i in count(1):
        # Isso serve para interpolar um ponto X entre A e B para cada thread
        xs = [lerp(a, b, (i + 1) / intervals) for i in range(intervals - 1)]

        print()
        print(f"{i=}, {xs=}")

        # Mapeia a função para cada x usando threads.
        fxs = p.map(function, xs)

        for i, (x, fx) in enumerate(zip(xs, fxs)):
            if abs(fx) <= error:
                return x

            if (x == a) or (x == b):
                return x

            if (fx < 0) and (x > a):
                a = x
            elif (fx >= 0) and (x < b):
                b = x

    # Não tem como chegar aqui


def store_values(values):
    for lambida in values:
        size_from_lambda(lambida)


def guess_optimal(
    target_bpp, jplm_bin, input_raw, lightfield_name, threads=1, **kwargs
):
    """
    Encontra o valor de lambda que corresponde a uma taxa de BBP desejada.
    """

    jpl = JPLM(jplm_bin)

    del_files = kwargs.get("del_files", False)
    u = kwargs.get("u", 625)
    v = kwargs.get("v", 434)
    t = kwargs.get("t", 13)
    s = kwargs.get("s", 13)
    pixels = u * v * t * s

    @cache_on_file("./.temp/cached_values.json")
    def bpp_from_lambda(lambida):
        output = f"./.temp/{lightfield_name}_{lambida}.jpl"
        jpl.encode(input_raw, output, lambida=lambida, **kwargs)
        size = Path(output).stat().st_size
        if del_files:
            Path(output).unlink()
        print(".", end=" ")
        return size / pixels

    function = lambda x: target_bpp - bpp_from_lambda(x)
    return multisect(0, 200_000, function, threads=threads)


def guess_optimal_cfg(config_path, *args):
    """
    Encontra o valor de lambda que corresponde a uma taxa de BBP desejada a partir de um arquivo de configuração.
    """

    with open(config_path, "r") as file:
        params = json.load(file)
    
    for path in args:
        with open(path, "r") as file:
            params.update(json.load(file))

    jplm_bin = params.get("jplm_bin")
    input_raw = params.get("lightfield_raw_path")
    lightfield_name = params.get("lightfield_name")
    threads = params.get("max_cores")
    targets = params.get("target_bpps")

    for target_bpp in targets:
        lambida = guess_optimal(
            target_bpp, jplm_bin, input_raw, lightfield_name, threads=threads
        )
        print(f"bpp: {target_bpp} \t lambda: {lambida}")


def plot_values(path):
    with open(path, "r") as file:
        data = json.load(file)

    x = [int(i) for i in data.keys()]
    y = list(data.values())

    plt.scatter(x, y)
    plt.xlabel("Lambda")
    plt.ylabel("File size (bytes)")
    plt.ticklabel_format(useOffset=False, style="plain")
    plt.tick_params(axis="x", labelrotation=45)
    plt.show()
