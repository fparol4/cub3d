*This project has been created as part of the 42 curriculum by fcardozo, g-alves-.*

# cub3D

## Description

cub3D is a C project that renders a first-person view of a maze with miniLibX. It uses a grid-based raycasting technique: one ray is cast for each screen column, the first wall hit is found with the Digital Differential Analyzer (DDA) algorithm, and a directional texture is drawn on that wall. The scene also has ceiling and floor colors.

The program loads its map, player position and orientation, wall texture paths, and floor/ceiling colors from the supplied `.cub` file. The parser checks required settings, RGB ranges, map characters, exactly one player, wall enclosure, and connectivity of walkable cells. Short map rows are padded with spaces before validation.

The current build includes wall collision and a minimap. The minimap is shown in the top-right corner of the 3D view and can be expanded to fill the window.

## Instructions

Build on Linux with a C compiler, make, and the X11 development libraries required by the bundled miniLibX:

```sh
make
```

Run the program from the repository root:

```sh
./cub3D maps/001.cub
```

Use W/S (or the up/down arrow keys) to move forward/backward, A/D to strafe, the left/right arrow keys to turn, and M to switch between the 3D view and the full-window map. Press Escape or close the window to quit. The `maps/` directory contains four example scenes; their texture paths are relative to the working directory.

Each scene declares `NO`, `SO`, `WE`, and `EA` texture paths and `F`/`C` RGB colors before the map. Map cells use `1` for walls, `0` for floor, and one `N`, `S`, `E`, or `W` for the player spawn. Errors are reported to standard error with an `Error` line.

The Makefile provides `all`, `bonus`, `clean`, `fclean`, and `re` rules. The `bonus` rule currently builds the same program as `all`.

Runtime components live directly in `source/`; the map parser and renderer have their own `source/map/` and `source/render/` folders. Shared types and constants live in `header/core/`, and component interfaces live in `header/source/`. File-local helpers use `_module_action` names and appear before public functions. Rendering writes to the image buffer and redraws the scene and minimap when the player/view changes or the window needs refreshing.

## Resources

- [Lode Vandevenne's raycasting tutorial](https://lodev.org/cgtutor/raycasting.html): reference for grid raycasting, DDA, camera projection, and textured walls.
- [miniLibX manual](library/mlx/man/man3/mlx.3): reference for window, image, and event functions used by the project.
- [Bundled miniLibX README](library/mlx/README.md): build and platform information for the graphics library.
- [Project libft](library/libft/): the project's C utility library.

### AI use

AI assistance was used to review the cub3D subject and the project's architecture, suggest a split between source files and headers, and help organize rendering, player movement, raycasting, and window handling. It was also used to review function naming and parameter conventions, check the 42 Norm, and identify implementation and evaluation issues. The current code and its behavior must still be understood, verified, and explained by the project author.
