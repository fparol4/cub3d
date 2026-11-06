#!/usr/bin/env python3
"""Build an offline, printable walkthrough of this cub3D source snapshot.
Run from any directory: python3 docs/build_flow_presentation.py
Print the resulting HTML with Chrome or a browser's Save as PDF (A4 landscape).
"""
from pathlib import Path
import re
import html
import subprocess

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / 'docs'
esc = html.escape
pages = []
def code(s): return '<pre>' + esc(s.strip()) + '</pre>'
def p(s): return '<p>' + s + '</p>'
def bullets(*xs): return '<ul>' + ''.join('<li>'+x+'</li>' for x in xs) + '</ul>'
def table(headers, rows):
    return '<table><thead><tr>'+''.join('<th>'+x+'</th>' for x in headers)+'</tr></thead><tbody>'+''.join('<tr>'+''.join('<td>'+x+'</td>' for x in row)+'</tr>' for row in rows)+'</tbody></table>'
def flow(*nodes):
    return '<div class="flow">'+ '<span class="arrow">→</span>'.join('<div class="node">'+x+'</div>' for x in nodes)+'</div>'
def cols(a,b): return '<div class="cols"><div>'+a+'</div><div>'+b+'</div></div>'
def slide(title, body, note, source='', tag='WALKTHROUGH'):
    pages.append(dict(title=title,body=body,note=note,source=source,tag=tag))

slide('cub3D: every function, one connected story',
      '<div class="hero">From a <em>.cub</em> file<br>to a textured first-person frame.</div>'+p('Presentation preparation • C / Linux / MiniLibX • source-based walkthrough')+flow('Load & validate','Create graphics','Process input','Cast & draw','Release resources'),
      'Opening: “We store a 2D grid, then find the first wall along each camera ray and project that hit as a textured vertical strip.”',
      'Scope: all functions under source/, including static helpers; library boundaries and parser test harness. Library internals are summarized where relevant.')
slide('How to use this pre-presentation',
      table(['Part','What you should be able to explain'],[
          ('01 • Program and data','Startup, state ownership, map parsing, setup, and callbacks.'),
          ('02 • One interactive frame','Key state, time steps, collision, camera vectors, DDA, textures, and minimap.'),
          ('03 • Defense and rehearsal','Failure paths, cleanup, complexity, test evidence, and likely questions.'),
          ('04 • Complete function reference','Every project function: signature, source line, purpose, callers, and direct calls.')])+p('Suggested delivery: 25–35 minutes for the flow walkthrough; allow another 20–30 minutes for the mathematical derivations. Use the reference pages to answer implementation questions. Printed page numbers and the contents page locate each module.'),
      'Rehearse the main story first. Then choose any function in the appendix and explain how its input came from the preceding stage.', 'Primary authority: source/*.c, source/parser/*.c, source/render/*.c and header/core/*.h.')
# Filled once all appendix pages exist.
contents_index = len(pages)
slide('Navigation & source map','', 'Follow the walkthrough in order; jump to a module reference when an evaluator asks for a specific helper.',tag='CONTENTS')
slide('The complete lifecycle',
      code('main(argc, argv)\n ├─ runtime_args()                         failure → return 1\n ├─ runtime_setup(&g, argv[1])              failure → cleanup → return 1\n │   ├─ zero-initialize t_game\n │   ├─ parser_load_map()\n │   └─ setup_game()\n └─ runtime_run()\n     ├─ mlx_loop() → events + game_frame()\n     ├─ runtime_cleanup() after loop ends\n     └─ return g.state.exit_status')+p('The stack variable <code>g</code> remains alive throughout the event loop. MiniLibX callbacks receive its address. Normal close requests end the loop; cleanup runs when control returns.'),
      'Trace a successful run before discussing details. Emphasize that runtime_close requests a stop; it does not free the game while callbacks are active.', 'source/main.c; source/runtime.c')
slide('The shared state and its ownership',
      table(['Owner / type','Contents','Lifetime / role'],[
          ('t_game.map / t_map','grid, dimensions, four allocated paths, packed colors','Parser owns allocations until transfer; game owns them afterward.'),
          ('t_game.player / t_player','position, unit direction, perpendicular camera plane','Mutable runtime values; spawn marker becomes floor.'),
          ('t_game.graphics / t_graphics','MLX context, window, frame, four texture images','Created after parsing; image data addresses belong to MLX images.'),
          ('t_game.state / t_game_state','keys[], view, dirty flag, last time, exit status','Zero starts V_3D, no keys, normal exit status.'),
          ('Temporary calculation types','t_raycast, t_ray_state, t_ray_hit, t_column','Per-ray setup, stepping, hit result, and projected strip.'),
          ('Drawing / parsing types','t_point, t_line, t_viewport; parser data/reader/line','Pixel geometry; temporary config and buffered input state.')]),
      'Distinguish owning an MLX image pointer from borrowing its pixel address. Never free addr independently.', 'header/core/struct.h; header/source/parser.h; header/core/enum.h')
slide('Parsing: a transactional pipeline',
      flow('parser_load_map','parser_read_file','rectangularize','validate','transfer to g')+bullets(
          '<code>runtime_args</code> requires exactly one argument ending in lowercase <code>.cub</code>. The reader independently checks that suffix.',
          '<code>parser_load_map</code> builds a zeroed local <code>t_parser_data</code>. It returns 0 on success, 1 after reporting an error.',
          'Reader, configuration, and validator helpers generally use 1 = success and 0 = failure. The line reader has a separate three-state contract.',
          'Failure frees local allocations. Success assigns <code>g-&gt;map = data.map</code>, clears the local map, replaces the spawn with <code>0</code>, and initializes the player.'),
      'Explain the return-value convention change explicitly. A leading ! in parser_load_map converts helper failure into the outer error path.', 'source/parser/parser.c; parser_reader.c; source/runtime.c')
slide('Buffered input and line ownership',
      code('parser_read_file → _parser_read_lines → parser_next_line\n    → _parser_collect_line → _parser_read_byte / _parser_append_byte\n\nparser_next_line:  1 = allocated line, 0 = EOF, -1 = error\nparser_process_line: 1 = accepted, 0 = rejected\ninternal content status: 2 = accepted map line, ownership retained')+bullets(
          'The reader fetches blocks of 4096 bytes and retries interrupted reads (EINTR). Each line starts at 128-byte capacity and doubles when needed.',
          'Newline bytes are consumed; a NUL terminator is added. Embedded NUL input is rejected. A final line without newline is supported.',
          'Configuration and blank lines are freed after processing. A stored map line remains owned by the grid. The trimmed inspection copy is always freed.',
          'The file is closed even when line processing fails. A close failure also rejects the read.'),
      'Point to the status 2 convention: it prevents freeing a string that was just stored in the map.', 'source/parser/parser_line_reader.c; parser_reader.c; parser_lines.c')
slide('The parser is a small state machine',
      flow('BEFORE MAP<br>config or blank','IN MAP<br>store original rows','AFTER MAP<br>blank lines only')+
      table(['Input','Action'],[
          ('Blank before map','Accept without starting the map.'),
          ('First map-like line','Set map_started; append original text. NO/SO/WE/EA prefixes remain config candidates.'),
          ('Nonblank once map started','Store as a row; the later character scan rejects configuration or illegal characters.'),
          ('Blank after map started','Set map_finished. Any later nonblank input fails.'),
          ('Short row','At EOF pad with spaces to maximum width. Spaces remain void, never floor.')])+p('Trimming is for classification. Leading spaces in the actual map are preserved. Tabs in a stored map row are rejected by character validation.'),
      'Use a blank line inside a maze as the example: the blank ends the map, so a later row is invalid.', 'source/parser/parser_lines.c; parser_map_lines.c; parser_grid.c')
