#include <cassert>
#include <iomanip>
#include <iostream>

#include "project4.hpp"
#include "History.hpp"
#include "Transaction.hpp"

////////////////////////////////////////////////////////////////////////////////
// Definitions for Transaction class
////////////////////////////////////////////////////////////////////////////////
//
//

// Constructor
// TODO
Transaction::Transaction( std::string ticker_symbol,  unsigned int day_date,  
    unsigned int month_date,  unsigned year_date, 
    bool buy_sell_trans,  unsigned int number_shares,  
    double trans_amount ):

    symbol {ticker_symbol},
    day {day_date},
    month {month_date},
    year  {year_date},
    trans_type{},
    shares {number_shares},
    amount {trans_amount},
    trans_id {assigned_trans_id}
 
    {
      acb = 0.0;
      acb_per_share = 0.0;
      share_balance = 0;
      cgl = 0.0;
      p_next = nullptr;

      if (buy_sell_trans == true){
        trans_type = "Buy";
      } else {
        trans_type = "Sell";
      }
      ++assigned_trans_id;
    }


// Destructor
// TODO
Transaction::~Transaction()
{

}

// Overloaded < operator.
// TODO
bool Transaction::operator<( Transaction const &other )
{

  if (get_year() < other.get_year() ){
    return true;
  } else if (get_year() > other.get_year()) {
    return false;

  } else if (get_month() < other.get_month()){
    return true;
  } else if (get_month() > other.get_month()) {
    return false;

  } else if (get_day() < other.get_day()){
    return true;
  } else if (get_day() > other.get_day()){
    return false;

  } else if (get_trans_id() < other.get_trans_id()){
    return true;
  } else {
    return false;
  }

}

// GIVEN
// Member functions to get values.
//
std::string Transaction::get_symbol() const { return symbol; }
unsigned int Transaction::get_day() const { return day; }
unsigned int Transaction::get_month() const { return month; }
unsigned int Transaction::get_year() const { return year; }
unsigned int Transaction::get_shares() const { return shares; }
double Transaction::get_amount() const { return amount; }
double Transaction::get_acb() const { return acb; }
double Transaction::get_acb_per_share() const { return acb_per_share; }
unsigned int Transaction::get_share_balance() const { return share_balance; }
double Transaction::get_cgl() const { return cgl; }
bool Transaction::get_trans_type() const { return (trans_type == "Buy") ? true: false ; }
unsigned int Transaction::get_trans_id() const { return trans_id; }
Transaction *Transaction::get_next() { return p_next; }

// GIVEN
// Member functions to set values.
//
void Transaction::set_acb( double acb_value ) { acb = acb_value; }
void Transaction::set_acb_per_share( double acb_share_value ) { acb_per_share = acb_share_value; }
void Transaction::set_share_balance( unsigned int bal ) { share_balance = bal ; }
void Transaction::set_cgl( double value ) { cgl = value; }
void Transaction::set_next( Transaction *p_new_next ) { p_next = p_new_next; }

// GIVEN
// Print the transaction.
//
void Transaction::print() {
  std::cout << std::fixed << std::setprecision(2);
  std::cout << std::setw(4) << get_trans_id() << " "
    << std::setw(4) << get_symbol() << " "
    << std::setw(4) << get_day() << " "
    << std::setw(4) << get_month() << " "
    << std::setw(4) << get_year() << " ";


  if ( get_trans_type() ) {
    std::cout << "  Buy  ";
  } else { std::cout << "  Sell "; }

  std::cout << std::setw(4) << get_shares() << " "
    << std::setw(10) << get_amount() << " "
    << std::setw(10) << get_acb() << " " << std::setw(4) << get_share_balance() << " "
    << std::setw(10) << std::setprecision(3) << get_acb_per_share() << " "
    << std::setw(10) << std::setprecision(3) << get_cgl()
    << std::endl;
}


////////////////////////////////////////////////////////////////////////////////
// Definitions for the History class
////////////////////////////////////////////////////////////////////////////////
//
//


