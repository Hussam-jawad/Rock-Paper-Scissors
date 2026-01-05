#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

// ===================== ENUMS =====================
// Spielentscheidungen: Stein, Papier, Schere
enum enGameChoice { Stone = 1, Paper = 2, Scissor = 3 };

// Gewinner der Runde oder Unentschieden
enum enWinner { Player = 1, Computer = 2, Draw = 3 };

// ===================== STRUCTS =====================
// Struktur für eine Runde
struct stRoundInfo
{
    short RoundNumber = 0;          // Runden Nummer
    enGameChoice PlayerChoice;      // Wahl des Spielers
    enGameChoice ComputerChoice;    // Wahl des Computers
    enWinner Winner;                // Gewinner dieser Runde
    string WinnerName;              // Name des Gewinners
};

// Struktur für das gesamte Spielergebnis
struct stGameResult
{
    short GameRounds = 0;           // Anzahl der gespielten Runden
    short PlayerWinTimes = 0;       // Spieler Siege
    short ComputerWinTimes = 0;     // Computer Siege
    short DrawTimes = 0;            // Unentschieden
    enWinner GameWinner;            // Gesamtsieger
    string WinnerName;              // Name des Gewinners
};

// ===================== UTILITIES =====================
// Zufallszahl zwischen From und To generieren
int RandomNumber(int From, int To) {
    return rand() % (To - From + 1) + From;
}

// Erzeugt Tabs für Formatierung
string Tabs(short NumberOfTabs) {
    string t = "";
    for (short i = 0; i < NumberOfTabs; i++) t += "\t";
    return t;
}

// ===================== GAME CHOICES =====================
// Computer wählt zufällig Stein, Papier oder Schere
enGameChoice GetComputerChoice() {
    return (enGameChoice)RandomNumber(1, 3);
}

// Spieler wählt Stein, Papier oder Schere
enGameChoice ReadPlayerChoice()
{
    short Choice;
    do
    {
        cout << "\nYour Choice : [1]:Stone, [2]:Paper, [3]:Scissors ? ";
        cin >> Choice;
    } while (Choice < 1 || Choice > 3);
    return (enGameChoice)Choice;
}

// Gibt Name der Wahl als String zurück
string ChoiceName(enGameChoice Choice)
{
    string arrGameChoice[3] = { "Stone","Paper","Scissor" };
    return arrGameChoice[Choice - 1];
}

// ===================== WINNER LOGIC =====================
// Bestimmt Gewinner der Runde
enWinner WhoWinTheRound(stRoundInfo RoundInfo)
{
    // Bei Gleichstand gibt es Draw
    if (RoundInfo.PlayerChoice == RoundInfo.ComputerChoice)
        return enWinner::Draw;

    // Computer gewinnt wenn:
    switch (RoundInfo.PlayerChoice)
    {
    case enGameChoice::Stone:
        if (RoundInfo.ComputerChoice == enGameChoice::Paper)
            return enWinner::Computer;
        break;
    case enGameChoice::Paper:
        if (RoundInfo.ComputerChoice == enGameChoice::Scissor)
            return enWinner::Computer;
        break;
    case enGameChoice::Scissor:
        if (RoundInfo.ComputerChoice == enGameChoice::Stone)
            return enWinner::Computer;
        break;
    }

    // Spieler gewinnt, wenn Computer nicht gewinnt
    return enWinner::Player;
}

// Bestimmt Gesamtsieger nach allen Runden
enWinner WhoWinTheGame(short PlayerWins, short ComputerWins)
{
    if (PlayerWins > ComputerWins) return enWinner::Player;
    if (ComputerWins > PlayerWins) return enWinner::Computer;
    return enWinner::Draw;
}

// Gibt Namen des Gewinners zurück
string WinnerName(enWinner Winner)
{
    string arrWinnerName[3] = { "Player","Computer","No Winner (Draw)" };
    return arrWinnerName[Winner - 1];
}

// ===================== PRINTING =====================
// Ergebnisse einer Runde anzeigen
void PrintRoundResults(stRoundInfo RoundInfo)
{
    cout << "\n____________ Round [" << RoundInfo.RoundNumber << "] ____________\n\n";
    cout << "Player Choice   : " << ChoiceName(RoundInfo.PlayerChoice) << endl;
    cout << "Computer Choice : " << ChoiceName(RoundInfo.ComputerChoice) << endl;
    cout << "Round Winner    : " << RoundInfo.WinnerName << endl;
    cout << "_______________________________________________\n";
}

// Ändert Farbe je nach Gewinner
void SetWinnerScreenColor(enWinner Winner)
{
    switch (Winner)
    {
    case enWinner::Player:
        system("color 2F");    // Grün Hintergrund für Spieler
        break;
    case enWinner::Computer:
        system("color 4F");    // Rot für Computer
        cout << "\a";          // Piep Ton
        break;
    default:
        system("color 6F");    // Gelb für Draw
        break;
    }
}

