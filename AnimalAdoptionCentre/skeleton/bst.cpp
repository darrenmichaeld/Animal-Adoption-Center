#include "bst.h"
using namespace std;

/**
 * Returns true if "src" contains *any* words in "filterWords" (case-insensitive).
 * e.g.: 
 *   containAnyWords("Example string", "example") == true
 *   containAnyWords("Example string", "amp") == true
 *   containAnyWords("Example string", "this is a long sentence") == true  [contains 'a']
 *   containAnyWords("Example string", "no occurence") == false
*/
bool containAnyWords(string src, string filterWords) {
    auto strToLower = [](string& str) {
        for (char& c: str) c = tolower(c);
    };
    strToLower(src);
    strToLower(filterWords);
    size_t found = filterWords.find(' ');
    while (found != string::npos) {
        if (src.find(filterWords.substr(0, found)) != string::npos)
            return true;
        filterWords = filterWords.substr(found+1);
        found = filterWords.find(' ');
    }
    return src.find(filterWords) != string::npos;
}


// TASK 2.1: Filter::match(const Animal&) const
// Returns true if the animal satisfies ALL 3 conditions:
// - species (including breed) contains *any* of the words in speciesMatch
// - health description contains *any* of the words in healthMatch
// - vaccine status contains *all* of the non-empty strings in vaccineMatch
bool Filter::match(const Animal& a) const
{
    // TODO
    bool species = containAnyWords(a.getSpecies(),this->speciesFilter);
    bool healthCondition = containAnyWords(a.getHealthCondition().description, this->healthFilter);
    bool vaccine = true;
    VaccinationStatus vaccineCheck = a.getVaccinationStatus();
    for(int i = 0; i < VACCINE_TABLE_SIZE; i++){
        if(!(a.getVaccinationStatus().hasVaccine(vaccineFilter[i]))){
                vaccine = false;
        }
    }
    
    if(species && vaccine && healthCondition){
        return true;
    }
    else{
        return false;
    }
}

// TASK 2.2: AnimalLLnode::print(unsigned int&, unsigned int&, const Filter&) const
// Print the animals in this linked list if it matches with the filter.
// The linked list should be maintained such that it is in decreasing ID order.
//
// E.g. (each node is shown as ID[Name]):
//      7[Cat] -> 6[Cat] -> 5[Dog] -> 3[Cat] -> 1[Bird] -> 0[Cat]
// should be printed in the following order using Filter{Name = "Cat"}: 
//      0[Cat]
//      3[Cat]
//      6[Cat]
//      7[Cat]
//
// The parameters ignoreCount and displayCount should be passed to the animal's display() function
void AnimalLLnode::print(unsigned int& ignoreCount, unsigned int& displayCount, const Filter& filter) const
{
    // TODO
    if(next != nullptr){
        next->print(ignoreCount, displayCount, filter);
    }
    if(filter.match(*animal)){
        animal->display(ignoreCount, displayCount);
    }

}

// TASK 2.3: BSTnode destructor
BSTnode::~BSTnode()  {
    // TODO
    while(head != nullptr){
        AnimalLLnode* curr = head;
        head = head->next;
        delete curr;
    }
}

// TASK 2.4: BSTnode::addAnimal(const Animal* a)
// Add an animal to the linked list.
// Ensure the order is *decreasing ID*, e.g.: 7[Cat] -> 6[Cat] -> 5[Dog] -> 3[Cat] -> 1[Bird] -> 0[Cat]
// You may assume no two animals with the same ID will be added to a node
// (this also means you should not add the same animal to a node twice!)
void BSTnode::addAnimal(const Animal* a) {
    // TODO
    if(head == nullptr){
        head = new AnimalLLnode(a, nullptr);
        return;
    }
    AnimalLLnode *curr = head;
    AnimalLLnode *back = head;
    if(a->getID()>head->animal->getID()){
        head = new AnimalLLnode(a,curr);
        return;
    }

    for(;curr != nullptr; curr = curr->next){
        if(back->animal->getID() > a->getID() && curr->animal->getID() < a->getID())
            {
                back->next = new AnimalLLnode(a, curr);
                return;
            }
        back = curr;
    }
    back->next = new AnimalLLnode(a,curr);
    return;
}

// TASK 2.5: BSTnode::addAnimal(const Animal* a)
// Remove an animal from the linked list.
// Ensure the order of the other animals are kept.
// If the animal does not exist, do nothing.
// If there are no animals left after removing, set head to nullptr.
void BSTnode::removeAnimal(const Animal* a) {
    
    // TODO
    AnimalLLnode *curr = this->head;
    AnimalLLnode *back = nullptr;
    while((curr != nullptr) && (curr->animal != a)){
        back = curr;
        curr = curr->next;
    }
    if(curr != nullptr)
    {
        if (curr == head){
            head = head->next;
        }
        else{
            back->next = curr->next;
        }
        delete curr;
        curr = nullptr;
    }
}


