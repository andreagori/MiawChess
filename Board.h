#ifndef MEOWCHESS_BOARD_H
#define MEOWCHESS_BOARD_H

class Board;  // Forward declaration (circular dependency).

#include "Piece.h"
#include "PieceEnums.h"
#include "Moves.h"
#include "raylib.h"

#include <map>
#include <string>
#include <vector>

class Board {
public:
    Board() = default;
    Board(const Board& other);
    ~Board();

    void Init();
    Piece* At(const Position& position) const;
    void Add(Piece* piece);
    void Destroy(const Position& position);
    void Clear();

    bool IsPositionWithinLimits(const Position& position) const;
    std::vector<Piece*> GetPiecesByColor(PIECE_COLOR color) const;
    Piece* GetLastMovedPiece() const;

    void DoMove(Piece* piece, const Move& move);
    bool MoveLeadsToCheck(Piece* piece, const Move& move);
    bool IsInCheck(PIECE_COLOR color);

private:
    void DoShortCastling(Piece* selectedPiece, const Move& move);
    void DoLongCastling(Piece* selectedPiece, const Move& move);

    std::vector<Piece*> whitePieces;
    std::vector<Piece*> blackPieces;

    Position lastMovedPiecePosition;
};

#endif //MEOWCHESS_BOARD_H
