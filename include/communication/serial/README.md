# Serial library

<a href="https://zebro.space/" target="_blank">
<img src="documentation/figures/projectluna.png" width="125" height="125" />
</a>

## About This Repository

This repository contains the source and header files of the Serial library as
well as the MSP-Serial library. 
Design-wise, it brings enough flexibility to be used for both the bus manager, [app]-suite (Serial) and any MSP430FR5969 microcontroller used for a subsystem (MSP-Serial).

## Dependencies

* **gcc**: Compiler for the OBC target platform.
* **gdb**: Debugger for the OBC target platform.
* **bazel**: Build tool to manage the project.
* **pdflatex**: Compiler for the documentation.
* **make**: Used to simplify commands by mapping them to make targets
* **lcov**: Code coverage tool and visualiser.

## Build (all commands must be executed from the root monorepo solution directory)

To run the unit tests that comes with the PPU app run
```
make serial/bazel/test
```

To build the documentation and generate the output file which is called `documentation/report.pdf` run
```
make serial/documentation/build
```
