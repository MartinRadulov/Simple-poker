/**
*
* Solution to course project # 10
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2024/2025
*
* @author Martin Radulov
* @idnumber 9MI0600496 @compiler VC
*
* Simplified Poker
*
*/
#include <iostream>
#include <fstream>

//Initiazlize all needed constants
const size_t CARD_AMOUNT = 32, NUMBER_SUITS = 4, ROW_SIZE = 2, ARRAY_SIZE = 128;
const size_t MIN_PLAYERS = 2, MAX_PLAYERS = 9, PLAYER_CARDS = 3, CHIP_VALUE = 10;
const size_t ACE_PAIR = 22, SEVEN_PAIR = 23, THREE_SEVENS = 34;

//Create structs for the player and the deck
struct Deck
{
    char* suitless;
    char suits[4];
    char** cards;
};

struct Player
{
    char** hands;
    int* plPots;
    bool* activePl;
};

struct GameLogic
{
    //size_t plCount;
    size_t originalCount;
    size_t playerTracker;
    size_t pot;
    size_t betAmount;
    bool roundEnded;
    bool stateOfGame;
    size_t currentRaiser;
    bool raiseOccured;
    size_t turnsTaken;
    bool tieActive;
    size_t maxCombo;
};

//Initiazlize all needed fucntions
void initLogic(GameLogic& game, size_t plCount);
void initDeck(Deck& deck);
void freeDeck(Deck& deck);
void initPlayer(Player& player, size_t count);
void freePlayer(Player& player);
void resetVariables(GameLogic& game, size_t plCount);
void consoleMessage1();
void createDynamicChar(char** a, size_t size);
void createDynamicBool(bool* a);
void freeMemory(char** a, size_t size);
void createDeckNumbers(char* a);
void combineChars(const char* a, const char* b, char** comb);
bool strCompare(const char* str1, const char* str2);
size_t playerCount();
void shuffleCards(char** a);
void givePlHands(Deck& deck, Player& player, size_t plCount);
void giveChips(Player& player, size_t plCount); 
void foldPlayer(Player& player, size_t& playerIndex, size_t& plCount, GameLogic& game);
void consoleMessage2(int* plPots, size_t plCount);
size_t giveValue(char** a, size_t cardIndex, size_t playerTracker);
size_t highestValue(Player& player, size_t playerTracker);
size_t cardCombs(Player& player, size_t playerTracker);
void playerMessage(Player& player, GameLogic& game, size_t& plCount);
void playerWin(Player& player, GameLogic& game, size_t& plCount);
void checkWinner(Player& player, size_t& plCount, GameLogic& game);
void winningMessage(Player& player, GameLogic& game, size_t& plCount);
void deletePlayer(Player& player, GameLogic& game, size_t index, size_t& plCount);
void consoleMessageEnd(GameLogic& game, Player& player, size_t& winner);
void checkCombo(GameLogic& game, Player& player, size_t& winner);
size_t biggestNumber(size_t a, size_t b, size_t c);
void tieRound(GameLogic& game, Player& player, size_t& plCount);
void reduceBlind(Player& player, GameLogic& game);
void saveFile(GameLogic& game, Player& player, size_t plCount);
void loadFile(GameLogic& game, Player& player, size_t plCount);
size_t lowestPot(Player& player, size_t plCount);
void gameProgression(Player& player, GameLogic& game, Deck& deck, size_t plCount);

int main()
{
    //Opening message
    consoleMessage1();

    //Initialize the palyer and the deck
    size_t plCount = playerCount();
    GameLogic game;
    initLogic(game, plCount);
    Deck deck;
    initDeck(deck);
    Player player;
    initPlayer(player, plCount);

    //Create the deck and shuffle it
    createDeckNumbers(deck.suitless);
    combineChars(deck.suitless, deck.suits, deck.cards);
    shuffleCards(deck.cards);

    //Give cards and chips to players
    givePlHands(deck, player, plCount);
    giveChips(player, plCount);

    //Ask to load the save
    loadFile(game, player, plCount);


    //The whole logic about the progression of the game
    gameProgression(player, game, deck, plCount);

    //Free memory
    freeDeck(deck);
    freePlayer(player);

    return 0;
}

