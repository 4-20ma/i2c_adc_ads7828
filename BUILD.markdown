## Update release version/date
- VERSION file (use v1.0.2 notation)
- i2c_adc_ads7828.h file (\date field)
- source documentation (Doxygen GUI frontend):
  ```
  open /Applications/Doxygen.app/
  ```
- HISTORY.markdown release history

---
## Commit source code changes
```
git add .
git commit -v -m "Update ..."
git push
```

---
## Tag source code
```
git tag vX.Y.Z
git push --tags
```

---
## Commit documentation changes
```
cd doc/html
git add .
git commit -v -m "Update docs ..."
git push
cd ../..
```
