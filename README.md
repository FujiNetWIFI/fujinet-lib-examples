# fujinet-lib-examples

The way to test, exercise and explore the use of FujiNet via the fn-lib libraries is here in this repo.

## dir structure

This repo requires that projects are in the 2nd directory deep from the root folder, and that directory is the project's root.
This is to satisfy the build paths for finding the `makefiles` directories correctly.

## duck - webdav copy

```shell
duck --upload dav://anonymous@fujinet.home/dav/target-file source-file -existing overwrite

# copying from SD to local
duck -d dav://anonymous@fujinet.home/dav/target-file /path/to/local/file
```
