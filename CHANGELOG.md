# Changelog

## 0.1.4

### Changed

- `MOV M,M` is now rejected; it previously encoded silently as `HLT`

## 0.1.3

### Changed

- Extracted sjasmplus compatibility layer to `compat.inc`
- Defined CALM instructions via `iterate` instead of macros

### Fixed

- Typo in `apogee.inc`

## 0.1.2

### Changed

- `dba` macro renamed to `str`

## 0.1.1

### Changed

- The carry into `cs_hi` is now suppressed on the final loop iteration using the multiplier `(1 - % / %%)`, exploiting fasmg integer division to produce `0` on the last step and `1` on all others; matches the fix applied in Apogee 0.2.3

## 0.1.0

### Added

- Initial commit
