# Warning regressions

Run from the repository root with a Clang compiler that accepts C++26:

```sh
sh tests/run_warning_regressions.sh
```

Set `CXX=/path/to/clang++` to select a different compiler. Binaries are built
in a temporary directory and removed on exit. No files are generated in the
source tree.

The tests enable `-Werror`, AddressSanitizer (including allocation/deallocation
mismatch detection), and UndefinedBehaviorSanitizer. They verify the 33rd
source-prefix entry in every prefix distribution, array-owning destructors,
empty and populated parser inputs, and generation of packets that match a
known exact rule. These are focused regressions, not an audit of every legacy
code path.