slide('Configuration: six required fields',
      cols(code('NO textures/north.xpm\nSO textures/south.xpm\nWE textures/west.xpm\nEA textures/east.xpm\nF 50, 50, 50\nC 110, 160, 210'),bullets(
          'A bitmask records NO, SO, WE, EA, F and C. Repeated fields fail; configuration order is flexible before the map.',
          'Texture identifiers require space or tab, followed by a nonempty trimmed path. Parsing stores paths; image loading happens later.',
          'RGB requires exactly three unsigned decimal values, each 0–255, separated by commas. Extra characters or components fail.',
          'Packing: <code>(red &lt;&lt; 16) | (green &lt;&lt; 8) | blue</code>. Floor 50,50,50 becomes <code>0x323232</code>.')),
      'A valid configuration string does not prove the image can load. Explain the second validation boundary in texture_load.', 'source/parser/parser_config.c; parser_color.c; header/core/enum.h')
slide('Map validation: what is actually guaranteed',
      flow('All six fields','At least 3 × 3','Legal cells + 1 spawn','Check floor neighbors')+bullets(
          'Allowed stored cells: <code>0</code>, <code>1</code>, space, and <code>N/S/E/W</code>. Exactly one player marker must exist.',
          'Each walkable cell must have four cardinal neighbors that are wall or walkable. Outside the grid reads as space, so exposed floor fails.',
          'Every walkable cell is inspected, including rooms unreachable from the player. Disconnected enclosed rooms are accepted.',
          'There is no flood fill or reachability requirement in this implementation. README connectivity wording is stale; current tests explicitly accept disconnected regions.'),
      'Say “enclosure of every walkable cell” precisely. Do not claim that all floor cells must connect to the player.', 'source/parser/parser_validate.c; parser_cells.c; tests/valid_maps/FF_*.cub')
slide('Spawn and coordinate conventions',
      cols(table(['Spawn','Direction (x,y)','Camera plane (x,y)'],[
          ('N','(0, -1)','(0.6, 0)'),('S','(0, 1)','(-0.6, 0)'),('E','(1, 0)','(0, 0.6)'),('W','(-1, 0)','(0, -0.6)')]),code('World x increases to the right.\nWorld y increases downward.\nGrid access: grid[y][x]\n\npos = (cell_x + 0.5, cell_y + 0.5)\nplane = (-dir_y, dir_x) * 0.6'))+p('In <code>maps/001.cub</code>, the spawn E is at zero-based cell (3,2), so position = (3.5,2.5). The grid becomes ordinary floor at that position. The horizontal field of view is approximately 2 × atan(0.6) = 61.9°.'),
      'Draw the camera direction and the perpendicular plane. The player is centered inside a tile, not located at a grid intersection.', 'source/parser/parser.c; maps/001.cub; header/core/const.h')
slide('Graphics setup and callback registration',
      flow('_setup_mlx','texture_load','_setup_frame','_setup_hook')+
      table(['Resource / callback','Implementation'],[
          ('Context and window','mlx_init; mlx_new_window at 1024 × 768.'),
          ('Four textures','XPM images; require an address, positive dimensions, and 32 bpp.'),
          ('Frame','mlx_new_image; borrow addr/bpp/stride/endian; clear its bytes.'),
          ('Window hooks','Key press/release, focus out, expose, and window close.'),
          ('Loop hook','game_frame registered with mlx_loop_hook; all callbacks receive g.')])+p('A failed stage short-circuits setup. The caller uses the same cleanup routine for partially created resources. Hook registration stores function pointers; it does not run the callbacks immediately.'),
      'Make the transition from sequential startup to event-driven execution explicit.', 'source/setup.c; texture.c; library/mlx/mlx_hook.c; mlx_loop_hook.c')
slide('Events change state; frames use that state',
      table(['Event','Function and effect'],[
          ('W/S or up/down; A/D; left/right','common_key_id maps to logical motion; hook_press sets keys[id], hook_release clears it. Changes request redraw.'),
          ('M press','Toggle only if GK_VIEW was previously clear. Latch until release; screen_toggle_view changes V_3D/V_MINIMAP and marks dirty.'),
          ('Escape or window close','runtime_close calls mlx_loop_end; cleanup follows after mlx_loop returns.'),
          ('Focus lost','hook_focus_out clears every logical key to prevent stuck movement.'),
          ('Expose / window refresh','hook_expose requests redraw.')])+p('Callbacks are dispatched through the MiniLibX/X11 event loop. Motion is continuous because the loop callback reads held-key state, rather than moving only on a key press.'),
      'Mention a current edge case: W and up share one boolean. Holding both and releasing either clears forward until a later press updates it.', 'source/hook.c; common.c; library/mlx/mlx_loop.c')
slide('One frame: timing and dirty rendering',
      code('game_frame(g)\n  if no motion and no redraw: return\n  now = common_now()                         error → status=1 → close\n  elapsed = 0 on first frame, otherwise now - last_frame_time\n  last_frame_time = now; clamp elapsed into [0, 0.05]\n  if motion: save player → player_update → compare → mark dirty\n  if dirty: screen_clear → screen_render → clear dirty flag')+
      p('At 2.5 world units/second, a clamped step travels at most 0.125 units before collision. Rotation is 1.8 radians/second. Idle frames avoid redraw work, though the MiniLibX loop still runs.')+
      p('The timestamp is not updated on the early idle return. The first update after a long idle can use the maximum 0.05-second step. Timing uses gettimeofday, so negative deltas are clamped.'),
      'Explain both why time scaling avoids frame-rate-dependent speed and why the cap bounds large pauses. The cap does not provide fixed-step simulation.', 'source/game.c; common.c; header/core/const.h')
slide('Movement, collision and rotation',
      cols(code('forward = forward_key - backward_key\nstrafe  = left_key - right_key\nnormalize(forward, strafe)\n\nmove_x = (dir_x*forward + dir_y*strafe)\n         * 2.5 * elapsed\nmove_y = (dir_y*forward - dir_x*strafe)\n         * 2.5 * elapsed'),bullets(
          'Normalize input so diagonal movement has the same total speed as moving straight.',
          'Test four corners at (x ± 0.2, y ± 0.2). Outside bounds or anything other than floor blocks movement.',
          'Try the X move first, then Y using the possibly updated X. This allows sliding along a wall.',
          'Move before turning. Rotate direction and camera plane by the same 2D matrix; left uses a negative angle and right a positive angle.'))+
      p('Rotation: <code>x′ = x cos(a) − y sin(a)</code>, <code>y′ = old_x sin(a) + y cos(a)</code>. Saving old_x avoids using an already changed value.'),
      'Collision is a four-corner square footprint, not an exact circular collision test. Opposite forward/back keys cancel.', 'source/player.c')
slide('Camera rays: direction plus a screen offset',
      flow('ray index i','camera_x in [-1,1]','dir + plane × camera_x','DDA hit')+
      code('camera_x = 2.0 * i / (300 - 1.0) - 1.0\nray_x = player.dir_x + player.plane_x * camera_x\nray_y = player.dir_y + player.plane_y * camera_x')+
      bullets('300 rays cover a 1024-pixel view. Ray i draws pixels from <code>i*width/300</code> up to <code>(i+1)*width/300</code>, excluding the end.',
              'For an east-facing player, left edge ray = (1,-0.6), right edge = (1,0.6). With 300 samples, there is no exact center sample; rays 149 and 150 straddle it.',
              'Camera rays are deliberately unnormalized. Their DDA parameter is depth along the unit forward direction, which supplies the projection distance without an extra cosine correction.'),
      'Derivation: dir dot plane = 0 and |dir| = 1, so dir dot ray = 1. For hit = pos + t*ray, forward depth is t.', 'source/raycast_camera.c; source/render/render_wall.c')
slide('DDA: visit grid boundaries in order',
      cols(code('_raycast_setup:\n  map = floor(player position)\n  delta_x = abs(1/ray_x)\n  delta_y = abs(1/ray_y)\n  step = sign of ray components\n  side = time to first X/Y boundary\n\n_raycast_advance:\n  choose smaller side time\n  record time, add delta, step one cell'),bullets(
          'A zero ray component uses 1e30 as the unreachable boundary distance.',
          'Crossing X enters a vertical grid boundary (RAYSIDE_X); crossing Y enters a horizontal boundary (RAYSIDE_Y). Equal times take Y.',
          'After each step, reject out-of-bounds access; floor continues, any other cell records a hit.',
          'Stop at the first hit or after width + height steps. Hit includes world x/y, side, and the crossing parameter.'))+
      p('The ray moves monotonically through the grid. Choosing the next closest boundary avoids tiny fixed-distance sampling and does not skip a wall cell along the traversed path.'),
      'Do not confuse delta with distance to the first boundary: delta is the spacing between later boundaries on the same axis.', 'source/raycast.c; header/core/struct.h')
