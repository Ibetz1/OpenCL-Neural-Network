# Neural Network From Scratch

This project is an end-to-end implementation of a **neural network system built from scratch**, with a strong focus on low-level control, performance, and learning-by-building. It includes a lightweight **OpenCL wrapper library** designed to reduce boilerplate and make GPU compute workflows significantly less verbose and more approachable.

On top of this wrapper, the project provides a growing set of tools for **constructing, training, and experimenting with neural networks**, without relying on high-level ML frameworks.

---

## Motivation

Most modern machine learning frameworks abstract away the underlying compute model. While this is great for productivity, it obscures how neural networks actually work at a systems level.

This project exists to:
- Understand neural networks **from first principles**
- Learn how GPU compute pipelines operate under the hood
- Build a flexible ML foundation without heavyweight dependencies
- Maintain full control over memory, execution, and performance

---

## Core Components

### OpenCL Wrapper Library
A minimal abstraction layer over OpenCL that:
- Simplifies platform, device, and context creation
- Reduces kernel compilation and execution boilerplate
- Provides safer and more ergonomic buffer management
- Exposes explicit control over memory transfers and synchronization

The wrapper is intentionally thin—designed to remove noise without hiding the OpenCL execution model.

---

### Neural Network Toolkit
Built on top of the OpenCL wrapper, the toolkit includes (or will include):
- Tensor and buffer utilities
- Layer abstractions (dense, activation, etc.)
- Forward and backward propagation
- Loss functions and optimizers
- Training loops and evaluation helpers

All components are designed to be modular and inspectable.

---

## Use Cases

- Learning how neural networks work at a low level
- Experimenting with custom architectures
- GPU compute experimentation
- Building ML systems without large frameworks
- Research and educational projects

---

## Design Philosophy

- **Transparency over abstraction**  
  You should always know what the GPU is doing.

- **Minimalism**  
  Only build what is necessary; no hidden magic.

- **Performance-aware**  
  Memory movement and execution cost matter.

- **Hackable by design**  
  Easy to modify, extend, and instrument.

---

## Future Roadmap

- Convolutional layers
- Recurrent and attention-based models
- Automatic differentiation
- Model serialization
- Visualization and debugging tools
- Multi-device training support

---

## Disclaimer

This project is experimental and educational in nature. It is not intended to replace production-grade ML frameworks, but rather to provide insight into how such systems are built.

Use at your own risk—and enjoy going deep.
