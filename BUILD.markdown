## Update release version/date
- update version, as appropriate:
```bash
$ rake version:bump       # or
$ rake version:bump:minor # or
$ rake version:bump:major
```

- update `HISTORY.markdown` file:
```bash
$ rake prepare
```

- update release date, add & commit files, tag, push to origin/master:
```bash
$ rake release
```


## Commit documentation changes
```bash
$ cd doc/ && doxygen Doxyfile && cd ..
$ cd doc/html
$ git add .
$ git commit -v -m "Update docs ..."
$ git push
$ cd ../..
```
