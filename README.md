# Lana Tennis [![Build Status](https://travis-ci.org/andystanton/lana-tennis.png?branch=master)](https://travis-ci.org/andystanton/lana-tennis)

An implementation of the classic game Pong. It is written in C++ and uses the [GLFW library](http://www.glfw.org) and [CMake](http://www.cmake.org/).

## Requirements

* A C++ compiler
* [CMake >= 2.8](http://www.cmake.org/cmake/resources/software.html)

## Quick Start

Clone the project:

```sh
git clone https://github.com/andystanton/lana-tennis.git --recursive && cd lana-tennis
```

Run the following commands to generate the makefile and build:

```sh
./configure
make
```

This will create the lana-tennis executable in the project directory.

## Generating project files for your IDE

Run the following command to generate the project files for your IDE:

```sh
cmake . -G {ide_name}
```

For more information on CMake's project generators and a full list of supported platforms and IDEs, see the [documentation](http://www.cmake.org/Wiki/CMake_Generator_Specific_Information).


## Gameplay

<a href="http://www.youtube.com/watch?feature=player_embedded&v=78twd4Xdnxc
" target="_blank"><img src="http://img.youtube.com/vi/78twd4Xdnxc/0.jpg" 
alt="Lana Tennis AI vs. AI" width="240" height="180" border="10" /></a>