// Constructor
// TODO 2nd sub
History::History()
{
  p_head = nullptr;
}
// Destructor
// TODO 2nd sub
History::~History()
{
  Transaction *hist_destr{get_p_head()};
  if (hist_destr != nullptr){

    while (get_p_head() != nullptr){
      
      hist_destr = get_p_head();
      p_head = get_p_head()->get_next();
      delete hist_destr;
      hist_destr = nullptr;
    }
  
  }
}
// insert(...): Insert transaction into linked list.
// TODO 2nd sub
void History::insert(Transaction *p_new_trans)
{

  Transaction *insert{get_p_head()};

  if (get_p_head() != nullptr){
    
    while (insert->get_next() != nullptr){
        insert = insert->get_next();
      }

      if (insert->get_next() == nullptr){
        insert->set_next(p_new_trans);
      }

  } else {
    p_head = p_new_trans;
  }
 
  return;
}
// read_history(...): Read the transaction history from file.
// TODO 2nd sub
void History::read_history()
{
  Transaction *read_hist;
    // Goes through list
  ece150::open_file();
  while (ece150::next_trans_entry() == true){

    read_hist = new Transaction( ece150::get_trans_symbol(), ece150::get_trans_day(), ece150::get_trans_month(),
                                ece150::get_trans_year(), ece150::get_trans_type(), ece150::get_trans_shares(),
                                ece150::get_trans_amount() );
    insert(read_hist);
  }
  ece150::close_file();

  return;
}
// print() Print the transaction history.
// TODO 2nd sub
void History::print()
{
  Transaction *cout{get_p_head()};
  std::cout<<"========== BEGIN TRANSACTION HISTORY ============"<<std::endl;
  
  while (cout != nullptr){
    cout->print();
    cout = cout->get_next();
  }
  
  std::cout<<"========== END TRANSACTION HISTORY ============"<<std::endl;
  return;
}
// sort_by_date(): Sort the linked list by trade date.
// TODO
void History::sort_by_date()
{

  // Selection Sort

  Transaction *big{p_head};
  Transaction *before{p_head}; // Location before big in the list
  Transaction *search{p_head};
  Transaction *temp_list{nullptr};
 
  while (p_head != nullptr){

    while (search != nullptr){
      if (*big < *search){
        big = search;
      }
      search = search->get_next();
    }

    if (p_head->get_next() == nullptr){ // One node
 
      p_head = nullptr;
      big->set_next(temp_list);
      temp_list = big;

    } else if (p_head == big){ // If big is at the start of the list
      p_head = (big->get_next());
      big->set_next(nullptr);
            
      if (temp_list == nullptr){
        temp_list = big;
        big->set_next(nullptr);

      } else {
        big->set_next(temp_list);
        temp_list = big;

      }

    } else { // 2+ nodes & p_head != big
      while (before->get_next() != big){
        before = before->get_next();
      }

      before->set_next(big->get_next());
      big->set_next(nullptr);
 
      if (temp_list == nullptr){
        temp_list = big;
        big->set_next(nullptr);

      } else {
        big->set_next(temp_list);
        temp_list = big;
      }
    }
    search = p_head;
    before = p_head;
    big = p_head;
  }
  p_head = temp_list;


  return;
}

// update_acb_cgl(): Updates the ACB and CGL values.
// TODO
void History::update_acb_cgl()
{
  Transaction *money{p_head};
  double total_money{0};
  int total_shares{0};
  double total_acb_per_share{0};

  while (money != nullptr){
    
    if (money->get_trans_type() == true){ // buy
      money->set_acb((money->get_amount())+total_money);
      money->set_share_balance((money->get_shares())+total_shares);
      money->set_acb_per_share((money->get_acb())/(money->get_share_balance()));

      total_money = money->get_acb();
      total_shares = money->get_share_balance();
      total_acb_per_share = money->get_acb_per_share();
    } else { // sell
      money->set_acb(total_money - (money->get_shares()*(total_acb_per_share)));
      money->set_share_balance(total_shares - (money->get_shares()));
      money->set_acb_per_share(total_acb_per_share);
      money->set_cgl((money->get_amount()) - ((money->get_shares())*(money->get_acb_per_share())));

      total_money = money->get_acb();
      total_shares = money->get_share_balance();
    }

    money = money->get_next();
  }


  return;
}
// compute_cgl(): )Compute the ACB, and CGL.
// TODO
double History::compute_cgl(unsigned int year)
{
Transaction *cgl{p_head};
double tot_cgl{0};

while (cgl != nullptr){

  if (cgl->get_trans_type() != true){ // sell
    if (cgl->get_year() == year){
      tot_cgl = ((cgl->get_cgl())+tot_cgl);
    } 

  }
  cgl = cgl->get_next();
}
  return tot_cgl;
}
// GIVEN
// get_p_head(): Full access to the linked list.
//
Transaction *History::get_p_head() { return p_head; }
