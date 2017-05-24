# scramjet
the world's fastest general-purpose JavaScript engine

## Mission

Welcome to the scramjet project!  We aim to make scramjet the world's fastest
and best *general purpose* JavaScript engine.

Modern JavaScript engines are highly-optimized and maintained by some of the
world's largest tech companies.  What value could a little open source project
add to this crowded arena?  JavaScript engines are exquisitely fine-tuned for
the use case that matters to their owners: their web browsers.  Scramjet has
different objectives:

1. *Scramjet is generally useful.*  By avoiding optimizations (and implied
   pessimizations) driven by the browser wars, Scramjet can provide a more
   reliably-performant experience in contexts such as command-line tools and
   back-end services.
1. *Scramjet is engineered as a reusable, embeddable, library.*  Because of the
   assumptions made about their intended purpose, JavaScript engines tend to be
   bad neighbors, providing embedders little control over when and how
   resources such as threads and memory are used.
1. *We care about memory.*  I mean, we really, really care.  In Scramjet, we
   aim to minimize the memory footprint of executing JavaScript, and to utilize
   advanced garbage collection techniques to ensure, for example, that your
   service never tailspins into a multi-minute GC cycle.
1. *Marshalling performance is paramount.*  A JavaScript program relies on
   access to powerful, high-performance native libraries.  We aim to make the
   transition between script and native code *fast*, and the creation of
   bindings to native libraries simple.

## Building

One time after creating a clone, you will need to build BDE (until I can get
this automated somehow):

```
$ git clone https://github.com/scramjet-js/scramjet.git scramjet
$ cd scramjet
$ cd tools
$ bash build-bde.bash
```

Subsequently, to build:

```
$ cd src
$ mkdir build     # once
$ cd build
$ cmake ..
$ make
```

The following templates are provided:

- `src/groups/sjt` -- sample package group
- `src/apps/hello` -- sample application using `sjt` and BDE