slide('A worked ray from the supplied map',
      cols(code('maps/001.cub, spawn (3.5,2.5), E\nActual camera sample i = 149:\n  camera_x = -1/299\n  ray = (1, -0.6/299)\n  delta_x = 1; delta_y ≈ 498.333\n  side_x = 0.5; side_y ≈ 249.167'),table(['Boundary time','Entered cell','Result'],[
          ('0.5','(4,2)','Floor → continue'),('1.5','(5,2)','Floor → continue'),('2.5','(6,2)','Floor → continue'),('3.5','(7,2)','Wall → hit')]))+
      p('Hit ≈ (7.0, 2.49298), side X, depth 3.5. Positive ray_x chooses the west-facing wall texture. Projected height = int(768 / 3.5) = 219; top = 275. This sample fills screen columns 508–511.')+
      p('For comparison, a conceptual exact center ray (1,0) hits (7,2.5). It illustrates the geometry but is not one of the 300 samples.'),
      'Walk the table by hand. This ties the player state, DDA time, texture direction, and screen strip into a single concrete example.', 'maps/001.cub; raycast_camera.c; raycast.c; source/render/render_wall.c')
slide('A hit becomes a textured wall strip',
      flow('texture_for_hit','column geometry','texture coordinates','copy pixels')+
      table(['Calculation','Meaning'],[
          ('depth = max(hit.distance, 0.05)','Keep projection finite near a wall.'),
          ('height = int(frame.height / depth)','Near walls are tall; distant walls are short.'),
          ('top = H/2 − height/2; start/end clipped','Center the wall vertically, restrict writes to the screen.'),
          ('offset = fract(hit.y) for X; fract(hit.x) for Y','Position along the struck wall; tex_x = int(offset * texture.width).'),
          ('Flip tex_x if (X and ray_x&gt;0) or (Y and ray_y&lt;0)','Keep directional texture orientation consistent.'),
          ('tex_y = int((y − top) * texture.height / height)','Use the unclipped top to preserve the crop when the wall extends off screen.')]),
      'First paint ceiling and floor for each screen column, then overwrite the wall span. Texture lookup is nearest-neighbor with no lighting pass.', 'source/render/render_wall.c; source/texture.c')
slide('Which texture is visible, and where pixels go',
      cols(table(['Hit side','Ray sign','Texture face'],[
          ('X','ray_x &gt; 0','WE'),('X','ray_x &lt; 0','EA'),('Y','ray_y &gt; 0','NO'),('Y','ray_y &lt; 0','SO')]),code('pixel address = image.addr\n              + y * image.stride\n              + x * (image.bpp / 8)\n\n32-bit color copy:\n*(unsigned int *)dst = color;'))+
      bullets('Names describe the wall face, not the direction the player is looking. Moving a ray east hits a west-facing surface.',
              'stride is bytes per row and may include padding; width × 4 must not replace it blindly.',
              'render_px clips to image bounds. The wall renderer uses its own already-clipped loops and direct buffer writes.',
              'Textures explicitly require 32 bpp. Frame writes also assume 32-bit pixels; frame setup stores its format but does not validate bpp or convert endian.'),
      'Treat pixel format support as a current implementation assumption. The endian field is stored but is not used to transform colors.', 'source/texture.c; source/render/render_draw.c; render_wall.c; source/setup.c')
slide('Screen composition and the minimap',
      cols(code('screen_render\n ├─ V_3D: render_3d\n │        → _screen_overlay viewport\n ├─ V_MINIMAP: full-frame viewport\n ├─ render_map_view\n │   ├─ _render_map → each cell\n │   ├─ render_fov → 300 DDA rays\n │   └─ _render_player\n └─ mlx_put_image_to_window'),bullets(
          'Overlay preserves map aspect ratio inside a 256 × 256 limit, at top right with a 16-pixel margin.',
          'Full-map mode uses the entire 1024 × 768 viewport; independent X/Y scaling may stretch map cells.',
          'World-to-map conversion: viewport origin + world coordinate × viewport size / grid dimension.',
          'Draw order matters: cells first, orange FOV rays second, green player marker last. Border cells use a separate border fill when large enough.'))+
      p('The 3D pass casts 300 rays, then the minimap casts another 300. Full-map mode casts only the minimap set. Hits are recomputed rather than shared between passes.'),
      'Use the minimap as a debugging view: it displays the same camera and DDA calculations used by the 3D renderer.', 'source/screen.c; source/render/render_view.c; render_fov.c')
slide('Drawing primitives and complexity',
      table(['Function / stage','How it works','Cost'],[
          ('render_px','Bounds check, calculate byte address, write one color','O(1)'),
          ('render_rect','Half-open X and Y loops call render_px','O(rectangle area)'),
          ('render_line','Bresenham error accumulator; step X/Y toward endpoint','O(max(|dx|,|dy|))'),
          ('Map validation / padding','Inspect or copy the rectangular grid','O(map width × height)'),
          ('Appending map rows','Reallocate pointer array for every row','O(height²) pointer copies overall'),
          ('Ray traversal','Each ray crosses at most width + height cells','O(rays × (width + height))'),
          ('3D raster + minimap','Fill frame pixels, map cells, and FOV lines','O(frame area + grid area + ray traversal + line pixels)')])+p('Memory: grid O(width × height), four textures plus frame buffers, and bounded per-ray state. Dynamic line storage grows geometrically. Idle frames skip rendering; this is not a sleeping or frame-limited event loop.'),
      'Discuss optimization from measurements: caching minimap ray hits could remove duplicate traversal, but it is not implemented here.', 'source/render/render_draw.c; parser_map_lines.c; parser_grid.c; game.c; raycast.c')
slide('Failures, closing, and resource release',
      flow('Parse/setup failure OR close request','leave setup / loop','runtime_cleanup','return status')+
      table(['Path','Behavior'],[
          ('Bad arguments','Print Error + usage to stderr; return 1 without allocating game resources.'),
          ('Read / map validation failure','Free local parser map, print Invalid map file; runtime setup cleans zero/partial game.'),
          ('Graphics setup failure','Print the graphics/texture error; cleanup destroys only resources already created.'),
          ('Frame time or rendering failure','Set exit_status to 1 through common_error, call runtime_close.'),
          ('Escape / window close','Leave status 0; request loop end; runtime_run cleans afterward.')])+p('<b>Cleanup order:</b> texture images → frame image → window → display and context → grid rows / grid / texture paths. Graphics handles and map are reset. The context must outlive destruction of its images and window.'),
      'Explain why zero-initialization makes partial cleanup safe, and why parser transfer clears the old owner.', 'source/runtime.c; source/texture.c; source/parser/parser.c')
slide('Three end-to-end traces to rehearse',
      table(['Trigger','Exact journey'],[
          ('Hold W','MLX key event → hook_press → common_key_id → keys[GK_FORWARD]=1 → game_frame → player_update → _player_move → corner collision checks → changed state → screen_clear → screen_render → 3D + map → image blit.'),
          ('Press M','hook_press checks GK_VIEW latch → screen_toggle_view → view changes + dirty=1 → game_frame → screen_render chooses full-map viewport → render_map_view → image blit. Release clears latch.'),
          ('Malformed RGB','parser_next_line → parser_process_line → parser_parse_config → _parser_parse_color_config → parser_parse_color fails → stop file processing → close fd → parser_cleanup(local map) → common_error → runtime_cleanup(game) → main returns 1.')])+p('For each trace, name the data that changes, which function owns it, and what value signals success or failure.'),
      'An evaluator can enter the code at any function. Use these traces to reconnect it to the larger lifecycle.', 'source/hook.c; game.c; player.c; screen.c; source/parser/*.c')