void initLogic(GameLogic& game, size_t plCount)
{
    //Initialize the game variebles
    game.originalCount = plCount;
    game.playerTracker = 1;
    game.pot = 0;
    game.betAmount = 0;
    game.roundEnded = false;
    game.stateOfGame = true;
    size_t currentRaiser = 0;
    bool raiseOccured = false;
    size_t turnsTaken = 0;
    game.tieActive = false;
}

void initPlayer(Player& player, size_t count)
{
    //Initialize the player variebles
    player.hands = new char* [ARRAY_SIZE];
    createDynamicChar(player.hands, count);
    player.plPots = new int[ARRAY_SIZE];
    player.activePl = new bool[ARRAY_SIZE];
    createDynamicBool(player.activePl);
}

void freePlayer(Player& player)
{
    //Free the memory about the player
    freeMemory(player.hands, ARRAY_SIZE);
    delete[] player.plPots;
    delete[] player.activePl;
}

void resetVariables(GameLogic& game, size_t plCount)
{
    //Reset variebles to default values
    game.currentRaiser = 0;
    game.raiseOccured = false;
    game.stateOfGame = false;
    game.roundEnded = false;
    game.turnsTaken = 0;
    game.playerTracker = 1;
    game.betAmount = 0;
}

void initDeck(Deck& deck)
{
    //Initialize the deck variebles
    deck.suitless = new char[CARD_AMOUNT];
    deck.suits[0] = 'C';
    deck.suits[1] = 'H';
    deck.suits[2] = 'D';
    deck.suits[3] = 'S';
    deck.cards = new char* [CARD_AMOUNT];
    createDynamicChar(deck.cards, CARD_AMOUNT);
}

void freeDeck(Deck& deck)
{
    //Free the deck
    delete[] deck.suitless;
    freeMemory(deck.cards, CARD_AMOUNT);
}

void consoleMessage1()
{
    //Prints out the opening message
    std::cout << "Welcome to simple poker!" << std::endl;
    std::cout << "------------------------" << std::endl;
    std::cout << "How many players are going to play?" << std::endl;
    std::cout << "Enter player amount:" << std::endl;
    std::cout << "(Will be overwriten if you load save)" << std::endl;
}

void createDynamicChar(char** a, size_t size)
{
    //Creates a dynamic char array
    for (int i = 0; i < size; i++)
    {
        a[i] = new char[ROW_SIZE];
    }
}

void createDynamicBool(bool* a)
{
    //Creates a bool arrays with every element being "true"
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        a[i] = true;
    }
}

void freeMemory(char** a, size_t size)
{
    //Frees memory, used for the dynamic double arrays
    for (int i = 0; i < size; i++)
    {
        delete[] a[i];
    }
}

size_t playerCount()
{
    //Checks if the allowed number of players are playing and lets you input the amount
    size_t count = 0;
    std::cin >> count;
    if (count < MIN_PLAYERS || count > MAX_PLAYERS)
    {
        std::cout << "Only a player amount of 2-9 is allowed!";
        return 1;
    }
    return count;
}

bool strCompare(const char* str1, const char* str2)
{
    //Basic string compare function
    while (*str1 && *str2)
    {
        if (*str1 != *str2)
        {
            return false;
        }
        str1++;
        str2++;
    }
    return *str1 == '\0' && *str2 == '\0';
}

void createDeckNumbers(char* a)
{
    //Make four cards for every rank from 7 - Ace
    int i = 0, counter = 0;
    char ranks[] = { '7', '8', '9', 'D', 'J', 'Q', 'K', 'A' };
    for (int j = 0; j < CARD_AMOUNT; j++)
    {
        a[j] = ranks[i];
        counter++;
        if (counter == 4)
        {
            i++;
            counter = 0;
        }
    }
}

