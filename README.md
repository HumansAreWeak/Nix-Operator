# Nix Operator

A tool to help creating configuration files for NixOS, by enabling live-editing on files with features, such as
automatic
package and options search.

It is still under development!

## Plans

Here I summarize what features I want to accomplish with this project. The list is not static and may change over time.
It helps me to keep track of what tasks need more work.

- [ ] NCurses window with basic functionality
    - Eventually extend the basic functionality to support:
    - [ ] Editing of .nix files
- [ ] Look through all available repositories
    - And it should be possible with each repository to:
    - [ ] Look at all available packages for the repository
    - [ ] Look at all options for each package
    - [ ] Look at compatible flakes
    - [ ] Only show stuff if supported by host machine (Optional)
- [ ] Super fast startup time with smart caching

## Development

Developers need ```libncurses``` as well as ```libcurl```  installed on their systems, as the project depends on them.
The other dependencies are included as submodules, so cloning this project with:

HTTPS

```
git clone --recurse-submodules https://github.com/HumansAreWeak/Nix-Operator.git
```

SSH

```
git clone --recurse-submodules git@github.com:HumansAreWeak/Nix-Operator.git
```

should include them automatically. 