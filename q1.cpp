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

class GameTree
{
    GameNode *root;

    // Helper function for insertion
    GameNode *insertNode(GameNode *node, const Game &data)
    {
        if (!node)
            return new GameNode(data);
        if (data.gameID < node->data.gameID)
            node->left = insertNode(node->left, data);
        else if (data.gameID > node->data.gameID)
            node->right = insertNode(node->right, data);
        else
            cout << "Conflict: Game ID already exists.\n";
        return node;
    }

    // Helper function for search
    GameNode *searchNode(GameNode *node, const string &gameID) const
    {
        if (!node || node->data.gameID == gameID)
            return node;
        if (gameID < node->data.gameID)
            return searchNode(node->left, gameID);
        return searchNode(node->right, gameID);
    }

    // Helper function for deletion
    GameNode *deleteNode(GameNode *node, const string &gameID)
    {
        if (!node)
            return node;
        if (gameID < node->data.gameID)
            node->left = deleteNode(node->left, gameID);
        else if (gameID > node->data.gameID)
            node->right = deleteNode(node->right, gameID);
        else
        {
            if (!node->left)
                return node->right;
            else if (!node->right)
                return node->left;
            GameNode *temp = minValueNode(node->right);
            node->data = temp->data;
            node->right = deleteNode(node->right, temp->data.gameID);
        }
        return node;
    }

    GameNode *minValueNode(GameNode *node)
    {
        while (node && node->left)
            node = node->left;
        return node;
    }
