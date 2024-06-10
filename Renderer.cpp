#include "Renderer.h"
#include "Game.h"

#include <algorithm>

void Renderer::Clear()
{
	ClearBackground(RAYWHITE);
}

void Renderer::RenderBackground()
{
	int offsetX = (Game::WINDOW_WIDTH - 8 * Game::CELL_SIZE) / 2;
	int offsetY = (Game::WINDOW_HEIGHT - Game::INFO_BAR_HEIGHT - 8 * Game::CELL_SIZE) / 2 + Game::INFO_BAR_HEIGHT;

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			int x = j * Game::CELL_SIZE + offsetX;
			int y = i * Game::CELL_SIZE + offsetY;

			Color cellColor = GetShadeColor(GetColorCell({ i, j }));
			DrawRectangle(x, y, Game::CELL_SIZE, Game::CELL_SIZE, cellColor);
		}
	}
}

void Renderer::RenderPieces(const Board& board, const std::map<string, Texture>& textures)
{
	int offsetX = (Game::WINDOW_WIDTH - 8 * Game::CELL_SIZE) / 2;
	int offsetY = (Game::WINDOW_HEIGHT - Game::INFO_BAR_HEIGHT - 8 * Game::CELL_SIZE) / 2 + Game::INFO_BAR_HEIGHT;

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			Piece* piece = board.At({ i, j });

			if (piece != nullptr)
			{
				int x = j * Game::CELL_SIZE + offsetX;
				int y = i * Game::CELL_SIZE + offsetY;

				DrawTexture(textures.at(piece->GetName()), x, y, WHITE);
			}
		}
	}
}

void Renderer::RenderPossibleMoves(const std::map<string, Texture>& textures, const std::vector<Move>& possibleMoves)
{
	int offsetX = (Game::WINDOW_WIDTH - 8 * Game::CELL_SIZE) / 2;
	int offsetY = (Game::WINDOW_HEIGHT - Game::INFO_BAR_HEIGHT - 8 * Game::CELL_SIZE) / 2 + Game::INFO_BAR_HEIGHT;

	for (const Move& move : possibleMoves)
	{
		DrawTexture(
			textures.at(GetTextureFromMoveType(move.type)),
			move.position.j * Game::CELL_SIZE + offsetX,
			move.position.i * Game::CELL_SIZE + offsetY,
			WHITE
		);
	}
}

void Renderer::RenderGuide()
{
	int padding = 5;
	int fontSize = 20;
	int characterSize = 10;

	int offsetX = (Game::WINDOW_WIDTH - 8 * Game::CELL_SIZE) / 2;
	int offsetY = (Game::WINDOW_HEIGHT - Game::INFO_BAR_HEIGHT - 8 * Game::CELL_SIZE) / 2 + Game::INFO_BAR_HEIGHT;

	// Render row numbers (1-8) on the left side
	for (int i = 0; i < 8; i++)
	{
		Color textColor = GetShadeColor(Piece::GetInverseColor(GetColorCell({ i, 0 })));
		int x = padding;
		int y = i * Game::CELL_SIZE + (Game::CELL_SIZE - characterSize) / 2 + offsetY;

		char text[2];
		text[0] = '1' + (7 - i); // For 1-8 from top to bottom
		text[1] = '\0';

		DrawText(text, x + offsetX - padding * 2, y, fontSize, textColor); // Adjust x position for centering
	}

	// Render column letters (a-h) on the bottom side
	for (int j = 0; j < 8; j++)
	{
		Color textColor = GetShadeColor(Piece::GetInverseColor(GetColorCell({ 7, j })));
		int x = j * Game::CELL_SIZE + (Game::CELL_SIZE - characterSize) / 2 + offsetX;
		int y = Game::WINDOW_HEIGHT - Game::INFO_BAR_HEIGHT + padding;

		char text[2];
		text[0] = 'a' + j; // For a-h from left to right
		text[1] = '\0';

		DrawText(text, x, y, fontSize, textColor);
	}
}


void Renderer::RenderPromotion(const std::map<std::string, Texture> &textures, PIECE_COLOR pawnColor)
{
	DrawRectangle(0, 0, Game::WINDOW_WIDTH, Game::WINDOW_HEIGHT, Color{ 0, 0, 0, 127 });
	DrawText("Promotion", Game::WINDOW_WIDTH / 2 - 98, Game::WINDOW_HEIGHT / 4, 40, WHITE);

	string prefix = pawnColor == PIECE_COLOR::C_WHITE ? "w" : "b";

	int textureY = Game::CELL_SIZE * 3 + Game::INFO_BAR_HEIGHT;
	int textY = Game::CELL_SIZE * 4 + 5 + Game::INFO_BAR_HEIGHT;

	// Queen
	{
		DrawTexture(textures.at(prefix + "q"), Game::CELL_SIZE * 2, textureY, WHITE);
		DrawText("Queen", Game::CELL_SIZE * 2 + 9, textY, 20, WHITE);
	}

	// Rook.
	{
		DrawTexture(textures.at(prefix + "r"), Game::CELL_SIZE * 3, textureY, WHITE);
		DrawText("Rook", Game::CELL_SIZE * 3 + 14, textY, 20, WHITE);
	}

	// Bishop.
	{
		DrawTexture(textures.at(prefix + "b"), Game::CELL_SIZE * 4, textureY, WHITE);
		DrawText("Bishop", Game::CELL_SIZE * 4 + 7, textY, 20, WHITE);
	}

	// Knight.
	{
		DrawTexture(textures.at(prefix + "n"), Game::CELL_SIZE * 5, textureY, WHITE);
		DrawText("Knight", Game::CELL_SIZE * 5 + 9, textY, 20, WHITE);
	}
}

