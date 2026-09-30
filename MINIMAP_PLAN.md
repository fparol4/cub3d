# Minimap: current state and implementation plan

## Objective

Complete the 2D minimap with a visible player, smooth movement, rotation,
collision, and a field of view whose rays stop at walls. Reuse the same player
and ray calculations for 3D rendering in the next session.

Work through one checkpoint at a time together. The first implementation pass
now covers the core interactive minimap and leaves the parser and 3D renderer
for later work.

## Current state

| File | Responsibility and state |
| --- | --- |
| `source/main.c` | Creates the game, enters the MLX event loop, and cleans up afterward. Arguments are ignored. |
| `source/game/set.c` | Creates a static 8-by-6 map with border and internal walls, initializes the player and camera, sets up a 1024-by-768 window, image, and input/frame hooks. |
| `source/game/render_minimap.c` | Draws the map, player and heading, and a 41-ray 90-degree FOV fan clipped by DDA wall hits. |
| `source/game/render_common.c` | Provides bounded pixel writes, filled squares, and line drawing shared by renderers. |
| `source/game/hook.c` | Tracks pressed and released movement/turn keys, exits on Escape, and clears input when focus is lost. |
| `source/game/player.c` | Applies time-based movement, diagonal normalization, axis-separated wall collision, and direction/camera rotation. |
| `source/game/raycast.c` | Exposes DDA wall hits with cell, side, world position, and ray parameter for reuse by 3D rendering. |
| `source/game/general.c` | Owns the frame callback, caps elapsed time after stalls, clears and redraws the frame, and handles cleanup. |
| `cub3d.h` | Contains image, map, player, and game types. Tiles are 128 pixels; the 8-by-6 demo map fills the 1024-by-768 window. Player position and direction use map units; camera plane is initialized to a 90-degree FOV. Texture paths and four texture images remain placeholders. |
| `source/game/game.h` | Declares setup, cleanup, input, update, and drawing functions, plus named MLX event IDs and masks. |
| `Makefile` | Builds with `-Wall -Wextra -Werror`, libft, Linux MLX, X11, and libm. Generates dependency files. |

### Problems and missing pieces

- The minimap uses a fixed demo map; arguments and `.cub` files are not parsed.
- The repository has generated outputs under `build/` and a tracked `cub3D`
  executable. Ignore rules now cover future generated outputs, but existing
  tracked build artifacts remain in the index.
- The project's own source files do not pass Norminette. Style cleanup is outside
  the current implementation focus.
- There is no map parser, map validation, texture loading, or 3D projection yet.
  No subject PDF or automated project tests were found in the inspected tree.

### Verification performed

- A fresh `make` succeeds with the project's warning flags after restoring
  execute permission on `library/mlx/configure`.
- Runtime interaction has not been checked in a visible X session.

## Proposed conventions

- Keep position, collision size, and ray calculations in map units. Convert to
  pixels only when drawing the minimap.
- Use `grid[y][x]`; positive X points right and positive Y points down.
- Start in the center of a floor tile, for example `(2.5, 2.5)`, facing east.
- Use a unit direction vector and a perpendicular camera plane. The current
  90-degree FOV uses plane length `tan(FOV / 2) = 1`.
- Use W/S for forward/backward, A/D for strafing, and left/right arrows for turning.
- Keep input, simulation, raycasting, and drawing separate so the 3D view can
  consume the same simulation and ray results.
- Use enums for key slots, ray hit sides, tile kinds, and the current view mode.
- Treat out-of-map positions as blocked. Later, the parser must also reject
  invalid maps; collision checks do not replace input validation.

## Checkpoints

### 1. Restore a reliable build and valid starting scene — done

- Restore execute permission on MLX's configure script and verify a fresh build.
- Move player initialization into setup; initialize position, direction, and plane.
- Add a small test map with internal walls, an exposed corner, and a corridor.
- Use image dimensions in pixel bounds checks and explicitly clear each frame.
- Add ignore rules for generated build outputs.

**Check:** fresh build succeeds; the player appears centered on a floor tile;
Escape and the window close button both cleanly exit.

### 2. Establish input state and continuous rendering — done

- Register key-press and key-release hooks and store held keys in game state.
- Add a loop callback: compute elapsed time, update the player, render, present.
- Use elapsed time for speed and cap large time steps after stalls.
- Clear held keys when focus is lost; account for X11 key repeat behavior.
- Render in order: background, map, rays once available, then player and heading.

**Check:** holding and releasing keys changes state reliably; no stuck movement
after switching windows; repeated frames leave no trails.

### 3. Add movement and collision — implemented

- Move relative to the direction vector and its perpendicular strafe vector.
- Normalize combined input so diagonal movement is not faster.
- Give the player a small collision footprint in map units consistent with its
  visible marker; check all wall cells overlapped by the candidate footprint.
- Resolve X and Y separately to allow sliding along walls.
- Subdivide movement when needed so a large step cannot cross a wall.

**Check:** forward, backward, strafe, diagonal input, opposing keys, border walls,
internal obstacles, corners, and corridors behave consistently. The player never
enters a wall or leaves the map.

### 4. Add rotation and a visible heading — implemented

- Rotate the direction and camera plane together using the same rotation matrix.
- Express rotation speed in radians per second.
- Draw a short line from the player's center to show its direction.
- Keep the direction unit length and the plane perpendicular with fixed length.

**Check:** a full turn restores the original orientation; movement follows the
new heading; moving and turning together works; the FOV does not widen over time.

### 5. Implement reusable wall raycasting — implemented

- Implement DDA: advance through grid boundaries until the ray reaches a wall.
- Handle horizontal and vertical rays without dividing by zero, and check bounds
  before every grid access. Define consistent behavior at exact grid corners.
- Return a hit result containing hit/miss, cell coordinates, hit side, hit position,
  and ray parameter. Distinguish the ray parameter from Euclidean distance.
- Keep ray traversal independent of MLX and drawing.

**Check:** cast single rays in cardinal and diagonal directions, near corners,
and toward internal obstacles. Verify known hit positions and safe termination.

### 6. Show the field of view with obstacles — implemented

- For horizontal camera coordinate `u` between -1 and 1, cast
  `ray_dir = dir + plane * u`.
- Draw a configurable fan of rays from the player to their first wall hits.
  Use enough samples to show the visible area clearly; a filled cone is optional.
- Draw the FOV edge rays and center heading distinctly, keeping the player visible.
- Use the same camera sampling convention tomorrow for screen columns.

**Check:** walls occlude rays, turning moves the fan smoothly, the FOV stays
constant, and movement close to walls or corners produces no out-of-bounds reads.

### 7. Build check and prepare the 3D handoff — in progress

- Keep the fresh build passing while iterating on behavior.
- Review movement, rotation, collision, focus loss, Escape, and window close in the
  graphical session when available.
- Document the camera convention and ray hit fields for the next session.

**Current implementation:** an interactive minimap with a visible player, input
state, time-based movement and turning, collision against internal and border
walls, and wall-clipped FOV rays. Build succeeds; graphical behavior still needs
a visual run.

## Next session: 3D

Reuse the camera-plane rays per screen column. With a unit direction and its
perpendicular plane, the hit parameter for these unnormalized rays gives the
perpendicular wall distance needed for projection. Do not substitute Euclidean
ray length, which causes fisheye distortion.

The first 3D MVP is planned in [MVP_3D_PLAN.md](MVP_3D_PLAN.md). Texture loading
and a validated `.cub` parser remain beyond that MVP.