slide('Build, tests and a practical demonstration',
      cols(code('make\n./cub3D maps/001.cub\nbash tests/test_maps.sh\n\nControls:\nW/S or up/down: forward/back\nA/D: strafe     left/right: turn\nM: map view    Escape: exit'),bullets(
          'Verified for this document: build completed; all 18 valid fixtures accepted; all 27 invalid fixtures rejected.',
          'The test script builds a parser-only checker. --textures checks path readability with access; it does not initialize MLX or decode XPM images.',
          'Tests cover required fields, RGB errors, illegal characters, spawn count, enclosure, ordering, extensions, and accepted disconnected regions.',
          'For a live presentation: walk, strafe into a wall, rotate, toggle M, release focus, then exit. These graphics interactions were not exercised by the parser tests.'))+
      p('Executable name is case-sensitive: <code>cub3D</code>, as defined in Makefile. The README example uses a different case. Vendored MLX emits a build warning; project sources compile with -Wall -Wextra -Werror.'),
      'Report parser checks as parser checks. Do not claim automated coverage of drawing, key events, or graphical resource teardown.', 'Makefile; tests/test_maps.sh; tests/map_check.c; README.md')
slide('Questions you should be ready to answer',
      table(['Question','Answer to rehearse'],[
          ('Why no fish-eye correction?','Unnormalized camera ray has dot(dir,ray)=1, so its DDA parameter is forward depth.'),
          ('Why does diagonal motion not speed up?','Forward/strafe inputs are normalized before scaling by speed and elapsed time.'),
          ('Why do short rows use spaces?','Missing cells must remain void so enclosure checks detect openings.'),
          ('Why copy the map into g and clear data.map?','Ownership transfers without duplicating allocations or freeing them twice.'),
          ('Why can the player slide along a wall?','X and Y moves are tested independently against the four-corner footprint.'),
          ('Why rotate both direction and plane?','They must keep the same camera orientation, perpendicularity and plane scale.'),
          ('Why does closing not immediately free g?','Callbacks request loop termination; runtime_run frees resources after control returns.'),
          ('Is the map validated with flood fill?','No. Every walkable cell is checked for cardinal enclosure; disconnected enclosed rooms pass.')]),
      'Answer from the actual code. If asked about alternatives, distinguish current behavior from possible future changes.', 'Walkthrough synthesis; complete function reference follows.')

math_start = len(pages) + 1
def mathslide(title, body, note, source):
    slide(title, body, note, source, 'MATHEMATICS / DERIVATIONS')
mathslide('Math foundations: vectors, cells and units',
    cols(code('p = (pos_x, pos_y)       world position\nd = (dir_x, dir_y)       unit forward vector\nc = (plane_x, plane_y)   camera-plane vector\nr = d + u*c             ray direction\nq(t) = p + t*r          point on that ray\n\n|v| = sqrt(v_x² + v_y²)\na dot b = a_x*b_x + a_y*b_y'),bullets(
        'One map tile is one world unit wide. World coordinates are continuous; grid indices are integers.',
        'Cell (i,j) occupies [i,i+1) × [j,j+1). floor maps a position to that cell; center is (i+0.5,j+0.5).',
        'floor(-0.2) = -1, while a C integer cast gives 0. Collision uses floor before rejecting negative indices.',
        'Distance, ray parameter, and pixel counts are different quantities. They agree only under specific scaling assumptions.')),
    'Define symbols before deriving anything. Here c is a vector, not the C color field; u is a dimensionless screen coordinate.',
    'header/core/struct.h; source/player.c; source/raycast.c')
mathslide('Why diagonal normalization and strafe signs work',
    code('a = forward_key - backward_key; b = left_key - right_key\nL = sqrt(a² + b²); if L == 0, no displacement\n(a,b) := (a/L, b/L)\nleft unit vector l = (d_y, -d_x)\nvelocity v = PLAYER_SPEED * (a*d + b*l)')+
    p('Because |d| = |l| = 1 and d dot l = 0, |a*d + b*l|² = a² + b² = 1. Thus |v| = 2.5 whether one key or a diagonal combination is held.')+
    p('Example: facing east d=(1,0), W+A gives (a,b)=(1/√2,1/√2). For dt=0.02 s: displacement ≈ (0.035355,−0.035355), whose length is 0.05. Without normalization its length would be 0.070711.')+
    p('The Y sign follows downward-positive coordinates: left of east points upward, l=(0,−1). Floating-point rotation can slightly change the unit-length invariant over long runs.'),
    'Show the Pythagorean calculation. The input vector is normalized; the code does not renormalize the direction after each turn.',
    'source/player.c:_player_move; _player_rotate')
mathslide('Rotation: where sine and cosine enter',
    cols(code('R(a) = [ cos(a)  -sin(a) ]\n       [ sin(a)   cos(a) ]\n\nv_new = R(a) * v_old\na = turn_sign * 1.8 * dt\n\nR(a)^T * R(a) = I\n|R(a)*v| = |v|\n(R(a)*d) dot (R(a)*c) = d dot c'),bullets(
        'Radian measure: a full turn is 2π radians = 360°. 1.8 rad/s is approximately 103.13°/s.',
        'At dt=0.05, a right turn is 0.09 rad ≈ 5.157°. East becomes (cos 0.09, sin 0.09) ≈ (0.995953,0.089879).',
        'The plane (0,0.6) becomes approximately (−0.053927,0.597572). Length remains 0.6 and it stays perpendicular to direction.',
        'In screen/world coordinates with Y downward, positive angle turns clockwise. Save the old X value before changing Y.')),
    'Orthogonality preservation follows from R transpose times R equaling identity. Both vectors must use the same matrix.',
    'source/player.c:_player_rotate; player_update')
mathslide('Time integration and the collision footprint',
    table(['Quantity','Formula / interpretation'],[
        ('Time conversion','seconds = tv_sec + tv_usec / 1,000,000; dt = clamp(now − previous, 0, 0.05). First active frame uses dt=0.'),
        ('Translation and turn','Δp = velocity × dt; Δangle = angular_speed × dt. Both are explicit per-frame updates.'),
        ('Collision footprint','Candidate square [x−0.2,x+0.2] × [y−0.2,y+0.2]; query its four corners with floor.'),
        ('Maximum update','Translation ≤ 2.5 × 0.05 = 0.125 world units when |d|≈1; a one-direction turn ≤ 0.09 rad.'),
        ('Sliding','Accept X at current Y, then accept Y at the resulting X. This is axis-separated endpoint testing, not swept collision.'),
        ('Example at wall x=7','A center at x=6.79 has right edge 6.99: floor cell. At x=6.81 its right edge 7.01 reaches wall cell 7: blocked.')])+p('The square is only 0.4 units wide, less than a cell, so its corners cover the cells touched by its footprint. These constants keep steps below a tile width; this is not a general continuous-collision proof for arbitrary speeds, radii or geometry.'),
    'A long pause is capped, so the simulation discards part of the elapsed time rather than catching up through multiple fixed steps.',
    'source/game.c:game_frame; source/player.c; header/core/const.h')
mathslide('Camera plane and the field-of-view triangle',
    code('c = 0.6 * (-d_y, d_x)\nd dot c = 0; |d| = 1; |c| = 0.6\nray at edge = d ± c\ntan(FOV_x / 2) = opposite / adjacent = |c| / |d|\nFOV_x = 2 * atan(0.6) ≈ 1.080839 rad ≈ 61.9275°')+
    p('The conceptual camera plane is one unit forward from p and extends 0.6 to either side. Each ray goes from p through a point on that plane. Increasing plane scale widens the view; increasing direction length without scaling the plane narrows it.')+
    p('For N rays, u_i = 2i/(N−1)−1 includes both edges. Equal steps in u are equal camera-plane steps, not equal angular steps: angle_i = atan(0.6u_i). For N=1, this implementation uses u=0.'),
    'Use a right triangle with forward side 1 and lateral side 0.6. Do not describe the samples as evenly spaced angles.',
    'source/parser/parser.c:_parser_apply_player; source/raycast_camera.c')