void Renderer::RenderInfo(int round, double timer)
{
	DrawRectangle(0, 0, Game::WINDOW_WIDTH, Game::INFO_BAR_HEIGHT, BLACK);

	std::string roundText = "Round: " + std::to_string(round);
	std::string timeText = "Time: " + std::to_string((int)timer) + "s";

	int timeTextWidth = MeasureText(timeText.c_str(), 20);
	int padding = 5;

	DrawText(roundText.c_str(), padding, Game::INFO_BAR_HEIGHT / 2 - 10, 20, WHITE);
	DrawText(timeText.c_str(), Game::WINDOW_WIDTH - timeTextWidth - padding, Game::INFO_BAR_HEIGHT / 2 - 10, 20, WHITE);
}

void Renderer::RenderEndGame(GAME_STATE gameState)
{
	DrawRectangle(0, 0, Game::WINDOW_WIDTH, Game::WINDOW_HEIGHT, Color{ 0, 0, 0, 127 });

	const char* text{}; //{}

	if (gameState == GAME_STATE::GS_WHITE_WON) {
		text = "WHITE WINS !";
	}
	else if (gameState == GAME_STATE::GS_BLACK_WON) {
		text = "BLACK WINS !";
	}
	else if (gameState == GAME_STATE::GS_STALEMATE) {
		text = "STALEMATE";
	}

	int textLength = MeasureText(text, 40);
	DrawText(text, Game::WINDOW_WIDTH / 2 - textLength / 2, Game::WINDOW_HEIGHT / 2, 40, WHITE);
}

void Renderer::MouseCursor(const Board& board, const std::vector<Move>& possibleMoves, PIECE_COLOR turn, bool inPromotion)
{
	Vector2 mousePosition = GetMousePosition();
	mousePosition.y -= Game::INFO_BAR_HEIGHT;

	int offsetX = (Game::WINDOW_WIDTH - 8 * Game::CELL_SIZE) / 2;
	int offsetY = (Game::WINDOW_HEIGHT - Game::INFO_BAR_HEIGHT - 8 * Game::CELL_SIZE) / 2;

	mousePosition.x -= offsetX;
	mousePosition.y -= offsetY;

	Position hoverPosition = { int(mousePosition.y) / Game::CELL_SIZE, int(mousePosition.x) / Game::CELL_SIZE };

	if (!inPromotion)
	{
		bool isHoveringOverPiece = board.At(hoverPosition) && board.At(hoverPosition)->color == turn;
		auto it = std::find_if(possibleMoves.begin(), possibleMoves.end(), [hoverPosition](const Move& m)
			{
				return m.position.i == hoverPosition.i && m.position.j == hoverPosition.j;
			});

		bool isHoveringOverMove = it != possibleMoves.end();

		// Set mouse to pointer if hovering over piece or hovering over move.
		if (isHoveringOverPiece || isHoveringOverMove)
		{
			SetMouseCursor(4);
		}
		else
		{
			SetMouseCursor(0);
		}
	}
	else {
		// If in promotion screen, also set mouse to pointer if hovering over the options.
		if (hoverPosition.i == 3 && hoverPosition.j >= 2 && hoverPosition.j <= 5)
		{
			SetMouseCursor(4);
		}
	}
}

std::string Renderer::GetTextureFromMoveType(MOVES moveType)
{
	switch (moveType) {
	case MOVES::STEP:
	case MOVES::DOUBLE_STEP:
	case MOVES::ATTACK:
		return "move";

	case MOVES::SHORT_CASTLE:
	case MOVES::LONG_CASTLE:
		return "castling";

	case MOVES::EN_PASSANT:
		return "enpassant";

	case MOVES::PROMOTION:
	case MOVES::ATTACK_PROMOTION:
		return "promotion";
	}
}

Color Renderer::GetShadeColor(PIECE_COLOR color)
{
	return color == PIECE_COLOR::C_WHITE ? Game::LIGHT_SHADE : Game::DARK_SHADE;
}

PIECE_COLOR Renderer::GetColorCell(const Position& cellPosition)
{
	int startingColorInRow = cellPosition.i % 2 == 0 ? 0 : 1;
	int colorIndex = (startingColorInRow + cellPosition.j) % 2;

	return colorIndex == 0 ? PIECE_COLOR::C_WHITE : PIECE_COLOR::C_BLACK;
}
