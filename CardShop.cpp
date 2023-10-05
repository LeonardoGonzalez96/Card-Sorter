#include "CardShop.hpp"
#include "Node.hpp"
#include <iostream>
#include <fstream>
#include <sstream>


/**
  @pre  : the input file is in csv format: "name,type,level_rank,race,attribute,atk,def"
  @param: the name of the input file
  @post:  Reads the input file and adds pointers to YGOCard objects 
          instantiated with the information read from the file. 
    */
CardShop::CardShop(std::string input_file_name) 
{
  std::ifstream fin(input_file_name);
  if (fin.fail())
  {
    std::cerr << "File cannot be opened for reading. \n";
    exit(1); // exit if failed to open the file
  }
  //we don't use the first line
  std::string ignore;
  getline(fin, ignore);

  //the columns of the csv associated with the data members
  std::string name, type, race, attribute;
  int level_rank, atk, def;

  std::string temp_string;
  int temp_int;
  int item_counter = 0;
  while (getline(fin, temp_string, ','))
  {
    name = temp_string;

    getline(fin, type, ',');

    getline(fin, temp_string, ',');
    std::istringstream(temp_string) >> temp_int;
    level_rank = temp_int;

    getline(fin, race, ',');

    getline(fin, attribute, ',');

    getline(fin, temp_string, ',');
    std::istringstream(temp_string) >> temp_int;
    atk = temp_int;

    getline(fin, temp_string);
    std::istringstream(temp_string) >> temp_int;
    def = temp_int;

    //create new card
    YGOCard *card_ptr = new YGOCard(name, type, level_rank, race, attribute, atk, def);

    //add to linked list and increment counter
    insert(item_counter++, card_ptr);
  }
}

//@post: removes all cards from the shop
//Deallocate and delete items
void CardShop::clear()
{
  for (int i = 0; i < item_count_; i++)
  {
    YGOCard *ptr = getItem(i);
    delete ptr;
    ptr = nullptr;
  }
  DoublyLinkedList::clear();
}

//destructor
CardShop::~CardShop()
{
  clear();
}

//@post: displays information of all cards in the shop, one per line
void CardShop::display()
{
  for (int i = 0; i < item_count_; i++)
  {
    getItem(i)->display();
  }
}

  //@param: the index of the first card to be displayed
  //@param: the index of the last card to be displayed
  //@post: displays the names of all cards in the shop with startRange and endRange, 
  //       inclusive, one per line
void CardShop::displayName(int startRange, int endRange)
{
  for (int i = startRange; i <= endRange; i++)
  {
    getItem(i)->displayName();
    if (i != endRange)
    {
      std::cout << ", ";
    }
    else
    {
      std::cout << std::endl;
    }
  }
}


//@return:  true if all the cards in rhs are equal to the cards in the shop, false otherwise
bool CardShop::operator==(const CardShop &rhs) const
{
  if (getLength() != rhs.getLength())
  {
    return false;
  }
  for (int i = 0; i < getLength(); i++)
  {
    if (*(getPointerTo(i)->getItem()) == *(rhs.getPointerTo(i)->getItem()))
    {
    }
    else
    {
      return false;
    }
  }
  return true;
}

//@param: the comparator to show descending or ascending order
//@param: the key which is either atk or def 
//@post: sorts the cards in the order that corresponds with the parameters using a bubbleSort algorithm
//@return: returns the number of times it uses the swap function
template<typename Comparator>
int CardShop::bubbleSort(Comparator comp, std::string key){
  int size = item_count_;
  bool swapped = true;
  int pass = 1;
  int counter = 0;
  while(swapped && (pass < size) ){
    swapped = false;
    for(int i = 0; i < size - pass; i++){
      if(key == "atk" && (!(comp(getItem(i)->getAtk(), getItem(i + 1)->getAtk()))) && (getItem(i)->getAtk() != getItem(i + 1)->getAtk())){
        swap(i, i + 1);
        swapped = true;
        counter++;
      }
      else if(key == "def" && (!(comp(getItem(i)->getDef(), getItem(i + 1)->getDef()))) && (getItem(i)->getDef() != getItem(i + 1)->getDef())){
        swap(i, i + 1);
        swapped = true;
        counter++;
      }
    }
    pass++;

  } //end while
  return counter;
}

//@param: the comparator to show descending or ascending order
//@param: the key which is either atk or def 
//@post: sorts the cards in the order that corresponds with the parameters using a insertionSort algorithm
//@return: returns the number of times it uses the swap function
template<typename Comparator>
int CardShop::insertionSort(Comparator comp, std::string key){
  int size = item_count_;
  int counter = 0;
  for(int unsorted = 1; unsorted < size; unsorted++){
    int current = unsorted;
    if(key == "atk"){
      while((current > 0) && !(comp(getItem(current - 1)->getAtk(), getItem(current)->getAtk())) && (getItem(current - 1)->getAtk() != getItem(current)->getAtk())){
        swap(current, current - 1);
        current--;
        counter++;
      }
    }
    else if(key == "def"){
      while((current > 0) && !(comp(getItem(current - 1)->getDef(), getItem(current)->getDef())) && (getItem(current - 1)->getDef() != getItem(current)->getDef())){
        swap(current, current - 1);
        current--;
        counter++;
      }
    }
  }
  return counter;
}

//@param: the comparator to show descending or ascending order
//@param: the key which is either atk or def 
//@param: the starting index
//@param: the ending index
//@param: the counter that keeps track of the swaps
//@post: sorts from the start index to the end index then adds to counter for each swap done
template<typename Comparator>
int CardShop::partitionSort(Comparator comp, std::string key, int start, int end, int& counter){
  int i = start - 1;
  if(key == "atk"){
    int pivot = getItem(end)->getAtk();
    for(int j = start; j < end; j++){
      if(comp(getItem(j)->getAtk(), pivot)){
        i++;
        swap(i, j);
        counter++;
      }
    }
  } 
  else if(key == "def"){
    int pivot = getItem(end)->getDef();
    for(int j = start; j < end; j++){
      if(comp(getItem(j)->getDef(), pivot)){
        i++;
        swap(i, j);
        counter++;
      }
    }
  } 
  swap(i + 1, end);
  counter++;
  return i + 1;
}
//@param: the comparator to show descending or ascending order
//@param: the key which is either atk or def 
//@param: the starting index
//@param: the ending index
//@param: the counter that keeps track of the swaps
//@post: gets a pivot then sends the parameters to the partitionSort function to sort the left and right side of the pivot
template<typename Comparator>
void CardShop::partition(Comparator comp, std::string key, int start, int end, int& counter){
  if(start >= end){
    return;
  }
  int index = partitionSort(comp, key, start, end, counter);
  partition(comp, key, start, index - 1, counter);
  partition(comp, key, index + 1, end, counter);
}
//@param: the comparator to show descending or ascending order
//@param: the key which is either atk or def 
//@post: sorts the cards in the order that corresponds with the parameters using a quickSort algorithm
//@return: returns the number of times it uses the swap function
template<typename Comparator>
int CardShop::quickSort(Comparator comp, std::string key){
  int counter = 0;
  partition(comp, key, 0, item_count_ - 1, counter);
  return counter;
}

// didn't implement this
template<typename Comparator>
int CardShop::mergeSort(Comparator comp, std::string key){
  return 1;
}
