#include <iostream>
#include <vector>
using namespace std;


int main() {
    // int lucky_number = 10;
    // std::cout << "Hello, world!";
    // return 0;

    // int a = 0;
    // while (a<5) {
    //     cout << a << endl;
    //     a++;
    // }
    // return 0;

    // std::cout << "Please enter value";
    // int number;
    // std::cin >> number;
    // std::cout << "you entered:" << number << std::endl;
    // return 0;

    // std::vector<int> numbers = {10,15,20};
    // std::cout << "List of numbers:\n";
    // for (int num : numbers){
    //     std::cout <<num << std::endl;
    // }

    // std::cout << "pleast pick a number (1,2 or 3):";

    // int choice;
    // std::cin >> choice;

    // switch(choice){
    //     case 1:
    //         std::cout <<"you picked " <<numbers[0] <<".\n";
    //         break;
    //     case 2:
    //         std::cout << "you picked " <<numbers[1] <<".\n";
    //         break;
    //     case 3:
    //         std::cout << "you picked " <<numbers[2] <<".\n";
    //         break;
    //     default:
    //         std::cout << "Invalid choice. Please pick 1, 2, or 3. \n";
    //         break;
    // }    
    // return 0;
    string Card1 = "Yo mama";
    string Card2 = "Amongussy";
    string Card3 = "Sussy";
    string Ucard1 = "baka";
    string Ucard2 = "Fishy";
    string Ucard3 = "whatever";
    string Ans1 = "yes";
    string Ans2 = "yes";
    string Ans3 = "no";

    std::vector<string> Choices ={"Discarded Pile", "Unanswered Pile"};
    //card Pile
    std::vector<string> Dcard ={Card1,Card2,Card3};
    std::vector<string> Uncard ={Ucard1,Ucard2,Ucard3};
    std::vector<string> DPile ={Ans1,Ans2,Ans3};
    std::cout << "please pick a choice Discarded or Unanswered.\n";


    string Choices1,UChoices;
    int DChoices;
    string DAnswer,UAnswer;
    std::cin >> Choices1;
    std::cout << Choices1;
    if(Choices1 == "Discarded"){
        std::cout << "These are the items:\n";
        for (const std::string& choice : Dcard) {//string& makes a reference that directly effects the orginal variable and const ensures it doesn't get changed.
            std::cout << choice << std::endl;
        }

        std::cout << "which one? :";
        std::cin >> DChoices;
        DChoices --;

        if (DChoices >= 0 && DChoices < Dcard.size()) {
            std::cout << "Answer: ";
            std::cin >> DAnswer;
            // Answer is stored in order of the card so picking 1 gives first answer.
            if (DAnswer == DPile[DChoices]) {
                std::cout << "Correct Answer." <<std ::endl;
            } else {
                std::cout << "Incorrect answer." << std::endl;
            }
        } else {
            std::cout << "Invalid choice." << std::endl;
        }


    } else if (Choices1 == "Unanswered") { // Needs to be updated.
        std::cout << "This is your card:\n";
        
    } else {
        std::cout<< "type better stupid";
    }
    return 0;
}