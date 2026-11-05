# Understanding cub3d: from one pixel to DDA and the first-person view

This guide describes the code in `/home/fcardozo/Universe/xNow/cub3d`, as read on November 5, 2026. It concentrates on the geometry behind the implementation. Function names and file links refer to this checkout.

The central idea is small: **follow a straight ray through a grid, visiting cells in the order the ray enters them, until it enters a wall.** DDA organizes those visits. Drawing a line of pixels and projecting a wall are separate tasks that use the result.

Start with sections 1–9. Leave the camera, projection, and textures for a second reading. Open [dda-explorer.html](dda-explorer.html) in a browser beside this document and use its default example while reading sections 5–8. It works locally without dependencies or network access.

## Contents

1. [What the program produces](#1-what-the-program-produces)
2. [Coordinates and units](#2-coordinates-and-units)
3. [From one pixel to the minimap](#3-from-one-pixel-to-the-minimap)
4. [Position, vectors, and a ray](#4-position-vectors-and-a-ray)
5. [The problem DDA solves](#5-the-problem-dda-solves)
6. [Deriving the DDA variables](#6-deriving-the-dda-variables)
7. [A complete numerical walkthrough](#7-a-complete-numerical-walkthrough)
8. [Reading the actual DDA code](#8-reading-the-actual-dda-code)
9. [Special directions and corners](#9-special-directions-and-corners)
10. [Making a field of view](#10-making-a-field-of-view)
11. [From a hit to a wall strip](#11-from-a-hit-to-a-wall-strip)
12. [Why the distance avoids fisheye distortion](#12-why-the-distance-avoids-fisheye-distortion)
13. [Textures](#13-textures)
14. [Movement, rotation, and collision](#14-movement-rotation-and-collision)
15. [The rest of the program](#15-the-rest-of-the-program)
16. [Source reading route](#16-source-reading-route)
17. [Questions and answers for self-checking](#17-questions-and-answers-for-self-checking)
18. [Reference sheet](#18-reference-sheet)

## 1. What the program produces

The map is a two-dimensional arrangement of squares. A `1` is a wall; a `0` is walkable floor. The player occupies a precise position inside that world.

The minimap draws those squares from above. It can draw rays on top to show what the player sees. The first-person view uses the same wall hits to decide how tall each strip of wall should appear.

```text
map + player
      |
      +--> rectangles + player marker --> minimap
      |
      +--> ray directions --> DDA --> wall hits
                                      |
                                      +--> lines on minimap
                                      +--> textured strips in first-person view
```

You can draw the minimap before implementing raycasting. You need raycasting to find visibility and wall depth, not to display the map itself.

## 2. Coordinates and units

World `x` grows to the right. World `y` grows downward, following the order of columns and rows in the map file.

```text
(0,0) --------> +x / East / Leste
  |
  |
  v
 +y / South
```

| Direction | Vector `(x, y)` |
|---|---|
| North / Norte / N | `(0, -1)` |
| South / Sul / S | `(0, 1)` |
| East / Leste / E or L | `(1, 0)` |
| West / Oeste / W or O | `(-1, 0)` |

The map file uses `N`, `S`, `E`, `W` for the starting orientation, even if we use Portuguese compass names while discussing it.

A map cell is **one world unit wide and one world unit high**. It is not one screen pixel. Cell `(3, 2)` occupies `3 <= x < 4`, `2 <= y < 3`.

The array access is `grid[y][x]`: row first, then column. A continuous position `(3.7, 2.2)` belongs to cell `(floor(3.7), floor(2.2)) = (3, 2)`.

The parser places the player at a tile's center by adding `0.5` to both cell coordinates. In [maps/001.cub](maps/001.cub), the `E` is in column 3, row 2, so the starting position is `(3.5, 2.5)` and direction is `(1, 0)`. See `_parser_apply_player` in [parser.c](source/parser/parser.c).

Keep three quantities distinct:

| Quantity | Example | Meaning |
|---|---|---|
| Cell index | `(3, 2)` | Which map square? |
| World position | `(3.7, 2.2)` | Where inside the world? |
| Pixel position | `(168, 108)` | Where in a particular image? |

## 3. From one pixel to the minimap

Read [render_draw.c](source/render/render_draw.c) and [render_view.c](source/render/render_view.c).

An image is a block of memory. `render_px` finds the bytes for a pixel using:

```text
address = image.addr + pixel_y * stride + pixel_x * (bpp / 8)
```

`stride` is the number of bytes occupied by one image row, including any padding. `bpp` means bits per pixel. For 32-bit pixels, moving one pixel right means moving four bytes. Moving one row down means adding `stride` bytes.

`render_rect` fills a rectangle by calling the pixel writer for every pixel in it. To draw a map cell, the code converts its two opposite world corners into image positions and fills the rectangle between them. Large enough tiles receive a border.

The conversion is:

```text
pixel_x = viewport.x + int(world_x * viewport.width  / map.width)
pixel_y = viewport.y + int(world_y * viewport.height / map.height)
```

For an 8-by-7 map drawn in a 320-by-280 viewport starting at `(20, 20)`, each world unit occupies 40 pixels. World position `(3.5, 2.5)` becomes pixel `(160, 120)`.

The world coordinates do not change when the minimap changes size. Only their drawing scale changes. The overlay uses a common scale to approximately preserve square tiles, with integer rounding. The full map mode fills the whole frame, so a map with a different aspect ratio can appear stretched.

`render_line` draws the visible ray lines using an integer error algorithm of the Bresenham family. **This line drawing routine is separate from the DDA traversal in `raycast.c`.** DDA finds a wall hit in world space. The line routine colors screen pixels between the player and that hit.

## 4. Position, vectors, and a ray

A position is a point. A vector describes a change.

If `P = (1.5, 1.5)` and `r = (1, 0.4)`, then adding `r` to `P` gives `(2.5, 1.9)`: one unit right and 0.4 units down.

We describe all points along a ray with one equation:

```text
Q(t) = P + t * r, with t >= 0

x(t) = P.x + t * r.x
y(t) = P.y + t * r.y
```

`t` is a shared progress parameter. Imagine a clock: at each value of `t`, both coordinates are evaluated at that same value. It is a mathematical analogy, not the game's elapsed frame time.

| `t` | `x = 1.5 + t` | `y = 1.5 + 0.4t` |
|---|---|---|
| 0 | 1.5 | 1.5 |
| 0.5 | 2.0 | 1.7 |
| 1 | 2.5 | 1.9 |
| 1.25 | 2.75 | 2.0 |
| 1.5 | 3.0 | 2.1 |

The ray is continuous. DDA chooses particular values of `t` where this line crosses grid boundaries.

The vector `r` does not have to be unit length. Here its length is `sqrt(1² + 0.4²) ≈ 1.077`. Consequently, `t` is not automatically the physical length traveled along the ray. That length is `t * length(r)`. We will return to this distinction in section 12.

Before continuing, make sure you can calculate `Q(0.5)` from this equation. That one substitution is the basis of everything that follows.

## 5. The problem DDA solves

Suppose you follow the ray by adding a tiny fixed step repeatedly. If the step is small, you do a lot of work. If it is too large, you can skip a cell containing a wall.

The useful observation is that **you remain in the same cell until you cross one of its edges**. Map occupancy cannot change between those crossings.

Every grid boundary is one of two kinds:

```text
vertical lines:   x = ..., 1, 2, 3, 4, ...
horizontal lines: y = ..., 1, 2, 3, 4, ...
```

You need only ask: which boundary will this ray reach next, a vertical one or a horizontal one?

DDA means Digital Differential Analyzer. In this project, its job is to keep the two upcoming boundary crossings in order. Picture two appointment lists:

```text
next vertical crossing:   t = 0.50, then 1.50, then 2.50, ...
next horizontal crossing: t = 1.25, then 3.75, then 6.25, ...
```

Merge those lists in increasing order:

```text
0.50 vertical -> 1.25 horizontal -> 1.50 vertical -> ...
```

At each appointment, enter the adjacent cell and inspect it. Stop at a wall. This is the entire traversal idea.

## 6. Deriving the DDA variables

### The current cell: `map_x`, `map_y`

The starting cell is `floor(P.x), floor(P.y)`. During traversal these integers identify the cell being inspected, not the continuously moving point on the ray.

### The direction of cell changes: `step_x`, `step_y`

If `r.x > 0`, crossing a vertical boundary increments the column. If `r.x < 0`, it decrements the column. Thus `step_x` is either `+1` or `-1`. The same applies to rows and `step_y`.

These values say which neighboring cell to enter. They do not say how long it takes to get there.

### The interval between vertical crossings: `delta_x`

Start from the ray equation `x = P.x + t*r.x`. Between two consecutive vertical boundaries, x changes by exactly one world unit in magnitude:

```text
|change in x| = |change in t * r.x| = 1

change in t = 1 / |r.x|

delta_x = abs(1 / r.x)
```

Similarly:

```text
delta_y = abs(1 / r.y)
```

These are **intervals in the shared parameter `t`**. They are not x and y distances being compared against each other.

For `r = (1, 0.4)`, advancing x by one unit takes `1` unit of `t`, while advancing y by one unit takes `2.5` units of `t`. This explains why the ray crosses more vertical grid lines than horizontal grid lines.

### The first appointments: `side_x`, `side_y`

The first boundary usually lies less than one whole cell away. Find its coordinate first:

```text
if step_x > 0: boundary_x = map_x + 1
else:          boundary_x = map_x
```

Then compute its appointment:

```text
side_x = abs(boundary_x - P.x) * delta_x
side_y = abs(boundary_y - P.y) * delta_y
```

The subtraction measures how far to move along that coordinate. Multiplication by `delta` converts it to the common `t` scale.

For `P = (1.5, 1.5)` and `r = (1, 0.4)`, both first boundaries are half a world unit away along their respective axes:

```text
side_x = 0.5 * 1.0 = 0.50
side_y = 0.5 * 2.5 = 1.25
```

The horizontal coordinate reaches its boundary first: `0.50 < 1.25`. Therefore the ray crosses a **vertical line**, enters the next column, and keeps the same row.

There is a naming trap here: an **X crossing means crossing a line of constant x**. On the map, that line is vertical. A Y crossing is a horizontal line.

### How appointments advance

When X wins, save `side_x` as the current crossing parameter, then add `delta_x` to schedule the next vertical crossing. Leave `side_y` alone because its appointment has not happened yet.

**`side_x` and `side_y` always refer to future crossings measured from the original ray origin. They are not distances remaining from the current crossing.** You do not subtract the winning time from the other value.

## 7. A complete numerical walkthrough

This is the default example in the interactive explorer. It is an illustrative map, separate from `maps/001.cub`:

```text
       x: 01234567
y = 0     11111111
    1     10000001
    2     10010001
    3     10010001
    4     10010001
    5     10000001
    6     11111111
```

Start at `P=(1.5,1.5)` in cell `(1,1)`. Use ray `r=(1,0.4)`. The first relevant wall is cell `(3,2)`.

```text
step_x = +1       step_y = +1
delta_x = 1       delta_y = 2.5
side_x = 0.5      side_y = 1.25
```

| Crossing | Compare before crossing | Winner and saved `t` | Entered cell | Exact crossing point | Next `side_x`, `side_y` |
|---|---|---|---|---|---|
| 1 | 0.50 < 1.25 | X, `t=0.50` | `(2,1)` floor | `(2.0,1.7)` | `1.50, 1.25` |
| 2 | 1.50 > 1.25 | Y, `t=1.25` | `(2,2)` floor | `(2.75,2.0)` | `1.50, 3.75` |
| 3 | 1.50 < 3.75 | X, `t=1.50` | `(3,2)` wall | `(3.0,2.1)` | `2.50, 3.75` |

Notice crossing 2: only the row changes. The world x-coordinate is `2.75`, but the cell column is still `floor(2.75)=2`.

Notice crossing 3: the wall is reached at `t=1.5`. The code has already changed `side_x` to `2.5`, the next hypothetical crossing. This is why it saves the old value in `hit.distance` before adding `delta_x`.

The final hit is calculated by substituting `t=1.5` into the ray equation:

```text
hit.x = 1.5 + 1.0 * 1.5 = 3.0
hit.y = 1.5 + 0.4 * 1.5 = 2.1
```

No tiny spatial increments were needed. The algorithm inspected three newly entered cells.

Try covering the final table row and predicting it from row 2. You only need to compare two numbers and update one axis.

## 8. Reading the actual DDA code

Now open [raycast.c](source/raycast.c).

`_raycast_setup` establishes the current cell, the crossing intervals, the step signs, and the first appointments. This expression:

```c
rc->state.map_x + (rc->state.step_x > 0)
```

is the first boundary coordinate. The comparison evaluates to `1` when moving right and `0` when moving left. It implements the boundary rule from section 6.

The heart of `_raycast_advance` is:

```c
if (rc->state.side_x < rc->state.side_y)
{
    rc->hit.distance = rc->state.side_x;
    rc->state.side_x += rc->state.delta_x;
    rc->state.map_x += rc->state.step_x;
    rc->state.side = RAYSIDE_X;
}
```

Read the assignments in this order:

1. Remember the crossing we are making now.
2. Schedule the next crossing of this kind.
3. Enter the neighboring cell.
4. Remember which kind of boundary we crossed.

The `else` block does the same for Y. Neither block changes `P` or `r`; both remain fixed throughout the cast.

`raycast_cast` repeatedly advances, checks that the new cell is within the grid, and checks its contents. Only `T_FLOOR` permits continuation; any other in-bounds tile stops the ray. On a validated map, walls surround the walkable area.

`_raycast_is_hit` computes `P + r * hit.distance`, stores the side, and returns success. The public result is `1` for a hit and `0` for failure to find an in-bounds hit.

The `width + height` iteration limit bounds traversal: a straight ray has fixed signs on its x/y components, so its cell indices move monotonically. It cannot turn around and revisit arbitrary cells.

The starting cell is not checked by the loop. The parser and movement logic establish that the player starts and remains on walkable floor.

## 9. Special directions and corners

**A ray moving left or up:** the first boundary is the current cell's left or top edge. For `P=(3.2,2.7)` and `r=(-0.8,0.6)`:

```text
starting cell = (3,2)
delta_x = 1.25              delta_y ≈ 1.6667
first x boundary = 3       first y boundary = 3
side_x = 0.2*1.25 = 0.25  side_y ≈ 0.3*1.6667 = 0.50
```

X wins. The new cell is `(2,2)` and the crossing point is `(3.0,2.85)`. At the exact boundary, the entered cell is chosen according to the traversal direction; blindly flooring that boundary point would not describe the left-hand cell.

**A zero component:** if `ray_x=0`, x never changes, so there is no future vertical crossing. The code uses `1e30` as a practical stand-in for infinity and avoids division by zero. Y crossings win. The corresponding rule applies to `ray_y=0`. A valid camera ray cannot have both components zero.

**Starting exactly on a grid boundary:** a first crossing parameter can be zero. When moving left from an integer x-coordinate, DDA immediately enters the cell to the left. This is consistent with traveling from that boundary into that neighboring cell.

**An exact corner crossing:** `side_x == side_y`. The code's strict `<` chooses Y first. If it continues, the next advance chooses X at the same parameter, entering the diagonal cell. This is a particular tie convention, not a test of every cell that touches the corner. A different application might require a different corner policy. Floating point arithmetic can also make theoretically equal values differ slightly.

## 10. Making a field of view

Read [parser.c](source/parser/parser.c), [render_fov.c](source/render/render_fov.c), and `_render_3d_ray` in [render_wall.c](source/render/render_wall.c).

The player stores a unit facing vector `d` and a perpendicular vector `p`, called `plane`:

```text
d = (dir_x, dir_y)
p = (-dir_y, dir_x) * 0.66
```

The perpendicular relation can be checked with a dot product:

```text
d dot p = dir_x*(-dir_y*0.66) + dir_y*(dir_x*0.66) = 0
```

The plane vector has length `0.66`. It is the **half-width** of an imaginary screen one unit ahead, not the full screen width. The full segment is `1.32` units wide.

Place the imaginary screen's center at `C=P+d`. Its points are `C+u*p` for `-1 <= u <= 1`. A ray from P through such a point has direction:

```text
r = (C + u*p) - P = d + u*p
```

This is the code's `direction + camera_x * plane` formula. The plane vector can be drawn anchored at the player to show its orientation, but the imaginary screen segment that the rays pass through is centered ahead at `P+d`.

For an east-facing player, `d=(1,0)` and `p=(0,0.66)`:

| `camera_x` / `u` | Ray | World direction |
|---|---|---|
| -1 | `(1,-0.66)` | right and up; left screen edge |
| 0 | `(1,0)` | straight east; screen center |
| +1 | `(1,+0.66)` | right and down; right screen edge |

The horizontal field of view is `2*atan(0.66) ≈ 66.85°`, because the screen's half-width divided by its distance from P is `0.66/1`.

The code uses:

```text
camera_x = 2*i/(RENDER_FOV_RAYS - 1) - 1
```

It samples the straight camera segment uniformly. Those samples are not uniformly spaced in angle. Uniform screen sampling is what a flat perspective screen needs.

The current constant is **260 rays**, and the frame is **1024 pixels wide**. Each ray fills a strip of three or four pixel columns. An earlier general explanation of one ray per pixel column does not describe this checkout's current sampling. With an even number of rays, no sample is exactly `camera_x=0`; the two middle rays straddle the center.

## 11. From a hit to a wall strip

DDA supplies a hit point, an entry side, and a parameter. Projection decides what to draw.

Perspective uses similar triangles. If a wall has world height `h`, forward depth `z`, and the camera has focal scale `f` pixels, its projected height is:

```text
projected_height = f * h / z
```

Twice the depth produces half the height. This project's convention is unit-height walls and a vertical focal scale equal to the frame height:

```text
column.height = int(frame.height / distance)
column.top    = frame.height/2 - column.height/2
```

This centers the wall around the horizon, corresponding to an eye halfway up the wall. It clamps drawing to the image bounds and clamps very small depth to `RENDER_MIN_DISTANCE=0.05`.

The general horizontal focal scale implied by the camera plane would be `frame.width/(2*0.66)`. This implementation uses `frame.height` for wall scaling instead; at 1024-by-768 these scales are close, but they are different conventions. Preserve that distinction when explaining the exact formula in the code.

The background fills the upper half with ceiling color and the lower half with floor color. The wall strip then overwrites its visible interval. This renderer does not cast textured floor or ceiling rays.

## 12. Why the distance avoids fisheye distortion

This is optional on a first reading. It explains why `hit.distance` has a useful meaning even though camera rays are not normalized.

The ray is `r=d+u*p`. Since `d` has length 1 and `p` is perpendicular to it:

```text
r dot d = (d + u*p) dot d = 1 + u*0 = 1
```

At the hit, the displacement from the player is `H-P=t*r`. Its depth measured along the facing direction is:

```text
z = (H-P) dot d = t*(r dot d) = t
```

Thus the parameter saved by this DDA is already the forward depth needed for projection. The Euclidean length along the slanted ray would be `t*length(r)`.

For an east-facing camera looking at a flat wall at `x=5`, starting at `x=1.5`, every camera ray has `r.x=1`. All hits have `t=3.5`, so all wall strips have the same height. Side rays travel a greater Euclidean length, but they do not make that flat wall shrink at its edges.

The proof depends on the unit direction and perpendicular camera plane. For an arbitrary input ray, the raycaster simply returns its parameter; it cannot promise that parameter is camera depth. If you normalize the camera rays before casting, the parameter changes meaning and the projection would need a corresponding depth correction.

## 13. Textures

Read [texture.c](source/texture.c) and `_render_column_setup` / `_render_wall` in [render_wall.c](source/render/render_wall.c).

The entry side and ray sign identify the visible wall face:

| Boundary crossed | Ray sign | Texture face |
|---|---|---|
| X / vertical line | `ray_x > 0` | West |
| X / vertical line | `ray_x < 0` | East |
| Y / horizontal line | `ray_y > 0` | North |
| Y / horizontal line | `ray_y < 0` | South |

Moving east reaches the west face of a wall. The face name describes the wall surface's outward direction, not the player's facing direction.

For an X-side hit, x is fixed on the wall boundary, while y identifies the position along the wall. For a Y-side hit, use x instead. Taking its fractional part produces a texture coordinate between 0 and 1:

```text
offset = coordinate_along_wall - floor(coordinate_along_wall)
tex_x = int(offset * texture.width)
```

Our example hits `(3,2.1)` on an X side. Its offset is approximately `0.1`, before the orientation flip. Some faces reverse `tex_x` to keep textures oriented consistently.

For each visible screen row:

```text
tex_y = int((screen_y - unclipped_column_top)
            * texture.height / full_column_height)
```

Using the unclipped top keeps the mapping correct when a nearby wall extends beyond the image. The renderer samples one texture column and stretches it over the wall's projected height.

## 14. Movement, rotation, and collision

Read [player.c](source/player.c).

Forward/backward and left/right keys become signed values `fw` and `st`. The pair is divided by `sqrt(fw²+st²)` so diagonal input does not move faster. With both forward and left held, this changes `(1,1)` to approximately `(0.707,0.707)`.

The forward vector is `(dir_x,dir_y)`. The left vector is `(dir_y,-dir_x)`. Their weighted sum is multiplied by `PLAYER_SPEED * elapsed`. Speed is 2.5 world units per second. Elapsed time is capped at 0.05 seconds per update to limit large jumps.

Collision uses four corners at `(x±0.2,y±0.2)`. Although the constant is named `PLAYER_RADIUS`, the implemented footprint is an axis-aligned square. Each tested corner must belong to a floor cell. X and Y movement are attempted separately, allowing sliding along walls.

Rotation applies the same matrix to both direction and plane:

```text
x' = x*cos(angle) - y*sin(angle)
y' = x*sin(angle) + y*cos(angle)
```

Both vectors rotate together, preserving their lengths and perpendicular relationship mathematically, subject to floating point rounding. Because y points down, positive angles turn clockwise on the displayed map. Turning speed is 1.8 radians per second.

## 15. The rest of the program

### Loading and validating the map

The [parser](source/parser/parser.c) reads the four texture paths and two RGB colors, then the map. Configuration flags reject duplicates and ensure required fields exist. RGB components must be integers between 0 and 255 and are packed as `(red<<16)|(green<<8)|blue`.

Rows are padded with spaces to form a rectangular array. Validation accepts only supported characters, requires one player, and checks that each walkable cell's four direct neighbors are walkable or wall. Spaces and out-of-bounds neighbors make a walkable edge invalid.

[parser_connectivity.c](source/parser/parser_connectivity.c) uses a queue to visit four-connected walkable cells from the player. It compares the reachable count against the total walkable count. This implementation rejects disconnected walkable regions, even when those regions are separately enclosed.

Finally, the player letter is replaced by floor, and the position, unit direction, and perpendicular plane are initialized.

### Graphics and the frame loop

```text
main
  runtime_args
  runtime_setup
    zero the game structure
    parser_controller
    setup_game
      initialize MiniLibX and window
      load four XPM textures
      allocate image buffer
      register hooks
  runtime_run
    MiniLibX event loop
      key events update held-key state
      game_frame updates player and renders when needed
    runtime_cleanup
```

[game.c](source/game.c) updates the player when motion keys are held and redraws when needed. [screen.c](source/screen.c) draws either the first-person view plus a minimap overlay, or the full map view. It sends the completed image to the window with `mlx_put_image_to_window`.

The minimap and first-person renderer each cast their own set of rays. They share the algorithm and camera construction; the current code does not cache one set for both views.

Cleanup destroys textures, frame, window, and display, and frees parsed map data. Zero-initialized pointers permit cleanup after partial setup failure.

| Input | Action |
|---|---|
| W / up arrow | Forward |
| S / down arrow | Backward |
| A / D | Strafe left / right |
| Left / right arrows | Rotate |
| M | Toggle map mode |
| Escape / close window | Exit |

## 16. Source reading route

Read one purpose at a time:

| Order | Source | Question to answer |
|---|---|---|
| 1 | [parser.c](source/parser/parser.c), `_parser_apply_player` | Where do position and camera vectors come from? |
| 2 | [render_draw.c](source/render/render_draw.c) | How does a world point become pixels? |
| 3 | [render_view.c](source/render/render_view.c) | How are tiles and player drawn? |
| 4 | [raycast.c](source/raycast.c), `_raycast_setup` | What are the first two appointments? |
| 5 | [raycast.c](source/raycast.c), `_raycast_advance` | Which crossing wins, and what changes? |
| 6 | [raycast.c](source/raycast.c), remaining functions | When does traversal stop? |
| 7 | [render_fov.c](source/render/render_fov.c) | How are many rays generated and shown? |
| 8 | [render_wall.c](source/render/render_wall.c) | How does a hit become a wall strip? |
| 9 | [texture.c](source/texture.c) | Which wall surface supplies its texture? |
| 10 | [player.c](source/player.c) | How do movement and rotation change the camera? |
| 11 | [runtime.c](source/runtime.c), [game.c](source/game.c), [screen.c](source/screen.c) | How are these pieces scheduled? |

## 17. Questions and answers for self-checking

Try one question at a time. Predict before revealing its answer.

<details><summary>1. Why can DDA skip the points between two consecutive boundary crossings?</summary>

The ray remains within one cell throughout that open interval. The map cell being inspected cannot change until a boundary is crossed.

</details>

<details><summary>2. A ray has ray_x=0.25. What is delta_x?</summary>

`delta_x=4`. It takes four units of the shared parameter to change x by one world unit.

</details>

<details><summary>3. side_x=0.8, side_y=1.3, delta_x=2. What happens next?</summary>

X wins. Save `t=0.8`; change `side_x` to `2.8`; change `map_x` by `step_x`; keep `side_y=1.3` and `map_y` unchanged. Then inspect the newly entered cell.

</details>

<details><summary>4. Why not subtract 0.8 from side_y in that example?</summary>

Both side values are appointments measured from the original ray origin. `1.3` is still the correct absolute appointment. Subtracting only from Y would mix two reference points.

</details>

<details><summary>5. Why is an X-side wall vertical on the map?</summary>

Its boundary has constant x. Moving along the boundary changes y, so it appears as a vertical line in the top-down map.

</details>

<details><summary>6. Does the ray turn when DDA changes from an X crossing to a Y crossing?</summary>

No. `P` and `r` remain fixed. DDA changes which grid cell it is inspecting along that one straight ray. The sequence of cell indices can look like stairs, but the ray is straight.

</details>

<details><summary>7. Why is hit.distance saved before incrementing side_x or side_y?</summary>

It must describe the crossing happening now. After the increment, that side value describes the next boundary of the same kind.

</details>

<details><summary>8. Is DDA responsible for wall height or texture sampling?</summary>

No. DDA supplies the hit. Projection uses its depth for height; texture mapping uses its position and side for image sampling.

</details>

<details><summary>9. Why does normalizing camera rays change the interpretation of hit.distance?</summary>

Scaling a ray vector changes how much displacement one unit of parameter produces. For a unit ray, t is ray length. For this project's unnormalized camera ray, t is forward depth. The world line stays the same, but its parameter scale changes.

</details>

## 18. Reference sheet

| Name | Meaning |
|---|---|
| `pos_x`, `pos_y` | Fixed ray origin for one cast |
| `ray_x`, `ray_y` | Fixed ray vector for one cast |
| `map_x`, `map_y` | Currently inspected cell indices |
| `step_x`, `step_y` | Which adjacent column or row to enter |
| `delta_x`, `delta_y` | Parameter intervals between crossings of the same kind |
| `side_x`, `side_y` | Parameters of the next X and Y crossings, from the original origin |
| `hit.distance` | Parameter of the crossing just taken; forward depth for this camera construction |
| `hit.side` | Whether the hit entered through an X or Y boundary |
| `hit.x`, `hit.y` | Exact world hit position, `P + r*t` |

```text
1. Find the starting cell.
2. Schedule the first X and Y crossings.
3. Choose the earlier crossing.
4. Save its parameter, advance its schedule, enter its neighboring cell.
5. If the cell is floor, repeat. If it is a wall, return the hit.
```

For further reading after the worked example: [Lode's raycasting tutorial](https://lodev.org/cgtutor/raycasting.html), [Permadi's tutorial](https://permadi.com/1996/05/ray-casting-tutorial-table-of-contents/), and the [SplashKit explanation](https://splashkit.io/guides/physics/8-3d-projection-raycasting/). The code in this repository remains the reference for implementation-specific choices.
