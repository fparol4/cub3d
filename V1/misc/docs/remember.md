# [1]

- difference between frame.ptr and frame.addr
- why `ft_memset(g->frame.addr, 0, g->frame.llen * g->frame.height)`

# [doing]
	main [ok]
		runtime [ok]
			map [ok]
				hook [ok]
			setup [ok]

remember
 - [ ] minimap should be on the screen all the time


# [nextsteps#2]
- write the player_update + player_move [ok]
- write the raycast methods (+understand math) [NOW]
- write the render methods
- rm: add the minimap directly into the 3d vie

# [!] why to use `elapsed`

elapsed is a way of writing player movement for `smooth movement` that is
not dependent on `framerate` to work - the approach relies on `elapsed` time
between now and the new frame to know `how far` the movement should occur. 

move_x = direction_x * 2.5 (speed) * elapsed (0.05)
* elapsed is fixed into 50 milliseconds

[other ways]
fps = 1.0 / elapsed | 0.0168 (60fps), 0.0333 (30fps)

# [raycast]

ponto(t) = posição_do_jogador + t × direção_do_raio

Em coordenadas:

x(t) = pos_x + t × ray_x
y(t) = pos_y + t × ray_y
