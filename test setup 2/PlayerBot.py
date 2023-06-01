"""
Value code for each piece is as follows:
    - pawn = 1 for white, -1 for black
    - bishop =  2 for white, -2 for black
    - knight =  3 for white, -3 for black
    - rook =  4 for white, -4 for black
* It is recommended to store the direction of movement of both your pawn and opponent's pawn.
"""

import copy
import time
import random

class TTCPlayer:
    # valuesCode is a list containing the value code that you must use to represent your pieces over the board. 
    # The sign of the value code will tell you if you are playing as white or black pieces.
    # The values are in the order: pawn, bishop, knight, rook
    def __init__(self,name):
        self.name = name
        self.pawnDirection = -1
        self.currentTurn = -1
        self.availableCaptures = 5
        self.maxiterations = 4
        self.initialPawnPosition = ()
        '''
        self.columncoeficient = [0,0,0,0]
        self.rowcoeficient = [0,0,0,0]
        self.diag1coeficient = [0,0,0,0]
        self.diag2coeficient = [0,0,0,0]
        '''

        self.piecesOnBoard = [0] * 5
        self.piecesPositions = [(-1,-1)]*5
        self.enemiesPositions = [(-1,-1)]*5

    def setColor(self, piecesColor):
        self.piecesCode = [0, 1, 2, 3, 4]
        self.piecesCode = [x*piecesColor for x in self.piecesCode]
        self.piecesColor = piecesColor

    def __updatePawnDirection(self, board):
        # If the pawn is in the limit of the board, it should reverse
        if self.piecesColor in board[0]:
            self.pawnDirection = 1
        
        # If the pawn is in the start of the board, it should go forward
        if self.piecesColor in board[3]:
            self.pawnDirection = -1

    def __sameSign(self, a, b):
        return ((a < 0 and b < 0) or (a > 0  and b > 0))
    
    def __isInsideBoard(self, row, col):
        return (row >= 0 and row < 4 and col >= 0 and col < 4)

        # Function to check whether a movement was a movement or not
    # If it was a movement, it also checks if it was a capture or not.
    # For a movement to be classified as a capture, 2 conditions have to occur
    # 1. Only 2 squares changed value
    # 2. One square has to change from used to empty and the other from used to used with different color    

    def __getMaxCoefficientValue(self,board):
        # Personalized values for pieces (only the pawn)
        # The Pawn is such a bad piece, that it needs a handicap
        # Pawn value = 4
        # Other pieces = 1
        colmax = [0,0,0,0]
        filmax = 0
        actfilmax = 0
        diag1max = 0
        diag2max = 0
        for i in range(4):
            actfilmax = 0
            for j in range(4):
                if board[i][j]*self.piecesColor > 0:
                    if board[i][j]*self.piecesColor == 1:
                        if i == j:
                            diag1max+=4
                        if i+j == 3:
                            diag2max+=4
                        colmax[j]+=4
                        actfilmax+=4
                    else:
                        if i == j:
                            diag1max+=1
                        if i+j == 3:
                            diag2max+=1
                        colmax[j]+=1
                        actfilmax+=1
            filmax = max(filmax,actfilmax)
        c = max(colmax)
        maxcoefficient = max(filmax,diag1max,diag2max,c)
        if maxcoefficient == filmax:
            return (maxcoefficient, 1)
        if maxcoefficient == c:
            return (maxcoefficient, 2)
        if maxcoefficient == diag1max:
            return (maxcoefficient, 3)
        if maxcoefficient == diag2max:
            return (maxcoefficient, 4)

    def __getMinCoefficientValue(self,board):
        # Personalized values for pieces (only the pawn)
        # The Pawn is such a bad piece, that it needs a handicap
        # Pawn value = 4
        # Other pieces = 1
        colmin = [0,0,0,0]
        filmin = 0
        actfilmin = 0
        diag1min = 0
        diag2min = 0
        for i in range(4):
            actfilmin = 0
            for j in range(4):
                if board[i][j]*self.piecesColor < 0:
                    if board[i][j]*self.piecesColor == -1:
                        if i == j:
                            diag1min-=4
                        if i+j == 3:
                            diag2min-=4
                        colmin[j]-=4
                        actfilmin-=4
                    else:
                        if i == j:
                            diag1min-=1
                        if i+j == 3:
                            diag2min-=1
                        colmin[j]-=1
                        actfilmin-=1
            filmin = min(filmin,actfilmin)
        c = min(colmin)
        mincoefficient = min(filmin,diag1min,diag2min,c)
        if mincoefficient == filmin:
            return (mincoefficient, 1)
        if mincoefficient == c:
            return (mincoefficient, 2)
        if mincoefficient == diag1min:
            return (mincoefficient, 3)
        if mincoefficient == diag2min:
            return (mincoefficient, 4)
    #The heuristic value we are going to use, in this case we will calculate based on the number of pieces on a row or col 
    def __getCoefficientValue(self,board):
        # Personalized values for pieces (only the pawn)
        # The Pawn is such a bad piece, that it needs a handicap
        # Pawn value = 4
        # Other pieces = 1
        colmax = [0,0,0,0]
        filmax = 0
        actfilmax = 0
        diag1max = 0
        diag2max = 0
        colmin = [0,0,0,0]
        filmin = 0
        actfilmin = 0
        diag1min = 0
        diag2min = 0
        for i in range(4):
            actfilmax = 0
            actfilmin = 0
            for j in range(4):
                if board[i][j]*self.piecesColor > 0:
                    if board[i][j]*self.piecesColor == 1:
                        if i == j:
                            diag1max+=4
                        if i+j == 3:
                            diag2max+=4
                        colmax[j]+=4
                        actfilmax+=4
                    else:
                        if i == j:
                            diag1max+=1
                        if i+j == 3:
                            diag2max+=1
                        colmax[j]+=1
                        actfilmax+=1
                elif board[i][j]*self.piecesColor < 0:
                    if board[i][j]*self.piecesColor == -1:
                        if i == j:
                            diag1min-=4
                        if i+j == 3:
                            diag2min-=4
                        colmin[j]-=4
                        actfilmin-=4
                    else:
                        if i == j:
                            diag1min-=1
                        if i+j == 3:
                            diag2min-=1
                        colmin[j]-=1
                        actfilmin-=1
            filmax = max(filmax,actfilmax)
            filmin = min(filmin,actfilmin)
        maxcoefficient = max(filmax,diag1max,diag2max,max(colmax))
        mincoefficient = min(filmin,diag1min,diag2min,min(colmin))
        if maxcoefficient < abs(mincoefficient):
            return mincoefficient
        else:
            return maxcoefficient 
    
    def __wasPieceMovement(self, oldBoard, newBoard):
        changedSquares = []

        for i in range(4):
            for j in range(4):
                if oldBoard[i][j] != newBoard[i][j]:
                    changedSquares.append((i, j))

        if len(changedSquares) != 2:
            return False, False
        #Esta mamada
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

    def __getPawnValidMovements(self, position, board, pawnDirection):
        validMovements = []
        
        row = position[0]
        col = position[1]

        # Move 1 to the front
        newRow = row + pawnDirection
        if self.__isInsideBoard(newRow, col) and board[newRow][col] == 0:
            validMovements.append((newRow, col))

        # Attack to the left
        if self.availableCaptures > 0:
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
                        # If the piece that occupies the square its from the opponent, then its a valid movement and there are captures remaining
                        if not self.__sameSign(board[row][col], board[newRow][newCol]) and self.availableCaptures > 0:
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
            if (self.__isInsideBoard(newRow, newCol) and (not self.__sameSign(board[row][col], board[newRow][newCol]) and self.availableCaptures > 0)):
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
                        if not self.__sameSign(board[newRow][newCol], board[row][col]) and self.availableCaptures > 0:
                            validMovements.append((newRow, newCol))
                        dirPieceEncountered[j] = True
                    else:
                        validMovements.append((newRow, newCol))

        return validMovements        

    def __getValidMovements(self, pieceCode, position, board):
        if abs(pieceCode) == 1:
            return self.__getPawnValidMovements(position, board, self.pawnDirection)
        elif abs(pieceCode) == 2:
            return self.__getBishopValidMovements(position, board)
        elif abs(pieceCode) == 3:
            return self.__getKnightValidMovements(position, board)
        elif abs(pieceCode) == 4:
            return self.__getRookValidMovements(position, board)
        else:
            print("Piece ", pieceCode, " not recognized")
            return []

    def __updatePiecesOnBoard(self, board):
        self.piecesOnBoard = [0] * 5
        self.piecesPositions = [(-1,-1)]*5
        self.enemiesPositions = [(-1,-1)]*5

        for i in range(len(board)):
            for j in range(len(board[0])):
                if self.__sameSign(board[i][j], self.piecesColor):
                    self.piecesPositions[abs(board[i][j])] = (i,j)
                    self.piecesOnBoard[abs(board[i][j])] = 1
                elif board[i][j]*self.piecesColor < 0:
                    self.enemiesPositions[abs(board[i][j])] = (i,j)

    def __checkGameOver(self,board):
        #El juego termina y ganamos
        if self.__getCoefficientValue(board) == 7:
            return 1
        #El juego termina y perdimos
        elif self.__getCoefficientValue(board) == -7:
            return -1
        #El juego sigue
        else:
            return 0
        
    def __generateBoard(self,board,initialPositon,finalPosition):
        newBoard = copy.deepcopy(board)
        piece = newBoard[initialPositon[0]][initialPositon[1]]
        newBoard[initialPositon[0]][initialPositon[1]] = 0
        newBoard[finalPosition[0]][finalPosition[1]] = piece
        return newBoard

    def __minmax(self,board,ismax,cont):
        movements = []
        positions = []
        finalBoard = board
        coefficient = self.__getCoefficientValue(board)
        if cont == 0 or coefficient == -7 or coefficient == 7: #Llegamos a tope o perdemos en esta iteracion
            return (coefficient,board) #Entonces retornamos
        #Caso maximizamos
        if ismax:
            for i in range(4):
                for j in range(4):
                    #Si es peon
                    if (board[i][j] == self.piecesCode[1]):positions.append((i,j)),  movements.append(self.__getValidMovements(self.piecesCode[1],(i,j),board))
                    #Si es nuestro alfil
                    elif(board[i][j] == self.piecesCode[2]): positions.append((i,j)), movements.append(self.__getValidMovements(self.piecesCode[2],(i,j),board))
                    #Si es nuestro caballo
                    elif(board[i][j] == self.piecesCode[3]):positions.append((i,j)), movements.append(self.__getValidMovements(self.piecesCode[3],(i,j),board))
                    #Si es nuestra torre
                    elif(board[i][j] == self.piecesCode[4]):positions.append((i,j)), movements.append(self.__getValidMovements(self.piecesCode[4],(i,j),board))
            coefficient = -10000
            for i in range(len(movements)):
                for j in range (len(movements[i])):
                    newBoard = self.__generateBoard(board,positions[i],movements[i][j])
                    aux = self.__minmax(newBoard, not ismax, cont-1)
                    if(coefficient < aux[0]):
                        coefficient = aux[0]
                        if cont == self.maxiterations: finalBoard = aux[1]
        else: #Caso si minimizamos
            for i in range(4):
                for j in range(4):
                    #Si es peon
                    if (board[i][j] == self.piecesCode[1]*-1):positions.append((i,j)),  movements.append(self.__getValidMovements(self.piecesCode[1],(i,j),board))
                    #Si es alfil enemigo
                    elif(board[i][j] == self.piecesCode[2]*-1): positions.append((i,j)), movements.append(self.__getValidMovements(self.piecesCode[2],(i,j),board))
                    #Si es caballo enemigo
                    elif(board[i][j] == self.piecesCode[3]*-1):positions.append((i,j)), movements.append(self.__getValidMovements(self.piecesCode[3],(i,j),board))
                    #Si es torre enemiga
                    elif(board[i][j] == self.piecesCode[4]*-1):positions.append((i,j)), movements.append(self.__getValidMovements(self.piecesCode[4],(i,j),board))
            coefficient = 10000
            for i in range(len(movements)):
                for j in range (len(movements[i])):
                    newBoard = self.__generateBoard(board,positions[i],movements[i][j])
                    aux = self.__minmax(newBoard, not ismax, cont-1)
                    if(coefficient > aux[0]):
                        coefficient = aux[0]
                        if cont == self.maxiterations: finalBoard = aux[1]
        return (coefficient,finalBoard)

    def __putPiece(self, board, piece):
        newBoard = copy.deepcopy(board)
        co = self.__getMaxCoefficientValue(newBoard)
        coefficient = co[0]
        maxType = co[1]
        sample = self.piecesPositions[1]
        if piece == self.piecesCode[1]:
            sample = self.piecesPositions[2]
        if coefficient == 6 or coefficient == 3: #Casi victoria
            winningTile = (-1,-1)
            if maxType == 1: #Fila ganadora
                fil = sample[0]
                for i in range(4):
                    if board[fil][i]*self.piecesColor <= 0:
                        winningTile = (fil,i)
                        break
            elif maxType == 2: #Columna ganadora
                col = sample[1]
                for i in range(4):
                    if board[i][col]*self.piecesColor <= 0:
                        winningTile = (i,col)
                        break
            elif maxType == 3: #diagonal ixi ganadora
                for i in range(4):
                    if board[i][i]*self.piecesColor <= 0:
                        winningTile = (i,i)
                        break
            else: #diagonal i+j=3 ganadora
                for i in range(4):
                    if board[i][3-i]*self.piecesColor <= 0:
                        winningTile = (i,3-i)
                        break
            if board[winningTile[0]][winningTile[1]] == 0:
                newBoard[winningTile[0]][winningTile[1]] = self.piecesCode[piece]
                return newBoard
            else:
                if piece == 1:
                    if self.pawnDirection == -1:
                        if self.__isInsideBoard(winningTile[0]+1,winningTile[1]-1) and board[winningTile[0]+1][winningTile[1]-1] == 0:
                            newBoard[winningTile[0]+1][winningTile[1]-1] = self.piecesCode[1]
                        elif self.__isInsideBoard(winningTile[0]+1,winningTile[1]+1) and board[winningTile[0]+1][winningTile[1]+1] == 0:
                            newBoard[winningTile[0]+1][winningTile[1]+1] = self.piecesCode[1]
                    else:
                        if self.__isInsideBoard(winningTile[0]-1,winningTile[1]-1) and board[winningTile[0]-1][winningTile[1]-1] == 0:
                            newBoard[winningTile[0]-1][winningTile[1]-1] = self.piecesCode[1]
                        elif self.__isInsideBoard(winningTile[0]-1,winningTile[1]+1) and board[winningTile[0]-1][winningTile[1]+1] == 0:
                            newBoard[winningTile[0]-1][winningTile[1]+1] = self.piecesCode[1]
                else:
                    mov = self.__getValidMovements(piece,winningTile,board)
                    if len(mov) > 0:
                        newBoard[mov[0][0]][mov[0][1]] = self.piecesCode[piece]
                        return newBoard
        newFil = random.randint(0,3)
        newCol = random.randint(0,3)
        while board[newFil][newCol] != 0:
            newFil = random.randint(0,3)
            newCol = random.randint(0,3)
        newBoard[newFil][newCol] = self.piecesCode[piece]
        return newBoard
            
    def play(self, board):
        start = time.time()
        n = self.maxiterations
        self.currentTurn += 1
        self.__updatePiecesOnBoard(board)
        newBoard = copy.deepcopy(board)
        #Iniciamos colando siempre el peon en una de las esquinas
        if self.currentTurn ==0:          
            if board[0][0] == 0:
                newBoard[0][0] = self.piecesCode[1]
                self.initialPawnPosition = (0,0)
            elif board[0][3] == 0:
                newBoard[0][3] = self.piecesCode[1]
                self.initialPawnPosition = (0,3)
        #Colocamos el caballo en el lado disponible del peon
        elif self.currentTurn == 1:
            #Caso 1:
            if self.initialPawnPosition[0] == 0 and self.initialPawnPosition[1] ==0 and board[0][1] ==0:
                newBoard[0][1] = self.piecesCode[3]
            elif self.initialPawnPosition[0] == 0 and self.initialPawnPosition[1] ==0 and board[1][0] ==0:
                newBoard[1][0] = self.piecesCode[3]
            #Caso 2:
            if self.initialPawnPosition[0] == 0 and self.initialPawnPosition[1] ==3 and board[0][2] ==0:
                newBoard[0][2] = self.piecesCode[3]
            elif self.initialPawnPosition[0] == 0 and self.initialPawnPosition[1] ==3 and board[1][3] ==0:
                newBoard[1][3] = self.piecesCode[3]
        #Colocamos la torre en una posición estratégica
        elif self.currentTurn == 2:
            if self.initialPawnPosition[0] == 0 and self.initialPawnPosition[1] == 0:
                if board[3][3] == 0:
                    newBoard[3][3] = self.piecesCode[4]
                elif board[0][1] == self.piecesCode[3]:
                    if board[3][0] == 0:
                        newBoard[3][0] = self.piecesCode[4]
                    else: newBoard[0][3] = self.piecesCode[4]
                else:
                    if board[0][3] == 0:
                        newBoard[0][3] = self.piecesCode[4]
                    else: board[3][0] = self.piecesCode[4]
            if self.initialPawnPosition[0] == 0 and self.initialPawnPosition[1] == 3:
                if board[3][0] == 0:
                    newBoard[3][0] = self.piecesCode[4]
                elif board[0][2] == self.piecesCode[3]:
                    if board[3][3] == 0:
                        newBoard[3][3] = self.piecesCode[4]
                    else: board[0][0] = self.piecesCode[4]
                else:
                    if board[0][0] == 0:
                        newBoard[0][0] = self.piecesCode[4]
                    else: board[3][3] = self.piecesCode[4]
        #Si es posible, ganamos el juego
        elif self.currentTurn == 3:
            if self.initialPawnPosition[0] == 0 and self.initialPawnPosition[1] == 0:
                if board[3][3] == self.piecesCode[4]:
                    if board[1][1] == 0: newBoard[1][1] = self.piecesCode[2]
                    elif board[0][1] == self.piecesCode[3]:
                        if board[1][0] == 0: newBoard[1][0] = self.piecesCode[2]
                        elif board[0][2] == 0: newBoard[0][2] = self.piecesCode[2]
                        else: newBoard[2][0] = self.piecesCode[2]
                    else:
                        if board[0][1] == 0: newBoard[0][1] = self.piecesCode[2]
                        elif board[2][0] == 0: newBoard[2][0] = self.piecesCode[2]
                        else: newBoard[0][2] = self.piecesCode[2]
                elif board[3][0] == self.piecesCode[4]:
                    if board[1][0] == 0: newBoard[1][0] = self.piecesCode[2]
                    else: newBoard = self.__putPiece(board,2)
                else:
                    if board[0][1] == 0: newBoard[0][1] = self.piecesCode[2]
                    else: newBoard = self.__putPiece(board,2)
            else:
                if board[3][0] == self.piecesCode[4]:
                    if board[1][2] == 0: newBoard[1][2] = self.piecesCode[2]
                    elif board[0][2] == self.piecesCode[3]:
                        if board[1][3] == 0: newBoard[1][3] = self.piecesCode[2]
                        elif board[0][1] == 0: newBoard[0][1] = self.piecesCode[2]
                        else: newBoard[2][3] = self.piecesCode[2]
                    else:
                        if board[0][2] == 0: newBoard[0][2] = self.piecesCode[2]
                        elif board[2][3] == 0: newBoard[2][3] = self.piecesCode[2]
                        else: newBoard[0][1] = self.piecesCode[2]
                elif board[3][3] == self.piecesCode[4]:
                    if board[1][3] == 0: newBoard[1][3] = self.piecesCode[2]
                    else: newBoard = self.__putPiece(board,2)
                else:
                    if board[0][2] == 0: newBoard[0][2] = self.piecesCode[2]
                    else: newBoard = self.__putPiece(board,2)
        else:
            notOnBoard = 0
            for i in range(1,5):
                if self.piecesOnBoard[i] == 0:
                    notOnBoard = i
                    break
            if notOnBoard == 0: newBoard = self.__minmax(board,True,n)[1] 
            else: newBoard = self.__putPiece(board,notOnBoard)     
        # We put a limit since there can be a really rare case when the only valid movement is a capture
        # And if in that moment it happens that you can no longer make any capture, it will cicle. That's why we put a limit.
        move,wasCapture = self.__wasPieceMovement(board,newBoard)
        if wasCapture:
            self.availableCaptures-=1
        self.__updatePawnDirection(newBoard)
        #print(time.time() - start,"s")

        return newBoard
                    
    def reset(self):
        self.pawnDirection = -1
        self.piecesOnBoard = [0] * 5
        self.currentTurn = -1
        self.availableCaptures = 5