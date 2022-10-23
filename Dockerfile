FROM ubuntu:jammy

WORKDIR /home

ENV TZ=America/Sao_Paulo
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ

RUN apt-get update

# Install utilities
RUN apt-get install -y \
        wget \
        unzip \
        make \
        cmake \
        git

# Install compiler stuff
RUN apt-get install -y \
        g++ \
        libx11-*

RUN apt-get install -y \
        python3 \
        pip

RUN pip install matplotlib

VOLUME /code
WORKDIR /code

CMD ["bash"]
