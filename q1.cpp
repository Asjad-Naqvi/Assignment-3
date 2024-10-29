#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// Structures for Game and Player Data
struct Game
{
    string gameID;
    string name;
    string developer;
    string publisher;
    double rating;
    int downloads;

    Game(string id, string n, string d, string p, double r, int dl)
        : gameID(id), name(n), developer(d), publisher(p), rating(r), downloads(dl) {}
};

struct GameScore
{
    string gameID;
    double score;
    GameScore *next;

    GameScore(string id, double s) : gameID(id), score(s), next(nullptr) {}
};

struct Player
{
    string playerID;
    string name;
    string contact;
    string email;
    GameScore *games; // Linked list of games and scores

    Player(string id, string n, string c, string e)
        : playerID(id), name(n), contact(c), email(e), games(nullptr) {}

    // Add a game to the player's list
    void addGameScore(string gameID, double score)
    {
        GameScore *newGame = new GameScore(gameID, score);
        newGame->next = games;
        games = newGame;
    }
};

// --- Node structure for Binary Search Tree ---
struct GameNode
{
    Game data;
    GameNode *left;
    GameNode *right;

    GameNode(const Game &g) : data(g), left(nullptr), right(nullptr) {}
};