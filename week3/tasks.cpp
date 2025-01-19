//If someone other than player one riases then it should loop back to the beggining
//fix joker for all combos , ace pair/flush/something else if there is any
//make possible for mulitple people to win
//da se zapisva v fail
//ako nqkoi nqma poveche pari da ne moje da igrae poveche
#include <iostream>

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

struct GameComponents
{
    size_t originalCount;

};

//Initiazlize all needed fucntions
void initDeck(Deck& deck);
void freeDeck(Deck& deck);
void initPlayer(Player& player, size_t count);
void freePlayer(Player& player);
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
void foldPlayer(Player& player, size_t& playerIndex, size_t& plCount, size_t originalCount, size_t& pot, bool& game, bool& roundEnded);
void consoleMessage2(int* plPots, size_t plCount);
size_t giveValue(char** a, size_t cardIndex, size_t playerTracker);
size_t highestValue(Player& player, size_t playerTracker);
size_t cardCombs(Player& player, size_t playerTracker);
//void raiseOccured()
void playerMessage(Player& player, size_t& playerTracker, size_t& plCount, size_t& pot, size_t& betAmount, size_t originalCount, bool& game, bool& roundEnded);
void playerWin(Player& player, size_t playerTracker, size_t& pot, bool& game, size_t originalCount, bool& roundEnded);
void checkWinner(Player& player, size_t& plCount, size_t originalCount, size_t& pot, bool& game, bool& roundEnded);
void winningMessage(Player& player, bool& game, size_t originalCount, bool& roundEnded);

int main()
{
    //Opening message
    consoleMessage1();

    //Initialize variables
    size_t plCount = playerCount();
    size_t originalCount = plCount;
    size_t playerTracker = 1;
    size_t pot = 0;
    size_t betAmount = 0;
    bool roundEnded = false;
    bool game = true;

    //Initialize the palyer and the deck
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

    //Start the game and continue if required
    while (game && plCount > 1)
    {
        //Reshuffle the deck and deal new cards if the game continues
        plCount = originalCount; // Reseting to the original amount of players
        game = false;
        shuffleCards(deck.cards);
        givePlHands(deck, player, plCount);
        consoleMessage2(player.plPots, plCount);
        playerTracker = 1;

        for (size_t i = 0; i < originalCount; i++)
        {
            playerMessage(player, playerTracker, plCount, pot, betAmount, originalCount, game, roundEnded);
        }
        if (game)
        {
            continue;
        }

        playerWin(player, playerTracker, pot, game, originalCount, roundEnded);
        //plCount = originalCount; // Reseting to the original amount of players
    }

    //Free memory
    freeDeck(deck);
    freePlayer(player);

    return 0;
}

void initPlayer(Player& player, size_t count)
{
    player.hands = new char* [ARRAY_SIZE];
    createDynamicChar(player.hands, count);
    player.plPots = new int[ARRAY_SIZE];
    player.activePl = new bool[ARRAY_SIZE];
    createDynamicBool(player.activePl);
}

void freePlayer(Player& player)
{
    freeMemory(player.hands, ARRAY_SIZE);
    delete[] player.plPots;
    delete[] player.activePl;
}

