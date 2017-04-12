#include "MinerOwnedStates.h"
#include "State.h"
#include "Miner.h"
#include "Locations.h"
#include "ConsoleUtils.h"
#include "EntityNames.h"

#include <iostream>
using std::cout;

//define this to output to a file
#ifdef TEXTOUTPUT
#include <fstream>
extern std::ofstream os;
#define cout os
#endif





//--------------------------------------methods for EnterMineAndDigForNugget

EnterMineAndDigForNugget* EnterMineAndDigForNugget::Instance()
{
  static EnterMineAndDigForNugget instance;

  return &instance;
}

void EnterMineAndDigForNugget::Enter(Miner* pMiner)
{
  //if the miner is not already located at the goldmine, he must
  //change location to the gold mine
  if (pMiner->Location() != goldmine)
  {
    SetTextColor(FOREGROUND_RED| FOREGROUND_INTENSITY);
    cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "�������� �ɾ";

    pMiner->ChangeLocation(goldmine);
  }
}


void EnterMineAndDigForNugget::Execute(Miner* pMiner)
{  
  //the miner digs for gold until he is carrying in excess of MaxNuggets. 
  //If he gets thirsty during his digging he packs up work for a while and 
  //changes state to go to the saloon for a whiskey.
  pMiner->AddToGoldCarried(1);

  pMiner->IncreaseFatigue();

  SetTextColor(FOREGROUND_RED| FOREGROUND_INTENSITY);
  cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "�ݵ�� �ϳ��� ä��";

  //if enough gold mined, go and put it in the bank
  if (pMiner->PocketsFull())
  {
    pMiner->ChangeState(VisitBankAndDepositGold::Instance());
  }

  if (pMiner->Thirsty())
  {
    pMiner->ChangeState(QuenchThirst::Instance());
  }
}


void EnterMineAndDigForNugget::Exit(Miner* pMiner)
{
  SetTextColor(FOREGROUND_RED| FOREGROUND_INTENSITY);
  cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " 
       << "���� ���� ������ ������ ����";
}



//----------------------------------------methods for VisitBankAndDepositGold

VisitBankAndDepositGold* VisitBankAndDepositGold::Instance()
{
  static VisitBankAndDepositGold instance;

  return &instance;
}


void VisitBankAndDepositGold::Enter(Miner* pMiner)
{  
  //on entry the miner makes sure he is located at the bank
  if (pMiner->Location() != bank)
  {
    SetTextColor(FOREGROUND_RED| FOREGROUND_INTENSITY);
    cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "�������� �̵�";

    pMiner->ChangeLocation(bank);
  }
}


void VisitBankAndDepositGold::Execute(Miner* pMiner)
{

  //deposit the gold
  pMiner->AddToWealth(pMiner->GoldCarried());
    
  pMiner->SetGoldCarried(0);

  SetTextColor(FOREGROUND_RED| FOREGROUND_INTENSITY);
  cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " 
       << "���� ����. ���� �ܰ�: "<< pMiner->Wealth();

  //wealthy enough to have a well earned rest?
  if (pMiner->Wealth() >= ComfortLevel)
  {
    SetTextColor(FOREGROUND_RED| FOREGROUND_INTENSITY);
    cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " 
         << "\"���! ���� �� ���ڴ�. ���� �ִ� �Ƴ����Է� ���ư�����.\"";
      
    pMiner->ChangeState(GoHomeAndSleepTilRested::Instance());      
  }

  //otherwise get more gold
  else 
  {
    pMiner->ChangeState(EnterMineAndDigForNugget::Instance());
  }
}


void VisitBankAndDepositGold::Exit(Miner* pMiner)
{
  SetTextColor(FOREGROUND_RED| FOREGROUND_INTENSITY);
  cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "������ ����";
}


//----------------------------------------methods for GoHomeAndSleepTilRested

GoHomeAndSleepTilRested* GoHomeAndSleepTilRested::Instance()
{
  static GoHomeAndSleepTilRested instance;

  return &instance;
}

void GoHomeAndSleepTilRested::Enter(Miner* pMiner)
{
  if (pMiner->Location() != shack)
  {
    SetTextColor(FOREGROUND_RED| FOREGROUND_INTENSITY);
    cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "������ �ɾ";

    pMiner->ChangeLocation(shack); 
  }
}

void GoHomeAndSleepTilRested::Execute(Miner* pMiner)
{ 
  //if miner is not fatigued start to dig for nuggets again.
  if (!pMiner->Fatigued())
  {
    SetTextColor(FOREGROUND_RED| FOREGROUND_INTENSITY); 
    cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " 
          << "\"���� �þ����� ���! �ٽ� ���� ĳ�� ���߰ڱ�.\"";

     pMiner->ChangeState(EnterMineAndDigForNugget::Instance());
  }

  else 
  {
    //sleep
    pMiner->DecreaseFatigue();

    SetTextColor(FOREGROUND_RED| FOREGROUND_INTENSITY);
    cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "������... ";
  } 
}

void GoHomeAndSleepTilRested::Exit(Miner* pMiner)
{ 
  SetTextColor(FOREGROUND_RED| FOREGROUND_INTENSITY);
  cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "���� ����";
}




//------------------------------------------------methods for QuenchThirst

QuenchThirst* QuenchThirst::Instance()
{
  static QuenchThirst instance;

  return &instance;
}

void QuenchThirst::Enter(Miner* pMiner)
{
  if (pMiner->Location() != saloon)
  {    
    pMiner->ChangeLocation(saloon);

    SetTextColor(FOREGROUND_RED| FOREGROUND_INTENSITY);
    cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "\"���� ������!\" �������� �ɾ";
  }
}

void QuenchThirst::Execute(Miner* pMiner)
{
   if (pMiner->Thirsty())
   {
     pMiner->BuyAndDrinkAWhiskey();

     SetTextColor(FOREGROUND_RED| FOREGROUND_INTENSITY);
     cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "���� ����";

     pMiner->ChangeState(EnterMineAndDigForNugget::Instance());
  }

  else 
  {
    SetTextColor(FOREGROUND_RED| FOREGROUND_INTENSITY);
    cout << "\nERROR!\nERROR!\nERROR!";
  } 
}

void QuenchThirst::Exit(Miner* pMiner)
{ 
  SetTextColor(FOREGROUND_RED| FOREGROUND_INTENSITY);
  cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "������ ����";
}