void combineChars(const char* a, const char* b, char** comb)
{
    //Using a basic function for combining chars
    int suit = 0;
    for (int j = 0; j < CARD_AMOUNT; j++)
    {
        comb[j][0] = a[j];
        comb[j][1] = b[suit];
        suit++;
        if ((suit == NUMBER_SUITS))
        {
            suit = 0;
        }
    }
}

void shuffleCards(char** a)
{
    //Set a random srand seed so the shuffle is random every time
    //Then change the i card with the random card
    std::srand(time(0)); //Using time() because there isnt a "true" random function
    for (int i = 0; i < CARD_AMOUNT; i++)
    {
        size_t random = i + rand() % (CARD_AMOUNT - i);
        char temp[ROW_SIZE];
        temp[0] = a[random][0];
        temp[1] = a[random][1];
        a[random][0] = a[i][0];
        a[random][1] = a[i][1];
        a[i][0] = temp[0];
        a[i][1] = temp[1];
    }
}

void givePlHands(Deck& deck, Player& player, size_t plCount)
{
    //Give each player 3 cards, all cards having a rank and a suit
    size_t count = 0;
    for (int i = 0; i < plCount; i++)
    {
        player.hands[i] = new char[PLAYER_CARDS * 2];
        size_t j = 0;
        for (int cards = 0; cards < PLAYER_CARDS; cards++)
        {
            player.hands[i][j] = deck.cards[count][0];
            j++;
            player.hands[i][j] = deck.cards[count][1];
            count++;
            j++;
        }
    }
}

void giveChips(Player& player, size_t plCount)
{
    //Give every player a starting value of 1000 chips
    for (int i = 0; i < plCount; i++)
    {
        player.plPots[i] = 100 * CHIP_VALUE;
    }
}

void foldPlayer(Player& player, size_t& playerIndex, size_t& plCount, GameLogic& game)
{
    //Set the player as inactive
    player.activePl[playerIndex] = false;

    //Reduce the amount of palyers
    plCount--;
    
    //Logic when only one active player remains
    size_t activePlayerCount = 0;
    for (size_t i = 0; i < game.originalCount; i++)
    {
        if (player.activePl[i])
        {
            activePlayerCount++;
        }
    }
    if (activePlayerCount == 1)
    {
        checkWinner(player, plCount, game);
    }
}

void consoleMessage2(int* plPots, size_t plCount)
{
    //Makes a grid that dispalys players pots
    for (int i = 0; i < plCount; i++)
    {
        if ((i + 1) % 3 == 0)
        {
            std::cout << std::endl;
        }
        std::cout << "Player" << (i + 1) << ": " << plPots[i] << "| ";
    }
    std::cout << std::endl;
    std::cout << "Press enter to continue..." << std::endl;
    std::cin.ignore();
    std::cin.get();
}

size_t giveValue(char** a, size_t cardIndex, size_t playerTracker)
{
    //Gives the value of a card
    if (a[playerTracker][cardIndex] == 'A')
    {
        return 11;
    }
    else
    {
        switch (a[playerTracker][cardIndex])
        {
        case 'J':
        case 'Q':
        case 'K':
        case 'D': return 10;
            break;
        default: return a[playerTracker][cardIndex] - '0';
        }
    }
}

size_t highestValue(Player& player, size_t playerTracker)
{
    //Function for finding the highest value card in a player's hand
    size_t tempMax = 0, tempSize = PLAYER_CARDS * 2;
    for (int i = 0; i < tempSize; i += 2)
    {
        size_t cardValue = giveValue(player.hands, i, playerTracker);
        if (cardValue > tempMax)
        {
            tempMax = cardValue;
        }
    }
    return tempMax;
}

