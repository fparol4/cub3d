# Base completion plan

Map parsing remains deferred. Steps are implemented and checked in order.

1. [x] Authorized timing and elapsed clamping.
2. [x] Render folder and Makefile source discovery.
3. [x] Return contracts, naming, collision predicates and constants.
4. [x] Four directional textures and development assets.
5. [x] Resource cleanup and explicit error propagation.
6. [x] Application source/header Norm compliance.
7. [x] Single-build Makefile arrangement and English README.
8. [x] Build, failure, movement, raycasting and cleanup checks.

The placeholder map is intentional until parser work begins. The code now
uses one build with the map overlay, full-window map view, and collisions.
The `bonus` rule aliases `all`; the subject classifies the map view and
collisions as bonus features, so their final grading placement remains open.

Validation: application and libft pass Norm; the unified build passes
-Wall -Wextra -Werror; four axial rays and wall-face selection pass; movement
bounds and wall collision checks pass; the window-close path has zero
Valgrind errors and zero bytes retained. Invalid arguments print Error.