mathslide('Deriving DDA increments and first boundaries',
    code('Ray equation: q_x(t) = p_x + t*r_x\nAt vertical boundary x=b: t = (b - p_x) / r_x\nConsecutive vertical boundaries differ by one cell:\n    delta_x = |1 / r_x|; likewise delta_y = |1 / r_y|\n\nm_x = floor(p_x)\nif r_x > 0: first boundary b_x = m_x + 1\nif r_x < 0: first boundary b_x = m_x\nside_x = |b_x - p_x| * delta_x   (and likewise for Y)')+
    p('The code compresses the branch into b_x = map_x + (step_x &gt; 0), where the comparison contributes 1 or 0. It stores the chosen boundary time before adding delta, so hit.distance already equals t and needs no “subtract delta” correction.')+
    p('If r_x=0, there is no vertical crossing. Code uses a finite 1e30 sentinel rather than dividing by zero. It is a practical approximation for ordinary map dimensions.'),
    'delta is a change in ray parameter t. It is Euclidean distance only for a unit-length ray, which these camera rays generally are not.',
    'source/raycast.c:_raycast_setup; _raycast_advance')
mathslide('An oblique DDA trace, including the tie rule',
    p('Take p=(3.5,2.5), east-facing left-edge ray r=(1,−0.6). Then delta=(1,1.666667), step=(+1,−1), initial side=(0.5,0.833333). In maps/001.cub:')+
    table(['Choose','Time t','Entered cell','Next side times / result'],[
        ('X','0.5','(4,2)','side_x=1.5, side_y≈0.833333; floor'),
        ('Y','≈0.833333','(4,1)','side_x=1.5, side_y=2.5; floor'),
        ('X','1.5','(5,1)','side_x=2.5, side_y=2.5; floor'),
        ('Y on equality','2.5','(5,0)','Top wall: hit at (6.0,1.0); RAYSIDE_Y; SO texture')])+p('At a grid corner, several cells touch the same point. The strict side_x &lt; side_y comparison chooses Y on equality; it does not test every corner-adjacent cell. Corner hits can select a different face if floating-point rounding changes a near-tie.')+
    p('The width+height bound follows because every iteration advances X or Y monotonically by one cell; a ray must leave a finite rectangle within that many crossings if no hit stops it.'),
    'This trace complements the nearly horizontal ray: it exercises both axes and shows the exact boundary tie convention.',
    'source/raycast.c; maps/001.cub')
mathslide('Depth, Euclidean length, and the fish-eye proof',
    code('r = d + u*c; |d|=1; d dot c=0\nhit - p = t*r\nforward depth D = (hit-p) dot d\n                = t * (d dot d + u*c dot d) = t\nEuclidean ray length E = |t*r| = t*|r|\ncos(theta) = (r dot d)/(|r|*|d|) = 1/|r|\nTherefore: E*cos(theta) = t = D')+
    p('For edge ray r=(1,−0.6), |r|=√1.36≈1.166190. At t=2.5, Euclidean length ≈2.915476 while forward depth is 2.5. Projecting with Euclidean length would shrink this edge wall by factor 1/1.166190≈0.857493.')+
    p('The stored hit.distance is already t, so no explicit cosine multiplication is needed. Normalizing r without changing the distance calculation would invalidate that shortcut. The proof assumes exact camera invariants; repeated floating-point rotations preserve them approximately.'),
    'A straight wall at constant forward depth should have constant projected height. Using longer edge-ray lengths makes it appear curved.',
    'source/raycast_camera.c; source/raycast.c; source/player.c')
mathslide('Projection: similar triangles and the chosen scale',
    code('Similar triangles: projected_height / focal_pixels = wall_height / D\nGeneral form: projected_height = focal_pixels * wall_height / D\nImplementation: height = int(screen_height / max(D, 0.05))\nThis uses focal_pixels * wall_height = screen_height = 768.')+
    p('For a unit-height wall and a square-pixel camera calibrated to horizontal FOV, f_x = (W/2)/tan(FOV_x/2) = 1024/(2×0.6) ≈ 853.333 pixels. This code uses 768 for the vertical wall scale, so its walls are 768/853.333 = 0.9 times that particular unit-wall calibration.')+
    p('That is an independent vertical scale choice, not a DDA error or a fish-eye effect. Equivalently, one could interpret the walls as height 0.9 with the horizontal focal calibration. The code does not explicitly define a world-space wall-height parameter.')+
    p('The 0.05 depth clamp caps projected height at 15,360 pixels. Very distant walls can truncate to height 0; the empty drawing span then avoids a tex_y division by zero.'),
    'Do not claim that screen_height/D follows uniquely from plane scale 0.6. Similar triangles explain inverse depth, but the focal/height scale is an implementation choice.',
    'source/render/render_wall.c:_render_column_setup; header/core/const.h')
mathslide('Screen strips, clipping and integer arithmetic',
    table(['Stage','Exact arithmetic and consequence'],[
        ('Strip partition','x_start=floor(iW/N), x_end=floor((i+1)W/N). End-exclusive strips share boundaries; first starts at 0 and final ends at W.'),
        ('1024 pixels / 300 rays','1024=300×3+124: 176 strips are 3 pixels wide and 124 are 4 pixels wide. Each strip reuses one texture column.'),
        ('Vertical centering','top = H/2 − height/2 with C integer division. H=768 and height=219 give top=384−109=275; floating-point centering would differ.'),
        ('Visible interval','start=max(top,0); end=min(top+height,H). Draw start ≤ y &lt; end.'),
        ('Near-wall example','D=0.5 → height=1536, top=−384, visible y=0..767. The screen shows the central section of the tall wall.'),
        ('Sampling caveat','u includes camera-plane endpoints, while strips cover intervals of pixels. The mapping is a finite approximation, not one ray through each pixel center.')]),
    'Explain half-open intervals once and use that convention consistently for rectangles, screen strips and texture spans.',
    'source/render/render_wall.c:_render_3d_ray; _render_column_setup; _render_wall')
mathslide('Texture coordinates: fractional position and crop',
    code('s = hit.y for X-side hits; hit.x for Y-side hits\nu = s - floor(s)                       so 0 ≤ u < 1\ntex_x = floor(u * texture_width)\nif face orientation needs reversal: tex_x = width - 1 - tex_x\ntex_y = int((y - top) * texture_height / projected_height)')+
    p('Using the worked ray i=149 and the actual 16×16 west texture: hit.y≈2.492976589, u≈0.492976589, raw tex_x=7, flipped tex_x=8. At screen y=384 with top=275 and height=219, tex_y=floor(109×16/219)=7.')+
    p('Near-wall example: height=1536, top=−384, texture height=16. Visible top row y=0 samples tex_y=4; y=767 samples 11. Using clipped start=0 as the origin would incorrectly stretch the full texture onto the screen.')+
    p('Because 0≤u&lt;1, tex_x stays in [0,width−1]. For a nonempty wall span, 0≤y−top&lt;height, keeping tex_y in [0,texture_height−1]. Tiny floating-point differences near a texel boundary may change the selected texel.'),
    'The wall coordinate uses the axis along the wall: Y on vertical walls, X on horizontal walls. Sampling uses nearest-neighbor truncation.',
    'source/render/render_wall.c; source/texture.c; textures/west.xpm')
mathslide('Minimap transform and pixel/color arithmetic',
    cols(code('overlay scale = min(256/map_W, 256/map_H)\nv_W = int(map_W * scale)\nv_H = int(map_H * scale)\nv_x = frame_W - v_W - 16; v_y = 16\n\npx = v_x + int(world_x * v_W / map_W)\npy = v_y + int(world_y * v_H / map_H)'),bullets(
        'For the 8×7 sample map: overlay 256×224 at (752,16). Spawn (3.5,2.5) maps to pixel (864,96).',
        'Full-map mode uses 1024×768, so a tile spans 128 pixels horizontally and about 109.714 vertically. Integer boundaries can make adjacent tile heights differ by one pixel.',
        'RGB packing is R×65536+G×256+B. A channel uses eight bits; 50,50,50 gives 3,289,650 = 0x323232.',
        'At 32 bpp with stride=4096, pixel (10,20) is 20×4096+10×4 = 81,960 bytes from addr. Actual stride comes from MLX.')),
    'Viewport fitting avoids clipping the map. Independent viewport conversion in full-map mode stretches its proportions.',
    'source/screen.c; source/render/render_draw.c; source/parser/parser_color.c')
