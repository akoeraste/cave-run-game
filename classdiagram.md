@startuml
title Design-Level Class Diagram for Cave Run

' =======================
' Core helper classes
' =======================

class Position {
  + x : int
  + y : int
  + distanceTo(other : Position) : int
  + isAdjacentTo(other : Position) : bool
}

class HealthStatus {
  - health : int
  - poisoned : bool
  --
  + damage(amount : int) : void
  + isDead() : bool
  + isPoisoned() : bool
  + setPoisoned(flag : bool) : void
}

' =======================
' Room hierarchy
' =======================

abstract class Room {
  - position : Position
  - color : string
  - label : string
  - visited : bool
  --
  + getPosition() : Position
  + setColor(c : string) : void
  + getColor() : string
  + setLabel(l : string) : void
  + getLabel() : string
  + isVisited() : bool
  + markVisited() : void
  + visit(player : Player) : void
  + getType() : string
}

class NormalRoom {
  + visit(player : Player) : void
  + getType() : string
}

class PoisonRoom {
  - poisonDamage : int
  --
  + visit(player : Player) : void
  + getType() : string
}

class TrapRoom {
  - damageAmount : int
  --
  + visit(player : Player) : void
  + getType() : string
  + getDamage() : int
}

Room <|-- NormalRoom
Room <|-- PoisonRoom
Room <|-- TrapRoom

Room --> Position

' =======================
' Character hierarchy
' =======================

abstract class Character {
  - position : Position
  - currentRoom : Room
  - movesRemaining : int
  - maxMoves : int
  --
  + getPosition() : Position
  + setRoom(room : Room) : void
  + getRoom() : Room
  + setMoves(m : int) : void
  + getMoves() : int
  + resetMoves() : void
  + sense(game : Game) : void
  + move(map : Map) : void
  + update() : void
  + getDirection() : Position
}

class Player {
  - healthStatus : HealthStatus
  - poisonDamage : int
  - cureChance : double
  - moveDirection : Position
  --
  + getHealth() : int
  + damage(amount : int) : void
  + cure() : void
  + isAlive() : bool
  + setMoveDirection(dir : Position) : void
  + sense(game : Game) : void
  + move(map : Map) : void
  + update() : void
  + getDirection() : Position
}

class Monster {
  - playerPosition : Position
  - exitPosition : Position
  --
  + sense(game : Game) : void
  + getDirection() : Position
  + calculateDistance(p1 : Position, p2 : Position) : double
  + update() : void
}

Character <|-- Player
Character <|-- Monster

Character --> Room
Character --> Position
Player --> HealthStatus

' =======================
' Map and Game
' =======================

class Map {
  - rooms : vector<vector<Room*>>
  - rows : int
  - cols : int
  - exitPosition : Position
  --
  + Map(rows : int, cols : int)
  + addRoom(room : Room) : void
  + getRoom(pos : Position) : Room
  + getDimensions() : Position
  + getExitPosition() : Position
  + setExitPosition(pos : Position) : void
  + isValidPosition(pos : Position) : bool
  + display() : void
}

class Game {
  - map : Map
  - player : Player
  - monster : Monster
  - activeCharacter : Character
  - gameOver : bool
  - playerWon : bool
  - turnCount : int
  --
  + Game(rows : int, cols : int)
  + start() : void
  + run() : void
  + getActivePosition() : Position
  + isGameOver() : bool
  + playerWins() : bool
  + playerTurn() : void
  + monsterTurn() : void
  + checkGameOver() : void
  + switchTurn() : void
  + displayMap() : void
  + displayGameOver() : void
}

Game --> Map
Game --> Player
Game --> Monster
Game --> Character

Map "1" o-- "many" Room

@enduml
