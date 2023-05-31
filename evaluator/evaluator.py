"""
Tic-Tac-Chec Evaluator
Program to run and evaluate Tic-Tac-Chess analysis modules. 

This program makes two modules compete against each other several times and
gather statics about the number of wins, losses and draws for each program.
It also checks for legal and illegal moves as well as for winning and losing positions. 

The set of rules that this evaluator checks are as follows:
    1. Tic-Tac-Chess is played by 2 players on a 4x4 chess board. The game starts with an empty board, each player taking a contrary side, facing each other.
    
    2. Each player has a set of pieces, white and black respectively, that contains: 1 rook, 1 knight, 1 bishop and 1 pawn. 
    Each piece moves the same as in regular chess. The only exception is when the pawn reaches the opponent's side of the board, in which case it reverses direction.

    3. A new piece can only be positioned on an empty square, it cannot capture pieces while entering the board.

    4. For the first 3 turns, each player can only place new pieces on the board. In this initial stage, players cannot move or capture any piece that is already on the board.

    5. Starting on the 4th turn, the player can move or capture pieces from his/her opponent. When a piece is captured, it is returned to the owner, who can
    place it inmediately on his/her next turn.

    6. There is a limit on the number of times a player can capture an opponent's piece. After that limit is reached by the player, he/she can no longer capture pieces.

    7. The game is finished when a player is able to align his/her four pieces horizontally, vertically or diagonally.

    8. There is a limit of turns that can be played. If that limit is reached and no player has won, the game would be considered a draw.

"""
from player import TTCPlayer
import copy
import sys
    
class PlayerWrapper:
    def __init__(self, player, piecesColor):
        self.player = player
        self.captures = 0
        self.pawnDirection = -1
        self.piecesColor = piecesColor

        self.statistics = {
            'name': self.player.name,
            'wins': 0,
            'loses': 0,
            'draws': 0,
            'raised_errors': 0,
            'invalid_moves': 0,
            'early_movements': 0,
            'exceed_max_captures': 0,
        }

    def resetValues(self, color):
        self.player.reset()
        self.player.setColor(color)
        self.captures = 0
        self.pawnDirection = -1
        self.piecesColor = color