void initDeck(Deck& deck)
{
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
    //Checks if the allowed number of players are playing
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

void foldPlayer(Player& player, size_t& playerIndex, size_t& plCount, size_t originalCount, size_t& pot, bool& game, bool& roundEnded)
{
    //Set the player as inactive
    player.activePl[playerIndex] = false;

    //Reduce the amount of palyers
    plCount--;
    if (plCount == 1)
    {
        checkWinner(player, plCount, originalCount, pot, game, roundEnded);
    }
}

void consoleMessage2(int* plPots, size_t plCount)
{
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
    size_t tempMax = 0, tempSize = PLAYER_CARDS * 2;
    for (int i = 0; i < tempSize; i += 2)
    {
        if (player.hands[playerTracker][i] > tempMax)
        {
            tempMax = giveValue(player.hands, i, playerTracker);
        }
    }
    return tempMax;
}

size_t cardCombs(Player& player, size_t playerTracker)
{
    size_t combAmount = 0;
    size_t suitAmount = 0;
    size_t tempSize = PLAYER_CARDS * 2;
    size_t rankCounter = 1;
    size_t suitCounter = 0;
    size_t aceCounter = 0;
    size_t sevenCounter = 0;
    bool joker = false;
    size_t highCard = highestValue(player, playerTracker);

    //Search for the joker
    for (int rank = 0, suit = 1; rank < tempSize; rank += 2, suit += 2)
    {
        if (player.hands[playerTracker][rank] == '7' && player.hands[playerTracker][suit] == 'C')
        {
            joker = true;
            rankCounter++; //Only two cards would be needed now for the three of a kind
            sevenCounter++;
            combAmount += 11; //Gives +11 to every combination
        }
    }

    //Go thru the hand to look for combinations
    for (int rank = 0, suit = 1; suit <= PLAYER_CARDS; rank += 2, suit += 2) //Only one loop is required
    {
        //Counter goes up by 1 if there are mathing ranks
        if (player.hands[playerTracker][rank] == player.hands[playerTracker][rank + 2]) //checks the next rank
        {
            rankCounter++;
            combAmount = giveValue(player.hands, playerTracker, rank) * rankCounter;
        }

        //Check for 7 or Ace because they can also have a pair
        if (player.hands[playerTracker][rank] == '7')
        {
            sevenCounter++;
        }
        if (player.hands[playerTracker][rank] == 'A')
        {
            aceCounter++;
        }

        //If there are three sevens immidietly return 34
        if (sevenCounter == 3)
        {
            return THREE_SEVENS;
        }

        //If there is a three of kind then return the combAmount
        if (rankCounter == 3)
        {
            return combAmount;
        }

        //Checks for matching suit
        if (player.hands[playerTracker][suit] == player.hands[playerTracker][suit + 2])
        {
            combAmount += giveValue(player.hands, playerTracker, rank);
        }
    }

    //If there wasnt a three of a kind then combAmount would be flush/pair/high card
    //If there arent same suits then it would return highCard
    if (aceCounter == 2 && combAmount < ACE_PAIR)
    {
        combAmount = ACE_PAIR;
    }
    if (sevenCounter == 2 && combAmount < SEVEN_PAIR)
    {
        combAmount = SEVEN_PAIR;
    }
    if (combAmount > highCard)
    {
        return combAmount;
    }
    return highCard;
}

void playerMessage(Player& player, size_t& playerTracker, size_t& plCount, size_t& pot, size_t& betAmount, size_t originalCount, bool& game, bool& roundEnded)
{
    size_t tempTracker = playerTracker - 1;

    //Clears the console
    std::cout << "\x1B[2J\x1B[H";

    //Display players cards and chips
    std::cout << "Current pot: " << pot << std::endl;
    std::cout << "Current bet: " << betAmount << std::endl << std::endl;
    //playerTracker = tempTracker + 1;
    int cards = PLAYER_CARDS * 2;
    char* choice = new char[ARRAY_SIZE];
    std::cout << "Player" << playerTracker << ":" << std::endl;
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
        if (raiseAmount > betAmount && raiseAmount <= player.plPots[tempTracker])
        {
            betAmount = raiseAmount;
        }
        else
        {
            std::cout << "Raise is lower, automaticly check" << std::endl;
        }
        player.plPots[tempTracker] -= betAmount;
        pot += betAmount;

    }
    else if (strCompare(choice, "check"))
    {
        std::cout << "Checked" << std::endl;
        player.plPots[tempTracker] -= betAmount;
        pot += betAmount;
    }
    else if (strCompare(choice, "fold"))
    {
        std::cout << "Folded" << std::endl;
        foldPlayer(player, tempTracker, plCount, originalCount, pot, game, roundEnded);
    }
    else
    {
        std::cout << "Invalid input" << std::endl << "Skipping turn" << std::endl;
    }

    //Wait for input before going to the next screen
    std::cout << "Press enter to continue..." << std::endl;
    std::cin.ignore();
    std::cin.get();

    do
    {
        playerTracker++;
        if (playerTracker > originalCount) {
            playerTracker = 1; // Wrap back to the first player
        }
        tempTracker = playerTracker - 1;
    } while (!player.activePl[tempTracker]); //Makes sure it does this at least once even without the while

    delete[] choice;
}

void playerWin(Player& player, size_t playerTracker, size_t& pot, bool& game, size_t originalCount, bool& roundEnded)
{
    //Initialize variables
    size_t comb = 0;
    size_t winner = 0;
    size_t maxComb = 0;
    size_t tempSize = PLAYER_CARDS * 2;

    //Check for highest scoring hand
    for (int i = 0; i < originalCount; i++)
    {
        if (player.activePl[i])
        {
            size_t curretnComb = cardCombs(player, i);
            if (curretnComb > maxComb)
            {
                maxComb = curretnComb;
                winner = i;
            }
        }
    }

    //Display the winner, give him the chips and reset the pot
    std::cout << "Player" << winner + 1 << " wins " << pot << " chips!!!" << std::endl;
    player.plPots[winner] += pot;
    pot = 0;
    winningMessage(player, game, originalCount, roundEnded);
}

void checkWinner(Player& player, size_t& plCount, size_t originalCount, size_t& pot, bool& game, bool& roundEnded)
{
    for (int i = 0; i < originalCount; i++)
    {
        if (player.activePl[i])
        {
            std::cout << "Player" << i + 1 << " wins as last active player!!" << std::endl;
            std::cout << pot << " Chips won!!" << std::endl;
            player.plPots[i] += pot;
            break;
        }
    }
    pot = 0;
    winningMessage(player, game, originalCount, roundEnded);
    return;
    //roundEnded = true;

    plCount = originalCount;
}

void winningMessage(Player& player, bool& game, size_t originalCount, bool& roundEnded)
{
    char* continueGame = new char[ARRAY_SIZE];
    std::cout << "Continiue playin?..." << std::endl;
    std::cout << "Type (YES) to continue playing" << std::endl;
    std::cin.getline(continueGame, ARRAY_SIZE);
    if (strCompare(continueGame, "YES"))
    {
        game = true;
        for (size_t i = 0; i < originalCount; i++)
        {
            player.activePl[i] = true;
        }
    }
    else
    {
        game = false;
        roundEnded = false;
    }

    delete[] continueGame;
}