size_t cardCombs(Player& player, size_t playerTracker)
{
    //Initialize all necessary variables
    size_t combAmount = 0;
    size_t flushAmount = 0;
    size_t tempSize = PLAYER_CARDS * 2;
    size_t rankCounter = 1;
    size_t aceCounter = 0;
    size_t sevenCounter = 0;
    size_t flushCounter = 1;
    size_t jokerFlush = 0;
    bool joker = false;
    size_t highCard = highestValue(player, playerTracker); //Calculates high card
    size_t lastCard = giveValue(player.hands, 4, playerTracker); //Searches for the last card in hand

    //Search for the joker
    for (int rank = 0, suit = 1; rank < tempSize; rank += 2, suit += 2)
    {
        if (player.hands[playerTracker][rank] == '7' && player.hands[playerTracker][suit] == 'C')
        {
            joker = true;

            //Makes combos easier
            rankCounter++;
            aceCounter++;
            jokerFlush++;
        }

        //Searches for seven or A
        //(cannot be done in the other fors because they have different loop logic)
        if (player.hands[playerTracker][rank] == '7')
        {
            sevenCounter++;
        }
        if (player.hands[playerTracker][rank] == 'A')
        {
            aceCounter++;
        }
    }

    //Go thru the hand to look for Three of a kind
    for (int rank = 0, suit = 1; suit <= PLAYER_CARDS; rank += 2, suit += 2) //Only one loop is required
    {
        //Counter goes up by 1 if there are mathing ranks
        if (player.hands[playerTracker][rank] == player.hands[playerTracker][rank + 2]) //checks the next rank
        {
            rankCounter++;
            if (rankCounter == 3)
            {
                combAmount = giveValue(player.hands, rank, playerTracker) * rankCounter; //Combo is the card * 3
            }
        }

        //If there are three sevens immidietly return 34
        if (sevenCounter == 3)
        {
            return THREE_SEVENS;
        }
    }

    //Check for flush
    for (int suitFirst = 0; suitFirst < PLAYER_CARDS; suitFirst++)
    {
        for (int suitNext = suitFirst + 1; suitNext < PLAYER_CARDS; suitNext++)
        {
            size_t rank1 = suitFirst * 2;
            size_t rank2 = suitNext * 2;
            if (player.hands[playerTracker][rank1 + 1] == player.hands[playerTracker][rank2 + 1])
            {
                flushCounter++;
                if (flushCounter == 2 || (flushCounter + jokerFlush) == 2) //Joker flush has to be a separate variable
                {
                    flushAmount = giveValue(player.hands, rank1, playerTracker) + giveValue(player.hands, rank2, playerTracker);
                }
                else if (flushCounter == 3 || (flushCounter + jokerFlush) == 3) //A flush with 2 or 3 cards is possible
                {
                    flushAmount += lastCard;
                }
            }
        }
    }
    
    //Give +11 if a joker is in hand
    if (joker = true)
    {
        combAmount += 11;
    }

    //Determine what combAmount should be
    if (aceCounter == 2 && combAmount < ACE_PAIR)
    {
        combAmount = ACE_PAIR;
    }
    if (sevenCounter == 2 && combAmount < SEVEN_PAIR)
    {
        combAmount = SEVEN_PAIR;
    }
    
    //If more then one combinations possible, return the highest
    return biggestNumber(flushAmount, combAmount, highCard);
}

