# Bus Manager

<a href="https://zebro.space/" target="_blank">
<img src="documentation/figures/projectluna.png" width="125" height="125" />
</a>

## About This Repository

This repository contains the source and header files of the Bus Manager.
The Bus Manager is responsible for reliably transferring commands and data between subsystems.
This implies checking for CRC errors and sending a retransmission request or NACK when it needs to.
The Bus Manager acts between the Application Layer and the Physical Layer of the OSI model.
For more information about the exact Message Format, see the documentation in this repository as well as
the `Proposed_Message_Format.pdf` document which can be found in MS Teams.

## Dependencies

* **gcc**: Compiler for the OBC target platform.
* **gdb**: Debugger for the OBC target platform.
* **bazel**: Build tool to manage the project.
* **pdflatex**: Compiler for the documentation.
* **make**: Used to simplify commands by mapping them to make targets
* **lcov**: Code coverage tool and visualiser.
* **texlive-science**: Used for documentation.

## Build (all commands must be executed from the root monorepo solution directory)

To run the unit tests that come with the Bus Manager:
```
make bus_manager/bazel/test
```

To build the documentation and generate the output file which is called `documentation/report.pdf`:
```
make bus_manager/documentation/build
```
