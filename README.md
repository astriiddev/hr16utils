# hr16utils

An update to nekosynth's [original hr16utils](https://www.nekosynth.co.uk/wiki/AlesisHR16/).

Mostly focused on cleaning up the memory/Segfault issues in romutil at the moment.

Also cleaned up all the warning that gcc produced and made it ANSI C compatible since it was mostly there already.

# Build all

Either build the "old fashioned" way with the orignal SConstruct file:
```
git clone https://github.com/astriiddev/hr16utils/
cd hr16utils
scons
```
Or with the new Makefile:
```
git clone https://github.com/astriiddev/hr16utils/
cd hr16utils
make
```
# Build individual
The added Makefile allows for building the programs individually
```
git clone https://github.com/astriiddev/hr16utils/
cd hr16utils
make ROMUTIL
make DUMPTITLE
make SAMPLEUTIL
make UNROM
```
(NOTE: the program name must be typed in all caps when building it individually with make)
