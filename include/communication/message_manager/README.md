# Message Manager

<a href="https://zebro.space/" target="_blank">
<img src="documentation/figures/projectluna.png" width="125" height="125" />
</a>

## About This Repository

This repository contains the source and header files of the Message Manager.
The Message Manager is a library that is included in `apps/app/active_object`
in order to enable any app that is created in our app-framework to communicate
with the router. 

## Dependencies

* **gcc**: Compiler for the OBC target platform.
* **gdb**: Debugger for the OBC target platform.
* **bazel**: Build tool to manage the project.
* **pdflatex**: Compiler for the documentation.
* **make**: Used to simplify commands by mapping them to make targets
* **lcov**: Code coverage tool and visualiser.
* **texlive-science**: Used for documentation.

## Build (all commands must be executed from the root monorepo solution directory)

To run the unit tests that come with the Message Manager:
```
make message_manager/bazel/test
```

To build the documentation and generate the output file which is called `documentation/report.pdf`:
```
make message_manager/documentation/build
```
