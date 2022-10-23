import subprocess

COMMAND_TEMPLATE = """
{jplm_bin}/jpl-encoder-bin 
--show-progress-bar 
--show-runtime-statistics 
--part 2
--type 0 --enum-cs YCbCr_2 -u {u} -v {v} -t {t} -s {s} -nc 3 --show-error-estimate
--border_policy 1 --lambda {lambida} 
--transform_size_maximum_inter_view_vertical 13
--transform_size_maximum_inter_view_horizontal 13
--transform_size_maximum_intra_view_vertical 31
--transform_size_maximum_intra_view_horizontal 31
--transform_size_minimum_inter_view_vertical 13
--transform_size_minimum_inter_view_horizontal 13
--transform_size_minimum_intra_view_vertical 4
--transform_size_minimum_intra_view_horizontal 4
--input {input} --output {output}
"""


class JPLM:
    """
    Wrapper do jplm. Só roda um comando aleatório
    """

    def __init__(self, jplm_bin):
        self.jplm_bin = jplm_bin

    def encode(self, input_raw, output, **kwargs):
        u = kwargs.get("u", 625)
        v = kwargs.get("u", 434)
        t = kwargs.get("u", 13)
        s = kwargs.get("u", 13)
        lambida = kwargs.get("lambida", 10_000)

        command = COMMAND_TEMPLATE.format(
            jplm_bin=self.jplm_bin,
            input=input_raw,
            output=output,
            lambida=lambida,
            u=u,
            v=v,
            t=t,
            s=s,
        ).split()

        res = subprocess.run(command, capture_output=True)

        if res.returncode != 0:
            raise Exception(res.stderr)

        return res
