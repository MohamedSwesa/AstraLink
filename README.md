# AstraLink

> A C++ Telecommunications Channel and Error-Correction Simulator

## 📌 Overview
AstraLink is a robust C++ simulation application designed to model telecommunications channels and implement active error-correction mechanisms. Bridging the gap between low-level software programming and signal processing algorithms, it provides a functional demonstration of physical and data link layer concepts through programmatic transmission physics and data integrity protection.

## 🚀 System Architecture & Data Encapsulation
The simulator is built on a highly modular, object-oriented design that separates distinct behaviors into dedicated C++ classes to ensure clean, maintainable code.

* **Data Integrity (`DataStream.hpp`):** The raw binary payload is kept strictly private within the `DataStream` class, adhering to encapsulation principles to prevent accidental data modification during transit.
* **Operator Overloading:** An inline friend function overloads the stream insertion operator (`<<`) within the header file, allowing `DataStream` objects to be pushed directly to the console or file streams without multiple-definition compilation errors.
* **Polymorphic Framework (`ChannelCoder.hpp`):** An abstract base class named `ChannelCoder` serves as the blueprint, forcing any derived subclass to implement specific `encode()` and `decode()` functions. This enables the main program to utilize dynamic method dispatch, running the simulation without needing to know the exact algorithm in use.

## 🧮 Mathematical Error Correction
The `HammingCoder` class advances beyond basic error detection by implementing Forward Error Correction (FEC) to actively find and fix anomalies.

* **Parity Generation:** The algorithm pads the data to a multiple of four and processes it in 4-bit chunks (d1, d2, d3, d4). It uses bitwise XOR operations to calculate three overlapping parity bits to construct a 7-bit block: `p1 = d1 ⊕ d2 ⊕ d4`, `p2 = d1 ⊕ d3 ⊕ d4`, and `p3 = d2 ⊕ d3 ⊕ d4`.
* **Syndrome Calculation:** Upon receiving the block, the decoder recalculates these equations to generate a 3-bit syndrome (s3, s2, s1).
* **Error Location:** Converting this binary syndrome into a decimal integer calculates the exact 1-indexed position of a corrupted bit using the formula: `Error Position = (s1 × 1) + (s2 × 2) + (s3 × 4)`.
* **Bit Flipping:** If the resulting position is greater than zero, the decoder utilizes a ternary operator to flip the erroneous bit back to its correct state before stripping the parity bits.
* **Aliasing Limitation:** Because the Hamming (7,4) algorithm possesses a Hamming Distance of d_min = 3, it mathematically only guarantees the correction of single-bit errors. If the simulated channel flips two bits in the same block, a "false syndrome" occurs, causing the algorithm to mistakenly alter a correct bit and further degrade the payload.

## 📡 Stochastic Channel Simulation
To replicate physical transmission interference, the `Channel` class avoids predictable legacy functions like `rand()` in favor of modern C++ generation techniques.

* **Hardware Seeding:** The generator is seeded using `std::random_device` to capture actual hardware noise.
* **Engine & Distribution:** The simulator utilizes the `std::mt19937` (Mersenne Twister) engine to guarantee evenly distributed numbers, coupled with `std::uniform_real_distribution` to output a decimal between 0.0 and 1.0.
* **Interference Injection:** During the simulation, a "dice roll" is generated for every transmitted bit; if the generated float falls below the user-defined error probability (such as the 3.5% rate configured in the main pipeline), the bit is intentionally flipped to simulate corruption.

## ⚙️ Pipeline Orchestration & Memory Management
The `main.cpp` file acts as the central orchestrator, tying the mathematical blocks and channel physics into a stable pipeline.

* **Dynamic Allocation:** Upon startup, an interactive console menu takes the user's coding selection and utilizes the `new` keyword to dynamically allocate memory for the specific algorithm to a `ChannelCoder*` pointer.
* **File I/O Streams:** The standard `<fstream>` library manages data flow, utilizing `std::ifstream` to ingest raw binary from `input.txt` and `std::ofstream` to write the finalized, corrected payload out to `output.txt`.
* **Exception Safety:** The entire transmission process is heavily guarded by `try/catch` blocks. Issues such as missing files, non-binary payload characters, or failed parity checks throw `std::runtime_error` or `std::invalid_argument` exceptions, which are caught and safely pushed to the console via `std::cerr` to prevent application crashes.
* **Memory Cleanup:** Before terminating, the dynamically assigned pointer is deleted to prevent memory leaks.

## 📖 User Guide

### Prerequisites
- A modern C++ compiler (e.g., GCC, MSVC, Clang).

### Installation & Compilation
1. Clone the repository:
   ```bash
   git clone [https://github.com/MohamedSwesa/AstraLink.git](https://github.com/MohamedSwesa/AstraLink.git)
   ```
2. Navigate to the project directory:
   ```bash
   cd AstraLink
   ```
3. Compile the source code:
   ```bash
   g++ -o astralink main.cpp Channel.cpp ChannelCoder.cpp
   ```

### Running the Simulator
Launch the compiled executable from your terminal:
```bash
./astralink
```
Follow the interactive console prompts to configure channel parameters, input your target data payload, and observe the simulated transmission, error injection, and correction process in real-time.

## 🎓 Academic Context
This project was developed as a laboratory assignment for the Faculty of Electrical Engineering and Informatics at the Budapest University of Technology and Economics.
