#include <iostream>

const size_t CARD_AMOUNT = 32, NUMBER_SUITS = 4, ROW_SIZE = 2, ARRAY_SIZE = 128;
const size_t MIN_PLAYERS = 2, MAX_PLAYERS = 9, PLAYER_CARDS = 3, CHIP_VALUE = 10;

void consoleMessage1();
void createDeckNumbers(char* a);
void combineChars(const char* a, const char* b, char comb[][ROW_SIZE]);
void outputChArray(const char* a);
void outputDeck(const char a[][ROW_SIZE]);
size_t playerCount();
void shuffleCards(char a[][ROW_SIZE]);
void givePlHands(char deck[][ROW_SIZE], char hands[][ARRAY_SIZE], size_t plCount, size_t& stopPoint);
void giveChips(int plPots[ARRAY_SIZE], size_t plCount);
void consoleMessage2(int plPots[ARRAY_SIZE], size_t plCount);
void playerMessage(char hands[][ARRAY_SIZE], size_t& playerTracker, size_t plCount);

int main()
{
    // Opening message
    consoleMessage1();

    // Initialize variables
    size_t plCount = playerCount();
    size_t stopPoint = 0;
    size_t playerTracker = 1;

    char suitless[CARD_AMOUNT];
    char suits[] = { 'C', 'H', 'D', 'S' };
    char deck[CARD_AMOUNT][ROW_SIZE];
    char hands[ARRAY_SIZE][ARRAY_SIZE];
    int plPots[ARRAY_SIZE];

    // Create the deck and shuffle it
    createDeckNumbers(suitless);
    combineChars(suitless, suits, deck);
    shuffleCards(deck);

    // Distribute cards and assign chips to players
    givePlHands(deck, hands, plCount, stopPoint);
    giveChips(plPots, plCount);

    // Display initial chip amounts
    consoleMessage2(plPots, plCount);

    // Example usage of playerMessage
    for (size_t i = 0; i < plCount; i++) {
        playerMessage(hands, playerTracker, plCount);
    }
}

void consoleMessage1()
{
    std::cout << "Welcome to simple poker!" << std::endl;
    std::cout << "------------------------" << std::endl;
    std::cout << "How many players are going to play?" << std::endl;
    std::cout << "Enter player amount:" << std::endl;
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

void combineChars(const char* a, const char* b, char comb[][ROW_SIZE])
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

void shuffleCards(char a[][ROW_SIZE])
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

void givePlHands(char deck[][ROW_SIZE], char hands[][ARRAY_SIZE], size_t plCount, size_t& stopPoint)
{
    size_t count = 0;
    for (int i = 0; i < plCount; i++)
    {
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

void giveChips(int plPots[ARRAY_SIZE], size_t plCount)
{
    for (int i = 0; i < plCount; i++)
    {
        plPots[i] = 10 * CHIP_VALUE;
    }
}

void consoleMessage2(int plPots[ARRAY_SIZE], size_t plCount)
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

void playerMessage(char hands[][ARRAY_SIZE], size_t& playerTracker, size_t plCount)
{
    int temp = playerTracker - 1, cards = PLAYER_CARDS * 2;
    std::cout << "Player" << playerTracker << ":" << std::endl;

    for (int i = 0; i < cards; i++)
    {
        std::cout << hands[temp][i++] << hands[temp][i] << " ";
    }
    std::cout << std::endl;

    playerTracker++;

    if (playerTracker > plCount) {
        playerTracker = 1;
    }
}