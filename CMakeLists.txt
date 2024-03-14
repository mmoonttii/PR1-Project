cmake_minimum_required(VERSION 3.26)
project(66424_montalto_lorenzo C)

set(CMAKE_C_STANDARD 99)

add_executable(66424_montalto_lorenzo
		main.c
		general.c
		general.h
		errors.h
		carteOstacolo.c
		carteOstacolo.h
		giocatori.c
		giocatori.h
		turno.c
		turno.h
		memoria.c
		memoria.h
		mazzoCfu.c
		mazzoCfu.h
		effetti.c
		effetti.h
		saves.c
		saves.h
		starting.c
		starting.h
		istantanee.c
		istantanee.h)
