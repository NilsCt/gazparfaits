#include <iostream>
#include <vector>
#include "Enceinte.h"
using namespace std;


// Tests associés à la classe Enceinte
int main () {
	Enceinte enceinte1;
	Enceinte enceinte2(50,60, 20);
	Enceinte enceinte3(enceinte2);
    vector<Enceinte> enceintes({enceinte1, enceinte2, enceinte3});

    for(auto const& enceinte : enceintes) {
        cout << enceinte;
    }

	return 0;
}