class TTCEvaluator:
    def __init__(self):
        self.whitePlayer = None
        self.blackPlayer = None

        self.currentTurn = 0
        self.maxCaptures = 0
        self.maxTurns = 0
        self.board = None

        self.WIN = 1
        self.LOSE = -1
        self.CONTINUE = 0

    def __printBoard(self, board):
        print('-------')
        for i in range(len(board)):
            for j in range(len(board)):
                if board[i][j] >= 0:
                    print(' ', end='')
                print(board[i][j], end='')
            print()
        print('-------')

    def __sameSign(self, a, b):
        return ((a < 0 and b < 0) or (a > 0  and b > 0))
    
    def __isInsideBoard(self, row, col):
        return (row >= 0 and row < 4 and col >= 0 and col < 4)

    def __updatePawnDirection(self, board, player):
        # If the pawn is in the limit of the board, it should reverse
        if player.piecesColor in board[0]:
            player.pawnDirection = 1
        
        # If the pawn is in the start of the board, it should go forward
        if player.piecesColor in board[3]:
            player.pawnDirection = -1

    def __getPawnValidMovements(self, position, board, pawnDirection):
        validMovements = []
        
        row = position[0]
        col = position[1]

        # Move 1 to the front
        newRow = row + pawnDirection
        if self.__isInsideBoard(newRow, col) and board[newRow][col] == 0:
            validMovements.append((newRow, col))

        # Attack to the left
        newCol = col - 1
        if self.__isInsideBoard(newRow, newCol) and board[newRow][newCol] != 0 and not self.__sameSign(board[newRow][newCol], board[row][col]):
            validMovements.append((newRow, newCol))

        # Attack to the right
        newCol = col + 1
        if self.__isInsideBoard(newRow, newCol) and board[newRow][newCol] != 0 and not self.__sameSign(board[newRow][newCol], board[row][col]):
            validMovements.append((newRow, newCol))

        return validMovements
    
    def __getBishopValidMovements(self, position, board):
        validMovements = []

        row = position[0]
        col = position[1]

        # To check whether I already encountered a piece in this diagonal or not
        # 0 -> Up-Left Diagonal
        # 1 -> Up-Right Diagonal
        # 2 -> Down-Left Diagonal
        # 3 -> Down-Right Diagonal
        diagEncounteredPiece = [False] * 4

        # Describe the direction of the movement for the bishop in the same
        # order as described above
        movDirection = [[-1, -1],
                     [-1, 1],
                     [1, -1],
                     [1, 1]]

        # A bishop can move at most 3 squares
        for i in range(1,4):
            # Check 4 directions of movement
            for j in range(4):
                newCol = col + i * movDirection[j][0]
                newRow = row + i * movDirection[j][1]

                # If I haven't found a piece yet in this direction and its inside the board
                if not diagEncounteredPiece[j] and self.__isInsideBoard(newRow, newCol):
                    # If the proposed square its occupied
                    if board[newRow][newCol] != 0:
                        # If the piece that occupies the square its from the opponent, then its a valid movement
                        if not self.__sameSign(board[row][col], board[newRow][newCol]):
                            validMovements.append((newRow, newCol))
                        diagEncounteredPiece[j] = True
                    else: # If not, just append the movement
                        validMovements.append((newRow, newCol))

        return validMovements

    def __getKnightValidMovements(self, position, board):
        validMovements = []

        row = position[0]
        col = position[1]


        # Describe the movements of the knight
        movements = [[-2, 1],
                     [-1, 2],
                     [1, 2],
                     [2, 1],
                     [2, -1],
                     [1, -2],
                     [-1, -2],
                     [-2, -1]]
        
        # Loop through all possible movements
        for move in movements:
            newRow = row + move[0]
            newCol = col + move[1]

            # For the knight we just need to check if the new square is valid and it is not occupied by a piece of the same color
            if self.__isInsideBoard(newRow, newCol) and not self.__sameSign(board[row][col], board[newRow][newCol]):
                validMovements.append((newRow, newCol))

        return validMovements

    def __getRookValidMovements(self, position, board):
        validMovements = []

        row = position[0]
        col = position[1]

        # Checks whether or not I have found a piece in this direction
        # 0 - Up
        # 1 - Right
        # 2 - Down
        # 3 - Left
        dirPieceEncountered = [False] * 4

        # Describe the direction of movement for the rook
        # The order is the same as described above
        movDirection = [[-1, 0],
                        [0, 1],
                        [1, 0],
                        [0, -1]]
        
        # The rook can move maximum 3 squares
        for i in range(1, 4):
            # Loop through all possible movements
            for j in range(4):
                newRow = row + i * movDirection[j][0]
                newCol = col + i * movDirection[j][1]

                if not dirPieceEncountered[j] and self.__isInsideBoard(newRow, newCol):
                    if board[newRow][newCol] != 0:
                        if not self.__sameSign(board[newRow][newCol], board[row][col]):
                            validMovements.append((newRow, newCol))
                        dirPieceEncountered[j] = True
                    else:
                        validMovements.append((newRow, newCol))

        return validMovements        

    def __getValidMovements(self, pieceCode, position, board, player):
        if abs(pieceCode) == 1:
            return self.__getPawnValidMovements(position, board, player.pawnDirection)
        elif abs(pieceCode) == 2:
            return self.__getBishopValidMovements(position, board)
        elif abs(pieceCode) == 3:
            return self.__getKnightValidMovements(position, board)
        elif abs(pieceCode) == 4:
            return self.__getRookValidMovements(position, board)
        else:
            print("Piece ", pieceCode, " not recognized")
            return []

    def __compareWithBoardsWithNewPiece(self, pieceCode, oldBoard, newBoard):
        for i in range(4):
            for j in range(4):
                if oldBoard[i][j] == 0:
                    oldBoard[i][j] = pieceCode

                    if oldBoard == newBoard:
                        oldBoard[i][j] = 0
                        return True
                    
                    oldBoard[i][j] = 0

        return False
    
    def __compareWithBoardsWithMovement(self, pieceCode, position, oldBoard, newBoard, player):
        validMovementsSquares = self.__getValidMovements(pieceCode, position, oldBoard, player)
        row = position[0]
        col = position[1]

        oldBoard[row][col] = 0
        for newSquare in validMovementsSquares:
            # No se si esto haga una copia o solo sea una referencia
            prevPiece = oldBoard[newSquare[0]][newSquare[1]]
            oldBoard[newSquare[0]][newSquare[1]] = pieceCode
            if oldBoard == newBoard:
                oldBoard[newSquare[0]][newSquare[1]] = prevPiece
                oldBoard[row][col] = pieceCode
                return True
            
            oldBoard[newSquare[0]][newSquare[1]] = prevPiece

        oldBoard[row][col] = pieceCode
        return False

    def __wasValidMove(self, oldBoard, newBoard, player):
        pieces = [None] * 5

        # No puede haber errores porque es el tablero antiguo
        for i in range(4):
            for j in range(4):
                if self.__sameSign(oldBoard[i][j], player.piecesColor):
                    pieces[abs(oldBoard[i][j])] = (i, j)

        wasBoardFound = False
        for i in range(1, len(pieces)):
            if pieces[i] is not None:
                wasBoardFound = self.__compareWithBoardsWithMovement(i * player.piecesColor, pieces[i], oldBoard, newBoard, player)
            else:
                wasBoardFound = self.__compareWithBoardsWithNewPiece(i * player.piecesColor, oldBoard, newBoard)

            if wasBoardFound:
                return True
            
        return False

    # Function to check whether a movement was a movement or not
    # If it was a movement, it also checks if it was a capture or not.
    # For a movement to be classified as a capture, 2 conditions have to occur
    # 1. Only 2 squares changed value
    # 2. One square has to change from used to empty and the other from used to used with different color    
    def __wasPieceMovement(self, oldBoard, newBoard):
        changedSquares = []

        for i in range(4):
            for j in range(4):
                if oldBoard[i][j] != newBoard[i][j]:
                    changedSquares.append((i, j))

        if len(changedSquares) != 2:
            return False, False
        
        def areChangesFromCapture(row1, col1, row2, col2):
            return (newBoard[row1][col1] == 0
                    and oldBoard[row2][col2] != 0 
                    and newBoard[row2][col2] == oldBoard[row1][col1])
        
        def areChangesFromMovement(row1, col1, row2, col2):
            return (newBoard[row1][col1] == 0
                    and newBoard[row2][col2] == oldBoard[row1][col1])
        
        wasMovement = (areChangesFromMovement(changedSquares[0][0], changedSquares[0][1], changedSquares[1][0], changedSquares[1][1]) 
                or areChangesFromMovement(changedSquares[1][0], changedSquares[1][1], changedSquares[0][0], changedSquares[0][1]))
        
        wasCapture = (areChangesFromCapture(changedSquares[0][0], changedSquares[0][1], changedSquares[1][0], changedSquares[1][1]) 
                or areChangesFromCapture(changedSquares[1][0], changedSquares[1][1], changedSquares[0][0], changedSquares[0][1]))

        return (wasMovement, wasCapture)

    # Check if the position on the board is a winning position.
    # It checks all the rows, columns and both diagonals looking for 4-pieces in a row.
    def __isWinningPosition(self, board, color):
        lrDiagSum = 0
        rlDiagSum = 0
        for i in range(4):
            # Check diagonal left-right, up-down
            if self.__sameSign(board[i][i], color):
                lrDiagSum += 1

            # Check diagonal right-left, up-down
            if self.__sameSign(board[i][3-i], color):
                rlDiagSum += 1

            rowSum = 0
            colSum = 0
            # Loop to check all squares in a row or column
            for j in range(4):
                # Check row i
                if self.__sameSign(board[i][j], color):
                    rowSum += 1

                # Check column i
                if self.__sameSign(board[j][i], color):
                    colSum += 1

            # If the sum is 4, we have 4-pieces in a row
            if rowSum == 4 or colSum == 4:
                return True
            
        # If the sum is 4, we have 4-pieces in a row   
        if lrDiagSum == 4 or rlDiagSum == 4:
            return True
        
        return False

    # Rotate board 180 degrees
    def __rotateBoard(self, board):
        N = len(board)
        # rotate the matrix by 180 degrees
        for i in range(N // 2):
            for j in range(N):
                temp = board[i][j]
                board[i][j] = board[N - i - 1][N - j - 1]
                board[N - i - 1][N - j - 1] = temp
        
        return board

    def __playTurn(self, player):
        print("-----", player.player.name, "with pieces", player.piecesColor, "turn:", self.currentTurn, "-----")
        newBoard = copy.deepcopy(self.board)
        try:
            newBoard = player.player.play(newBoard)
        except:
            print(player.player.name, "raised an exception. Loses automatically")
            player.statistics['raised_errors'] += 1
            
            exc_type, exc_value, _ = sys.exc_info()
            print("Exception type:", exc_type)
            print("Exception message:", exc_value)
            
            return self.LOSE
    
        if self.__wasValidMove(self.board, newBoard, player):
            wasMovement, wasCapture = self.__wasPieceMovement(self.board, newBoard)
            if wasMovement:
                # Check if player made a movement on the first 3 moves
                if self.currentTurn < 3:
                    print(player.player.name, "made a movement on the first 3 moves. Loses automatically")
                    player.statistics['early_movements'] += 1
                    self.__printBoard(self.board)
                    self.__printBoard(newBoard)
                    return self.LOSE
            
            if wasCapture:
                player.captures += 1

                # Check if player has exceeded the maximum number of captures allowed
                if player.captures > self.maxCaptures:
                    player.statistics['exceed_max_captures'] += 1
                    print(player.player.name, "exceeded the limit of captures. Loses automatically")
                    self.__printBoard(self.board)
                    self.__printBoard(newBoard)
                    return self.LOSE

            if self.__isWinningPosition(newBoard, player.piecesColor):
                print(player.player.name, " wins!")
                self.__printBoard(newBoard)
                return self.WIN

            self.__updatePawnDirection(newBoard, player)
            # Update board
            self.board = newBoard
            return self.CONTINUE

        else:
            print("Referee Pawn Direction:", player.pawnDirection)
            print(player.player.name, "made an illegal move. Loses automatically")
            player.statistics['invalid_moves'] += 1
            self.__printBoard(self.board)
            self.__printBoard(newBoard)

            return self.LOSE

    def __startGame(self):
        while self.currentTurn < self.maxTurns:
            resultWhite = self.__playTurn(self.whitePlayer)

            if resultWhite == self.WIN:
                self.whitePlayer.statistics['wins'] += 1
                self.blackPlayer.statistics['loses'] += 1
                return
            elif resultWhite == self.LOSE:
                self.blackPlayer.statistics['wins'] += 1
                self.whitePlayer.statistics['loses'] += 1
                return
            
            # Rotate board so the player has the correct face.
            self.board = self.__rotateBoard(self.board)
            resultBlack = self.__playTurn(self.blackPlayer)

            if resultBlack == self.WIN:
                self.blackPlayer.statistics['wins'] += 1
                self.whitePlayer.statistics['loses'] += 1
                return
            elif resultBlack == self.LOSE:
                self.whitePlayer.statistics['wins'] += 1
                self.blackPlayer.statistics['loses'] += 1
                return
            
            self.currentTurn += 1
            # Rotate board so the player has the correct face.
            self.board = self.__rotateBoard(self.board)

        self.blackPlayer.statistics['draws'] += 1
        self.whitePlayer.statistics['draws'] += 1

    def __initializeGame(self):
        self.board = [[0] * 4 for _ in range(4)]
        self.currentTurn = 0

        # Swap all the info of the players
        self.whitePlayer, self.blackPlayer = self.blackPlayer, self.whitePlayer

        # Reset the values of players
        self.whitePlayer.resetValues(1)
        self.blackPlayer.resetValues(-1)

    def runAnalysis(self, player1, player2, noGames, maxCaptures, maxTurns):
        self.maxCaptures = maxCaptures
        self.maxTurns = maxTurns

        # We put them this way because initializeGame is going to swap them
        self.blackPlayer = PlayerWrapper(player1, -1)
        self.whitePlayer = PlayerWrapper(player2, 1)

        for i in range(noGames):
            self.__initializeGame()
            print("----- STARTING GAME", i, "-----")
            self.__startGame()

        return self.whitePlayer.statistics, self.blackPlayer.statistics


evaluator = TTCEvaluator()