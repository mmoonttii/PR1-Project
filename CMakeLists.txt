cmake_minimum_required(VERSION 3.27)
project(PR1_Project C)

set(CMAKE_C_STANDARD 99)

add_executable(PR1_Project main.c
		general.h
		preparazione.c
		preparazione.h
		errors.h
		memManagement.c
		memManagement.h)
