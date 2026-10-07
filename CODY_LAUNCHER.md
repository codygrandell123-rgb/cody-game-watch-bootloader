# Cody Game & Watch Launcher

This branch contains the first scaffold for a custom Game & Watch launcher.

## Layout

- Bank 1: existing Game & Watch bootloader
- Bank 2: Cody Launcher
- External flash / SD support: to be added as launcher features mature

The original Bank 1 bootloader build remains unchanged. The launcher has its own
makefile and target.

## Current v0.1 menu

- Original Games
- Custom ROMs
- Cheats
- System Info

The entries are placeholders for now. D-pad Up/Down moves the selection, A opens
the selected page, and B or PAUSE returns to the menu. POWER enters deep sleep.

## Build

```sh
make -f Makefile.launcher
```

Expected binary:

```text
build-cody/cody_launcher.bin
```

The launcher makefile defaults to `INTFLASH_BANK=2`.

## Flashing

Do not flash this binary over Bank 1.

After making a verified backup of the device and confirming your debug/flashing
connection, the launcher target can use the inherited flash rule:

```sh
make -f Makefile.launcher flash
```

This targets Bank 2 because `Makefile.launcher` defaults to
`INTFLASH_BANK=2`.

## Next implementation steps

1. Add a proper launcher page/state system.
2. Add a ROM/application catalog abstraction.
3. Add original-game entries backed by user-supplied backups.
4. Add cheat configuration as metadata, not patched copyrighted ROM data in Git.
5. Add recovery/fallback behavior before enabling automatic launching.
