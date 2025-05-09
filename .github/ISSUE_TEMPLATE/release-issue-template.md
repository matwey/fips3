---
name: Release issue template
about: Create this issue to start release process
title: Release 3.x.x
labels: release
assignees: matwey, hombit

---

- [ ] Fix title of the issue
- [ ] CMake
- [ ] Git tag
- [ ] Wait for CI
- [ ] Homebrew Caskroom
- [ ] Flatpack
- [ ] openSUSE
- [ ] Fedora

### CMake
1. Go to `CMakeLists.txt` and update version

### Git tag
1. Commit the chage with `git commit -a -v -S`
2. Make signed tag `git tag -s 3.x.x`
3. `git push origin —tags`

### Wait for CI
1. Wait for [travis](https://travis-ci.org/matwey/fips) and [appvevor](https://ci.appveyor.com/project/matwey/fips3)

### Homebrew Caskroom
1. Check no PR is open in [Homebrew Casroom repo](https://github.com/Homebrew/homebrew-cask/pulls?utf8=✓&q=is%3Apr+fips)
2. `cask-repair -v 3.x.x fips`

### Flatpack
1. fork <https://github.com/flathub/space.fips.Fips>
2. update `space.fips.Fips.appdata.xml` and `space.fips.Fips.json`
3. try local build `flatpak install flathub org.kde.Sdk//5.12`, `flatpak-builder ./t space.fips.Fips.json`, `test run flatpak-builder --run ./t/ space.fips.Fips.json fips`
4. Create pull request to `https://github.com/flathub/space.fips.Fips`

### openSUSE
1. Go to opensuse:
2. `osc branch science fips`
3. `osc co home:matwey:branches:science/fips`
4. Edit `_service`
5. `osc service dr`
6. drop old sources
7. `osc build --clean --local`
8. `osc commit`
9. `osc sr —cleanup`
