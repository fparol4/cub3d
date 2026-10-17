*This project has been created as part of the 42 curriculum by fcardozo.*

# cub3D

## Description

cub3D is a C project that renders a first-person view of a maze with miniLibX. It uses a grid-based raycasting technique: one ray is cast for each screen column, the first wall hit is found with the Digital Differential Analyzer (DDA) algorithm, and a directional texture is drawn on that wall. The scene also has ceiling and floor colors.

The current codebase is a development version. It uses a fixed map, player position, colors, and texture paths in the code. The `.cub` argument is checked for its extension but its contents are not parsed, so this version does not yet meet the mandatory scene-file parsing requirements of the subject.

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

The program currently renders its fixed development scene regardless of the contents of the supplied `.cub` file. Use W, A, S, or D to move, the left and right arrow keys to turn, and M to switch between the 3D view and the full-window map. Press Escape or close the window to quit.

The Makefile provides `all`, `bonus`, `clean`, `fclean`, and `re` rules. The `bonus` rule currently builds the same program as `all`.

## Resources

- [Lode Vandevenne's raycasting tutorial](https://lodev.org/cgtutor/raycasting.html): reference for grid raycasting, DDA, camera projection, and textured walls.
- [miniLibX manual](library/mlx/man/man3/mlx.3): reference for window, image, and event functions used by the project.
- [Bundled miniLibX README](library/mlx/README.md): build and platform information for the graphics library.
- [Project libft](library/libft/): the project's C utility library.

### AI use

AI assistance was used to review the cub3D subject and the project's architecture, suggest a split between source files and headers, and help organize rendering, player movement, raycasting, and window handling. It was also used to review function naming and parameter conventions, check the 42 Norm, and identify implementation and evaluation issues. The current code and its behavior must still be understood, verified, and explained by the project author.
