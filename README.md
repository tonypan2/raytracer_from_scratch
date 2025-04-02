# A CPU-based raytracer

This is the raytracer I implemented for [Grinnell College](https://www.cs.grinnell.edu/)'s `CSC295: Computer Graphics` (Spring 2009), taught by [
Marge Coahran](https://www.dgp.toronto.edu/~mcoahran/). The code was written in May 2009. It has the following features:

- Anti-aliasing
- Reflection
- Refraction
- Soft shadows
- Texture mapping
- Bump mapping

The project has been adapted to run on Mac (confirmed working on macOS Sequoia 15.4 with Apple M4).

![Output](final.png?raw=true)

## Getting started

```
make
./raytrace 0 599
```

The output will be `result_0_599.raw`, which you can convert to PNG with:

```
brew install imagemagick
convert -size 800x600 -depth 8 result_0_599.raw output.png
```