void playerMessage(Player& player, GameLogic& game, size_t& plCount)
{
    size_t tempTracker = game.playerTracker - 1;
    game.turnsTaken++;

    //Clears the console
    std::cout << "\x1B[2J\x1B[H";

    //Display players cards and chips
    std::cout << "Current pot: " << game.pot << std::endl;
    std::cout << "Current bet: " << game.betAmount << std::endl << std::endl;

    int cards = PLAYER_CARDS * 2;
    char* choice = new char[ARRAY_SIZE];
    std::cout << "Player" << game.playerTracker << ":" << std::endl;
    std::cout << "Chips: " << player.plPots[tempTracker] << std::endl;
    for (int i = 0; i < cards; i++)
    {
        std::cout << player.hands[tempTracker][i++] << player.hands[tempTracker][i] << " ";
    }
    std::cout << std::endl;

    //Set players action
    std::cout << "raise/fold/check?" << std::endl;
    std::cin >> choice;
    if (strCompare(choice, "raise"))
    {
        size_t raiseAmount = 0;
        std::cout << "Raise to:" << std::endl;
        std::cin >> raiseAmount;
        if (raiseAmount > game.betAmount && raiseAmount <= lowestPot(player, plCount) && raiseAmount > 1)
        {
            game.betAmount = raiseAmount * CHIP_VALUE; //It works with chips
            game.raiseOccured = true;
            game.currentRaiser = tempTracker;
        }
        else
        {
            std::cout << "Raise is lower, automaticly check" << std::endl;
        }
        player.plPots[tempTracker] -= game.betAmount;
        game.pot += game.betAmount;

    }
    else if (strCompare(choice, "check") && game.betAmount <= player.plPots[tempTracker])
    {
        std::cout << "Checked" << std::endl;
        player.plPots[tempTracker] -= game.betAmount;
        game.pot += game.betAmount;
    }
    else if (strCompare(choice, "fold"))
    {
        std::cout << "Folded" << std::endl;
        foldPlayer(player, tempTracker, plCount, game);
    }
    else
    {
        std::cout << "Invalid input" << std::endl << "Folding" << std::endl;
        foldPlayer(player, tempTracker, plCount, game);
    }

    //Wait for input before going to the next screen
    std::cout << "Press enter to continue..." << std::endl;
    std::cin.ignore();
    std::cin.get();

    //If there has been a raise it loops back around and skips inactive player
    do
    {
        std::cout << "Advancing turn " << std::endl;
        game.playerTracker++;
        if (game.playerTracker > game.originalCount)
        {
            game.playerTracker = 1; // Wrap back to the first player
        }
        tempTracker = game.playerTracker - 1;
    }
    while (!player.activePl[tempTracker]); //Makes sure it does this at least once even without the while

    //Mark the begging of the loop around if there has been a raise and end the round when necessary
    if (game.raiseOccured)
    {
        if (game.currentRaiser == tempTracker)
        {
            game.roundEnded = true;
        }
    }
    else
    {
        if (game.turnsTaken >= game.originalCount)
        {
            game.roundEnded = true;
        }
    }

    delete[] choice;
}

void playerWin(Player& player, GameLogic& game, size_t& plCount)
{
    size_t winner = 0;
    checkCombo(game, player, winner); //Look for the highest combo

    //If more than one player have the highest combination then its a tie
    if (game.tieActive)
    {
        game.stateOfGame = true;
        return; //immidietly end the round and start the tie
    }
    player.plPots[winner] += game.pot;

    //Delete the players with 0 chips
    for (int i = game.originalCount - 1; i >= 0; i--)
    {
        if (player.plPots[i] <= 0 && player.activePl[i])
        {
            deletePlayer(player, game, i, plCount);
        }
    }
    if (game.originalCount == 1)
    {
        consoleMessageEnd(game, player, winner); //Message when only one player has chips
    }

    //Message when a player wins
    else
    {
        std::cout << "Player" << winner + 1 << " wins " << game.pot << " chips!!!" << std::endl;
        winningMessage(player, game, plCount);
    }

    //Pot is reset
    game.pot = 0;
}

void checkWinner(Player& player, size_t& plCount, GameLogic& game)
{
    //For loop to check the for the last remaming active player
    for (int i = 0; i < game.originalCount; i++)
    {
        if (player.activePl[i])
        {
            std::cout << "Player" << i + 1 << " wins as last active player!!" << std::endl;
            std::cout << game.pot << " Chips won!!" << std::endl;
            player.plPots[i] += game.pot;
            game.pot = 0;
            if (game.originalCount != 1)
            {
                winningMessage(player, game, plCount);
            }
            plCount = game.originalCount;
            return;
        }
    }
}