// Zeigt Game Over Bildschirm
void ShowGameOverScreen()
{
    cout << Tabs(2) << "_________________________________________________\n\n";
    cout << Tabs(2) << "                 +++ Game Over +++\n";
    cout << Tabs(2) << "_________________________________________________\n\n";
}

// Zeigt Endresultat aller Runden
void ShowFinalGameResults(stGameResult GameResults)
{
    cout << Tabs(2) << "_________________[ Game Results ]_________________\n\n";
    cout << Tabs(2) << "Game Rounds        : " << GameResults.GameRounds << endl;
    cout << Tabs(2) << "Player Wins        : " << GameResults.PlayerWinTimes << endl;
    cout << Tabs(2) << "Computer Wins      : " << GameResults.ComputerWinTimes << endl;
    cout << Tabs(2) << "Draw Times         : " << GameResults.DrawTimes << endl;
    cout << Tabs(2) << "Final Winner       : " << GameResults.WinnerName << endl;
    cout << Tabs(2) << "_________________________________________________\n\n";

    SetWinnerScreenColor(GameResults.GameWinner);
}

// ===================== CORE GAME =====================
// Füllt stGameResult Struktur mit den Ergebnissen
stGameResult FillGameResults(int GameRound, short PlayerWinTimes, short ComputerWinTimes, short DrawTimes)
{
    stGameResult GameResults;
    GameResults.GameRounds = GameRound;
    GameResults.PlayerWinTimes = PlayerWinTimes;
    GameResults.ComputerWinTimes = ComputerWinTimes;
    GameResults.DrawTimes = DrawTimes;
    GameResults.GameWinner = WhoWinTheGame(PlayerWinTimes, ComputerWinTimes);
    GameResults.WinnerName = WinnerName(GameResults.GameWinner);
    return GameResults;
}

// Spielt alle Runden und sammelt Ergebnisse
stGameResult PlayGame(short HowManyRounds)
{
    stRoundInfo RoundInfo;
    short PlayerWins = 0, ComputerWins = 0, DrawTimes = 0;

    for (short Round = 1; Round <= HowManyRounds; Round++)
    {
        cout << "\nRound [" << Round << "] begins:\n";
        RoundInfo.RoundNumber = Round;
        RoundInfo.PlayerChoice = ReadPlayerChoice();
        RoundInfo.ComputerChoice = GetComputerChoice();
        RoundInfo.Winner = WhoWinTheRound(RoundInfo);
        RoundInfo.WinnerName = WinnerName(RoundInfo.Winner);

        // Zählt Siege und Unentschieden
        if (RoundInfo.Winner == enWinner::Player)
            PlayerWins++;
        else if (RoundInfo.Winner == enWinner::Computer)
            ComputerWins++;
        else
            DrawTimes++;

        PrintRoundResults(RoundInfo);
    }

    return FillGameResults(HowManyRounds, PlayerWins, ComputerWins, DrawTimes);
}

// Anzahl Runden abfragen
short ReadHowManyRounds()
{
    short Rounds;
    do
    {
        cout << "How Many Rounds [1 - 10] ? ";
        cin >> Rounds;
    } while (Rounds < 1 || Rounds > 10);
    return Rounds;
}

// Bildschirm zurücksetzen
void ResetScreen()
{
    system("cls");
    system("color 0F"); // Standardfarbe
}

// Fragt, ob Spieler nochmal spielen will
char ReadPlayAgain()
{
    char PlayAgain;
    cout << Tabs(2) << "\nDo You want to Play Again ? Y/N ";
    cin >> PlayAgain;

    while (PlayAgain != 'Y' && PlayAgain != 'y' &&
        PlayAgain != 'N' && PlayAgain != 'n')
    {
        cout << "Invalid Choice Please enter Y or N ?? ";
        cin >> PlayAgain;
    }

    if (PlayAgain == 'N' || PlayAgain == 'n')
    {
        cout << Tabs(2) << "Game Ended. Thank you for playing.\n";
    }

    return PlayAgain;
}

// Spiel starten
void StartGame()
{
    char PlayAgain = 'Y';
    do
    {
        ResetScreen();
        stGameResult GameResults = PlayGame(ReadHowManyRounds());
        ShowGameOverScreen();
        ShowFinalGameResults(GameResults);
        PlayAgain = ReadPlayAgain();
    } while (PlayAgain == 'Y' || PlayAgain == 'y');
}

// ===================== MAIN =====================
int main()
{
    srand((unsigned)time(NULL)); // Zufallsgenerator initialisieren
    StartGame();                 // Spiel starten
    return 0;
}