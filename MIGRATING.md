# Parser migration

> Historical migration record: the component now lives in `source/parser/` and its interface is `header/source/parser.h`.

## Plan

1. **Establish the project boundary.** Keep the parser component under `source/map/` and use the existing `header/source/map.h` for its runtime API and parser declarations.
2. **Replace the development map.** Remove the hard-coded grid, colors, textures, and player spawn from `source/map.c`; make `map_parse(t_game *g, char *map_path)` load the requested `.cub` file.
3. **Port archive/config parsing.** Read lines with the project `ft_get_next_line`, accept each required texture and RGB setting once, preserve map whitespace, and reject malformed/unknown configuration lines.
4. **Port map validation.** Validate map characters, exactly one player, closed walkable cells, and connectivity; handle ragged rows and out-of-range coordinates safely.
5. **Adapt parser data to runtime structs.** Transfer the validated grid, dimensions, RGB colors, texture paths, player position, direction vector, and camera plane into `t_map` and `t_player` only after all validation succeeds.
6. **Integrate the build and lifecycle.** Compile all `source/map/*.c` files, preserve the existing `map_cleanup` lifecycle, and report parser errors through the project error helper.
7. **Verify the migration.** Build with the project warning flags, check Norminette and whitespace, and exercise valid and invalid maps including orientation, duplicate/missing settings, invalid RGB, invalid cells/player counts, open borders, ragged rows, and disconnected walkable areas.

## Applied changes

- [x] Move the map implementation into the `source/map/` component and add parser-private declarations.
- [x] Replace the development map with `.cub` archive/config parsing using `ft_get_next_line`.
- [x] Validate configuration values and map structure before committing parsed state.
- [x] Convert parsed RGB and player orientation into the runtime representation.
- [x] Update source discovery/build dependencies and preserve cleanup on failure.
- [x] Run build, style, and parser behavior checks.

### Change log

- `source/map/map.c`: replaces the hard-coded implementation formerly in `source/map.c`. The existing `map_parse` entry point coordinates parsing in temporary state, reports an invalid map through `common_error`, and commits only a fully validated map. Player directions become unit direction vectors and a perpendicular camera plane scaled by `PLAYER_PLANE_SCALE`. The player's source marker is converted to floor in the runtime grid so movement and raycasting treat the spawn cell as walkable. It also implements the existing `map_cleanup` lifecycle entry point, freeing all row strings and texture paths and resetting the map struct.
- `source/map/map_reader.c`: checks the `.cub` suffix, opens the archive, reads with the repository's `ft_get_next_line`, preserves map-line whitespace, and collects map rows. Texture identifiers are excluded from map-start detection even though `NO`, `SO`, `WE`, and `EA` begin with direction letters. Ragged rows are normalized by the grid helper after reading.
- `source/map/map_config.c`: parses `NO`, `SO`, `WE`, and `EA` texture paths plus `F` and `C` color declarations. Rejects duplicate declarations, missing values, malformed identifiers, and unknown configuration lines; records presence with the existing `CUB_*` field bits.
- `source/map/map_color.c`: parses exactly three decimal RGB components in the 0–255 range and packs them into the renderer's `0xRRGGBB` integer format.
- `source/map/map_cells.c`: centralizes player/walkable-cell checks, safe grid access, and walkable-cell counting.
- `source/map/map_validate.c`: validates required settings, map dimensions and characters, exactly one player, and cardinal enclosure of all walkable cells; then requests a connectivity check.
- `source/map/map_connectivity.c`: performs a bounded breadth-first traversal from the player and rejects disconnected walkable regions. Allocation failures are reported to validation as failure.
- `source/map/map_grid.c`: pads shorter rows with spaces into a rectangular, NUL-terminated grid. This matches the runtime renderer's direct `[y][x]` indexing while keeping missing cells void and non-walkable.
- `header/source/map.h`: retains the existing runtime API (`map_parse` and `map_cleanup`) and its `t_map`/`t_game` types.
- `header/source/map.h`: now holds the runtime API plus parser staging/connectivity structs and internal declarations; the separate nested `header/source/map/parser.h` has been removed, and no parser-only fields were added to the public game structs.
- `Makefile`: adds `source/map/*.c` to the source list so the component is compiled and linked.
- `source/runtime.c`: removes the development-map-specific error and duplicate parser error report; `map_parse` now reports parse failures before runtime cleanup.
- `library/libft` and `cub-parser/`: left unchanged. The parser uses existing project libft functions and the existing `ft_get_next_line` symbol rather than importing the parser's bundled libft or renaming the project's API.
- `maps/002.cub`, `maps/003.cub`, and `maps/004.cub`: added runnable parser examples with different map sizes/layouts, RGB settings, and south/west/south spawn directions, all pointing at the repository's existing XPM textures.

### Verification

- `make -j4` succeeds with `-Wall -Wextra -Werror`.
- `norminette source/map header/source/map` passes.
- A temporary parser harness accepts all four player orientations and verifies map dimensions, packed RGB values, spawn coordinates/direction, and that the spawn grid cell is floor.
- The harness accepts a ragged but enclosed map after padding its rows, and rejects missing settings, duplicate textures, out-of-range RGB, invalid map characters, multiple players, open maps, and disconnected walkable regions.
- The bundled `cub-parser/map.cub` is rejected because its walkable cells at row 4, columns 25–31 touch void spaces directly above; this matches the parser's closed-map rule.
- The map parser validates map geometry and configuration paths; texture file existence and XPM loading remain checked by the existing `texture_load` stage.

