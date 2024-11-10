#include <iostream>
#include <fstream>
#include <string>

using namespace std;

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
    GameScore *games;

    Player(string id, string n, string c, string e)
        : playerID(id), name(n), contact(c), email(e), games(nullptr) {}

    void addGameScore(string gameID, double score)
    {
        GameScore *newGame = new GameScore(gameID, score);
        newGame->next = games;
        games = newGame;
    }

    int countGames() const
    {
        int count = 0;
        GameScore *current = games;
        while (current)
        {
            count++;
            current = current->next;
        }
        return count;
    }
};

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

public:
    GameTree() : root(nullptr) {}

    void insert(const Game &data)
    {
        root = insertNode(root, data);
    }

    GameNode *search(const string &gameID) const
    {
        return searchNode(root, gameID);
    }

    void remove(const string &gameID)
    {
        root = deleteNode(root, gameID);
    }

    // Preorder traversal to save to CSV
    void preorderTraversal(GameNode *node, ofstream &out) const
    {
        if (node)
        {
            out << node->data.gameID << "," << node->data.name << "," << node->data.developer << ","
                << node->data.publisher << "," << node->data.rating << "," << node->data.downloads << "\n";
            preorderTraversal(node->left, out);
            preorderTraversal(node->right, out);
        }
    }

    void saveToCSV(const string &filename) const
    {
        ofstream out(filename);
        preorderTraversal(root, out);
        out.close();
    }

    void displayLayers(GameNode *node, int currentLayer, int maxLayers) const
    {
        if (!node || currentLayer > maxLayers)
            return;
        cout << "Layer " << currentLayer << ": " << node->data.gameID << "Developer Name  " << node->data.developer << "Game Name" << node->data.name << "Game Publisher" << node->data.publisher << "\n";
        displayLayers(node->left, currentLayer + 1, maxLayers);
        displayLayers(node->right, currentLayer + 1, maxLayers);
    }

    void displayLayers(int n) const
    {
        displayLayers(root, 1, n);
    }
};

// DBMS Class with Player Management
class GameDBMS
{
private:
    Player *players[100]; // Fixed-size array for players (up to 100 players)
    int numPlayers;

public:
    GameTree gameTree;
    GameDBMS() : numPlayers(0) {}

    // Load data from CSV files
    void loadGames(const string &filename)
    {
        ifstream file(filename);
        string line;
        while (getline(file, line))
        {
            string id, name, dev, pub, rating, downloads;
            size_t pos = 0;
            pos = line.find(",");
            id = line.substr(0, pos);
            line.erase(0, pos + 1);
            pos = line.find(",");
            name = line.substr(0, pos);
            line.erase(0, pos + 1);
            pos = line.find(",");
            dev = line.substr(0, pos);
            line.erase(0, pos + 1);
            pos = line.find(",");
            pub = line.substr(0, pos);
            line.erase(0, pos + 1);
            pos = line.find(",");
            rating = line.substr(0, pos);
            line.erase(0, pos + 1);
            downloads = line;

            gameTree.insert(Game(id, name, dev, pub, stod(rating), stoi(downloads)));
        }
    }

    // Add a new game to the game tree
    void addGame(const Game &game)
    {
        gameTree.insert(game);
    }

    // Add a new player
    void addPlayer(const Player &player)
    {
        if (numPlayers < 100)
        {
            players[numPlayers] = new Player(player);
            numPlayers++;
        }
        else
        {
            cout << "Maximum number of players reached.\n";
        }
    }

    // Query Functions
    void searchGame(const string &gameID)
    {
        GameNode *result = gameTree.search(gameID);
        if (result)
            cout << "Game found: " << result->data.name << "\n";
        else
            cout << "Game not found.\n";
    }

    void deleteGame(const string &gameID)
    {
        gameTree.remove(gameID);
    }

    void saveGames(const string &filename)
    {
        gameTree.saveToCSV(filename);
    }

    void showLayers(int n)
    {
        gameTree.displayLayers(n);
    }

    void showPlayerDetails(const string &playerID)
    {
        bool found = false;
        for (int i = 0; i < numPlayers; i++)
        {
            if (players[i]->playerID == playerID)
            {
                found = true;
                Player &player = *players[i];
                cout << "Player Name: " << player.name << "\n";
                cout << "Player Contact: " << player.contact << "\n";
                cout << "Player Email: " << player.email << "\n";
                cout << "Games Played:\n";
                GameScore *current = player.games;
                while (current)
                {
                    GameNode *gameNode = gameTree.search(current->gameID);
                    if (gameNode)
                    {
                        cout << "- " << gameNode->data.name << " (Score: " << current->score << ")\n";
                    }
                    current = current->next;
                }
                break;
            }
        }
        if (!found)
        {
            cout << "Player not found.\n";
        }
    }

    void checkPlayerHasPlayed(const string &playerID, const string &gameID)
    {
        bool found = false;
        for (int i = 0; i < numPlayers; i++)
        {
            if (players[i]->playerID == playerID)
            {
                found = true;
                Player &player = *players[i];
                GameScore *current = player.games;
                while (current)
                {
                    if (current->gameID == gameID)
                    {
                        cout << "Player has played the game " << gameID << "\n";
                        return;
                    }
                    current = current->next;
                }
                cout << "Player has not played the game " << gameID << "\n";
                break;
            }
        }
        if (!found)
        {
            cout << "Player not found.\n";
        }
    }

    // Show top N players by the number of games played
    void topNPlayers(int N)
    {
        for (int i = 0; i < numPlayers; i++)
        {
            Player &player = *players[i];
            cout << player.playerID << " (Games Played: " << player.countGames() << ")\n";
        }
    }
};

int main()
{
    GameDBMS dbms;

    // Load games data from the CSV file
    dbms.loadGames("Games.txt");

    Player player("0291782995", "Ali ASjad", "032937458834", "0291782995@nu.edu.pk");
    dbms.addPlayer(player);

    // Add game scores for the player
    player.addGameScore("3584218103", 50.8);
    player.addGameScore("3269995415", 32.4);
    player.addGameScore("7442373216", 76.5);

    dbms.showPlayerDetails("0291782995");

    // Check if a specific player has played a game
    dbms.checkPlayerHasPlayed("0291782995", "9876543210");

    dbms.topNPlayers(1);

    dbms.saveGames("UpdatedGames.csv");

    cout << "\nSearching for game with ID '9721733099':\n";
    dbms.searchGame("9721733099");

    cout << "\nSearching for game with ID '3667310249':\n";
    dbms.searchGame("3667310249");

    cout << "\nAdding a new game...\n";
    Game newGame("9876543210", "ghost of tsushima", "suckerpunch", "Bandai", 4.5, 15000);
    dbms.gameTree.insert(newGame);

    cout << "\nSearching for the newly added game with ID '9876543210':\n";
    dbms.searchGame("9876543210");

    cout << "\nDeleting game with ID '3584218103'...\n";
    dbms.deleteGame("3584218103");

    cout << "\nSearching for game with ID '3584218103' after deletion:\n";
    dbms.searchGame("3584218103");

    cout << "\nSaving current games to 'saved_games.csv'...\n";
    dbms.saveGames("saved_games.csv");

    int displayLayers = 3;
    cout << "\nDisplaying the first " << displayLayers << " layers of the game tree:\n";
    dbms.showLayers(displayLayers);

    return 0;
}