void winningMessage(Player& player, GameLogic& game, size_t& plCount)
{
    //If "YES" is typed continue the game, otherwise it ends
    char* continueGame = new char[ARRAY_SIZE];
    std::cout << "Continiue playin?..." << std::endl;
    std::cout << "Type (YES) to continue playing" << std::endl;
    std::cin >> continueGame;
    if (strCompare(continueGame, "YES"))
    {
        game.stateOfGame = true;
        for (int i = 0; i < game.originalCount; i++)
        {
            if (player.plPots[i] > 0)
            {
                player.activePl[i] = true;
            }
        }
    }
    else
    {
        game.stateOfGame = false;
    }
    delete[] continueGame;
}

void deletePlayer(Player& player, GameLogic& game, size_t index, size_t& plCount)
{
    //Making sure that there isn't e memory leak
    if (game.originalCount == 0 || index >= game.originalCount)
    {
        return;
    }

    //Decrease the amount of players
    game.originalCount--;
    plCount--;

    //Switch stuff with the player to the right
    for (int i = index; i < game.originalCount; i++)
    {
        player.hands[i] = player.hands[i + 1];
        player.activePl[i] = player.activePl[i + 1];
        player.plPots[i] = player.plPots[i + 1];
    }

    //Set the last pointer to null(practicly deleteing it)
    player.hands[game.originalCount] = nullptr;
}

void consoleMessageEnd(GameLogic& game, Player& player, size_t& winner)
{
    //Display the winner
    std::cout << "Player" << winner + 1 << " wins the whole game!!" << std::endl;
    std::cout << "Only one player still has chips" << std::endl;
    std::cout << "Game ending" << std::endl;
}

void checkCombo(GameLogic& game, Player& player, size_t& winner)
{
    //Going thru the hands and finding the biggest one
    for (int i = 0; i < game.originalCount; i++)
    {
        if (player.activePl[i])  // Only check active players
        {
            size_t currentComb = cardCombs(player, i);
            if (currentComb > game.maxCombo)
            {
                game.maxCombo = currentComb;
                winner = i;
            }
        }
    }

    //Search for a tie
    size_t tieCount = 0;
    for (int i = 0; i < game.originalCount; i++)
    {
        if (player.activePl[i])
        {
            size_t currentComb = cardCombs(player, i);
            if (currentComb == game.maxCombo)
            {
                tieCount++;
            }
        }
    }

    //If there is then mark a the tie as active
    if (tieCount > 1)
    {
        game.tieActive = true;
    }
}

size_t biggestNumber(size_t a, size_t b, size_t c)
{
    //Find the biggest between three numbers
    size_t tempMax = 0;
    size_t arr[3] = { a, b, c };
    for (int i = 0; i < 3; i++)
    {
        if (arr[i] > tempMax)
        {
            tempMax = arr[i];
        }
    }
    return tempMax;
}

void tieRound(GameLogic& game, Player& player, size_t& plCount)
{
    std::cout << "There is a tie" << std::endl;
    for (int i = 0; i < plCount; i++)
    {
        player.activePl[i] = false;

        //The players with the tied hand are automaticly put in the next round for free
        if (cardCombs(player, i) == game.maxCombo)
        {
            player.activePl[i] = true;
        }

        //Else you are asked to join and have to pay halve the pot
        else
        {
            char* joinGame = new char[ARRAY_SIZE];
            std::cout <<"Player" << i + 1 <<  " join the tie?..." << std::endl;
            std::cout << "Type (YES) to join" << std::endl;
            std::cin >> joinGame;
            if (strCompare(joinGame, "YES") && player.plPots[i] > game.pot / 2)
            {
                player.activePl[i] = true;
                player.plPots[i] -= game.pot / 2;
            }

            //If you have 0 chips then you are given 5
            else if(strCompare(joinGame, "YES") && player.plPots[i] == 0)
            {
                player.activePl[i] = true;
                player.plPots[i] = 5 * CHIP_VALUE;
            }
            delete[] joinGame;
        }
    }

    //Same logic as the game progression
    int j = 0;
    while (!game.roundEnded && j < plCount)
    {
        if (player.activePl[j])
        {
            playerMessage(player, game, plCount);
            if (game.stateOfGame)
            {
                break;
            }
        }
        j++;
    }

    //End the tie round
    game.tieActive = false;
}

