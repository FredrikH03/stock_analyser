FYI, enable nix, follow the arch wiki if needed
(https://wiki.archlinux.org/title/Nix)

Along with that, you need to set the following line
"experimental-features = nix-command flakes" in the file ~/.config/nix/nix.conf

after this, you should be able to run "nix develop" and you should be brought into the dev environment
