# PPf24-SnakeGame

PPf24-SnakeGame is a research-focused project that explores parallelizing Genetic Algorithms (GA). The project leverages the classic Snake game as the simulation environment to test and optimize the performance of parallelized GA on evolving AI strategies.

## Table of Contents

- [Build](#build)
- [Usage](#usage)
- [License](#license)

## Build

1. Clone the repository:

```bash
git clone https://github.com/ChicoChen/PPf24-SnakeGame.git
cd PPf24-SnakeGame
```

2. Create and navigate to the build directory:

```bash
mkdir build
cd build
```

3. Configure the project with CMake:

```bash
cmake ..
```

4. build the project

```bash
make -j
```

## Usage

- Snake Viewer (GUI)

```bash
./snake_gui <model_file>
```

- Snake Viewer (TUI)

```bash
./snake_tui <model_file>
```

- GA Training (serial)

```bash
./test_serial <output_file>
```

- GA Training (parallel)

```bash
./test_openmp <output_file> <thread_num>
```

## License

This project is licensed under the [MIT License](./LICENSE). See the LICENSE file for details.
