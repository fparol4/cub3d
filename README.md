*This project has been created as part of the 42 curriculum by fcardozo.*

# cub3D

## Description

cub3D is a first-person maze viewer written in C with miniLibX. It casts one
ray per screen column using DDA, chooses a texture for the wall face hit by
that ray, and draws ceiling and floor colors around the wall.

This version contains a temporary fixed map so rendering and movement can be
finished before the `.cub` parser. The provided scene argument is checked for
its extension but is not parsed yet. It is therefore a development build, not
a complete submission of the mandatory subject.

## Instructions

On Linux with X11 development libraries installed:

```sh
make
./cub3D maps/001.cub
```

Use W, A, S, D to move, left/right arrows to turn, and Escape or the window
close button to quit. The map stays visible at the top right of the 3D view.
Press M to expand it to the full window; press M again to return to 3D.
Wall collisions are enabled. `make clean`, `make fclean`, and `make re` are
available. The `bonus` rule currently aliases `all`.

The subject classifies the minimap and wall collisions as bonus features.
They are included here as development aids; their final submission treatment
must be decided before evaluation.

The XPM textures in `textures/` are simple development assets. Paths in scene
files will replace the fixed paths when parsing is implemented. Run from the
repository root until then.

## Resources

- [cub3D subject, version 12.0](../cub3d.subject.pdf): project requirements.
- [miniLibX manual](library/mlx/man/man3/mlx.3): window, image, and event API.
- [miniLibX README](library/mlx/README.md): build and platform details.
- [libft sources](library/libft/): allocation, string, and memory helpers.

AI assistance was used to review the project architecture and subject,
organize functions and headers, split rendering, add temporary textures,
check the 42 Norm, and identify failure paths. The code and behavior must
still be reviewed and explained by the author during evaluation.
