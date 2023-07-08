#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <thread>
#include <array>

/*
	Chess bot notation converter
*/

using namespace std;

array<array<int, 2>, 2> coord(const string& move1, const string& move2) //converts chess notation to corodinate system
{
    array<array<int, 2>, 2> coordinates = { -1 };

    if (move1.length() < 2 || move2.length() < 2)
    {
        cout << "Invalid chess notation." << endl;
        return coordinates;
    }

    auto parseNotation = [](const string& notation) -> array<int, 2> {
        array<int, 2> coords = { -1 };
        if (notation.length() < 2)
            return coords;

        // Extract the file and rank information from the notation
        char fileChar = notation[notation.length() - 2];
        char rankChar = notation[notation.length() - 1];

        // Convert the file and rank to indices
        int file = fileChar - 'a';
        int rank = 8 - (rankChar - '0');

        if (file < 0 || file >= 8 || rank < 0 || rank >= 8)
            return coords;

        coords[0] = file;
        coords[1] = rank;
        return coords;
    };

    array<int, 2> move1Coords = parseNotation(move1);
    array<int, 2> move2Coords = parseNotation(move2);

    if (move1Coords[0] == -1 || move1Coords[1] == -1 ||
        move2Coords[0] == -1 || move2Coords[1] == -1)
    {
        cout << "Invalid chess notation." << endl;
        return coordinates;
    }

    coordinates[0] = move1Coords;
    coordinates[1] = move2Coords;

    return coordinates;
}


int main()
{
    string inputFilename = "moves.txt";
    string outputFilename = "results.txt";
    string previousMove1 = "";
    string previousMove2 = "";
    string currentContent = "";

    while (true)
    {
        ifstream fin(inputFilename); // Open the input file for reading

        if (!fin)
        {
            cout << "Unable to find file." << endl;
            return 1;
        }

        // Read the current content of the file
        stringstream buffer;
        buffer << fin.rdbuf();
        currentContent = buffer.str();

        fin.close(); // Close the input file

        // Find the last two chess notations
        string lastMove, secondLastMove;
        stringstream ss(currentContent);
        string move;

        while (ss >> move)
        {
            secondLastMove = lastMove;
            lastMove = move;
        }

        if (!lastMove.empty() && !secondLastMove.empty() &&
            (lastMove != previousMove1 || secondLastMove != previousMove2)) //checks for new information
        {
            // The last two moves have changed, process the new content
            cout << "New moves: " << secondLastMove << ", " << lastMove << endl << flush;

            // Update the previous moves
            previousMove1 = lastMove;
            previousMove2 = secondLastMove;

            size_t beforeNum = secondLastMove.find_last_of('.');
            string firstMove = secondLastMove.substr(beforeNum + 1); // splice number

            array<array<int, 2>, 2> coordinates = coord(firstMove, lastMove);

            if (coordinates[0][0] != -1 && coordinates[0][1] != -1 && coordinates[1][0] != -1 && coordinates[1][1] != -1)
            {
                cout << "Coordinates for Move 1: (" << coordinates[0][0] << ", " << coordinates[0][1] << ")" << endl << flush;
                cout << "Coordinates for Move 2: (" << coordinates[1][0] << ", " << coordinates[1][1] << ")" << endl << flush;
            }
            else
            {
                continue;// Skip writing to the output file if the coordinates are invalid
            }

            // Write the result to the output file
            ofstream fout(outputFilename, ios::app); // Open the output file in append mode
            if (!fout)
            {
                cout << "unable to output file." << endl;
                return 1;
            }
            fout << coordinates[0][0] << " " << coordinates[0][1] << " " << coordinates[1][0] << " " << coordinates[1][1] << endl;
            fout.close(); // Close the output file
        }

        // Sleep for some time before checking the file again
        this_thread::sleep_for(chrono::milliseconds(1000));
    }

    return 0;
}