void reduceBlind(Player& player, GameLogic& game)
{
    //Inital tax to join
    for (int i = 0; i < game.originalCount; i++)
    {
        player.plPots[i] -= CHIP_VALUE;
    }
}

void saveFile(GameLogic& game, Player& player, size_t plCount)
{
    //Creates the file
    std::ofstream saveFile("Save.txt");

    //Inout the player amount and their chips
    if (saveFile)
    {
        saveFile << game.originalCount;
        saveFile << std::endl;
        saveFile << plCount;
        saveFile << std::endl;
        for (int i = 0; i < game.originalCount; i++)
        {
            saveFile << player.plPots[i] << " ";
        }
        saveFile << std::endl;
        saveFile.close();
    }

    //Look for an error with the oppening of the file
    else
    {
        std::cerr << "Failed to open file" << std::endl;
    }
}

void loadFile(GameLogic& game, Player& player, size_t plCount)
{
    //Ask if you want to load
    std::cout << "Do you want to continue from save?" << std::endl;
    std::cout << "(New game will start if save doesn't exist)" << std::endl;
    char* loadGame = new char[ARRAY_SIZE];
    std::cout << "Type (YES) to load" << std::endl;
    std::cin >> loadGame;

    //If YES then just load the player count and chip amount
    if (strCompare(loadGame, "YES"))
    {
        std::ifstream saveFile("Save.txt");

        if (saveFile) //Checks if the file opens
        {
            saveFile >> game.originalCount;
            saveFile >> plCount;
            for (int i = 0; i < game.originalCount; i++)
            {
                saveFile >> player.plPots[i];
            }
            saveFile.close();
            std::cout << "Loading file..." << std::endl;
        }

        //If it doesn't the game goes into not loaded mode
        else
        {
            std::cout << "New game starting..." << std::endl;
            giveChips(player, plCount);
        }
    }

    //Before the option for loading the file the inital variables are already initialized, loading just overwrites them
    //If you dont want to load the game continues witch the default variebles
    else
    {
        std::cout << "New game starting..." << std::endl;
        giveChips(player, plCount);
    }
    delete[] loadGame;
}

size_t lowestPot(Player& player, size_t plCount)
{
    //Finds the lowest pot
    size_t tempMin = 1e10;
    for (int i = 0; i < plCount; i++)
    {
        if (player.plPots[i] < tempMin)
        {
            tempMin = player.plPots[i];
        }
    }
    return tempMin;
}

void gameProgression(Player& player, GameLogic& game, Deck& deck, size_t plCount)
{
    //Runs while the game is set to true and there are enough players
    while (game.stateOfGame && plCount > 1)
    {
        //If there has been a tie activate the tie round and give new cards
        if (game.tieActive)
        {
            shuffleCards(deck.cards);
            givePlHands(deck, player, plCount);
            tieRound(game, player, plCount);
        }

        //Reshuffle the deck and deal new cards if the game continues
        reduceBlind(player, game);
        resetVariables(game, plCount);
        plCount = game.originalCount;
        shuffleCards(deck.cards);
        givePlHands(deck, player, plCount);
        consoleMessage2(player.plPots, plCount);
        game.maxCombo = 0;

        //Logic about the rounds
        while (!game.roundEnded)
        {
            playerMessage(player, game, plCount);
            if (game.stateOfGame)
            {
                break;
            }
        }
        if (game.stateOfGame)
        {
            continue;
        }

        //Function about the winnig player and at the end saves your progression
        playerWin(player, game, plCount);
        saveFile(game, player, plCount);
    }
}