mathslide('Bresenham: integer error instead of slope division',
    code('dx=|to.x-from.x|; dy=|to.y-from.y|\nsx=sign(to.x-from.x); sy=sign(to.y-from.y)\nerror=dx-dy\nDraw current point; stop if at endpoint. Otherwise:\n  twice = 2*error                         save once\n  if twice > -dy: error -= dy; x += sx\n  if twice <  dx: error += dx; y += sy')+
    p('The ideal segment has a fixed ratio of horizontal to vertical progress. The error accumulator tracks the imbalance in integer units; subtracting dy accounts for an X step, adding dx accounts for a Y step. Both tests can pass in one iteration.')+
    p('Example from (0,0) to (5,3): dx=5, dy=3, initial error=2. Drawn points are (0,0), (1,1), (2,1), (3,2), (4,2), (5,3). After the first step, error=4; the next iteration steps only X.')+
    p('The saved twice value must feed both decisions. Endpoint equality is checked before stepping, so a zero-length segment draws once. No dy/dx division is needed, including for vertical lines.'),
    'This is the minimap FOV line rasterizer. Its error measures a discrete line decision, not the world distance to a wall.',
    'source/render/render_draw.c:render_line; _render_line_step')
math_end = len(pages)

# Human-written contracts supplement source-extracted signatures and call edges.
DESCRIPTIONS = '''
common_key_id|Map X11 keycodes to logical movement/view IDs; unknown keys return -1. W/up and S/down each share an ID.
common_now|Read gettimeofday and return seconds as a double, or -1.0 on failure.
common_error|Write Error and the supplied message to stderr; return 1 for a caller to propagate or store.
_game_has_motion|Return whether any of the six movement/turn booleans is set; view toggle is excluded.
_game_player_changed|Compare all position, direction and plane components; return whether any value changed.
_game_update_player|Snapshot the player, update movement/rotation, and set needs_redraw only if the player changed.
_game_render|Skip a clean frame; otherwise clear and render. On failure set exit_status and request loop end; on success clear needs_redraw.
game_frame|Loop callback: early-out when idle, obtain/clamp elapsed time, optionally update player, then render if dirty. Returns callback status.
hook_press|Escape requests close; M toggles once per latched press; other recognized keys set logical held-state and request redraw on transition.
hook_release|Clear the mapped logical key; request redraw when the key had been held. Unknown keycodes have no effect.
hook_focus_out|Clear every held key and mark dirty if any key was active, preventing motion from sticking after focus loss.
hook_expose|Mark the frame dirty when the window is exposed; actual composition occurs in the frame callback.
main|Validate arguments, initialize the local game, then run the MLX loop. Argument/setup failures return 1.
_parser_apply_player|Convert N/S/E/W to unit direction, place the player at cell center, and construct a perpendicular plane scaled by 0.6.
_parser_transfer|Move the local map into g, zero the previous owner, replace the spawn with floor, and apply player coordinates/orientation.
parser_cleanup|Free all owned grid rows, the pointer array, and four texture path strings; reset the map. NULL map is accepted.
parser_load_map|Build local parser state, read/validate, then transfer ownership to g. Free local data and report an error on failure. Returns 0 success / 1 failure.
parser_is_player|Return true only for N, S, E or W spawn markers.
parser_is_walkable|Return true for floor or a player marker; spaces and walls are not walkable.
parser_map_at|Return a grid cell, or space for a missing map or coordinates outside its rectangular dimensions.
_parser_skip_space|Advance a borrowed string cursor over spaces and tabs; allocate nothing.
_parser_parse_rgb_value|Parse one unsigned decimal component, allowing surrounding spaces/tabs; reject absent digits or values above 255; advance cursor.
_parser_parse_separator|Require a comma at the cursor, then consume it. Return 1 success or 0 missing separator.
_parser_parse_rgb|Parse three components separated by two commas; require end of string after the third. Return a success boolean.
parser_parse_color|Validate input/output pointers, parse RGB, then pack 0xRRGGBB into the destination. Return 1 on success, 0 on failure.
_parser_texture_field|Recognize a NO/SO/WE/EA two-character prefix and return its field bit; otherwise return 0. Whitespace is checked later.
_parser_texture_slot|Return the address of the texture-path slot for a recognized field bit, or NULL.
_parser_parse_texture_config|Require identifier whitespace, reject duplicates, allocate a trimmed nonempty path, store it and set its field bit. Return boolean success.
_parser_parse_color_config|Select F or C destination, enforce whitespace and uniqueness, parse RGB and set the corresponding field bit. Return boolean success.
parser_parse_config|Reject empty input; dispatch texture prefixes or F/C to their config parsers. Unknown identifiers return 0.
_parser_free_grid|Free up to height row pointers and the grid array. Handles NULL grid and zeroed row pointers.
_parser_pad_row|Allocate width+1 bytes, copy a row, pad the rest with spaces, and append NUL. Return the new row or NULL.
_parser_copy_grid|Allocate a zeroed pointer array and padded copies of all rows. Clean the new partial grid if a copy fails.
parser_rectangularize|Create the complete padded grid before freeing the original grid, then swap it in. Return 1 success / 0 allocation failure.
_parser_read_byte|Refill the 4096-byte buffer when consumed, retry EINTR, and output one byte. Return 1 byte / 0 EOF / -1 error.
_parser_append_byte|Append to a growing line; start at capacity 128 and double with overflow protection. Return 1 success / 0 failure.
_parser_collect_line|Collect non-newline bytes, reject embedded NUL, append a terminator, and support final unterminated lines. Return 1 line / 0 EOF / -1 error.
parser_next_line|Initialize temporary line storage; transfer its allocation to the caller only on status 1, otherwise free it. Set output NULL before reading.
_parser_is_map_start|Exclude texture prefixes; classify an initial wall or walkable marker as the start of map content.
_parser_process_content|Drive before/in/after-map state. Return 0 rejected, 1 accepted without ownership transfer, or 2 accepted with original line stored in grid.
parser_process_line|Trim a classification copy, process content, free the copy, and free the original unless stored. Return accepted/rejected boolean.
_parser_resize_grid|Allocate height+2 row pointers, copy existing entries and terminator, then free the old pointer array. Row strings retain ownership.
parser_store_map_line|Check integer/allocation limits; append the owned original row, update height/max width, and maintain NULL termination. Return boolean success.
_parser_read_lines|Initialize the reader and process lines until EOF. Reject reader errors or the first rejected line; return boolean success.
parser_read_file|Validate path suffix, open/read/close, require nonempty dimensions, then rectangularize. Return 1 success / 0 failure.
_parser_is_closed_cell|Require all four cardinal neighbors to be wall or walkable. Out-of-bounds and void neighbors fail.
_parser_inspect_map_line|Reject illegal characters; count and record player marker coordinates/direction. Return 1 valid row / 0 invalid character.
_parser_validate_closed_cells|Inspect every rectangular cell; all walkable cells must satisfy cardinal enclosure. Does not perform reachability traversal.
_parser_scan_cells|Scan rows and aggregate player count; return -1 for invalid characters or the total number of markers.
parser_validate_map|Require all six fields, a grid at least 3 by 3, exactly one player, legal cells and enclosure. Return boolean success.
_player_cell_blocked|Floor world coordinates to cell coordinates; treat outside-grid and every non-floor cell as blocked.
_player_position_valid|Check the four square-footprint corners at offsets +/- PLAYER_RADIUS; return true only if all are unblocked.
_player_move|Normalize forward/strafe inputs, calculate time-scaled displacement, test/commit X then Y independently for wall sliding.
_player_rotate|Apply the same sine/cosine rotation to direction and camera plane, preserving the camera relationship.
player_update|Move using the current direction, then apply left and right rotations according to held keys and elapsed time.
_raycast_setup|Initialize current grid cell, reciprocal-component delta values, signed steps and first-boundary times for this ray.
_raycast_advance|Choose the smaller side time (ties choose Y), record hit.distance, increment that time and step one cell; store crossed side.
_raycast_in_boundaries|Return whether the stepped cell lies within the map before any grid lookup occurs.
_raycast_record_hit|Continue on floor; otherwise record crossed side and world hit point pos + ray*distance. Return 1 for a hit.
raycast_cast|Reset hit, initialize DDA, and traverse at most width+height steps; return 1 for first non-floor hit, 0 for escape or step exhaustion.
raycast_camera_ray|Zero a ray result and calculate dir + plane*camera_x for an indexed camera sample. Handles a single configured ray with camera_x=0.
_render_line_step|Update Bresenham error and move the current point in X and/or Y using the saved doubled error.
render_px|Clip coordinates to image bounds, compute address using stride and bpp, then write a 32-bit color.
render_line|Initialize integer Bresenham state and draw an inclusive segment, calling render_px for each point until the endpoint.
render_rect|Fill a rectangle with half-open coordinate bounds using render_px; empty ranges perform no writes.
render_world_to_map|Scale world coordinates independently into viewport X/Y dimensions and add viewport origin. Return integer pixel coordinates.
_render_fov_ray|Build/cast one camera ray; on a hit convert its world endpoint and draw an orange line from the player.
render_fov|Convert player position once, then draw all 300 camera rays through the minimap viewport.
_render_tile_color|Choose gray for walls, dark gray for floor, and black for void.
_render_map_cell|Convert cell corners to screen coordinates, optionally draw a border, then fill the cell interior with its tile color.
_render_map|Visit every rectangular grid coordinate and draw its cell inside the given viewport.
_render_player|Map the player center to pixels; draw a green square sized from tile scale with a two-pixel minimum.
render_map_view|Compose map cells, FOV rays and player marker in that order into the frame buffer.
_render_background|Fill one complete frame column using ceiling color above the midline and floor color below it.
_render_column_setup|Select texture, clamp distance, project and clip wall height, calculate fractional wall coordinate and horizontal texture flip.
_render_wall|Copy the selected texture column into one clipped screen column. Vertical sampling is based on the original unclipped top.
_render_3d_ray|Generate/cast one ray, prepare a column if it hits, then paint background and optional wall across that ray's screen strip.
render_3d|Process every configured camera sample to fill the 3D image; no window blit occurs here.
runtime_args|Require argc=2 and exact lowercase .cub suffix; print usage and return 1 on invalid arguments, otherwise 0.
runtime_cleanup|Destroy texture/frame images, window, display/context, then parser-owned map allocations; reset graphics handles.
runtime_setup|Zero the entire game; parse map and create graphics with short-circuit failure cleanup; set initial needs_redraw. Return 0 success / 1 failure.
runtime_run|Enter mlx_loop, clean up after it returns, and return the stored application exit status.
runtime_close|Request loop termination with mlx_loop_end. Return 0 as a callback result; actual cleanup occurs later.
_screen_overlay|Fit map aspect ratio within a 256x256 maximum, enforce at least one pixel per viewport dimension and position at top-right margin.
screen_toggle_view|Switch V_3D and V_MINIMAP, then request redraw.
screen_clear|Check the game/frame address and zero stride*height bytes. Return 0 success / 1 invalid state.
screen_render|Check frame/window; choose 3D plus overlay or full map, draw map layers, then submit the frame through MLX. Return 0 success / 1 invalid state.
_setup_mlx|Create the display context and 1024x768 window; report and return 1 if either creation fails, otherwise 0.
_setup_hook|Register five window event callbacks and the per-loop game_frame callback, passing g to each.
_setup_frame|Create the frame image, borrow its data/format information and clear it; return 1 with an error if image/address acquisition fails.
setup_game|Create MLX/window, textures and frame in order; stop on failure. Register callbacks after resources succeed. Return 0 success / 1 failure.
_texture_load|Decode one XPM into an MLX image and acquire its buffer; require address, positive dimensions and 32 bpp. Return 0 success / 1 failure.
texture_load|Load all four map texture paths into graphics texture slots, stopping at first error. Return 0 success / 1 failure.
texture_cleanup|Destroy any allocated texture image through MLX and zero every texture slot; safe for partially loaded sets.
texture_for_hit|Select WE/EA on X hits or NO/SO on Y hits according to ray sign; return a borrowed texture image pointer.
'''
desc = dict(line.split('|',1) for line in DESCRIPTIONS.strip().splitlines())
pattern = re.compile(r'^(?:static\s+)?(?:unsigned\s+int|int|void|double|char|t_img|t_point|t_viewport)\s*\*?\s*\*?\s*(\w+)\s*\([^;{}]*\)\s*\{', re.M)
functions = []
modules = []
for path in sorted((ROOT/'source').rglob('*.c')):
    text = path.read_text()
    funcs = []
    for match in pattern.finditer(text):
        name = match.group(1)
        start = match.end()-1; end=start+1; depth=1
        while depth:
            depth += (text[end]=='{') - (text[end]=='}'); end+=1
        body=text[start:end]
        calls=list(dict.fromkeys(re.findall(r'\b([A-Za-z_]\w*)\s*\(',body)))
        calls=[c for c in calls if c not in {'if','while','for','switch','return','sizeof'}]
        f=dict(name=name,signature=' '.join(match.group(0)[:-1].split()),line=text[:match.start()].count('\n')+1,path=str(path.relative_to(ROOT)),calls=calls)
        funcs.append(f);functions.append(f)
    modules.append((path,funcs))
