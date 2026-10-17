#!/bin/bash

ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
VALID_DIR="$ROOT_DIR/tests/valid_maps"
INVALID_DIR="$ROOT_DIR/tests/invalid_maps"

if [ ! -f "$ROOT_DIR/library/libft/libft.a" ]; then
	printf 'Erro: biblioteca nao encontrada: %s\n' "$ROOT_DIR/library/libft/libft.a"
	printf 'Execute "make" na raiz do projeto antes dos testes.\n'
	exit 1
fi

CHECKER="$(mktemp)" || exit 1
trap 'rm -f "$CHECKER"' EXIT
if ! cc -Wall -Wextra -Werror -I"$ROOT_DIR/header" \
	-I"$ROOT_DIR/library/libft" "$ROOT_DIR/tests/map_check.c" \
	"$ROOT_DIR"/source/map/*.c "$ROOT_DIR/source/common.c" \
	"$ROOT_DIR/library/libft/libft.a" -o "$CHECKER"; then
	exit 1
fi
cd "$ROOT_DIR" || exit 1

valid_total=0
valid_failures=0
invalid_total=0
invalid_failures=0

printf '%s\n' '=== Mapas validos ==='
for file in "$VALID_DIR"/*.cub; do
	[ -f "$file" ] || continue
	valid_total=$((valid_total + 1))
	if "$CHECKER" --textures "$file" >/dev/null 2>&1; then
		printf 'OK      %s\n' "$(basename "$file")"
	else
		printf 'FALHOU  %s\n' "$(basename "$file")"
		valid_failures=$((valid_failures + 1))
	fi
done

printf '\n%s\n' '=== Mapas invalidos ==='
for file in "$INVALID_DIR"/*.cub "$INVALID_DIR"/*.cube "$INVALID_DIR"/*.txt; do
	[ -f "$file" ] || continue
	invalid_total=$((invalid_total + 1))
	if "$CHECKER" "$file" >/dev/null 2>&1; then
		printf 'ACEITO  %s\n' "$(basename "$file")"
		invalid_failures=$((invalid_failures + 1))
	else
		printf 'OK      %s\n' "$(basename "$file")"
	fi
done

printf '\n%s\n' '=== Resumo ==='
printf 'Validos:   %d | falhas: %d\n' "$valid_total" "$valid_failures"
printf 'Invalidos: %d | aceitos indevidamente: %d\n' \
	"$invalid_total" "$invalid_failures"

if [ "$valid_failures" -ne 0 ] || [ "$invalid_failures" -ne 0 ]; then
	exit 1
fi

exit 0
