# TTC-Crusher

Bot written in C++ to play Tic Tac Chek game (Mix between tic tac toe and chess)

This bot wraps to a python library, this because the evaluator for the bot is written in python, so this C++ code compiles into a python lib.

The bot uses alpha beta prunning as its minmax algorithm, and bitboards for efficient board reprecentation and fast move generation.