assert set(f['name'] for f in functions)==set(desc), ('Missing/extraneous descriptions',set(f['name'] for f in functions)^set(desc))
all_names={f['name'] for f in functions}
callback_callers = {'game_frame':['MLX loop callback (registered in _setup_hook)'], 'hook_press':['MLX key-press event'], 'hook_release':['MLX key-release event'], 'hook_focus_out':['MLX focus-out event'], 'hook_expose':['MLX expose event'], 'runtime_close':['MLX close event'], 'main':['C runtime entry']}
appendix_pages=[]
for path,funcs in modules:
    items=[]
    for f in funcs:
        callers=[x['name'] for x in functions if f['name'] in x['calls']]
        callers += callback_callers.get(f['name'],[])
        if f['name']=='parser_load_map': callers += ['tests/map_check.c:main']
        if f['name']=='parser_cleanup': callers += ['tests/map_check.c:main']
        internal=[c for c in f['calls'] if c in all_names]
        external=[c for c in f['calls'] if c not in all_names]
        item='<div class="fn" id="fn-'+f['name']+'"><div class="signature">'+esc(f['signature'])+'</div>'
        item+='<div class="contract">'+esc(desc[f['name']])+'</div>'
        item+='<div class="edges"><b>Called by:</b> '+esc(', '.join(callers) or 'No direct project caller')+'<br><b>Calls:</b> '+esc(', '.join(internal) or 'No project functions')
        if external: item+=' <span class="muted">| Library/system: '+esc(', '.join(external))+'</span>'
        item+='</div><div class="location">'+esc(f["path"])+':'+str(f['line'])+(' • file-local helper' if f['signature'].startswith('static ') else ' • public function')+'</div></div>'
        items.append(item)
    appendix_pages.append((str(path.relative_to(ROOT)), len(pages)+1))
    slide(path.name, '<div class="functions">'+''.join(items)+'</div>',
          'Reading key: signatures and call edges are extracted from the source snapshot. Callback edges are identified separately. Calls are listed once per function; branch order is explained in the walkthrough.',
          str(path.relative_to(ROOT)), 'FUNCTION REFERENCE')
