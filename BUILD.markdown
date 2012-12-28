## Update release version/date
- `VERSION` file (use [semantic versioning](http://semver.org))
- `i2c_adc_ads7828.h` file (`\date` field)
- source documentation (Doxygen command-line):
  ```
  $ cd doc/ && doxygen Doxyfile && cd ..
  ```
- `HISTORY.markdown` file release history

## Commit source code changes
```bash
$ git add .
$ git commit -v -m "Update ..."
$ git push
```

## Tag source code
```bash
$ git tag vX.Y.Z
$ git push --tags
```

## Commit documentation changes
```bash
$ cd doc/html
$ git add .
$ git commit -v -m "Update docs ..."
$ git push
$ cd ../..
```
