#include <iostream>; 
#include <stdlib.h>
#include <string> 

using namespace std; 

// @ Declarations
struct ICard {
	bool available; 
	string suit; 
	int number; 
	bool onPlayerDeck; 
};

ICard Cards[48]; 

string Suits[4] = { "Corazones", "Picas", "Treboles", "Diamantes" };

int playerSum = 0; 
int croupierSum = 0; 

// @ Functions 
void generateCards() {
	int activeCard = 0;

	for (int suitIndex = 0; suitIndex < 4; suitIndex++) {
		for (int cardIndex = 1; cardIndex < 13; cardIndex++) {
			Cards[activeCard].available = true;
			Cards[activeCard].suit = Suits[suitIndex];
			Cards[activeCard].number = cardIndex + 1;
			Cards[activeCard].onPlayerDeck = false; 

			activeCard++; 
		}
	}

	/*
		@ Debug: Preview from cards.

		for (int cardIndex = 0; cardIndex < 48; cardIndex++) {
			cout << cardIndex << " - " << Cards[cardIndex].number << " - " << Cards[cardIndex].suit << endl;
		}
	*/
} 

bool isCardAvailable(int reqNumber, string reqSuit) {
	for (int cardIndex = 0; cardIndex < 48; cardIndex++) {
		if (Cards[cardIndex].suit == reqSuit && Cards[cardIndex].number == reqNumber) {
			return Cards[cardIndex].available; 
		}
	}

	return false; 
}

int getRandomCardIndex() {
	int randomIndex = rand() % 48;

	if (randomIndex < 48) {
		if (Cards[randomIndex].available) {
			return randomIndex; 
		}
	}

	return getRandomCardIndex(); 
} 

int isAs() {
	int selected;
	cout << "Ha salido un AS, quieres seleccionar 1 o 11 (Tus cartas suman " << playerSum << "): ";
	cin >> selected;

	if (selected == 1 or selected == 11) {
		return selected; 
	} 

	return isAs(); 
}

void renderPlayerCards() {
	int counter = 1; 

	for (int cardIndex = 0; cardIndex < 48; cardIndex++) {
		if (!Cards[cardIndex].available && Cards[cardIndex].onPlayerDeck) {
			cout << counter << ". Baza: " << Cards[cardIndex].suit << ", numero: " << Cards[cardIndex].number << endl;
			counter++; 
		}
	}
}

void renderCroupierCards() {
	int counter = 1;

	for (int cardIndex = 0; cardIndex < 48; cardIndex++) {
		if (!Cards[cardIndex].available && !Cards[cardIndex].onPlayerDeck) {
			cout << counter << ". [Croupier] Baza: " << Cards[cardIndex].suit << ", numero: " << Cards[cardIndex].number << endl;
			counter++;
		}
	}
}

void cardSelection() {
	int firstCard = getRandomCardIndex();
	int secondCard = getRandomCardIndex(); 
	int selected; 

	system("CLS");
	renderPlayerCards(); 

	cout << "Tienes dos cartas por elegir, quieres seleccionar carta 1 o 2: "; 
	cin >> selected; 
	int selectedCardIndex; 
	int otherIndex; 

	if (selected == 1) {
		selectedCardIndex = firstCard; 
		otherIndex = secondCard; 
	}
	else if(selected == 2) {
		selectedCardIndex = secondCard;
		otherIndex = firstCard;
	}
	else {
		return cardSelection();
	}

	// @ Player 

	Cards[selectedCardIndex].available = false;
	Cards[selectedCardIndex].onPlayerDeck = true;

	int currentNum = Cards[selectedCardIndex].number;

	if (currentNum == 13) {
		currentNum = isAs();

	}
	else if (currentNum >= 10) {
		currentNum = 10;
	}

	playerSum += currentNum;

	// @ Player loose the game.
	if (playerSum > 21) {
		return; 
	}

	// @ Croupier 

	Cards[otherIndex].available = false;
	currentNum = Cards[otherIndex].number;

	if (currentNum == 13) {
		if ((croupierSum + 11) > 21) {
			currentNum = 1;
		}
		else {
			currentNum = 11;
		}

	} 
	else if (currentNum >= 10) {
		currentNum = 10;
	}

	croupierSum += currentNum;
}

int main() {
	generateCards(); 
	cardSelection(); 
	
	while (true) {

		if (playerSum > 21) { 
			cout << "Ha ganado el croupier, has superado 21." << endl;
			break;
		}
		else if (croupierSum > 21) { 
			cout << "Has ganado, el croupier ha superado 21." << endl;
			break;
		}

		string playerInput;

		cout << "Quieres seguir cogiendo cartas? (Tus cartas suman " << playerSum << ") [si/no]" << endl;
		cin >> playerInput;

		if (playerInput == "si") {
			cardSelection();
		}
		else { 
			system("CLS");

			if (playerSum > croupierSum) {
				cout << "Has ganado, tienes una cantidad de cartas superior a la del croupier" << endl;
			}
			else if (playerSum == croupierSum) {
				cout << "Habéis empatado" << endl;
			}
			else {
				cout << "Ha ganado el croupier" << endl;
			}

			break;
		}
	};  

	renderPlayerCards(); 
	renderCroupierCards(); 
}; 