slide('Library boundaries and parser-only test entry',
      table(['Boundary','Project-relevant behavior'],[
          ('MiniLibX lifecycle','mlx_init / mlx_new_window create context/window; mlx_loop dispatches X11 events and the loop callback; mlx_loop_end sets a termination flag.'),
          ('MiniLibX image I/O','mlx_new_image and mlx_xpm_file_to_image create images; mlx_get_data_addr returns borrowed pixels; mlx_put_image_to_window presents the frame.'),
          ('MiniLibX release','mlx_destroy_image, mlx_destroy_window, mlx_destroy_display; the project then frees the allocated MLX context.'),
          ('libft used here','ft_strlen, ft_strcmp/ft_strncmp, ft_strtrim; ft_memcpy, ft_memset, ft_calloc; ft_abs; ft_putendl_fd. String trimming allocates a new string.'),
          ('System and math','open/read/close for input; malloc/free for ownership; gettimeofday for time; floor/fabs/fmin/fmax/sqrt/sin/cos for geometry.'),
          ('tests/map_check.c:main','Accept map path or --textures path; zero game; parser_load_map; optionally access(R_OK) for four paths; parser_cleanup; return 0 accepted / 1 rejected.')])+p('The reference covers all authored application functions under source/. Vendored libft and MLX internals are dependencies; their unrelated functions are outside the presentation scope.'),
      'The test harness intentionally avoids graphics initialization. A readable texture file can still fail decoding in a real run.', 'library/mlx/mlx_loop.c; mlx_hook.c; mlx_loop_hook.c; library/libft/; tests/map_check.c', 'REFERENCE')
slide('Final rehearsal: explain the project without the slides',
      bullets('In 30 seconds: explain how a 2D grid becomes a first-person view.',
              'In 2 minutes: trace main through parser ownership, setup, callbacks, and cleanup.',
              'In 3 minutes: work the sample ray from (3.5,2.5) through DDA, wall-face selection, texture sampling, and screen strip.',
              'In 2 minutes: explain dirty rendering, time scaling, diagonal normalization, and four-corner collision.',
              'In 2 minutes: explain a malformed file, a graphics initialization failure, and a normal window close.',
              'From any reference page: identify a function’s caller, input state, result, ownership effects, and next stage.')+
      p('<b>Completion standard:</b> you can connect every project function to startup, parsing, input/update, rendering, or teardown, and explain the important branches without inventing behavior.'),
      'Keep the code available during rehearsal. Verify names and assumptions when you are unsure; this document is a snapshot, not an automatic guarantee after future edits.', 'Source snapshot and generation details appear in the page footer.', 'REHEARSAL')

navrows=[]
# Two-column index packs all module pages on one sheet.
for i in range(0,len(appendix_pages),2):
    pair=appendix_pages[i:i+2]
    row=[]
    for path,number in pair: row += [path.replace('source/',''),str(number)]
    if len(row)==2: row+=['','']
    navrows.append(row)
pages[contents_index]['body']=p('Walkthrough: pages 4–27. Function reference begins on page 28. Each module below includes every public function and file-local helper.')+table(['Module','Page','Module','Page'],navrows)+p('Library boundary and test harness: page '+str(len(pages)-1)+'. Rehearsal checklist: page '+str(len(pages))+'.')
# Derive walkthrough page range instead of relying on hand-maintained numbers.
first_ref=appendix_pages[0][1]
pages[contents_index]['body']=pages[contents_index]['body'].replace('pages 4–27. Function reference begins on page 28.', f'pages 4–{math_start-1}; mathematics: {math_start}–{math_end}. Function reference begins on page {first_ref}.')
revision=subprocess.check_output(['git','rev-parse','--short','HEAD'],cwd=ROOT,text=True).strip()
css='''
@page { size: A4 landscape; margin: 0; }
* { box-sizing: border-box; }
html,body { margin:0; padding:0; background:#e7edf3; color:#16293d; font-family:Arial, sans-serif; }
.slide { background:white; width:297mm; height:210mm; padding:12mm 15mm 10mm; position:relative; overflow:hidden; break-after:page; page-break-after:always; }
.slide:last-child { break-after:auto; page-break-after:auto; }
.kicker { color:#00887f; font-size:11px; letter-spacing:2px; font-weight:bold; margin-bottom:10px; }
h1 { font-size:30px; line-height:1.12; margin:0 0 18px; max-width:950px; }
p,li { font-size:16px; line-height:1.48; }
p { margin:12px 0; } ul { padding-left:23px; margin:14px 0; } li { margin:12px 0; }
code { font-family:'DejaVu Sans Mono',monospace; font-size:.88em; color:#05665f; }
pre { white-space:pre-wrap; overflow-wrap:anywhere; padding:18px; border-radius:6px; background:#eef4f7; font-family:'DejaVu Sans Mono',monospace; font-size:14px; line-height:1.55; margin:12px 0; }
.hero { font-size:45px; line-height:1.25; font-weight:bold; margin:40px 0 24px; }
.hero em { color:#00887f; font-style:normal; }
.flow { display:flex; gap:8px; align-items:stretch; margin:22px 0; }
.node { flex:1; background:#e6f3f0; border-top:4px solid #00887f; padding:17px 10px; text-align:center; font-weight:bold; font-size:15px; display:flex; justify-content:center; align-items:center; }
.arrow { align-self:center; color:#00887f; font-size:21px; }
.cols { display:grid; grid-template-columns:1fr 1fr; gap:25px; }
table { border-collapse:collapse; width:100%; font-size:14px; line-height:1.4; margin:12px 0; }
th { background:#18354a; color:white; text-align:left; padding:10px 12px; }
td { padding:10px 12px; border-bottom:1px solid #d8e1e7; vertical-align:top; }
tr:nth-child(even) td { background:#f3f7f9; }
.note { position:absolute; bottom:17mm; left:15mm; right:15mm; padding:10px 14px; background:#fff5dd; border-left:3px solid #ddac48; font-size:12px; line-height:1.45; }
.note b { color:#906211; }
.footer { position:absolute; bottom:7mm; left:15mm; right:15mm; color:#5d7183; font-size:10px; display:flex; justify-content:space-between; gap:15px; }
.source { max-width:76%; }
.functions { margin-top:4px; }
.fn { padding:9px 0 10px; border-bottom:1px solid #d6e2e8; }
.signature { color:#006b63; font-family:'DejaVu Sans Mono',monospace; font-size:12px; font-weight:bold; line-height:1.35; overflow-wrap:anywhere; }
.contract { font-size:13px; line-height:1.4; margin:5px 0; }
.edges { font-size:11px; line-height:1.45; overflow-wrap:anywhere; }
.location { font-size:10px; color:#718395; margin-top:4px; }
.muted { color:#657b8d; }
.slide.reference h1 { margin-bottom:8px; }
.slide.reference .note { font-size:10px; }
.slide.contents table { font-size:12px; line-height:1.2; }
.slide.contents td, .slide.contents th { padding:6px 10px; }
.slide.contents p { font-size:14px; }
@media screen { .slide { margin:20px auto; box-shadow:0 5px 25px #bac8d3; } }
@media print { body { background:white; } .slide { -webkit-print-color-adjust:exact; print-color-adjust:exact; } }
'''
htmlpages=[]
for i,s in enumerate(pages,1):
    klass='reference' if s['tag']=='FUNCTION REFERENCE' else ('contents' if s['tag']=='CONTENTS' else '')
    footer=esc(s['source'])+'<br>Snapshot '+revision+' • 2026-11-06 • '+str(len(functions))+' application functions / '+str(len(modules))+' source files'
    htmlpages.append('<section class="slide '+klass+'" id="page-'+str(i)+'"><div class="kicker">CUB3D / '+esc(s['tag'])+'</div><h1>'+esc(s['title'])+'</h1><div class="body">'+s['body']+'</div><aside class="note"><b>SPEAKER NOTE</b> &nbsp;'+s['note']+'</aside><footer class="footer"><div class="source">'+footer+'</div><div>'+str(i)+' / '+str(len(pages))+'</div></footer></section>')
(OUT/'cub3d-function-flows.html').write_text('<!doctype html><html lang="en"><head><meta charset="utf-8"><title>cub3D — Complete Function Flows</title><style>'+css+'</style></head><body>'+''.join(htmlpages)+'</body></html>')
(OUT/'function-flow-index.txt').write_text('\n'.join(f"{i:02d}  {s['title']}" for i,s in enumerate(pages,1))+'\n\n'+'\n'.join(f"{f['path']}:{f['line']} {f['name']}" for f in functions)+'\n')
print(f'Generated {len(pages)} pages; {len(functions)} functions; {len(modules)} source files. Revision {revision}.')
