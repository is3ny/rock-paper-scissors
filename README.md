# Rock-Paper-Scissors Automaton

A beautiful automaton that produces spiral patterns. My goal is to implement the cellular automaton entirely in GPU and see how well it performs.

**Done:**

  1. OpenGL primitive wrappers
  2. Cellular automata on GPU
  3. Canvas resize via GPU relative to the top-left corner

**TODO:**

  1. Add ImGui and create basic UI for manipulation purposes
  2. Refactor code to favor genericity and modularity

This means that the project is ok and accomplishes the initial goal! But for it to be shippable, it needs to support a certain number of features that allow interaction:

  1. Add/delete and change colors
  2. Set up the feed chain (who eats who)
  3. Speed up the simulation (ohh, yeah! It can be super fast!)
  4. Screenshots? (I mean, if the canvas is 3000x3000 pixels, you can't really take a screenshot by yourself).
  5. Move across the canvas: left, right, up, down...

It compiles without a problem with the latest g++, clang, and MSVC compilers. 

## So, how well it performs?

Very, very, very **good**! All complex logic is encapsulated into a shader, which is run in parallel for each pixel in the texture by GPU. This means that, if an average GPU has at least 1000 cores available (and the # of threads is even bigger), then it will perform magnitudes better than CPU (in the worst case!).

I've programmed a CPU version years ago: it's fps started to go downhill from 60 at around 600x600 canvas. This version was not optimized: at each step, a new array of pixels was computed and converted to a texture for further rendering in a single thread. The GPU version presented here also employs no optimization: each texel is processed and output to the second texture, then they swap. An obvious advantage is that there's no need to move pixel arrays from RAM to VRAM and therefore waste time.

All in all, 600x600 canvas renders 60 fps and may perform up to 5000 steps per frame without throttling the fps on Intel Intel HD Graphics 3000 IGPU (P.S., it seems that it supports OpenGL up to 3.1 while being able to run (this) OpenGL 3.3 application). Fps starts to decrease when the canvas' size exceeds approx. 2000x2000 pixels.

ATI Radeon 6730M allows for 10,000 steps per 1/60 frame on 600x600. The frame cap is broken at 5000x5000 canvas.

I have tested it on a relatively new (2017) notebook (my friend kindly let me have for an hour :), the cap was achieved at ~8,000x8,000 canvas on some Nvidia GPU. (Yeah, yeah, I know... it was some 7xxx series if I remember correctly... a more rigorous benchmark will be conducted, when the app will be shippable).