// TASK 3.1: BST destructor
BST::~BST() {
    
    // TODO
    delete root;
}

// TASK 3.2: BST::findMinNode()
// Optional task, but may be needed for BST::remove().
// Return a reference to the BSTnode* of the min node in this BST.
BSTnode*& BST::findMinNode()
{
    // TODO
    if(this->root->left.isEmpty() && this->root->right.isEmpty()){
        return this->root;
    }

    BST* node = this;
    while (node->root->left.isEmpty() == false){
        node = &(node->root->left);
    }
    return node->root;
}

// TASK 3.3: BST::insert(const Animal* a)
// Insert an animal 'a' to the BST.
// Use the comparator "data member function" to compare the animal with the current node:
// - If 'a' is "less than" the current node, insert it to the left subtree.
// - If 'a' is "more than" the current node, insert it to the right subtree.
// - If 'a' "equals" the current node, insert it into this node's linked list.
// - Otherwise, if the node is empty, create a new node using 'a'.
void BST::insert(const Animal* a)
{
    // TODO
    if(this->isEmpty()){
        this->root = new BSTnode(a, comparator);
        return;
    }
    else if(comparator(a, this->root->head->animal) < 0){
         this->root->left.insert(a);
    }
    else if(comparator(a, this->root->head->animal) > 0){
         this->root->right.insert(a);
    }
    else if (comparator(a, this->root->head->animal) == 0){
        this->root->addAnimal(a);
    }
    return;
}

// TASK 3.4: BST::remove(const Animal* a)
// Remove an animal 'a' from the BST
// Follow the same steps in BST::insert() to locate the node to remove.
// Removal strategy is similar to lecture notes example with a few differences:
// - If the node is not found, do nothing.
// - If the node is found, remove the animal from its linked list. If it makes the linked list become empty,
//   remove the node:
//   + If the node contains 0 or 1 child, move the children node to current root, and deallocate the old root.
//   + Else, *move* the linked list from the right subtree's min node to current root, and deallocate right subtree's min node.
void BST::remove(const Animal* a)
{
    // TODO
    if(this->isEmpty())
    {
        return;
    }
    
    if(comparator(a, this->root->head->animal) < 0){
         this->root->left.remove(a);
    }
    else if(comparator(a, this->root->head->animal) > 0){
         this->root->right.remove(a);
    }
    else if (comparator(a, this->root->head->animal) == 0){
        this->root->removeAnimal(a);
        if(this->root->head == nullptr){
            if(!(this->root->left.isEmpty()) && !(this->root->right.isEmpty())){
                BSTnode* minNode = this->root->right.findMinNode();
                const Animal* animalTemp = minNode->head->animal;
                AnimalLLnode* temp = minNode->head->next;
                minNode->head->next = nullptr;
                this->root->right.remove(minNode->head->animal);
                this->root->head = new AnimalLLnode(animalTemp, temp);
            
            }
            else{
                BSTnode* rootDelete = this->root;
                this->root = (this->root->left.isEmpty()) ? this->root->right.root : this->root->left.root;
                rootDelete->left.root = rootDelete->right.root = nullptr;
                delete rootDelete;
            }
        }
        else{
                return;
            }
    }
}


// TASK 3.5: BST::print(unsigned int&, unsigned int&, const Filter&) const
// Print the BST using in-order traversal.
//
// E.g.: Consider a BST containing animals sorted by species name:
//                      8[Reptile, Healthy] -> 6[Reptile, Bad]
//              5[Rabbit, Critical]
// 7[Dog, Healthy] -> 3[Dog, Healthy]
//                      9[Cat, Bad] -> 4[Cat, Healthy] -> 2[Cat, Very poor]
//              1[Bird, Healthy]
// 
// should print in the following order using Filter{Health = "Healthy"}:
//      1[Bird, Healthy]
//      4[Cat, Healthy]
//      3[Dog, Healthy]
//      7[Dog, Healthy]
//      8[Reptile, Healthy]
void BST::print(unsigned int& ignoreCount, unsigned int& displayCount, const Filter& filter) const
{
    // TOD
    if(!(this->isEmpty())){
        this->root->left.print(ignoreCount, displayCount, filter);
        this->root->head->print(ignoreCount, displayCount, filter);
        this->root->right.print(ignoreCount, displayCount, filter);
    }
}
