import json
import re
import subprocess
from itertools import count
from pathlib import Path

import matplotlib.pyplot as plt


find_bytes_info = re.compile("Bytes written to file: \d* bytes")
get_number = re.compile(" \d* ")


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


def bissect(a, b, function, *, error=1e-6):
    """
    Encontra uma raíz de uma função no intervalo [a, b]
    usando o método da bissecção.
    """

    last = None

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

        last = fx

    return x


@cache_on_file("./.temp/cached_values.json")
def size_from_lambda(lambda_):
    """
    Codifica o lightfield bikes de acordo com um valor de lambda e
    retorna o tamanho gerado em bytes.
    """

    pgx_path = "../../RAW/encoded/pgx/"
    output_path = "./.temp/bikes.jpl"

    # fmt: off
    command = [
        "../../bin/jpl-encoder-bin",
        "--show-progress-bar",
        "--show-runtime-statistics",
        "--part", "2",
        "--type", "0",
        "--enum-cs", "YCbCr_2",
        "-u", "625",
        "-v", "434",
        "-t", "13",
        "-s", "13",
        "-nc", "3",
        "--show-error-estimate",
        "--border_policy", "1",
        "--lambda", str(lambda_),
        "--transform_size_maximum_inter_view_vertical", "13",
        "--transform_size_maximum_inter_view_horizontal", "13",
        "--transform_size_maximum_intra_view_vertical", "31",
        "--transform_size_maximum_intra_view_horizontal", "31",
        "--transform_size_minimum_inter_view_vertical", "13",
        "--transform_size_minimum_inter_view_horizontal", "13",
        "--transform_size_minimum_intra_view_vertical", "4",
        "--transform_size_minimum_intra_view_horizontal", "4",
        "--input", pgx_path,
        "--output", output_path,
    ]
    # fmt: on

    res = subprocess.run(command, capture_output=True)

    if res.returncode != 0:
        raise Exception(res.stderr)

    bytes_info = find_bytes_info.search(str(res.stdout)).group()
    number_bytes = get_number.search(bytes_info).group()
    return int(number_bytes)


def store_values(values):
    for lambda_ in values:
        size_from_lambda(lambda_)


def guess_optimal(size):
    function = lambda x: int(size - size_from_lambda(int(x)))
    bissect(0, 50_000, function, error=100)


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


if __name__ == "__main__":
    Path(".temp/").mkdir(parents=True, exist_ok=True)

    # store_values(range(1_000, 50_000, 1000))
    guess_optimal(300_000)
    plot_values("./.temp/cached_values.json")
