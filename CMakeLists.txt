cmake_minimum_required(VERSION 3.27)
project(PR1_Project C)

set(CMAKE_C_STANDARD 99)

add_executable(PR1_Project
		main.c
		general.c
		general.h
		errors.h
		carteCfu.c
		carteCfu.h
		carteOstacolo.c
		carteOstacolo.h
		giocatori.c
		giocatori.h
		turno.c
		turno.h)
