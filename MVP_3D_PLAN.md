# First 3D MVP plan

## Goal

Add a basic first-person 3D view of the existing demo map, switchable with the
minimap using a key. Keep movement, collision, rotation, and the 90-degree camera
shared between both views.

## MVP scope

- Keep the current hard-coded 8-by-6 map and its internal obstacles as the test
  scene. Do not add map parsing in this milestone.
- Add a named view-mode enum state and a key toggle, proposed as **M**.
- In minimap mode, keep drawing the map, player heading, and FOV rays.
- In 3D mode, cast one DDA ray per screen column using
  `ray_dir = direction + camera_plane * camera_x`.
- Project the first wall hit as a vertical slice. Use `t_ray_hit.distance` for
  perpendicular wall distance; the DDA ray parameter already has this meaning
  because the direction vector is unit length and the camera plane is
  perpendicular to it.
- Use distinct flat colors for ceiling, floor, and walls, and shade one wall
  orientation differently using the hit side. Leave texture mapping for later.
- Keep the existing window dimensions and draw into the current MLX image.
- Preserve Escape and window-close behavior in both views.

## Implementation steps

1. Add the view-toggle key to the named input definitions and handle it once per
   key press. Holding the key must not flip views every frame.
2. Split frame drawing into a small view dispatcher and a `render_3d` function.
   The simulation and input updates stay shared.
3. For each screen column, compute its camera coordinate in `[-1, 1]`, cast a
   ray, and use the hit distance to compute `line_height = screen_height /
   distance` with a small minimum-distance guard.
4. Clamp the projected top and bottom to the frame. Fill ceiling, wall, and
   floor spans in that column. Use `hit.side` to shade the wall faces.
5. Leave minimap rendering untouched and ensure the same player state survives
   switching views.

## Completion checks

- A fresh `make` succeeds with the project's warning flags.
- The M key switches between minimap and 3D without resetting the player.
- Forward/backward, strafing, turning, collision, Escape, and window close work
  in either view.
- Walls grow as the player approaches and shrink as the player moves away.
- Internal walls and the enclosing border appear in the expected directions.
- Near-zero distances do not cause division errors or unbounded drawing.

## Out of scope

- `.cub` parsing and map validation
- Wall textures and texture-coordinate sampling
- Sprites, doors, floor casting, and ceiling textures
- Resizing the window or changing the map to a file-loaded level

After this MVP, texture loading and validated `.cub` parsing can be planned as
separate steps.
