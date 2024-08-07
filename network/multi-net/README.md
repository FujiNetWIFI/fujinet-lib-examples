# cross platform multi-network test

This application uses cc65 and fujinet-network-lib to show common code
using network get calls over multiple network adapters.

## building

```shell
# clean and make all platforms exe files (note: do the clean separately, else you will clean in between each target and lose the previous builds)
$ make clean
$ make release

# make apple2enh PO disk
make TARGETS=apple2enh disk

# make atari ATR disk
make TARGETS=atari clean dist atr
```
