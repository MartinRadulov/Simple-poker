#include <iostream>

const size_t CARD_AMOUNT = 32, NUMBER_SUITS = 4, ROW_SIZE = 2, ARRAY_SIZE = 128;
const size_t MIN_PLAYERS = 2, MAX_PLAYERS = 9, PLAYER_CARDS = 3, CHIP_VALUE = 10;

void consoleMessage1();
void createDynamicChar(char** a, size_t size);
void createDynamicBool(bool* a);
void freeMemory(char** a, size_t size);
void createDeckNumbers(char* a);
void combineChars(const char* a, const char* b, char** comb);
void outputChArray(const char* a);
void outputDeck(const char** a);
bool strCompare(const char* str1, const char* str2);
size_t playerCount();
void shuffleCards(char** a);
void givePlHands(char** deck, char** hands, size_t plCount, size_t& stopPoint);
void giveChips(int* plPots, size_t plCount);
void foldPlayer(size_t& playerIndex, size_t& plCount, bool* activePl);
void consoleMessage2(int* plPots, size_t plCount);
void playerMessage(char** hands, size_t& playerTracker, size_t plCount, int* plPots, size_t& pot, size_t& betAmount, bool* activePl);

int main()
{
    //Opening message
    consoleMessage1();

    //Initialize variables
    size_t plCount = playerCount();
    size_t stopPoint = 0;
    size_t playerTracker = 1;
    size_t pot = 0;
    size_t betAmount = 0;
    char* suitless = new char[CARD_AMOUNT];
    char suits[] = { 'C', 'H', 'D', 'S' };
    char** deck = new char* [CARD_AMOUNT];
    createDynamicChar(deck, CARD_AMOUNT);
    char** hands = new char* [ARRAY_SIZE];
    createDynamicChar(hands, plCount);
    int* plPots = new int[ARRAY_SIZE];
    bool* activePl = new bool[ARRAY_SIZE];
    createDynamicBool(activePl);

    //Create the deck and shuffle it
    createDeckNumbers(suitless);
    combineChars(suitless, suits, deck);
    shuffleCards(deck);

    //Give cards and chips to players
    givePlHands(deck, hands, plCount, stopPoint);
    giveChips(plPots, plCount);

    // Display initial chip amounts
    consoleMessage2(plPots, plCount);

    // Example usage of playerMessage
    for (size_t i = 0; i < plCount; i++)
    {
        playerMessage(hands, playerTracker, plCount, plPots, pot, betAmount, activePl);
    }

    //Free memory
    delete[] suitless;
    freeMemory(deck, CARD_AMOUNT);
    freeMemory(hands, ARRAY_SIZE);
    delete[] plPots;
    delete[] activePl;

    return 0;
}

void consoleMessage1()
{
    std::cout << "Welcome to simple poker!" << std::endl;
    std::cout << "------------------------" << std::endl;
    std::cout << "How many players are going to play?" << std::endl;
    std::cout << "Enter player amount:" << std::endl;
}

void createDynamicChar(char** a, size_t size)
{
    for (int i = 0; i < size; i++)
    {
        a[i] = new char[ROW_SIZE];
    }
}

void createDynamicBool(bool* a)
{
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        a[i] = true;
    }
}

void freeMemory(char** a, size_t size)
{
    for (int i = 0; i < size; i++)
    {
        delete[] a[i];
    }
}

size_t playerCount()
{
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
    while (*str1 && *str2)
    {
        if (*str1 != *str2)
        {
            return false;
        }
        str1++;
        str2++;
    }
    return true;
}

void createDeckNumbers(char* a)
{
    int i = 0, counter = 0;
    char faces1[] = { '7', '8', '9', 'D', 'J', 'Q', 'K', 'A' };
    for (int j = 0; j < CARD_AMOUNT; j++)
    {
        a[j] = faces1[i];
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

void givePlHands(char** deck, char** hands, size_t plCount, size_t& stopPoint)
{
    size_t count = 0;
    for (int i = 0; i < plCount; i++)
    {
        hands[i] = new char[PLAYER_CARDS * 2];
        size_t j = 0;
        for (int cards = 0; cards < PLAYER_CARDS; cards++)
        {
            hands[i][j] = deck[count][0];
            j++;
            hands[i][j] = deck[count][1];
            count++;
            j++;
        }
    }
    stopPoint = count; // Tracks the last used card index in the deck
}

void giveChips(int* plPots, size_t plCount)
{
    for (int i = 0; i < plCount; i++)
    {
        plPots[i] = 100 * CHIP_VALUE;
    }
}

void foldPlayer(size_t& playerIndex, size_t& plCount, bool* activePl)
{
    //Set the player as inactive
    activePl[playerIndex] = false;

    //Reduce the amount of palyers
    plCount--;
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
}

void playerMessage(char** hands, size_t& playerTracker, size_t plCount, int* plPots, size_t& pot, size_t& betAmount, bool* activePl)
{
    //Skipping the inactive players
    size_t tempTracker = playerTracker - 1;
    while (!activePl[tempTracker])
    {
        tempTracker++;
        if (tempTracker >= plCount)
        {
            tempTracker = 0;
        }
    }

    //Clears the console
    std::cout << "\x1B[2J\x1B[H";

    //Display players cards and chips
    std::cout << "Current pot: " << pot << std::endl;
    std::cout << "Current bet: " << betAmount << std::endl << std::endl;
    playerTracker = tempTracker + 1;
    int cards = PLAYER_CARDS * 2;
    char choice[ARRAY_SIZE];
    std::cout << "Player" << playerTracker << ":" << std::endl;
    std::cout << "Chips: " << plPots[tempTracker] << std::endl;
    for (int i = 0; i < cards; i++)
    {
        std::cout << hands[tempTracker][i++] << hands[tempTracker][i] << " ";
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
        if (raiseAmount > betAmount && raiseAmount <= plPots[tempTracker])
        {
            betAmount = raiseAmount;
        }
        else
        {
            std::cout << "Raise is lower, automaticly check" << std::endl;
        }
        plPots[tempTracker] -= betAmount;
        pot += betAmount;

    }
    else if (strCompare(choice, "check"))
    {
        std::cout << "Checked" << std::endl;
        plPots[tempTracker] -= betAmount;
        pot += betAmount;
    }
    else if (strCompare(choice, "fold"))
    {
        std::cout << "Folded" << std::endl;
        foldPlayer(tempTracker, plCount, activePl);
    }
    else
    {
        std::cout << "Invalid input" << std::endl << "Skipping turn" << std::endl;
    }

    //Wait for input before going to the next screen
    std::cout << "Press enter to continue..." << std::endl;
    std::cin.ignore();
    std::cin.get();

    //Update to the next player
    playerTracker++;
    if (playerTracker > plCount) {
        playerTracker = 1;
        betAmount = 0;
    }
}
